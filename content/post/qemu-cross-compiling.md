---
title: "QEMU cross compiling"
date: 2021-07-01T01:37:56+08:00
lastmod: 2017-07-02T01:37:56+08:00
draft: false
tags: ["qemu", "cross compiling", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# Ubuntu 20 TLS

-   VMware 4核17G 100G磁盘
-   编译好后共用42G

# install

``` bash
git clone https://github.com/crosstool-ng/crosstool-ng

sudo apt-get install build-essential autoconf  ncurses-dev  # solved:  [[https://github.com/c9/core/issues/19][error: curses library not found]]

sudo apt install flex texinfo  help2man gawk libtool-bin autoconf autopoint bison

sudo ./bootstrap

./configure

sudo make

sudo make install

ct-ng -v
```

# 获取工具类型

``` bash
mkdir crosstool

cd crosstool/

ct-ng arm-unknown-linux-gnueabi
```

# 配置文件

参考[链接](https://www.programmersought.com/article/29085564038/)

ct-ng menuconfig

目录什么的自己设定位置，路径要自己记住

## 关键配置

goto [link](https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html) to find

-   Architecture level = -march `armv7`
-   Emit assembly for CPU = -mcpu `arm920t`
-   Tune for CPU = -mtune 不搞, 因为新版的会自动推导

![](/img/20210701_185909_7vxSfe.png)

编译: `ct-ng build`

![](/img/20210702_010025_G3WtLy.png)

测试: 到 prefix 代表的目录下

![](/img/20210702_012357_Any6LG.png)

运行

![](/img/20210702_012816_XJ8bs7.png)

# 参考

[flow](https://www.cnblogs.com/andyfly/p/10507600.html)
