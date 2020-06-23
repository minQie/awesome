# Spring事务

## 事务管理方式

### 1. 编程式

```java
@Autowired
private TransactionTemplate transactionTemplate;
public void testTransaction() {

        transactionTemplate.execute(new TransactionCallbackWithoutResult() {
            @Override
            protected void doInTransactionWithoutResult(TransactionStatus transactionStatus) {

                try {

                    // ....  业务代码
                } catch (Exception e){
                    //回滚
                    transactionStatus.setRollbackOnly();
                }

            }
        });
}
```

```java
@Autowired
private PlatformTransactionManager transactionManager;

public void testTransaction() {

  TransactionStatus status = transactionManager.getTransaction(new DefaultTransactionDefinition());
          try {
               // ....  业务代码
              transactionManager.commit(status);
          } catch (Exception e) {
              transactionManager.rollback(status);
          }
}
```

### 2. 声明式

```java
@org.springframework.transaction.annotation.Transactional
```

## 重要接口

### PlatformTransactionManager

**概念**：Spring事务策略的核

**实现**：DataSourceTransactionManager、HibernateTransactionManager、JpaTransactionManager

**使用接口**：将事务管理行为抽象出来 -> 供给外部的行为不变，具有可拓展性

### TransactionDefinition

**概念**：TransactionDefinition顾名思义事务定义，定义事务的属性，接口定义如下

```java
public interface TransactionDefinition {
    
    /** 隔离级别 */
    int ISOLATION_DEFAULT = -1;
    int ISOLATION_READ_UNCOMMITTED = 1;
    int ISOLATION_READ_COMMITTED = 2;
    int ISOLATION_REPEATABLE_READ = 4;
    int ISOLATION_SERIALIZABLE = 8;
    
    /** 传播行为 */
    int PROPAGATION_REQUIRED = 0;
    int PROPAGATION_SUPPORTS = 1;
    int PROPAGATION_MANDATORY = 2;
    int PROPAGATION_REQUIRES_NEW = 3;
    int PROPAGATION_NOT_SUPPORTED = 4;
    int PROPAGATION_NEVER = 5;
    int PROPAGATION_NESTED = 6;
    
    int TIMEOUT_DEFAULT = -1;
    
    /** 传播行为，默认 REQUIRED */
    int getPropagationBehavior();
    /** 隔离级别，默认 DEFAULT */
    int getIsolationLevel();
    /** 超时时间，默认-1。如果超过该时间限制但事务还没有完成，则自动回滚事务 */
    int getTimeout();
    /** 是否为只读事务，默认 false */
    boolean isReadOnly();

    @Nullable
    String getName();
}
```

可以通过`platformTransactionManager.getTransaction(TransactionDefinition transactionDefinition)`获取来获取一个事务

#### 隔离级别

