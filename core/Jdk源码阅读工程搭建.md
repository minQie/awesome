# Jdk源码阅读工程搭建

## 说明

​	虽然在任意的开发环境下，都能够进行jdk源码的阅读和断点调试，但是专门为阅读jdk源码搭建一个项目工程的能够带来如下好处：

1. 清晰的源码目录结构
2. 能够在jdk源码上，留下自己的注释说明

## 准备

- JDK

  **版本**

  `java -version`

  ```
  java version "1.8.0_261"
  Java(TM) SE Runtime Environment (build 1.8.0_261-b12)
  Java HotSpot(TM) 64-Bit Server VM (build 25.261-b12, mixed mode)
  ```

  **目录结构**

  ```cmd
  目录: E:\Learning-Program\jdk\jdk1.8.0_261
  
  
  Mode                LastWriteTime         Length Name
  ----                -------------         ------ ----
  d-----        2020/7/25     11:38                bin
  d-----        2020/7/25     11:38                include
  d-----        2020/7/25     11:38                jre
  d-----        2020/7/25     11:38                legal
  d-----        2020/7/25     11:38                lib
  -a----        2020/6/18      7:39           3244 COPYRIGHT
  -a----        2020/7/25     11:38        5219010 javafx-src.zip
  -a----        2020/7/25     11:38            195 jmc.txt
  -a----        2020/7/25     11:38             44 LICENSE
  -a----        2020/7/25     11:38            159 README.html
  -a----        2020/7/25     11:38            426 release
  -a----        2020/6/18      7:39       21150466 src.zip
  -a----        2020/7/25     11:38            190 THIRDPARTYLICENSEREADME-JAVAFX.txt
  -a----        2020/7/25     11:38            190 THIRDPARTYLICENSEREADME.txt
  ```

- IntelliJ IDEA 2020.1

## 搭建

​	创建一个项目，最简单的 `Java Project` 或者一个最基本的  `Maven Project` 都行，这里以`Maven Project`为例

- `pom.xml`

  ```maven
  <?xml version="1.0" encoding="UTF-8"?>
  <project xmlns="http://maven.apache.org/POM/4.0.0"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
  
    <groupId>priv.wmc</groupId>
    <artifactId>JdkSource</artifactId>
    <version>1.0-SNAPSHOT</version>
  
    <build>
      <plugins>
        <plugin>
          <groupId>org.apache.maven.plugins</groupId>
          <artifactId>maven-compiler-plugin</artifactId>
          <version>3.8.1</version>
          <configuration>
            <source>1.8</source>
            <target>1.8</target>
          </configuration>
        </plugin>
      </plugins>
    </build>
  
    <properties>
      <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
      <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
      <java.version>1.8</java.version>
      <skipTests>true</skipTests>
    </properties>
  
  </project>
  ```

- 创建包目录结构

  `priv.wmc.source`

  在系统的文件管理层面，将jdk根目录下的`src.zip`解压后内容复制到上面包目录下
  （不要在idea层面操作，否则会卡死）

- idea配置1

  Setting → Build, Execution, Deployment → Compiler

  `Build process heap size` 由默认的 `600` 改为 `1600`，否则项目无法编译通过

- idea配置2

  Setting → Build, Execution, Deployment → Debugger → Stepping

  将 `Do not step into the classes` 下边的 `java.*` 取消勾选，否则通过断点调试进入到jdk源码层面

- idea配置3

  Project Structure → SDKs → Sourcepath

  将 `javafx-src.zip`  和 `src.zip` 中的 `src.zip` 删除，将当前项目的 `source` 对应的硬盘物理文件路径，作为源码路径添加到到Sourcepath，否则实际测试中，使用断点调试，进入到的jdk源码文件，还是jdk中的，和项目毫无关系

## 常见问题

### 1. 找不到 `sun` 包，引入的源码文件，在自定义测试代码编译的过程中报错

​	虽然程序羊大叔的视频中，没有出现问题，但实际自己搭建的时候，最大的问题就是这个，有解决方案说，缺什么就上OpenJdk的网站上下载，并且大多数人的问题就是缺少两个文件 `sun.awt.UNIXToolkit` 和 `sun.font.FontConfigManager` ，但是实际自己从Oracle官网下载的jdk，还缺少了其他，由于实在烦人和解决的过于麻烦，最终的解决方式是把引入的源码包中的 `com.sum` 删除了（主要是，java开发人员，重点要关注的源码模块不涉及这个）

### 2. 缺少 `com.sum.tools` 包

​	这个的解决方式是，Project Structure → Libraries → 新增（选择到jdk根目录下的lib文件夹）

​	最开始使用的jdk出现了这个问题，使用上面的方式也解决了，但是把 `com.sum` 删掉后，也就没有这个问题了

## 解惑

### 1. 复制到source包下的java源码文件的包声明问题

​	保持原样就好，虽然 `import` 声明和类实际所在的包名结构不匹配，但是不会有问题，因为这些源码文件并不参与实际的编译运行，只作为源码模板（参与实际运行，那就是另外一块的内容了，和本文章主旨不服）

## 缺陷

### 1. 为项目中的jdk源码文件添加的注释，导致代码所在的行号的更改

​	目前只能在行尾添加注释，不然实际调试就会出现，断点位置和源码文件不匹配的问题

