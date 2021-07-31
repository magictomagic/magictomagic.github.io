---
title: "Deploy Django on VPS"
date: 2020-05-01T01:37:56+08:00
lastmod: 2020-05-01T01:37:56+08:00
draft: false
tags: ["Django"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# 在 CentOS 8 上部署基于 Django 和 Nginx 的用户注册登录系统
## 初識 Django
### 學習步驟
  参考[教程][1]，搭建本地 Django server
  使用 git 管理 Django，使之可衝用於服務器。
#### [基础知识][2]
##### Linux 基本命令
```shell
pwd     # display current path
whereis [file_name] #  [file_name] means file you want to find
```
##### CentOS 8 防火牆的管理
在測試 Django 是否向外網提供服務時，要打開服務運行的端口。
```shell
firewall-cmd --zone=public --add-port=8000/tcp --permanent
```
[安裝 nginx][5] 后，若要從外網通過 http 服務進入主機，需要在防火牆協議里添加 http 協議，然後 reload 使之 apply。
```shell
firewall-cmd --permanent --add-service=http # permanently enable HTTP connections on port 80
firewall-cmd --permanent --list-all         # verify that the http firewall service was added correctly
firewall-cmd --reload      # apply the changes
```
如果不加 --permanent，systemctl restart firewalld 后之前的配置會消失。

## 於 VPS 上重新構建之前本地的 Django 項目
### [Setting up a new environment][3]
使用venv创建一个新的虚拟环境，确保我们的包配置与我们正在进行的任何其他项目保持良好的隔离。
```shell
python3 -m venv ytbkiller  # ytbkiller is my virtual environment directory name
source ytbkiller/bin/activate  # activate virtual environment
## all of your option should have (ytbkiller) before [root@vultrgues ... e.g.
# (ytbkiller) [root@vultrguest ~]#
```
从现在起，任何你使用pip安装的包将会放在 ytbkiller 文件夹中，与全局安装的 Python 隔绝开。
### [Getting started][4]
文件的目录结构如下，其中灰色的是 .gitignore 中列出的项，clone 后是找不到这些文件的，因为这些文件包含隐私数据。

![](/img/20200501003050.png)

```shell
pip install --upgrade pip
git clone https://github.com/magictomagic/login-register
###
# 省略 cd 文件夹的操作，文件位置见上图
###
pip install -r requirements.txt # 使用pip安装第三方依赖
vi settings.example.py  # 编辑 settings.example.py 的邮箱、密码於密鑰，可以自己创建一个，组员见那个压缩包
mv settings.example.py settings.py # rename
python manage.py makemigrations # 在该app下建立 migrations 目录，并记录下你所有的关于modes.py的改动，比如0001_initial.py， 但是这个改动还没有作用到数据库文件
python manage.py migrate # 将该改动作用到数据库文件，比如产生table之类
python manage.py runserver # 启动项目，僅本機可訪問
python manage.py runserver 0.0.0.0:8000 # 外網可訪問，注意配置防火牆，開放端口
```

## uWSGI 的安裝、配置和啓動
>環境：CentOS 8

### 安裝
先參考[官方教程][7]安裝 Anaconda，注意隨着 Anaconda 的版本是不斷更新的([更新目錄][8])，所以不能照搬我下面的命令
```shell
wget -P /tmp https://repo.anaconda.com/archive/Anaconda3-2020.02-Linux-x86_64.sh
bash Anaconda3-2020.02-Linux-x86_64.sh
# 遇到 [yes|no] 全部 yes，其它情況，全部 enter，詳情見上面的官網鏈接
```
再使用 conda 安裝 uWSGI，[uWSGI][9]在這裏充當連接 Nginx 和 Django 的橋樑。
```shell
conda config --add channels conda-forge
conda install uwsgi
```
### 配置

### 啓動
可能會遇到報錯：
**uwsgi: error while loading shared libraries: libiconv.so.2: cannot open shared object file: no such file or directory**
安裝依賴
```shell
conda install -c conda-forge libiconv
```
然後启动 uWSGI
```shell
uwsgi uwsgi_config.ini
```

## Nginx 的安裝、配置和管理
### 安裝
Centos 8 的包管理工具推薦使用 [dnf][10]，不過用 yum 也可以。
```shell
yum install nginx
```
### 配置
/etc/nginx/conf.d

### 管理
```shell
systemctl enable nginx   # 开机自启
systemctl start nginx    # 启动 Nginx
systemctl stop nginx     # 关闭 Nginx
systemctl restart nginx  # 重启 Nginx
```

已解决:单独使用 Django 访问
http://149.248.57.125:8000/
admin/
index/
login/
register/
logout/
confirm/

待解决：通过 uWSGI 连接 Django 和 Nginx
-------------------------------------------------------------------------
## supervisor 的掃盲、安裝、配置和管理
### 掃盲
#### [What][15]
Linux/Unix 进程监控工具。
#### Why
统一管理各进程的运行，避免编写大量实现进程功能的脚本。
### 安裝
```shell
pip install supervisor

vi /etc/supervisord.conf
```

### 配置


### 管理



## 常用管理命令匯總



![](/img/Snipaste_2020-05-01_17-59-41.png)


[1]:https://www.liujiangblog.com/course/django/2
[2]:https://www.liujiangblog.com/blog/36/
[3]:https://www.django-rest-framework.org/tutorial/1-serialization/#setting-up-a-new-environment
[4]:https://www.django-rest-framework.org/tutorial/1-serialization/#getting-started
[5]:https://www.digitalocean.com/community/tutorials/how-to-install-nginx-on-centos-8
[6]:https://www.thegeekdiary.com/5-useful-examples-of-firewall-cmd-command/
[7]:https://linuxize.com/post/how-to-install-anaconda-on-centos-8/
[8]:https://repo.anaconda.com/archive/
[9]:https://github.com/conda-forge/uwsgi-feedstock
[10]:https://www.howtoforge.com/centos-8-package-management-with-dnf-on-the-command-line/


[13]:https://www.digitalocean.com/community/tutorials/how-to-serve-django-applications-with-uwsgi-and-nginx-on-centos-7

[15]:http://supervisord.org/introduction.html
