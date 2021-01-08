# SQL

## 大佬良言

**敖丙**

对于 MySQL 每个版本发布的新特性，尤其是跟优化器和SQL相关的，应该去关注和了解，可能会发现适合自己业务场景的特性

**小哈学 Java**

数据库编译器产生执行计划，决定着SQL的实际执行方式。但是编译器只是尽力服务，所有数据库的编译器都不是尽善尽美的。

上述提到的多数场景，在其它数据库中也存在性能问题。了解数据库编译器的特性，才能避规其短处，写出高性能的SQL语句。

程序员在设计数据模型以及编写SQL语句时，要把算法的思想或意识带进来。

编写复杂SQL语句要养成使用 WITH 语句的习惯。简洁且思路清晰的SQL语句也能减小数据库的负担 。

## 开发规范

- 数据订正（特别是删除或修改记录操作）时，要先 select，避免出现误删除，确认无误才能执行更新语句

- 对于数据库中表记录的查询和变更，只要涉及多个表，都需要在列名前加表的别名（或表名）进行限定

  说明：对多表进行查询记录、更新记录、删除记录时，如果对操作列没有限定表的别名（或表名），并且操作列在多个表中存在时，就会抛异常。 

  正例：`select t1.name from table_first as t1 , table_second as t2 where t1.id=t2.id;`

  反例：在某业务中，由于多表关联查询语句没有加表的别名（或表名）的限制，正常运行两年后，最近在某个表中增加一个同名字段，在预发布环境做数据库变更后，线上查询语句出现出1052异常：`Column 'name' in field list is ambiguous`

- SQL语句中表的别名前加as，并且以 t1、t2、t3、...的顺序依次命名。 说明：

  1）别名可以是表的简称，或者是依照表在SQL语句中出现的顺序，以t1、t2、t3的方式命名

  2）别名前加as使别名更容易识别。 

  正例：`select t1.name from table_first as t1, table_second as t2 where t1.id=t2.id;`

- in操作能避免则避免，若实在避免不了，需要仔细评估in后边的集合元素数量，控制在1000个之内

- 在表查询中，一律不要使用 * 作为查询的字段列表，需要哪些字段必须明确写明。 说明：

  1）增加查询分析器解析成本

  2）增减字段容易与 `resultMap` 配置不一致

  3）无用字段增加网络消耗，尤其是text类型的字段

## 执行计划

通常

using where：查询使用索引的情况下，需要回表去查询所需的数据。

using index condition：查询使用了索引，但是需要回表查询数据。

using index：查询使用覆盖索引的时候会出现。

using index & using where：查询使用了索引，但是需要的数据都在索引列中能找到，不需要回表查询数据

---

以下内容来自 Java 阿里开发手册

至少要达到 `range` 级别，要求是 `ref` 级别，如果可以是 `consts` 最好。说明：

​	1） `consts` 单表中最多只有一个匹配行（主键或者唯一索引），在优化阶段即可读取到数据

​	2） `ref` 指的是使用普通的索引（normal index）

​	3） `range` 对索引进行范围检索

​	4） `index` 索引物理文件全扫描，速度非常慢，与全表扫描是小巫见大巫

## 优化方法

### 方法一

- 1.**查看优化器状态**

  ```mysql
  SHOW VARIABLES LIKE 'optimizer_trace';
  ```

- 2.**会话级别临时开启**

  ```mysql
  SET session optimizer_trace="endable=on", end_marks_in_json=on;
  ```

- 3.**设置优化器追踪的内存大小**

  ```mysql
  SET OPTIMIZER_TRACE_MAX_MEM_SIZE=1000000;
  ```

- 4.**执行自己的sql**

  ```mysql
  SELECT * FROM XXX WHERE XXX = XXX
  ```

- 5.**Information_schema.optimizer_trace表**

  ```mysql
  SELECT trace FROM information_schema.OPTIMIZER_TRACE;
  ```

