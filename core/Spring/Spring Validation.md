## Spring Validation

> 参考：https://mp.weixin.qq.com/s/8hUIdz2NHcsAu_NiZsGPYg

## 概念

- Java API规范(JSR303)定义了Bean校验的标准validation-api，但没有提供实现。hibernate validation是对这个规范的实现，并增加了校验注解如@Email、@Length等

- Spring Validation是对hibernate validation的二次封装，用于支持spring mvc参数自动校验。接下来，我们以spring-boot项目为例，介绍Spring Validation的使用

## 原理

`RequestResponseBodyMethodProcessor.resolveArgument() → validateIfApplicable()`

`WebDataBinder.validate()`

## 说明

- 如果spring-boot版本小于2.3.x，spring-boot-starter-web会自动传入hibernate-validator依赖。如果spring-boot版本大于2.3.x，则需要手动引入依赖：`org.hibernate.hibernate-validator`
- DTO表示数据传输对象（Data Transfer Object），用于服务器和客户端之间交互传输使用的。在spring-web项目中可以表示用于接收请求参数的Bean对象。

## 传参方式

大部分情况下，POST、PUT请求的参数数据主要在请求体中 - 对应Controller接口方法形参前的`@RequestBody`

大部分情况下，GET请求的参数数据主要在url中 - 对应Controller接口方法形参前的`@PathVariable`

## 核心注解

### @Valid

​	来源：JSR-303规范

​	可修饰范围：METHOD、FIELD、CONSTRUCTOR、PARAMETER、TYPE_USE

​	独有特性：嵌套校验

### @Validated

​	来源：Spring提供

​	可修饰范围：TYPE、METHOD、PARAMETER

​	独有特性：接口方法基本类型校验、分组

## 原理

- 接口方法形参前如果有被`@Valid`或者`@Validated`修饰，那么Spring在接收封装调用接口传输的数据时，会按照参数实体中的`javax.validation.constraints`下的约束注解规则，进行校验

- GET请求，如果参数比较多(比如超过6个)，推荐使用DTO对象接收。否则，将一个个参数平铺到方法入参中，不仅代码可读性差；并且这种情况下，还必须在Controller类上加上`@Validated`注解（加`@Valid`无效）。

- 参数实体校验不通过：

  1、抛出`MethodArgumentNotValidException`

  2、400响应码（Bad Request）

  3、接口响应数据没有任何提示

  参数实体参数平铺校验不通过：

  1、抛出`ConstraintViolationException`

  2、500响应码（Internal Server Error）

  3、接口响应数据没有任何提示

## 统一异常处理

- 像上面说的那样，如果校验不通过，会抛出`MethodArgumentNotValidException`或者`ConstraintViolationException`异常。这时调用接口的响应码就是500，并且表示错误的 json 数据里边，不含任何错误信息

- 有些系统要求无论发生什么异常，http的状态码必须返回200，由业务码去区分系统的异常情况

  ```java
  @RestControllerAdvice
  public class CommonExceptionHandler {
  
      @ExceptionHandler({MethodArgumentNotValidException.class})
      @ResponseStatus(HttpStatus.OK)
      @ResponseBody
      public Result handleMethodArgumentNotValidException(MethodArgumentNotValidException ex) {
          BindingResult bindingResult = ex.getBindingResult();
          StringBuilder sb = new StringBuilder("校验失败:");
          for (FieldError fieldError : bindingResult.getFieldErrors()) {
              sb.append(fieldError.getField()).append("：").append(fieldError.getDefaultMessage()).append(", ");
          }
          String msg = sb.toString();
         return Result.fail(BusinessCode.参数校验失败, msg);
      }
  
      @ExceptionHandler({ConstraintViolationException.class})
      @ResponseStatus(HttpStatus.OK)
      @ResponseBody
      public Result handleConstraintViolationException(ConstraintViolationException ex) {
          return Result.fail(BusinessCode.参数校验失败, ex.getMessage());
      }
  }
  ```

  

## 分组

​	在实际项目中，可能多个方法需要使用同一个DTO类来接收参数，而不同方法的校验规则很可能是不一样的。例如，通用的一个参数实体中的一些参数，仅在创建时要求不为空，且这些字段不能编辑。这个时候，简单地在DTO类的字段上加约束注解无法解决这个问题。因此，spring-validation支持了分组校验的功能，专门用来解决这类问题。

- `Default`

  Default是一个接口

- `@GroupSequence({Xxx1.class, Xxx2.class, ...})`

  指定多个分组的校验顺序

接口方法形参校验，不指定分组；参数实体中校验规则注解的字段，不指定分组：校验

