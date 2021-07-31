---
title: "My First Android Application"
date: 2020-05-05T01:37:56+08:00
lastmod: 2020-05-05T01:37:56+08:00
draft: false
tags: ["Android", "project", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

## Android Field
>**假**话
学习安卓要有**锲而不舍**的精神与浓厚的**兴趣**。

>**大**话
学习安卓要坚持**四个自信**。

>**空**话
要密切关注所选择教程的**日期**。

>**套**话
Android Studio 的发展**日新月异**。

>**废**话
学习 Android 要习惯看**英文**的文档与问答。

### 环境搭建常见问题(待完善)
绝大部分问题都是 GFW 造成的。

[流量走 http，很多包不能通过 https 下载][37]
>具体实现是：AS 端口无论是 https 的还是 http 的全部填系统代理的 http 端口。

### [安卓常识][8]

### 添加第三方库
根据[文档][5]/[问答][4]。
以添加 jsch 为例

In file ...\app\build.gradle

![](/img/Snipaste_2020-05-05_04-44-29.png)

```gradle
dependencies {
    ...
    implementation 'com.jcraft:jsch:0.1.55'
}
```

another e.g.
[gradle doc][13] \ [新版本变化][12]
```gradle
implementation 'me.neavo:vitamio:4.2.2'
```


### [注册登陆系统实现][7]

### 安卓端实现 rtmp 点播
#### [服务端搭建][9]
#### 用 java 获取链接
在 eclipse 上开发
```java
package getLinkThroughSftp;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;  

import com.jcraft.jsch.Channel;  
import com.jcraft.jsch.ChannelExec;  
import com.jcraft.jsch.JSch;  
import com.jcraft.jsch.JSchException;  
import com.jcraft.jsch.Session;

public class getVideoLink {
    final private static String protocalName = "rtmp";
    final private static String IPaddress = "149.248.57.125";
    final private static int ffmpegPort = 1935;
    final private static String applicationName = "vod";

    final private static int defaultSSHport = 22;
    final private static String password = "B2$nBo7!]UkEL1@w";
    final private static String defaultUserName = "root";
    final public static String command = "ls /mnt/mp4s";

    public static int totalVideos;

    static List<String> videoNameList = new ArrayList<String>();

    private static String protocol(){
        String pro = protocalName;
        return pro;
    }

    private static String address(){
        String adr = IPaddress;
        return adr;
    }

    private static String application(){
        String app = applicationName;
        return app;
    }

    public void convertStringToList(String s) {
    	String str[] = s.split("\\s+");
    	videoNameList = Arrays.asList(str); 	
    }

    public void getVideoNameList(){
        JSch jsch = new JSch();    
        try {
        	java.util.Properties config = new java.util.Properties();
        	config.put("StrictHostKeyChecking", "no");
        	Session session = jsch.getSession(defaultUserName, IPaddress, defaultSSHport);
        	session.setPassword(password);
        	session.setConfig(config);
        	session.connect();
	    	System.out.println("Connected");

	    	// Create and connect channel.  
            Channel channel = session.openChannel("exec");  
            ((ChannelExec) channel).setCommand(command);
            channel.setInputStream(null);
	        ((ChannelExec)channel).setErrStream(System.err);
	        InputStream in=channel.getInputStream();
	        channel.connect();

	        byte[] tmp=new byte[1024];
	        while(true){
	          while(in.available()>0){
	            int i=in.read(tmp, 0, 1024);
	            if(i<0)break;
	            convertStringToList(new String(tmp, 0, i));
	          }
	          if(channel.isClosed()){
	            System.out.println("exit-status: "+channel.getExitStatus());
	            break;
	          }
	          try{Thread.sleep(1000);}catch(Exception ee){}
	        }
	        channel.disconnect();
	        session.disconnect();
	        System.out.println("DONE");

        }catch (JSchException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }

    public String getVideoName(int index) {		
    	return videoNameList.get(index);
    }

    public String jointLink(int index){
    	return protocol() + "://" + address() + ":" + ffmpegPort + "/" + application() + "/" +
                getVideoName(index);
    }
}

```
可以运行成功，但之后在 AS 里就不行。
#### 安卓端的开发
之前的 Java 代码在 AS 不能运行原因是 jsch 的方法为耗时进程，作为主进程时会被阻塞，导致应用程式结束，解决方法是用 [AsyncTask][41] 将获取链接的动作作为子进程。参考[相似的代码][42]，将 onPostExecute 作为返回主线程的方法。

打算在 onPostExecute 中进行后续的将 List 中的元素一链接的方式显示在 UI 上。
##### 根据上一个接口获得的 list 生成 xml
>之前的想法：[List][11]、[Intent][25]

**使用 AsyncTask 遇到的问题**

结束时设置 flagOfAsync 加一个 while 来确保 videoNameList 拿到值
```java
    @Override
        protected void onPostExecute(String host){
            Log.d("video","videoNameList.toString()");
            Log.d("video",videoNameList.toString());
            Toast.makeText(getVideoLink.this,"连接成功", Toast.LENGTH_LONG).show();
            flagOfAsync = true;
        }
```
```java
while (!flagOfAsync) {
            Log.d("video", "mainThread");
            Log.d("video", videoNameList.toString());
            if(flagOfAsync){
                break;
            }
        }
```
一开始一直拿不到值，陷入循坏消耗资源，等待时间过长。

分析：异步，AsyncTask 滞后执行，不是并发的。

解决方法：尝试并发编程

若用 wait()，则无论是异步还是并发都异常终止。

最终的解决办法是把获取视频列表的方法作为一个线程放到前面的活动中去。

**基于 MVC 模式对安卓应用程式的理解**

V: View接受用户的请求，然后将请求传递给Controller
>View 与部分 Activity

C: Controller进行业务逻辑处理后，通知Model去更新。
>Activity

M: Model数据更新后，通知View去更新界面显示。我们针对业务模型，建立的数据结构和相关的类。
>[the Activity with the fresh data should feed it to do Adapter.][43]

参考 [stackoverflow][43] 中的回答与 MVC 的定义，个人认为 Adapter 是 Model。

##### 播放 rtmp 链接的视频
**vitamio + ffmpeg**
[Vitamio API][22]。

参考[教程][24]实现代码。

ffmpeg 出现 libavcodec.so: has text relocations 的问题，可以[降低版本][15]解决，但不符合我的信仰，[官方][16]给出的 x86 模拟器的解决方案，但内容过于陈旧，不适合现在的环境，仍然无法解决问题。

![](\img\Snipaste_2020-05-05_11-55-41.png)

尝试更换 arm 模拟器，无法连接。。。

尝试连接 蛟骁 865 的真机，添加配置参数

in :app
```gradle
defaultConfig {
...
    ndk {
                abiFilters "armeabi", "armeabi-v7a", "x86", "mips","arm64_v8a"
            }
...
}
```
add a line in gradle.properties
```
android.useDeprecatedNdk=true
```
然后，报错与 x86 模拟器下相同

下面从两个方向寻找解决方法

1. 参考 Sat, Jul 16, 2016 的[教程][18]，从源码编译 x264 和 ffmpeg。

2. [寻找非 vitamio 包，播放 rtmp 链接][39]

* [jiecao:jiecaovideoplayer][23]: 尝试后放弃。

**After 9 days intermittently tring, ijkplayer saved me.**

对照这个[Activity][40]，与前面说的[教程][36]，将相关的一串依赖全部复制粘贴进来。

**考虑到 vitamio 搜到的都是远古时代的，且配置过于麻烦，不想再编译 ffmpeg 和 x264 了，就尝试下面的方法**
* [Streamaxia][29]: latest technology, need to learn build Android github project to test and integrate which Android [method][30].

5/5/2020 saved [tutorial][31]. **在 CentOS 8 用 wget 下载 pdf 文件不全，有损坏，不知道为什么。**

[official tutorial directory][32].

遇到困难：下载了 .aar 文件并作为模块导入，但无法使用类似 import com.karumi.dexter.PermissionToken; 的函数。

* ijkplayer

参考[这篇教程][36]，添加 implementation 后，将与播放 Rtmp 链接相关的 class，xml 复制粘贴到自己的工程中。最终成功播放。


* [偏底层][35]

[stackoverflow][21]

##### 学到
[Kotlin 优质博客][27]

《第一行代码(第三版)》
###### 整合别人的库（待尝试）
**其它尝试**
>将所有依赖打包起来

.AppImage格式的包

**之前失败**

.arr .so 模块导入

###### Android设备的 cpu 类型(参数说明)：
armeabiv-v7a: 第7代及以上的ARM处理器;

arm64-v8a: 第8代、64位ARM处理器;

armeabi: 第5代、第6代的ARM处理器;

mips:一种RISC处理器。

mips64:64位的。

x86: 平板;

x86_64:64位的平板。
###### Google Play sdk 版本管理
Google Play 会利用在应用清单中声明的 <uses-sdk>属性，从不符合其平台版本要求的设备上滤除您的应用。[17]
###### [AVD Manager 参数][19]
**[ABI][20]: Application Binary Interface**

ABI常表示两个程序模块之间的接口，且其中一个模块常为机器码级别的library或操作系统。

ABI定义了函数库的调用、应用的二进制文件（尤其是.so）如何运行在相应的系统平台上等细节。

Android目前支持以下七种ABI：armeabi、armeabi-v7a、arm64-v8a、x86、x86_64、mips、mips64。

###### 日期-oriented-信息-filter
**[Google 搜索指令][34]**
```Google
how to build FFmpeg-Android.sh               # all in English
android studio  play rtmp stream  after:2019 # latest
avengers endgame before:2019-04-01           # previous
```
**Chrome plugin**

Finitimus
###### code 搜索技巧
>github

**[搜索文件或路径中的字符][38]**

	octocat in:file,path 匹配文件内容或文件路径中出现 "octocat" 的代码。

**搜索文件名**
>AS

Ctrl + Shift + F 全局搜索某个变量

path/ 中直接找
######  .findViewById 中的 Id 存在
通过前一条方法寻找

string，color ... 标签中，res/下.xml文件中定义的标签

xml 本身的文件名

###### 兼容性
**NDK 兼容性**

NDK平台不是向后兼容（兼容过去的版本）的，而是向前兼容（兼容未来的版本）的。

NDK编译的版本应该尽量使用较低的版本，如minSdkVersion="8"。

**libxxx.so- text relocations [解决方案][26]**

放弃降低版本的方法，放弃在 Windows 下编译底层 ffmpeg 的解决方案，使用 ijkplayer 或基于 Linux 的 AS。
###### 从 VPS 下载各种文件
**解决：git clone 速度慢，中断。**
[设置 Xshell][28]下载路径。
```shell
yum install -y lrzsz   # for CentOS
tar cvf FileName.tar DirName
sz FileName.tar
```

###### 开发时的易错点(待完善)
**[so库的常见错误][19]**

使用高版本编译出的so库运行在低版本的平台上会出错。考虑到NDK是向前兼容的，应使用低版本编译。

so库放置的路径有误。

没有在每个生效的ABI目录下放置对应的so库。

**解决方法**

若某应用有armeabi和arm64-v8a两个ABI目录，armeabi目录里有a.so和b.so，但arm64-v8a只有a.so。
当ARMv8设备在安装此应用时，根据ABI优先级，首先发现arm64-v8a目录存在，并决定使用此ABI下的so库。
但arm64-v8a目录中没有b.so，于是报错。

此时的解决方案有：一，删除arm64-v8a目录；二，arm64-v8a目录的so库情况要与armeabi一致。

## 战略放弃
数据库同步云端
web登陆同步，配合nginx与Django
安卓端邮箱验证机制
#### 行迈靡靡，中心摇摇
停止运行的原因，无 finish?
生命周期，理解，防止崩溃？
[remain to see see][14]


## java
### 回忆
#### 调用非静态方法：
方法静态化
```java
public static int fxn(int y) {
    y = 5;
    return y;
}
```
生成对象
```java
Two two = new Two();
x = two.fxn(x);
```
#### [调用静态方法][10]
#### [键值-选择数据类型][6](待完善)
#### eclipse如何在项目中添加第三方jar文件
鼠标右击你的项目的名称->Buid Path->单击Add External Archives…->选择文件
### 新
#### [截取字符串][2]/[convert string to arraylist][3]

## 实现结果
* 注册登陆系统
* 通过 SSH 获取服务器上的视频名称集合，生成相应的 ListView
* 刷新按钮，获取服务器新的视频名称集合
* CentOS 8 上搭建可产生 Rtmp 流的服务器
* 基于 ijkplayer，点击相应的 ListView 播放视频

### BUG 与局限性
* 视频观看时退出后，部分系统声音仍在播放
* 只支持安卓10及以上系统
* 登录注册系统数据库未同步云端，且没有邮箱注册验证机制

### 可以优化的地方
* UI
* ListView 换效率更高的 RecyclerView，且按照日期从新到旧排列
* CentOS 8 上搭建 ffmpeg 的过程写成自动化脚本
* push 视频到服务器的自动化

[1]:https://stackoverflow.com/questions/11491750/cannot-make-a-static-reference-to-the-non-static-method-fxnint-from-the-type-t
[2]:https://blog.csdn.net/tterminator/article/details/52761483
[3]:https://beginnersbook.com/2015/05/java-string-to-arraylist-conversion/
[4]:https://stackoverflow.com/questions/49592197/packaging-jar-with-jsch-with-gradle
[5]:https://developer.android.com/studio/build/dependencies?hl=zh-cn
[6]:https://blog.csdn.net/chenchudongsg/article/details/39561817
[7]:https://blog.csdn.net/StayFoolish_Fan/article/details/51933563
[8]:http://c.biancheng.net/android/10/
[9]:https://github.com/magictomagic/magictomagic.github.io/blob/master/_posts/2020-05-04-CentOS-FFmpeg-%E6%B5%81%E5%AA%92%E4%BD%93%E6%92%AD%E6%94%BE.md
[10]:https://stackoverflow.com/questions/5642834/why-should-the-static-field-be-accessed-in-a-static-way
[11]:https://ithelp.ithome.com.tw/articles/10208440
[12]:https://stackoverflow.com/questions/44493378/whats-the-difference-between-implementation-and-compile-in-gradle
[13]:https://docs.gradle.org/current/userguide/java_library_plugin.html#sec:java_library_separation
[14]:https://source.android.com/devices/architecture/vintf/objects
[15]:https://stackoverflow.com/questions/32346402/libavcodec-so-has-text-relocations
[16]:https://trac.ffmpeg.org/ticket/4928
[17]:https://developer.android.com/guide/topics/manifest/uses-sdk-element?hl=zh-cn
[18]:https://yesimroy.gitbooks.io/android-note/content/compile_ffmpeg_for_android.html
[19]:https://www.jianshu.com/p/170f65439844
[20]:https://developer.android.com/ndk/guides/abis
[21]:https://stackoverflow.com/questions/53278193/java-lang-unsatisfiedlinkerror-dlopen-libffmpeg-so-has-text-relocations
[22]:https://blog.csdn.net/u010181592/article/details/49406353
[23]:https://blog.csdn.net/u011277123/article/details/70207854
[24]:https://github.com/hehonghui/android-tech-frontier/blob/master/issue-10/Android%E5%A6%82%E4%BD%95%E7%9B%B4%E6%92%ADRTMP%E6%B5%81.md
[25]:https://blog.csdn.net/zhiboshequ/article/details/53845993?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-3
[26]:https://blog.csdn.net/ahence/article/details/68953878
[27]:http://daking.tech/kotlin/Kotlin%E8%AF%AD%E6%B3%95%E5%9F%BA%E7%A1%80.html#%E5%9F%BA%E6%9C%AC%E5%85%83%E7%B4%A0
[28]:https://www.138vps.com/vpsjc/914.html
[29]:https://github.com/Streamaxia/Android-Open-SDK/blob/master/StreamaxiaOpenSDKDemo/appdemo/src/main/java/com/streamaxia/opensdkdemo/StreamActivity.java
[30]:https://github.com/Streamaxia/Android-Open-SDK
[31]:/files/OpenSDK_Android_3.2.1.pdf
[32]:https://raw.githubusercontent.com/Streamaxia/Android-Open-SDK/master/files/
[33]:https://stackoverflow.com/questions/9635050/how-to-play-rtmp-live-streaming-url-in-android
[34]:https://cn.engadget.com/2019/04/11/google-search-before-after-date/
[35]:https://juejin.im/post/5e5d17276fb9a07cc01a29d3#heading-62
[36]:https://www.jianshu.com/p/64e8d6fcef33
[37]:https://developer.android.com/studio/intro/studio-config.html#proxy
[38]:https://help.github.com/cn/github/searching-for-information-on-github/searching-code
[39]:https://www.jianshu.com/p/53581512ba3f
[40]:https://github.com/jiutianbian/android_learn/blob/0903fdeba571724b18cf442c25b0dd8a68ee9927/MyTV/app/src/main/java/jiutianbian/com/mytv/MainActivity.java
[41]:https://developer.android.com/reference/android/os/AsyncTask#onPostExecute(Result)
[42]:https://www.coder.work/article/687084
[43]:https://stackoverflow.com/questions/23345434/mvc-who-should-fetch-data-adapter-or-activity