- 6.**导入到一个命名为xx.trace的文件，然后用JSON阅读器来查看**（如果没有控制台权限，或直接交由运维，让他把该trace文件，输出给你就行了）

  ```mysql
  SELECT TARCE INTO DUMPFILE "E:\\test.trace" FROM INFORMATION_SCHEMA.OPTIMIZER_TRACE;
  ```

- 7.**查看文件**

  注意 `rows_estimation` 和 `considered_execution_plans`

- **注意**

  如果不设置优化器最大容量的话，可能导致优化器返回的结果不全

### 方法二（待验证）

1. 执行你的查询语句

2. 查看 SQL 执行期间各阶段的资源使用

   `show profile cpu,block io for query 2;`

还有个待验证：分析 Status

## 索引失效

### 查询条件（待完善）

**核心**：不符合最左原则

- 表达式计算：... WHERE `id` + 1 = 2

- 函数计算：... WHERE xxx(id) = 1

- 隐式转换：数据表字段的编码是 utf8mb4，查询条件的字段值编码是 utf8
- or
- in、not in
- is null、is not null

### 组合索引

​	注意查询条件字段的顺序、排序条件的字段的规则得相同（要不都升序、要不都降序）

### 排序

- 如果GROUP BY的字段上如果有索引，因为索引默认有序，可以避免排序行为。所以如果你的分组对排序没有要求，且字段没有对应索引，一定要显示指定不排序，例：`GROUP BY xxx ORDER BY NULL`

- 注意利用索引的有序性。order by 最后的字段是组合索引的一部分，并且放在索引组合顺序的最后，避免出现file_sort的情况，影响查询性能。

  正例：`where a=? and b=? order by c;` 索引：a_b_c 反例：索引如果存在范围查询，那么索引有序性无法利用，如：`WHERE a>10 ORDER BY b;` 索引a_b无法排序

### 索引提示

​	当MySQL确实因为某些原因，采用了错误的执行计划（没有使用预期的索引），可以采用使用 FORCE INDEX 手动指定的方式，让MySQL去使用指定的索引进行查询。但是，这仅作为一种应急预案，不推荐采用该方式，因为数据库迁移可能就不支持了（拓展性不好），还要重新做一个代码发布。首先MySQL关于优化器的设计是复杂且优秀的，我们应该尽量做到以提示的方式，而不是强制的

​	1） 从SQL语句的角度：相同目的，不同的SQL

​	2） 索引统计信息有问题：`analyze table` 一下

​		简单说一下原理就是，MySQL的Optimizer（优化器），采用随机取样来估计（即不是一个一定准确的值）指定列数据的散列层面（区分度），并估计包括回表等IO次数等成本，会决定是否采用相关索引或是全表扫描。有时优化器可能会因为数据

​	3） 删除误导MySQL的索引，这个确实是一条在实际中，确实挺常用的技巧

```mysql
IGNORE INDEX(index1, index2, ...)
FORCE INDEX(index1, index2, ...)
```

## 业务场景

### JOIN

1. 代替子查询
2. 阿里规范规定

- 超过三个表禁止join

- 需要join的字段，数据类型保持绝对一致

  多表关联查询时，保证被关联的字段需要有索引

- 即使双表join也要注意表索引、SQL性能

### UNION（待补充）

​	`UNION` 是将 `UNION ALL` 后的结果镜像一次 `DISTINCT`，去除重复的记录后的结果，如果对数据的重复方面没有要求，则应该使用后者

### 1、模糊查询（[内容来自](https://mp.weixin.qq.com/s?__biz=MzAwNDA2OTM1Ng==&mid=2453147984&idx=1&sn=ad5448a9f6f4f7ac41416e8fe9319b05&chksm=8cfd33d3bb8abac5c7f2b06ebf54b42db116961acee7ba80f0baea22b281f2282b4303ef579b&scene=126&sessionid=1605782437&key=8f83c7812301d98ff5871560cb5046ca659b02577ded4dfd7f88b34563dc36041e9df4b645a613201e7bc367dfc2cf1b6090469834d39248f03b2db6bd4f150097803280db90f9000dd462d1c5d3c9fcd010c1bb9158b0a25167bae2224870e37576edad4481d65eebccaedad2d18d4aa9fd6eb78a582a93e07fe76277db8932&ascene=1&uin=MjQzMjg2NTAzMA%3D%3D&devicetype=Windows+10+x64&version=6300002f&lang=zh_CN&exportkey=AfBeamWI2iH7R7Rimq%2Bue4I%3D&pass_ticket=EjbRlHPp9Rnb9amAgB7eMyH7FiidS2Eui96AcunhZ%2B6xYFxvMk5ma6Banxk4PrNW&wx_header=0)）

