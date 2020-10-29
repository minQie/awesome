# Ebean

## Who Created Modified

- **官网提示**

1. 创建一个`io.ebean.config.CurrentUserProvider`实现类

   官网坑，给的例子不对，但也有说例子实现不起作用，实际是需要返回一个数据表实体的

2. `ebean.properties`中配置`ebean.currentUserProvider=实现类完整类名`

- **原理**

1. Ebean会根据`ebean.properties`初始化`ServerConfig`？

   **答**：会，但需要在`EbeanFactoryBean`中显示调用`ebeanConfig.loadFromProperties()`（官网没有说到这个）

   **验证**：`io.ebean.config.properties.LoadContext   : loaded properties from [application.yaml, ebean.properties]`

2. 如何修改叶飞框架源码，去手动调用方法？

   **答**：手动创建一个自己的实现，然后排除掉框架中的实现`@ComponentScan`注解指定`excludeFilters  = {@ComponentScan.Filter(type = FilterType.ASSIGNABLE_TYPE, value = EbeanFactoryBean.class)}`

   **解释**：Ebean构建配置环境是依赖`EbeanServer`的实现（为容器注入一个EbeanServier，这里面还有Spring的知识，实际是注入了`FactoryBean<EbeanServer>`的实现类）。不像2.9.2版本的Swagger，所有类都写死，没有任何内部文档构成插件的拓展

3. 在主配置文件中配置步骤2，是否起作用？

   **答**：不起作用

- **拓展**

  可以不创建`ebean.properties`来指定`currentUserProvider`，首先肯定要创建`CurrentUserProvider`实现类，然后将实现类注入自定义的`FactoryBean<EbeanServer>`实现类，显示调用`config.setCurrentUserProvider(xxx)`
