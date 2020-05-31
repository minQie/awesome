#!/bin/bash

DATABASE_NAME='shangtang'
BACKUP_PATH='/var/stxcx/sql'
BACKUP_NAME=${DATABASE_NAME}_`date +%Y-%m-%d_%H-%M-%S`.sql.gz

mysqldump -uroot -pShangtang2018 $DATABASE_NAME | gzip > $BACKUP_PATH/$BACKUP_NAME
echo 备份$BACKUP_PATH/$BACKUP_NAME成功!
