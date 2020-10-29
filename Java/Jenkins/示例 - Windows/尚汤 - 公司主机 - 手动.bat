@echo off
setlocal enableDelayedExpansion
title �������Է���
set port=8081
set index=0
set killed_pid_array=
for /f "tokens=1-5" %%a in ('netstat -ano ^| find ":%port%"') do (

    :: pidУ��
    set normal=1
    if "%%e" == "" (
        set normal=0
        goto end_of_loop
    )
    if %%e EQU 0 (
        set normal=0
        goto end_of_loop
    )

    :: �Ϸ�pid��ֵ
    set pid=%%e

    :: pid�Ƿ�����ɱ���̵�������
    set flag=0
    for %%i in (!killed_pid_array[%index%]!) do (
        if %%i == !pid! (
            echo ʧ�ܣ��Ѿ���ֹ���� PID Ϊ !pid! �Ľ���
            set flag=1
        )
    )

    :: ����-ɱ���̡�������
    if !flag! == 0 (
        taskkill /f /pid !pid!
        set killed_pid_array[!index!]=!pid!
        set /a index+=1
    )

    :: pid�Ƿ�����
    :end_of_loop
    if !normal! == 0 (
        echo invalid pid: !pid!, loop continue......
    )
)

if !index! == 0 (
    echo û���ҵ�������!port!�˿��ϵĽ���
)

:: ���뵽git�ֿ���
d:
cd D:\software\Jenkins\workspace\����

:: ��ȡ���µĴ���
call git pull origin ����

:: ���´��
call mvn clean package -Dmaven.test.skip=true

:: �����µ�jar��
start java -jar .\target\stxcx.jar --server.port=!port!

exit