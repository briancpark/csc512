/*
 *  Copyright (c) 2020-2021 Xuhpclab. All rights reserved.
 *  Licensed under the MIT License.
 *  See LICENSE file for more information.
 */

#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <cstdint>
#include <vector>
#include "operand.h"
#include "operator.h"

namespace runtime_profiling {

class Instruction {
public:
    uint64_t ip;
    void
    addSrcOperand(Operand operand)
    {
        srcOpndList_.push_back(operand);
    }

    void
    addDstOperand(Operand operand)
    {
        dstOpndList_.push_back(operand);
    }

    void
    setOperatorType(OperatorType opType)
    {
        opType_ = opType;
    }

    OperatorType
    getOperatorType()
    {
        return opType_;
    }

    size_t
    getSrcOperandListSize()
    {
        return srcOpndList_.size();
    }

    std::vector<Operand>
    getSrcOperandList()
    {
        return srcOpndList_;
    }

    size_t
    getDstOperandListSize()
    {
        return dstOpndList_.size();
    }

    std::vector<Operand>
    getDstOperandList()
    {
        return dstOpndList_;
    }

    void clearOperands() {
        srcOpndList_.clear();
        dstOpndList_.clear();
    }

    void
    setSrcOperand(int index, Operand opnd)
    {
        srcOpndList_[index] = opnd;
    }

    Operand
    getSrcOperand(int index)
    {
        if ((size_t)index >= srcOpndList_.size() || index < 0) {
            Operand temp;
            temp.type = OperandType::kNullOpnd;
            temp.value.valueUint = 0;
            temp.size = 0;
            temp.options.addr = NULL;
            return temp;
        }
        return srcOpndList_[index];
    }

    void
    setDstOperand(int index, Operand opnd)
    {
        dstOpndList_[index] = opnd;
    }

    Operand
    getDstOperand(int index)
    {
        if ((size_t)index >= dstOpndList_.size() || index < 0) {
            Operand temp;
            temp.type = OperandType::kNullOpnd;
            temp.value.valueUint = 0;
            temp.size = 0;
            temp.options.addr = NULL;
            return temp;
        }
        return dstOpndList_[index];
    }

private:
    OperatorType opType_;
    std::vector<Operand> srcOpndList_;
    std::vector<Operand> dstOpndList_;
};
}

#endif //_INSTRUCTION_H_