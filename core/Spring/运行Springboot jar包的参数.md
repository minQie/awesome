#  Java命令

1. **运行项目**

   `java -jar xxx.jar`

2. **指定参数示例**

   `-Dspring.datasource.username=数据库用户名`
   `-Dspring.datasource.password=数据库密码`
   `-Dserver.port=端口`

   `--server.port=端口`

   `-Dspring.profiles.active=环境（不生效）`

   `--spring.profiles.active=环境`

3. **后台运行**

   Windows

   - `javaw -jar xxx.jar`（java的原生后台运行命令 - Jenkins会提示构建失败）

   - `start /B <命令>`

   - `start "标题" cmd /C <命令>`（开启新窗口运行命令，运行完关闭窗口）

   - `start "标题" cmd /k <命令>`（开启新窗口运行命令，运行完不关闭窗口）

   Linux

   - `nohup <命令> >/dev/null 2>&1&`

4. **其他**

   “--”是命令行参数

   “-D”是JVM参数

# Maven命令

1. **运行项目**

   `mvn spring-boot:run`

2. **指定参数示例**

   `-Dspring-boot.run.arguments="--server.port=端口"`

   `-Drun.profiles=环境`

   `-Drun.jvmArguments="-Dserver.port=端口"`

3. **跳过测试**

生命周期`mvn clean package`跳过test阶段

- 配置式1：在pom.xml中的properties标签下添加`<skipTests>true</skipTests>`
  （但会编译测试类，在target/test-classes目录下生成相应的class）
- 配置式2：在pom.xml中的properties标签下添加`<maven.test.skip>true</maven.test.skip>`
- 参数式1：`mvn clean package -DskipTests`
  （但会编译测试类，在target/test-classes目录下生成相应的class）
- 参数式2：`mvn clean package -Dmaven.test.skip=true`

# 增量更新

## 概念

1. 通常，一个工程项目架构确定后，引入的jar包基本上不会变，改变的大部分是业务逻辑；

2. 后面如果需要变更业务逻辑，只需要轻量地编译工程，大大提高项目部署的效率

## 导出项目的lib

```
mvn dependency:copy-dependencies -DoutputDirectory=/xxx/.../lib -DincludeScope=runtime
```



## 构建出不含lib的jar包

在spring-boot-maven-plugin标签下添加如下配置

```
<configuration>
    <includes>
        <include>
            <groupId>nothing</groupId>
            <artifactId>nothing</artifactId>
        </include>
    </indludes>
</configuration>
```

运行jar包需要的参数

```
-Dloader.path=/xxx/.../lib
```