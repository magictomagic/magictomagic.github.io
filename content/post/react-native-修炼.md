---
title: "react native 修炼"
date: 2021-03-21T01:37:56+08:00
lastmod: 2021-03-21T01:37:56+08:00
draft: true
tags: ["React Native"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# 每日一句

不练习就会被卷出 马太效应 自激振荡

# 环境搭建

## adb 命令
```shell
adb devices
```
## 环境变量

## AS 装相关包

##  跑起来

```shell
yarn react-native start  # 或 yarn global add expo-cli 后直接 yarn start
adb reverse tcp:8081 tcp:8081
# AS 打开，运行 
yarn react-native run-android  # 直接 yarn run 即可
```
## 错误处理

```shell
adb kill-server [设备名字]

# 杀端口
netstat -ano | findstr 8081 # 列出进程极其占用的端口，且包含 8081
taskkill -PID <进程号> -F # 强制关闭某个进程

# 若 jb 系列因端口被占用而无法启动
net stop winnat
net start winnat

# 端口被占用（permission denied）
https://superuser.com/questions/1437780/how-to-fix-listen-eacces-permission-denied-on-any-port

# 修改RN监听端口
https://blog.csdn.net/fengyuzhengfan/article/details/77389953

# 其它
https://reactnative.dev/docs/troubleshooting
```

# 编程思想

## 函数式组件

### 上层建筑 | 逼话

抓住主脉，忽略细节，上来就干。分析出需求点和关键点。

事件（动作）改变状态，Hook 组件函数操纵生命周期。

重点在 定义状态，明确状态之间是如何 转移|变换 的。

状态的要分 逻辑层|后端 与 视图层|前端，状态不够之间 Hook 加一个过来。

明确 哪些事件和状态转移有关，哪些无关。

初次写项目容易把状态分少。
>增加状态：并列 | 包含(特殊情况处理)

一个个需求实现，敏捷开发，状态按需 Hook。

Hook 的本质是闭包，需求分析与软件结构设计时要考虑哪些状态应该始终保留在内存中。

### 理解与坑

状态可以理解为后端。Hook（以useState为例）是`设置后端数据的方法`与`后端的数据`。

useMemo shouldcomponentUpdate

useEffect 中的方法改变页面的 render。

#### useEffect 

副作用：

+ 第一个参数

  + 回调函数：完成状态更新（componentDidUpdate）或组件渲染（componentDidMount）被触发。
  + 返回函数：componentWillUnmount

+ 第二个参数，可选，列表。

  + 不存在：第一次初始化 & 每次重新渲染 都触发回调

  + 数组存在且有值：数组中的任何值发生更改，每次重新渲染触发回调。

  + 空数组：回调只触发一次，类似于compoentDidMount

坑：

  多个状态改变导致频繁调用useEffect

避坑：

  明确限制哪些状态改变触发useEffect 

#### 函数“实例”

useRef()

可自定义Hook保存上一个“实例”的状态

##### TODO:

demo

### 模板 | 经验

#### 事件脉冲 导致视图层改变

```txt
useEffect外部定义平稳状态 | 里面定义处理高电平时的状态，处理完后返回平稳状态（低电平状态）
                      __
                     /  \
                    /    \
                ___/       \_____
```

### 不同的useEffect监控不同的状态

### 尽早规整化输出，方便发现错误

### useEffect最小状态包含

目前的实践是 结合**事件脉冲**使用

# 组件概览

https://reactnative.cn/docs/components-and-apis

## 长列表
### FlatList 

### SectionList