接口方法形参校验，不指定分组；参数实体中校验规则注解的字段，指定分组：不校验



接口方法形参校验，指定一个分组；参数实体中校验规则注解的字段，不指定分组

​	分组接口类继承了Default：校验

​	分组接口类没有继承Default：不校验

接口方法形参校验，指定一个分组；参数实体中校验规则注解的字段，指定分组

​	对应的分组才校验



接口方法形参校验，指定多个分组；参数实体中校验规则注解的字段

​	指定了分组的：对应的分组才校验

​	没有指定分组的：只要Controller接口方法的形参层面如果指定的分组中含有继承Default的：校验

## 嵌套校验

​	A（自定义实体类型）实体中定了一个B（自定义实体类型）字段。B实体同A实体，其中都定义了校验规则的相关注解，A实体中的B类型字段，不做处理的话，默认校验A时，是不会校验B类型的属性字段的

​	希望校验A时，不仅校验B类型的字段，还希望校验B的属性字段，只需要在A实体中的B类型字段上加上`@Valid`注解即可

## 自定义校验

​	业务需求总是比框架提供的简单校验规则复杂的多。例如，List中的数据不能有重复，又或者是某些字段需要符合某些特定的正则表达式，虽然有`@Regex`注解来解决这样的问题，但是每个类似需要校验的字段都要书写正则表达式和对应的错误提示，是很不合理的。所有这些都推荐自定义一个校验规则

- 实例（@NoRepeat）

  ```java
  /**
   * 修饰请求参数类型为List，对包含的元素是否有重复进行校验
   *
   * @author 王敏聪
   * @date 2019/11/6 11:43
   */
  @Constraint(validatedBy = ListValidator.class)
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ METHOD, FIELD, ANNOTATION_TYPE, CONSTRUCTOR, PARAMETER, TYPE_USE })
  public @interface NoRepeat {
  
      String message() default "列表中含有重复元素";
  
      Class<?>[] groups() default { };
  
      Class<? extends Payload>[] payload() default { };
  
  }
  
  ```

  ```java
  /**
   * 自定义List校验器
   *
   * @author 王敏聪
   * @date 2019/11/6 14:53
   */
  public class ListValidator implements ConstraintValidator<NoRepeat, List<?>> {
  
      @Override
      public void initialize(NoRepeat constraintAnnotation) {
          // do nothing
      }
  
      @Override
      public boolean isValid(List<?> list, ConstraintValidatorContext context) {
          // 为空不校验
          if (list == null) {
              return true;
          }
  
          // 利用HashSet的快速查找，实现查重
          Set<Object> set = new HashSet<>(HashCollectionUtils.getAppropriateSize(list));
          Set<Object> duplicateSet = new HashSet<>(16);
  
          for (Object local : list) {
              if (!set.add(local)) {
                  duplicateSet.add(local);
              }
          }
  
          if (!duplicateSet.isEmpty()) {
              if (EnumDefine.class.isAssignableFrom(duplicateSet.iterator().next().getClass())) {
                  duplicateSet = duplicateSet
                      .stream()
                      .map(duplicate -> ((EnumDefine)duplicate).getValue())
                      .collect(Collectors.toSet());
              }
  
              changeTipMessage(context, "列表中包含重复元素：" + duplicateSet);
              return false;
          }
  
          return true;
      }
  
      private void changeTipMessage(ConstraintValidatorContext context, String tip) {
          // 禁用默认提示
          context.disableDefaultConstraintViolation();
          // 修改错误提示
          context.buildConstraintViolationWithTemplate(tip).addConstraintViolation();
      }
  
  }
  ```

  

## 显示调用校验方法

将自定义规则的校验器注入到Spring容器中

```java
@Bean
public Validator validator() {
    ValidatorFactory validatorFactory = Validation.byProvider(HibernateValidator.class)
        .configure()
        .failFast(false)
        .buildValidatorFactory();
    return validatorFactory.getValidator();
}
```

需要使用校验器的地方，注入使用即可

## 全局方法形参校验

不贴出实现 - 增对Controller方法来说，是二次校验，暂时没什么特别好的方法。反正思路就是aop - `MethodInterceptor`

## 最佳实践

- Number

  @Range(min = xxx, max = xxx) = @Max + @Min

- String

  @NotBlank：@NotNull + @Size(min = 1)
  @Size：限定长度
  @Verify：姓名、邮箱、电话、编号、二维码（自定义）

- List

  @NoRepeat
  List<@NotNull Seriziable>
  List<@Valid @NotNull XxxForm> 或者 @Valid List<@NotNull XxxForm>
  @NotEmpty = @NotNull + @Size(min = 1)（可选）