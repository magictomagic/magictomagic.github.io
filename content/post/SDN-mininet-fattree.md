---
title: "SDN mininet fattree creat"
date: 2020-03-30T01:37:56+08:00
lastmod: 2020-03-30T01:37:56+08:00
draft: false
tags: ["SDN", "fattree"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

### 环境
>VMware
Host: Windows10    
Guest: Ubuntu18.04 LTS
##安装mininet
>参考[这里][1]、[这里][11]
```shell
# git clone git://github.com/mininet/mininet
# cd mininet/util/
# ./install.sh -a   # -a 表示安装所有组件和依赖
```
**注意：使用 socks 代理来连接国外的源会导致部分文件无法下载，因为这些文件的下载不支持 socks 代理，可以换国内的源**

##[Mininet基本操作][16]
####dump
![](https://i.loli.net/2020/03/29/JBATHlzCIfjcFq4.png)
####ping
![](https://i.loli.net/2020/03/29/1lspcemTCPG6YoO.png)


##编写 fat tree 拓扑
>思路参考[这里][2]
代码参考[这里][3]

##运行定制拓扑(python脚本)
>若采用推荐的 --switch ovs-stp,不能避免广播风暴
控制广播风暴使用[STP][12]协议，参考[FAQ][10]
即添加 --switch ovsbr,stp=1

进入 custom 目录后，运行 python 脚本
```shell
 sudo mn --custom topo-fat-tree.py --topo fattree --switch ovsbr,stp=1
```
结果如下
![](https://i.loli.net/2020/03/29/cCazhGvf2mI43x5.png)

##使用 iperf 测带宽
>参考在 Minimap 中使用 iperf 的[简介][14]
或 iperf [常用命令][13]

结果如下
![](https://i.loli.net/2020/03/29/wk34cXpZbx5sFdH.png)



##其它方法(未测试)
>[floodlight + Mininet][9]

安装参考[这里][4](推荐)、[这里][5]
运行与启动参考[这里][6]、[这里][7]或[官网][8]

[1]:https://www.linuxprobe.com/sdn-mininet.html
[2]:http://roan.logdown.com/posts/191753-sdn-lab-2-use-mininet-create-fat-tree-topology
[3]:https://github.com/howar31/MiniNet/blob/master/topo-fat-tree.py
[4]:https://henulwj.github.io/2016/11/08/floodlight-install-use/
[5]:https://www.cnblogs.com/luckyguy/p/4062313.html
[6]:https://blog.csdn.net/qq_15437629/article/details/45766583
[7]:https://www.sdnlab.com/2909.html
[8]:https://floodlight.atlassian.net/wiki/spaces/floodlightcontroller/pages/1343544/Installation+Guide
[9]:https://blog.csdn.net/alex1997222/article/details/88881286
[10]:https://github.com/mininet/mininet/wiki/FAQ#openflow-versions
[11]:https://www.ibm.com/developerworks/cn/cloud/library/1404_luojun_sdnmininet/
[12]:https://zh.wikipedia.org/wiki/%E7%94%9F%E6%88%90%E6%A0%91%E5%8D%8F%E8%AE%AE
[13]:https://man.linuxde.net/iperf
[14]:https://blog.csdn.net/Limexc/article/details/82899856
[16]:https://blog.csdn.net/ten_sory/article/details/79593554
