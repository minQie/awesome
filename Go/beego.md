# beego

> 官方文档：https://beego.me/

## 简介

beego 是一个快速开发 Go 应用的 HTTP 框架，他可以用来快速开发 API、Web 及后端服务等各种应用，是一个 RESTful  的框架，主要设计灵感来源于 tornado、sinatra 和 flask 这三个框架，但是结合了 Go  本身的一些特性（interface、struct 嵌入等）而设计的一个框架

## 快速开始

1. 说明

   安装和下载 beego 和 Bee，默认会生成一个 `%GOPATH%\bin\bee.exe`
   
   （Go 语言的 GOPATH 是可以配置多个的，默认选择第一个，下面都是同理，就不会再赘述了）
   
2. 安装更新 beego 和 Bee

   ```shell
   go get -u github.com/beego/bee
   go get -u github.com/astaxie/beego
   ```

   **问题**：可能是获取该框架的相关信息，需要科学上网，现象就是执行 `go get github.com/astaxie/beego` 控制台就卡住，并在达到超时时间后，报出相关的错误信息

   **解决**：需要像下边一样配置一下代理（也有说直接上 github 将相关代码下载下来也行）

   > 代理：https://goproxy.cn/

   ```shell
   # 这个参数并不只是简单的之和代理相关，详见：https://blog.csdn.net/u011897301/article/details/105758056/
   go env -w GO111MODULE=on
   go env -w GOPROXY=https://goproxy.cn,direct 
   ```

3. 使用 bee 快速创建项目

   [bee new](#bee 命令介绍)

   [bee api](#bee 命令介绍)

4. 运行（类比 Java 的 Tomcat）

   ```bash
   # 完整命令如下
   
   # 创建项目
   bee new 项目名
   
   # 将 %GOPATH%/src/项目名 拷贝到 自定义目录下
   cd 自定义目录\项目名
   
   # 运行
   bee run
   ```

5. 访问 `http://127.0.0.1:8080` 有响应

   beego 默认包含热部署的概念（得通过 bee run 命令运行项目，才有这个特性），例如，修改项目目录下的 default.go，可以看到控制台输出相关日志

## bee 命令介绍

**new**

​	`bee new 项目名`：快速创建一个 Web 项目

**api**

​	`bee api 项目名`：快速创建一个 Api 应用

​	上述命令默认还支持一些参数，并且默认会在 `%GOPATH%\src` 生成项目代码；具体参数、默认的目录结构见 https://beego.me/docs/install/bee.md

**pack**

​	只要执行 `bee pack` 就可以打包，然后，上传到服务器解压、更新

**version**

​	`bee version` 可以查看 bee、beego、go 的版本

**其他命令**

​	**generate**、**migrate**、**dockerize**

## 路由

​	基本概念，见官网

​	helloworld 见 hellobeego 项目



