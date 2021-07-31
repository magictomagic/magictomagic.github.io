---
title: "Django Nginx uWSGI login-register"
date: 2020-05-03T01:37:56+08:00
lastmod: 2020-05-03T01:37:56+08:00
draft: false
tags: ["Django", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---
uWSGI
测试时
```ini
[uwsgi]
# Django-related settings
# socket=149.248.57.125:8000
http=0.0.0.0:8000
# socket=127.0.0.1:8000
chdir=/root/login-register/mysite/\
home=/root/ytbkiller
wsgi-file=mysite/wsgi.py
processes=4
threads=2
master=True
pidfile=uwsgi.pid
daemonize=uswgi.log
buffer-size=32768

```


格式，顺序？
```ini
[uwsgi]
# Django-related settings
# socket=149.248.57.125
# 指定IP端口，web访问入口
http=0.0.0.0:9000

socket=127.0.0.1:8000
chdir=/root/login-register/mysite/
home=/root/ytbkiller
wsgi-file=mysite/wsgi.py
processes=8
workers=5
threads=2
master=True
pidfile=uwsgi.pid
daemonize=uswgi.log
buffer-size=32768
```

```shell
uwsgi --ini uwsgi_config.ini             # 启动
uwsgi --reload uwsgi.pid          # 重启
uwsgi --stop uwsgi.pid            # 关闭
```

Nginx
vi /etc/nginx/conf.d/default.conf
```conf
server {
  listen 9000;
  server_name 149.248.57.125; #公网地址
  location / {
  include uwsgi_params;
  uwsgi_pass 127.0.0.1:8000; # 指向uwsgi 所应用的内部地址,所有请求将转发给 uwsgi 处理
  uwsgi_param UWSGI_PYHOME /root/ytbkiller; # 指向虚拟环境目录
  uwsgi_param UWSGI_CHDIR /root/login-register/mysite; # 指向网站根目录
  uwsgi_param UWSGI_SCRIPT manage:app; # 指定启动程序
  uwsgi_read_timeout 100;
 }
  location /static {  #转发对静态文件的请求
  root /root/login-register/mysite/login/static;  #必须填 static 文件夹所在目录
}
}
```

```conf
 server {
        listen       80 default_server;
        listen       [::]:80 default_server;
        server_name  _;
        root         /usr/share/nginx/html;

        # Load configuration files for the default server block.
        include /etc/nginx/default.d/*.conf;

        location / {
        }

        error_page 404 /404.html;
            location = /40x.html {
        }

        error_page 500 502 503 504 /50x.html;
            location = /50x.html {
        }
    }

```






supervisor
```conf
[program:mysite]
# 启动命令入口
command=/root/anaconda3/bin/uwsgi /root/login-register/mysite/uwsgi_config.ini
# 命令程序所在目录
directory=/root/login-register/mysite
user=root
autostart=true
autorestart=true
startsecs=10
startretries=10
#日志地址
stdout_logfile=/root/login-register/mysite/uwsgi_supervisor.log
stdout_logfile_maxbytes = 50MB
stderr_logfile=/root/login-register/mysite/uwsgi_err.log
stderr_logfile_maxbytes = 50MB
```





### 常用管理命令匯總
```shell
uwsgi --ini uwsgi_config.ini
systemctl status nginx
python manage.py runserver
source ytbkiller/bin/activate
python manage.py runserver 0.0.0.0:8000
systemctl restart nginx

```
