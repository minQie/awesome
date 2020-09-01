# Javadoc

- 参考

  https://zhuanlan.zhihu.com/p/37594408

## 编程规范的必要性

​	代码编程规范之所以重要是因为：

- 软件生命周期中的80%时间是软件维护期；
- 几乎没有任何软件在其整个生命周期中一直由它的原作者来负责维护；
- 代码编程规范能够增加软件的可读性，使得软件工程师更快更准确地理解新代码；
- 编程规范能提高软件的封装性；

## 规范文档的一些要求

  后面文中将深入讨论结构良好的程序文档的规则和要求。一个结构良好的文档应首先做到以下几点。

- **文档注释应能增加代码的可读性**

​    编写注释的目的是使你、你的开发伙伴以及后续的开发人员更好地理解你的代码。SUN公司JAVA开发组的Nagle曾说过“如果程序不值得写文档的话，意味着代码也不值得运行”。



- **避免文档的过度修饰**

​    六七十年代的COBOL语言开发着有一种注释习惯，通常用星号画一个方盒子来填写注释。这样做的确很好看，但对于最终产品来讲没有太多的益处。应当注意文档中注释和代码的比例，代码的注释应当简洁、明了。



- **在编码前就开始写文档**

​    原则上讲，我们看一段代码总能发现它在做什么。例如，下面这段代码中，我们能分析出代码的行为，但不能真正看出它的确切语义。

## 注释

### HTML标签

#### html转译字符

​	https://www.136.la/tech/show-107847.html

​	https://blog.csdn.net/surebestchina/article/details/88926889

#### 常用标签

- `<br>`

  放在行尾, 起到换行符的作用

- `<p>`

  放在行头, 起到换行符的作用

- `<b></b>` `<strong></strong>`

  加黑, 正常转译回车符

- `<ul><li>xxx1</li><li>xxx2</li>...</ul>`

  无序列表

- `<ol><li>xxx1</li><li>xxx2</li>...</ol>`

  有序列表

- `<h3></h3>`

  标题

- `<pre></pre>`

  对于较复杂的类或方法，最好在其注释文档中增加一段如何使用本类（方法）的例子代码。由于javadoc生成的文档会忽略空格和换行，应在样例代码段前后加上该标签

  但是内容如果用到特殊字符, 需要转译, 如`@`需要转译为`&#064;`

  正常转译回车符

### javadoc标签

- 官网

  https://www.oracle.com/technical-resources/articles/java/javadoc-tool.html#tag

  https://docs.oracle.com/javase/8/docs/technotes/tools/windows/javadoc.html#Javadoc%20Tags

@author

@version @since

@deprecated

```
/**
 * @deprecated  As of JDK 1.1, replaced by {@link #setBounds(int,int,int,int)}
 */
```

@throws

@exception

@param

@return

{@link xxx.xxx.xxx.Xxx#xxx}

@see

{@code xxx}

