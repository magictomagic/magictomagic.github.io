# 固件解密

## 茴有四种写法

### 茴
用 https://github.com/fkie-cad/fact_extractor/tree/master 中 docker 的方式，注意**文件目录结构**要按照它要求的来创建，不然它会报错文件路径找不到


#### 文件目录结构

```txt
<path_to_shared_folder>
├── files
├── input
│   └── firmware_file
└── reports
```

#### 执行命令

```shell
sudo docker run -v /home/magic/Desktop/shared_folder/:/tmp/extractor -v /dev:/dev --privileged --rm fkiecad/fact_extractor
```

解秘后的文件在files中

### 回 
用 https://github.com/0xricksanchez/dlink-decrypt 中的脚本，先要`easy_install pycrypto`一下，否则会[报错][1]，

#### 运行
```shell
python3 ./dlink-dec.py -i <in> -o <out>
```

## 结果

### 解密前

![image-20210330045044436](../img/image-20210330045044436.png)

### 解密后

![image-20210330044929812](../img/image-20210330044929812.png)

两个工具解密以后得到的结果一样，说明解密部分`99.9999%`成功了

## 经验

docker 的 image create container 后，container 里面可能会有自动检测程序，如果没有**按照规定的方法指定容器数据卷或其它外部传参命令**，改container会自动退出。

# qemu 模拟运行

## 获取 rootfs folder

参考[博客][2]

![image-20210330050231154](../img/image-20210330050231154.png)

```shell
# loop
binwalk -e xxx  # xxx is the file to extract
# wait sync
cd yourfile.extracted/
# ls and find xxx xxx.7z
# goto: loop
# 到最后，若发现 cpio-root，则
tar zcf cpio-root.tar.gz cpio-root
```

记得看一下

![image-20210330052041723](../img/image-20210330052041723.png)

如果是小端的，就要将`cpio-root.tar.gz`发送到**QEMU MIPSEL**中的 **rootfs folder** 

## qemu 运行

### 常识

```txt
mips 是32位大端字节序 
mipsel 是32位小端字节序 
mips64el 是64位小端字节序 

Initrd ramdisk或者initrd是指一个临时文件系统，它在启动阶段被Linux内核调用。initrd主要用于当“根”文件系统被挂载之前，进行准备工作
```

### 放弃的方向(坑)

以 Windows 为 host，用 qemu manager, 参考 https://people.debian.org/~aurel32/qemu/mipsel/README.txt 中 `qemu-system-mips64el -M malta -kernel vmlinux-3.2.0-4-5kc-malta -hda debian_wheezy_mipsel_standard.qcow2 -append "root=/dev/sda1 console=tty0"` 配置GUI

![image-20210330063846226](../img/image-20210330063846226.png)

然后，无论我把RAM调大还是调小，都会报错，妈的自相矛盾的bug。

![image-20210330064326406](../img/image-20210330064326406.png)

然后又试了`qemu-system-mipsel -M malta -kernel vmlinux-2.6.32-5-4kc-malta -hda debian_squeeze_mipsel_standard.qcow2 -append "root=/dev/sda1 console=tty0"`，还是老问题。

### 回归 Linux

在 Ubuntu20.04TLS 下到 https://people.debian.org/~aurel32/qemu/mipsel/ 中下载

![image-20210330065303212](../img/image-20210330065303212.png)

然后参考`README.txt`运行，最后：

![image-20210330065445835](../img/image-20210330065445835.png)

## qemu 网络配置

参考[1][5]

### 常识

#### Tun/Tap

参考[1][3],[2][4]

TAP等同于一个以太网设备，它操作第二层数据包如以太网数据帧。

TUN模拟了网络层设备，操作第三层数据包比如IP数据封包。

