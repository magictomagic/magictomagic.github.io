---
title: "Simple chat program"
date: 2020-05-06T01:37:56+08:00
lastmod: 2020-05-06T01:37:56+08:00
draft: false
tags: ["project", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

## 基本功能
### [C++ 资源][1]
**自带：主机内通信。**

```shell
# first, run server to listen
./tcp_server localhost 9999

# then, run client to establish connect
./tcp_client localhost 9999
```
**server 固定公网 IP**
>阿里云: CentOS 7

>server IP: 47.94.157.136

開放端口
```shell
systemctl start firewalld
systemctl enable firewalld
systemctl status firewalld
# ...
# running
# ...

firewall-cmd --zone=public --add-port=9999/tcp --permanent

firewall-cmd --reload
# success

firewall-cmd --list-ports    # check
# 9999/tcp
```
server:
```shell
./server1.o 0.0.0.0 9999
```
client:
```shell
./client.o 47.94.157.136 9999
```
come out:
```error log
connect error
: Connection refused
```
ping 可以連接

![](/img/Snipaste_2020-05-05_22-32-46.png)

端口掃不到

![](/img/Snipaste_2020-05-05_22-31-53.png)

是因为阿里云外面又套了一層牆，而之前 [vultr][3] 就不存在这个问题。

不得不感慨，墙国真是从里到外都是墙啊！

![](/img/Snipaste_2020-05-05_22-39-13.png)

端口放行

![](/img/Snipaste_2020-05-05_22-46-36.png)
![](/img/Snipaste_2020-05-05_22-49-20.png)

reboot

if you do not listen 9999 port on server, you will get Connection refused as well.

run on server
```shell
./server1.o 0.0.0.0 9999
```
重新扫描，and Connection success.

![](../img/Snipaste_2020-05-05_22-51-50.png)

阿里云在外面建了一个墙，把里面的墙禁掉，看来是为了符合国人的使用习惯啊：图形化界面设置端口。

implement

![](/img/Snipaste_2020-05-05_23-03-02.png)

### 传输文件
#### 函数说明
[IP 地址格式转换][4]。
#### socket 类型
阻塞型：立即返回，更高效。
非阻塞型：轮询方式。
#### [NAT 穿越 and P2P][5]
NAT 不允许外网主机主动访问内网主机，破坏了端到端的网络通信。

P2P 技术却要求通信双方都能主动发起访问。

**解决方案：**
[特殊场景][5]

![](http://www.52im.net/data/attachment/forum/201903/13/110231szo3jg073l0onjvn.jpg)

基于 UDP 打洞。

[典型场景][5]

单层 NAT

![](http://www.52im.net/data/attachment/forum/201610/10/134836i9jb3hccbq8cz33q.png)

多层 NAT

![](http://www.52im.net/data/attachment/forum/201912/16/122604d3xdqtyx0iotytxt.png)

### [选择解决方案][6]
>原则：典型，普遍，通用，简单

**参考[常见解决技术][7]。**

ALG：适应性不强。

中间件技术：需要网关、内部主机和应用程序都支持UPnP技术。

打洞技术：STUN，对中间服务器轻依赖，不支持对防火墙和对称NAT的穿越；ICE，非对称NAT环境下首选的NAT穿越解决方案。

Relay服务器中转技术：turn，对中间服务器强依赖，当数据量大时，服务器的并发和处理能力成为瓶颈。

**选择方案 ICE**
可以参考[教程][9]选择[免费商品类][8]，注册一个账号，在服务器上搭建。

但出于对这个专业的尊重，还是用 ICE 框架实现吧。

**[测试 NAT 类型][10]**
### 目标：用 ICE 框架实现文件的传输 \ communication
**[ICE 原理][11]**
#### ICE 实现方案
```shell
sudo apt install gtk-doc-tools
wget https://libnice.freedesktop.org/releases/libnice-0.1.16.tar.gz
# 解压到合适位置
 ./configure && make && sudo make install
```
经过






# Java
[貌似现成的][2]

[1]:https://blog.csdn.net/Hanani_Jia/article/details/83449090
[2]:https://juejin.im/post/5d69e0c65188257ec84732c1
[3]:https://github.com/magictomagic/magictomagic.github.io/blob/master/_posts/2020-05-04-CentOS-FFmpeg-%E6%B5%81%E5%AA%92%E4%BD%93%E6%92%AD%E6%94%BE.md#open-firewall
[4]:https://linux.die.net/man/3/ntohs
[5]:http://www.52im.net/thread-542-1-1.html
[6]:http://www.52im.net/thread-2872-1-1.html
[7]:https://www.jianshu.com/p/aee235c6d6d7
[8]:https://www.natfrp.com/
[9]:https://www.7lez.com/?p=120
[10]:http://www.52im.net/thread-609-1-1.html
[11]:https://www.jianshu.com/p/aee235c6d6d7
