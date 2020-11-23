## SQL

### 规范

- 数据订正（特别是删除或修改记录操作）时，要先 select，避免出现误删除，确认无误才能执行更新语句

- 对于数据库中表记录的查询和变更，只要涉及多个表，都需要在列名前加表的别名（或表名）进行限定

  说明：对多表进行查询记录、更新记录、删除记录时，如果对操作列没有限定表的别名（或表名），并且操作列在多个表中存在时，就会抛异常。 

  正例：select t1.name from table_first as t1 , table_second as t2 where t1.id=t2.id; 

  反例：在某业务中，由于多表关联查询语句没有加表的别名（或表名）的限制，正常运行两年后，最近在某个表中增加一个同名字段，在预发布环境做数据库变更后，线上查询语句出现出1052异常：Column 'name' in field list is ambiguous

- SQL语句中表的别名前加as，并且以t1、t2、t3、...的顺序依次命名。 说明：

  1）别名可以是表的简称，或者是依照表在SQL语句中出现的顺序，以t1、t2、t3的方式命名

  2）别名前加as使别名更容易识别。 

  正例：select t1.name from table_first as t1, table_second as t2 where t1.id=t2.id;

- in操作能避免则避免，若实在避免不了，需要仔细评估in后边的集合元素数量，控制在1000个之内

- 在表查询中，一律不要使用 * 作为查询的字段列表，需要哪些字段必须明确写明。 说明：

  1）增加查询分析器解析成本

  2）增减字段容易与resultMap配置不一致

  3）无用字段增加网络消耗，尤其是text类型的字段

### 优化方法

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

### 性能优化目标

​	至少要达到 range 级别，要求是ref级别，如果可以是consts最好。说明：

​	1） consts 单表中最多只有一个匹配行（主键或者唯一索引），在优化阶段即可读取到数据

​	2） ref 指的是使用普通的索引（normal index）

​	3） range 对索引进行范围检索

​	4） index 索引物理文件全扫描，速度非常慢，与全表扫描是小巫见大巫

### 索引失效

#### 查询条件（待完善）

**核心**：不符合最左原则

- 表达式计算：... WHERE `id` + 1 = 2

- 函数计算：... WHERE xxx(id) = 1

- 隐式转换：数据表字段的编码是 utf8mb4，查询条件的字段值编码是 utf8
- or
- in、not in
- is null、is not null

#### 组合索引

​	注意查询条件字段的顺序、排序条件的字段的规则得相同（要不都升序、要不都降序）

#### 排序

- 如果GROUP BY的字段上如果有索引，因为索引默认有序，可以避免排序行为。所以如果你的分组对排序没有要求，且字段没有对应索引，一定要显示指定不排序，例：`GROUP BY xxx ORDER BY NULL`

- 注意利用索引的有序性。order by 最后的字段是组合索引的一部分，并且放在索引组合顺序的最后，避免出现file_sort的情况，影响查询性能。

  正例：where a=? and b=? order by c; 索引：a_b_c 反例：索引如果存在范围查询，那么索引有序性无法利用，如：WHERE a>10 ORDER BY b; 索引a_b无法排序

#### 索引提示

​	当MySQL确实因为某些原因，采用了错误的执行计划（没有使用预期的索引），可以采用使用 FORCE INDEX 手动指定的方式，让MySQL去使用指定的索引进行查询。但是，这仅作为一种应急预案，不推荐采用该方式，因为数据库迁移可能就不支持了（拓展性不好），还要重新做一个代码发布。首先MySQL关于优化器的设计是复杂且优秀的，我们应该尽量做到以提示的方式，而不是强制的

​	1） 从SQL语句的角度：相同目的，不同的SQL

​	2） 索引统计信息有问题：`analyze table` 一下

​		简单说一下原理就是，MySQL的Optimizer（优化器），采用随机取样来估计（即不是一个一定准确的值）指定列数据的散列层面（区分度），并估计包括回表等IO次数等成本，会决定是否采用相关索引或是全表扫描。有时优化器可能会因为数据

​	3） 删除误导MySQL的索引，这个确实是一条在实际中，确实挺常用的技巧

```mysql
IGNORE INDEX(index1, index2, ...)
FORCE INDEX(index1, index2, ...)
```

### 场景

#### JOIN

1. 代替子查询
2. 阿里规范规定

- 超过三个表禁止join

- 需要join的字段，数据类型保持绝对一致

  多表关联查询时，保证被关联的字段需要有索引

