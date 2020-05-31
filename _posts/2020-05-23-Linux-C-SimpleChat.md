## setup ftp server on centos 7
### useful commands

参考：[root 登录][1]
[install vsftpd][2]
[离线下载/上传][3]、[思路参考][5]
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

# 允许 ftp 服务

irewall-cmd --permanent --zone=public --add-service=ftp

# 重新载入配置

firewall-cmd --reload

firewall-cmd --list-ports    # check

firewall-cmd --zone=public --add-port=21/tcp --permanent

systemctl status firewalld


[1]: https://my.oschina.net/jack088/blog/734772
[2]: https://www.jianshu.com/p/05dc6455b513
[3]: https://www.cnblogs.com/zlcxbb/p/6006861.html
[4]: https://curl.haxx.se/libcurl/c/curl_easy_setopt.html
[5]: https://www.cnblogs.com/chang290/archive/2012/08/12/2634858.html
[6]: https://github.com/str2num/libice
[7]: https://github.com/anyRTC/anyRTC-RTMP-OpenSource
