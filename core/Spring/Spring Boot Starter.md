# Spring Boot Starter

## 简介

- 如通用业务的异常处理、有要求的规范的接口数据格式（自定义处理器）等。如果每次开发都要进行重复的基建步骤，这样即无意义，不合理。所以我们应当实现一种将类似的模块进行整合，达到可插拔的插件效果。我们只需要一次性的实现，之后便能够根据需要，轻松的实现复用

- **横切关注点**：一些具有横越多个模块的行为

- 在Spring Boot中，用于表示提供这种横切关注点的模块术语是 **start** ，通过依赖 start 可以轻松使用其包含的一些功能特性，无论我们是否真的会构建自定义的 start，构建 start 的思想，我们都应该了解

- 宏观来看，start 由 Auto-Configure Module组件（包含自动配置类的 Maven 或 Gradle 模块） 和 Start组件（提供启动某个特性需要的所有依赖） 构成

```
Official：You may combine the auto-configuration code and the dependency management in a single module if you do not need to separate those two concerns
```

- 官方的 starter 都是以 spring-boot-starter 开头

## 实现机制

- Spring spi（<a href="#Spring AutoConfigure">见下边</a>）
- @Import(XxxSelector)（不贴示例了）

# Spring AutoConfigure

## 简介

​	Spring 中按条件将类交给 IOC 容器的核心是 Condition接口 和 Conditional注解，Spring 中其实还有一种 AutoConfigure 的机制也可以实现同样的效果，即 Spring spi 机制

## 示例

0. 启动该特性，需要在配置类上添加 EnableAutoConfiguration 注解，当然 SpringBootApplication 注解已经聚合了该注解

 1. 我们在 xxx.xxx.xxx 包下创建一个类，名为 XxxAutoConfiguration，定义一个 @Bean 修饰的方法，方法假如返回一个 new Xxx() 

 2. 在 Xxx 的构造函数中添加一条日志输出，用于判定类是否有被加载

 3. 然后根据，sspi 的机制，在项目的 `MEAT-INF/spring.factories` 中添加如下内容

    ```
    org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
      xxx.xxx.xxx.XxxAutoConfiguration
    ```

4. 上述步骤根据 SpringBoot 实现的 spi 机制，搭建了一个简单的启动加载指定类的代码模块，接下来是体现 AutoConfigure 的地方，启动项目会自动生成 `META-INF/spring-autoconfigure-metadata.properties` ，里面是类似如下的内容

   ```
   xxx.xxx.xxx.XxxAutoConfiguration.ConditionalOnClasss=xxx.xxx.Xxx
   ```

5. 实际测试

   人为修改这个文件并不能起到影响bean是否被加载的效果

## 实际作用

```
Official：Spring Boot uses an annotation processor to collect the conditions on  auto-configurations in a metadata file  (META-INF/spring-autoconfigure-metadata.properties). If that file is  present, it is used to eagerly filter auto-configurations that do not match, which will improve startup time.
```

​	对于所有自动配置类，Spring boot 必须计算 `@Condition...` 条件值，用于决定是否加载自动配置及其所需的所有类，根据 Spring 启动应用程序中 starter 的大小和数量，这可能是一个非常昂贵的操作，这就是影响启动时间的地方

## 补充

- 依赖

  ```
  <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-autoconfigure-processor</artifactId>
      <optional>true</optional>
  </dependency>
  ```

- 作用

  ​	如果引入了上面的依赖，并且 File → Settings → Build, Execution, Deployment → Annotation Processors → ☑ Enable annotation processing，当 mvn compile 时，会生成 `MEAT-INF/spring-autoconfigure-metadata.properties` 的文件。Spring boot 项目在启动期间会去读取生成的生成的 properties 文件，可以直接过滤出不满足条件的配置，就不用实际检查这些类，提升启动速度。

# 配置处理器

- 依赖

  ```
  <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-configuration-processor</artifactId>
      <optional>true</optional>
  </dependency>
  ```

- 作用（IDEA友好提示）

  如果引入了上面的依赖，并且 File → Settings → Build, Execution, Deployment → Annotation Processors → ☑ Enable annotation processing。当我们 mvn compile 时，会在生成 `META-INF/spring-configuration-metadata.json` 的文件。编辑 application.yml 文件，配置属性时，IDE 会去读取生成的 json 文件，从而给出友好的提示

- 实际问题

  Cannot resolve configuration property 'xxx.xxx.xxx' 