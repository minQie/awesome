#  运行命令

## 前台运行（java命令）

`java -jar xxx.jar`

## 前台运行（Springboot的maven插件）

`mvn spring-boot:run`

- 跳过测试

  生命周期`mvn clean package`跳过test阶段

  **配置式1**：在pom.xml中的properties标签下添加`<skipTests>true</skipTests>`
  （但会编译测试类，在target/test-classes目录下生成相应的class）

  **配置式2**：在pom.xml中的properties标签下添加`<maven.test.skip>true</maven.test.skip>`

  **参数式1**：`mvn clean package -DskipTests`
  （但会编译测试类，在target/test-classes目录下生成相应的class）

  **参数式2**：`mvn clean package -Dmaven.test.skip=true`

- 参数

  例：`-Drun.arguments="--server.port=8888"`

  可以通过 `mvn spring-boot:help -Ddetail` 命令查看

  - 要在一个Springboot项目所在目录下执行
  - 输入指令后在 `spring-boot:run` 下边

## 后台运行

- Windows

  `javaw -jar xxx.jar`（java的原生后台运行命令 - Jenkins会提示构建失败）

  `start /B <命令>`

  `start "标题" cmd /C <命令>`（开启新窗口运行命令，运行完关闭窗口）

  `start "标题" cmd /k <命令>`（开启新窗口运行命令，运行完不关闭窗口）

- Linux

  `nohup <命令> >/dev/null 2>&1&`

# 命令参数

### 待办

--参数要在最后？-D参数要在运行命令的jar包前面？都指定的话谁的优先级高？

### 配置文件配置

- 默认配置文件的优先级
  1. 运行jar包所在目录/config/application.yml
  2. 运行jar包所在目录/application.yml
  3. classpath/config/application.yml
  4. classpath/application.yml

- 自定义配置文件

  ​	参数的形式指定：

​		`--spring.config.location=classpath:/default.properties,classpath:/override.properties`

​		`-Dspring.config.location=D:\config\config.properties`

​			变向的参数形式指定：

​		`--spring.profiles.active=环境`

​		`-Drun.profiles=环境`

​		`-Dspring.profiles.active=环境（不生效）`

​			代码中指定：

​		配置类上的`@PropertySource(value={"file:config.properties"})`

### 端口配置

​	`--server.port=端口`

​	`-Dserver.port=端口`

​	`-Drun.jvmArguments="-Dserver.port=端口"`

​	`-Dspring-boot.run.arguments="--server.port=端口"`

### 其他配置

​	`-Dspring.datasource.username=数据库用户名`
​	`-Dspring.datasource.password=数据库密码`

# 增量更新

- 概念

  1. 通常，一个工程项目架构确定后，引入的jar包基本上不会变，改变的大部分是业务逻辑；

  2. 后面如果需要变更业务逻辑，只需要轻量地编译工程，大大提高项目部署的效率

- 导出项目的lib

  ```
  mvn dependency:copy-dependencies -DoutputDirectory=/xxx/.../lib -DincludeScope=runtime
  ```

- 构建出不含lib的jar包

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

- 运行jar包时指定lib

  ```
  -Dloader.path=/xxx/.../lib
  ```



