#! /bin/bash

# **********************************************************
# Copyright (c) 2020-2021 Xuhpclab. All rights reserved.
# Licensed under the MIT License.
# See LICENSE file for more information.
# **********************************************************

CUR_DIR=$(cd "$(dirname "$0")";pwd)

DYNAMORIO_ROOT_PATH=$CUR_DIR/../DrCCTProf/dynamorio
DYNAMORIO_CLIENT_PATH=$DYNAMORIO_ROOT_PATH/clients

rm -rf $DYNAMORIO_CLIENT_PATH/drcctlib_integer_overflow