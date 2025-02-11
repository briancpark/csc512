/* 
 *  Copyright (c) 2022 Xuhpclab. All rights reserved.
 *  Licensed under the MIT License.
 *  See LICENSE file for more information.
 */

#include <iterator>
#include <vector>
#include <map>

#include "dr_api.h"
#include "drcctlib.h"

#define DRCCTLIB_PRINTF(_FORMAT, _ARGS...) \
    DRCCTLIB_PRINTF_TEMPLATE("instr_statistics_clean_call", _FORMAT, ##_ARGS)
#define DRCCTLIB_EXIT_PROCESS(_FORMAT, _ARGS...) \
    DRCCTLIB_CLIENT_EXIT_PROCESS_TEMPLATE("instr_statistics_clean_call", _FORMAT, ##_ARGS)

#ifdef ARM_CCTLIB
#    define OPND_CREATE_CCT_INT OPND_CREATE_INT
#else
#    define OPND_CREATE_CCT_INT OPND_CREATE_INT32
#endif

#define MAX_CLIENT_CCT_PRINT_DEPTH 10
#define TOP_REACH_NUM_SHOW 10

uint64_t *load_hndl_call_num;
uint64_t *store_hndl_call_num;
uint64_t *cond_branch_hndl_call_num;
uint64_t *uncond_branch_hndl_call_num;

static file_t gTraceFile;

uint64_t global_load;
uint64_t global_store;
uint64_t global_cond_branch;
uint64_t global_uncond_branch;

using namespace std;

// Execution
void
InsCount(int32_t slot, int32_t itype)
{
    // Get the context unique Id during the application runtime
    void *drcontext = dr_get_current_drcontext();
    context_handle_t cur_ctxt_hndl = drcctlib_get_context_handle(drcontext, slot);

    // Add one for the count number of the context’s instruction and
    // store it to a global array.
    
    if (itype & 1) {
        global_uncond_branch++;
        uncond_branch_hndl_call_num[cur_ctxt_hndl]++;
    }
    if (itype & 2) {
        global_cond_branch++;
        cond_branch_hndl_call_num[cur_ctxt_hndl]++;
    }
    if (itype & 4) {
        global_store++;
        store_hndl_call_num[cur_ctxt_hndl]++;
    }
    if (itype & 8) {
        global_load++;
        load_hndl_call_num[cur_ctxt_hndl]++;
    }
}

// Transformation
void
InsTransEventCallback(void *drcontext, instr_instrument_msg_t *instrument_msg)
{

    instrlist_t *bb = instrument_msg->bb;
    instr_t *instr = instrument_msg->instr;
    int32_t slot = instrument_msg->slot;

    int32_t itype = 0;

    if (instr_is_ubr(instr))
        itype |= 1;
    if (instr_is_cbr(instr))
        itype |= 2;
    if (instr_writes_memory(instr))
        itype |= 4;
    if (instr_reads_memory(instr))
        itype |= 8; 

    dr_insert_clean_call(drcontext, bb, instr, (void *)InsCount, false, 2, OPND_CREATE_CCT_INT(slot), OPND_CREATE_CCT_INT(itype));
}

static inline void
InitGlobalBuff()
{
    global_load = 0;
    global_store = 0;
    global_cond_branch = 0;
    global_uncond_branch = 0;

    load_hndl_call_num = (uint64_t *)dr_raw_mem_alloc(
        CONTEXT_HANDLE_MAX * sizeof(uint64_t), DR_MEMPROT_READ | DR_MEMPROT_WRITE, NULL);
    if (load_hndl_call_num == NULL) {
        DRCCTLIB_EXIT_PROCESS(
            "init_global_buff error: dr_raw_mem_alloc fail load_hndl_call_num");
    }
    store_hndl_call_num = (uint64_t *)dr_raw_mem_alloc(
        CONTEXT_HANDLE_MAX * sizeof(uint64_t), DR_MEMPROT_READ | DR_MEMPROT_WRITE, NULL);
    if (store_hndl_call_num == NULL) {
        DRCCTLIB_EXIT_PROCESS(
            "init_global_buff error: dr_raw_mem_alloc fail store_hndl_call_num");
    }
    cond_branch_hndl_call_num = (uint64_t *)dr_raw_mem_alloc(
        CONTEXT_HANDLE_MAX * sizeof(uint64_t), DR_MEMPROT_READ | DR_MEMPROT_WRITE, NULL);
    if (cond_branch_hndl_call_num == NULL) {
        DRCCTLIB_EXIT_PROCESS(
            "init_global_buff error: dr_raw_mem_alloc fail cond_branch_hndl_call_num");
    }
    uncond_branch_hndl_call_num = (uint64_t *)dr_raw_mem_alloc(
        CONTEXT_HANDLE_MAX * sizeof(uint64_t), DR_MEMPROT_READ | DR_MEMPROT_WRITE, NULL);;
    if (uncond_branch_hndl_call_num == NULL) {
        DRCCTLIB_EXIT_PROCESS(
            "init_global_buff error: dr_raw_mem_alloc fail uncond_branch_hndl_call_num");
    }
}

static inline void
FreeGlobalBuff()
{
    dr_raw_mem_free(load_hndl_call_num, CONTEXT_HANDLE_MAX * sizeof(uint64_t));
    dr_raw_mem_free(store_hndl_call_num, CONTEXT_HANDLE_MAX * sizeof(uint64_t));
    dr_raw_mem_free(cond_branch_hndl_call_num, CONTEXT_HANDLE_MAX * sizeof(uint64_t));
    dr_raw_mem_free(uncond_branch_hndl_call_num, CONTEXT_HANDLE_MAX * sizeof(uint64_t));
}

static void
ClientInit(int argc, const char *argv[])
{
    char name[MAXIMUM_FILEPATH] = "";
    DRCCTLIB_INIT_LOG_FILE_NAME(
        name, "drcctlib_instr_analysis", "out");
    DRCCTLIB_PRINTF("Creating log file at:%s", name);

    gTraceFile = dr_open_file(name, DR_FILE_WRITE_OVERWRITE | DR_FILE_ALLOW_LARGE);
    DR_ASSERT(gTraceFile != INVALID_FILE);

    InitGlobalBuff();
    drcctlib_init(DRCCTLIB_FILTER_ALL_INSTR, INVALID_FILE, InsTransEventCallback, false);
}

typedef struct _output_format_t {
    context_handle_t handle;
    uint64_t count;
} output_format_t;

static void
ClientExit(void)
{
    output_format_t *output_list =
        (output_format_t *)dr_global_alloc(TOP_REACH_NUM_SHOW * sizeof(output_format_t));

    // Get the number of all contexts. 
    context_handle_t max_ctxt_hndl = drcctlib_get_global_context_handle_num();
    uint64_t *hndl_call_num [4] = { load_hndl_call_num, store_hndl_call_num, cond_branch_hndl_call_num, uncond_branch_hndl_call_num };
    uint64_t global_nums[4] = { global_load, global_store, global_cond_branch, global_uncond_branch };
    const char* instr_label[4]
        = { "MEMORY LOAD", "MEMORY STORE", "CONDITIONAL BRANCHES", "UNCONDITIONAL BRANCHES" };

    for (int32_t instr_type = 0; instr_type < 4; instr_type++) {
        for (int32_t i = 0; i < TOP_REACH_NUM_SHOW; i++) {
            output_list[i].handle = 0;
            output_list[i].count = 0;
        }
        
        dr_fprintf(gTraceFile, "%s : %d\n", instr_label[instr_type], global_nums[instr_type]);
        for (context_handle_t i = 0; i < max_ctxt_hndl; i++) {
            if (hndl_call_num[instr_type][i] <= 0) {
                continue;
            }
            if (hndl_call_num[instr_type][i] > output_list[0].count) {
                uint64_t min_count = hndl_call_num[instr_type][i];
                int32_t min_idx = 0;
                for (int32_t j = 1; j < TOP_REACH_NUM_SHOW; j++) {
                    if (output_list[j].count < min_count) {
                        min_count = output_list[j].count;
                        min_idx = j;
                    }
                }
                output_list[0].count = min_count;
                output_list[0].handle = output_list[min_idx].handle;
                output_list[min_idx].count = hndl_call_num[instr_type][i];
                output_list[min_idx].handle = i;
            }
        }
        output_format_t temp;
        for (int32_t i = 0; i < TOP_REACH_NUM_SHOW; i++) {
            for (int32_t j = i; j < TOP_REACH_NUM_SHOW; j++) {
                if (output_list[i].count < output_list[j].count) {
                    temp = output_list[i];
                    output_list[i] = output_list[j];
                    output_list[j] = temp;
                }
            }
        }

        // Output the execution times and backtrace of the ordered top contexts
        for (int32_t i = 0; i < TOP_REACH_NUM_SHOW; i++) {
            if (output_list[i].handle == 0) {
                break;
            }
            dr_fprintf(gTraceFile, "NO. %d PC ", i + 1);
            drcctlib_print_backtrace_first_item(gTraceFile, output_list[i].handle, true, false);
            dr_fprintf(gTraceFile, "=>EXECUTION TIMES\n%lld\n=>BACKTRACE\n",
                    output_list[i].count);
            drcctlib_print_backtrace(gTraceFile, output_list[i].handle, false, true, -1);
            dr_fprintf(gTraceFile, "\n\n\n");
	    }
    }    
    dr_global_free(output_list, TOP_REACH_NUM_SHOW * sizeof(output_format_t));

    FreeGlobalBuff();
    drcctlib_exit();

    dr_close_file(gTraceFile);
}

#ifdef __cplusplus
extern "C" {
#endif

DR_EXPORT void
dr_client_main(client_id_t id, int argc, const char *argv[])
{
    dr_set_client_name("DynamoRIO Client 'drcctlib_instr_analysis'",
                       "http://dynamorio.org/issues");

    ClientInit(argc, argv);
    dr_register_exit_event(ClientExit);
}

#ifdef __cplusplus
}
#endif