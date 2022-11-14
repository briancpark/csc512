/*
 *  Copyright (c) 2020-2021 Xuhpclab. All rights reserved.
 *  Licensed under the MIT License.
 *  See LICENSE file for more information.
 */

#include <cstddef>

#include "dr_api.h"
#include "drmgr.h"
#include "drreg.h"
#include "drutil.h"
#include "drcctlib.h"
#include "hashtable.h"
#include "instruction.h"
#include "memory_cache.h"
#include "profiler.h"
#include "ctxt_container.h"

#define DRCCTLIB_PRINTF(_FORMAT, _ARGS...) \
    DRCCTLIB_PRINTF_TEMPLATE("integer_overflow_core", _FORMAT, ##_ARGS)
#define DRCCTLIB_EXIT_PROCESS(_FORMAT, _ARGS...) \
    DRCCTLIB_CLIENT_EXIT_PROCESS_TEMPLATE("integer_overflow_core", _FORMAT, ##_ARGS)

using namespace runtime_profiling;

static int tls_idx;

enum {
    INSTRACE_TLS_OFFS_BUF_PTR,
    INSTRACE_TLS_COUNT, /* total number of TLS slots allocated */
};

static reg_id_t tls_seg;
static uint tls_offs;
#define TLS_SLOT(tls_base, enum_val) (void **)((byte *)(tls_base) + tls_offs + (enum_val))
#define BUF_PTR(tls_base, type, offs) *(type **)TLS_SLOT(tls_base, offs)
#define MINSERT instrlist_meta_preinsert

#ifdef ARM_CCTLIB
#    define OPND_CREATE_CCT_INT OPND_CREATE_INT
#else
#    ifdef CCTLIB_64
#        define OPND_CREATE_CCT_INT OPND_CREATE_INT64
#    else
#        define OPND_CREATE_CCT_INT OPND_CREATE_INT32
#    endif
#endif

#ifdef CCTLIB_64
#    define OPND_CREATE_CTXT_HNDL_MEM OPND_CREATE_MEM64
#else
#    define OPND_CREATE_CTXT_HNDL_MEM OPND_CREATE_MEM32
#endif

#define OPND_CREATE_MEM_IDX_MEM OPND_CREATE_MEM64

typedef struct _instr_shadow_t {
    int32_t key;
    app_pc ip;
    uint opcode;
    byte num_srcs;
    byte num_dsts;
    Operand* srcs;
    Operand* dsts;
} instr_shadow_t;

// THREAD_SHARED_MEMORY(TSM) (instr_shadow_t)
#define TSM_CACHE_PAGE1_BIT 15  // max support 2^31
#define TSM_CACHE_PAGE2_BIT 16 // 65536
#define INSTR_PC_TABLE_HASH_BITS 10
static hashtable_t global_instr_pc_table;
thread_shared_memory_cache_t<instr_shadow_t> *global_instr_shadow_cache;
static void *instr_shadow_lock;
static void *instr_shadow_cache_lock;
static CtxtContainer *global_ctxt_container;
static void *ctxt_container_lock;


#define MAX_NUM_MODULES 0x1000
typedef struct _module_array_t {
    app_pc base;
    app_pc end;
} module_array_t;
/* the last slot is where all non-module address go */
static module_array_t global_mod_array[MAX_NUM_MODULES];
static int global_loaded_module_num = 0;

static bool
ModuleFilter(uint64_t pc) {
    for (int i = 0; i < global_loaded_module_num; i ++) {
        if (pc <= (uint64_t)(ptr_int_t)global_mod_array[i].end && pc >= (uint64_t)(ptr_int_t)global_mod_array[i].base) {
            return false;
        }
    }
    return true;
}

static inline void
instr_shadow_create(instr_shadow_t *instr_shadow, int32_t index)
{
    instr_shadow->key = index;
    instr_shadow->opcode = 0;
    instr_shadow->num_dsts = 0;
    instr_shadow->num_srcs = 0;
    instr_shadow->srcs = NULL;
    instr_shadow->dsts = NULL;
}

static inline void
instr_shadow_init(instr_shadow_t *instr_shadow, app_pc ip, int num_srcs, int num_dsts)
{
    instr_shadow->ip = ip;
    instr_shadow->num_srcs = num_srcs;
    instr_shadow->num_dsts = num_dsts;
    instr_shadow->srcs = (Operand *)dr_raw_mem_alloc(
        (instr_shadow->num_srcs + instr_shadow->num_dsts) * sizeof(Operand),
        DR_MEMPROT_READ | DR_MEMPROT_WRITE, NULL);
    instr_shadow->dsts = instr_shadow->srcs + instr_shadow->num_srcs;
}

static inline void
instr_shadow_free_cache(instr_shadow_t *instr_shadow)
{
    if (instr_shadow->num_dsts <= 0 && instr_shadow->num_srcs <= 0) {
        return;
    }
    if (instr_shadow->srcs == NULL) {
        return;
    }
    dr_raw_mem_free(instr_shadow->srcs,
                    (instr_shadow->num_dsts + instr_shadow->num_srcs) *
                        sizeof(Operand));
}


static inline uint8_t
get_reg_size(opnd_t ref) {
    uint8_t size = 0;
    opnd_size_t op_size = reg_get_size(opnd_get_reg(ref));
    switch (op_size)
    {
    case OPSZ_8:
        /* code */
        size = 8;
        break;
    case OPSZ_4:
        /* code */
        size = 4;
        break;
    case OPSZ_2:
        /* code */
        size = 2;
        break;
    case OPSZ_1:
        /* code */
        size = 1;
        break;
    default:
        break;
    }
    return size;
}

static inline uint64_t
get_reg_value(dr_mcontext_t *mcontext, RegisterType type, int size)
{
    uint64_t value = 0;
    int realType = (type - 1) % 16 + 1;
    switch (realType)
    {
    case RegisterType::kRAX:
        value = mcontext->xax;
        break;
    case RegisterType::kRCX:
        /* code */
        value = mcontext->xcx;
        break;
    case RegisterType::kRDX:
        value = mcontext->xdx;
        break;
    case RegisterType::kRBX:
        value = mcontext->xbx;
        break;
    case RegisterType::kRSP:
        value = mcontext->xsp;
        break;
    case RegisterType::kRBP:
        value = mcontext->xbp;
        break;
    case RegisterType::kRSI:
        value = mcontext->xsi;
        break;
    case RegisterType::kRDI:
        value = mcontext->xdi;
        break;
    case RegisterType::kR8:
        value = mcontext->r8;
        break;
    case RegisterType::kR9:
        value = mcontext->r9;
        break;
    case RegisterType::kR10:
        value = mcontext->r10;
        break;
    case RegisterType::kR11:
        value = mcontext->r11;
        break;
    case RegisterType::kR12:
        value = mcontext->r12;
        break;
    case RegisterType::kR13:
        value = mcontext->r13;
        break;
    case RegisterType::kR14:
        value = mcontext->r14;
        break;
    case RegisterType::kR15:
        value = mcontext->r15;
        break;
    default:
        break;
    }
    value = value << (8 - size) * 8;
    value = value >> (8 - size) * 8;
    return value;
}

static inline uint64_t
get_mem_value(uint8_t size, void* addr) {
    uint64_t value = 0;
    switch (size)
    {
    case 1:
        uint8_t temp1;
        if (dr_safe_read(addr, 1, &temp1, NULL)) {
            value = temp1;
        }
        break;
    case 2:
        uint8_t temp2;
        if (dr_safe_read(addr, 2, &temp2, NULL)) {
            value = temp2;
        }
        break;
    case 4:
        uint32_t temp4;
        if (dr_safe_read(addr, 4, &temp4, NULL)) {
            value = temp4;
        }
        break;
    case 8:
        uint64_t temp8;
        if (dr_safe_read(addr, 8, &temp8, NULL)) {
            value = temp8;
        }
        break;
    default:
        break;
    }
    // DRCCTLIB_PRINTF("size %d value %p", size, (void*)value);
    return value;
}

typedef struct _mem_ref_t {
    app_pc addr;
    uint32_t size;
    uint32_t tag;// 16bit(src/dst)16bit(index)
} mem_ref_t;

typedef struct _per_thread_t {
    mem_ref_t *pre_buf_list;
    mem_ref_t *cur_buf_list;
    void *cur_buf;
    context_handle_t pre_ctxt_hndl;
    int pre_mem_ref_num;
    Instruction* pre_instr;
    Instruction* cur_instr;
    CtxtContainer* ctxt_container;
} per_thread_t;

#define TLS_MEM_REF_BUFF_SIZE 100


// dr clean call
void
InsertCleancall(instr_shadow_t* instr_shadow, int32_t mem_ref_num, int32_t slot)
{
    void *drcontext = dr_get_current_drcontext();
    per_thread_t *pt = (per_thread_t *)drmgr_get_tls_field(drcontext, tls_idx);
    dr_mcontext_t mcontext = {
        sizeof(mcontext),
        DR_MC_ALL,
    };
    dr_get_mcontext(drcontext, &mcontext);
    for(size_t i = 0; i < pt->pre_instr->getDstOperandListSize(); i++) {
        Operand opnd = pt->pre_instr->getDstOperand((int)i);
        if (opnd.type == OperandType::kRegister) {
            opnd.value.valueUint = get_reg_value(&mcontext, opnd.options.regType, opnd.size);
            pt->pre_instr->setDstOperand((int)i, opnd);
        }
    }
    for (int i = 0; i < pt->pre_mem_ref_num; i++) {
        if (pt->pre_buf_list[i].tag > (1 << 16)) {
            int index =  pt->pre_buf_list[i].tag - (1 << 16);
            Operand opnd = pt->pre_instr->getDstOperand(index);
            opnd.options.addr = pt->pre_buf_list[i].addr;
            opnd.value.valueUint = get_mem_value(pt->pre_buf_list[i].size, pt->pre_buf_list[i].addr);
            pt->pre_instr->setDstOperand(index, opnd);
        }
    }
    
    if (ModuleFilter(pt->pre_instr->ip)) {
        OnAfterInsExec(pt->pre_instr, pt->pre_ctxt_hndl, (uint64_t)mcontext.xflags, pt->ctxt_container);
    }
    pt->pre_ctxt_hndl = drcctlib_get_context_handle(drcontext, slot);

    pt->cur_instr->ip = (uint64_t)(ptr_int_t)(instr_shadow->ip);
    pt->cur_instr->setOperatorType((OperatorType)instr_shadow->opcode);
    for(int i = 0; i < instr_shadow->num_srcs; i++) {
        Operand opnd = instr_shadow->srcs[i];
        if (opnd.type == OperandType::kRegister) {
            opnd.value.valueUint = get_reg_value(&mcontext, opnd.options.regType, opnd.size);
        }
        pt->cur_instr->addSrcOperand(opnd);
    }
    for(int i = 0; i < instr_shadow->num_dsts; i++) {
        pt->cur_instr->addDstOperand(instr_shadow->dsts[i]);
    }
    for (int i = 0; i < mem_ref_num; i++) {
        pt->pre_buf_list[i] = pt->cur_buf_list[i];
        if (pt->cur_buf_list[i].tag < (1 << 16)) {
            int index =  pt->cur_buf_list[i].tag;
            Operand opnd = pt->cur_instr->getSrcOperand(index);
            opnd.options.addr = pt->cur_buf_list[i].addr;
            opnd.value.valueUint = get_mem_value(pt->cur_buf_list[i].size, pt->cur_buf_list[i].addr);
            pt->cur_instr->setSrcOperand(index, opnd);
        }
    }
    pt->pre_mem_ref_num = mem_ref_num;
    
    Instruction* temp = pt->pre_instr;
    pt->pre_instr = pt->cur_instr;
    pt->cur_instr = temp;
    pt->cur_instr->clearOperands();
    BUF_PTR(pt->cur_buf, mem_ref_t, INSTRACE_TLS_OFFS_BUF_PTR) = pt->cur_buf_list;
}

// insert
static void
InstrumentMem(void *drcontext, instrlist_t *ilist, instr_t *where, opnd_t ref, uint64_t tag)
{
    /* We need two scratch registers */
    reg_id_t reg_mem_ref_ptr, free_reg;
    if (drreg_reserve_register(drcontext, ilist, where, NULL, &reg_mem_ref_ptr) !=
            DRREG_SUCCESS ||
        drreg_reserve_register(drcontext, ilist, where, NULL, &free_reg) !=
            DRREG_SUCCESS) {
        DRCCTLIB_EXIT_PROCESS("InstrumentMem drreg_reserve_register != DRREG_SUCCESS");
    }
    if (!drutil_insert_get_mem_addr(drcontext, ilist, where, ref, free_reg,
                                    reg_mem_ref_ptr)) {
        MINSERT(ilist, where,
                XINST_CREATE_load_int(drcontext, opnd_create_reg(free_reg),
                                      OPND_CREATE_CCT_INT(0)));
    }
    dr_insert_read_raw_tls(drcontext, ilist, where, tls_seg,
                           tls_offs + INSTRACE_TLS_OFFS_BUF_PTR, reg_mem_ref_ptr);
    // store mem_ref_t->addr
    MINSERT(ilist, where,
            XINST_CREATE_store(
                drcontext, OPND_CREATE_MEMPTR(reg_mem_ref_ptr, offsetof(mem_ref_t, addr)),
                opnd_create_reg(free_reg)));
    // store mem_ref_t->size
#ifdef ARM_CCTLIB
    MINSERT(ilist, where,
            XINST_CREATE_load_int(drcontext, opnd_create_reg(free_reg),
                                  OPND_CREATE_INT32(drutil_opnd_mem_size_in_bytes(ref, where))));
    MINSERT(ilist, where,
            XINST_CREATE_store(drcontext, OPND_CREATE_MEMPTR(reg_mem_ref_ptr, offsetof(mem_ref_t, size)),
                             opnd_create_reg(free_reg)));
#else
    MINSERT(ilist, where,
            XINST_CREATE_store(drcontext, OPND_CREATE_MEMPTR(reg_mem_ref_ptr, offsetof(mem_ref_t, size)),
                             OPND_CREATE_INT32(drutil_opnd_mem_size_in_bytes(ref, where))));
#endif

    // store mem_ref_t->tag
#ifdef ARM_CCTLIB
    MINSERT(ilist, where,
            XINST_CREATE_load_int(drcontext, opnd_create_reg(free_reg),
                                  OPND_CREATE_INT32(tag)));
    MINSERT(ilist, where,
            XINST_CREATE_store(drcontext, OPND_CREATE_MEMPTR(reg_mem_ref_ptr, offsetof(mem_ref_t, tag)),
                             opnd_create_reg(free_reg)));
#else
    MINSERT(ilist, where,
            XINST_CREATE_store(drcontext, OPND_CREATE_MEMPTR(reg_mem_ref_ptr, offsetof(mem_ref_t, tag)),
                             OPND_CREATE_INT32(tag)));
#endif

#ifdef ARM_CCTLIB
    MINSERT(ilist, where,
            XINST_CREATE_load_int(drcontext, opnd_create_reg(free_reg),
                                  OPND_CREATE_CCT_INT(sizeof(mem_ref_t))));
    MINSERT(ilist, where,
            XINST_CREATE_add(drcontext, opnd_create_reg(reg_mem_ref_ptr),
                             opnd_create_reg(free_reg)));
#else
    MINSERT(ilist, where,
            XINST_CREATE_add(drcontext, opnd_create_reg(reg_mem_ref_ptr),
                             OPND_CREATE_CCT_INT(sizeof(mem_ref_t))));
#endif
    dr_insert_write_raw_tls(drcontext, ilist, where, tls_seg,
                            tls_offs + INSTRACE_TLS_OFFS_BUF_PTR, reg_mem_ref_ptr);
    /* Restore scratch registers */
    if (drreg_unreserve_register(drcontext, ilist, where, reg_mem_ref_ptr) !=
            DRREG_SUCCESS ||
        drreg_unreserve_register(drcontext, ilist, where, free_reg) != DRREG_SUCCESS) {
        DRCCTLIB_EXIT_PROCESS("InstrumentMem drreg_unreserve_register != DRREG_SUCCESS");
    }
}

// analysis
void
InstrumentInsCallback(void *drcontext, instr_instrument_msg_t *instrument_msg)
{
    instrlist_t *bb = instrument_msg->bb;
    instr_t *instr = instrument_msg->instr;
    int32_t slot = instrument_msg->slot;
    // instr_t *instr_operands = drmgr_orig_app_instr_for_operands(drcontext);
    // if (instr_operands == NULL)
    //     return;
    int32_t instr_key = 0;
    instr_shadow_t *instr_shadow;
    app_pc pc = instr_get_app_pc(instr);
    dr_mutex_lock(instr_shadow_lock);
    bool is_instr_inited = false;
    void *stored_key = hashtable_lookup(&global_instr_pc_table, (void *)pc);
    if (stored_key != NULL) {
        instr_key = (int32_t)(ptr_int_t)stored_key;
        instr_shadow = global_instr_shadow_cache->get_object_by_index(instr_key);
        is_instr_inited = true;
    } else {
        instr_shadow = global_instr_shadow_cache->get_next_object();
        instr_key = instr_shadow->key;
        instr_shadow_init(instr_shadow, pc, instr_num_srcs(instr), instr_num_dsts(instr));
        // char c[80];
        // instr_disassemble_to_buffer(drcontext, instr, c, 80);
        // DRCCTLIB_PRINTF("pc %p %s", pc, c);
        hashtable_add(&global_instr_pc_table, (void *)pc, (void *)(ptr_int_t)instr_key);
        is_instr_inited = false;
    }
    dr_mutex_unlock(instr_shadow_lock);

    if (!is_instr_inited) {
        instr_shadow->opcode = instr_get_opcode(instr);
        for (int i = 0; i < instr_num_srcs(instr); i++) {
            opnd_t ref = instr_get_src(instr, i);
            int index = instr_num_srcs(instr) - i - 1;
            if (opnd_is_memory_reference(ref)) {
                instr_shadow->srcs[index].type = OperandType::kMemory;
                instr_shadow->srcs[index].options.addr = NULL;
                instr_shadow->srcs[index].value.valueUint = 0;
                instr_shadow->srcs[index].size = drutil_opnd_mem_size_in_bytes(ref, instr);
            } else if (opnd_is_reg(ref)) {
                instr_shadow->srcs[index].type = OperandType::kRegister;
                instr_shadow->srcs[index].options.regType = (RegisterType)opnd_get_reg(ref) >= RegisterType::kUnsupportReg ? RegisterType::kNULL : (RegisterType)opnd_get_reg(ref);
                instr_shadow->srcs[index].value.valueUint = 0;
                instr_shadow->srcs[index].size = get_reg_size(ref);
            } else if (opnd_is_immed_int(ref)){
                instr_shadow->srcs[index].type = OperandType::kImmediateInt;
                instr_shadow->srcs[index].options.addr = NULL;
                instr_shadow->srcs[index].value.valueUint = opnd_get_immed_int64(ref);
                instr_shadow->srcs[index].size = 8;
            } else if (opnd_is_immed_float(ref)){
                instr_shadow->srcs[index].type = OperandType::kImmediateFloat;
                instr_shadow->srcs[index].options.addr = NULL;
                instr_shadow->srcs[index].value.valueUint = opnd_get_immed_float(ref);
                instr_shadow->srcs[index].size = 4;
            } else {
                instr_shadow->srcs[index].type = OperandType::kUnsupportOpnd;
                instr_shadow->srcs[index].options.addr = NULL;
                instr_shadow->srcs[index].value.valueUint = 0;
                instr_shadow->srcs[index].size = 0;
            }
        }
        for (int i = 0; i < instr_num_dsts(instr); i++) {
            opnd_t ref = instr_get_dst(instr, i);
            int index = instr_num_dsts(instr) - i - 1;
            if (opnd_is_memory_reference(ref)) {
                instr_shadow->dsts[index].type = OperandType::kMemory;
                instr_shadow->dsts[index].options.addr = NULL;
                instr_shadow->dsts[index].value.valueUint = 0;
                instr_shadow->dsts[index].size = drutil_opnd_mem_size_in_bytes(ref, instr);
            } else if (opnd_is_reg(ref)) {
                instr_shadow->dsts[index].type = OperandType::kRegister;
                instr_shadow->dsts[index].options.regType = (RegisterType)opnd_get_reg(ref);
                instr_shadow->dsts[index].value.valueUint = 0;
                instr_shadow->dsts[index].size = get_reg_size(ref);
            } else {
                instr_shadow->dsts[index].type = OperandType::kUnsupportOpnd;
                instr_shadow->dsts[index].options.addr = NULL;
                instr_shadow->dsts[index].value.valueUint = 0;
                instr_shadow->dsts[index].size = 0;
            }
        }
    }

#ifdef x86_CCTLIB
    if (drreg_reserve_aflags(drcontext, bb, instr) != DRREG_SUCCESS) {
        DRCCTLIB_EXIT_PROCESS("instrument_before_every_instr_meta_instr "
                              "drreg_reserve_aflags != DRREG_SUCCESS");
    }
#endif
    int mem_ref_num = 0;
    for (int i = 0; i < instr_num_srcs(instr); i++) {
        if (opnd_is_memory_reference(instr_get_src(instr, i))) {
            uint32_t tag = instr_num_srcs(instr) - 1 - i;
            mem_ref_num++;
            InstrumentMem(drcontext, bb, instr, instr_get_src(instr, i), tag);
        }
    }
    for (int i = 0; i < instr_num_dsts(instr); i++) {
        if (opnd_is_memory_reference(instr_get_dst(instr, i))) {
            uint32_t tag = (1 << 16) + instr_num_dsts(instr) - 1 - i;
            mem_ref_num++;
            InstrumentMem(drcontext, bb, instr, instr_get_dst(instr, i), tag);
        }
    }
#ifdef x86_CCTLIB
    if (drreg_unreserve_aflags(drcontext, bb, instr) != DRREG_SUCCESS) {
        DRCCTLIB_EXIT_PROCESS("drreg_unreserve_aflags != DRREG_SUCCESS");
    }
#endif
    dr_insert_clean_call(drcontext, bb, instr, (void *)InsertCleancall, false, 3,
                         OPND_CREATE_INTPTR(instr_shadow), OPND_CREATE_CCT_INT(mem_ref_num), OPND_CREATE_CCT_INT(slot));
}

static void
ClientThreadStart(void *drcontext)
{
    per_thread_t *pt = (per_thread_t *)dr_thread_alloc(drcontext, sizeof(per_thread_t));
    if (pt == NULL) {
        DRCCTLIB_EXIT_PROCESS("pt == NULL");
    }
    drmgr_set_tls_field(drcontext, tls_idx, (void *)pt);

    pt->pre_buf_list =
        (mem_ref_t *)dr_global_alloc(TLS_MEM_REF_BUFF_SIZE * sizeof(mem_ref_t));
    pt->cur_buf = dr_get_dr_segment_base(tls_seg);
    pt->cur_buf_list =
        (mem_ref_t *)dr_global_alloc(TLS_MEM_REF_BUFF_SIZE * sizeof(mem_ref_t));
    BUF_PTR(pt->cur_buf, mem_ref_t, INSTRACE_TLS_OFFS_BUF_PTR) = pt->cur_buf_list;

    pt->cur_instr = new Instruction();
    pt->pre_instr = new Instruction();
    pt->pre_instr->setOperatorType(OperatorType::kOPINVALID);
    pt->pre_mem_ref_num = 0;
    pt->ctxt_container = new CtxtContainer();
    pt->pre_ctxt_hndl = 0;
}

static void
ClientThreadEnd(void *drcontext)
{
    per_thread_t *pt = (per_thread_t *)drmgr_get_tls_field(drcontext, tls_idx);
    dr_global_free(pt->cur_buf_list, TLS_MEM_REF_BUFF_SIZE * sizeof(mem_ref_t));
    dr_global_free(pt->pre_buf_list, TLS_MEM_REF_BUFF_SIZE * sizeof(mem_ref_t));
    delete pt->cur_instr;
    delete pt->pre_instr;
    dr_mutex_lock(ctxt_container_lock);
    std::vector<context_handle_t> list = pt->ctxt_container->getCtxtList();
    for (size_t i = 0; i < list.size(); i++) {
        global_ctxt_container->addCtxt(list[i]);
    }
    dr_mutex_unlock(ctxt_container_lock);
    delete pt->ctxt_container;
    dr_thread_free(drcontext, pt, sizeof(per_thread_t));
}

static void
ModuleLoadEvent(void *drcontext, const module_data_t *info, bool loaded)
{
    DRCCTLIB_PRINTF("%s %p %p", dr_module_preferred_name(info), info->start, info->end);
    if (strstr(dr_module_preferred_name(info), "integer_overflow.so") != NULL ||
        strstr(dr_module_preferred_name(info), "ld-linux-x86-64.so") != NULL ||
        strstr(dr_module_preferred_name(info), "dynamorio.so") != NULL ||
        strstr(dr_module_preferred_name(info), "vdso.so") != NULL ||
        strstr(dr_module_preferred_name(info), "libc.so") != NULL) {
        global_mod_array[global_loaded_module_num].base = info->start;
        global_mod_array[global_loaded_module_num].end = info->end;
        global_loaded_module_num++;
    }
}

static void
ClientInit(int argc, const char *argv[])
{
    if (!drmgr_init()) {
        DRCCTLIB_EXIT_PROCESS("ERROR: integer_overflow_core unable to "
                              "initialize drmgr");
    }
    drreg_options_t ops = { sizeof(ops), 4 /*max slots needed*/, false };
    if (drreg_init(&ops) != DRREG_SUCCESS) {
        DRCCTLIB_EXIT_PROCESS("ERROR: integer_overflow_core unable to "
                              "initialize drreg");
    }
    if (!drutil_init()) {
        DRCCTLIB_EXIT_PROCESS("ERROR: integer_overflow_core unable to "
                              "initialize drutil");
    }
    drmgr_register_thread_init_event(ClientThreadStart);
    drmgr_register_thread_exit_event(ClientThreadEnd);
    drmgr_register_module_load_event(ModuleLoadEvent);


    tls_idx = drmgr_register_tls_field();
    if (tls_idx == -1) {
        DRCCTLIB_EXIT_PROCESS("ERROR: integer_overflow_core "
                              "drmgr_register_tls_field fail");
    }
    if (!dr_raw_tls_calloc(&tls_seg, &tls_offs, INSTRACE_TLS_COUNT, 0)) {
        DRCCTLIB_EXIT_PROCESS("ERROR: integer_overflow_core "
                              "dr_raw_tls_calloc fail");
    }
    drcctlib_init(DRCCTLIB_FILTER_ALL_INSTR, INVALID_FILE, InstrumentInsCallback, true);

    hashtable_init(&global_instr_pc_table, INSTR_PC_TABLE_HASH_BITS, HASH_INTPTR, false);
    global_instr_shadow_cache = new thread_shared_memory_cache_t<instr_shadow_t>(
        TSM_CACHE_PAGE1_BIT, TSM_CACHE_PAGE2_BIT, instr_shadow_create, instr_shadow_free_cache,
        instr_shadow_cache_lock);
    instr_shadow_lock = dr_mutex_create();
    instr_shadow_cache_lock = dr_mutex_create();
    global_ctxt_container = new CtxtContainer();
    ctxt_container_lock = dr_mutex_create();
}

static void
ClientExit(void)
{
    OnBeforeAppExit(global_ctxt_container);
    drcctlib_exit();
    dr_mutex_destroy(instr_shadow_lock);
    dr_mutex_destroy(instr_shadow_cache_lock);
    delete global_instr_shadow_cache;
    dr_mutex_destroy(ctxt_container_lock);
    delete global_ctxt_container;
    hashtable_delete(&global_instr_pc_table);

    if (!dr_raw_tls_cfree(tls_offs, INSTRACE_TLS_COUNT)) {
        DRCCTLIB_EXIT_PROCESS("ERROR: integer_overflow_core "
                              "dr_raw_tls_calloc fail");
    }

    if (!drmgr_unregister_thread_init_event(ClientThreadStart) ||
        !drmgr_unregister_thread_exit_event(ClientThreadEnd) ||
        !drmgr_unregister_module_load_event(ModuleLoadEvent) ||
        !drmgr_unregister_tls_field(tls_idx)) {
        DRCCTLIB_PRINTF("ERROR: integer_overflow_core "
                        "failed to unregister in ClientExit");
    }
    drmgr_exit();
    if (drreg_exit() != DRREG_SUCCESS) {
        DRCCTLIB_PRINTF("failed to exit drreg");
    }
    drutil_exit();
}

#ifdef __cplusplus
extern "C" {
#endif

DR_EXPORT void
dr_client_main(client_id_t id, int argc, const char *argv[])
{
    ClientInit(argc, argv);
    dr_register_exit_event(ClientExit);
}

#ifdef __cplusplus
}
#endif