![img](https://zhaohuabing.com/img/2020-02-24-linux-taptun/linux-tuntap.png)

tun/tap设备的用处是将协议栈中的部分数据包转发给用户空间的应用程序

```txt
+----------------------------------------------------------------+
|                                                                |
|  +--------------------+      +--------------------+            |
|  | User Application A |      | User Application B |<-----+     |
|  +--------------------+      +--------------------+      |     |
|               | 1                    | 5                 |     |
|...............|......................|...................|.....|
|               ↓                      ↓                   |     |
|         +----------+           +----------+              |     |
|         | socket A |           | socket B |              |     |
|         +----------+           +----------+              |     |
|                 | 2               | 6                    |     |
|.................|.................|......................|.....|
|                 ↓                 ↓                      |     |
|             +------------------------+                 4 |     |
|             | Newwork Protocol Stack |                   |     |
|             +------------------------+                   |     |
|                | 7                 | 3                   |     |
|................|...................|.....................|.....|
|                ↓                   ↓                     |     |
|        +----------------+    +----------------+          |     |
|        |      eth0      |    |      tun0      |          |     |
|        +----------------+    +----------------+          |     |
|    10.32.0.11  |                   |   192.168.3.11      |     |
|                | 8                 +---------------------+     |
|                |                                               |
+----------------|-----------------------------------------------+
                 ↓
         Physical Network
```

eth0的另一端是物理网络，这个物理网络可能就是一个交换机，而tun0的另一端是一个用户层的程序，协议栈发给tun0的数据包能被这个应用程序读取到，并且应用程序能直接向tun0写数据。

#### [用户和用户组][6]

**什么是用户组**

用户组是具有相同特征用户的逻辑集合，简单来说，就是具有相同权限的用户的集合。 

例如：
人事部有20名员工，他们都需要访问一个文件夹，如果我们给这20个用户的账号分别设置权限，这样太麻烦了，所以我们会建立一个用户组叫HR，对这个组设置权限，将这20个用户加入这个组就可以了。

**用户和用户组的关系**

**A** 一个用户可以属于一个用户组，具有此用户组的权限。

HR组可以访问/hrfile的文件夹，当user01属于HR组，那么user01就可以访问/hrfile这个文件夹

**B** 一个用户可以属于多个用户组，此时具有多个组的共同权限

HR可以访问/hrfile的文件夹，运维可以访问/yunweifile的文件夹，当user01同时属于HR组和运维组，那么user01可以访问 /hrfile和/yunweifile

**C** 多个用户可以属于一个用户组，多个用户都具有此用户组的权限。

**主组**：指用户创建时默认所属的组，**每个用户的主组只能有一个**。创建用户时会同时创建一个和用户名相同的组。

例如：添加用户xiaoming，在建立用户 xiaoming 的同时，就会建立 xiaoming 组作为 xiaoming 用户的初始组。

**附加组**：每个用户只能有一个主组，除主组外，用户再加入其他的用户组，这些用户组就是这个用户的附加组。**每个用户的附加组可以有多个**，而且用户可以有这些附加组的权限。

#### 用户组命令

```shell
groups  # 查看系统当前登录用户的所属组
groups magic  # 查询特定用户的用户组
newgrp 用户组名称 # 切换用户组
```

#### qemu 命令行配置网络

##### 老版本

按照这个[博客][https://shadow-file.blogspot.com/2013/05/running-debian-mips-linux-in-qemu.html]中配置好macaddr和tap

```shell
qemu-system-mipsel 
-net nic,macaddr="52:54:00:12:34:XX" 
-net tap,ifname=[tap0 | tap1],script=no,downscript=no 
-nographic 
############################################################################################################
-M malta -kernel vmlinux-2.6.32-5-4kc-malta -hda debian_squeeze_mipsel_standard.qcow2 -append "root=/dev/sda1 console=tty0"
```

##### 20年代版本

整合到后面的**操作**>**配置网卡**中了

### 操作

#### 安装依赖

```shell
 sudo apt-get install uml-utilities bridge-utils
```

#### 配置用户组

[You need to add a group that will own the tap that will be used to bridge each QEMU system to Ubuntu's network.  Then add your user to that group.][5]

```shell
sudo groupadd -r tuntap
sudo usermod -a -G tuntap magic
newgrp tuntap
```

#### 配置网卡

参考这个[博客][https://gist.github.com/extremecoders-re/e8fd8a67a515fee0c873dcafc81d811c]

```shell
sudo brctl addbr br0
ifconfig
sudo ip addr flush dev ens33  # ens33 用 ifconfig 看到的你自己的
sudo brctl addif br0 ens33
sudo tunctl -t tap0 -u magic  # magic 是你的用户名
# Set 'tap0' persistent and owned by uid 1000
sudo brctl addif br0 tap0
sudo ifconfig ens33 up
sudo ifconfig tap0 up
sudo ifconfig br0 up
brctl show
# bridge name	bridge id		STP enabled	interfaces
# br0		8000.000c29e8ddf9	no		ens33
# 							tap0
sudo dhclient -v br0
# Internet Systems Consortium DHCP Client 4.4.1
# Copyright 2004-2018 Internet Systems Consortium.
# All rights reserved.
# For info, please visit https://www.isc.org/software/dhcp/
# 
# Listening on LPF/br0/00:0c:29:e8:dd:f9
# Sending on   LPF/br0/00:0c:29:e8:dd:f9
# Sending on   Socket/fallback
# DHCPDISCOVER on br0 to 255.255.255.255 port 67 interval 3 (xid=0x51b2ed4f)
# DHCPOFFER of 192.168.239.216 from 192.168.239.254
# DHCPREQUEST for 192.168.239.216 on br0 to 255.255.255.255 port 67 (xid=0x4fedb251)
# DHCPACK of 192.168.239.216 from 192.168.239.254 (xid=0x51b2ed4f)
# bound to 192.168.239.216 -- renewal in 767 seconds.

```

### 启动qemu

下面命令启动后17GB内存消耗完且无法`Ctrl + c`中断

```shell
sudo qemu-system-mipsel -net nic,macaddr="00:0c:29:e8:dd:f9" -net tap,ifname=tap0,script=no,downscript=no -nographic -M malta -kernel vmlinux-3.2.0-4-5kc-malta -hda debian_wheezy_mipsel_standard.qcow2 -append "root=/dev/sda1 console=tty0"


```

下面命令启动后出现虚拟机但一直在加载, 可以中断

```shell
sudo qemu-system-mipsel  -netdev tap,id=mynet0,ifname=tap0,script=no,downscript=no  -device e1000,netdev=mynet0,mac=52:55:00:d1:55:01 -M malta -kernel vmlinux-3.2.0-4-5kc-malta -hda debian_wheezy_mipsel_standard.qcow2

sudo qemu-system-mipsel  -netdev tap,id=mynet0,ifname=tap0,script=no,downscript=no  -device e1000,netdev=mynet0,mac=00:0c:29:e8:dd:f9 -M malta -kernel vmlinux-3.2.0-4-5kc-malta -hda debian_wheezy_mipsel_standard.qcow2

sudo qemu-system-mipsel  -netdev tap,id=mynet0,ifname=tap0,script=no,downscript=no  -device e1000,netdev=mynet0,mac=12:05:68:7f:3f:0b -M malta -kernel vmlinux-3.2.0-4-5kc-malta -hda debian_wheezy_mipsel_standard.qcow2

sudo qemu-system-mipsel -net nic,macaddr="00:0c:29:e8:dd:f9" -net tap,ifname=tap0,script=no,downscript=no -M malta -kernel vmlinux-3.2.0-4-5kc-malta -hda debian_wheezy_mipsel_standard.qcow2 -append "root=/dev/sda1 console=tty0"

# 原先的命令也不行了,估计是虚拟机崩了
qemu-system-mips64el -M malta -kernel vmlinux-3.2.0-4-5kc-malta -hda debian_wheezy_mipsel_standard.qcow2 -append "root=/dev/sda1 console=tty0"
```

回滚虚拟机, 还不就是从头再来

原先的命令又行了, 但上面那几条命令还是执行了以后好长时间虚拟机都只是单纯消耗内存.

最后押注

```shell
sudo qemu-system-mipsel -net nic,macaddr="00:0c:29:e8:dd:f9" -net tap,ifname=tap0,script=no,downscript=no -M malta -kernel vmlinux-3.2.0-4-5kc-malta -hda debian_wheezy_mipsel_standard.qcow2 -append "root=/dev/sda1 console=tty0"
```

如果还是不行, 就不用`uml-utilities bridge-utils`这两个工具了.

之后的思路是

+ 放弃桥接的意图,直接用wget等方式下载
+ 参考[博客](https://www.cyberciti.biz/faq/ubuntu-20-04-add-network-bridge-br0-with-nmcli-command/?__cf_chl_jschl_tk__=22c400bef32b6772a555bf5ff9ffa5ff9e04c1d1-1617131812-0-AQKv2LUa6xOkuTsG8IUoUm8y7QIDb6SZco__lxEYc65eM7ZHODs5fTYbj3FC-TKTUHBmLmj3NRP6TDdH6Qk3BoN37dtMW8Lbqe_gsVZsf45Izkm9bWv28LZKKkBDlh_IzLAurXIcPXfMchIiW7cxZ-4yVeaL_TBm4YkPh66-nn96RbQtNj99NS3pAskPsC2MS31CpX7VQRFQO3w5dMYIYmk92lnd0hqx0X6PXRqriFVG-G5CmPFOFS-pXdJnMJrCuJmTqGEsb9pnH6F2FWdG98gJxSEBiN4kSH8tEZsfcER8ib-mWdL6QC7GYmXnSKk6R6KCN9Odb-BpuWRqXtU-L-t-kPDZ6URgUQ1f51ICuSGWqjFuxeziAsWK8DB7GvG3p3RRQSUkfD74oFdNwEDsTKOuoQU1y-z600eDuqhho_nx5MJHR-_L8DvkxvuNJME2e6Msha2iIOHn5tcY-tZCrCrAmCYg829UgPAaE1fDdx1kflrC7V8_utjdVH2wI3ddpgLF76X-LI7gJvT6jVVCtjQ)用nmcli配置桥接









[1]: https://stackoverflow.com/questions/19623267/importerror-no-module-named-crypto-cipher
[2]: https://tsublogs.wordpress.com/2019/12/16/emulate-d-link-dcs-932l-camera-using-qemu/
[3]: https://zhaohuabing.com/post/2020-02-24-linux-taptun/
[4]: https://segmentfault.com/a/1190000009249039
[5]: https://shadow-file.blogspot.com/2013/05/running-debian-mips-linux-in-qemu.html
[6]: https://zhuanlan.zhihu.com/p/113916659