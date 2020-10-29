@echo off&setlocal enableDelayedExpansion

title: ��������

echo ���б��ű���֪��

:: nginxĿǰ���ô���9003�˿ڣ�ת����9000�˿ڵĺ�̨
echo 1���ű������õ���Ŀ���ж˿ڣ������nginx�����ö�Ӧ����9000��
echo 2��pid�ļ���jar��������ʵ���ڣ���ǰ�ű�����������ļ��Ƿ���ڽ���У�飩
echo ------------------------------------------------------------------------------------------------

set PORT=9000
set CORE_DIR=C:\project\mes\jar\
set PID_FILE=!CORE_DIR!application.pid
set JAR_FILE=!CORE_DIR!ft-firstouch-mes-0.25.0-SNAPSHOT.jar

:: 1��У���ļ�����
if not exist !JAR_FILE! (
    echo ʧ�ܣ�jar��:!JAR_FILE!�����ڣ�
    goto end
)
if not exist !PID_FILE! (
    echo ʧ�ܣ�jar��:!PID_FILE!�����ڣ�
    goto end
)

:: 2��ɱ����ǰ���� - û�л�ȡ��pid���Ͳ�ɱ�����ˣ���Ӱ������jar��
:: ps��ctrl + c jar��pid�ļ��ᱻɾ��

:: 2.1��У��pid�ļ�����
if not exist !PID_FILE! (
    echo ʧ�ܣ�pid�ļ�:!PID_FILE!�����ڣ�
    goto skip_kill
)

:: 2.2����ȡpid�ļ��е�pid
set /p pid=<!PID_FILE!
if "!pid!" == "" (
    echo ʧ�ܣ���ȡpidʧ�ܣ�
    goto skip_kill
)

:: 2.3��ɱ������
taskkill /f /pid !pid!
echo ɱ������ pid: !pid!

:skip_kill

:: 3�������½���
:: ���ɵ�pid�ļ���sql�ļ���û���ر��ƶ�Ŀ¼�Ļ���Ĭ�������������������ڵ�Ŀ¼(�ؼ��ǣ�����spring��������pid�ļ�������Ŀ¼������ebeanû�й�������sql����Ŀ¼����չ)
cd !CORE_DIR!
:: java -jar -Dserver.port=9000 -Dfirstouch.ebean.ddl-generate=false -Dfirstouch.ebean.ddl-run=false -Dspring.pid.file=C:\project\mes\jar\application.pid C:\project\mes\jar\ft-firstouch-mes-0.25.0-SNAPSHOT.jar
java -jar -Dserver.port=!PORT! -Dfirstouch.ebean.ddl-generate=true -Dfirstouch.ebean.ddl-run=false -Dspring.pid.file=!PID_FILE! !JAR_FILE!
echo ������jar���ɹ���

:end
exit