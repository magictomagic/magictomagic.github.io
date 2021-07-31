---
title: "SDN Mininet"
date: 2020-04-19T01:37:56+08:00
lastmod: 2020-04-19T01:37:56+08:00
draft: false
tags: ["SDN", "Mininet", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# Mininet analyze
在[配置环境][1]后，执行
```shell
ipconfig -a
```
对比创建了 Mininet 后，在 mininet> 下，【打开一个新的终端(Ctrl + Alt + t)】**或**【在已有终端上打开另一个新的终端(Shift +Ctrl + n)】(用**或**是因为为两者结果一样)，执行同样的命令
![](/img/mn-c.png)![](/img/mininet-2.png)
发现主机里的网络设备多了 s1, s1-eth1, s1-eth2, 分别表示控制器，连接 h1, h2 的端口
若没有正常退出 Mininet，会有 ovs-system, s2, s3 残留
![](/img/bj-4.png)
可以使用命令
```shell
sudo mn -c
```
清除多余的控件。
# Ryu + Mininet 基本命令
```shell
# shell prompt
mn -h # 查看mininet命令中的各个选项
sudo mn -c # 不正确退出时清理mininet
# 下⾯的命令可以在'sudo mn'新建的简单拓扑上查看运⾏结果
# mininet CLI
net # 显示当前⽹络拓扑
dump # 显示当前⽹络拓扑的详细信息
xterm h1 # 给节点h1打开⼀个终端模拟器
sh [COMMAND] # 在mininet命令⾏中执⾏COMMAND命令
h1 ping -c3 h2 # 即h1 ping h2 3次
pingall # 即ping all
h1 ifconfig # 查看h1的⽹络端⼝及配置
h1 arp # 查看h1的arp表
link s1 h1 down/up # 断开/连接s1和h1的链路
exit # 退出mininet CLI
# ovs(run in shell prompt)
sudo ovs-ofctl show s1 # 查看交换机s1的基本信息
sudo ovs-ofctl dump-flows s1 # 查看s1的流表
sudo ovs-ofctl -O OpenFlow13 dump-flows # 查看s1中OpenFlow1.3版本的流表信息
```
# h1, h2 间通信协议分析
参考[简析 OpenFlow 协议][2]


[1]:https://magictomagic.github.io/2020/04/18/SDN-ex1/
[2]:https://magictomagic.github.io/2020/04/18/SDN-ex1/#a-brief-analysis-of-openflow-protocol
