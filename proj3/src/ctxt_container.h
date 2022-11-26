/*
 *  Copyright (c) 2020-2021 Xuhpclab. All rights reserved.
 *  Licensed under the MIT License.
 *  See LICENSE file for more information.
 */

#ifndef _CTXT_CONTAINER_H_
#define _CTXT_CONTAINER_H_

#include <cstdint>
#include <vector>

#define context_handle_t int32_t
namespace runtime_profiling {
class CtxtContainer {
public:
    void
    addCtxt(context_handle_t ctxt)
    {
        ctxtList_.push_back(ctxt);
    }
    std::vector<context_handle_t>
    getCtxtList()
    {
        return ctxtList_;
    }
private:
    std::vector<context_handle_t> ctxtList_;
};
}

#endif //_CTXT_CONTAINER_H_