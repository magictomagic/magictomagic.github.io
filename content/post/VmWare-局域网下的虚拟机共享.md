---
title: "局域网下共享VmWare虚拟机"
date: 2020-06-30T01:37:56+08:00
lastmod: 2020-06-30T01:37:56+08:00
draft: false
tags: ["VmWare"]
categories: ["just for fun"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

## 虚拟机的拥有者（服务器）

![](/img/Snipaste_2020-06-30_15-46-52.png)
![](/img/Snipaste_2020-06-30_15-47-39.png)

去防火墙的入站规则选择【公用网络】【TCP】，端口填【80, 443,500,902,3091-3094,3099,3101,3389,4001,4002, 4100,4172, 4500,5443,8472, 9427,8009, 18443,22389】。因为没怎么理解[官网]][1]这些端口怎么搞，就把它们都填了。另外UDP也要设置，再新建一个规则，按照[官网][1]最后的端口填写即可。

shared的虚拟机存在一些固有的问题
![](/img/Snipaste_2020-06-30_16-12-01.png)

## 配置连接服务器的机器
![](/img/Snipaste_2020-06-30_15-54-55.png)
ip地址通过服务器的 ipconfig/all 查看
![](/img/Snipaste_2020-06-30_15-59-23.png)

[1]:https://kb.vmware.com/s/article/1027217?lang=zh_CN
