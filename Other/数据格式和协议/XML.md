# XML

## 特殊字符

1. **替代**

   | 特殊字符 | 替代    |
   | -------- | ------- |
   | &        | \&amp;  |
   | \>       | \&gt;   |
   | \<       | \&lt;   |
   | '        | \&apos; |
   | "        | \&quot; |

2. **CDATA**

- 说明

  CDATA 是不应该由 XML 解析器解析的文本数据

- 语法

  <![CDATA[XXX]]>

- 注意

  1. CDATA 部分不能包含字符串 "]]>"。也不允许嵌套的 CDATA 部分

  2. 标记 CDATA 部分结尾的 "]]>" 不能包含空格或换行

