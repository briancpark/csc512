/*
 *  Copyright (c) 2020-2021 Xuhpclab. All rights reserved.
 *  Licensed under the MIT License.
 *  See LICENSE file for more information.
 */

#ifndef _PROFILER_H_
#define _PROFILER_H_

#include "drcctlib.h"
#include "instruction.h"
#include "ctxt_container.h"

namespace runtime_profiling {

void
OnAfterInsExec(Instruction *instr, context_handle_t contxt, uint64_t flagsValue,
                        CtxtContainer *ctxtContainer);

void
OnBeforeAppExit(CtxtContainer *ctxtContainer);

}
#endif //_PROFILER_H_