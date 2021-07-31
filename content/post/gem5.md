---
title: "gem5 benchmark"
date: 2020-06-24T01:37:56+08:00
lastmod: 2020-06-25T01:37:56+08:00
draft: false
tags: ["gem5", "python", "recurrence"]
categories: ["exp"]
author: "magictomagic"

contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'

---

复现[论文][1]

复现过程简介(在Ubuntu 18/20测试成功)：

* 下载编译安装gem5(X86和ARM都要)，照论文在X86中添加一个CPU类型(修改两处)，重新编译gem5(X86)
* apt命令安装交叉编译工具，使其可执行arm-linux-gnueabi-gcc命令
* 运行[自动化基准测试脚本][2]前，看其前面注释的说明，要在gem5/下git clone一个网址，再运行。
* 运行完成后，会在gem5/m5out/下生成40个文件夹，在gem5/下运行[自动化生成柱状图脚本][3]，若输出路径，说明此路径下的stats.txt文件中没有所需要的数据，要手动调试。
* 最后生成20张图，关掉一张来一张。若遇到其它错误，可以参考[实验报告][]
* https://github.com/magictomagic/magictomagic.github.io/blob/master/files/gem5/result.docx



手动调试命令参考

```shell
 build/ARM/gem5.opt --outdir=m5out/patricia-ARM-MinorCPU-32kB configs/example/se.py -c ./mibench/network/patricia/patricia -o ./mibench/network/patricia/small.udp --cpu-type=MinorCPU --l1d_size=32kB --l1i_size=32kB --l2_size=512kB --l1d_assoc=2 --l1i_assoc=2 --l2_assoc=8 --caches --l2cache --cpu-clock=1GHz
```





[1]:https://ieeexplore.ieee.org/iel7/8384070/8391283/08391354.pdf
[2]:../files/gem5/auto_bench.py
[3]:../files/gem5/auto_figure.py
[4]:../files/gem5/result.docx