#### ICP

Index 全称 Condition Pushdown，这个特性是 MySQL 5.6 开始支持，简单来说就是先根据索引来查找数据，再通过 where 条件来过滤数据的过程，扫描了大量不必要的数据，增加了数据库IO操作。ICP 就是 MySQL 在取出索引数据时，判断是否可以进行 where 条件过滤，将 where 的部分过滤操作放在存储引擎层，减少了不必要数据带来的 IO 开销，在某些查询下，可以减少 Server 层对存储引擎层数据的读取，从而提高数据库的整体性能

**特性**

1. 支持 range、ref、eq_ref、ref_or_null 的方式访问数据
2. 支持辅助索引
3. 支持 InnoDB 和 MyISAM 的普通表和分区表
4. 支持 Explain 解释计划中 Extra = "Using index conditon"

---

1. 不支持主键聚簇索引
2. 不支持基于虚拟列的辅助索引
3. 不支持条件包含子查询
4. 不支持条件包含存储过程，用户函数的下推

**控制参数**

`show variables like 'optimizer_switch';` → `index_condition_pushdown`

默认是开启的，可以进行设置开启或关闭

`set optimizer_switch ='index_condition_pushdown=on | off';`

**详解**

![图片](MySQL Business.assets/640)

（关闭索引下推）

![图片](MySQL Business.assets/640)

（开启索引下推）

**优化示例**

Handler_read_next=16384说明扫描了16384行的数据，SQL实际返回只有12行数，耗时50ms。对于这种扫描大量数据行，只返回少量数据的SQL，可以从两个方面去分析。

1. **索引选择率低**：对于符合索引`(name,nickname)，name`作为前导列出现 where 条件，CBO（Cost Base Optimizer）都会选择走索引，因为扫描索引比全表扫描的COST要小，但由于 name 列的基数不高，导致扫描了索引中大量的数据，导致SQL性能也不太高。

   **Column_name: name  Cardinality: 6** 可以看到`users01`表中`name`的不同的值只有6个，选择率6/114688很低。

2. **数据分布不均匀**：对于`where name = ?`来说，name数据分布不均匀时，SQL第一次传入的值返回结果集很小，CBO就会选择走索引，同时将SQL的执行计划缓存起来，以后不管name传入任何值都会走索引扫描，这其实是不对的，如果传入name会导致返回表中80%的数据，这时走全表扫描更快。

**结果**

| 关闭ICP | Using where           | 16384 | 12   | 50ms |
| ------- | --------------------- | ----- | ---- | ---- |
| 关闭ICP | Using where           | 16384 | 12   | 50ms |
| 开启ICP | Using index condition | 12    | 12   | 10ms |

#### SQL 优化

以下就简单来归纳了

```mysql
# 修改前（nickname 字段上有索引）
select Id from users01 where nickname like '%SK%'
# type=ALL，Extra=Using where 索引全扫描，没有用到 ICP 特性

# 修改后
select * from users01 a, (select id from users01 where nickname like '%SK%') b where a.id = b.id;
# type = index 说明没有用到ICP特性，但是可以利用 Using where; Using index 这种索引扫描不回表的方式减少资源开销来提升性能
```

#### 全文索引

**概念**：MySQL 5.6开始支持全文索引，可以在变长的字符串类型上创建全文索引，来加速模糊匹配业务场景的DML操作。它是一个inverted index（**反向索引**），创建 `fulltext index` 时会自动创建6个 `auxiliary index tables`（辅助索引表），同时支持索引并行创建，并行度可以通过参数 `innodb_ft_sort_pll_degree` 设置，对于大表可以适当增加该参数值。

