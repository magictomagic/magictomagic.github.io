---
title: "Redis 笔记"
date: 2021-02-07T01:37:56+08:00
lastmod: 2021-02-07T01:37:56+08:00
draft: false
tags: ["Redis"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# 基础

## 操作

> Redis.conf

```bash
daemonize no # 默认不以守护进程方式运行 # 变为 daemonize yes
bind 127.0.0.1 # 绑定的IP # 注释掉 让其它主机来访问
requirepass # 设置密码 登录 > auth yourpassword
pidfile /var/run/redis_6379.pid # 如果以后台方式运行，需要指定pid文件
logfile "xxx" # 日志的文件名，若为空，则为标准输出

save 900 1 # 900s内，如果至少1个key进行了修改，进行持久化操作
save 60 10000 # 60s内，如果至少10000个key进行了修改，进行持久化操作

stop-writes-on-bgsave-error yes # 持久化如果出错，是否继续工作
rdbcompression yes # 是否压缩rdb文件，需要消耗一些cpu资源
rdbchecksum yes # 保存rdb文件时，进行错误检查校验
dir ./ # rdb 文件保存目录
```

登录（客户端访问）

```shell
redis-cli -h IP地址 -p 端口 -a 密码
```

```shell
select 3 # 切换编号为3的数据库（默认16个数据库）
dbsize # 当前数据库大小（键的数量）
flushdb # 清除当前数据库
flushall # 清空全部数据库
```

## 概念

+ 没有什么是加一层解决不了的
+ 数据一直新增，内存很快占满：1.为数据设置超时时间。2.设置内存空间。3.6-8种LRU

基于内存，CPU不是性能瓶颈，瓶颈是机器的内存和网络带宽。

Redis作用：数据库、缓存、消息中间件

+ Redis 单条命令是原子性的，但事务（一组命令的集合，所有命令都会被序列化，按顺序执行）不保证原子性（不被打断，同时成功|同时失败）的。
+ 没有隔离级别的概念？？？
+ Redis 
  * 开启事务(multi)
  * 命令入队(一系列的命令)
  * 执行事务(exec) | 放弃事务(discard)

# 基本数据类型

> 记动词，后面就看提自动示了
>
> key 为你自己的键名
>
> 下标都从0开始（区别于显示的下标（要-1））

## String

+ 计数器
+ 统计多单位的数量 like:23:counts:44:nums:13

```shell
set key xxx 
get key 
exists key
move key 1 # 移除 当前数据库(用1来代表)key
getset key xxxx # 返回本来存储的值(或nil)，设置新值为xxxx

# 缓存：热点数据自动过期
expire key 10 # 设置key过期时间为10秒
ttl key # 查看还有多长时间过期，若返回-2，则get不到，说明已经过期了

append key xxx # 在原key上追加为xxx的字符串，返回追加后字符串的总长度；若当前key不存在，相当于创建（set）

# set key 2
incr key # +1
decr key # -1
incrby key 13 # +13
decrby key 34 # -34

getrange key 0 3 # 取[0,3]字符
getrange key 0 -1 # 取所有
setrange key 2 xx # 从下标为2的地方开始替换

setex key 34 dfas # 设置key的值为dfas，34秒后过期
setnx key asd # 如果key不存在，创建key。在分布式锁中经常使用。

mset key1 value1 key2 value2 ... # 同时设置多个值
mget key1 key2 ... # 同时获取多个值

msetnx key1 value1 key2 value2 # 原子性操作
```

## List

+ 栈、队列，阻塞队列，消息队列
+ 实际上是个链表

```shell
lpush key xxx
rpush key xxx
lpop key
rpop key
lrange key 0 -1
lindex key 2 # 获取下为2的元素
llen key # 获取长度
lrem key 2 xxx # 删除2个值为xxx的元素（若数字为0，则是所有）
ltrim key 3 5 # 截取[3,5]
rpoplpush src dst # 移除列表中最后一个元素，将他添加到新的列表中
lset key 2 xxx # 更新下标为2的值为xxx。如果不存在则报错。

# 插入值
linsert key [before|after] value xxx # 在 value 前|后 插入 xxx 
```

## Set

```shell
sadd key xxx
smembers key
scard key
sismember key xxx # 看key中xxx是否存在
srem key xxx # 删除为xxx的元素
srandmember key # 随机取一个
srandmember key 2 # 随机取两个
spop key # 随机移除元素
smove src dst xxx # 移动元素

sdiff key1 key2 # 差集 key1 - key2
sinter key1 key2 # 交集
sunion key1 key2 # 并集
```

## Hash

适合存储经常变更的数据、对象的存储

```shell
hset key hashkey hashvalue
hget key hashkey 
hmset key hashkey1 hashvalue1 hashkey2 hashvalue2 ...
hmget key hashkey1 hashkey2 ...
hdel key hashkey1
hlen key
hexists key hashkey

hgetall key
hkeys hashkey # 获取所有key
hvals hashvalue # 获取所有value

hincrby key hashkey 2 # 增2
hsetnx key hashkey hashvalue # 若不存在则set，否则失败
```

## Zset

排行榜

带权重消息

```shell
zadd key score value # 以score排
zrangebyscore key -inf +inf # 升序排列
zrangebyscore key -inf +inf withscores # 显示score
zrangebyscore key -inf 400 # 圈定范围
zrevrangebyscore key +inf -inf # 降序排列
zrange key 0 -1
zrem key value # 删除元素
zcard key # 统计总数量
zcount key 3 5 # 统计score在3到5之间的元素数量

```

# 特殊数据类型

## geospatial

```shell
geoadd key 120.34 30 24 hangzhou # 键值 经度 纬度(到85) 城市
geopos key hangzhou # get 值
geodist key value1 value2 [km] # 两地距离（默认m，可设置为km等）mi英里 ft英尺
georadius key x y radius [km] [withdist]|[withcoord] [count 1] # x y 经纬度；半径；显示距离|显示坐标；筛选出指定的结果（数量）
georadiusbymember key hangzhou 200 km # 根据城市找附近城市

zrange key 0 -1 # list all
zrem key hangzhou # delete
```

## Hyperloglog

固定内存，统计用户数量，0.81%错误率。

网页UA（同一个用户不重复统计）

```shell
pfadd key a s d f g
pfcount key # 统计数量
pfmerge dst src1 src2 ... # 合并，去重

```

## Bitmap

位存储：登录|未登录，是否打卡，活跃

```shell
setbit key num 0|1 # num 为整数，后面跟0或1
getbit key num
bitcount key # 统计
```

# 锁

## 悲观锁

无论做什么都加锁

## 乐观锁

更新数据时去判断一下，期间是否有人去修改过数据，若修改，则提交失败

```shell
watch key 
multi
...
exec # 若执行前另一个线程使key改动，则ecec返回(nil)，则提交失败
```

# SpringBoot 整合 ？？？

jedis: 直连，线程不安全。若要安全，jedis pool。像BIO

lettuce: 采用netty，线程安全。像NIO

# 持久化

> Redis.conf

## 限制 CLIENTS

```shell
maxclinets 10000 # 设置能连接上redis的最大客户端数量
maxmemory <bytes> # redis 配置最大的内存容量
maxmemory-policy noeviction # 内存达到上限之后的处理策略
 1. volatile-lru: 只对设置了过期时间的key进行LRU（默认值）
 2. allkeys-lru: 删除lru算法的key
 3. volatile-random: 随机删除即将过期的key
 4. allkeys-random 随机删除
 5. volatile-ttl: 删除即将过期的key
 6. noeviction: 用不过期，返回错误
```


## RDB 持久化

单独fork一个子进程进行持久化，临时生成RDB替换正式RDB文件，占用一定空间，若最后一次持久化宕机，则无法保存数据

```shell
dbfilename dump.rdb
```

触发机制：（生成RDB文件）

1. save规则满足
2. 执行flushall
3. 退出

### 恢复rdb文件

将rdb文件放在启动目录即可，redis启动自动检查dump.rdb

`config get dir # 查看需要存在的位置`

`bgsave` 保存快照rdb

### RDB优点
适合大规模数据恢复
对数据完整性要求不高

## AOF 配置
以日志的形式记录每一个写的操作。数据文件大，性能低

>配置

```shell
appendonly on # 默认以rdb方式持久化，不开启aof。开启要设置成yes。

# 后面常常保持默认
appendfilename "appendonly.aof" # 持久化文件的名字(记录写操作)

appendfsync always # 每次修改都会同步，消耗性能
appendfsync everysec # 每秒执行一次sync，可能会丢失这1秒的数据
appendfsync on # 不执行sync，自己同步，速度最快
```

# 发布订阅

消息通信模式

实时消息系统，实时聊天

```shell
subscribe channel # 接收端：订阅
publish channel message # 发送端：向channel发布消息message
```

底层：频道存于字典键值，订阅者由此键值牵头组成链表。

# 主从复制

主：写     从：只能读

数据冗余、故障恢复、负载均衡、高可用基石

```shell
info replication # 查看当前库的信息
slaveof ip_address port # 认老大，e.g. slaveof 127.0.0.1 6379
slaveof no one # 自己当老大
```

## 集群搭建

复制多个redis.conf，修改 port、pidfile、logfile、dbfilename  (防止冲突)

启动 `redis-server path/redis.conf` 后，若对应的 .log 文件生成，则说明启动成功。 

+ 若命令行使从机关了又开，则从机变为主机，再次执行slaveof，则同步主机的内容（slave发一个sync同步命令，全量复制）。

## 哨兵模式

> 自动选举老大. 自动化主从模式.
>
> 哨兵，监控所有主机是否是活着的，若master宕机，则将slave切换成master，然后通过**发布订阅模式**通知其他的slave，修改配置，使它们变成主机

多哨兵模式：哨兵之间互相监控。仅哨兵自己认为服务器不可用,为主观下线,认为的多了,客观下线,然后进行投票(投票算法),有结果后,进行故障转移(failover)操作,然后通过订阅发布模式...

`redis-sentinel kconfig/sentinel.conf` 启动哨兵. 配置文件如下:

```shell
# sentinel moniter 被监控的名称 hostIP port 1(代表主机挂了)
sentinel moniter myredis 127.0.0.1 6379 1
```

flaws: 1. Redis 不好在线扩容 2. 配置麻烦(选择很多)

# Redis 缓存穿透与雪崩

## 缓存穿透

查询没有命中缓存,直接去mysql去查,给持久层造成很大的压力

### 解决方案

布隆过滤器: 对所有可能查询的参数以hash存储,在控制层进行校验,不符合则丢弃.

缓存空对象: 存储层没命中,返回空对象,缓存起来,设置一个过期时间. flaw: 缓存空值的键占空间; 缓存层与数据层有一段时间数据不一致, 对需要保持一致性的业务会有影响

## 缓存击穿

一个热点key失效的瞬间大并发直接请求数据库,就像在屏障上早开了一个洞.

### 解决方案

+ 设置热点数据永不过期

+ 加互斥锁: 分布式锁. 加锁,只保证有一个线程能够进Mysql, 将高并发压力转移到了分布式锁.

## 缓存雪崩

某一时间段,缓存集中过期, Redis 宕机

### 解决方案

+ 异地多活

+ 限流降级

+ 数据预热: 部署前把所有可能的数据都先访问一遍. 不同的key设置不同的过期时间,使缓存失效的时间尽量均匀.

  

https://www.bilibili.com/video/BV1S54y1R7SB?p=1