# Windows

## 快捷键

| 层面     | 快捷键           | 作用               |
| -------- | ---------------- | ------------------ |
| 系统     | Win + s、Win + q | 弹出搜索框         |
| 微软拼音 | Ctrl + .         | 切换中英文标点     |
| 系统     | Win + L          | 待机               |
| 系统     | Win + V          | 打开剪切板         |
| 系统     | Win + E          | 打开文件资源管理器 |
|          |                  |                    |
|          |                  |                    |
|          |                  |                    |
|          |                  |                    |

## 指令

window远程桌面连接指令：mstsc（Microsoft Terminal Services Client）

## 操作

### 窗口拖到显示器外

​	小白肯定关掉程序，重新打开了。应该这样操作：Alt + Tab 切到程序对应的任务栏，然后 Alt + Space 再按 M，这时候就可以用鼠标或者小键盘的方向箭移动窗口了

### 修改账户名

​	控制台输入：control userpasswords2

​	点击要改名的账户 → 属性 → 改名字 → 重启

### 不同类型文件打开方式

**方式一（使用Windows提供的设置进行设置）**

点击屏幕右下角 → 所有设置 → 应用 → 默认应用 → 按文件类型指定默认应用

**方式二（修改注册表）**
以Xmind为例，HKEY_CLASSES_ROOT 下你会看到：
`.xmind`
`.xmind_auto_file`

修改 `.xmind_auto_file\shell\open\command` 的值为：`"D:\软件\XMind_ZEN_10.1.2\_绿色版\_64位\XMind.exe" "%1"`

### 修改微软拼音时间格式

解释：键盘键入“sj”，候选词的第一个是这样格式的时间 `yyyy-MM-dd HH:mm:ss`

右键右下角任务栏的 `中` 或 `英` 图标 → 设置 → 词库和自学习 → 用户定义的短语/添加或编辑自定义短语 → 添加

```
sj
1
%yyyy%-%MM%-%dd% %HH%:%mm%:%ss%
```

### 管理右键菜单栏

按住win+R打开运行框，在运行框内输入：`Regedit` 打开注册表编辑器

一、子项

找到shell目录：HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\CommandStore\shell

1、在shell目录，右键新建：项(K)名字自定义，最好是英文。例如：Steam(一般与实际快捷方式相对应)
双击右边名称为(默认)的字符串值将数值数据(V)，这就是该快捷方式的名称

2、在Steam目录，右键新建：字符串值(S)，名字为icon 双击新建的这个icon字符串值，在数值数据(V)中填写你需要添加的快捷方式的路径。实际上这个字符串值的数值数据(V)为该快捷方式的图标路径，直接用快捷方式的路径也可以。如不填，则此处为空图标，对菜单没有影响。

3、在Steam目录，右键新建：项(K)，命名为command。双击右边名称为(默认)的字符串值，将数值数据(V)改为你需要添加的快捷方式文件路径，右键点击时，运行的就是这个路径对应的程序的快捷方式。

这样你就完成了一个子项，可通过上述方式可设置多个子项，接下来就是告诉你如何将这些快捷菜单添加到你的桌面右键菜单中去。


二、菜单项

找到shell目录：HKEY_CLASSES_ROOT\Directory\Background\shell

1、在shell目录右键新建：项(K)名字自定义，最好是英文。例如：Menu

2、在Menu目录，新建：字符串值(S)，名字为Position。双击新建的这个Position字符串值，在数值数据(V)中填写top，点确定。这个top代表该菜单在你桌面右键菜单中的位置，一般写top，就会在最上面。其他请自行修改。

3、在Menu目录，新建：字符串值(S)，名字为icon。双击新建的这个icon字符串值，同理，在数值数据(V)中填写图标路径。如不填，则此处为空图标，对菜单没有影响。

4、在Menu目录，新建：字符串值(S)，名字为MUIVerb。双击新建的这个MUIVerb字符串值，在数值数据(V)中填写右键菜单的名字，点确定。

5、在Menu目录，新建：字符串值(S)，名字为SubCommands。双击新建的这个SubCommands字符串值，在数值数据(V)中填写你前面做好的快捷菜单，例如Steam;Gang Beasts，中间必须用;隔开，最后不要用;结尾。

因为微软限制，此菜单中的子项最多为16项。

## 其他

- 分隔符（public static final String separator）

  windows：\

  linux：/

  除了在bat脚本中，windows也能识别“/”

- 搜狗输入法配置时间模板

  #$year-$month_mm-$day_dd $fullhour_hh:$minute:$second