**删除全文索引**的表的数据时，会导致辅助索引表大量delete操作， InnoDB 内部采用标记删除，将已删除的 DOC_ID 都记录特殊的FTS_*_DELETED 表中，但索引的大小不会减少，需要通过设置参数`innodb_optimize_fulltext_only=ON` 后，然后运行 OPTIMIZE TABLE来 重建全文索引

**示例**：![图片](MySQL Business.assets/640)

**命令**

```mysql
# 设置当前的全文索引表
SET GLOBAL innodb_ft_aux_table = '数据库名/表名';

# 查看数据文件
SELECT * FROM information_schema.innodb_ft_index_cache;

# 全文索引对象分析
SELECT table_id, name, space FROM INFORMATION_SCHEMA.INNODB_TABLES WHERE name LIKE 'test/%';
```

**自己理解**：反向索引顾名思义，我们平时通过索引找具体的数据，而反向索引就是通过特定的数据字段找索引（主键id）；建立全文索引，书写 SQL 时，通过 `MATCH AGAINST` 函数来使用全文索引进行查询：`SELECT * FROM user01 WHERE MATCH(nickname) against('看风')`

#### 生成列

**声明**

仅能够解决左模糊

**概念**

MySQL 5.7 开始支持生成列，生成列是由表达式的值计算而来，有两种模式：`VIRTUAL` 和 `STORED`，如果不指定默认是 `VIRTUAL`

```mysql
col_name data_type [GENERATED ALWAYS] AS (expr)  [VIRTUAL | STORED] [NOT NULL | NULL]
```

**VIRTUAL** 生成列用于复杂的条件定义，能够简化和统一查询，不占用空间，访问列是会做计算。

**STORED** 生成列用作物化缓存，对于复杂的条件，可以降低计算成本，占用磁盘空间。

​	支持辅助索引的创建，分区以及生成列可以模拟函数索引。不支持存储过程，用户自定义函数的表达式，`NONDETERMINISTIC`的内置函数，如NOW()， RAND()以及不支持子查询

**示例**

```mysql
#添加基于函数 reverse 的生成列 reverse_nickname
ALTER TABLE users01 ADD reverse_nickname varchar(200) generated always AS (reverse(nickname));

#查看生成列信息
SHOW COLUMNS FROM users01;
| reverse_nickname | varchar(200) | YES  |     | NULL              | VIRTUAL GENERATED | #虚拟生成列

# 实际查询
# 利用内置reverse函数将like '%AB'反转为like 'BA%'，基于此函数添加虚拟生成列 → 将SQL改写成通过生成列like reverse('%AB')去过滤，走生成列上的索引

# 添加虚拟生成列并创建索引
ALTER TABLE users01 ADD reverse_nickname VARCHAR(200) generated always AS (reverse(nickname));

# 在虚拟生成列上创建索引
ALTER TABLE users01 ADD INDEX idx_reverse_nickname(reverse_nickname);

# SQL执行计划
|  1 | SIMPLE      | users01 | NULL       | range | idx_reverse_nickname | idx_reverse_nickname | 803     | NULL |    1 |   100.00 | Using where |
```

#### 总结

对于模糊查询优化的核心就是，不能使用 MySQL 默认的索引方式来提高效率，就考虑采用另外的索引方式（全文索引、生成列）来提高效率，实在用不上所以我们就想办法来减少数据扫描（ICP）进而来减小性能开销 - 本质上是减小磁盘的 IO 扫描。

### 2、大批量分页

> 代码中写分页查询逻辑时，若count为0应直接返回，避免执行后面的分页语句

利用延迟关联或者子查询优化超多分页场景。 说明：MySQL并不是跳过 offset 行，而是取 offset+N 行，然后返回放弃前 offset 行，返回 N 行，那当 offset 特别大的时候，效率就非常的低下，要么控制返回的总页数，要么对超过特定阈值的页数进行 SQL 改写。实际应当参照如下方式进行优化，编写 SQL语句

