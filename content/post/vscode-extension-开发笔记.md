---
title: "vscode extension 开发笔记"
date: 2021-03-29T01:37:56+08:00
lastmod: 2021-03-29T01:37:56+08:00
draft: true
tags: ["vscode", "extension", "develop"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

## 发布流程

### 直接上传

![image-20210329022301191](/img/image-20210329022301191.png)

### 根据官方文档

使用token管理

https://docs.microsoft.com/en-gb/azure/devops/organizations/accounts/use-personal-access-tokens-to-authenticate?view=azure-devops&tabs=preview-page

```shell
vsce package
vsce publish
```



https://marketplace.visualstudio.com/manage

## 经验

### TS 语法

Number(xxx) 有0 不用C++ isdigit

强类型如何开？（eslint有无这种风格：必须有分号，类型必须声明，两个空格）

#### 需要练习

定义回调函数

将异步操作(如readfile) 变为同步(控制顺序)

实现hook一样的闭包

python装饰器

### 开发流程

注释{最后}删

测试在开发目录下

{有时}正则匹配不如自己写算法，两者比较后再决策

找API或轮子切忌完美主义，不如自己改造或完全自己写

学习或创作,最好的方式是去看别人的源码. 想到一个功能, 去github看看别人是怎么实现的. 别人的项目结构是怎么设计的. 在模仿别人的同时感慨自己的失败.

### 模块划分

每个模块可测试，自底向上构建时以数据流为主体，就像积木是从地面上搭起来的一样

###  调研咨询

明确别人的使用场景与需求，现有的解决方案，别人在用自己的东西时候的体验

### 没事看一看

https://code.visualstudio.com/api/extension-guides/command 

  