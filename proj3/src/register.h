/* 
 *  Copyright (c) 2020-2021 Xuhpclab. All rights reserved.
 *  Licensed under the MIT License.
 *  See LICENSE file for more information.
 */

#ifndef _REGISTER_H_
#define _REGISTER_H_

namespace runtime_profiling{

typedef enum {
    kNULL, /**< Sentinel value indicating no register, for address modes. */
    /* 64-bit general purpose */
    kRAX, /**< The "rax" register. */
    kRCX, /**< The "rcx" register. */
    kRDX, /**< The "rdx" register. */
    kRBX, /**< The "rbx" register. */
    kRSP, /**< The "rsp" register. */
    kRBP, /**< The "rbp" register. */
    kRSI, /**< The "rsi" register. */
    kRDI, /**< The "rdi" register. */
    kR8,  /**< The "r8" register. */
    kR9,  /**< The "r9" register. */
    kR10, /**< The "r10" register. */
    kR11, /**< The "r11" register. */
    kR12, /**< The "r12" register. */
    kR13, /**< The "r13" register. */
    kR14, /**< The "r14" register. */
    kR15, /**< The "r15" register. */
    /* 32-bit general purpose */
    kEAX,  /**< The "eax" register. */
    kECX,  /**< The "ecx" register. */
    kEDX,  /**< The "edx" register. */
    kEBX,  /**< The "ebx" register. */
    kESP,  /**< The "esp" register. */
    kEBP,  /**< The "ebp" register. */
    kESI,  /**< The "esi" register. */
    kEDI,  /**< The "edi" register. */
    kR8D,  /**< The "r8d" register. */
    kR9D,  /**< The "r9d" register. */
    kR10D, /**< The "r10d" register. */
    kR11D, /**< The "r11d" register. */
    kR12D, /**< The "r12d" register. */
    kR13D, /**< The "r13d" register. */
    kR14D, /**< The "r14d" register. */
    kR15D, /**< The "r15d" register. */
    /* 16-bit general purpose */
    kAX,   /**< The "ax" register. */
    kCX,   /**< The "cx" register. */
    kDX,   /**< The "dx" register. */
    kBX,   /**< The "bx" register. */
    kSP,   /**< The "sp" register. */
    kBP,   /**< The "bp" register. */
    kSI,   /**< The "si" register. */
    kDI,   /**< The "di" register. */
    kR8W,  /**< The "r8w" register. */
    kR9W,  /**< The "r9w" register. */
    kR10W, /**< The "r10w" register. */
    kR11W, /**< The "r11w" register. */
    kR12W, /**< The "r12w" register. */
    kR13W, /**< The "r13w" register. */
    kR14W, /**< The "r14w" register. */
    kR15W, /**< The "r15w" register. */
    /* 8-bit general purpose */
    kAL,   /**< The "al" register. */
    kCL,   /**< The "cl" register. */
    kDL,   /**< The "dl" register. */
    kBL,   /**< The "bl" register. */
    kAH,   /**< The "ah" register. */
    kCH,   /**< The "ch" register. */
    kDH,   /**< The "dh" register. */
    kBH,   /**< The "bh" register. */
    kR8L,  /**< The "r8l" register. */
    kR9L,  /**< The "r9l" register. */
    kR10L, /**< The "r10l" register. */
    kR11L, /**< The "r11l" register. */
    kR12L, /**< The "r12l" register. */
    kR13L, /**< The "r13l" register. */
    kR14L, /**< The "r14l" register. */
    kR15L, /**< The "r15l" register. */
    kUnsupportReg,
} RegisterType;

}

#endif //_REGISTER_H_
