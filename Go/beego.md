# beego

> 官方文档：https://beego.me/

## 简介

beego 是一个快速开发 Go 应用的 HTTP 框架，他可以用来快速开发 API、Web 及后端服务等各种应用，是一个 RESTful  的框架，主要设计灵感来源于 tornado、sinatra 和 flask 这三个框架，但是结合了 Go  本身的一些特性（interface、struct 嵌入等）而设计的一个框架

## 快速开始

1. 说明

   安装和下载 beego 和 Bee，默认会生成一个 `%GOPATH%\bin\bee.exe`
   
   （Go 语言的 GOPATH 是可以配置多个的，默认选择第一个，下面都是同理，就不会再赘述了）
   
2. 安装更新 beego 和 Bee

   ```shell
   go get -u github.com/beego/bee
   go get -u github.com/astaxie/beego
   ```

   **问题**：可能是获取该框架的相关信息，需要科学上网，现象就是执行 `go get github.com/astaxie/beego` 控制台就卡住，并在达到超时时间后，报出相关的错误信息

   **解决**：需要像下边一样配置一下代理（也有说直接上 github 将相关代码下载下来也行）

   > 代理：https://goproxy.cn/

   ```shell
   # 这个参数并不只是简单的之和代理相关，详见：https://blog.csdn.net/u011897301/article/details/105758056/
   go env -w GO111MODULE=on
   go env -w GOPROXY=https://goproxy.cn,direct 
   ```

