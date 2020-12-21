# GO

## 声明

​	全文全部基于《菜鸟教程》，加上自己的实战记录组成

## 简介

​	计算机软件经历了数十年的发展，形成了多种学术流派，有面向过程编程、面向对象编程、函数式编程、面向消息编程等，这些思想究竟孰优孰劣，众说纷纭。

​	除了OOP外，近年出现了一些小众的编程哲学，GO语言对这些思想亦有所吸收。例如，GO语言接受了函数式编程的一些想法，支持匿名函数与闭包。再如，GO语言接受了以Erlang语言为代表的面向消息编程思想，支持goroutine和通道，并推荐使用消息而不是共享内存来进行并发编程。总体来说，GO语言是一个非常现代化的语言，精小但非常强大

​	go 语言程序可以有多个标记组成，可以是关键字，标识符，常量，字符串，符号，一个标识符实际上就是一个或多个字母（A~Z 和 a~z）、数字（0~9）、下划线（_）组成的序列，但是第一个字符必须是字母或者下划线，不能是数字

### GO 和 GOlang

详见  [知乎](https://www.zhihu.com/question/39508749)

### GO 语言最主要的特性

- 自动垃圾回收
- 更丰富的内置类型
- 函数多返回值
- 错误处理
- 匿名函数和闭包
- 类型和接口
- 并发编程
- 反射
- 语言交互性

### GO 语言程序的基础组成部分

- 包声明
- 引入包
- 函数
- 变量
- 语句 & 表达式
- 注释

## 基础环境（Windows）

### 基础软件

​	[下载 GO 的 SDK](https://golang.google.cn/dl/)，解压版和 msi 安装版都行

​	[下载 IntelliJ GOland](https://www.jetbrains.com/go/)，破解（详见破解文件，InelliJ 全家桶的东西，激活套路都是一样的）

### 目录结构

- 项目目录

  *引出*：先把目录结构说明白了，才能进行环境变量的配置，因为这个 GOPATH 环境变量息息相关。将实际工作学习中根据公司的项目和个人的学习笔记项目区分开来，分为两个不同的工作空间

  ```markown
  工作：D:\workspace\src\公司域名\项目
  学习：D:\learnspace\src\项目
  ```

  *补充*：看起来，也就是 `src` 目录感觉有点奇怪，可以不加这一级目录么？答，不行，一般一个工作空间就认为是一个 GOPATH，而 GO 语言关于 GOPATH 的定义，是必须包含三个目录：src（源文件）、bin（可执行文件）、pkg（编译好的库文件, 主要是*.a文件）。看一下 GO SDK 的目录结构，你会发现也是包含上面三个目录的

- 三方类库

  *引出*：并且一般我们会将第三方库的相关内容单独作为一个工作空间（类比 Java 里的 Maven）

  ```markdown
  三方：D:\golib
  ```

  *补充*：并且在顺序上，我们会这个 GOPATH 项放在第一的位置，因为我们通过 GO 的相关命令下载第三方的相关内容时，会默认放到第一项 GOPATH 下，例如，通过 `go get github.com/astaxie/beego`  下载 beego

- 其他概念

  一般在 GOPATH 下使用 `go get` 和 `go install` 命令对来项目进行编译，这样特定类别的文件就会生成在指定的文件目录下了，`go build` 就会直接在源码所在目录生成对应的可执行文件

  编写 GO 语言代码时，对于 GO 标准库而言，import 不需要写全路径，而对于 GOPATH 中的包而言需要根据目录层次写清楚包的路径

  为了能够在任何目录下执行，我们自己的 GO 程序，我们会将 GOPATH 的 bin 目录添加环境变量

### 环境变量

| 变量名      | 变量值                                                       |
| ----------- | ------------------------------------------------------------ |
| GOROOT      | GO SDK 所在目录                                              |
| GOPATH      | 指定的 lib 目录、企业工作目录、个人学习目录（多个配置项之间用分号隔开） |
| GOPROXY     | https://goproxy.cn,direct（概念类比 Maven 的国内私有仓库，[详见](https://goproxy.cn/)） |
| path        | 添加 %GOROOT%\bin（最后不要添加“\”）                         |
|             | 以及GOPROXY 每一项下的 bin 目录                              |
| GO111MODULE | on（1.13版本之后是默认开启的）                               |

#### GO111MODULE

​	初学者应该都会遇到这样的问题：`package 项目名 is not in GOROOT`。并且第一反应是，我已经配置 GOPATH 了啊，我难道要把项目放到 GO SDK 下么。解决方法在项目目录下执行 `go mod init` 命令，就可以了

## Hello World

假设文件名是 `helloworld.go`，且文件内容如下

```go
package main

import "fmt"

func main() {
    fmt.Println("Hello, World!")
}
```

- 通过 `go run helloworld.go` 你可以直接得到程序的输出结果（不会产生二进制文件 - windows 平台下是 `.exe` 文件）
- 通过 `go build helloworld.go` 就可以得到 `helloworld.exe` 文件，然后通过 `.\helloworld`  直接运行得到程序输出结果（注意，还是这个概念，就是你在什么目录下执行的指令，生成的二进制文件就在什么目录下）

### go env

​	用于查看当前 GO SDK 的环境变量参数

​	改：`go env -w 键=值` 这样的形式去配置 GO 的环境变量，但是那是临时的，重启系统就会丢失

​	查：过滤 `go env` 的结果（类比 Linux 中的 grep）：`go env | findstr 要找的关键字（即使在Windows操作系统下，也要注意大小写）`

### go build

 一般用法：直接在项目目录下执行 `go build`

在其他路径下执行 `go build`，需要在后面加上项目的路径（项目路径从 `GOPATH/src` 后开始写起，编译之后的可执行文件就保存在当前目录下）

-o 生成的二进制文件名：假如当前操作系统是 Windows，然后你要构建可执行的二进制文件到 Linux 系统的服务器中，那么后缀名为 exe 就不合适了，需要通过指定该参数的形式指定

### go fmt

**简介**

​	go 语言官方提供的 SDK 的 bin 目录下边除了 `go.exe` 还有一个 `gofmt.exe`，这个程序主要是用于格式化代码的（JDK 中并没有类似的概念，但是 IntelliJ IDEA 提供了）

**帮助命令**

​	通过 `gofmt --help` 指令，可以查看详细的命令帮助，简单介绍一下参数

​	-l：代码优化会影响的文件

​	-s：将优化的文件的优化结果输出到控制台

​	-w：只有使用了该参数，才会真的将优化文件后的结果写到源码文件中

### go get

​	用自己的话来说就是，类比 CentOS 系统自带的 yum 命令，yum 可以下载并安装各种软件，而 go get 就是为 go 集成更多的第三方类库的命令。有一点要提，就是这个命令是基于源码管理工具（不限于 Git），所以你需要安装相关工具软件，并将其命令配置到环境变量中。其原理就是说，先通过源码工具下载相关的 go 源码，再就是进行编译，还有就是 GOPATH，源码和编译产物都是放到 GOPATH 下的

## 概念

1. 包的定义声明

   关键字：package

   说明：每个 go 语言程序都应该包含名为 main 的包

2. 引入包

   关键字：import

   说明：`import "fmt"` 的意思是告诉GO 编译器这个程序需要使用 fmt 包（的函数，或其他元素）

3. 主函数定义

   `func main()` 是程序开始执行的函数。main 函数是每一个可执行程序所必须包含的，一般来说都是在启动后第一个执行的函数

4. 注释

   单行：`// xxx`

   多行：`/* xxx */`

5. 简单函数调用说明

   `fmt.println("xx")` 等同于 `fmt.print("xxx\n")`

6. 标识符首字母是否大写的含义（权限）

   标识符（包括常量、变量、类型、函数、结构字段）的首字母是否大写，可以决定该对象能够被外部包所使用（当然，其他包下的程序需要先导入这个包），这个被称为导出（类比 Java 语言中的 `public`）；同理，标识符如果以小写字母开头，则类比 Java 语言中的 `protected`

   用自己的话来说就是：Java 语言中通过相关权限修饰符来表述和修饰权限，GO 语言中只要通过代码风格（变量名、函数名的首字母的大小写），个人觉得是非常好的点，因为换个角度来说，就这是站在语言层面上对代码更进一步的约定和规范

7. go 的代码风格

   `{` 左括号是不允许在单独的行上的

   通常 go 语言不需要语句结束符号（这些都将由 GO 编译器自动完成），但是如果多条语句在一行时，可以使用 `;` 分号进行分隔，但是注意，并不推荐这样的写法，从 `gofmt` 代码优化指令，会将你的单行分号分隔写法优化掉就可以知道了

其他：GO 语言中是允许 变量名 和 数据结构名相同的

## 包

**简介**

​	*go 源码文件名* 以及 *go 源码文件所在的文件夹名* 和 *包名* 没有直接联系，但是同一个文件夹下的 go 文件 只能有一个包名

​	注意：并不要求包名一定要和文件夹名相同

**导包**

​	`import "项目名/xxx/.../最后一级"`：常规导包，导入后，当前 GO 源码文件内就可以通过 `最后一级.` 的方式调用大驼峰命名的变量、方法

​	`import . "xxx"`：通过该种方式的导包，调用导入的包就无需通过 `最后一级.` 的形式了

​	`import _ "xxx"`：通过该种方式的导包，无法调用导入的包的内容，作用就是会执行包下（就是指定包下的，不会递归子包）所有 GO 源码文件中定义的 `func init()` 方法

**引申**

1. GO 语言中，相同的包下，即使是不同文件，函数签名也不能相同

2. Java 中开发者无法为包取名为关键字，因为包名会体现在源码中，所以就会出现语法错误。而 GO 语言中是可以的，以 `package` 关键子为例，自定义包名为 `package` 的包，最终在源码会变为 `_package`（多了 `_`）

### type

最常用的用法就是定义结构体和接口了，详见具体的模块，这里提一个，替换的概念

```go
type Xxx string

// 像这样为 string 起一个别名为 Xxx，这样就可以为 Xxx 定义方法了
func (xxx *Xxx) test() {
	...
}

// 使用
abc := Xxx{"test"}
```

## 预定义标识符

string、append、cap、recover

close、copy、imag、real、bool

iota、len、make、new

panic、print、println

uintptr（无符号整型，用于存放一个指针）、nil（空值）

*布尔类型*：false、true

*无符号整数*：`uint8（0~255）别名 byte`、`uint16`、`uint32`、`uint64`

*有符号整数*：`int8（-128~127）`、`int16`、`int32 别名 rune`、`int64`

*浮点型*：float32（IEEE-754 32位浮点型数）、float64（IEEE-754 64位浮点型数）

*复数*：complex64（32位实数和虚数）、complex128（64位实数和虚数）

## 数据类型

可以参见 `GOROOT\src\builtin\builtin.go`

`int`, `uint` 和 `uintptr` 在 32 位系统上通常为 32 位宽，在 64 位系统上则为 64 位宽。当你需要一个整数值时应使用 `int` 类型，除非你有特殊的理由使用固定大小或无符号的整数类型

### 类型转换

​	**GO 语言不支持隐式转换类型，必须显式转换**

​	将 `int32` 类型的变量赋值给 `int64`，编译不通过

​	语法：`类型(表达式)`

- 可以将 `int` 转成 `float32`

- 通过 type 关键字定义的类型别名可以直接通过类型转换，而不是类型断言

### 类型断言

​	用自己的话来说，就是类比 Java 方法中 Object 类型的形参，其实真实的类型判断

**引出**：使用不同的接口类型来引用结构体实例时，只能调用接口类型对应的相关方法。举例，假如说有结构体 S 实现了 A 和 B 接口，现在我们拿到了 `val a A = S {...}` 的变量 a，那么我们如何通过 a 来调用 B 接口的方法？内存分析上是合理可行的，是的

​	GO 提供了类型断言：`value := i.(类型)`、`var value Type = i.(类型)`，拿上例来说，可以通过 变量a 获取其底层值，`val s S = a.(S)`，拿到了结构体类型的实例本身，就可以调用其实现的任何接口方法了

**再引出**：如果一个结构体没有实现接口，调用该结构体实例变量的进行接口类型的断言是编译不通过的，那么，如何知道一个接口的底层值是否实现了其他接口？

​	还有一种类型断言的变体，其语法是 `value, ok := i.(类型)`，该语法的意思是，如果 Type 实现了 i 的接口类型，那么 ok 为 true，否则 ok 为 false，value 为 Type 对应的结构体类型的零值，所以下面这样的写法是比较常见的

```go
func test(xxx {}interface) {
    value, ok = xxx.(int)
    if ok {
        // 断定 xxx 是 int 类型才执行的逻辑
    } else {
        ...
    }
}
```

- 其实再往深处说一些，就说到反射了，类型 → 接口 → 反射，这样的关联，详见 “反射三定律”

### 类型开关

​	用自己的话来说，就是可以对变量的类型进行 switch 进行判断

```go
func explain(i interface{}) {
    switch i.(type) {
        case string:
        	fmt.Println("string", strings.ToUpper(i.(string)))
        case int:
        	fmt.Println("int", strings.ToUpper(i.(int)))
    }
}
```

### 集合

#### 数组

- 语法

  ```go
  var 变量名 [数组大小] 数组存储的元素类型
  ```

-  初始化数组中 {} 中的元素个数不能大于 [] 中的数字

  ```go
  // 简单示例（注意，等号左右两边中括号中的数字得相同）
  var intArray [5]int = [5]int {0, 1, 2}
  // 类型自动判断 ↓
  var intArray = [5]int {0, 1, 2}
  // 简写 ↓
  intArray := [5]int {0, 1, 2}
  ```

- 如果忽略 [] 中的数字不设置数组大小，GO 语言会根据元素的个数来设置数组的大小

  ```go
  // 这样初始化，数组大小为4
  var intArray []int = []int {0, 1, 2, 3}
  ```

#### 切片（Slice）

​	GO 语言切片是对数组的抽象，GO 数组的长度不可改变，在特定场景中这样的集合就不太适用，GO中提供了一种灵活，功能强悍的内置类型切片("动态数组"),与数组相比切片的长度是不固定的，可以追加元素，在追加时可能使切片的容量增大

​	切片在类型上就是不指定具体大小的数组，通过 `make` 函数来创建切片

​	空切片（切片类型变量的零值）：并不是空（nil），而是 []，length：0，capacity：0

​	切片之间不同通过 `==` 来进行比较

- **基础示例**

  ```go
  /* 通过 make 函数直接构建一个 */
  /* type：切片存储的类型，length：底层数组长度，capacity：底层数组最大长度 */
  var slice1 []type = make([]类型, length)
  var slice2 []type = make([]类型, length, capacity)
  
  // make([]int, 3, 5) 初始化的时候底层的那个数组长度就是3，访问下标为3的元素将得到 panic: runtime error: index out of range [3] with length 3
  
  // make([]int, 3) 相当于 length：3，capacity：3
  ```

  ```go
  /* 通过一个已有的数组初始化 */
  s1 := s[<startIndex>:<endIndex>]
  // 所有参数都不能超过父数组或切片的参数，第三个参数不填默认和父数据组或切片一样
  s2 := s[<startIndex>:<endIndex>:<capacity>]
  arr := []int{1, 2, 3}
  // 下面 s1, s2, s3, s4 都是相同的
  s1 := arr[:]
  s2 = arr[0:2]
  s3 = arr[0:]
  s4 = arr[:2]
  ```

- **相关函数**

  1. 首先，先解释一下相关函数的含义

     append：向**切片**追加元素（如果 append 后切片的大小大于其 capacity，那么 append 底层会重新创建一个新数组，即切片的内存地址会发生改变）

     len：数组的长度

     cap：数组的实际长度（注意，这个 len - cap 区间的元素并不能直接通过下标访问）

     copy：切片之间的元素拷贝

  2. 然后，好好看下下面一段话

     ```go
     GO 语言为了尽可能的减少二次分配内存，并不是每一次都只增加一个单位的内存空间，而且遵循这样一种扩容机制：
     
     当有预留的未使用的空间时，直接对未使用的空间进行切片追加。当预留的空间全部使用完毕的时候，扩容的空间将会是当前的slice长度的一倍，例如当前slice的长度为4，进行一次append操作之后，cap(a)返回的长度将会是8
     ```

  3. 总结

     cap 并不是动态数组的最大长度上限，当切片长度到达这个值之后，会以默认两倍扩容的机制。这是开发者对数组最大长度的一个预估上限，这完全是处于对性能考虑的一个设计
  
- 例（解析 cap）

  ```
  numbers := []int{0,1,2,3,4,5,6,7,8}
  number3 := numbers[2:5]
  printSlice(number3)
  
  // cap 等于 7，是因为从下标为 2 的数开始（包括）后边总共有 7 个元素
  len=3 cap=7 slice=[2 3 4]
  ```

- 例（合并切片）

  ```go
  var arr1 = []int{1,2,3}
  var arr2 = []int{4,5,6}
  var arr3 = []int{7,8,9}
  // s1 {1, 2, 3, 4, 5, 6, 7, 8, 9}
  var s1 = append(append(arr1, arr2...), arr3...)
  ```

- 例（切片复制）

  ```go
  dst := make([]int, 0)
  src := []int{1, 2, 3}
  copy(dst, src)
  // len=3 cap=3 slice=[1 2 3]
  printSlice(src)
  // len=0 cap=0 slice=[]
  printSlice(dst)
  ```

- 例（数组默认是值传递、切片默认是引用传递）

  ```go
  func changeSlice(arr []int) {
      arr[0] = 1
  }
  func changeArray(arr [3]int) {
      arr[0] = 1
  }
  func changeArrayByPointer(arr *[3]int) {
      arr[0] = 1
  }
  
  func main() {
      arr1 := []int{0, 0, 0}
      // 相当于 arr2 := [3]int{0, 0, 0}
      var arr2 [3]int
      var arr3 [3]int
      
      // 切片默认引用传递：[1, 0 ,0]
      changeSlice(arr1)
      // 数组默认值传递：[0, 0 ,0]
      changeArray(arr2)
      // 数组地址传递：[1, 0 ,0]
      changeArrayByPointer(&arr3)
  }
  ```

  ```go
  // 切片的底层是指针数组的指针
  s1 := []int{1, 2, 3} // len=3, cap=3
  s2 := s1
  s1[0] = 0
  s1 = append(s1, 4)
  fmt.Println(s1, len(s1), cap(s1)) // 输出：[0 2 3 4] 4 6
  fmt.Println(s2, len(s2), cap(s2)) // 输出：[0 2 3] 3 3
  ```


#### 可变长参数

​	同 Java 语言，可变长参数指的是特殊的专门用于表示函数方法的形参类型

- 实例

  ```go
  func test(array ...int) {
  	fmt.Println(interfaces)
  }
  
  func main() {
      // 常规使用
      test(1, 2, 3)
  	// 也可以接收 Unpack slice 后的数组类型
  	var array = []int{1, 2, 3}
  	test(array...)
  }
  ```

#### Map

​	Map 是无序的，我们无法决定遍历时的返回顺序，因为 Map 是使用 hash 表来实现的

- 语法（声明）

  ```go
  /* 声明变量，默认 map 是 nil */
  var 变量名 map[键类型]值类型
  
  /* 初始化 */
  var testMap map[string]string = map[string]string{"键1": "值1", "键2": "值2", ...}
  
  /* 通过 make 函数初始化 */
  变量名 := make(map[键类型]值类型)
  ```

  不通过 make 初始化的 Map 变量默认是 nil，不能添加元素

- 语法（取值）

  ```go
  value := testMap["键名"]
  
  /* 可以用两个变量来接收返回值：value 是键对应的值，exist 是 bool 类型的值表示键对应的值是否存在 */
  value, exist := testMap["键名"]
  ```

- delete

  通过 delete 函数，删除键值对，语法：delete(map, 键)

### 指针

​	和 C 语言中指定的概念没有太大区别，指定类型的变量的默认零值是 nil

- 指针数组

  ```go
  var ptr []*int;
  ```

- 数组指针（指向数组的指针）

  ```go
  var ptr *[]int
  ```

- 还有如指向指针的指针等概念（在大学学习 C 语言的时候，都玩过了，这里不展开了）

  关于结构体类型和指向结构体的指针类型 GO 语言 和 C 语言是有区别的，在 C 语言中，结构体类型也是通过 `.`，但是结构体指针类型就是通过 `->` 来操作的，在 GO 语言中都是通过 `.` 来操作结构体的属性的


### 结构体

- 语法

  ```go
  type 结构体名 struct {
     变量名 数据类型
     变量名 数据类型
     ...
     变量名 数据类型
  }
  ```

- 示例

  ```go
  type Person struct {
      age int
      name string
  }
  
  // 使用方式一：不能缺少元素
  person1 := Person {10, "小明"}
  // 使用方式二：可以缺少元素
  person2 := Person { age: 10, name: "小红"}
  ```

- 特殊点

  通过 `fmt.print` 相关方法来打印一个结构体示例，会默认按照 "{字段1值 字段2值 ...}" 这样的格式。相较于 Java，一个相当于所有数据实体默认实现 `toString` 方法，二个，Java 中默认打印对象的地址在日常的业务场景是在是不实用
  
  结构体里边的变量也有变量名首字母大小写的权限理念

### 接口

- **语法**

  ```go
  // 定义起来，很简便，不用写没用的形参名
  type 接口名 interface {
    方法名(参数类型1, 参数类型2) 返回值类型1, 返回值类型2, ...
  }
  ```

- **概念**

  GO 语言中的接口是非入侵的，这不同于 Java 的强制实现要求，类比鸭子模型的思想，是说如果一个结构体实现了一个接口中定义的所有函数，那么就认为这个结构体实现了这个接口（详见[方法](#方法)）；

  定义一个接口类型的变量，无论是其值，还是其类型，得到的输出都是 nil

  如果一个结构体实现了一个接口，那么这个结构体的实例就能够被赋值给一个该接口类型的变量，硬要说 Go 语言有面向对象的多态，那可能就体现在这里了

- **空接口**

  根据上面的概念进行申义，空接口没有任何的接口方法，所以任何类型的结构体都能说实现了该接口，其实就这就是 `fmt.Println` 能够收任何类型变量的原因

- **嵌入式接口**

  在 GO 语言中，接口不能实现其他接口或扩展它们，但我们可以通过合并两个或多个接口来创建新接口

  ```go
  // 定义接口一
  type A interface {
      a()
  }
  
  // 定义接口二
  type B interface {
      b()
  }
  // 接口聚合
  type C interface {
      A
      B
  }
  
  // 结构体 S 实现了 A、B 接口，即它实现了聚合后的 C 接口
  type S struct {
      i int
  }
  func (s S) a() {}
  func (s S) b() {}
  ```

- **指针接收器**

  其实就是方法关于对应结构体的声明是否在接口体类型前面添加了 `*` 符号，如果添加了就要注意如下的问题

  ```go
  // 定义接口一
  type A interface {
      a1()
      a2()
  }
  
  type S struct {
      i int
  }
  func (s S) a1() {}
  // 注意这里加了个 *
  func (s *S) a2() {}
  
  func main() {
      // 声明一个 S 实例
      s := S {1}
      // 报错，因为上面的 *，报错主要是说“a2 method has pointer receiver”
      var a A = s
      // 需要写成这样
      var a A = &s
      
  }
  ```

## 变量

- 零值（变量初始值）

  数字类型的变量零值为 0

  bool 类型的变量零值为 false

  string 类型的变量零值为空字符串（""）

  *int、[]int、map[string] int、chan int、func(string) int、error接口类型的变量的零值为 nil

  >  Java 方法内的局部变量必须赋值，才能使用；GO 语言里边，不论是全局还是局部变量没有初始化，默认都会有个零值

- **自行判定变量类型**

  ```go
  var flag = true
  ```

- 快速定义（`:=`）

  `var a int = 1` 等同于 `var a = 1` 等同于 `a := 1`，语法规则是语句的执行结果是必须产生一个新的变量

  ```go
  var intVal int = 1
  // 编译不通过（无法产生名为 intVal 的变量，已经声明定义过了）
  intVal := 1
// 编译通过（成功产生了名为 intVal2 的变量，同时还将 intVal 变量重新赋值了）
  intVal, intVal2 = 1, 2
  ```
- 空白标识符 `_`

  主要作为不被使用的变量名，甚至可以用该符号作为变量名在一个局部内重复定义变量，空白标识符 `_` 也被用于抛弃值，如值 5 在：_, b = 5, 7 中被抛弃

  `_` 实际上是一个只写变量，你不能得到它的值。这样做是因为 GO 语言中你必须使用所有被声明的变量，但有时你并不需要使用从一个函数得到的所有返回值

- 全局变量（在函数、方法体之外定义的变量）

  注意，全局变量定义无法使用 `:=`

  ```go
  // 一般采用因式分解关键字的格式定义
  var (
      a int
      b bool
  )
  ```
  
- 变量值交换

  这个单独拿出来出说，GO 语言中特别的支持一种方便的交换两个变量值的语法（JDK8 C++ C# 中都是没有，Python 中是有的）

  ```go
  // 声明两个要交换值的变量
  a, b := 1, 2
  
  // 1、常规做法（通过中间变量）
  c := a
  a = b
  b = c
  
  // 2、通过方法
  func swap(a, b *int) {
      c := *a
      *a = *b
      *b = c
  }
  
  // 3、特有
  a, b = b, a
  // 当然 2 中也可以这样
  *a, *b = *b, *a
  ```

- **并行（同时）赋值**

  ```go
  // 基本示例
  int val1, val2, val3 = 1, 2, 3
  // 交换两个变量的值
  a, b = b, a
  // 接收函数的多返回值
  val, err = Func1(var1)
  ```

- **局部同名**

  ```go
  var sum int
  func test() {
      // 没有问题，不会报错，这和 Java 的就不同，Java 不允许在一个局部内的变量名歧义（额...this例外）
      // 并且是没有途径可以
      var sum int
  }
  ```

### 值传递

GO 语言中，方法传参的变量赋值都是值传递，也就是在方法调用时，会创建一个副本，来执行函数逻辑。就是重点强调一下，指针就是一种特殊的类型，方法间传参也是完全遵守值传递的概念的

### 引用传递

**基础概念和理论**：

1. 复杂的数据通常会需要使用多个字，这些数据一般使用引用类型保存
2. 一个引用类型的变量 r1 存储的是 r1 的值所在的内存地址（数字），或内存地址中第一个字所在的位置（切片的设计理念），这个内存地址称之为指针，这个指针实际上也被存在另外的某一个值中
3. 同一个引用类型的指针指向的多个字可以是在连续的内存地址中（内存布局是连续的），这也是计算效率最高的一种存储形式；也可以将这些字分散存放在内存中，每个字都指示了下一个字所在的内存地址

**从实际语法应用角度：**

```go
func changeValue(sum int) {
    sum = 1
}

func main() {
    sum := 0
    // 如果 GO 是引用传递，那么经过下面的方法，sum 的值将会是 1
    changeValue(sum)
}
```

### 方法中的值传递、引用传递

```go
package main

import (
   "fmt"  
)

/* 定义结构体 */
type Circle struct {
  radius float64
}

func main()  { 
   var c Circle
   fmt.Println(c.radius) // 0
   c.radius = 10.00
   fmt.Println(c.getArea()) // 100
   c.changeRadius(20)
   fmt.Println(c.radius) // 20
   change(&c, 30)
   fmt.Println(c.radius) // 30
}
func (c Circle) getArea() float64  {
   return c.radius * c.radius
}
// 注意如果想要更改成功c的值，这里需要传指针
func (c *Circle) changeRadius(radius float64)  {
   c.radius = radius
}

// 方式二：调用函数修改，引用类型的值
func change(c *Circle, radius float64)  {
   c.radius = radius
}
```

### 注意

我们在日常开发，编写 GO 代码时，是否符合方法的定位以及功能并不是形参是否采用指针类型的唯一考量因素，因为值传递，就意味着副本的创建，视不同的情况，可能会相当损耗性能，但是传递的是指针类型，值传递的副本拷贝的也只是一个记录内存地址的指针类型，代价忽略不计

## 常量

常量是一个简单值的标识符，在程序运行时，不会被修改的量

- 定义

  ```go
  /* 显式 */
  const b string = "abc"
  
  /* 隐式 */
  const b = "abc"
  ```

- 定义常量组

  ```go
  const (
  	a = 1
      b
      c
  )
  
  // 实际值 a = 1，b = 1，c = 1
  ```

- 作为枚举使用

  ```go
  const (
      Unknown = 0
      Female = 1
      Male = 2
  )
  ```

- 不能将结构体实例定义为常量

### iota

​	特殊常量，可以认为是一个可以被编译器修改的常量

​	iota 在 const关键字出现时将被重置为 0(const 内部的第一行之前)，const 中每新增一行常量声明将使 iota 计数一次(iota 可理解为 const 语句块中的行索引)

```go
// 示例一（a=0, b=1, b=2）
const (
    a = iota
    b = iota
    c = iota
)

// 示例二（效果同上）
const (
    a = iota
    b
    c
)

// 示例三
const (
    a = iota   // 0
    b          // 1
    c          // 2
    d = "ha"   // 独立值，iota += 1
    e          // "ha"   iota += 1
    f = 100    // iota +=1
    g          // 100  iota +=1
    h = iota   // 7,恢复计数
    i          // 8
)

// 示例四
const (
    i=1<<iota // 1
    j=3<<iota // 6
    k // 12
    l // 24
)
```

## 运算符

​	最基础的运算符就不赘述了，这里重点提一下 GO 语言比较标志性的运算符。类比其他语言来说，C 语言有指针、值传递、引用传递，像 Java 中没有指针、引用传递的概念，GO 语言和 C 语言比较像，存在引用传递的概念

| 运算符 | 描述             | 示例                     |
| ------ | ---------------- | ------------------------ |
| &      | 返回变量存储地址 | &a; 将给出变量的实际地址 |
| *      | 指针变量         | *a; 是一个指针变量       |

- 自增、自减操作只能作为表达式使用，而不能用于赋值语句

## 条件语句

​	首先要提及的是，GO 语言没有三目运算符（? :）的概念

- **if**

  就传统 if 的语法 `if (条件) {...}` 来说，编译可以通过，但是 GO 语言中不推荐在整个条件表达式的左右加上括号

  然后就是还有一个可以类比 for 三段语句的二段语句的语法糖，随便举一个很简单的例子

  ```go
  if xxx := 表达式; xxx > 0 {
  
  }
  ```

- **switch**

  和传统 swith 的语法相比，GO 语言中主要有一下几点不同、拓展

  1. 可以理解为，case 下默认在最后一条语句加上 break 的；如果想要继续执行，可以在语句最后加上 `fallthrough` 关键字
  2. case 后边可以跟多个值，用逗号隔开
  3. 可以根据变量的类型进行条件分支

- **select**

  select 是 GO 中的一个控制结构，类似于用于通信的 switch 语句。每个 case 必须是一个通信操作，要么是发送要么是接收

  select 随机执行一个可运行的 case。如果没有 case 可运行，它将阻塞，直到有 case 可运行。一个默认的子句应该总是可运行的

  **概念**

  ```markdown
  - 每个 case 都必须是一个通信
  
  - 所有 channel 表达式都会被求值
  
  - 所有被发送的表达式都会被求值
  
  - 如果任意某个通信可以进行，它就执行，其他被忽略。
  
  - 如果有多个 case 都可以运行，Select 会随机公平地选出一个执行。其他不会执行。 
  否则：
  1. 如果有 default 子句，则执行该语句。
  2. 如果没有 default 子句，select 将阻塞，直到某个通信可以运行；GO 不会重新对 channel 或值进行求值
  ```

  **语法**

  ```go
  select {
      case communication clause  :
         statement(s);      
      case communication clause  :
         statement(s);
      /* 你可以定义任意数量的 case */
      default : /* 可选 */
         statement(s);
  }
  ```

## 循环

​	首先，GO 语言中只有 **for** 关键字

- 语法

  标准结构：`for init; condition; post {...}`

  同其他语言的 while 结构：`for condition {...}`

  死循环：for {...}、for true {...}

- For-each

  range 关键字用于 for 循环中迭代数组(array)、切片(slice)、通道(channel)或集合(map)的元素

  ```go
  // map
  for key, value := range oldMap {
      newMap[key] = value
  }
  
  // 数组
  numbers := [6]int{1, 2, 3, 5}
  for i,x:= range numbers {
      fmt.Printf("第 %d 位 x 的值 = %d\n", i,x)
  }
  
  第 0 位 x 的值 = 1
  第 1 位 x 的值 = 2
  第 2 位 x 的值 = 3
  第 3 位 x 的值 = 5
  第 4 位 x 的值 = 0
  第 5 位 x 的值 = 0
  ```

  这里重点提一下，就是使用 range 关键字遍历，开始遍历时，会创建一个副本，所以在遍历切片时，去增删切片中的元素，并不会改变实际要遍历的元素，但是修改元素的值，是有影响的

- 坑

  ```go
  a := 0
  for a:= 0; a < 10; a++ {
      // ...
  }
  // 0
  fmt.Println(a)
  ```

## 函数和方法

### 函数

​	GO 语言程序中，至少有一个 `main` 函数

- GO 语言标准库提供了多种可动用的内置的函数。例如，len() 函数可以接受不同类型参数并返回该类型的长度。如果我们传入的是字符串则返回字符串的长度，如果传入的是数组，则返回数组中包含的元素个数

- 语法

  ```markdown
  func 方法名(参数类型1, 参数类型2, ...) 返回值类型 {
     函数体
  }
  ```

- 多返回值

  ```go
  // 多个返回值类型之间用逗号“,”隔开，且所有参数要被小括号“()”括起来
  // 方式一：通过 return 关键字
  func numbers() (int, int, string) {
    a , b , c := 1 , 2 , "str"
    return a,b,c
  }
  
  // 方式二：定义形参变量，调用 return 时没有跟要返回的参数，则将形参变量的值作为返回值
  func numbers() (val1 int, val2 int, val3 string) {
    a , b , c := 1 , 2 , "str"
    return
  }
  ```

- 无用的方法参数

  ```go
  func test(val1 int, val2 int) {
      // 一堆逻辑，但是没有用到变量 val1 和 val2
  }
  
  // 那么推荐改成如下的形式
  func test(_ int, _ int) {}
  
  // 或者就不标注形参变量
  func test(int, int) {}
  ```

- 函数类型的变量

  ```go
  // 声明函数变量
  getSquareRoot := func(x float64) float64 {
      return math.Sqrt(x)
  }
  
  fmt.Println(getSquareRoot(9))
  
  // 函数变量作为函数参数
  func test(function func(float64) float64) {...}
  
  // 函数变量作为返回值类型（举一个语法都体现了，但是没有什么实际含义的例子）
  func test2() func(float64) (float64, string) {
  	function := func(f float64) (float64, string) {
  		return f, ""
  	}
  	return function
  }
  ```

- 匿名函数

  GO 语言支持匿名函数，可作为闭包。匿名函数是一个"内联"语句或表达式。匿名函数的优越性在于可以直接使用函数内的变量，不必申明

  ```go
  package main
  
  import "fmt"
  
  func getSequence() func() int {
     i:=0
      // 匿名函数在这里
     return func() int {
        i+=1
       return i  
     }
  }
  
  func main(){
     /* nextNumber 为一个函数，函数 i 为 0 */
     nextNumber := getSequence()  
  
     /* 调用 nextNumber 函数，i 变量自增 1 并返回 */
     fmt.Println(nextNumber()) // 1
     fmt.Println(nextNumber()) // 2
     fmt.Println(nextNumber()) // 3
     
     /* 创建新的函数 nextNumber1，并查看结果 */
     nextNumber1 := getSequence()  
     fmt.Println(nextNumber1()) // 1
     fmt.Println(nextNumber1()) // 2
  }
  ```

### 方法

​	GO 语言中同时有函数和方法。一个方法就是一个包含了接受者的函数，接受者可以是命名类型或者结构体类型的一个值或者是一个指针。所有给定类型的方法属于该类型的方法集

- 自己的话

  在 GO 语言中，面向过程编程 对应 函数，面向对象编程 对应 方法。GO 语言语法、代码结构给人的直接的感觉，就不会说像 Java 那样完全的面向对象的编程语言，可以说正是因为有了方法的概念，才可能说将某一个方法绑定给特定的数据结构体（在 Java 中就是一个单独的 POJO）使用，GO 语言没有面向对象，但是这里可以体现出面向对象的思想

  另外，GO 语言中没有像 C++、Java 中 this 关键字的概念，在 GO 里，这个 this 指针需要明确的申明出来，其实和其它 OO 语言并没有很大的区别

- 语法

  ```go
  /* 声明（注意，只有加上了“*”，通过“变量名.属性=xxx”才能赋值成功，可以通过打印内存地址来验证） */
  func (变量名 *结构体类型) 方法名() [返回值类型]{...}
  
  /* 调用（注意，方法是不能脱离结构体示例存在的 - 不仅体现在定义，调用时也是） */
  var 变量名 结构体类型 = 结构体类型 {...}
  变量名.方法名()
  ```

- 基本示例

  ```go
  package main
  
  import (
     "fmt"  
  )
  
  /* 定义结构体 */
  type Circle struct {
    radius float64
  }
  
  func main() {
    var c1 Circle
    c1.radius = 10.00
    fmt.Println("圆的面积 = ", c1.getArea())
  }
  
  //该 method 属于 Circle 类型对象中的方法
  func (c Circle) getArea() float64 {
    //c.radius 即为 Circle 类型对象中的属性
    return 3.14 * c.radius * c.radius
  }
  ```

## 错误处理

​	用自己的话来说，就是执行 `panic("错误信息")` 相当于 Java 中抛出了指定异常提示信息的异常，然后，继续类比 Java，`defer` 相当于 `catch`，不同的是，某一个位置调用 `panic` 抛出异常，GO 会先在当前函数方法体内，从 `panic` 的执行位置，向上，没错，是向上，开始找定义的 `defer` 修饰的函数（为了方便，下文会简称为 `defer` 函数），即异常处理逻辑，会由下至上的逆序将所有 `defer` 函数都执行一遍。如果在当前函数方法体内的某一个 `defer` 函数中执行到了 `recover` 函数，则认为错误或异常处理了、恢复了，调用当前方法的外面一层的方法栈会继续执行，而如果没有执行 `recover` 函数，相当于异常被抛出了，也就是回到了这段话的开头，相当于外层方法栈实际执行了 `panic` 函数一样。

**补充1**：就是内层执行到了 `recover`，表示异常被处理了，外层继续执行，当执行完了，还会从方法末尾开始由下至上的执行外层的 `defer` 函数

**补充2**：在 `defer` 函数中也是可以调用 `panic` 函数的，这时的效果是，时间上，后面调用的 `panic` 会覆盖前面调用的 `panic`

**补充3**：上面的现象描述的没毛病，但是不能将 `defer` 比拟成 Java 中的 `catch`，它应该被比拟成 `finally`。一个，`defer` 中可以不执行 `recover`，那么 `defer` 可就和 `panic` 没什么关系了；二个，从未解释清楚的 `补充1` 来看，无论是否发生异常，都会执行。说结论，`defer` 英文单词的意思是 延缓、推迟，所以，这个关键字只能类比 Java 的 `finally`，但是，延时才是它的核心理念，这也就解释了 `补充1` 里，`defer` 被执行的原因

### defer

其实这个关键字不应该被归为错误处理这块，但是日常中使用的最多的就是资源回收、错误处理，这个关键字就按照这个单词的释义，理解为延迟就好

- 语法

```go
// defer 修饰的函数可以定义返回值类型，但是不能添加参数
defer func() {
    if err := recover(); err != nil {
        // 这里 err 就是 panic 的内容，# 的意思是要以被双引号引起来的格式打印
        fmt.Printf("%#v\n", err)
    }
}()
```

```go
// 这样的写法也是可以的
func catch() {
    recover()
}

func main() {
    defer catch()
    panic("发生了未知异常")
}
```

- 这里有一个坑（defer在匿名返回值和命名返回值函数中的不同表现）

```go
// 这要从匿名和命名的底层说起，就是匿名本质上就是GO会为你额外声明一个有名字的，假定变量名是 value
// 那下面这个方法的返回流程是这样的
// 1、将 result（0） 赋值给 value
// 2、result 自增
// 3、返回 value
func returnValue() int {
    var result int
    defer func() {
        result++
    }()
    // 返回的是 0
    return result
}

// 这个与上面的不同在于显示声明定义了返回值变量，GO就不会再额外声明一个
func returnValue() (result int) {
    defer func() {
        result++
    }()
    // 返回的是 1
    return result
}
```

- 性能影响

```go
// defer会对其后需要的参数进行内存拷贝，还需要对 defer 结构进行压栈出栈操作，在循环中定义 defer 可能导致大量的资源开销
// 下面给出反例
func deferInLoops() {
    for i := 0; i < 100; i++ {
        f, _ := os.Open("/etc/hosts")
        defer f.Close()
    }
}

// 注意！！！defer 的作用区域是函数，而不是代码块，所以上面所有 defer 后面语句的执行时机是循环执行完毕，这是大误区
```

- 延时操作的值要注意空指针问题
- 同 Java 中调用 System.exit 就是直接退出程序，GO 里边调用 os.Exit 也是直接推出，注意，在这之前定义的 `defer` 就都不会执行了
- `defer` 后面可以直接跟语句，并不限定一定是函数

### panic 函数

1. 发生 `panic` 后，程序会从调用 `panic` 的函数位置或发生 `panic` 的地方立即返回，逐层向上执行函数的 `defer` 语句，然后逐层打印函数调用堆栈，直到被 `recover` 捕获或运行到最外层函数

2. `recover` 用来捕获 `panic`，阻止 `panic` 继续向上传递。`recover()` 和 `defer` 一起使用

- recover

- error

  ```go
  func Sqrt(f float64) (float64, error) {
      if f < 0 {
          return 0, errors.New("math: square root of negative number")
      }
  }
  ```

## 并发

​	GO 语言天生支持并发，这样一句话，想必听的确实不少，对于很多初学者，肯定都有些疑惑。说的好像，C、JAVA、PYTHON等编程语言不支持一样，其实这句话应该抓“天生”这个重点，拿 JAVA 来说，JAVA 诞生于1996年，确实有些年头了，那个时候的计算机因为硬件资源的限制，普遍都是单核，这样一个事实，这样一个道理，就是说，JAVA 需要预测计算机硬件资源未来的走向，这是看不清楚的，不断摸索，不断探索；然后说 GO，其实不只是 GO，新的开发语言都能够基于现在的计算机硬件资源，这是看的见的，并参考其他优秀的主流开发语言的优劣和经验，如果语言的开发人员有着足够的开发经验和架构设计经验，并且他们富有想法，敢于实践。这样分析，新的开发语言必然有老语言无法比拟的优势。注意，这样并不是说，GO 以及其他新的开发语言就都能够替代老的语言了。因为，其实说老语言都有些不合适，不同的语言在各自的领域深耘了那么多年，并在如今也仍在继续发展迭代，乍看，语言之间各自有擅长的领域，应该说不会有冲突和竞争，但实际不然，一个语言真正能够发展走多远是要看新血液的输入，真正有兴趣的入门者的数量。道理很简单，一个，性能最高的语言当然是C，是汇编，但是事实是，C并没有成为远远领先其他语言的佼佼者。说到语言发展，也不得不提一下，走向末路的PHP，几乎所有相关从业者都表示前景黯淡，一个计算机相关专业的学生，想要学编程，选择一门开发语言的时候，听学长、前辈的建议，无论是什么，肯定不是 PHP。说回来，先说一下并发模型，引用知乎上的一段话

```
并发模型包含 IO 多路复用、多进程以及多线程，这几种模型都各有优劣，现代复杂的高并发架构大多是几种模型协同使用，不同场景应用不同模型，扬长避短，发挥服务器的最大性能。而多线程，因为其轻量和易用，成为并发编程中使用频率最高的并发模型，包括后衍生的协程等其他子产品，也都基于它
```

GO 语言的主要通过其 Goroutine 来实现的轻量、IO 多路复用的线程模型实现的高性能，并且语法简单，假如我们希望开启一个并发执行的任务，只要通过 `go 函数名/方法名`

- 同一个程序中的所有 goroutine 共享同一个地址空间
- 主函数结束了，goroutine 就会结束

### 通道（channel）

​	通道可用于两个 goroutine 之间通过传递一个指定类型的值来同步运行和通讯。操作符 `<-` 用于指定通道的方向，发送或接收。如果未指定方向，则为双向通道

​	通道可以作为两个 goroutine 之间的通信工具

- 通道类型

  ```
  go func(c chan int) { // 读写均可的channel c } (xxx)
  go func(c <- chan int) { // 只读的Channel } (xxx)
  go func(c chan <- int) {  // 只写的Channel } (xxx)
  ```

- 基本示例

  ```go
  // 声明
  ch := make(chan int)
  // 把 v 发送到通道 ch
  ch <- v
  // 从 ch 接收数据，并把值赋给 v
  v := <-ch
  ```

- 示例

  ```go
  package main
  
  import "fmt"
  
  func sum(s []int, c chan int) {
          sum := 0
          for _, v := range s {
                  sum += v
          }
          c <- sum // 把 sum 发送到通道 c
  }
  
  func main() {
          s := []int{7, 2, 8, -9, 4, 0}
  
          c := make(chan int)
          go sum(s[:len(s)/2], c)
          go sum(s[len(s)/2:], c)
          x, y := <-c, <-c // 从通道 c 中接收
  
      	// -5 17 12
          fmt.Println(x, y, x+y)
  }
  ```

- 缓冲区：默认情况下，通道是不带缓冲区的。发送端发送数据，同时必须有接收端相应的接收数据

  ```go
  ch := make(chan int, 100)
  ```

  带缓冲区的通道允许发送端的数据发送和接收端的数据获取处于异步状态，就是说发送端发送的数据可以放在缓冲区里面，可以等待接收端去获取数据，而不是立刻需要接收端去获取数据

  不过由于缓冲区的大小是有限的，所以还是必须有接收端来接收数据的，否则缓冲区一满，数据发送端就无法再发送数据了

  **注意**：如果通道不带缓冲，发送方会阻塞直到接收方从通道中接收了值。如果通道带缓冲，发送方则会阻塞直到发送的值被拷贝到缓冲区内；如果缓冲区已满，则意味着需要等待直到某个接收方获取到一个值。接收方在有值可以接收之前会一直阻塞

  实例

  ```go
  package main
  
  import "fmt"
  
  func main() {
      	// 这里我们定义了一个可以存储整数类型的带缓冲通道
          // 缓冲区大小为2
          ch := make(chan int, 2)
  
          // 因为 ch 是带缓冲的通道，我们可以同时发送两个数据
          // 而不用立刻需要去同步读取数据
          ch <- 1
          ch <- 2
  
          // 获取这两个数据
          fmt.Println(<-ch)
          fmt.Println(<-ch)
  }
  ```

- 关闭通道并不会丢失里面的数据，只是让读取通道数据的时候不会读完之后一直阻塞等待新数据写入

## 测试

Go拥有一个轻量级的测试框架，它由 `go test` 命令和 `testing` 包构成。

你可以通过创建一个名字以 `_test.go` 结尾的，包含名为 `TestXXX` 且签名为 `func (t *testing.T)` 函数的文件来编写测试。 测试框架会运行每一个这样的函数；若该函数调用了像 `t.Error` 或 `t.Fail` 这样表示失败的函数，此测试即表示失败。

## 跨平台编译

​	跨平台编译又叫交叉编译，其实原理就是通过参数的形式指定目标操作系统的平台和处理器架构就可以了

- 参数举例说明

  ```shell
  SET CGO_ENABLED=0 // 禁用CGO
  SET GOOS=linux    // 目标平台是 linux
  SET GOARCH=amd64  // 目标处理器架构是 amd64
  ```

- 不同的环境的编译条件实例（不能实际执行，局部环境变量的设置需要通过不同平台特定的命令设置）

  Windows：SET

  Linux、Mac：EXPORT

  ```shell
  Windows → Linux：CGO_ENABLE=0 GOOS=linux   GOARCH=amd64 go build
  Windows → Max：  CGO_ENABLE=0 GOOS=darwin  GOARCH=amd64 go build
  Linux → Mac：    CGO_ENABLE=0 GOOS=darwin  GOARCH=amd64 go build
  Linux → Windows：CGO_ENABLE=0 GOOS=windows GOARCH=amd64 go build
  Max → Linux：    CGO_ENABLE=0 GOOS=linux   GOARCH=amd64 go build
  Max → Windows：  CGO_ENABLE=0 GOOS=windows GOARCH=amd64 go build
  ```

## 其他

### gops

​	这是用于查看系统正在运行的 go 程序有哪些的命令，安装后，就可以使用 gops 指令查看所有的 go 程序进程

- 安装

  ```
  go get -u github.com/google/gops
  ```
