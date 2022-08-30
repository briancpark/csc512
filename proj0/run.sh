#!/bin/bash

if [ -d "../DrCCTProf/build" ]
then
    ../DrCCTProf/scripts/build_tool/remake.sh
else
    ../DrCCTProf/build.sh
fi

$drrun -t drcctlib_instr_analysis -- ./p0_test_app