```mysql
-- 例1：10000 后查 100 条
-- 说明：先查询获取到满足分页条件的 id，再根据 id 查询数据
SELECT 
	t1.* 
FROM 
	表名
AS t1, (select id from 表名 where 条件 LIMIT 100000, 100) AS t2 
WHERE 
    查询条件 AND 
	t1.id = t2.id

-- 例2：100000 后查 100 条
-- 条件：id 是递增的
SELECT 
	t1.* 
FROM 表名
WHERE 
	查询条件 AND 
	id >= (SELECT id FROM 表名 WHERE 查询条件 LIMIT 100000, 1)
LIMIT 100

-- 总结，核心思想就是通过添加某些额外的条件，来替换直接使用 limit 进行大数据量的分页操作，例如 id、create_time 之类的，可以和前端同学配合好，在参数添加这么一个具有实际意义的值，对性能有很大影响，应该视为业务逻辑参数了
```

### 3、合并表数据

直接给出示例

```mysql
-- DDL
CREATE TABLE `product` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT KEY,
	`code` VARCHAR(20) NOT NULL UNIQUE COMMENT '商品编号',
	`name` VARCHAR(20) NOT NULL UNIQUE COMMENT '商品名称',
	`sum` INT NOT NULL COMMENT '售卖数'
);

CREATE TABLE `product_history` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT KEY,
	`code` VARCHAR(20) NOT NULL UNIQUE COMMENT '商品编号',
	`name` VARCHAR(20) NOT NULL UNIQUE COMMENT '商品名称',
	`sold_sum` INT NOT NULL COMMENT '售卖数量'
);

-- DML
INSERT INTO `product`(`id`, `code`, `name`, `sum`) VALUES (1, '001', 'A', 1);
INSERT INTO `product`(`id`, `code`, `name`, `sum`) VALUES (2, '002', 'B', 2);
INSERT INTO `product`(`id`, `code`, `name`, `sum`) VALUES (3, '003', 'C', 0);

INSERT INTO `product_history`(`id`, `code`, `name`, `sum`) VALUES (1, '003', 'C', 1);
INSERT INTO `product_history`(`id`, `code`, `name`, `sum`) VALUES (2, '004', 'D', 4);

-- 这样结构的 DML 在将两张表的数据合并的业务场景中使用较为频繁
-- 重点要说明的是，VALUES(sum) 的作用是取得要插入指定字段的值，下面实例中就是指 sold_sum 的值
INSERT `product` (`code`, `name`, `sum`)
SELECT `code`, `name`, `sold_sum` FROM `product_history`
ON DUPLICATE KEY UPDATE sum = sum + VALUES(sum);
```

### 4、混合排序

**特殊案例**

假如一个 布尔类型、含有枚举含义 或者就是枚举类型（即有限集）的字段作为第一个排序条件，然后另外一个就是任意一个有索引的字段，这个场景下可以采用对有限条件集进行用上索引的单字段排序，然后 `UNION ALL` 一下，来实现高效的查询

## 数据库设计

### 字段

- 必备字段 id、create_time、update_time
- 虽然违背 3NF，但是实际中，字段允许适当冗余，以提高查询性能，但必须考虑数据一致。频繁修改、唯一索引、大字段不允许冗余
- 命名不要用模糊不具体的单词，如 type

#### 编码

- 因国际化需要，所有的字符存储与表示，均采用 utf8 字符集，那么字符计数方法需要注意

  说明：

  `SELECT LENGTH("轻松工作")`  返回为 12
  
  `SELECT CHARACTER_LENGTH("轻松工作")` 返回为4

- 如果需要存储表情，那么选择 utf8mb4 来进行存储，注意它与utf8编码的区别

#### 类型

- 任何非负数的字段，必须是 `unsigned`
- 尽可能将所有列定义为 `not null`

##### COUNT

