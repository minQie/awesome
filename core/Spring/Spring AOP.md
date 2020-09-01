# IOC

- 介绍

  ​	IOC（Inversion of Control），译为**控制反转**，是一种设计思想或者说某种模式，将你设计好的对象交给专门管理的容器进行管理（不限对象，又如文件等其他资源），而不是传统的在你的对象内部直接控制

  ​	从Spring框架的角度说，这是 Spring的核心，贯穿始终，Spring IOC 就是由 Spring 容器来负责管理对象的生命周期以及对象之间的关系

- 带来的好处

  ​	它很好的体现了面向对象设计法则之一，**好莱坞法则**（别找我们，我们找你），它能指导我们如何设计出松耦合、更优良的程序

**控制什么**

​	控制了资源的创建获取、生命周期的管理；传统Java SE程序设计，我们直接在对象内部通过 new 关键字创建对象，是程序主动去创建依赖对象

**什么反转**

​	对象的获取反转了；由原来的自己创建获取对象，这是正转；变为从IOC容器中获取，由IOC容器提供给你使用，IOC负责管理对象的生命周期，这是反转

**依赖注入**

​	控制反转时IOC容器将对象提供给你，但是如何给你，是通过依赖注入给你（方式：构造器注入、Setter注入、字段注入）

# DI

> 说到 IOC 不得不提及 DI（Dependency Injection），即依赖注入，DI 和 IOC 只是同一个概念，不同角度的描述，当然，你也可以说DI是IOC一种最合理的实现

## 注入条件

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

  2. 所以上述注解修饰类的话，配合如`@Controller`、`@Component`、`@Service`、`@Bean`、`@Configuration`等注解使用；如果上述注解修饰方法的话，在配置类中（`@Configuration`修饰的类）bean定义方法上使用的话，配合`@Bean`注解使用。你都可以自定义实现`Condition`接口的类作为自定义条件，接口定义的方法会回调给你`ConditionContext`，通过`ConditionContext.getEnvironment().getProperty()`就可以通过配置文件来决定一些类、策略是否生效（当然配置文件的话，也可以直接使用@`ConditionalOnProperty`）

## 注入时机

- 按顺序如下

1. 构造方法
2. `@PostConstruct`修饰的方法
3. `afterPropertiesSet`方法（Bean实现了`InitializingBean`）
4. `@Bean(initMethod="方法名")` 

[详解博客]: https://www.cnblogs.com/april-chen/p/8182631.html

# Spring AOP

## 参考文档

