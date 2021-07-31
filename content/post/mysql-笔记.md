---
title: "mysql 常识 笔记"
date: 2021-04-01T01:37:56+08:00
lastmod: 2021-04-01T01:37:56+08:00
draft: true
tags: ["mysql"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

## 安装

Ubuntu20.04TLS `sudo apt install mysql-server`

## 基本命令

> 以`;`为脚本结束符，不写则一直追加

```mysql
show databases;
CREATE DATABASE database_name;
USE database_name;

create table t3(comment_texts text, role mediumint, dr mediumint, rs mediumint, part mediumint);

show tables; 

load data infile '/var/lib/mysql-files/label.csv' into table t3 fields terminated by ',' enclosed by '"' lines terminated by '\n' ignore 1 rows;

drop database 库名; 
drop table 表名； 
```



## 数据库结构???

information_schema 
mysql            
performance_schema 
sys   

面试高频？？？系统自带什么东西管理什么？？？

## 数据类型比较???

text vs varchar

# CSV数据的导入导出

## 导入

```mysql
# 获取路径
SHOW VARIABLES LIKE "secure_file_priv";
# 将待导入的文件放到获取到的路径下，我这里是 /var/lib/mysql-files/label.csv
load data infile '/var/lib/mysql-files/label.csv' into table t3 fields terminated by ',' enclosed by '"' lines terminated by '\n' ignore 1 rows;
```

## 导出

先测试一下

# 结合 Python 

## TODO

- [ ] 先`select role, count(*) from t3 group by role;`把数量(num)少于一定程度的统计，之后放到一个文件中。
- [ ] 再`select role, group_concat(comment_texts) from t3 group by role;`以rol为文件名以里面"`,`"分割开的为文件内容，"`,`"最好转换为"`\n`"。导出成csv文件
- [ ] 把上面的操作分装成一个函数，role, t3, num等为参数，调参要先在数据库里执行`select xxx, count(*) from t3 group by xxx;`做一个感觉判断。
- [ ] 把操作中的坑点与关键点记录在这里，把代码与导出后的文件整合到项目中，根据文件调参，要感觉参数而不是书本参数

## 最后决定

不搞上面这些。count了以后直接excel看，然后`感觉算法`。