- 即使双表join也要注意表索引、SQL性能

#### UNION（待补充）

​	union 是将 union all 后的结果镜像一次distinct，去除重复的记录后的结果，如果对数据的重复方面没有要求，则应该使用 union all

#### 大批量分页

> 代码中写分页查询逻辑时，若count为0应直接返回，避免执行后面的分页语句

利用延迟关联或者子查询优化超多分页场景。 说明：MySQL并不是跳过 offset 行，而是取 offset+N 行，然后返回放弃前 offset 行，返回 N 行，那当 offset 特别大的时候，效率就非常的低下，要么控制返回的总页数，要么对超过特定阈值的页数进行 SQL 改写。实际应当参照如下方式进行优化，编写 SQL语句

```mysql
-- 例1：10 000 后查100
-- 通用：先查询满足条件分页后的 id，再根据 id 查询数据
SELECT 
	t1.* 
FROM 
	表名
AS t1, (select id from 表名 where 条件 LIMIT 100000, 100) AS t2 
WHERE 
	t1.id = t2.id

-- 例2：10 0000 后查100条
-- 条件：id 是递增的
SELECT 
	t1.* 
FROM 表名
WHERE 
	查询条件 AND id >= (SELECT id FROM 表名 WHERE 查询条件 LIMIT 100000, 1)
LIMIT 100
```

#### 合并表数据

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

#### 主键ID生成策略

1. UUID

   **优点**

   - 生成足够简单，本地生成无网络消耗，具有唯一性

   **缺点**

   - 无序的字符串，不具备趋势自增特性
   - 没有具体的业务含义
   - 长度过长16 字节128位，36位长度的字符串，存储以及查询对MySQL的性能消耗较大，MySQL官方明确建议主键要尽量越短越好，作为数据库主键 `UUID` 的无序性会导致数据位置频繁变动，严重影响性能。

2. 数据库自增ID

   **优点**

   - 实现简单，ID单调自增，数值类型查询速度快

   **缺点**

   - DB单点存在宕机风险，无法扛住高并发场景

3. 数据库自增ID（集群模式）

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

4. 数据库的号段模式

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

5. Redis

   - 利用其`incr`命令实现ID的原子性自增

   - 注意持久化

     ​	`RDB`会定时打一个快照进行持久化，假如连续自增但`redis`没及时持久化，而这会Redis挂掉了，重启Redis后会出现ID重复的情况。

     ​	`AOF`会对每条写命令进行持久化，即使`Redis`挂掉了也不会出现ID重复的情况，但由于incr命令的特殊性，会导致`Redis`重启恢复的数据时间过长。

6. Snowflake

   - twitter公司内部分布式项目采用的ID生成算法，开源后广受国内大厂的好评，在该算法影响下各大公司相继开发出各具特色的分布式生成器

   - `Snowflake`生成的是Long类型的ID，一个Long类型占8个字节，每个字节占8比特，也就是说一个Long类型占64个比特

   - Snowflake ID组成结构：`正数位`（占1比特）+ `时间戳`（占41比特）+ `机器ID`（占5比特）+ `数据中心`（占5比特）+ `自增值`（占12比特），总共64比特组成的一个Long类型
     - 第一个bit位（1bit）：Java中long的最高位是符号位代表正负，正数是0，负数是1，一般生成ID都为正数，所以默认为0
     - 时间戳部分（41bit）：毫秒级的时间，不建议存当前时间戳，而是用（当前时间戳 - 固定开始时间戳）的差值，可以使产生的ID从更小的值开始；41位的时间戳可以使用69年，(1L << 41) / (1000L * 60 * 60 * 24 * 365) = 69年
     - 工作机器id（10bit）：也被叫做`workId`，这个可以灵活配置，机房或者机器号组合都可以
     - 序列号部分（12bit）：自增值支持同一毫秒内同一个节点可以生成4096个ID
   - 具体实现：https://github.com/beyondfengyu/SnowFlake

7. Uid-generator - 百度

   - 具体实现：https://github.com/baidu/uid-generator/blob/master/README.zh_cn.md

8. Leaf - 美团

   - 同时支持号段模式和雪花模式

   - 具体实现：https://github.com/Meituan-Dianping/Leaf

9. Tinyid - 滴滴

   - 具体实现：https://github.com/didi/tinyid

