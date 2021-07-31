---
title: "Simple Chat Program"
date: 2020-05-23T01:37:56+08:00
lastmod: 2020-05-24T01:37:56+08:00
draft: false
tags: ["Linux", "C/C++", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

## setup ftp server on centos 7
### useful commands

参考：

[root 登录][1]

[install vsftpd][2]

[离线下载/上传][3]

[思路参考][5]

[libcurl man][4]

[give up NAT & 语音][6]

[以后用安卓][7]

service vsftpd restart  # 启动ftp.

cd /etc/vsftpd

in ftpusers & user_list
注释掉root

iptables -I INPUT -p tcp --dport 21 -j ACCEPT

配置selinux

getsebool -a | grepftp 

setsebool -P ftpd_full_access on 

in vsftpd.conf
chroot_local_user=YES   # 是否限定用户在其主目录下（NO 表示允许切换到上级目录）



service httpd start 	# 启动 apache

# 允许 ftp 服务

irewall-cmd --permanent --zone=public --add-service=ftp

# 重新载入配置

firewall-cmd --reload

firewall-cmd --list-ports    # check

firewall-cmd --zone=public --add-port=21/tcp --permanent

systemctl status firewalld

# 在 CentOS 上安装 -lcurl(libcurl) 库

go [官网][8] and download the newest curl-x.xx.x.tar.bz2 by IDM with proxy, extract it in windows and use ssh protocol

```bat
scp C:\Users\magic\Desktop\server4.c root@47.94.157.136:/root/try1/
```

to transfer it to my VPS, go into the directory and make, then make install. 

# 其它实现过程、测试结果

见[ Word 文档][11]

# 代码

[server][12]
[client][13]






[1]: https://my.oschina.net/jack088/blog/734772
[2]: https://www.jianshu.com/p/05dc6455b513
[3]: https://www.cnblogs.com/zlcxbb/p/6006861.html
[4]: https://curl.haxx.se/libcurl/c/curl_easy_setopt.html
[5]: https://www.cnblogs.com/chang290/archive/2012/08/12/2634858.html
[6]: https://github.com/str2num/libice
[7]: https://github.com/anyRTC/anyRTC-RTMP-OpenSource
[8]:https://curl.haxx.se/download.html
[9]: ../files/simplechat/client4.c

[10]:  ../files/simplechat/server4.c
[11]:../files/simplechat/exp8.docx

[12]:../files/simplechat/server.c
[13]:../files/simplechat/client.c