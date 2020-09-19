# Springboot中的YAML解析

## 简介

**YAML**（/ˈjæməl/，尾音类似*camel*骆驼）是一个可读性高，用来表达数据[序列化](https://baike.baidu.com/item/序列化)的格式。YAML参考了其他多种语言，包括：[C语言](https://baike.baidu.com/item/C语言)、[Python](https://baike.baidu.com/item/Python)、[Perl](https://baike.baidu.com/item/Perl)，并从[XML](https://baike.baidu.com/item/XML)、电子邮件的数据格式（RFC 2822）中获得灵感。Clark Evans在2001年首次发表了这种语言，另外Ingy döt Net与Oren Ben-Kiki也是这语言的共同设计者。当前已经有数种编程语言或脚本语言支持（或者说解析）这种语言。

*YAML*是"YAML Ain't a Markup Language"（YAML不是一种[标记语言](https://baike.baidu.com/item/标记语言)）的[递归缩写](https://baike.baidu.com/item/递归缩写)。在开发的这种语言时，*YAML* 的意思其实是："Yet Another Markup Language"（仍是一种[标记语言](https://baike.baidu.com/item/标记语言)），但为了强调这种语言以数据做为中心，而不是以标记语言为重点，而用反向缩略语重命名。

## Springboot中

- 文件后缀名为`YAML`或者`YML`都视为该种类型的文件
- 使用`org.yaml.snakeyaml`解析`YAML`

- 坑

  不要使用true|false、on|off、yes|no（首字母大小写、全部大写）作为键值

  - 否则报错

    `Boolean`不能转成`String`

  - 原因

    - 通过跟踪`snakeyaml`的源码发现，本该是{"on": true}的键值对，变成了{true: true}

    - 发生这个错误的核心在于构建的map是错的，而构建map是由“org.yaml.snakeyaml.Yaml”构建的，而map中的数据，是根据节点构建来的，而构建节点的很重要的规则：
      	1、构建节点的tag，核心见下（org.yaml.snakeyaml.Resolver.resolve）
      	2、根据节点构建map中的数据，核心
      （org.yaml.snakeyaml.constructor.SafeConstructor.ConstructYamlMap.construct）
      最终回到“org.yaml.snakeyaml.constructor.BaseConstructor.constructObjectNoCheck()”,202根据布尔tag的节点拿到了“ConstructYamlBool”值解析构造器，这个解析器将"on" -> true 了

    - 为了利于调试，在主配置文件的顶部添加了“on: true”，进而一步步查看“snake”包解析yml的源码发现：map是从解析yml的node得到，而node分keynode和valuenode...，关键是node最终封装到map的类型是由Node中的“Tag”决定

    - 最终定位到：“org.yaml.snakeyaml.resolver”的“103行”的“public Tag resolve(NodeId kind, String value, boolean implicit)”方法处

      ```java
      其中有一个“protected Map<Character, List<ResolverTuple>> yamlImplicitResolvers”，这个解析器会根据值的首字母得到对应的tag，其中如下的的首字母会得到布尔类型的tag
      首字母大小写（2种）、全字母大写（1种）
      yes、no
      true、false
      on、off
      
      即：当value值为“on”时，最终返回的Tag是bool类型的
      ```

    