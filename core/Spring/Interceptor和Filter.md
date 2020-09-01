# 引出

​	在面向对象编程的过程中，我们很容易通过继承、多态来解决纵向拓展。但是对于横向的功能，比如，在所有service方法中开启事务，统一记录日志等场景，面向对象的思想是无法解决的。所以面向切面编程是面向对象编程的一个补充。过滤器和拦截器都属于面向切面编程的具体实现

# Interceptor 

## 介绍

**实现原理**：动态代理 - java反射，详见`DispatcherServlet.doDispatch`

1. preHandle：请求处理之前被调用。如果方法返回值为false，则视当前请求结束，当前以及其他拦截器都不再执行。常见场景：预处理，编码、权限校验等

2. postHandle：请求处理之后，视图未渲染之前调用。注意，执行顺序越早的拦截器，该方法的执行顺序越靠后

3. afterCompletion：DispatcherServlet完全处理完请求后被调用，已经渲染视图。postHandle方法返回值为true才会执行

## 定义

1、定义方式一（实现HandlerInterceptor）

```java
/** 自定义拦截器定义 */
public class XxxInterceptor implements HandlerInterceptor {

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) {
        // 放行：继续执行其他拦截器、接口方法
        return true;
        // 不放行：直接结束
        return false;
    }
}
```

2、定义方式二（继承HandlerInterceptor的实现类）

一般是继承`HandlerInterceptorAdapter`

**注册生效**（只有一种方式）

```java
@Configuration
public class WebMvcConfiguration implements WebMvcConfigurer {

    /** 注册自定义拦截器 */
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry
            .addInterceptor(new Xxx1Interceptor())
            .addPathPatterns("/*")
            .excludePathPatterns("/login")
            .order(1);
        
        registry
            .addInterceptor(new Xxx2Interceptor())
            .addPathPatterns("/*")
            .excludePathPatterns("/login")
            .order(2);
    }

}
```

ps：同HandlerInterceptor接口，WebMvcConfigurer也是有实现类的

## 问题

如果你希望将一个拦截器交给Spring容器管理，从而可以使用**DI**特性，实际你会发现，你本希望注入的对象为null，原因是`拦截器`加载的时间点在`springcontext`之前，解决办法：

```java
@Configuration
public class WebMvcConfiguration implements WebMvcConfigurer {

    @Bean
    public XxxInterceptor getXxxInterceptor() {
        return new XxxInterceptor();
    }

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(getXxxInterceptor());
    }

}
```

调试断点你会发现，调用`getXxxInterceptor`方法时，实际是一个Spring代理调用的该方法，详见：`ConfigurationClassPostProcessor` 

# Filter

## 介绍

首先，Filter是Servlet规范定义的，以前的Servlet项目，注册让一个Filter生效都是在web.xml中，而Springboot项目中没有web.xml文件，所以需要`FilterRegistrationBean`来完成配置。常用场景：拦截指定的请求、修改请求和响应的内容、CORS跨域请求支持等

**实现原理**：函数回调，详见`ApplicationFilterChain.doFilter`方法

1. init：一个Filter的生命周期只会执行一次，该方法在容器启动初始化过滤器时被调用，方法必须执行成功否则过滤器不起作用
2. doFilter：
3. destroy：一个Filter的生命周期只会执行一次。该方法在容器销毁过滤器实例时调用该方法，一般用于关闭或者销毁资源

## 定义

1、定义方式一（Springboot配置式）

```java
public XxxFilter implements Filter {
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        // 前置处理
        chain.doFilter(request, response);
        // 后置处理
    }
}
```

注册

```java
@Bean
public FilterRegistrationBean getXxxFilterBean() {
    FilterRegistrationBean bean = new FilterRegistrationBean();
  	bean.setFilter(new XxxFilter());
  	bean.set/addXxx(...);
  	...
  	return bean;
}
```

2、定义方式二（Springboot纯注解式）

```java
@WebFilter(urlPatterns = "/api/*")
public XxxFilter implements Filter {
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        // 前置处理
        chain.doFilter(request, response);
        // 后置处理
    }
}
```

注册：在能扫描的到上面Filter所在包的配置类上添加`@ServletComponentScan`，详情参见该注解类上的注释

## 优先级

​	@WebFilter这个注解并没有指定执行顺序的属性，其执行顺序依赖于Filter的名称，是根据**Filter类名**的字母顺序倒序排列，@WebFilter配置的过滤器优先级都高于`FilterRegistrationBean`配置的过滤器

## 优势场景

​	一般来说，能使用Inteceptor的都尽量使用Inteceptor，但是若需要完成的代码逻辑需要结合两个阶段才能完成的话，那就得使用成员变量，就涉及到安全问题了，虽然此时可以使用ThreadLocal来解决这个问题，但是这个情况下Filter实现起来更为简单

# 总结

过滤器和拦截器均体现了AOP编程思想，都可以实现注入日志记录、登录鉴权等功能

**不同**

- 实现原理：回调、动态代理

- 使用范围：`javax.servlet.Filter` 接口是在`Servlet`规范中定义的，即过滤器`Filter` 的使用要依赖于`Tomcat`等容器，所以它只能在`web`程序中使用

- 触发时机

  Filter比Interceptor先触发，实际顺序：Tomcat → Filter → Servlet → Interceptor → Controller。

  项目启动过程中Filter的init方法就会被执行。

  我们向一个后端服务发起请求，通常第一次访问都会额外发送一个网站静态图标获取的请求（favicon.ico），Filter会执行两次，而Interceptor只会执行一次，过滤器几乎可以对所有进入容器的请求起作用，而拦截器只会对`Controller`中请求或访问`static`目录下的资源请求起作用。

- 控制执行顺序的方式

  Filter：@Order、registrationBean.setOrder

  Interceptor：InterceptorRegistration.order