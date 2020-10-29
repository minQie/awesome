# Go

## 声明

​	全文全部基于《菜鸟教程》，加上自己的实战记录组成

## 简介

​	计算机软件经历了数十年的发展，形成了多种学术流派，有面向过程编程、面向对象编程、函数式编程、面向消息编程等，这些思想究竟孰优孰劣，众说纷纭。

​	除了OOP外，近年出现了一些小众的编程哲学，Go语言对这些思想亦有所吸收。例如，Go语言接受了函数式编程的一些想法，支持匿名函数与闭包。再如，Go语言接受了以Erlang语言为代表的面向消息编程思想，支持goroutine和通道，并推荐使用消息而不是共享内存来进行并发编程。总体来说，Go语言是一个非常现代化的语言，精小但非常强大

​	go 语言程序可以有多个标记组成，可以是关键字，标识符，常量，字符串，符号，一个标识符实际上就是一个或多个字母（A~Z 和 a~z）、数字（0~9）、下划线（_）组成的序列，但是第一个字符必须是字母或者下划线，不能是数字

### Go 和 Golang

详见  [知乎](https://www.zhihu.com/question/39508749)

### Go 语言最主要的特性

- 自动垃圾回收
- 更丰富的内置类型
- 函数多返回值
- 错误处理
- 匿名函数和闭包
- 类型和接口
- 并发编程
- 反射
- 语言交互性

### Go 语言程序的基础组成部分

- 包声明
- 引入包
- 函数
- 变量
- 语句 & 表达式
- 注释

## 基础环境

1. [下载 Go 的 SDK](https://golang.google.cn/dl/)，下载解压版，并配置环境变量

   | 变量名 | 变量值                                                       |
   | ------ | ------------------------------------------------------------ |
   | GOROOT | 你的 Go SDK 目录                                             |
   | GOPATH | 如果你指定了一个全局的 GOPATH 就配置一下，可能配置多个，用分号隔开 |
   | path   | 添加 %GOROOT%\bin（最后不要添加“\”）                         |
   |        | 添加 %GOPATH%\bin                                            |

2. [下载 IntelliJ Golang](https://www.jetbrains.com/go/)，破解（见破解文件）

### GoRoot

Go SDK 的根目录路径

### GoPath

- GoPath 之下主要包含三个目录：src（源文件）、bin（可执行文件）、pkg（编译好的库文件, 主要是*.a文件）
- GoPath 是作为编译后二进制的存放目的地（bin）和import包（src）时的搜索路径，也就是你的工作目录（不要把 GoPath 设置成 GoRoot）

- GoPath 中的可执行目录也配置到环境变量中, 否则你自行下载的第三方go工具就无法在任意目录下使用了

- 一般在 GoPath 下使用 `go get` 和 `go install` 命令对来项目进行编译，这样特定类别的文件就会生成在指定的文件目录下了，`go build` 就会直接在源码所在目录生成对应的可执行文件
- 对于GO标准库而言，import的时候不需要写全路径，而对于 GOPATH 中的包而言需要根据目录层次写清楚包的path

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

- 通过 `go build hellowworld.go` 就可以得到 `helloworld.exe` 文件，然后通过 `.\helloworld`  直接运行得到程序输出结果

## 概念解释

1. 包的定义声明

   关键字：package

   说明：每个 go 语言程序都应该包含名为 main 的包

2. 引入包

   关键字：import

   说明：`import "fmt"` 的意思是告诉Go 编译器这个程序需要使用 fmt 包（的函数，或其他元素）。fmt 包实现了格式化 IO（输入/输出）的函数

3. 主函数定义

   `func main()` 是程序开始执行的函数。main 函数是每一个可执行程序所必须包含的，一般来说都是在启动后第一个执行的函数。而如果有 `init()` 函数则会先执行该函数

4. 注释

   单行：`// xxx`

   多行：`/* xxx */`

5. 简单函数调用说明

   `fmt.println("xx")` 等同于 `fmt.print("xxx\n")`

6. 标识符首字母是否大写的含义（权限）

   标识符（包括常量、变量、类型、函数、结构字段）的首字母是否大写，可以决定该对象能够被外部包所使用（当然，其他包下的程序需要先导入这个包），这个被称为导出（类比 Java 语言中的 `public`）；同理，标识符如果以小写字母开头，则类比 Java 语言中的 `protected`

   用自己的话来说就是：Java 语言中通过相关权限修饰符来表述和修饰权限，Go 语言中只要通过代码风格（变量名、函数名的首字母的大小写），个人觉得是非常好的点，因为换个角度来说，就这是站在语言层面上对代码更进一步的约定和规范

7. go 的代码风格

   `{` 左括号是不允许在单独的行上的

   通常 go 语言不需要语句结束符号（这些都将由 Go 编译器自动完成），但是如果多条语句在一行时，可以使用 `;` 分号进行分隔，但是注意，并不推荐这样的写法，从 `gofmt` 代码优化指令，会将你的单行分号分隔写法优化掉就可以知道了

## gofmt

**简介**

​	go 语言官方提供的 SDK 的 bin 目录下边除了 `go.exe` 还有一个 `gofmt.exe`，这个程序主要是用于格式化代码的（JDK 中并没有类似的概念，但是 IntelliJ IDEA 提供了）

**帮助命令**

​	通过 `gofmt --help` 指令，可以查看详细的命令帮助，简单介绍一下参数

​	-l：代码优化会影响的文件

​	-s：将优化的文件的优化结果输出到控制台

​	-w：只有使用了该参数，才会真的将优化文件后的结果写到源码文件中

## 包

**简介**

​	*go 源码文件名* 以及 *go 源码文件所在的文件夹名* 和 *包名* 没有直接联系，但是同一个文件夹下的 go 文件 只能有一个包名

**引申**

​	go 语言中，相同的包下，即使是不同文件，函数签名也不能相同

## 关键字

break、default、func、interface

select、case、defer、go

map、struct、chan、else

goto、package、switch、const

failthrough、if、range、type

continue、for、import、return

var

## 预定义标识符

string、append、cap、recover

close、copy、imag、real、bool

iota、len、make、new

panic、print、println

布尔类型：false、true

*无符号整数*：unit8（0~255）、unit16、unit32、unit64

*有符号整数*：int8（-128~127）、int16、int32、int64

*浮点型*：float32（IEEE-754 32位浮点型数）、float64（IEEE-754 64位浮点型数）

*复数*：complex64（32位实数和虚数）、complex128（64位实数和虚数）

rune（类似int32）、byte（类似 unit8）、unit（32位 或 64位）、int（同unit）

unitptr（无符号整型，用于存放一个指针）、nil（空值）

## 数据类型

- 布尔

  值只能为 true 或 false

- 数字

  整型 int 和 浮点类型 float32、float64，Go 语言还支持复数，其中位运算采用补码

- 字符串

  字符串就是一串固定长度的字符序列，Go 的字符串是由单个字节连接起来的。Go 语言的字符串的字节使用 UTF-8 编码表示 Unicode 文本

- 派生类型

  指针

  数组

  结构化（struct）

  Channel

  函数

  切片

  接口（interface）

  Map

### 类型转换

​	**Go 语言不支持隐式转换类型**（将 int32 类型的变量赋值给 int64，报错）

​	语法：`类型(表达式)`

​	可以将 `int` 抓成 `float32`

### 数组和切片

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

- 如果忽略 [] 中的数字不设置数组大小，Go 语言会根据元素的个数来设置数组的大小

  ```go
  // 这样初始化，数组大小为4
  var intArray []int = []int {0, 1, 2, 3}
  ```

#### 切片（Slice）

​	Go 语言切片是对数组的抽象，Go 数组的长度不可改变，在特定场景中这样的集合就不太适用，Go中提供了一种灵活，功能强悍的内置类型切片("动态数组"),与数组相比切片的长度是不固定的，可以追加元素，在追加时可能使切片的容量增大

​	切片在类型上就是不指定具体大小的数组，通过 `make` 函数来创建切片

​	空切片（切片类型变量的零值）：并不是空（nil），而是 []，length：0，capacity：0

- 语法（初始化）

  ```go
  /* 方式一 */
  var slice1 []type = make([]type, len)
  
  // 简化 ↓（len：最大长度）
  slice1 := make([]type, len)
  
  // 还可以指定长度 ↓（length：初始长度，capacity：最大长度）
  // 例1，make([]int, 3, 5) 初始化的时候底层的那个数组长度就是3，访问下标为3的元素将得到 panic: runtime error: index out of range [3] with length 3
  // 例2，make([]int, 3) 相当于 length：3，capacity：3
  make([]type, length, capacity)
  
  /* 方式二（通过一个已有的数组初始化） */
  arr := []int{1, 2, 3}
  // 语法：s1 := s[<startIndex>:<endIndex>]，下面 s1, s2, s3, s4 都是相同的
  s1 := arr[:]
  s2 = arr[0:2]
  s3 = arr[0:]
  s4 = arr[:2]
  ```

- make 函数

  1. 首先，先解释一下相关函数的含义

     append：向**切片**追加元素

     len：数组的长度

     cap：数组的实际长度（注意，这个 len - cap 区间的元素并不能直接通过下标访问）

     copy：切片之间的元素拷贝

  2. 然后，好好看下下面一段话

     ```go
     Go 语言为了尽可能的减少二次分配内存，并不是每一次都只增加一个单位的内存空间，而且遵循这样一种扩容机制：
     
     当有预留的未使用的空间时，直接对未使用的空间进行切片追加。当预留的空间全部使用完毕的时候，扩容的空间将会是当前的slice长度的一倍，例如当前slice的长度为4，进行一次append操作之后，cap(a)返回的长度将会是8
     ```

  3. 总结

     cap 并不是动态数组的最大长度上限，当切片长度到达这个值之后，会以默认两倍扩容的机制。这是开发者对数组最大长度的一个预估上限，这完全是处于对性能考虑的一个设计

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

- 关于结构体类型和指向结构体的指针类型 Go 语言 和 C 语言是有区别的

  在 Go 语言中都是通过 `.` 来操作结构体的属性的

  在 C 语言中，结构体类型也是通过 `.`，但是结构体指针类型就是通过 `->` 来操作的

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

### 接口

- **语法**

  ```go
  // 定义起来，很简便，不用写没用的形参名
  type 接口名 interface {
    方法名(参数类型1, 参数类型2) 返回值类型1, 返回值类型2, ...
  }
  ```

- **概念**

  在 Go 语言中，实现接口的是方法，不是函数，所以实现接口离不开特定的结构体（详见[方法](#方法)）；并且不像 Java 语言中先以实现接口为标志申明，Go 语言中，如果一个结构体实现了一个接口中定义的所有方法，那么就认为这个结构体实现了这个接口

  定义一个接口类型的变量，无论是其值，还是其类型，得到的输出都是 nil

  如果一个结构体实现了一个接口，那么这个结构体的实例就能够被复制给一个该接口类型的变量

- **空接口**

  根据上面的概念进行申义，空接口没有任何的接口方法，所以任何类型的结构体都能说实现了该接口，其实就这就是 `fmt.Println` 能够收任何类型变量的原因

- **多接口**

  概念很简单，就是一个结构体实现了多个接口

- **类型断言**

  ​	在上边多接口概念的基础上，使用不同的接口类型来引用这样的结构体实例时，只能调用接口类型对应的相关方法。举例，假如说有结构体 S 实现了 A 和 B 接口，现在我们拿到了 `val a A = S {...}` 的变量 a，那么我们如何通过 a 来调用 B 接口的方法？

  **引出**：内存分析上是合理可行的，是的，Go 也提供了类型断言：`value := i.(Type)`、`var value Type = i.(Type)`，拿上例来说，可以通过 变量a 获取其底层值，`val s S = a.(S)`，拿到了结构体实例本身，就可以调用其实现的任何方法了

  ​	如果一个结构体没有实现任何接口，调用该结构体实例变量的类型断言是编译不通过的，那么，如果知道一个接口的底层值是否实现了其他接口？

  **再引出**：还有一种类型断言的变体，其语法是 `value, ok := i.(Type)`，该语法的意思是，如果 Type 实现了 i 的接口类型，那么 ok 为 true，否则 ok 为 false，value 为 Type对应的结构体类型的零值

- **类型开关（switch 的特殊用法）**

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

- **嵌入式接口**

  在 Go 语言中，接口不能实现其他接口或扩展它们，但我们可以通过合并两个或多个接口来创建新接口

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

### Map

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

## 变量

- 关键字：var

- 示例：var xxx1, xxx2, ... <类型>

- 变量初始值：go 语言中称之为 `零值` 及变量没有初始化，其值就为零值。

  数字类型的变量零值为 0

  bool 类型的变量零值为 false

  string 类型的变量零值为空字符串（""）

  *int、[]int、map[string] int、chan int、func(string) int、error接口类型的变量的零值为 nil

- 自行判定变量类型（类型一）：var flag = true

- 自行判定变量类型（类型二）：var a int = 1 可以简写成 a := 1，`:=` 变量声明语句。语法限定为 `:=` 的语句的执行结果是必须产生一个新的变量

  例（方法体内）

  ```go
  var intVal int = 1
  // 编译不通过（无法产生名为 intVal 的变量，已经声明定义过了）
  intVal := 1
  // 编译通过（成功产生了名为 intVal2 的变量，同时还将 intVal 变量重新赋值了）
  intVal, intVal2 = 1, 2
  ```

  例（编译通过）

  ```go
  // 方法体外定义变量（全局变量）
  var val1, val2, val3 int
  
  // 方法体内
  val1, val2, val3 = 1, 2, 3
  ```

- 全局变量（在函数体之外定义的变量）

  **一般采用如下因式分解关键字的格式定义**

  ```go
  var (
      a int
      b bool
  )
  ```

  **这种不带声明格式的只能在函数体中出现**
   `g, h := 123, "hello"`

- 并行（同时）赋值

  1. 如果你想要交换两个变量的值，则可以简单地使用 a, b = b, a，两个变量的类型必须是相同

  2. 空白标识符 _ 也被用于抛弃值，如值 5 在：_, b = 5, 7 中被抛弃

  3. _ 实际上是一个只写变量，你不能得到它的值。这样做是因为 Go 语言中你必须使用所有被声明的变量，但有时你并不需要使用从一个函数得到的所有返回值

  4. 并行赋值也被用于当一个函数返回多个返回值时，比如这里的 val 和错误 err 是通过调用 Func1 函数同时得到：val, err = Func1(var1)

### 值传递

1. 所有像 int、float、bool 和 string 这些基本类型都属于值类型，使用这些类型的变量直接指向存在内存中的值，且变量赋值实际上就是在内存中将变量的值进行了拷贝

2. 可以通过 &i 来获取变量 i 的内存地址，例如：0xf840000040

3. 值类型的变量的值存储在栈中

### 引用传递

​	引用传递的基本概念：当使用赋值语句 r2 = r1 时，只有引用（地址）被复制，如果 r1 的值被改变了，那么这个值的所有引用都会指向被修改后的内容，在这个例子中，r2 也会受到影响

1. 复杂的数据通常会需要使用多个字，这些数据一般使用引用类型保存
2. 一个引用类型的变量 r1 存储的是 r1 的值所在的内存地址（数字），或内存地址中第一个字所在的位置，这个内存地址称之为指针，这个指针实际上也被存在另外的某一个值中
3. 同一个引用类型的指针指向的多个字可以是在连续的内存地址中（内存布局是连续的），这也是计算效率最高的一种存储形式；也可以将这些字分散存放在内存中，每个字都指示了下一个字所在的内存地址

- **方法中的值传递、引用传递**

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

### 变量值交换

- 这个单独拿出来出说，Go 语言中特别的支持一种方便的交换两个变量值的语法（JDK8 C++ C# 中都是没有，Python 中是有的）

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

## 常量

​	常量是一个简单值的标识符，在程序运行时，不会被修改的量

- 显式类型定义： `const b string = "abc"`

- 隐式类型定义： `const b = "abc"`

- 作为枚举使用

  ```go
  const (
      Unknown = 0
      Female = 1
      Male = 2
  )
  ```

- 常量可以用len(), cap(), unsafe.Sizeof() 函数计算表达式的值。常量表达式中，函数必须是内置函数，否则编译不过

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

​	最基础的运算符就不赘述了，这里重点提一下 Go 语言比较标志性的运算符。类比其他语言来说，C 语言有指针、值传递、引用传递，像 Java 中没有指针、引用传递的概念，Go 语言和 C 语言比较像，存在引用传递的概念

| 运算符 | 描述             | 示例                     |
| ------ | ---------------- | ------------------------ |
| &      | 返回变量存储地址 | &a; 将给出变量的实际地址 |
| *      | 指针变量         | *a; 是一个指针变量       |

## 条件语句

​	首先要提及的是，Go 语言没有三目运算符（? :）的概念

- **if**

  就传统 if 的语法 `if (条件) {...}` 来说，编译可以通过，但是 Go 语言中不推荐在整个条件表达式的左右加上括号

- switch

  和传统 swith 的语法相比，Go 语言中主要有一下几点不同、拓展

  1. 可以理解为，case 下默认在最后一条语句加上 break 的；如果想要继续执行，可以在语句最后加上 `fallthrough` 关键字
  2. case 后边可以跟多个值，用逗号隔开
  3. 可以根据变量的类型进行条件分支

- **select**

  select 是 Go 中的一个控制结构，类似于用于通信的 switch 语句。每个 case 必须是一个通信操作，要么是发送要么是接收

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
  2. 如果没有 default 子句，select 将阻塞，直到某个通信可以运行；Go 不会重新对 channel 或值进行求值
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

​	首先，Go 语言中只有 **for** 关键字

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

## 函数和方法

### 函数

​	Go 语言程序中，至少有一个 `main` 函数

- Go 语言标准库提供了多种可动用的内置的函数。例如，len() 函数可以接受不同类型参数并返回该类型的长度。如果我们传入的是字符串则返回字符串的长度，如果传入的是数组，则返回数组中包含的元素个数

- 语法

  ```markdown
  func 方法名(参数类型1, 参数类型2, ...) 返回值类型 {
     函数体
  }
  ```

- 多返回值

  ```go
  # 需要多个返回值类型之间用逗号“,”隔开，且所有参数要被小括号“()”括起来
  func numbers() (val1 int, val2 int, val3 string) {
    a , b , c := 1 , 2 , "str"
    return a,b,c
  }
  ```

- 无用的方法参数

  ```go
  func test(val1 int, val2 int) {
      // 一堆逻辑，但是没有用到变量 val1 和 val2
  }
  
  // 那么推荐改成如下的形式
  func test(_ int, _ int) {}
  
  // 或者
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

  Go 语言支持匿名函数，可作为闭包。匿名函数是一个"内联"语句或表达式。匿名函数的优越性在于可以直接使用函数内的变量，不必申明

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

​	Go 语言中同时有函数和方法。一个方法就是一个包含了接受者的函数，接受者可以是命名类型或者结构体类型的一个值或者是一个指针。所有给定类型的方法属于该类型的方法集

- 自己的话

  在 Go 语言中，面向过程编程 对应 函数，面向对象编程 对应 方法。Go 语言语法、代码结构给人的直接的感觉，就不会说像 Java 那样完全的面向对象的编程语言，可以说正是因为有了方法的概念，才可能说将某一个方法绑定给特定的数据结构体（在 Java 中就是一个单独的 POJO）使用，Go 语言没有面向对象，但是这里可以体现出面向对象的思想

  另外，Go 语言中没有像 C++、Java 中 this 关键字的概念，在 Go 里，这个 this 指针需要明确的申明出来，其实和其它 OO 语言并没有很大的区别

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


## 其他（待整理）

过滤 `go env` 的结果：`go env | findstr 要找的关键字（注意区分大小写）`

通过 `go env -w 配置项名=配置项值` 配置的环境变量是临时的，想要永久生效还是要配置到环境变量中

type 关键字的作用：定义结构体、定义接口、定义别名（同 C 语言的 typedef）、函数类型定义（没有限定一定要在方法外定义）

特殊的 _ 符号：主要作为不被使用的变量名，甚至可以用该符号作为变量名在一个局部内重复定义变量

项目中结构体的字段定义后边，还有一段 json 什么的？

在个人学习工作目录，创建了这样的用于学习的项目目录结构 `D:\Workspace-learn\src\go-learn`，运行该目录下 main.go 的 main 函数，报错：`package go-learn is not in GOROOT`，解决方法在 `go-learn` 目录下执行 `go mod init` 命令

## 读书笔记

1、书买对了，适合有其他语言基础的中级开发者

2、编译速度快（不像 Java 遍历依赖链中所有依赖的库）

3、go的多线程 （内存消耗小的goroutine，通道替代传统设计的复杂同步锁机制）

4、没有继承！

5、接口实现非独特（？）

