# Java Jar

## 写作目的

​	不借助任何高级开发工具，仅通过简单的文本编辑器和 JRE，基于 Java Swing 开发 Windows 操作系统上的图形用户界面时，大概率会遇到打包、双击运行打好的 jar 包，没有反应等相关问题，这里仅留作纪念（因为确实没有任何实用性了）

## 打包问题

> 这里将不使用高级开发工具，仅使用 java 提供的 jar 命令，将打包的方法步骤记录下来

1. 创建一个1.txt，内容

   ```
   Main-Class: 你的main函数所在类的完整路径回车
   ```

   回车很重要，假如没有，main函数所在类没有写进 jar 的META-INF\MANIFEST.MF文件中，双击是不可能运行的起来的

2. DOS命令行命令

   ```java
   jar -cvfm xxx.jar 1.txt 包名1 包名2 ...
   ```

**原理**

其实 jar 包的本质就是一个压缩包，使用大众的压缩工具是可以解压的

而打包命令的本质是这样的：

1、创建一个"MANIFEST.MF"文件，将"MANIFEST.MF"放到一个新建的名为："META-INF"的文件夹里
用记事本打开"MANIFEST.MF"可以看到：

```
Manifest-Version: 1.0
Created-By: 1.8.0_20 (Oracle Corporation)
Main-Class: main函数所在类的完整类名
```

2、将 META-INF、包1、包2... 压缩一下，生成的压缩文件后缀名为jar（用压缩软件是可以打开jar文件的）

## 运行问题

- 原因

  jar 文件默认关联的程序是 java.exe 而不是 javaw.exe，在右键的打开方式里面这两个程序也分辨不出来，都显示的是：Java (TM) Platform SE binary

- 解决（可能失败）

  右击 jar 包 → 打开方式 →  jdk 根目录\bin\javaw.exe

- 解决（一定解决）

  `Win + R` → `regedit` → 打开注册表 → 找到 `HKEY_CLASSES_ROOT\Applications\javaw.exe\shell\open\command` → 值修改为 `"X:\jdk\bin\javaw.exe" -jar "%1"` ( 即，只需要在原来的 %1 前面添加 -jar )