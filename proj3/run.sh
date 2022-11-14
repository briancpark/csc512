#! /bin/bash

# **********************************************************
# Copyright (c) 2020 Xuhpclab. All rights reserved.
# Licensed under the MIT License.
# See LICENSE file for more information.
# **********************************************************

CUR_DIR=$(cd "$(dirname "$0")";pwd)

echo "remake..."

MAKE_STATUS=0
MAKE_LOG_FILE=$CUR_DIR/logs/remake.log
$CUR_DIR/DrCCTProf/scripts/build_tool/remake.sh > /dev/null || MAKE_STATUS=-1
cp $CUR_DIR/DrCCTProf/logs/remake* $MAKE_LOG_FILE
if [[ $MAKE_STATUS == 0 ]]; then
    echo -e "\033[32m Remake successfully! \033[0m"
    echo "make test..."
    $CUR_DIR/scripts/make_tests.sh
else
    echo -e "\033[31m Remake fail! See \033[34m$MAKE_LOG_FILE\033[31m for detail \033[0m"
fi


