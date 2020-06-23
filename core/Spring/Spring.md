# Spring

## IOC、DI

- 介绍

  IOC（Inversion of Control），译为**控制反转**，将你设计好的对象交给专门管理的容器管理（不限对象，又如文件等其他资源），而不是传统的在你的对象内部直接控制，这不仅是一种技术，更是一种设计思想；从Spring框架的角度说，这是 Spring的核心，贯穿始终，Spring IOC 就是由 Spring 容器来负责管理对象的生命周期以及对象之间的关系。说到 IOC 不得不提及 DI（Dependency Injection），即依赖注入，DI 和 IOC 只是同一个概念，不同角度的描述

- 优点

  它很好的体现了面向对象设计法则之一“好莱坞法则（别找我们，我们找你）”，它能指导我们如何设计出松耦合、更优良的程序

- 核心

  Spring IOC 容器来负责对象的声明周期和对象之间的关系

- 名词解释

  **控制什么**

  ​	控制了资源的创建获取、生命周期的管理；传统Java SE程序设计，我们直接在对象内部通过 new 关键字创建对象，是程序主动去创建依赖对象

  **什么反转**

  ​	对象的获取反转了；由原来的自己创建获取对象对象，这是正转；变为由IOC容器提供给你使用，IOC负责管理对象的生命周期，这是反转

  **依赖注入**

  ​	控制反转时IOC容器将对象提供给你，但是如何给你，是通过依赖注入给你（方式：构造器注入、Setter注入、字段注入）

## DI

> 依赖注入的概念上面已经和AOP一同描述了，这里主要强调列举一下Spring对于依赖注入的时机和条件的相关拓展

### 注入条件

- 简介

  该项在开发中经常使用到，故需要引起相当的重视，并且需要系统学习

- 配合@Bean一同使用的注解，也就是`org.springframework.boot.autoconfigure.condition`包下的注解

  1. @ConditionalOnBean、@ConditionalOnMissingBean
  2. @ConditionalOnClass、@ConditionalOnMissingClass
  3. @ConditionalOnCloudPlatform
  4. @ConditionalOnExpression
  5. @ConditionalOnJava
  6. @ConditionalOnJndi
  7. @ConditionalOnNotWebApplication
  8. @ConditionalOnProperty
  9. @ConditionalOnResource
  10. @ConditionalOnSingleCandidate
  11. @ConditionalOnWebApplication

- 原理

  1. 以上的注解，通过观看源码会发现都有`@Conditional`修饰，并且就在上述注解的同级目录下就有，因为这些注解的元注解定义的`@Target({ ElementType.TYPE, ElementType.METHOD })`

  2. 所以上述注解修饰类的话，配合如@Controller、@Component、@Service、@Bean、@Configuration等注解使用；如果上述注解修饰方法的话，在配置类中（@Configuration修饰的类）bean定义方法上使用的话，配合@Bean注解使用。你都可以自定义实现`Condition`接口的类作为自定义条件，接口定义的方法会回调给你ConditionContext，通过ConditionContext.getEnvironment().getProperty()就可以通过配置文件来决定一些类、策略是否生效（当然配置文件的话，也可以直接使用@ConditionalOnProperty）

### 注入时机

- 按顺序如下

1. 构造方法
2. `@PostConstruct`修饰的方法
3. `afterPropertiesSet`方法（Bean实现了`InitializingBean`）
4. `@Bean(initMethod="方法名")` 

[详解博客]: https://www.cnblogs.com/april-chen/p/8182631.html

## AOP

- 概念

  横向重复，纵向抽取

- 实现

  死代码、静态代理、动态代理、Spring AOP、Spring AspectJ

- 说到AOP，肯定离不开动态代理（jdk：接口实现、cglib：集成）

- 概念
  
  Target（目标对象）：被代理对象
  Joinpoint（连接点）：目标对象中，所有可以增强的方法
  Pointcut（切入点）：目标对象中，已经增强的方法
  Advice（通知/增强）：增强功能的代码，例如管理事务中的开启事务和提交事务
  Weaving（织入）：将“通知”应用到“连接点”的过程
  Proxy（代理）：“织入”到目标对象之后，形成代理对象
  aspect（切面）：“切入点”+“通知”

## Junit Test

@RunWith(SpringJUnit4ClassRunner.class)

@ContextConfiguration("classpath:xxx.application.yaml")

## 容器生命周期

- 项目初始化中：`ApplicationContextAware`

- 容器启动完成：`ApplicationRunner`

- 所有bean都加载完成：`SmartLifeCycle`
  - 有时候我们需要在Spring加载和初始化所有bean后，接着执行一些任务或者启动需要的异步服务，这样我们可以使用 SmartLifecycle 来做到
  - SmartLifecycle 是一个接口。当Spring容器加载所有bean并完成初始化之后，会接着回调实现该接口的类中对应的方法（start()方法）

## 点

### 注解聚合

- 概念

  不介绍，自行查看，见以下示例

  - `@RequestMapping`到`@GetMapping`、`@PostMapping`、...

  - 叶飞整合的`@ApiController`

  - Spring的`@RestController`

- 注意

  本质不是注解替换，仅spring的注解支持

### 生成目录

Springboot的pid文件、Ebean生成的sql文件不指定，默认为cmd运行命令时的所在目录（这个应该理解为操作系统的行为）

## 坑

### 请求参数

- 例1：POST提交表单
  - 方式：POST（contentType: application/x-www-form-urlencoded; charset=UTF-8）
  - 参数："param": "123"
  - Controller接口方法参数：String param
  - 注意不能使用`@RequestBody`修饰：请求的参数不在请求体中；即使在请求体中，parameterMap是会从请求体中拿数据的，springboot的方法形参注入就是从parameterMap取值注入，但是不使用`@RequestBody`读取的情况，无法自动将json转成自定义的参数实体

- 例2：PUT提交表单
  - 现象：读取不到
  - 结论：不要受用PUT方式提交表单

## 日志框架

- 概念

  默认的日志实现框架是Logback

- 问题

  如果在Springboot的主配置文件中不指定配置文件，或者没有在`resources`目录下创建`logback-spring.xml`，项目启动时会有日志`LOGBACK: No context given for c.q.l.core.rolling.SizeAndTimeBasedRollingPolicy@971608466`

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