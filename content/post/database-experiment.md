---
title: "Database experiment"
date: 2020-05-08T01:37:56+08:00
lastmod: 2020-05-08T01:37:56+08:00
draft: false
tags: ["database", "project", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---
## 数据库实验
### 环境选择
#### 目标：切题、易用、熟悉数据库命令
切题：选择数据库版本为 SQL SERVER

易用：支持 Excel 导入数据

熟悉数据库命令：支持命令行操作
#### 客户端
>环境：Windows 10

[安装][3]图形化管理软件。
>支持 2020/5/8 版本，if you are rich，请支持正版。

#### 连接 SQL SERVER
##### 客户端
可以使用自带的 2008 年的版本，也可以参考[提示][2]在[官网][1]找到 Microsoft® SQL Server® 2012 ADOMD.NET，选择对应版本的软件。
##### 服务端
可以选择远程或本地，因为租赁服务器要花钱，服务器没了数据都没了，且远程实现和本地实现知数连接的主机不同，其它操作无异。
###### 服务端安装与连接
可以参考[stackoverflow][5]的优质回答，也可以直接安装[开发版][4]，虽然比起付费的，版本又老，功能又少，但功能完全满足实验需求，且占内存小，适合本地环境。

![](/img/Snipaste_2020-05-07_00-04-33.png)

保存数据
```conf
Server=localhost;Database=master;Trusted_Connection=True;
C:\Program Files\Microsoft SQL Server\140\Setup Bootstrap\Log\20200506_235813
C:\SQLServer2017Media\Developer_ENU
C:\Program Files\Microsoft SQL Server\140\SSEI\Resources
```

装 SSMS 最好尽可能多地关闭其它正在运行的程序，之后参考[官网教程][6]，配置好用户，利用之前装的 GUI 工具连接。

![](/img/Snipaste_2020-05-07_01-01-12.png)

#### [导入数据][8]
##### Excel 导入
**[四种方法][8]**
###### [通过 SSMS 导入][7]
###### 通过 Navicat CLI 导入
>我选择在 Navicat + CLI 管理表，一方面是因为图形化界面对照教程用用太麻烦，另一方面，毕竟要对得起我的专业啊，多写代码 :)

**问题说明**

S# 在 Excel 里自动把前面的【0】丢掉了，后来才看见老师在群里说不能省略【0】。

**创建表**
```sql
CREATE TABLE JSJ74_S634
 (S#			CHAR(20)			NOT NULL,
  SNAME 	VARCHAR(8)	NOT NULL,
	SEX 		VARCHAR(4)	DEFAULT '男',
	BDATE 	DATE 				NOT NULL,
	HEIGHT 	NUMERIC(3,2) NOT NULL,
	DORM    VARCHAR(20)	NOT NULL,
 PRIMARY KEY (S#),
 )

 CREATE TABLE JSJ74_C634
  (C#			VARCHAR(20)	NOT NULL,
	 CNAME 	VARCHAR(160) NOT NULL,
	 PERIOD INT			NOT NULL,
	 CREDIT INT			NOT NULL,
	 TEACHER VARCHAR(8) NOT NULL,
 PRIMARY KEY (C#),
 )

CREATE TABLE JSJ74_SC634
	(S#			CHAR(20)			NOT NULL,
   C#     VARCHAR(20) NOT NULL,
	GRADE   DEC(4,1)    DEFAULT NULL,
PRIMARY KEY (S#, C#),
FOREIGN KEY (S#)
		REFERENCES JSJ74_S634
		ON DELETE  CASCADE,   
FOREIGN KEY (C#)
		REFERENCES JSJ74_C634
		ON DELETE NO ACTION,
CHECK (GRADE IS NULL OR GRADE BETWEEN 0 AND 100)
	)
```
![](/img/Snipaste_2020-05-08_02-33-24.png)

**注意点**：

中文一个字算两个字，在  CREATE TABLE JSJ74_C634 的 TEACHER VARCHAR(6) NOT NULL 中, 只能容纳 3 个字的名字。

程序顺序执行，若后面遇到问题，前面的命令做出的改变仍然提交，可以用回滚或相反的命令撤销操作；若前面遇到问题，则后面的操作不被执行。

使用 HEIGHT 	DECIMAL(1,2) NOT NULL 会有截断错误，因为原始数据小数点后不是严格的两位，可以在原始数据中将格式统一设置好，或使用 NUMERIC 数据类型。

**CLI 导入**

将 Excel 数据另存为 .txt 格式。
```txt
1032010	王涛	男	1992-04-05	1.72	东14舍221
1032023	孙文	男	1993-06-10	1.8	东14舍221
1032001	张晓梅	女	1993-11-17	1.58	东1舍312
1032005	刘静	女	1992-01-10	1.63	东1舍312
1032112	董蔚	男	1992-02-20	1.71	东14舍221
3031011	王倩	女	1993-12-20	1.66	东2舍104
3031014	赵思扬	男	1991-06-06	1.85	东18舍421
3031051	周剑	男	1991-05-08	1.68	东18舍422
3031009	田婷	女	1992-08-11	1.6	东2舍104
3031033	蔡明明	男	1992-03-12	1.75	东18舍423
```
执行 BULK 命令。
>感谢 [stackoverflow][9]。

