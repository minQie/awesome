@echo off
title ע��
echo ���Ҽ�ѡ���Թ���Աģʽ���С�
echo ������������FileTool.dll�����Ƶ����ĸ�^<�ļ���^>
set /p a=
set file="%a%\FileTool.dll"
regsvr32 %file%
