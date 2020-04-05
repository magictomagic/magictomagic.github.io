# 基于 github 的 git 基本操作与图床搭建
## windows10 家庭版疑难问题汇总
#### 账户登陆
>推荐使用 https 协议传输
>>原因： https 较 ssh 速度更快，且特殊时期更不易受到 GFW 的干扰

命令行输入用户名和邮箱地址
```shell
git config --global user.name <username>
git config --global user.email <username@example.com>
```
执行 push 等涉及变动远程库的操作会弹出输入框，要求输入用户名和密码。
e.g.
```shell
git push <url>  # url is like: https://github.com/username
```
之后，如下命令实现免登录长期存储密码
```shell
git config --global credential.helper store
```
#### 删除无法通过修改文件权限来删除的文件
把获取超级管理员权限添加到右键菜单：将下面脚本保存为后缀名为 .reg 的注册表文件，右键，合并注册表。使用前先 win + r 后输入 regedit，导出注册表对应的部分。导出过程中，程序可能无响应，不要慌，是导出的文件过大导致的，耐心等待即可。
```reg
Windows Registry Editor Version 5.00

[-HKEY_CLASSES_ROOT\*\shell\runas]

[HKEY_CLASSES_ROOT\*\shell\runas]
@="获取超级管理员权限"
"Icon"="C:\\Windows\\System32\\imageres.dll,-78"
"NoWorkingDirectory"=""

[HKEY_CLASSES_ROOT\*\shell\runas\command]
@="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"
"IsolatedCommand"="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"

[-HKEY_CLASSES_ROOT\Directory\shell\runas]

[HKEY_CLASSES_ROOT\Directory\shell\runas]
@="获取超级管理员权限"
"Icon"="C:\\Windows\\System32\\imageres.dll,-78"
"NoWorkingDirectory"=""

[HKEY_CLASSES_ROOT\Directory\shell\runas\command]
@="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
"IsolatedCommand"="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"

[-HKEY_CLASSES_ROOT\dllfile\shell]

[HKEY_CLASSES_ROOT\dllfile\shell\runas]
@="获取超级管理员权限"
"HasLUAShield"=""
"NoWorkingDirectory"=""

[HKEY_CLASSES_ROOT\dllfile\shell\runas\command]
@="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"
"IsolatedCommand"="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"

[-HKEY_CLASSES_ROOT\Drive\shell\runas]

[HKEY_CLASSES_ROOT\Drive\shell\runas]
@="获取超级管理员权限"
"Icon"="C:\\Windows\\System32\\imageres.dll,-78"
"NoWorkingDirectory"=""

[HKEY_CLASSES_ROOT\Drive\shell\runas\command]
@="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
"IsolatedCommand"="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
```
把获取超级管理员权限移除右键菜单
```reg
Windows Registry Editor Version 5.00

[-HKEY_CLASSES_ROOT\*\shell\runas]

[-HKEY_CLASSES_ROOT\Directory\shell\runas]

[-HKEY_CLASSES_ROOT\dllfile\shell]

[-HKEY_CLASSES_ROOT\Drive\shell\runas]

[-HKEY_CLASSES_ROOT\exefile\shell\runas]

[HKEY_CLASSES_ROOT\exefile\shell\runas]
"HasLUAShield"=""

[HKEY_CLASSES_ROOT\exefile\shell\runas\command]
@="\"%1\" %*"
"IsolatedCommand"="\"%1\" %*"
```
## 提交修改
```shell
git init  # 空文件夹 -> 仓库
git pull <url>  # url is like: https://github.com/username/packagename.git
git add .
git commit -a # delete "#" from which you want to alter
git status # 查看状态，可省略
git push -f <url> # the same as pull
```
## 大国关爱
设置代理(下面两者选一即可，不建议使用 http)
```shell
git config --global https.proxy 'socks5://127.0.0.1:1080'
git config --global https.proxy 'https://127.0.0.1:1080'
```
查看配置
```shell
git config --global --get https.proxy
```
取消代理
```shell
git config --global --unset https.proxy
```
我的配置
![Snipaste_2020-04-05_19-19-56](/img/Snipaste_2020-04-05_19-19-56.png)

## 搭建图床
#### 安装与使用
参考 [PicGo手册][1]。
#### 选择图床
>坚决不选老大哥控制的图床

选择 github 图床。建议另外创一个 package 用于保存所有图片。package 的名字不要有特殊符号，最好是无间隔的纯英文。
#### 中国特色
【PicGo设置】里可以配置代理，用来提高传输速度。

[1]:https://picgo.github.io/PicGo-Doc/zh/guide/#%E5%BA%94%E7%94%A8%E8%AF%B4%E6%98%8E