3. 使用 bee 快速创建项目

   [bee new](#bee 命令介绍)

   [bee api](#bee 命令介绍)

4. 运行（类比 Java 的 Tomcat）

   ```bash
   # 完整命令如下
   
   # 创建项目
   bee new 项目名
   
   # 将 %GOPATH%/src/项目名 拷贝到 自定义目录下
   cd 自定义目录\项目名
   
   # 运行
   bee run
   ```

5. 访问 `http://127.0.0.1:8080` 有响应

   beego 默认包含热部署的概念（得通过 bee run 命令运行项目，才有这个特性），例如，修改项目目录下的 default.go，可以看到控制台输出相关日志

## bee 命令介绍

**new**

​	`bee new 项目名`：快速创建一个 Web 项目

**api**

​	`bee api 项目名`：快速创建一个 Api 应用

​	上述命令默认还支持一些参数，并且默认会在 `%GOPATH%\src` 生成项目代码；具体参数、默认的目录结构见 https://beego.me/docs/install/bee.md

**pack**

​	只要执行 `bee pack` 就可以打包，然后，上传到服务器解压、更新

**version**

​	`bee version` 可以查看 bee、beego、go 的版本

**其他命令**

​	**generate**、**migrate**、**dockerize**

## 路由

> 这个东西真就全是坑

先简单说一下这个机制，你需要在 `项目目录/routers/route.go` 根据官方文档的路由配置规则来，我们就重点说一下注解方式的路由配置（详见Swagger，为什么重点说这个），假定你已经完成了接口和路由的相关配置。项目运行时，beego 路由初始化会去读取配置的 controllers，然后会生成对应目录结构层次的路由配置源码文件，真正初始化路由规则的代码就在这些生成的文件中 init 方法中，当然，这些文件就是根据接口方法头上的特定格式注释生成的

**动态生成的文件名的大致规则**

- controller/ →          commentsRouter___controllers.go

- controllers/xxx/ → commentsRouter___controllers_xxx.go

**路由配置注意点**

- @router 的 r 是小写的

- 接口方法 不是 接口函数（且方法对应的结构体一定要加 *）

- 方法名一定要大写

  **反（这都记，因为太坑了）**

- 请求方式没有要求一定要大写还是小写

### 坑1

​	你可以把 route.go 文件放到 router 包下，但是动态生成的文件还是放到 routers 包下

### 坑2

​	根据动态生成的文件名，如果你的包名是 `test`，那么动态生成的路由文件就是 `xxx_test.go` 了，没错 beego 认为这是测试文件，不管了（泪目）

### 坑3

​	和 Golang 自动保存文件的机制有关，改动了 Controller 接口方法上的路由注释，项目必须要重复运行两次，才会生效

## Swagger

想要使用 Swagger 的文档生成，router.go 的路由配置方式必须是下面这样，通过 Namespace 的注解式配置

```go
xxxNamespace := beego.NewNamespace(
	beego.NsNamespace{
        beego.NSInclude(
            &xxx.XxxController{},
        ),
        ...,
    },
    ...
)

...

beego.AddNamespace(xxxNamespace, ...)
```

并且是熟悉 Swagger 理念，需要在相关的接口方法上通过特定格式的注释来对参数、响应结果进行说明

## ORM

### 表

​	**多个设置间使用 `;` 分隔，设置的值如果是多个，使用 `,` 分隔**

- 自定义引擎（默认 INNODB）

  ```go
  func (xxx *Xxx) TableEngine() string {
      return "INNODB"
  }
  ```

- 自定义表名（默认结构体名的小蛇命名）

  ```go
  // 定义如下方法
  func (xxx *Xxx) TableName() string {
      return "表名"
  }
  
  // 注册 Model 的时候指定前缀也会影响最终的表名
  orm.RegisterModelWithPrefix("prefix_", new(Xxx))
  ```

### 字段

- 单字段索引（index、unique）

- 多字段索引

  ```go
  func (xxx *Xxx) TableIndex() [][]string {
      return [][]string{
          []string{"Id", "Name"},
      }
  }
  
  // 多字段唯一键
  func (xxx *Xxx) TableUnique() [][]string {
      return [][]string{
          []string{"Name", "Email"},
      }
  }
  ```

- 字段：忽略（-）

  GO 的结构体标签设计拓展，都默认遵守这个规则，`-` 代表忽略的意思

- 字段：名称（column(字段名)）

- 字段：自增（auto）

  当 Field 类型为 int, int32, int64, uint, uint32, uint64 时，可以设置字段为自增健

- 字段：主键（pk）

  当模型定义里没有主键时，符合上述类型且名称为 Id 的 Field 将被视为自增健

  鉴于 go 目前的设计，即使使用了 uint64，但你也不能存储到他的最大值。依然会作为 int64 处理

- 字段：可为空（null）

  所有字段默认 `NOT NULL`

- 字段：指定类型（type(具体的类型)）

- 字段：字符串类型长度（size(字段长度)）

  `string` 类型字段默认为 `varchar(255)`，设置了 size，将使用 `varchar(字段长度)`

- 字段：浮点数类型（digits(总长度);decimals(小数位长度)）

- 字段：时间类型（auto_now 和 auto_now_add）

  auto_now：每次 model 保存时都会对时间自动更新

  auto_now_add：第一次保存时才设置时间

  举例：

  ​	CreateTime time.Time \`orm:"auto_now_add;type(date)"\`

  ​	CreateTime time.Time \`orm:"auto_now_add;type(datetime)"\`

  注：对于批量更新设置不起作用

- 字段：注释（description）

  注：注释中禁止包含引号

**类型映射表**

| go                                          | mysql                           |
| ------------------------------------------- | ------------------------------- |
| int, int32 - 设置 auto 或者名称为 `Id` 时   | integer AUTO_INCREMENT          |
| int64 - 设置 auto 或者名称为 `Id` 时        | bigint AUTO_INCREMENT           |
| uint, uint32 - 设置 auto 或者名称为 `Id` 时 | integer unsigned AUTO_INCREMENT |
| uint64 - 设置 auto 或者名称为 `Id` 时       | bigint unsigned AUTO_INCREMENT  |
| bool                                        | bool                            |
| string - 默认为 size 255                    | varchar(size)                   |
| string - 设置 type(char) 时                 | char(size)                      |
| string - 设置 type(text) 时                 | longtext                        |
| time.Time - 设置 type 为 date 时            | date                            |
| time.Time                                   | datetime                        |
| byte                                        | tinyint unsigned                |
| rune                                        | integer                         |
| int                                         | integer                         |
| int8                                        | tinyint                         |
| int16                                       | smallint                        |
| int32                                       | integer                         |
| int64                                       | bigint                          |
| uint                                        | integer unsigned                |
| uint8                                       | tinyint unsigned                |
| uint16                                      | smallint unsigned               |
| uint32                                      | integer unsigned                |
| uint64                                      | bigint unsigned                 |
| float32                                     | double precision                |
| float64                                     | double precision                |
| float64 - 设置 digits, decimals 时          | numeric(digits, decimals)       |

注：就是虽然 GO 的基础数据类型有 uxxx 来对应 MySQL UNSIGNED 关键字，但是并不推荐使用 uxxx，因为没法进行数据转换；举例，数据库（BIGINT UNSIGNED）、ORM 实体（uint64）、其他类库、请求参数等（int64）。这可能就是数据库字段不推荐使用 UNSIGNED 的原因，ORM 实体字段和数据表字段保持一致，记住核心原因 GO 的 ORM 实体不好使用 uxxx

## 点

### 文件位置

- **可执行的二进制文件**

就是需要清楚，beego 框架的目录结构是通过 bee 生成的，这样的目录结构并不仅是一种规范和推荐的形式，而是强制的，beego 通过相关命令生成的二进制可执行文件应该放在在项目的源码目录下，因为该二进制文件运行时，会读取所在目录下，例如 conf、routers  等文件中的内容。就也是说这些静态的文件是不会一同编译进二进制文件中（这样也不合理），所以项目中需要配置 `Output directory` 为当前项目目录的绝对路径

实际二进制 `exe` 文件的输出目录，可以在项目运行时，Goland 控制台日志中最上边 `➕ <4 go setup calls>` 查看到

- 测试1

  ```markdown
  // 没有设置 `Output directory`，并勾选了 `Run after build`
  balabala\go.exe build -o C:\Users\用户名\AppData\Local\Temp\___go_build_xxx.exe xxx #gosetup
  C:\Users\用户名\AppData\Local\Temp\___go_build_xxx.exe #gosetup
  ```

  疑问：读取的到相应的配置文件？

  ```markdown
  // 在 main 函数中打印一下读取 conf 下配置文件的结果
  能够获取到
  ```

- 测试2

  ```markdown
  // 设置了 `Output directory` 为 `.`，并勾选了 `Run after build`
  Error running 'go build xxx': Cannot run program ".\go_build_xxx.exe" (in directory "balabala..."): CreateProcess error=216, 该版本的 %1 与你运行的 Windows 版本不兼容。请查看计算机的系统信息，然后联系软件发布者。
  ```

  疑问：上面运行不了，那当前所在目录是哪里？

  ```markdown
  // 设置了 `Output directory` 为 `.`，不勾选 `Run after build`
  balabala\go.exe build -o .\go_build_xxx.exe xxx #gosetup
  ```

  ```markdown
  // 设置了 `Output directory` 为 `.`，勾选了 `Run after build`，将二进制文件复制到 GOROOT 中
  同上报错
  ```

  ```markdown
  // 因为想到，二进制程序运行本质上也是通过 CMD 运行，所以打开了一个 CMD 看了一下默认目录：`C:\Users\用户名`
  // 设置了 `Output directory` 为 `.`，勾选了 `Run after build`，将二进制文件复制到 `C:\Users\用户名` 中
  同上报错
  ```

  ```markdown
  // 设置了 `Output directory` 为 `.`，CMD 进入到项目目录执行 exe
  成功...
  ```

  ```markdown
  // 重点就是 `CreateProcess error=216, 该版本的 %1 与你运行的 Windows 版本不兼容。请查看计算机的系统信息，然后联系软件发布者。`
  没有搜索到好的相关内容，暂时放掉这个，没有意义，时间精力投入的够多了
  ```

- 结论

  就是说，无论你是否自定义输出目录，就是你可以不写，或者一定要写绝对路径，不然，二进制文件可能会生成到相应的目录中，但是紧接着的命令执行 `${Output directory}\go_build_xxx.exe`，就会报错。所以，全局来说，推荐写项目源码所在目录的绝对路径；自己个人来说，乱起八糟的文件都在源码中是不好的，推荐就不配置
  
  **最终**：`项目目录\target`
  
- **日志文件**

  这是灵活的，开启 beego 的日志文件模块，就需要配置目录 

  **最终**：`target/logs/项目名.log`

- lastupdate.temp

  这个不是灵活的，是局部变量写死的

  **最终**：尝试使用反射修改

## 业务模块

### 用户

表结构

鉴权

角色

### 附件

- 项目启动时就需要保证存储文件的主要目录是存在的

  初始化存储文件的目录（如果没有才初始化），只要初始化主目录，子目录目前先不初始化
  考虑到根据时间分类，文件夹创建的操作在实际调用接口中无法避免的，所以就创建基本的，否则，都会在项目启动时初始化好
  如果存储文件夹的主目录都创建不了，附件管理模块将形同虚设，所以创建失败，直接报错