[微信公众号]: https://mp.weixin.qq.com/s?__biz=MzU5NTgzMDYyMA==&amp;mid=2247488761&amp;idx=1&amp;sn=4909fe1220c6d3cdf762e09766561905&amp;chksm=fe6aa6cac91d2fdcb6a2d61aa0ea8927d3348fb1b29c9356502ec343a8fcaab370a015791582&amp;scene=126&amp;sessionid=1591504903&amp;key=55850329b0c39edb7b515cbb9a50a69bfaf44cacfa71990fad565f4e35dec28d696581116eb0a79974a4fbc1b40d85349f8362f928dc365b8ab6fa8d0f5526488dee15ef8a0b8578fc26dfb5adbde067&amp;ascene=1&amp;uin=MjQzMjg2NTAzMA%3D%3D&amp;devicetype=Windows+10+x64&amp;version=62090070&amp;lang=zh_CN&amp;exportkey=AVO8s81eVVWpDJbHZ5x7YDs%3D&amp;pass_ticket=tSWfkndbTMJy7UxUdd2J66UIpvpKQOW8oIRIcNYmXmcBRwagb%2BJTdjMmaw5wKjWp	"Java 专栏"

## 数据库设计

### 字段

- 必备字段 id、create_time、update_time
- 虽然违背 3NF，但是实际中，字段允许适当冗余，以提高查询性能，但必须考虑数据一致。频繁修改、唯一索引、大字段不允许冗余
- 命名不要用模糊不具体的单词，如 type

**编码**

- 因国际化需要，所有的字符存储与表示，均采用utf8字符集，那么字符计数方法需要注意

  说明： SELECT LENGTH("轻松工作")； 返回为12 SELECT CHARACTER_LENGTH("轻松工作")； 返回为4

  如果需要存储表情，那么选择utf8mb4来进行存储，注意它与utf8编码的区别

**类型**

- 任何非负数的字段，必须是 unsigned

- 尽可能将所有列定义为 not null

  不要使用count(列名)或count(常量)来替代count(\*)，count(\*)是SQL92定义的标准统计行数的语法，跟数据库无关，跟NULL和非NULL无关。 说明：count(\*)会统计值为NULL的行，而count(列名)不会统计此列为NULL值的行

  count(distinct col) 计算该列除NULL之外的不重复行数，注意 count(distinct col1, col2) 如果其中一列全为NULL，那么即使另一列有不同的值，也返回为0

  当某一列的值全是NULL时，count(col)的返回结果为0，但sum(col)的返回结果为NULL，因此使用sum()时需注意NPE问题。 正例：可以使用如下方式来避免sum的NPE问题：SELECT IFNULL(SUM(column), 0) FROM table

  使用ISNULL()来判断是否为NULL值。 说明：如果在null前换行，影响可读性；从性能数据上分析，`ISNULL(column)`执行效率更快一些

- 如果存储的字符串长度几乎相等，使用char定长字符串类型

- varchar 类型的字段如果存储长度超过 5000，定义字段类型为 text 需要独立出来一张表，避免影响其他字段索引效率

**命名**

- 库名与应用名称尽量一致

- 表名不使用大写字母，表名不使用复数名词

- 表的命名最好是遵循“业务名称_表的作用”。 正例：alipay_task / force_project / trade_config

- 不使用保留字

- 表达是与否概念的字段，必须使用is_xxx的方式命名，数据类型是unsigned tinyint（1表示是，0表示否），例如：is_deleted

  注意：POJO类中的任何布尔类型的变量，都不要加is前缀，所以，需要在<resultMap>设置从is_xxx到Xxx的映射关系。数据库表示是与否的值，使用tinyint类型，坚持is_xxx的命名方式是为了明确其取值含义与取值范围

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

​	业务上具有唯一特性的字段，即使是组合字段，也必须建成唯一索引。 说明：不要以为唯一索引影响了insert速度，这个速度损耗可以忽略，但提高查找速度是明显的；另外，即使在应用层做了非常完善的校验控制，只要没有唯一索引，根据墨菲定律，必然有脏数据产生

#### 组合索引

​	建组合索引的时候，区分度最高的在最左边。 正例：如果where a=? and b=?，a列的几乎接近于唯一值，那么只需要单建idx_a索引即可。 说明：存在非等号和等号混合判断条件时，在建索引时，请把等号条件的列前置。如：where c>? and d=? 那么即使c的区分度更高，也必须把d放在索引的最前列，即建立组合索引 idx_d_c

#### 覆盖索引

​	利用覆盖索引来进行查询操作，避免回表。索引的分类就上面三种，覆盖索引只是一种查询的一种效果，用explain的结果，extra列会出现：using index