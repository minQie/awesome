@echo off
title 注册
echo 请右键选择“以管理员模式运行”
echo 请输入您将“FileTool.dll”复制到了哪个^<文件夹^>
set /p a=
set file="%a%\FileTool.dll"
regsvr32 %file%
