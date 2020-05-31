@echo off&setlocal enableDelayedExpansion

title: 北京地铁

echo 运行本脚本须知：

:: nginx目前配置代理9003端口，转发到9000端口的后台
echo 1、脚本中配置的项目运行端口，必须和nginx的配置对应，即9000！
echo 2、pid文件和jar包必须真实存在（当前脚本仅会对两个文件是否存在进行校验）
echo ------------------------------------------------------------------------------------------------

set PORT=9000
set CORE_DIR=C:\project\mes\jar\
set PID_FILE=!CORE_DIR!application.pid
set JAR_FILE=!CORE_DIR!ft-firstouch-mes-0.25.0-SNAPSHOT.jar

:: 1、校验文件存在
if not exist !JAR_FILE! (
    echo 失败，jar包:!JAR_FILE!不存在！
    goto end
)
if not exist !PID_FILE! (
    echo 失败，jar包:!PID_FILE!不存在！
    goto end
)

:: 2、杀死当前进程 - 没有获取到pid，就不杀进程了（不影响启动jar）
:: ps：ctrl + c jar，pid文件会被删除

:: 2.1、校验pid文件存在
if not exist !PID_FILE! (
    echo 失败，pid文件:!PID_FILE!不存在！
    goto skip_kill
)

:: 2.2、读取pid文件中的pid
set /p pid=<!PID_FILE!
if "!pid!" == "" (
    echo 失败，获取pid失败！
    goto skip_kill
)

:: 2.3、杀死进程
taskkill /f /pid !pid!
echo 杀死进程 pid: !pid!

:skip_kill

:: 3、启动新进程
:: 生成的pid文件和sql文件都没有特别制定目录的话，默认生成在运行命令所在的目录(关键是，不想spring可以配置pid文件的生成目录，辣鸡ebean没有关于设置sql生成目录的拓展)
cd !CORE_DIR!
:: java -jar -Dserver.port=9000 -Dfirstouch.ebean.ddl-generate=false -Dfirstouch.ebean.ddl-run=false -Dspring.pid.file=C:\project\mes\jar\application.pid C:\project\mes\jar\ft-firstouch-mes-0.25.0-SNAPSHOT.jar
java -jar -Dserver.port=!PORT! -Dfirstouch.ebean.ddl-generate=true -Dfirstouch.ebean.ddl-run=false -Dspring.pid.file=!PID_FILE! !JAR_FILE!
echo 运行新jar包成功！

:end
exit