不要使用 `count(列名)` 或 `count(常量)` 来替代 `count(*)`，`count(*)` 是SQL92 定义的标准统计行数的语法，跟数据库无关，跟 `NULL` 和 `非NULL` 无关。 说明：`count(*)` 会统计值为 NULL 的行，而 count(列名) 不会统计此列为 NULL 值的行

`count(distinct col)` 计算该列除 NULL 之外的不重复行数，注意 `count(distinct col1, col2)` 如果其中一列全为 NULL，那么即使另一列有不同的值，也返回为0

##### SUM

当某一列的值全是 NULL 时，count(col) 的返回结果为0，但 sum(col) 的返回结果为 NULL，因此使用 sum() 时需注意 NPE 问题。 正例：可以使用如下方式来避免 sum 的 NPE 问题：`SELECT IFNULL(SUM(column), 0) FROM table`

**道理上**

1. 难以优化引用可空列查询，它会使索引、索引统计和值更加复杂

2. 可空列需要更多的存储空间，还需要 MySQL 内部进行特殊处理

3. 可空列被索引后，每条记录都需要一个额外的字节
4. NULL 值到 非NULL 的更新无法做到原地更新，更容易发生索引分裂，从而影响性能

**业务上**

1. 所有使用 NULL 值的情况，都可以通过一个有意义的值的表示，这样有利于代码的可读性和可维护性，并能从约束上增强业务数据的规范性
2. 在时间类型的字段上，对 NULL 的处理容易出现问题
3. NULL 值和任何字段进行比较、操作得到的都是 NULL，容易出错

- 如果存储的字符串长度几乎相等，使用char定长字符串类型
- varchar 类型的字段如果存储长度超过 5000，定义字段类型为 text 需要独立出来一张表，避免影响其他字段索引效率

#### 命名

- 库名与应用名称尽量一致

- 表名不使用大写字母，表名不使用复数名词

- 表的命名最好是遵循“业务名称_表的作用”。 正例：alipay_task / force_project / trade_config

- 不使用保留字

- 表达是与否概念的字段，必须使用 `is_xxx` 的方式命名，数据类型是 `unsigned tinyint`（1表示是，0表示否），例如：`is_deleted`

  注意：POJO类中的任何布尔类型的变量，都不要加 `is` 前缀，所以，需要在 `<resultMap>` 设置从 `is_xxx` 到 `Xxx` 的映射关系。数据库表示是与否的值，使用 `tinyint` 类型，坚持 `is_xxx` 的命名方式是为了明确其取值含义与取值范围

### 外键

​	不得使用外键与级联，一切外键概念必须在应用层解决

### 索引

**命名**

​	pk\_字段名、uk\_字段名、idx\_字段名

**varchar**

​	在varchar字段上建立索引时，必须指定索引长度，没必要对全字段建立索引，根据实际文本区分度决定索引长度
​	说明：索引的长度与区分度是一对矛盾体，一般对字符串类型数据，长度为20的索引，区分度会高达90%以上，可以使用 `count(distinct left(列名, 索引长度))/count(*)` 的区分度来确定

#### 主键索引

​	在 innodb 存储引擎中又叫聚簇索引

#### 唯一索引

​	业务上具有唯一特性的字段，即使是组合字段，也必须建成唯一索引。 说明：不要以为唯一索引影响了 insert 速度，这个速度损耗可以忽略，但提高查找速度是明显的；另外，即使在应用层做了非常完善的校验控制，只要没有唯一索引，根据墨菲定律，必然有脏数据产生

#### 组合索引

​	建组合索引的时候，区分度最高的在最左边。 正例：如果where a=? and b=?，a列的几乎接近于唯一值，那么只需要单建idx_a索引即可。 说明：存在非等号和等号混合判断条件时，在建索引时，请把等号条件的列前置。如：where c>? and d=? 那么即使c的区分度更高，也必须把d放在索引的最前列，即建立组合索引 idx_d_c

#### 覆盖索引

​	利用覆盖索引来进行查询操作，避免回表。索引的分类就上面三种，覆盖索引只是一种查询的一种效果，用explain的结果，extra列会出现：using index

