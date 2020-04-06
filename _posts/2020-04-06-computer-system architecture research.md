
## TPU GPU FPGA
TPU，张量处理单元(Tensor Processing Unit)，基于脉动阵列设计，矩阵加速单元，用来加速神经网络的计算，Google 出品。专门用于深度学习，编程类似 GPU 和 CPU，指令集为 CISC，支持卷积神经网络、LSTM、全连接网络等多种神经网络。
GPU，图形处理单元(Graphics Processing Unit)。
FPGA，现场可编程门阵列(Field Programmable Gate Array),它是在PAL、GAL、CPLD等可编程逻辑器件的基础上进一步发展的产物。它是作为专用集成电路领域中的一种半定制电路而出现的，既解决了全定制电路的不足，又克服了原有可编程逻辑器件门电路数有限的缺点。

## [技术前沿 TSP][1]
里程碑：芯片之争由摩尔定律转向架构。

## 千里之行，始于足下
基于[国情][2]与[自身水平][3]，我们可以用 HDL 代码(Verilog HDL 或者 VHDL)仿真硬件功能，或者基于 Xilinx HLS，通过 HLS 技术将 C/C++ 语言翻译为 HDL 代码。

## Xilinx 账户申请以及 vivado 安装
参考[这篇博客][4]

## Ubuntu 下 gem 的使用
>[gem5 教程][5]
[强力推荐的教程][6]

安装
```shell
git clone https://gem5.googlesource.com/public/gem5
cd gem5
scons
```









[1]:https://www.geekmeta.com/article/1242262.html
[2]:https://zhuanlan.zhihu.com/p/36522026
[3]:菜
[4]:https://blog.csdn.net/sheng__jun/article/details/79489716
[5]:http://www.gem5.org/documentation/learning_gem5/introduction/
[6]:http://learning.gem5.org/tutorial/index.html
