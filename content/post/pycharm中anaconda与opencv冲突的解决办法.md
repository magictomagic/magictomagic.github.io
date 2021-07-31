---
title: "pycharm中anaconda与opencv冲突的解决办法"
date: 2020-04-23T01:37:56+08:00
lastmod: 2020-04-23T01:37:56+08:00
draft: false
tags: ["experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# 安装方法
参考[此博客][2]，[opencv 与 python 3.7 兼容][3]，环境变量里把 anaconda3 目录下的 3.7 的 python 版本设为所有 python 解释器的首选，去[官网][1]下载 opencv_python-4.1.2+contrib-cp37-cp37m-win_amd64.whl 到 anaconda3\Lib\site-packages，认准【cp37】，表示 python 3.7，最后在 anaconda3\Lib\site-packages 下 pip install opencv_python-4.1.2+contrib-cp37-cp37m-win_amd64.whl



[1]:https://www.lfd.uci.edu/~gohlke/pythonlibs/
[2]:https://blog.csdn.net/mawonly/article/details/87856530?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1
[3]:https://stackoverflow.com/questions/59699107/troubles-with-installing-jupyter-in-pycharm
