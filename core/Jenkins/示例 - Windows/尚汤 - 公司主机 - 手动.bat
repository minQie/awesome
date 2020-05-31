@echo off
setlocal enableDelayedExpansion
title 尚汤测试服务
set port=8081
set index=0
set killed_pid_array=
for /f "tokens=1-5" %%a in ('netstat -ano ^| find ":%port%"') do (

    :: pid校验
    set normal=1
    if "%%e" == "" (
        set normal=0
        goto end_of_loop
    )
    if %%e EQU 0 (
        set normal=0
        goto end_of_loop
    )

    :: 合法pid赋值
    set pid=%%e

    :: pid是否在已杀进程的数组中
    set flag=0
    for %%i in (!killed_pid_array[%index%]!) do (
        if %%i == !pid! (
            echo 失败：已经终止过了 PID 为 !pid! 的进程
            set flag=1
        )
    )

    :: 不在-杀进程、存数组
    if !flag! == 0 (
        taskkill /f /pid !pid!
        set killed_pid_array[!index!]=!pid!
        set /a index+=1
    )

    :: pid非法处理
    :end_of_loop
    if !normal! == 0 (
        echo invalid pid: !pid!, loop continue......
    )
)

if !index! == 0 (
    echo 没有找到运行在!port!端口上的进程
)

:: 进入到git仓库下
d:
cd D:\software\Jenkins\workspace\尚汤

:: 拉取最新的代码
call git pull origin 线上

:: 重新打包
call mvn clean package -Dmaven.test.skip=true

:: 启动新的jar包
start java -jar .\target\stxcx.jar --server.port=!port!

exit