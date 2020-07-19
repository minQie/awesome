#!/bin/bash

# 1、进入到"/var/stxcx/jar"目录下
# 2、上传要更新的stxcx-xxxxxxxx-xx.jar包到目录下
# 3、通过命令"sh ./shangtang.sh 上传的jar的名称"运行脚本
# 作用：kill掉之前运行的stxcx-xxxxxxxx-xx.jar，运行刚上传的jar

# 进程名
PROCESS_NAME=stxcx
# JAR包所在目录
JAR_PATH=/var/stxcx/jar
# JAR包名称（参数）
JAR_NAME=$1

# 1、参数个数校验
if [ $# == 0 ]; then
   echo -e "执行失败，请带上参数指示要运行的jar！\nexit..."
   exit
fi
if [ $# != 1 ]; then
   echo -e "执行失败，参数个数非法！\nexit..."
   exit
fi

# 2、参数文件是否存在校验
if [ ! -e "$JAR_PATH/$JAR_NAME" ]; then
   echo -e "执行失败，jar包不存在！\nexit..."
   exit
fi

# 3、查询进程id
PID=`ps -ef | grep stxcx.jar | grep -v grep | grep -v $0 | awk '{print $2}'`

# 4、查询到进程个数校验
if [ "$PID" == "" -o ${#PID[@]} == 0 ]; then
   echo -e "执行失败，没有找到相关进程！\nexit..."
   exit
fi
if [ ${#PID[@]} -gt 1 ]; then
   echo "找到的相关进程数量大于1！"
fi

# 5、查询进程名
PNAME=`ps -ef | grep stxcx.jar | grep -v grep | grep -v $0 | awk '{for(i=8; i<=NF; i++) {printf $i" "}}'`
echo -e "√ 查询得到的进程信息：\nPID：$PID\nPNAME：$PNAME"

while true
do
	# 结束进程前询问
	echo "$PID"
	read -r -p "Sure to kill ↑ ? [Y/n] " input

	case $input in
	    [yY][eE][sS]|[yY])
			echo "Yes"
			break
			;;

	    [nN][oO]|[nN])
			echo -e "No\nexit..."
			exit	       	
			;;

	    *)
			echo "Invalid input..."
			;;
	esac
done
# 结束进程
kill -9 $PID
echo -e "√ 结束进程：\nkill -9 $PID"

# 修改文件权限
cd $JAR_PATH 
chmod 755 $JAR_NAME.jar

# 运行jar
BUILD_ID=dontKillMe nohup java -jar $JAR_NAME.jar >/dev/null 2>&1&
echo "$JAR_NAME.jar启动，执行成功！"
