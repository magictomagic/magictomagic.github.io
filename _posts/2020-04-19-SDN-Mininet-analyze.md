# Mininet analyze
在[配置环境][1]后，执行
```shell
ipconfig -a
```
对比创建了 Mininet 后，在 mininet> 下，【打开一个新的终端(Ctrl + Alt + t)】**或**【在已有终端上打开另一个新的终端(Shift +Ctrl + n)】(用**或**是因为为两者结果一样)，执行同样的命令
![](/img/bj-4.png)![](/img/mininet-2.png)
发现主机里的网络设备多了 s1, s1-eth1, s1-eth2, 分别表示控制器，连接 h1, h2 的端口
对于 h1, h2 协议的分析，可以参考[简析 OpenFlow 协议][2]

[1]:https://magictomagic.github.io/2020/04/18/SDN-ex1/
[2]:https://magictomagic.github.io/2020/04/18/SDN-ex1/#jump