## 大点

### 主键ID生成策略

#### UUID

**优点**

- 生成足够简单，本地生成无网络消耗，具有唯一性

**缺点**

- 无序的字符串，不具备趋势自增特性
- 没有具体的业务含义
- 长度过长16 字节128位，36位长度的字符串，存储以及查询对MySQL的性能消耗较大，MySQL官方明确建议主键要尽量越短越好，作为数据库主键 `UUID` 的无序性会导致数据位置频繁变动，严重影响性能。

#### 数据库自增ID

**优点**

- 实现简单，ID单调自增，数值类型查询速度快

**缺点**

- DB单点存在宕机风险，无法扛住高并发场景

#### 数据库自增ID（集群）

```MySQL
-- 数据库1
set @@AUTO_INCREMENT_offset = 1;     -- 起始值
set @@AUTO_INCREMENT_increment = 2;  -- 步长
-- 数据库2
set @@AUTO_INCREMENT_offset = 2;     -- 起始值
set @@AUTO_INCREMENT_increment = 2;  -- 步长
```

**优点**

- 解决DB单点问题

**缺点**

- 不利于后续扩容，而且实际上单个数据库自身压力还是大，依旧无法满足高并发场景

#### 数据库的号段模式

- 号段模式是当下分布式ID生成器的主流实现方式之一，号段模式可以理解为从数据库批量的获取自增ID，每次从数据库取出一个号段范围，例如 (1,1000] 代表1000个ID，具体的业务服务将本号段，生成1~1000的自增ID并加载到内存

- 号段表核心字段

  biz_type ：代表不同业务类型

  max_id ：当前最大的可用id

  step ：代表号段的长度

  version ：是一个乐观锁，每次都更新version，保证并发时数据的正确性

- 当一批id用完了

  ```MySQL
  update id_generator set max_id = #{max_id+step}, version = version + 1 where version = # {version} and biz_type = XXX
  ```

- 由于多业务端可能同时操作，所以采用版本号`version`乐观锁方式更新，这种`分布式ID`生成方式不强依赖于数据库，不会频繁的访问数据库，对数据库的压力小很多

#### Redis

- 利用其`incr`命令实现ID的原子性自增

- 注意持久化

  ​	`RDB`会定时打一个快照进行持久化，假如连续自增但`redis`没及时持久化，而这会Redis挂掉了，重启Redis后会出现ID重复的情况。

  ​	`AOF`会对每条写命令进行持久化，即使`Redis`挂掉了也不会出现ID重复的情况，但由于incr命令的特殊性，会导致`Redis`重启恢复的数据时间过长。

#### Snowflake

- twitter公司内部分布式项目采用的ID生成算法，开源后广受国内大厂的好评，在该算法影响下各大公司相继开发出各具特色的分布式生成器

- `Snowflake`生成的是Long类型的ID，一个Long类型占8个字节，每个字节占8比特，也就是说一个Long类型占64个比特

- Snowflake ID组成结构：`正数位`（占1比特）+ `时间戳`（占41比特）+ `机器ID`（占5比特）+ `数据中心`（占5比特）+ `自增值`（占12比特），总共64比特组成的一个Long类型
  - 第一个bit位（1bit）：Java中long的最高位是符号位代表正负，正数是0，负数是1，一般生成ID都为正数，所以默认为0
  - 时间戳部分（41bit）：毫秒级的时间，不建议存当前时间戳，而是用（当前时间戳 - 固定开始时间戳）的差值，可以使产生的ID从更小的值开始；41位的时间戳可以使用69年，(1L << 41) / (1000L * 60 * 60 * 24 * 365) = 69年
  - 工作机器id（10bit）：也被叫做`workId`，这个可以灵活配置，机房或者机器号组合都可以
  - 序列号部分（12bit）：自增值支持同一毫秒内同一个节点可以生成4096个ID
- 具体实现：https://github.com/beyondfengyu/SnowFlake

#### Uid-generator (百度)

