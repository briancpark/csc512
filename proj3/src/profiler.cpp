
#include <iostream>
#include <bitset>
#include <cstdint>
#include <cmath>
#include <limits>
#include "profiler.h"
#include "drcctlib_vscodeex_format.h"

using namespace std;
using namespace DrCCTProf;

/*
    Tips: different integer types have distinct boundaries
    INT64_MAX, INT32_MAX, INT16_MAX, INT8_MAX
    INT64_MIN, INT32_MIN, INT16_MIN, INT8_MIN
*/

namespace runtime_profiling {

static inline int64_t
GetOpndIntValue(Operand opnd)
{
    if (opnd.type == OperandType::kImmediateFloat ||
        opnd.type == OperandType::kImmediateDouble ||
        opnd.type == OperandType::kUnsupportOpnd || opnd.type == OperandType::kNullOpnd) {
        return 0;
    }
    int64_t value = 0;
    switch (opnd.size) {
    case 1: value = static_cast<int64_t>(opnd.value.valueInt8); break;
    case 2: value = static_cast<int64_t>(opnd.value.valueInt16); break;
    case 4: value = static_cast<int64_t>(opnd.value.valueInt32); break;
    case 8: value = static_cast<int64_t>(opnd.value.valueInt64); break;
    default: break;
    }
    return value;
}

static inline int64_t
getIntMax(uint8_t bytes)
{
    switch (bytes) {
    case 1: return INT8_MAX;
    case 2: return INT16_MAX;
    case 4: return INT32_MAX;
    case 8: return INT64_MAX;
    default: return 0;
    }
}

static inline int64_t
getIntMin(uint8_t bytes)
{
    switch (bytes) {
    case 1: return INT8_MIN;
    case 2: return INT16_MIN;
    case 4: return INT32_MIN;
    case 8: return INT64_MIN;
    default: return 0;
    }
}

static inline bool
safe_add(int64_t a, int64_t b, uint8_t bytes)
{
    int64_t intMax = getIntMax(bytes);
    int64_t intMin = getIntMin(bytes);

    if (a > 0 && b > intMax - a)
        return true;
    else if (a < 0 && b < intMin - a)
        return true;
    else
        return false;
}

static inline bool
safe_sub(int64_t a, int64_t b, uint8_t bytes)
{
    int64_t intMax = getIntMax(bytes);
    int64_t intMin = getIntMin(bytes);

    if (a > 0 && b < intMin + a)
        return true;
    else if (a < 0 && b > intMax + a + 1)
        return true;
    else
        return false;
}

static inline bool
safe_shl(int64_t a, int64_t b, uint8_t bytes)
{
    __int128_t wideResult = a;
    wideResult = wideResult << b;

    return (bytes == 1 && (wideResult > INT8_MAX || wideResult < INT8_MIN)) ||
        (bytes == 2 && (wideResult > INT16_MAX || wideResult < INT16_MIN)) ||
        (bytes == 4 && (wideResult > INT32_MAX || wideResult < INT32_MIN)) ||
        (bytes == 8 && (wideResult > INT64_MAX || wideResult < INT64_MIN));
}

// implement your algorithm in this function
static inline bool
IntegerOverflow(Instruction *instr, uint64_t flagsValue)
{
    Operand srcOpnd0 = instr->getSrcOperand(0);
    Operand srcOpnd1 = instr->getSrcOperand(1);
    Operand dstOpnd = instr->getDstOperand(0);
    std::bitset<64> bitFlagsValue(flagsValue);
    OperatorType optype = instr->getOperatorType();

    // OVERFLOW BIT SOLUTION STARTS HERE
    // if (optype == OperatorType::kOPadd ||
    //     optype == OperatorType::kOPsub ||
    //     optype == OperatorType::kOPshl) {
    //     return (flagsValue & EFLAGS_READ_OF) == 0;
    // } else {
    //     return false;
    // }
    // OVERFLOW BIT SOLUTION STARTS HERE

    int64_t src0Val = GetOpndIntValue(srcOpnd0);
    int64_t src1Val = GetOpndIntValue(srcOpnd1);

    if (optype == OperatorType::kOPadd) {
        return safe_add(src0Val, src1Val, dstOpnd.size);
    } else if (optype == OperatorType::kOPsub) {
        return safe_sub(src0Val, src1Val, dstOpnd.size);
    } else if (optype == OperatorType::kOPshl) {
        return safe_shl(src0Val, src1Val, dstOpnd.size);
    } else {
        return false;
    }
}

void
OnAfterInsExec(Instruction *instr, context_handle_t contxt, uint64_t flagsValue,
               CtxtContainer *ctxtContainer)
{
    if (IntegerOverflow(instr, flagsValue)) {
        ctxtContainer->addCtxt(contxt);
    }
    // add: Destination = Source0 + Source1
    if (instr->getOperatorType() == OperatorType::kOPadd) {
        Operand srcOpnd0 = instr->getSrcOperand(0);
        Operand srcOpnd1 = instr->getSrcOperand(1);
        Operand dstOpnd = instr->getDstOperand(0);
        std::bitset<64> bitFlagsValue(flagsValue);

        if (IntegerOverflow(instr, flagsValue)) {
            ctxtContainer->addCtxt(contxt);
        }

        cout << "ip(" << hex << instr->ip << "):"
             << "add " << dec << GetOpndIntValue(srcOpnd0) << " "
             << GetOpndIntValue(srcOpnd1) << " -> " << GetOpndIntValue(dstOpnd) << " "
             << bitFlagsValue << endl;
    }
    // sub: Destination = Source0 - Source1
    if (instr->getOperatorType() == OperatorType::kOPsub) {
        Operand srcOpnd0 = instr->getSrcOperand(0);
        Operand srcOpnd1 = instr->getSrcOperand(1);
        Operand dstOpnd = instr->getDstOperand(0);

        std::bitset<64> bitFlagsValue(flagsValue);

        if (IntegerOverflow(instr, flagsValue)) {
            ctxtContainer->addCtxt(contxt);
        }

        cout << "ip(" << hex << instr->ip << "):"
             << "sub " << dec << GetOpndIntValue(srcOpnd0) << " "
             << GetOpndIntValue(srcOpnd1) << " -> " << GetOpndIntValue(dstOpnd) << " "
             << bitFlagsValue << endl;
    }
    // shl: Destination = Source0 << Source1
    if (instr->getOperatorType() == OperatorType::kOPshl) {
        Operand srcOpnd0 = instr->getSrcOperand(0);
        Operand srcOpnd1 = instr->getSrcOperand(1);
        Operand dstOpnd = instr->getDstOperand(0);
        std::bitset<64> bitFlagsValue(flagsValue);

        if (IntegerOverflow(instr, flagsValue)) {
            ctxtContainer->addCtxt(contxt);
        }

        cout << "ip(" << hex << instr->ip << "):"
             << "shl " << dec << GetOpndIntValue(srcOpnd0) << " "
             << GetOpndIntValue(srcOpnd1) << " -> " << GetOpndIntValue(dstOpnd) << " "
             << bitFlagsValue << endl;
    }
}

void
OnBeforeAppExit(CtxtContainer *ctxtContainer)
{
    Profile::profile_t *profile = new Profile::profile_t();
    profile->add_metric_type(1, "", "integer overflow occurrence");

    std::vector<context_handle_t> list = ctxtContainer->getCtxtList();
    for (size_t i = 0; i < list.size(); i++) {
        inner_context_t *cur_ctxt = drcctlib_get_full_cct(list[i]);
        profile->add_sample(cur_ctxt)->append_metirc((uint64_t)1);
        drcctlib_free_full_cct(cur_ctxt);
    }
    profile->serialize_to_file("integer-overflow-profile.drcctprof");
    delete profile;

    file_t profileTxt = dr_open_file("integer-overflow-profile.txt",
                                     DR_FILE_WRITE_OVERWRITE | DR_FILE_ALLOW_LARGE);
    DR_ASSERT(profileTxt != INVALID_FILE);
    for (size_t i = 0; i < list.size(); i++) {
        dr_fprintf(profileTxt, "INTEGER OVERFLOW\n");
        drcctlib_print_backtrace_first_item(profileTxt, list[i], true, false);
        dr_fprintf(profileTxt, "=>BACKTRACE\n");
        drcctlib_print_backtrace(profileTxt, list[i], false, true, -1);
        dr_fprintf(profileTxt, "\n\n");
    }
    dr_close_file(profileTxt);
}
}