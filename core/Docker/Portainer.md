# Portainer

## 官方

https://www.portainer.io/installation/

## 简介

一款非常优秀的Docker GUI，完整将一个docker服务的内容展示出来，镜像、容器、数据卷等

## 安装

```docker command
docker run --name portainer \
-p 9001:9000 \
-v /opt/portainer/docker.sock:/var/run/docker.sock \
-d portainer/portainer
```

像上面这样是不行的，需要向下面这样将sock文件暴露到宿主机中（这是强制的），原因见下边，实际安装脚本应该像下边这样

```
docker volume create portainer_data
```

```
docker run --name portainer \
-p 8999:9000 \
-v /var/run/docker.sock:/var/run/docker.sock \
-v portainer_data:/data \
-d  portainer/portainer
```

上面这个是什么操作，创建一个数据卷？原因见下边

## 使用

执行好指令，只要通过浏览器访问相应的端口就可以看到Portainer的界面了

1. 首先会让你初始化用户名和密码

2. 进入界面后，当然选择Local，然后你就可以看到

   ```
   Manage the Docker environment where Portainer is running.
   
   Ensure that you have started the Portainer container with the following Docker flag:
   
   -v "/var/run/docker.sock:/var/run/docker.sock"（Linux）
   
   or
   
   -v \\.\pipe\docker_engine:\\.\pipe\docker_engine（Windows）
   ```

   （这个就是上面要将docker.sock映射出来的原因）

3. 既然可以选择`Local`，那意思是还可以连接其他服务器的Docker服务咯

   当然，点击菜单项的`Endpoints`，这体现了Portainer优秀且理所当然的设计

## 其他



