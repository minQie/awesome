# 热部署

## 简介

​	开发人员在日常的开发中，经常需要修改一小块的局部代码，然后进行测试，为了让修改生效，就需要重启当项目。当项目达到一定规模时，每次重启项目的时间成本就不能忽视了，故引出热部署。

​	本文介绍一款IntelliJ的收费插件，来实现热部署。当然，Idea用的都是盗版，采用其他渠道激活，插件自然更不可能花费去使用正版，也采用激活的方式。

## 插件

* JRebel and XRebel for IntelliJ

## 激活

**说明**

通过 Github 上一个开源项目，原理是本地启动一个反向代理服务，来代理**插件的激活服务器**



链接：https://github.com/ilanyu/ReverseProxy/releases

本地Win10环境，下载最新版本的`ReverseProxy_windows_amd64.exe`

---

1. 双击运行`ReverseProxy_windows_amd64.exe`

---

2. Help → JRebel → Activition

   ☑ **Team URL（connect to online licesing service）**

   **Team URL（required）**：http://127.0.0.1:8888/d3545f42-7b88-4a77-a2da-5242c46d4bc2

   **Email address（required）**：test@123.com

   （瞎填就行，直接使用上面的配置也是可以的）

---

3. Setting → JRebel → Work offline（离线模式）

   必须要切换至离线模式，可离线180天，可随时重新点下“Renew Offline Seat”刷新激活周期

---

4. File → Settings → Compiler → ☑ Build project automatically

   JRebel是实时监控class文件的变化来实现热部署的，所以在idea环境下需要打开自动编译功能才能实现随时修改，随时生效

---

5. Ctrl+Shift+Alt+/ → compiler.automake.allow.when.app.running ☑
6. 通过 JRebel 插件提供的 Run 和 Debug 按钮去运行项目

