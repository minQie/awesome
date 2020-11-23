# Goland

## 快捷键

| 快捷键                                       | 说明                                                         |
| -------------------------------------------- | ------------------------------------------------------------ |
| Ctrl + Alt + L                               | 代码格式化                                                   |
| Ctrl + W                                     | 扩选                                                         |
| Ctrl + Shift + W                             | 缩选                                                         |
| Ctrl + Space（同时和输入法中英文快捷键冲突） | 提示                                                         |
| Ctrl + Shift + Space                         | 代码提示                                                     |
| Ctrl + Shift + Enter                         | 代码自动补全                                                 |
| 左Alt + Enter                                | Quick Fix                                                    |
| Ctrl + Alt + M                               | 将代码块抽成一个方法                                         |
| Shift + F1                                   | 打开光标指向的原生函数的文档                                 |
| Shfit + F6                                   | 光标所指的变量名、方法名等的重命名（自动将使用到的地方也重命名） |
| Ctrl + Alt + V                               | 抽取魔法值作为变量                                           |
| Ctrl + Alt + N                               | 删除变量，将使用的地方替换为变量值                           |
| Alt + 左键单击                               | 多光标                                                       |
| Alt + Shfit + G                              | 为选择的多行的行尾添加多光标                                 |
| 中键                                         | 方形框选，不会考虑换行的那种                                 |
| Ctrl + Shift + V                             | 打开剪切板                                                   |
| Ctrl + Alt + B（Ctrl + Alt + 左键单机）      | 跳转到接口的方法上的实现                                     |
| Ctrl + Alt +  ←  →                           | 在 A 方法中调用了 B 方法，进到 B 方法，返回刚才调用的地方    |

## 技巧

### 编辑

​	在出现提示选项框后，`Enter` 是直接将选项内容直接增到文件中，`Tab` 是将选项内容替换文件内容

### 书签

​	就像读书一样，把一些重要的代码打个标签，常用快捷键如下

​	打标签：F11

​	打一个名的标签：Ctrl + 标签名

### Changelist

> 官网：https://www.jetbrains.com/help/go/2020.2/work-on-several-features-simultaneously.html?utm_source=product&utm_medium=link&utm_campaign=GO&utm_content=2020.2#changelists
>
> - You can easily switch between different logical  sets of changes and commit them separately from each other.
> - Unlike using branches for the same purpose, you  have all your changes at hand without having to switch between branches which can take a while if your project is really  large.
> - It's convenient to test how different features work together.
> - You can remote-run a changelist on a build server.

​	假如说公司代码仓库包含有项目的配置文件，而本地拉下来，一般会修改配置文件的相关参数为本地参数；又或者说，想要自定义一个个人的习惯子类的；还或者说，某些配置在程序代码中写死，你希望修改。以上这些场景，你既希望本地修改，且不修改 .gitignore 文件，又不希望被 Git 版本管理检测到，那么你应该灵活的使用 JetBrain 的 Changelist。先简单介绍一下，首先在 `Git` 是视图中，查看 `Local Changes`（工作区），你会看到有一个 `Default Changelist` 的字段，没错，既然有 `Default`，那就肯定可以自定义。没错很简单的概念，你使用基于开发工具的 Git 版本管理工具，你就可以自定一个 `NO Commit`（只是举例子，名字任意定义） 概念的 Changelist，把所有本地想更改且不修改忽略文件配置，但不纳入 Git 管理的文件都放到这个 `No Commit`，以后就只关注 `Default Changelist` 中的改变内容就好。创建 Changelist 有一个 `Set active` 勾选项，这个就是是否要被管理的文件改动，默认的 `Default Changelist`，就是勾选上的，但是 `Set active` 的属性的 Changelist，全局只能有一个。总结一下，其实这就是 JetBrain 对 Git 工作区概念的增强，新增了工作区改动文件分组的概念

## 插件

- String Mainpulation（快捷键 Ctrl + M）

​	主要是结构体后边字段描述的对齐

## Go的拓展包

​	为什么不叫第三方，因为工具本身也还是 Google 的
