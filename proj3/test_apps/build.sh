#! /bin/bash

CUR_DIR=$(cd "$(dirname "$0")";pwd)
SRC=${CUR_DIR}/src
BUILD=${CUR_DIR}/build
if [ ! -d ${BUILD} ]; then
    mkdir ${BUILD}
fi

echo -e "\033[32m Start build test apps... \033[0m"
gcc -g ${SRC}/test1.c -o ${BUILD}/test1
echo -e "\033[32m Build test apps successfully! \033[0m"