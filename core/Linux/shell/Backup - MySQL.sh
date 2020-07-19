#!/bin/bash

DATABASE_NAME='shangtang'
BACKUP_PATH='/var/stxcx/sql'
BACKUP_NAME=${DATABASE_NAME}_`date +%Y-%m-%d_%H-%M-%S`.sql.gz

# -- 方式一 --
#mysqldump -uroot -pShangtang2018 $DATABASE_NAME | gzip > $BACKUP_PATH/$BACKUP_NAME
#echo 备份$BACKUP_PATH/$BACKUP_NAME成功!
# 上面的脚本由于在命令行中显示输入了密码，因此每次执行都会有警告，可以用下面脚本的方式来解决这个

# -- 方式二 --
export MYSQL_PWD=Shangtang2018
mysqldump -uroot $DATABASE_NAME | gzip > $BACKUP_PATH/$BACKUP_NAME
echo 备份$BACKUP_PATH/$BACKUP_NAME成功!
