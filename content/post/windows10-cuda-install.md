---
title: "使用CUDA实现并行奇偶排序并使用性能分析工具分析"
date: 2020-04-03T01:37:56+08:00
lastmod: 2020-04-03T01:37:56+08:00
draft: false
tags: ["CUDA", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

## 配置目标环境
>由于基于 VMware 的虚拟机不支持调用 Host 的 GPU 卡，除非装双系统。因为网上已经有在真机上配置环境的教程了，又因为有大量的同学使用的是白嫖百度 GPU 的方法，为了增加文化的多样性，俺先考虑在 windows 下配置 CUDA 环境。

到[官网][1]下载安装包,慢慢安装。
![如图](/img/photo/cuda-win-1.png)
一路按照默认的安装来，最终安装程序很贴心地显示了哪些是安装的，哪些没安装。俺的偏好是在 Mingw-w64 下用 Atom 编辑器写代码。偏好其它环境的，可以参考[这篇文章][2]
![](/img/photo/installed-1.png)
>由于之前 Visual Stdio 耗尽了俺系统盘的存储空间，而且卸载残留严重，因为怕影响系统，有些组件都不敢卸载，就一直处于给系统盘瘦身的操作中，终于下定决心重装。总之，是很不愉快的体验。因此，俺是抵制安装 VS 所有产品的。

然而,用 nvcc 编译源代码，报错如下：
![](/img/photo/error1.png)
解决报错的方法根据 [stackoverflow][3] 或 [官网教程][4]只有安装恶心的 VS :(
俺用的是 VS 2019，从**工作负载**标签下选择【使用C++的桌面开发】
mlgj安装还要重启，中途如果遇到 NVIDIA 需要更新就更新一下，俺的原则是凡是只要有最新的，就更新到最新，和 Ubuntu 的原则差不多，默认最新就是最好的。然后找到 cl.exe 将它添加到环境变量中去。另外，基于我对 VS 的偏见，推荐大家备份环境变量，可以用此[脚本][5]。
编译：
![](/img/photo/bianyi1.png)
运行：
![](/img/photo/yunxing1.png)
看来环境是勉强配好了。
总结一下，对于 VS 我只是用到了它的 cl.exe，这个 exe 文件可能用到了 VS 的其它文件，所以我建议还是把 VS 装上比较好。先决条件是安装了 Mingw 的 C/C++ 运行环境，当然，NVIDIA 的 GPU 卡肯定要有，可以 win + r 后输入 dxdiag 查看卡的型号。
>接下来还需解决两个问题：一个是自动化运行，一个是解决warning。

**解决 warning:**
    虽然说 warning 没事，但俺有强迫症啊~
    要去除 warning，就要将报错所在文件的 Unicode 变成带签名的，详见[这里][6],首先要取得文件的完全控制权限，参考
    ![](/img/photo/quanxian1.png)
    我一共修改了两个文件的编码类型，然后运行之前的命令，warning 消失
    ![](/img/photo/warning-success.png)
    之后别忘了将文件夹的权限改回来。

**自动化运行:**
>优先寻找：根据文件后缀名的差异执行不同的命令的插件。
其次寻找：快捷键执行批处理等脚本的插件。

战略放弃。

## 性能分析工具
>之前装 VS 已经让俺的节操碎了一地。这次，根据[官网指示][7]：Visual Profiler 和 nvprof 将在以后的 CUDA 版本中弃用.建议使用下一代工具 NVIDIA Nsight Compute 进行 GPU 配置，使用 NVIDIA Nsight Systems 进行 GPU 和 CPU 采样和跟踪。我就坚定地开始了装 NVIDIA Nsight Compute 的征程。

进入[下载界面][8]，发现还要[注册][9],注册界面卡得雅匹，我只能开了全局，再不断尝试下载各个版本的 Nsight Compute，终于试出一个不用注册的，还下载得非常慢。
![](/img/photo/2019-4-available.png)
经过无数次下载失败，结合最近上外网有时很卡，再结合最近的朝鲜化趋势，我怀疑我的 V2Ray + tls + ws 的翻墙流量被干扰得太严重了。尝试用tg群里别人推荐的 rixcloud，说是一直很稳。当然，我是买不起 richcloud, 只能用它的原理：[ssr][10]，也是之前我为了装B而弃用的方法。费了很长时间才找到一个不在黑名单里的 IP,搭建完成后，继续使用 IDM 下载。之前用 V2Ray 不能断点续传的文件居然可以多线程下载了！
![](/img/photo/ssr-IDM.png)
>看来还是 ssr 的功能强大，开销小啊，虽然带来的是它的不安全。看来以后的主力就是 ssr，V2Ray 作为备用。ssr 的流畅让我明白了技术是为了适应生产场景而变化的，不是为了装B的。这里给出一个对 ssr 讨论比较专业的[网站][11]。

安装 Nsight Compute 这个包时，系统显示我已经安装了。这时就没必要再安装一遍了，因为如果安装的版本不是最新的系统会推荐你装最新的。参考[官网教程][12]中的命令
```bat
 nv-nsight-cu-cli -o profile CuVectorAddMulti.exe
```
以管理员的身份运行 nv-nsight-cu-cli 命令，得
![](/img/photo/nv-nsight-cu-cli-1.png)
>略去中间的输出

![](/img/photo/nv-nsight-cu-cli-2.png)
之后用 nv-nsight-cu <MyReport.nsight-cuprof-report>，得出
![](/img/photo/nv-nsight-cu-1.png)
至此，性能分析部分任务完成。
>如果喜欢用图形化界面，可以参考[这篇教程][14]，命令行输入 nv-nsight-cu 后直接 Enter，即可启动。
如果追寻更加高级的可视化的性能分析结果，可以参考[这个文档][13]。

下面用 CLI 和相应的图形化界面进行性能分析。

## 并行奇偶排序
```c++
#include<stdio.h>
#include<cuda.h>
#include<fstream>
#include<iostream>
using namespace std;
__global__ void oddeven(int* x,int I,int n){
	int id=blockIdx.x;
	if(I==0 && ((id*2+1)< n)){
		if(x[id*2]>x[id*2+1]){
			int X=x[id*2];
			x[id*2]=x[id*2+1];
			x[id*2+1]=X;
		}
	}
	if(I==1 && ((id*2+2)< n)){
		if(x[id*2+1]>x[id*2+2]){
			int X=x[id*2+1];
			x[id*2+1]=x[id*2+2];
			x[id*2+2]=X;
		}
	}
}
int main(){
	int a[100],n,c[100],i;
	int *d;
  ifstream infile("input1.txt");
  infile >> n;
  for (size_t i = 0; i < n; i++) {
    infile >> a[i];
  }
	cudaMalloc((void**)&d, n*sizeof(int));
	cudaMemcpy(d,a,n*sizeof(int),cudaMemcpyHostToDevice);
	for(i=0;i<n;i++){
		oddeven<<<n/2,1>>>(d,i%2,n);
	}
	printf("\n");
  cudaMemcpy(c,d,n*sizeof(int), cudaMemcpyDeviceToHost);
	printf("Sorted Array is:\t");
	for(i=0; i<n; i++){
		printf("%d\t",c[i]);
	}

	cudaFree(d);
	return 0;
}
```
![](/img/photo/INPUT1.png)
CLI 性能分析
![](/img/photo/20200404120233.png)
图形化界面
![](/img/photo/Gresult2.png)
显然，图形化界面不但直观，还有许多控件，可以选择不同层次的视图类型，甚至可以把结果保存成PDF、图片等多种类型，功能强大。






[1]:https://developer.nvidia.com/cuda-downloads?target_os=Windows&target_arch=x86_64&target_version=10&target_type=exelocal
[2]:https://zhuanlan.zhihu.com/p/85231502
[3]:https://stackoverflow.com/questions/8125826/error-compiling-cuda-from-command-prompt
[4]:https://docs.nvidia.com/cuda/cuda-quick-start-guide/index.html#windows-local
[5]:https://raw.githubusercontent.com/magictomagic/hotchpotch-bat/master/environment-backup.bat
[6]:https://www.zhaokeli.com/article/8276.html
[7]:http://s0docs0nvidia0com.icopy.site/cuda/profiler-users-guide/index.html
[8]:https://developer.nvidia.com/gameworksdownload
[9]:https://developer.nvidia.com/rdp/assets/nsight-compute-2019_5_1-Win64-installer
[10]:https://github.com/Alvin9999/new-pac/wiki/%E8%87%AA%E5%BB%BAss%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%95%99%E7%A8%8B
[11]:https://ssr.tools/82
[12]:https://docs.nvidia.com/nsight-compute/NsightComputeCli/index.html
[13]:https://docs.nvidia.com/drive/drive_os_5.1.12.0L/nsight-graphics/user-interface-reference/index.html#app_configuration_and_activity_selection_ui
[14]:https://s0docs0nvidia0com.icopy.site/nsight-compute/NsightCompute/index.html#abstract
