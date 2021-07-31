---
title: "SDN 实验环境配置"
date: 2020-04-18T01:37:56+08:00
lastmod: 2020-04-18T01:37:56+08:00
draft: false
tags: ["SDN",  "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

#### Mininet
见[前一个实验][2]。

#### Ryu
**可以选择从源码编译或直接安装，详见[官网][3]。**

下面是厉害国专属的的简单安装方式
```shell
pip3 install ryu -i https://pypi.doubanio.com/simple
```
下载完一个包后，会卡一段时间，不要以为是真的卡了，它是在安装其它依赖项，等待即可

**安装完成后，就可以查看流表了**

首先，生成流表
```shell
sudo mn
...
mininet> h1 ping h2
```
其次，查看流表。you can use Ctrl + Shift + t to create another tab.
```shell
sudo ovs-ofctl dump-flows s1
```
其中，参数 dump-flows 表示 dump 所有流表。And you will get Flow table like that
```shell
cookie=0x0, duration=3.621s, table=0, n_packets=0, n_bytes=0, idle_timeout=60, priority=65535,arp,in_port="s1-eth2",vlan_tci=0x0000,dl_src=0a:a3:dc:b0:c3:0c,dl_dst=ea:1f:63:88:8c:ec,arp_spa=10.0.0.2,arp_tpa=10.0.0.1,arp_op=2 actions=output:"s1-eth1"
cookie=0x0, duration=3.619s, table=0, n_packets=3, n_bytes=294, idle_timeout=60, priority=65535,icmp,in_port="s1-eth1",vlan_tci=0x0000,dl_src=ea:1f:63:88:8c:ec,dl_dst=0a:a3:dc:b0:c3:0c,nw_src=10.0.0.1,nw_dst=10.0.0.2,nw_tos=0,icmp_type=8,icmp_code=0 actions=output:"s1-eth2"
cookie=0x0, duration=3.616s, table=0, n_packets=3, n_bytes=294, idle_timeout=60, priority=65535,icmp,in_port="s1-eth2",vlan_tci=0x0000,dl_src=0a:a3:dc:b0:c3:0c,dl_dst=ea:1f:63:88:8c:ec,nw_src=10.0.0.2,nw_dst=10.0.0.1,nw_tos=0,icmp_type=0,icmp_code=0 actions=output:"s1-eth1"
```
then it's time to analyze OpenFlow protocol
# A brief analysis of OpenFlow protocol
## What is [OpenFlow][4]
OpenFlow is a communications protocol that gives access to the forwarding plane of a network switch or router over the network
## Protocol content
![](https://www.sdxcentral.com/wp-content/uploads/2013/08/manipulated-openflow-switch.jpg)
## Flow table analysis
[ARP][5]将网络层的IP地址解析为数据链路层的物理地址(MAC地址)
```shell
# 匹配字段
arp                           # 指定数据包的类型是 arp (Address Resolution Protocol)
in_port="s1-eth2"             # switch Port (匹配域)
vlan_tci=0x0000               # VLAN ID
dl_src=0a:a3:dc:b0:c3:0c      # MAC Src (data link source (数据链路层的源地址))
dl_dst=ea:1f:63:88:8c:ec      # MAC Dsl (data link destination (数据链路层的目的地址)
arp_spa=10.0.0.2              # Source IP address of ARP
arp_tpa=10.0.0.1              # Target IP address of ARP
arp_op=2                      # Opcode of ARP
```
详细可见 [OpenFlow Protocol —— Ryubook 1.0 documentation][6]
ICMP是TCP/IP协议族的一个子协议，用于在IP主机、路由器之间传递控制消息。
```shell
icmp                          # 协议类型 Internet Control Message Protocol
nw_src=10.0.0.1               # 源 IP 地址
nw_dst=10.0.0.2               # 目的 IP 地址
```
流表由流条目组成

| Match Fields | Priority | Counters | Instructions | Timeouts | Cookie |
|     ----     |   ----   |   ----   |     ----     |   ----   |  ----  |

详细的 OpenFlow 介绍可以阅读 [openflow-spec-v1.3.1][7]
# 避免数据包的洪泛的⼆层⾃学习交换机
>Ryu的基本教程需要阅读⽂档 [Ryu docs][9]，阅读前两个部分 Getting Started 和 Write Application 即可
Python 编程需要理解 [decorator 的使用][8]

根据老师发的[实验指导说明书][1]和 github 上的 [Ryu 项目][10]，完成实验
运行 Ryu 控制器
```shell
ryu-manager yourapp.py
```
再创建 topo
```shell
sudo mn --topo linear,3 --mac --controller remote
```
![](/img/Snipaste_2020-04-20_00-58-28.png)
在 "mininet> " CLI 下打开 h1, h3 的 xtern，用 wireshark 抓 h1 ping h2 后它们两个的包。得到
![](/img/Snipaste_2020-04-20_01-00-24.png)
可以看到，此控制器避免了数据包的洪泛。
核心代码
```Python
# maybe you need a global data structure to save the mapping
        self.mac_to_port = {}
...
# you need to code here to avoid the direct flooding
        self.mac_to_port[dpid][src] = in_port
        if dst in self.mac_to_port[dpid]:
            out_port = self.mac_to_port[dpid][dst]
        else:
            out_port = ofp.OFPP_FLOOD
        actions = [parser.OFPActionOutput(out_port)]
        if out_port != ofp.OFPP_FLOOD:
            match = parser.OFPMatch(in_port=in_port, eth_dst=dst)
            self.add_flow(datapath, 1, match, actions)
```
另外，可以参考 [Ryu app][11] 了解更多 Ryu 控件的实现。

[1]:/files/sdn-exp/exp-1.pdf
[2]:2020-03-30-SDN-mininet-fattree.md
[3]:https://ryu.readthedocs.io/en/latest/getting_started.html
[4]:https://en.wikipedia.org/wiki/OpenFlow
[5]:https://zh.wikipedia.org/wiki/%E5%9C%B0%E5%9D%80%E8%A7%A3%E6%9E%90%E5%8D%8F%E8%AE%AE
[6]:https://osrg.github.io/ryu-book/en/html/openflow_protocol.html
[7]:https://www.opennetworking.org/wp-content/uploads/2013/04/openflow-spec-v1.3.1.pdf
[8]:https://realpython.com/primer-on-python-decorators/#first-class-objects
[9]:https://ryu.readthedocs.io/en/latest/index.html
[10]:https://github.com/osrg/ryu/blob/master/ryu/app/example_switch_13.py
[11]:https://github.com/osrg/ryu/tree/master/ryu/app
