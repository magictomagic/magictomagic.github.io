---
title: "cvpr project10"
date: 2020-05-31T01:37:56+08:00
lastmod: 2020-05-31T01:37:56+08:00
draft: false
tags: ["cvpr", "project", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---
## 观察每张图像对应的角点检测的结果

![](/img/Snipaste_2020-05-31_21-31-56.png)
![](/img/Snipaste_2020-05-31_21-34-39.png)
![](/img/Snipaste_2020-05-31_21-35-49.png)
![](/img/Snipaste_2020-05-31_21-36-36.png)
![](/img/Snipaste_2020-05-31_21-37-06.png)
![](/img/Snipaste_2020-05-31_21-37-25.png)
![](/img/Snipaste_2020-05-31_21-37-39.png)
![](/img/Snipaste_2020-05-31_21-38-07.png)


## 标定以后
由于我是摄像机不动

![](/img/Snipaste_2020-05-31_21-40-47.png)

![](/img/Snipaste_2020-05-31_21-45-42.png)

## 手机参数
由

![](/img/Snipaste_2020-05-31_21-50-02.png)

得内参 K = 
$$
\left[
 \begin{matrix}
   3625.7 & 0 & 1750.6 \\
   0 & 3619.2 & 2285.8 \\
   0 & 0 & 1  
  \end{matrix}
\right]
$$
对于外参

旋转矩阵

![](/img/Snipaste_2020-05-31_23-50-06.png) = ![](../img/Snipaste_2020-05-31_22-46-06.png)

平移向量

![](/img/Snipaste_2020-05-31_23-50-40.png) = ![](../img/Snipaste_2020-05-31_23-48-15.png)

