---
title: "docker 笔记"
date: 2021-02-16T01:37:56+08:00
lastmod: 2021-02-16T01:37:56+08:00
draft: false
tags: ["docker"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# DOCKER 基础

## DOCKER 基本命令

```shell
docker images

	-q 显示ID

	-a 显示所有layer

	--digests 显示摘要

	--no-trunc 镜像信息 & 完整id 



docker search tomcat 在dockerhub上搜tomcat镜像

	-s 30 过滤留stars数超过30的

	--no-trunc 不省略说明

	--automated 只列出automated build类型的images



docker pull tomcat 下载 image tomcat 



docker rmi tomcat 删除 image tomcat, 默认latest



docker rmi -f $(docker images -qa) 请求传参，批量删除

	-f force remove



docker run -it [IMAGE ID]

	--name xxx 指定名字为xxx

		docker run -it --name hhyzuishuai centos  # no [IMAGE ID]

	-i 以交互模式运行容器

	-t 为容器重新分配一个伪输入终端

	-d daemon 后台运行容器 (docker容器后台运行，必须有一个前台进程，如果不是一直挂起的命令(top, tail等)，就会自动退出)

		解决方法e.g.：docker run -d centos /bin/sh -c "while true; do echo hello asdf; sleep 2; done"

			/bin/sh -c “xxx”  # 直接执行脚本

	-P 随机端口映射

	-p 指定端口映射

		ip:hostPort:containerPort

		ip::containerPort

		hostPort:containerPort

		containerPort

			退出：快捷键 ^pq 或 exit 



docker start [容器ID或容器名]

docker attach [容器ID或容器名]  # 进入container终端

docker restart [容器ID或容器名]

docker stop  [容器ID或容器名]

docker kill  [容器ID或容器名]   # 强制停止

docker history [image ID]  # 看花卷

docker ps 默认显示当前正在运行的process

	-a, --all       Show all containers (default shows just running)

 -f, --filter filter  Filter output based on conditions provided

   --format string  Pretty-print containers using a Go template

 -n, --last int    Show n last created containers (includes all states) (default -1)

 -l, --latest     Show the latest created container (includes all states)

   --no-trunc    Don't truncate output

 -q, --quiet      Only display container IDs

 -s, --size      Display total file sizes



docker rm  [CONTAINER ID]  # 删除容器 

	批量删除容器

 		docker rm -f $(docker ps -a -q)

		docker ps -a -q | xargs docker rm  # xargs, linux 命令，接受传给的东东

			e.g. 	echo 'one two three' | xargs mkdir

					ls

					one two three



docker logs [container ID]  # 输出对应container的console logs  

	-t 显示时间

	-f 追加不停

	--tail 3 只看倒数3行



docker exec -t [dockerID] ls -l /tmp  # 隔山打牛

	若exec变attach，则要进入山
	
docker exec -it [dockerID] /bin/bash  # 进入docker cli


docker commit 使容器变为镜像

 -m="提交的描述信息“ -a="作者” [CONTAINER ID] [IMAGE ID]:[TAG]

e.g.:	sudo docker commit -m="my message" -a="magic1" caa723d2aac6 mytomcat:1.1



docker cp 容器ID:容器内路径 目的主机路径


docker inspect [image ID] 查看docker信息
```

## 容器数据卷

主机-容器-容器   持久化 & 数据共享

```shell
docker run -it /host绝对路径目录:/容器内目录[:ro 可选，只读] 镜像名
docker run -it -v /home/magic/Desktop/myDataVolume:/dataVolumeContainer 300e315adb2f
```

命令行的方式宿主机目录路径不一定存在，所以改Dockerfile来创数据卷（相当于填几个移动硬盘）

Dockerfile

```txt
FROM centos
VOLUME ["/dataContainer1", "/dataContainer2"]
CMD echo "finish, --------success"
CMD /bin/bash
```

```shell
docker build -f /home/magic/Desktop/Dockerfile  -t hhycentos .  # 若不写 -f，则默认读当前工作目录下的Dockerfile
```

`inspect`一下，路径种有一串很长的 [containe ID]  **注意，是正在运行的容器哦**

```shell
sudo docker run -it --name dc01 hhycentos
sudo docker run -it --name dc02 --volumes-from dc01 hhycentos  # 容器的继承
sudo docker run -it --name dc03 --volumes-from dc01 hhycentos
# 数据全部共享，删哪个都没事
```

### mysql

```shell
docker run -p 33060:3306 --name mysql -v /home/magic/Desktop/conf:/etc/mysql/conf.d -v /home/magic/Desktop/logs:/logs -v /home/magic/Desktop/data:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=123456 -d mysql

docker exec -it d27b1c7128a4 /bin/bash 
```

之后配置mysql文件，开启端口，可远程连接。

### redis

```shell
docker run -p 6379:6379 -v /yourpath/data:/data -v /yourpath/redis.conf:/usr/local/etc/redis/redis.conf -d redis:3.2 redis-server /usr/local/etc/redis/redis.conf --appendonly yes
```