概念：可以从MySQL的事务隔离级别引申过来，TransactionDefinition`中定义了5个关于传播行为的常量，相应的枚举类`Isolation`

- **TransactionDefinition.ISOLATION_DEFAULT**

  使用后端数据库默认的隔离级别，MySQL 默认采用的 `REPEATABLE_READ` 隔离级别 Oracle 默认采用的 `READ_COMMITTED` 隔离级别.

- **TransactionDefinition.ISOLATION_READ_UNCOMMITTED**

- **TransactionDefinition.ISOLATION_READ_COMMITTED**

- **TransactionDefinition.ISOLATION_REPEATABLE_READ**

- **TransactionDefinition.ISOLATION_SERIALIZABLE**

#### 传播行为

**概念**：事务传播行为是为了解决业务层方法之间互相调用的事务问题，`TransactionDefinition`中定义了7个关于传播行为的常量，相应的枚举类`Propagation`。

- **TransactionDefinition.PROPAGATION_REQUIRED**（默认）

  1. 如果外部方法没有开启事务的话，`Propagation.REQUIRED`修饰的内部方法会新开启自己的事务，且开启的事务相互独立，互不干扰
  2. 如果外部方法开启事务并且被`Propagation.REQUIRED`的话，所有`Propagation.REQUIRED`修饰的内部方法和外部方法均属于同一事务 ，只要一个方法回滚，整个事务均回滚

  举例：方法a上和方法b上如果都指定了该级别的事务传播行为，方法a调用方法b；a和b在一个事务中，其中任意一个方法抛出异常回滚，就是这个事务回滚

- **TransactionDefinition.PROPAGATION_REQUIRES_NEW**

  创建一个新的事务，如果当前存在事务，则把当前事务挂起。也就是说不管外部方法是否开启事务，`Propagation.REQUIRES_NEW`修饰的内部方法会新开启自己的事务，且开启的事务相互独立，互不干扰

  举例：方法a同上，方法b指定该级别的事务传播行为，方法a调用方法b。a方法发生异常，b方法不会回滚，b开启了独立的事务；b方法抛出异常，异常会被事务管理机制检测到，a和b都会各自回滚

- **TransactionDefinition.PROPAGATION_NESTED**

  如果当前存在事务，则创建一个事务作为当前事务的嵌套事务来运行；如果当前没有事务，则该取值等价于`TransactionDefinition.PROPAGATION_REQUIRED`

  1. 在外部方法未开启事务的情况下`Propagation.NESTED`和`Propagation.REQUIRED`作用相同，修饰的内部方法都会新开启自己的事务，且开启的事务相互独立，互不干扰

  2. 如果外部方法开启事务的话，`Propagation.NESTED`修饰的内部方法属于外部事务的子事务，外部主事务回滚的话，子事务也会回滚，而内部子事务可以单独回滚而不影响外部主事务和其他子事务

  举例：a方法（REQUIRED）调用b1方法（NESTED）和b2方法（NESTED）。a回滚，b1和b2回滚；b1回滚（不抛出异常），只有b1回滚

- **TransactionDefinition.PROPAGATION_MANDATORY**（用的不多）

  如果当前存在事务，则加入该事务；如果当前没有事务，则抛出异常。（mandatory：强制性）

- **TransactionDefinition.PROPAGATION_SUPPORTS**（几乎没用）

  如果当前存在事务，则加入该事务；如果当前没有事务，则以非事务的方式继续运行

- **TransactionDefinition.PROPAGATION_NOT_SUPPORTED**（几乎没用）

  以非事务方式运行，如果当前存在事务，则把当前事务挂起

- **TransactionDefinition.PROPAGATION_NEVER**（几乎没用）

  以非事务方式运行，如果当前存在事务，则抛出异常

#### 回滚规则（？）

- 默认规则

  运行期异常（RuntimeException 的子类） - 回滚

  Error - 回滚

  检查型（Checked）异常 - 不会回滚

#### 是否只读（？）

默认**false**，只读事务不涉及数据的修改，数据库会提供一些优化手段，适合用在有多条数据库查询操作的方法中

- MySQL 默认对每一个新建立的连接都启用了`autocommit`模式。在该模式下，每一个发送到 MySQL 服务器的`sql`语句都会在一个单独的事务中进行处理，执行结束后会自动提交事务，并开启一个新的事务
- 如果给方法加上了`@Transactional`，这个方法执行的所有`sql`会被放在一个事务中

#### 事务超时（？）

默认值**-1**（不会超时），但如果设置了具体的超时时间，如果一个事务超过该时间限制但事务还没有完成，则自动回滚事务

### TransactionStatus

**概念**：接口中定义如下

```java
public interface TransactionStatus extends SavepointManager, Flushable {
	boolean isNewTransaction();
	boolean hasSavepoint();
	void setRollbackOnly();
	boolean isRollbackOnly();
	boolean isCompleted();
}
```

可以通过 `platformTransactionManager.getTransaction`获取

## 核心注解

>  @Transactional

#### 概念

​	上面提到事务定义的属性，都可以通过设置注解属性来指定

#### 作用范围

​	方法：推荐将注解使用于方法上，该注解只能应用到 public 方法上，否则不生效

​	类：如果这个注解使用在类上的话，表明该注解对该类中所有的 public 方法都生效

​	接口：不推荐在接口上使用

#### 原理

- `@Transactional` 的工作机制是基于 AOP 实现的，AOP 又是使用动态代理实现的。如果目标对象现了接口，默认情况下会采用 JDK 的动态代理，如果目标对象没有实现了接口,会使用 CGLIB 动态代理

- `DefaultAopProxyFactory.createAopProxy`决定了是使用 JDK 还是 Cglib 来做动态代理

- 如果一个类或者一个类中的 public 方法上被标注`@Transactional` 注解的话，Spring 容器就会在启动的时候为其创建一个代理类，在调用被`@Transactional` 注解的 public 方法的时候，实际调用的是，`TransactionInterceptor` 类中的 `invoke()`方法。这个方法的作用就是在目标方法之前开启事务，方法执行过程中如果遇到异常的时候回滚事务，方法调用完成之后提交事务

- `TransactionInterceptor` 类中的 `invoke()`方法内部实际调用的是 `TransactionAspectSupport` 类的 `invokeWithinTransaction()`方法

- 若同一类中的其他没有 `@Transactional` 注解的方法内部调用有 `@Transactional` 注解的方法，有`@Transactional` 注解的方法的事务会失效。

  这是由于`Spring AOP`代理的原因造成的，因为只有当 `@Transactional` 注解的方法在类以外被调用的时候，Spring 事务管理才生效

## 参考资料

1. 发起本笔记的公众号文章：https://mp.weixin.qq.com/s/hT7rJLU-AnWc84Iu8jCajA
2. [总结]Spring 事务管理中@Transactional 的参数:http://www.mobabel.net/spring 事务管理中 transactional 的参数/
3. Spring 官方文档：https://docs.spring.io/spring/docs/4.2.x/spring-framework-reference/html/transaction.html
4. 《Spring5 高级编程》
5. 透彻的掌握 Spring 中@transactional 的使用: https://www.ibm.com/developerworks/cn/java/j-master-spring-transactional-use/index.html
6. Spring 事务的传播特性：https://github.com/love-somnus/Spring/wiki/Spring 事务的传播特性
7. Spring 事务传播行为详解 ：https://segmentfault.com/a/1190000013341344
8. 全面分析 Spring 的编程式事务管理及声明式事务管理：https://www.ibm.com/developerworks/cn/education/opensource/os-cn-spring-trans/index.html