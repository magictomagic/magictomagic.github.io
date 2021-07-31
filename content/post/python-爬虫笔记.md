---
title: "Python 爬虫笔记"
date: 2021-01-16T01:37:56+08:00
lastmod: 2021-01-16T01:37:56+08:00
draft: false
tags: ["爬虫", "Python"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# conda 天坑
+ 有依赖无法删除导致其它包无法安装：`conda clean --all`
+ 慎用`conda update conda` 和 `conda update --force conda`，虽然它可能解决上述问题，但依然出现跑12个小时，然后依然是同一个报错的情况。
+ 其它尝试：
  - 更新所有包：`conda update --all`
  - 更新conda：`conda install anaconda`
+ conda 虚拟环境的使用
```bat
conda env list # 列出所有conda的虚拟环境
activate xxx # 进入虚拟环境xxx
conda install xxx # 常用安装命令
```
+ 安装第三方包，优先通过conda装，再通过pip装，定制性|场景性 强的，包多且用的少的，项目大的，会污染全局环境变量的，通过虚拟环境装，虚拟环境高于工程几个层级，便于多个相同类型的工程共用同一份虚拟的环境变量 | 系统专门搞了一个存多个虚拟环境的文件夹，所以**对所有工程的父类（即虚拟环境）命名要做到能清楚的表示情境，**以后用到什么工作栈就调哪个虚拟环境。
+ Pycharm 的作用分割：创建虚拟环境，命令行安装，运行程序，方便编辑，管理项目

## conda 其它命令

```txt
如果想要导出当前环境的包信息可以用
conda env export > environment.yaml

将包信息存入yaml文件中.
当需要重新创建一个相同的虚拟环境时可以用
conda env create -f environment.yaml

其实命令很简单对不对, 我把一些常用的在下面给出来, 相信自己多打两次就能记住
activate // 切换到base环境

activate learn // 切换到learn环境

conda create -n learn python=3 // 创建一个名为learn的环境并指定python版本为3(的最新版本)

conda env list // 列出conda管理的所有环境

conda list // 列出当前环境的所有包

conda install requests 安装requests包

conda remove requests 卸载requets包

conda remove -n learn --all // 删除learn环境及下属所有包

conda update requests 更新requests包

conda env export > environment.yaml // 导出当前环境的包信息

conda env create -f environment.yaml // 用配置文件创建新的虚拟环境
```

# request

动态加载的页面，若是 ajax 的，可以用 post 获取，例如百度翻译。数据以json格式作为`request.post`的`data=`参数传入。


# selenium

element.text 获取元素里面的文本

send_keys('xxx') 发送文本（针对输入框等可输入东西的）

python for in 遍历 elements 可遍历完嵌套 ？

webdriver（整个web页面） 和 element 对象 选择元素的方法是一样的

wd.implicitly_wait(10) 每隔半秒去界面上查看一次元素是否存在，直到找到该元素，或者超过了10秒，抛出异常

wd.quit() 关闭浏览器和驱动

element.get_attribute('outerHTML') 获取整个页面（场景：网页出错判断，爬虫）

element.text 获取元素展示在页面上的内容 

element.get_attribute('innerText') 或 textContent 获取没有展示在界面上的文本内容

find_element_by_css_selector(CSS Selector 参数) // 相当于querySelector // 可在浏览器 console ctrl+f 测试
>span:nth-child(2) 父元素<span>的第二个子节点  | `even` `odd`
>span:nth-last-child(2) 父元素<span>的倒数第二个子节点
>span:nth-of-type(2) 父元素（随便）第二个span类型的元素  |  `nth-last-of-type`
>p:nth-of-type(2) 限制p类型的偶数
>h3 + span `紧跟着`h3的span兄弟结点  | `~` `所有的`

wd.switch_to.frame('frame1') 根据i`d='frame1'`切换frame | wd 也可以是 element
wd.switch_to.default_content() 切换回原来的主html

wd.switch_to.window(handle) // for [标题栏 wd.title | 地址栏 ?? 的名字] in 
```python
mainWindow = wd.current_window_handle # 先保存当前handle

for handle in wd.windwow_handles:
	wd.switch_to.window(handle)
	if 'Bing' in wd.title:
		break
		
print(wd.title)

wd.switch_to.window(mainWindow) # 回来
```

wd.title 当前窗口的标题栏

`ActionChains`特殊动作模拟 e.g. 拖拽，右键点击...
```python
from selenium.webdriver.common.action_chains import ActionChains
ac = ActionChains(wd)
ac.move_to_element(
	wd.find_element_by_css_selector('[name="asdasd"]') # element对象
).perform()
```

# Xpath
console 调试
tag名@属性名='属性值'
`//`表示多层级
不得有标签`tbody`
```python
 //*[@id='west']
 //div[@class='single_choice'] # class值 必须全部相符
 //*[@multiple] # 所有含 multiple 属性的
 
 a[href*='daasd'] # 包含
 a[href^='http'] # 开头
 a[href$='.com'] # 结尾
 
//*[contains(@style,'color')] # 包含
//*[starts-with(@style, 'color')] # 开头
//*[ends-with(style, 'color')] # 结尾 xpath 2.0 的语法
 
//p[2] # p类型的第二个
p:nth-of-type(2) 

//p[last()] # p类型的最后一个元素
 
//div/p[2] # div下p类型的第二个
//div/*[2] # div下随便类型的第二个
```
通过 xpath 选择 element 中的元素，要加`.`。e.g. `ele.find_elements_by_xpath('.//p')`
```python
# 范围选择
//option[position()<=2] # 选取为<option>的第1到2个元素 position() 指代元素位置
//*[@class='asdf']/option[position()<=2]

option, h4
//option | //h4

# 选择父节点 /..
//*[@id='china']/..

# 选择所有后续兄弟节点
following-sibiling::* # preceding 前面的

# 提取 xpath 返回对象后的data值，若是列表，则全部提取出来，返回的还是列表
.extract() 

```

# scrapy
## scrapy 基础模块（文件）
### Spider 模块（文件）
负责处理响应，响应可能经过中间件(middlewares.py)，处理响应的入口是parse函数，之后若要新解析一个页面，则通过`yield scrapy.Request(new_url, callback=your_function, meta={'item': item})`回调函数发起请求，`meta`用作传 `item`对象，使不同页面的内容存于同一个对象中。最后通过 `yield item` 将`item`对象提交给管道用于持久化存储。要持久化存储，先要实例化`items.py`文件中的对象，即 `from xxx.items import xxxx`了之后，`item = xxxx()`。同时在`items.py`中确定哪些变量是`item`对象中的。
### items 模块（文件）
持久化存储的时候用。`items.py`中一个`class`对应确定哪些变量是那个`class`对应的`item`对象中的。可以有多个`class`对应多个`item`。
### piplines 模块（文件）
管道，流经的数据给数据库，用于持久化存储。也可以将数据存储在文件中，为了避免多次打开关闭 文件|数据库 造成的性能损耗，要重写 `open_spider(self, spider)` 和 `close_spider(self, spider)` 方法。`process_item` 用于保存数据，最后返回一个`item` 用作给下一个即将被执行的优先级较低的管道类。之后还需在`setting.py`中开启相应的管道。将数据写入文件的示例：
```python
class FirstsPipeline: 
    fp = None
    def open_spider(self, spider):
        self.fp = open('./hello.txt', 'w', encoding='utf-8')
    def process_item(self, item, spider):
        content = item['content']
        next_page = item['next_page']
        self.fp.write(content + '\n' + next_page + '\n\n')
        return item 
    def close_spider(self, spider):
        self.fp.close()
```
### setting 模块（文件）
配置 `User-Agent`,`cookie`等`DEFAULT_REQUEST_HEADERS`参数。打开管道，下载中间件，设置反爬规则，日志等级，下载图片时指定目录，
```python
DEFAULT_REQUEST_HEADERS = {
  'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
  'Accept-Language': 'en',
  'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.13; rv:61.0) Gecko/20100101 Firefox/61.0',
    'Cookie': 'HMACCOUNT_BFESS=2A84B6741779FE1C; BAIDUID_BFESS=FD4FBB71A5C5836ED9F95191F50FE5C1:FG=1; BDUSS_BFESS=3NncEdJTXBqcHhOTGNMSFR2MHdnTnhVc3ZkaGN5SzZFaEt5UENaeHd3VjJJakpnRUFBQUFBJCQAAAAAAAAAAAEAAADMXIZtOLLdMMTgMMLtOAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHaVCmB2lQpgM',
}
LOG_LEVEL = 'WARNING'
IMAGES_STORE= './img'
ROBOTSTXT_OBEY = False
```
### middlewares 模块（文件）
### 下载中间件
引擎和下载器之间，批量拦截整个工程中所有的请求和响应
#### 拦截请求
UA伪装：`process_request`模块，代理ip：`process_exception`模块
#### 拦截响应
篡改响应数据，例如动态加载的内容，通过`selenium` 来访问。


## 图片下载
**setting**中添加图片下在目录，在下载中间件**middlewares**上挂随机UA请求头和代理ip（需要购买），用于拦截请求。对于正常请求，在`process_request`中开启UA伪装，对于异常，在`process_exception`中用代理ip，例如：
```python
    user_agent_list = [
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 "
        "(KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1",
        "Mozilla/5.0 (X11; CrOS i686 2268.111.0) AppleWebKit/536.11 "
        "(KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 "
        "(KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.6 "
        "(KHTML, like Gecko) Chrome/20.0.1090.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.1 "
        "(KHTML, like Gecko) Chrome/19.77.34.5 Safari/537.1",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 "
        "(KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/536.5 "
        "(KHTML, like Gecko) Chrome/19.0.1084.36 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_0) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1062.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1062.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.0 Safari/536.3",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.24 "
        "(KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/535.24 "
        "(KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24"
    ]
    PROXY_http = [
        '153.180.102.104:80',
        '195.208.131.189:56055',
    ]
    PROXY_https = [
        '120.83.49.90:9000',
        '95.189.112.214:35508',
    ]
        def process_request(self, request, spider):
        # UA 伪装
        request.headers['User-Agent'] = random.choice(self.user_agent_list)
        # request.meta['proxy'] = 'http://175.43.156.8:9999'
        return None
        
        def process_exception(self, request, exception, spider):
        # ip 被禁掉了的话，用代理 ip
        if request.url.split(':')[0] == 'http':
            request.meta['proxy'] = 'http://' + random.choice(self.PROXY_http)
        else:
            request.meta['proxy'] = 'http://' + random.choice(self.PROXY_https)

        return request # 将修正之后的请求进行重新的请求发送
```
## CrawlSpider 
>爬取全站数据，Spider的一个子类
```bat
scrapy genspider -t crawl sun www.xxx.com
```

