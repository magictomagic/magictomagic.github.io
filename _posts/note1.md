## 搜学
《The Little Schemer》（前身叫《The Little Lisper》

TeXmacs

想要真正理解 Java 设计模式的人可以看看这本：A Little Java, A Few Patterns:

braid on steam

。所以我觉得它是学习程序设计最好的入手点和进阶工具。当然 Scheme 也有少数的问题，而且缺少一些我想要的功能，但这些都瑕不掩瑜。在用了很多其它的语言之后，我觉得 Scheme 真的是非常优美的语言。

SICP里貌似有一节就是教你写个符号微分程序。做微积分这种无聊的事情，就是应该交给电脑去做。总之，这从一方面显示了，Lisp 的语法其实超越了传统的数学。

Chez Scheme 生成的“目标代码”效率之高，我还没有见到任何其它 Scheme 编译器可以与之匹敌。而它的“编译速度”之快，没有任何语言的任何编译器可以相提并论（注意我去掉了“Scheme”这个限定词）。Chez Scheme 可以在 5 秒钟之内完成从头到尾的自我编译。想想编译 GCC 或者 GHC 需要多少时间，你就明白差距了。

Kent 的课程编译器

如果你需要一个 Scheme 版本用于学习的话，Chez Scheme 有一个免费的版本叫做 Petite Chez Scheme，可以免费下载。因为 Petite 的出错信息非常不友好，所以我也推荐 Racket 作为替补。不过你需要注意的是，Racket 的速度比起 Chez Scheme 是天壤之别。

专门讲语义的书很少，现在推荐一本我觉得深入浅出的：《Programming Languages and Lambda Calculi》。只需要看完前半部分（Part I 和 II，100来页）就可以了。这书好在什么地方呢？它是从非常简单的布尔表达式（而不是 lambda calculus）开始讲解什么是递归定义，什么是解释，什么是 Church-Rosser，什么是上下文 (evaluation context)。在让你理解了这种简单语言的语义，有了足够的信心之后，才告诉你更多的东西。比如 lambda calculus 和 CEK，SECD 等抽象机 (abstract machine)。理解了这些概念之后，你就会发现所有的程序语言都可以比较容易的理解了。


---
能做的事越多，代码量却越少。也许这就叫做程序的“美”，它跟数学的“美”其实是一回事。

美的程序不可能从修修补补中来。它必须完美的把握住事物的本质，否则就会有许许多多无法修补的特例。其实程序员跟画家差不多，画家如果一天到头蹲在家里，肯定什么好东西也画不出来。程序员也一样，蹲在家里面对电脑，其实很难写出什么好的代码。你必须出去观察事物，寻找“灵感”，而不只是写代码。在修改代码的时候，你必须用“心灵之眼”看见代码背后所表达的事物。这也是为什么很多高明的程序员不怎么用调试器（debugger）的原因。他们只是用眼睛看着代码，然后闭上眼，脑海里浮现出其中信息的流动，所以他们经常一动手就能改到正确的地方。

---

#### Chez Scheme

世界上最快，最成熟可靠的 Scheme 实现是 R. Kent Dybvig 所作的 Chez Scheme。它可以把 Scheme 编译成机器代码，运行速度非常高。Chez Scheme 曾经是商业软件，价格昂贵，然而现在却开源了，并且可以免费使用。你可以在这里下载 Chez Scheme 的源代码：

https://github.com/cisco/ChezScheme

编译安装很快很方便，在 Linux 和 Mac 系统基本就是这样：

```shell
./configure
make
sudo make install
```

---

为了让段落的行看起来均匀，我使用了一种类似 TeX 的动态规划断行算法。它先算出多种断行方案的“难看程度”，然后从中选出最好看的一个。

---

。Lisp Machine 似乎是其中最接近的一个。[Oberon](http://www.yinwang.org/blog-cn/2013/03/07/oberon) 是另外一个。IBM System/38 是类似系统里面最老的一个。最近一些年出现的还有微软的 [Singularity](http://research.microsoft.com/en-us/projects/Singularity)，另外还有人试图把 JVM 和 Erlang VM 直接放到硬件上执行。

---

### 结构化编辑器

---

怎么说呢，我觉得每个程序员的生命中都至少应该有几个月在静心学习 Haskell。学会 Haskell 就像吃几天素食一样。每天吃素食显然会缺乏全面的营养，但是每天都吃荤的话，你恐怕就永远意识不到身体里的毒素有多严重。

---

first-class function 

---

没有任何一种语言值得你用毕生的精力去“精通”它。“精通”其实代表着“脑残”——你成为了一个高效的机器，而不是一个有自己头脑的人。你必须对每种语言都带有一定的怀疑态度，而不是完全的拥抱它。

---

