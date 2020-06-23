#!/bin/bash
if [ ! $GIT_PREVIOUS_SUCCESSFUL_COMMIT ];then
    echo "GIT_PREVIOUS_SUCCESSFUL_COMMIT is not exists."
    exit 0
else
    echo "GIT_COMMIT=[$GIT_COMMIT],GIT_PREVIOUS_SUCCESSFUL_COMMIT=[$GIT_PREVIOUS_SUCCESSFUL_COMMIT]"
    # 核心的是否有新提交的判断
    if [ $GIT_PREVIOUS_SUCCESSFUL_COMMIT == $GIT_COMMIT ];then
        echo "GIT_COMMIT is equals to GIT_PREVIOUS_SUCCESSFUL_COMMIT,skip build."
        exit -1
    else
    	echo "GIT_COMMIT is not equals to GIT_PREVIOUS_SUCCESSFUL_COMMIT"
        exit 0
    fi
fi