[Official](https://docs.spring.io/spring/docs/5.2.0.BUILD-SNAPSHOT/spring-framework-reference/core.html#aop)

[Spring aop](https://mp.weixin.qq.com/s?__biz=MzA5MTkxMDQ4MQ==&mid=2648934977&idx=1&sn=8e4caf6a17bf5e123884df81a6382214&chksm=8862127fbf159b699c4456afe35a17f0d7bed119a635b11c154751dd95f59917487c895ccb84&scene=126&sessionid=1593267278&key=55850329b0c39edb61ab5593f0fcf6a504178b085e8334b5ad0e355bbf9aca72761750c4d854261ea0d7c5f0eacf7f3915e87a8cb1f1efe0c37a3b7fe7e619700b6e7f60dcdae21fb344da3ef34b626e&ascene=1&uin=MjQzMjg2NTAzMA%3D%3D&devicetype=Windows+10+x64&version=62090070&lang=zh_CN&exportkey=AQCmXehePa1UkvX9EI8h6Fo%3D&pass_ticket=aOiZ1es1ZWVM2ZnqIV5Vv7dvJ0VUCdumm%2F83%2Fj2vRs%2FQEBDAjgbqLnfLI2nLkeQf) 

[Aspect](https://mp.weixin.qq.com/s?__biz=MzA5MTkxMDQ4MQ==&mid=2648935466&idx=2&sn=f536d7a2834e6e590bc7af0527e4de1f&chksm=88621414bf159d02c146e3f358ea6874b39030871359c7eb3efd9c42d8cef1c241bd670d3237&scene=158#rd)

[PointCut](https://mp.weixin.qq.com/s?__biz=MzA5MTkxMDQ4MQ==&mid=2648935037&idx=2&sn=cf813ac4cdfa3a0a0d6b5ed770255779&chksm=88621243bf159b554be2fe75eda7f5631ca29eed54edbfb97b08244625e03957429f2414d1e3&scene=158#rd)

## 简介

AOP（Aspect oriented programming ），译为面向切面编程， 是 OOP（面向对象编程）的一种延续。

例如，几乎所有Service方法都需要进行的事务控制管理、每个接口方法前必要的类如网关要做的登录校验以及权限校验、方法执行时间日志打印、接口方法的入参日志打印等等，像这样重复的代码，统称为**横切逻辑代码**，横切逻辑代码和业务代码混杂在一起，代码臃肿，不便维护。AOP 主要用于，在不改变原有业务逻辑的情况下，增强横切逻辑代码，从根本上解耦合，避免横切逻辑代码重复的问题。切面的面是指影响的方法如同多个点，这些点可以构成一个面

AOP的关键词如下：

| 名词      | 译        | 解释                                               |
| --------- | --------- | -------------------------------------------------- |
| Target    | 目标对象  | 被代理对象                                         |
| Joinpoint | 连接点    | 目标对象中，所有可以增强的方法                     |
| Pointcut  | 切入点    | 目标对象中，已经增强的方法                         |
| Advice    | 通知/增强 | 增强功能的代码，例如管理事务中的开启事务和提交事务 |
| Weaving   | 织入      | 将**通知**应用到**连接点**的过程                   |
| Proxy     | 代理      | **织入**到**目标对象**之后，形成代理对象           |
| Aspect    | 切面      | **切入点**、**通知**                               |

## 动态代理

说到AOP，肯定离不开动态代理（jdk：接口实现、cglib：继承）

## Advice

这里先科普一下interceptor，springmvc interceptor和spring interceptor

- springmvc interceptor（HandlerInterceptor）

  继承关系：HandlerInterceptor

  平时提及的拦截器实际上就是springmvc interceptor，这是定义用于拦截请求的拦截器

- spring interceptor（MethodInterceptor）

  继承关系：Advice → Interceptor → MethodInterceptor

  而spring interceptor则是一个全局概念，不限于接口方法。

前者先于后者执行；下文提到的interceptor指的是spring interceptor

---

纵观一个inteceptor的定义，完成符合Advice的概念，并且父级接口中就有`Advice`，所以我们应当将interceptor理解为一种能够简化Advice实现的方式

## 实例

### 定义方式一

原理：@Aspect + @Component

- 切点概念（给的就是注解增强的例子）：见`mine`项目的`priv.wmc.module.aop.AopAnnotationServiceMethodAspect`

​																																																																																																																																																																																																																															

### 定义方式二

原理：MethodInterceptor + 在配置类中注入配置bean（同定义Filter一样）

```java
public class XxxInterceptor implements MethodInterceptor {

    @Override
    public Object invoke(MethodInvocation invocation) throws Throwable {
        return invocation.proceed();
    }

}
```

```java
@Configuration
public class XxxConfiguration {
    
    public static final String POINT_CUT_EXPRESSION = "execution(* priv.wmc.service.*Service.*(..))";

    @Bean
    public DefaultPointcutAdvisor defaultPointcutAdvisor2() {
        DefaultPointcutAdvisor advisor = new DefaultPointcutAdvisor();

        // 设置切点
        AspectJExpressionPointcut pointcut = new AspectJExpressionPointcut();
        pointcut.setExpression(POINT_CUT_EXPRESSION);
        advisor.setPointcut(pointcut);

        // 设置通知
        XxxInterceptor xxxInterceptor = new XxxInterceptor();
        advisor.setAdvice(xxxInterceptor);

        return advisor;
    }
}
```

- 切点概念增强

  可以自定义一个注解，基于上例来说，可以按照如下步骤来做

  1、在`XxxInterceptor.invoke`中判断：拦截的方法上是否有自定义注解，来决定是否跳过增强逻辑

  （当前直接在切点表达式前加上`@annotation(priv.wmc.module.aop.Aop) &&` ，也可以达到同样的效果）
  
  2、扩大DefaultPointcutAdvisor的范围

> 参考：https://mp.weixin.qq.com/s/e8GhmEQ_RJbA7Dc03PZi8Q