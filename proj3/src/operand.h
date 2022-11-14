/*
 *  Copyright (c) 2020-2021 Xuhpclab. All rights reserved.
 *  Licensed under the MIT License.
 *  See LICENSE file for more information.
 */

#ifndef _OPERAND_H_
#define _OPERAND_H_

#include <cstdint>
#include "register.h"

namespace runtime_profiling {

typedef enum {
    kRegister,
    kImmediateInt,
    kImmediateFloat,
    kImmediateDouble,
    kMemory,
    kUnsupportOpnd,
    kNullOpnd
} OperandType;

typedef struct _operand {
    OperandType type;
    union {
        /* data */
        uint64_t valueUint;
        int64_t valueInt64;
        int32_t valueInt32;
        int16_t valueInt16;
        int8_t valueInt8;
        float valueFloat;
        double valueDouble;
    } value;
    uint8_t size; // byte (1byte=8bit;2byte=16bit;4byte=32bit;8byte=64bit)
    union {
        void *addr;
        RegisterType regType;
    } options;
} Operand;

}

#endif //_OPERAND_H_