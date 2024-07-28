#! /bin/bash

# **********************************************************
# Copyright (c) 2020 Xuhpclab. All rights reserved.
# Licensed under the MIT License.
# See LICENSE file for more information.
# **********************************************************

CUR_DIR=$(cd "$(dirname "$0")";pwd)
rm -rf $CUR_DIR/src/clients/*
mkdir $CUR_DIR/logs

$CUR_DIR/scripts/env_init.sh

$CUR_DIR/scripts/build_tool/env_init.sh

$CUR_DIR/scripts/build_tool/make.sh
cp $CUR_DIR/logs/cmake* $CUR_DIR/logs/cmake.log
cp $CUR_DIR/logs/make* $CUR_DIR/logs/make.log