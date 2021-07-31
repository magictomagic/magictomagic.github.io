---
title: "react native"
date: 2021-03-20T01:37:56+08:00
lastmod: 2021-03-20T01:37:56+08:00
draft: false
tags: ["React Native"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---


# 特点

声明式，组件化，全平台，高效(virtual DOM, DOM Diffing 算法)

# 练习
>关键字联想回忆实现

## 模板

```react
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>hello_react</title>
</head>

<body>
    <!-- 准备好一个“容器” -->
    <div id="test"></div>

    <!-- 引入react核心库 -->
    <script type="text/javascript" src="./js/react.development.js"></script>
    <!-- 引入react-dom，用于支持react操作DOM -->
    <script type="text/javascript" src="./js/react-dom.development.js"></script>
    <!-- 引入babel，用于将jsx转为js -->
    <script type="text/javascript" src="./js/babel.min.js"></script>

    <script type="text/babel">

    </script>
</body>

</html>
```


## 基本功
### 创建渲染虚拟DOM 
>babel  jsx  容器  render ReactDOM

Hello World

### 标签混入JS & 内联样式 & 样式类名
>{}  {{}}  className

![image-20210304055254239](/img/image-20210304055254239.png)![image-20210304055331679](/img/image-20210304055331679.png)

### 练习

[Angular, React, Vue]

![image-20210304055615358](/img/image-20210304055615358.png)

### 函数式组件

### 类式组件

> React  Component  （constructor）setState

### prop

> props propTypes defaultProps prop-types.js 

限制必须有姓名

![image-20210305004410654](/img/image-20210305004410654.png)

### ref

> 字符串形式 回调函数形式 createRef current

![image-20210305010812854](/img/image-20210305010812854.png)

### 事件处理

> event target





![image-20210304063640074](/img/image-20210304063640074.png)

![image-20210304063646299](/img/image-20210304063646299.png)

![image-20210304063654743](/img/image-20210304063654743.png)

![image-20210304063705450](/img/image-20210304063705450.png)