- 具体实现：https://github.com/baidu/uid-generator/blob/master/README.zh_cn.md

#### Leaf (美团)

- 同时支持号段模式和雪花模式

- 具体实现：https://github.com/Meituan-Dianping/Leaf

#### Tinyid (滴滴)

- 具体实现：https://github.com/didi/tinyid

[微信公众号]: 文章已被投诉下架了...

### 预编译

**结论**

从结论开始说，预编译在实际项目中，几乎没有作用。原因是作用不大，SQL 注入由服务端程序防治；预编译一个模板，多次调用来提高性能，在实际中也因为查询大多只执行一次，且要执行多次时，也更推荐进行批量操作，来减小网络IO的开销，还因为一个预编译模板只在一个 Connection 中有效等因素

**官方说明**

- If a prepared statement with the given name already exists, it is deallocated implicitly before the new statement is prepared. This means that if the new statement contains an error and cannot be prepared, an error is returned and no statement with the given name exists. 
- A prepared statement created in one session is not available to other sessions.
- When a session ends, whether normally or abnormally, its prepared statements no longer exist. If auto-reconnect is enabled, the client is not notified that the connection was lost. For this reason, clients may wish to disable auto-reconnect.
- A prepared statement created within a stored program continues  to exist after the program finishes executing and can be  executed outside the program later.
- A statement prepared in stored program context cannot refer to  stored procedure or function parameters or local variables  because they go out of scope when the program ends and would  be unavailable were the statement to be executed later outside  the program. As a workaround, refer instead to user-defined  variables, which also have session scope

- and more...See detail at [page](https://dev.mysql.com/doc/refman/8.0/en/prepare.html)

**手动执行预编译**

```mysql
-- 定义：对使用参数占位符的 sql 语句进行预编译
PREPARE get_users FROM 'SELECT * FROM user WHERE id = ? AND is_deleted = ?';
-- 变量，用于为预编译语句设置参数
SET @id='1', @is_deleted='0';
-- 执行：调用预编译的结果
EXECUTE get_users USING @id, @is_deleted;

-- 释放、删除预编译语句
{DEALLOCATE | DROP} PREPARE stmt_name
```

**定义预编译语句数量上限的变量**

```mysql
max_prepared_stmt_count
```

**查看数据库中记录的预编译相关的全局变量的记录信息**

```mysql
SHOW GLOBAL STATUS LIKE '%prepare%'
```

**web 程序开发使用预编译**（很早的博客文章里的内容了，参数的含义没错，但是因为 MySQL 版本或者不同语言的驱动程序版本的问题，参数的默认值是不同的）

- MySQL 的连接 url 参数1

  ```mysql
  useServerPrepStmts=true
  ```

  该参数在 JDBC 中的原理：ConnectionImpl 在 prepareStatement 时会产生一个 ServerPreparedStatement.在这个 ServerPreparedStatement 对象构造时首先会把当前SQL语句发送给 MySQL 进行预编译，然后将返回的结果缓存起来，其中包含预编译的名称（我们可以看成是当前SQL语句编译后的函数名），签名（参数列表），然后执行的时候就会直接把参数传给这个函数请求MySQL执行这个函数

- MySQL 的连接 url 参数2

  ```mysql
  cachePrepStmts=true
  ```

  实际上，参数1的效果是，每次执行 SQL 语句都会进行预编译，参数开启了，JVM 端才会缓存每个SQL语句的预编译结果，就是以SQL语句为key，将预编译结果缓存起来

  同一条SQL语句尽量在一个全局的地方定义，然后在不同地方引用，这样做一是为方便对 SQL 做统一检查和优化，就像 Mybatis 把 SQL 语句定义在 XML 文件中一样。二是同一语句不同写法，即使空格不同，大小写不同也会重新预编译，因为JVM端缓存是直接以 SQL 本身为 key 而不会对 SQL 格式化以后再做为 key

  某个 PrepareStatement 关闭，并不真正关闭这个底层，因为当前参数的缘故，预编译结果还保留着

