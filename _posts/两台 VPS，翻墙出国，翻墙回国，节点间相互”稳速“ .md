# 两台 VPS，翻墙出国，翻墙回国，节点间相互”稳速“ 
_为了积极响应d的意识形态建设，这里单用一个V字指代V吐蕊。由于我用的VPS供应商是 vultr ,所以对于 out of wall 的 VPS ,都用 vultr 代替，[vultr] 指外面的那台主机的 IP 地址，同理，对于里面的主机的IP，我用 [腾讯] _

***下面所有的 VPS 用的都是 CentOS 7 哦！***

**基础配置**
		原理如下图。基于 [ws + tls + v][2] 的【伪装】方式，配上另一面的VPS中转，达到比较稳定的速度，当然，还可以用上 BBR PLUS 来提速。
			![file](https://graph.baidu.com/resource/2228160e76a0c263a005901578820046.png)
		首先，你要买一个域名，假设是 abc.com ，因为在两个服务器上都要配置【伪装】，为了省钱，可以把域名解析成二级域名，比如 as.abc.com , qw.abc.com 。然后，先在国外服务器上用来自 [atrandys][1] 的一键脚本

```shell
curl -O https://raw.githubusercontent.com/atrandys/v2ray-ws-tls/master/v2ray_ws_tls.sh && chmod +x v2ray_ws_tls.sh && ./v2ray_ws_tls.sh
```

安装基本的 ws + tls + v 配置，[下载][3] V 的客户端（仅含 win & adb）（密码：uu5gxw 压缩包密码：hhyzuishuai）,看能不能连上。![file](https://graph.baidu.com/resource/22220dba77e96e435d04001578822987.png)
		开启 PAC 模式或全局模式，可以看到原来蓝色的图标变为紫色或红色
![file](https://graph.baidu.com/resource/22225dd70ca4f1489b45901578823349.png)
		测试一下，如果 google 能用，则可以进行下面的操作了。

**安装 BBR PLUS 加速**
		执行 [BBR PLUS 修正版脚本][4]

```shell
wget -N --no-check-certificate "https://raw.githubusercontent.com/chiakge/Linux-NetSpeed/master/tcp.sh" && chmod +x tcp.sh && ./tcp.sh
```
​		可以先选 0 升级脚本。输入 
```shell
./tcp.sh
```
​		再次运行后，选择 2 安装 BBRplus 版内核，待下载与编译内核完成后，输入 Y ，重启。
​		再次运行该脚本，选择 7 使用BBRplus版加速。
​		可以选择 10，优化一下。
​		有时候想看看自己有没有使用BBR，可以用命令

```shell
sysctl net.ipv4.tcp_congestion_control
```
或
```shell
lsmod | grep bbr
```
查看。



**使用 Socat 转发 TCP 和 UDP 流量**

​		脚本默认开启UDP、TCP转发，带开机自启功能，且一次只能转发单个端口，如果想转发多个端口请重复运行本脚本。_转自[这里][5]_

输入

```shell
wget https://www.moerats.com/usr/shell/socat.sh && bash socat.sh
```
运行后填写参数

```shell
#如果你要用腾讯的1984端口转发IP为[vultr]服务器的443端口，那就依次填入指定参数。
请输入本地端口:1984
请输入远程端口:443
请输入远程IP:[vultr]		# 不能填域名
```

对于选择什么端口有疑虑的，可以参考[端口列表][6]



**客户端连接国内 VPS **

​		将解析到 vultr 的域名改为 [腾讯]，端口改为 1984 ，其它不变。可以访问谷歌测试一下。



**镜像操作**

​		在腾讯的 VPS 上也执行【**基础配置**】中的操作。注意，由于是在国内，除了最开始的 connection refused 脚本自动终止外，之后脚本没有一个若下载的文件不存在就一直请求去下载或直接停止的机制，所以下载的资源可能不齐全，可以对照报错与那个脚本的代码，查看少了什么，一个个装上，缺得东西太多的话可以重装系统，再执行一遍脚本。对于 V 的 failed ，很大程度上是配置文件的语法不对，可以用[在线json语法检查](https://www.bejson.com/)。参考第一张图对称地操作一遍，记得快照备份。

​		

***反馈与排错：***

1. 输入

```shell
systemctl status v2ray
```

显示 success，

2. 浏览器访问解析到 [腾讯] 的域名，能显示出一个正常的网页，且左边的小锁显示是安全的。

   _注意：访问前清空浏览器缓存，或换一个浏览器，否则访问的可能是之前“记忆”的网页，不是真正访问到了你的服务器。_

3. 用【**基础配置**】中连接 vultr 的方法配置客户端，开全局，访问百度等国内网站，若可以访问到，则成功。_不放心的可以拿 Wireshark 去抓一下包_

4. 配置好BBR，然后在 vultr 上用 Socat 转发好后，可以叫身在海外的友人去试一下。

   给他 vultr 的 IP 与 解析到 [腾讯] 的域名，访问 https://v.qq.com/x/cover/z8njmlc1opwgxoa/h0020vlrno3.html?ptag=10524 ，如果在未开启客户端之前显示由于版权不能播放，开了客户端以后可以正常播放，则成功。






[1]:https://www.atrandys.com/2019/1579.html
[2]:https://github.com/magictomagic/magictomagic.github.io/blob/master/files/%E5%9F%BA%E4%BA%8Ehttpstls%E5%8C%85%E8%A3%85%E8%BF%9B%E8%A1%8C%E6%B5%81%E9%87%8F%E4%BC%AA%E8%A3%85%E7%AA%81%E7%A0%B4%E7%BD%91%E7%BB%9C%E5%B0%81%E9%94%81%E7%9A%84%E6%8E%A2%E7%B4%A2%E4%B8%8E%E7%A0%94%E7%A9%B6.doc
[3]:https://share.weiyun.com/5isDHDM
[4]:https://www.4spaces.org/centos7-install-bbr-plus-2019/
[5]:https://www.moerats.com/archives/621/
[6]:https://zh.wikipedia.org/wiki/TCP/UDP%E7%AB%AF%E5%8F%A3%E5%88%97%E8%A1%A8(https://zh.wikipedia.org/wiki/TCP/UDP端口列表)