#!/bin/bash
CAR_ID="HSR"

function error_msg () {
    echo -e "\033[1;5;40;31m${1}\033[0m"
}

function mkdir_data_dir() {
    if [ `echo ${CAR_ID}|wc -L` -lt 3 ]; then
        error_msg "carid长度过小. carid:${CAR_ID}"
        exit -1;
    fi

    local PROJ_ID=""
    if [ ${CAR_ID:0:3} == "HSR" ]||[ ${CAR_ID:0:3} == "HSO" ]||[ ${CAR_ID:0:3} == "HDR" ]||[ ${CAR_ID:0:3} == "HDO" ]; then
        PROJ_ID="HD"
    elif [ ${CAR_ID:0:3} == "VV7" ]||[ ${CAR_ID:0:3} == "V71" ]; then
        PROJ_ID="icu30"
    else
        error_msg "错误的carid. carid:${CAR_ID}"
        exit -1;
    fi
    echo ${PROJ_ID}
    return $?
}

mkdir_data_dir