```sql
BULK INSERT JSJ74_S634
FROM 'C:\Users\magic\Desktop\JSJ74_S634.txt'
WITH     
   (
                FIELDTERMINATOR = '\t',
                ROWTERMINATOR = '\n'
    )
GO
```
反馈。

![](/img/Snipaste_2020-05-09_00-11-26.png)

之后数据的导入类似，截图见文末。
#### 练习题
##### 查询
(1) 查询电子工程系所开课程的课程编号、课程名称及学分数。

![](/img/Snipaste_2020-05-09_01-08-46.png)

(2) 查询未选修课程“CS-01”的女生学号及其已选课程编号、成绩。

![](/img/Snipaste_2020-05-09_01-57-48.png)

(3) [查询1993年～1994年出生的学生的基本信息。][10]

![](/img/Snipaste_2020-05-09_02-08-15.png)

(4) 查询每位学生的学号、学生姓名及其已修学分数。

![](/img/Snipaste_2020-05-09_04-11-06.png)

(5) 查询选修课程“CS-02”的学生中成绩第二高的学生学号。

![](/img/Snipaste_2020-05-09_03-10-46.png)

(6) 查询平均成绩超过“王涛“同学的学生学号、姓名和平均成绩，并按学号进行降序排列。

![](/img/Snipaste_2020-05-09_04-26-40.png)

(7) 查询选修了3门以上课程（包括3门）的学生中平均成绩最高的同学学号及姓名

![](/img/Snipaste_2020-05-09_18-45-18.png)

##### 增删改
(1) 分别在JSJ74_S634和JSJ74_C634表中加入记录(‘01032005’，‘刘竞’，‘男’，‘1993-12-10’，1.75，‘东14舍312’)及(‘CS-03’，“离散数学”，64，4，‘陈建明’)。

![](/img/Snipaste_2020-05-09_19-02-25.png)
![](/img/Snipaste_2020-05-09_19-03-56.png)

(2) 将JSJ74_S634表中已修学分数大于60的学生记录删除。

![](/img/Snipaste_2020-05-09_20-24-12.png)

(3) 将“张明”老师负责的“信号与系统”课程的学时数调整为64，同时增加一个学分。

![](/img/Snipaste_2020-05-10_02-00-57.png)

##### 视图
(1)居住在“东18舍”的男生视图，包括学号、姓名、出生日期、身高等属性。

![](/img/Snipaste_2020-05-10_02-15-05.png)

(2)“张明”老师所开设课程情况的视图，包括课程编号、课程名称、平均成绩等属性。

![](/img/Snipaste_2020-05-10_02-46-48.png)
![](/img/Snipaste_2020-05-10_03-04-15.png)

(3)所有选修了“人工智能”课程的学生视图，包括学号、姓名、成绩等属性。

![](/img/Snipaste_2020-05-10_02-56-33.png)
![](/img/Snipaste_2020-05-10_02-57-09.png)

## 常用命令
清空表中所有数据
```sql
truncate TABLE JSJ74_C634
```
删除所有表，因为存在依赖，执行两次。
```sql
DROP TABLE JSJ74_S634
DROP TABLE JSJ74_C634
DROP TABLE JSJ74_SC634
```


## 实验截图
![](/img/Snipaste_2020-05-09_00-21-14.png)
![](/img/Snipaste_2020-05-09_00-23-49.png)



[1]:https://www.microsoft.com/en-us/download/confirmation.aspx?id=29065
[2]:https://docs.microsoft.com/en-us/sql/relational-databases/native-client/applications/installing-sql-server-native-client?view=sql-server-ver15
[3]:https://github.com/magictomagic/magictomagic.github.io/blob/master/files/Navicat%20Premium%2015.x%E6%9C%80%E6%96%B0%E6%B3%A8%E5%86%8C%E6%9C%BA.rar
[4]:https://www.microsoft.com/en-us/sql-server/sql-server-2017-pricing#CP_StickyNav_1
[5]:https://stackoverflow.com/questions/38556327/any-free-version-of-sql-server
[6]:https://docs.microsoft.com/zh-cn/sql/relational-databases/lesson-1-connecting-to-the-database-engine?view=sql-server-ver15
[7]:https://docs.microsoft.com/en-us/sql/relational-databases/import-export/import-data-from-excel-to-sql?view=sql-server-ver15#wiz
[8]:https://www.sqlshack.com/import-data-excel-file-sql-server-database/
[9]:https://stackoverflow.com/questions/13124680/how-to-bulk-insert-from-xlsx-file-extension
[10]:https://stackoverflow.com/questions/44528286/date-is-incompatible-with-int
