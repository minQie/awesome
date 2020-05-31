# Springboot

## 注解聚合

- 概念

  不介绍，自行查看，以下示例

  - `@RequestMapping`到`@GetMapping`、`@PostMapping`、...

  - 叶飞整合的`@ApiController`

  - Spring的`@RestController`

- 注意

  本质不是注解替换，仅spring的注解支持

## 日志配置文件

- 概念

  默认的日志实现框架是Logback

- 问题

  如果在Springboot的主配置文件中不指定配置文件，或者没有没有在`resources`目录下创建`logback-spring.xml`，项目启动时会有日志`LOGBACK: No context given for c.q.l.core.rolling.SizeAndTimeBasedRollingPolicy@971608466`

## 文件生成目录

Springboot的pid文件、Ebean生成的sql文件不指定，默认为cmd运行命令时的所在目录

## Bean的生命周期

- 按顺序如下

1. 构造方法
2. `@PostConstruct`修饰的方法
3. `afterPropertiesSet`方法（Bean实现了`InitializingBean`）
4. `@Bean(initMethod="方法名")` 

[详解博客]: https://www.cnblogs.com/april-chen/p/8182631.html



## Bean整体生命周期

- 项目初始化中：`ApplicationContextAware`

- 容器启动完成：`ApplicationRunner`

- 所有bean都加载完成：`SmartLifeCycle`
  - 有时候我们需要在Spring加载和初始化所有bean后，接着执行一些任务或者启动需要的异步服务，这样我们可以使用 SmartLifecycle 来做到
  - SmartLifecycle 是一个接口。当Spring容器加载所有bean并完成初始化之后，会接着回调实现该接口的类中对应的方法（start()方法）

## Bean的值注入

1. springboot配置文件初始化注入，是根据Getter
2. 所以如果要初始化的是静态成员，需要手动编写Getter（lombok默认不会为static成员的变量生成Getter）
3. 如果要初始化的变量是自定义实体，自定义实体必须得有空参构造，并且提供Setter（经过测试）

## 请求

- 例1：POST提交表单
  - 方式：POST（contentType: application/x-www-form-urlencoded; charset=UTF-8）
  - 参数："param": "123"
  - Controller接口方法参数：String param
  - 注意不能使用`@RequestBody`修饰：请求的参数不在请求体中；即使在请求体中，parameterMap是会从请求体中拿数据的，springboot的方法形参注入就是从parameterMap取值注入，但是不使用`@RequestBody`读取的情况，无法自动将json转成自定义的参数实体

- 例2：PUT提交表单
  - 现象：读取不到
  - 结论：不要受用PUT方式提交表单

## Filter

- 定义方式

1. `@Component`+ `implements Filter`
2. `@ServletComponentScan` +`@WebFilter`+ `implements Filter`
3. `@Bean public FilterRegistrationBean getXxxFilterBean() {`
   	`FilterRegistrationBean bean = new FilterRegistrationBean();`
   	`bean.setFilter(new XxxFilter());`
   	`bean.set/addXxx(...);`
   	`...`
   	`return bean;`
   `}`

## Async

- **启用异步**

  配置类（`@Configuration`修饰）或者启动类上添加`@EnableAsync`

- **问题**

  类中的方法使用了异步`@Async`，如果该类显示实现了接口，那么项目在启动时会报出：

  ***************************
  `APPLICATION FAILED TO START`
  ***************************

  `Description:`

  `The bean 'mailService' could not be injected as a 'priv.wmc.service.MailService' because it is a JDK dynamic proxy that implements:`
  	`priv.wmc.service.EmailSender`


  `Action:`

  `Consider injecting the bean as one of its interfaces or forcing the use of CGLib-based proxies by setting proxyTargetClass=true on @EnableAsync and/or @EnableCaching.`

  意思是：这个类中有使用@Async注解修饰的需要异步调用的方法，并且这个类实现了接口（这个方法就是接口方法），而实现这个异步，springboot使用的是“JDK dynamic proxy”

- 解决

  方法一：将spring的动态代理策略由默认java标准库改为CGLIB（注解式和配置式都行）

  ​	`@EnableAsync(proxyTargetClass = true)`

  方法二（推荐）：修改`@EnableAsync`的`proxyTargetClass`属性，按照说明会影响整个spring框架，所以应该使用局部指定的方式，为类配置动态代理策略，即在上述报错的类上添加`@Scope(proxyMode = ScopedProxyMode.TARGET_CLASS)`