# Windows Terminal美化

## 0. 概念

官方：*https://docs.microsoft.com/zh-cn/windows/terminal/*

## 1. 打开Microsoft Store

​	搜索Windows Terminal安装

## 2. 打开PowerShell，执行如下命令（管理员身份）

```
Set-ExecutionPolicy Bypass
# 安装Oh-My-Posh
Install-Module posh-git -Scope CurrentUser
# 安装Posh-Git
Install-Module oh-my-posh -Scope CurrentUser
```

ps：有询问的，一律输入”y“

## 3. 继续执行命令

```
if (!(Test-Path -Path $PROFILE )) { New-Item -Type File -Path $PROFILE -Force }
notepad $PROFILE
```

ps：上面命令执行完，会弹出空的记事本，输入如下内容

```
Import-Module posh-git
Import-Module oh-my-posh
Set-Theme Paradox
```

## 4. 解决乱码（安装 Powerline 字体）

- 描述

  理想情况下，命令行行头的命令结尾会有一个可读性非常好的箭头（git仓库中也会有），本质是：调节文字的背景色、文件的颜色、以及一个右三角符号实现的。而实际右三角符号显现出来的是乱码

- 解决

  https://github.com/powerline/fonts（具体就是详见该github页面，下面是归纳）

  1. PowerShell（管理员身份）

     一定得是PowerShell，因为下面仓库拉取下来，支持windows的安装脚本是“ps1”后缀的

  2. 拉取

  ```
  git clone https://github.com/powerline/fonts.git --depth=1
  ```

  3. 安装

  ```
  cd fonts
  ./install.ps1
  ```

  本质：将`fonts`目录下的字体文件复制到`C:\Windows\Fonts`

## 5. 修改Windows Terminal配置

点击Windows Terminal浏览器标签的下三角、设置（弹出文件编辑）、将当前目录下的`Setting.json`文件中的配置内容复制替换

## 6. 变更主题

Get-Theme

Set-Theme <主题名称>

## 7. 实现同Git的右键“Git Bash Here”

右键 `.\WindowsTerminalHere-main\WindowsTerminalHere.inf`  → 安装

## 引用

Oh-My-Posh: *https://github.com/JanDeDobbeleer/oh-my-posh*

Posh-Git: *https://github.com/dahlbyk/posh-git*

Powerline：https://powerline.readthedocs.io/en/latest/installation.html#patched-fonts

Windows Terminal Themes: *https://atomcorp.github.io/themes*