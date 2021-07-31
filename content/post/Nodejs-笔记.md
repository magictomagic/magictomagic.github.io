---
title: "Nodejs 笔记"
date: 2021-01-11T01:37:56+08:00
lastmod: 2021-01-11T01:37:56+08:00
draft: false
tags: ["node.js"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# 基础
+ Node.js 没有 BOM、DOM，使 JS 可不通过浏览器运行，是 JS 的运行时环境，构建于Chrome 的 v8 引擎(专门处理JS的虚拟机，一般附带在网页浏览器中，解析执行JS代码)，

+ REPL: Read–eval–print loop

+ 服务器重定向针对异步请求无效

+ header('location')
  - 301 永久重定向，浏览器会记住
  - 302 临时重定向，浏览器不记忆

+ 同一个对象的两个引用若其中一个重新赋一个对象，则被赋对象的那个和另一个指的不是同一个，赋对象中的某个值没事，若重新 = 回来，则还是指同一个对象
```js
// 模块中默认有一句代码：
// var exports = module.exports

// 这里导致 exports !== module.exports
module.exports = {
	foo: 'bar'
}

// 这里重新建立两者的引用关系
exports = module.exports
```

```js
exports.a = 123
exports = {}
exports.foo = 'bar'
module.exports.b = 456
// result:
// a:123, b:456
```

+ markdowm css 避免自动升级覆盖：github.css 复制，重命名 github.user.css

+ npm install 加 --save 让 package.json（可通过 npm init 自动初始化出来） 中的 dependencies 选项保存第三方包的依赖信息。
  - npm install ：自动把 package.json 中的 dependencies 中所有依赖项下载回来   
  - npm uninstall  包名 ：只删除，若有依赖仍然会保存
  - npm uninstall --save 包名 ：删除的同时把依赖信息也去除

+ package-lock.json 锁定版本号，防止自动升级新版

+ 对密码进行 md5 重复加密 `body.password = md5(md5(body.password))`

+ 模块查找机制
  - 缓存(所以在循环加载时不会重新加载)
  - 核心模块
  - 路径形式的模块
  - 第三方模块(当前目录找不到则往上一个目录找)

+ nodemon 修改完代码自动重启，npm --global 安装

+ 第三方模块下的 package.json 中的main属性记录了模块的入口模块，默认是 index.js

+ 代码风格(JavaScript Standard Style / Airbnb JavaScript Style(更严谨))若无“;”，则当一行代码以"("、"["、"`" 开头, 前面要加 ";"。 e.g. ;`hello`.toString()

` 是 EcmaScript 6 新增的字符串包裹方式，叫模板字符串。

```js
var foo = `
adasd       add
adads
`
```
+ 路由
  - 请求方法
  - 请求路径
  - 请求处理函数
```js
app
  .get('/login', 函数1)
  .get('/asdfaf', 函数2)
  ...
```
## extend
```js
extend(obj1, obj2) // obj2 有了 obj1 的所有成员了
```

## 方法
+ $('div').each(function) 可用于遍历 jQuery 选择器选择到的伪数组(其原型链中无forEach)实例对象，此each在 jQuery 提供的原型链中

+ splice(start, length)
```js
var arr = [10, 5, 20] // [5]
arr.splice(1, 1)
// arr = [10, 20]
```
## 文件/模块 路径
```js
require('./b') // OK
require('./b.js') // OK
require('b') // error
require('/afa/asf.js') // 磁盘根目录
require(加载文件模块并执行里面的代码，拿到被加载文件导出的接口对象) 返回别的文件 exports 的对象，若被 require 的文件无 exports，则默认返回空(0)。将所有需要被外部访问的成员对象 exports
exports.add = function(x, y){ return x + y }
```

## 创建服务器
```js
var server = http.createServer();
server.on('request', function(req, res){
	var url = req.url;
	if(url === '/'){
	fs.readFile('./a/index.html', function(err, data){
		if(err){
			res.setHeader('Content-Type', 'text/plain; charset=utf-8');
			res.end('file read error');
		}else{
			res.setHeader('Content-Type', 'text/html; charset=utf-8');
			res.end(data);
		}
	})
	}else if(url === '/hhy'){
		fs.readFile('./a/asd.jpg', function(err, data){
			if(err){
				res.setHeader('Content-Type', 'text/plain; charset=utf-8');
				res.end('file read error');
			}else{
			// 也可以通过 <meta charset="UTF-8"> 实现
				res.setHeader('Content-Type', 'image/jpeg; charset=utf-8');
				res.end(data);
			}
		})
	}
})
```


## 模板引擎
将得到的文件名和目录名替换到 template.html 中：模板引擎，或在template.html中预留一个特殊的标记；根据 files 生成需要的 html 内容 ；替换
```html
<tbody id="tbody">HHY</tbody>
```
```js
//server.on ...
fs.readFile('./template.html', function(err, data){
	if(err){
		return res.end('404 Not Found.')
	}
	// 得到目录列表中的文件名和目录名： fs.readdir // 类似Apache FTP
	fs.readdir(wwwDir, function(err, files)){
	if(err){
		return res.end('Can not find Dir')
	}
	var content = ''
	files.forEach(function (item){
        content += `
         <tr>
            <td data-value="apple"><a>${item}<a></td>
            <td></td>
            <td></td>
         </tr>
        `
	})
})
    data = data.toString()
    data = data.replace('HHY', content)
    res.end(data)
})
```

### 模板引擎只关心自己能认识的模板标记语法
```html
<script type="text/template" id="tpl">
  大家好，我叫{{ name }}
  我喜欢 {{each hobbies}} {{ $value }} {{/each}}
</script>
<script>
var ret = template('tpl',{
	name: 'HHY',
	hobbies:[
		'唱',
		'跳',
		'Rap',
		'篮球'
	]
})
</script>
````

+ 客户端渲染(ajax，异步)，至少两次请求。第一次，页面；第二次，动态数据。可尽早看到页面(用户体验好)。爬虫爬不到。
+ 服务端渲染，一次请求。增加服务端压力。SEO。

## 表单提交方法 ？？？
`action`是表单提交地址(请求的`url`地址)
`method`请求方法
	post 数据放到请求体里
	get 数据放到 url 中
1. 默认
```html
<form action="asdf.php" method="get">

</form>
```
2. 异步

## exports
+ 如果一个模块需要直接导出某个（单个）成员，而非挂载的方式，若多个，后面会覆盖前面的
```js
// 导出单个成员
module.exports = add
```

+ 若导出为对象
```js
// 导出多个成员
  // 方法1
exports.add = add // function
exports.a = "asdas"

  // 方法2
module.exports = {
}
```

+ 默认在代码的最后一句有：`return module.exports`

# express 框架 
web 开发框架之一，提高开发效率

+ + e.g.
```js
res.redirect('/')
// res.statusCode = 302
// res.setHeader('Location', '/')
```

## 模板引擎 art-template
+ 详见官方文档
+ install
```shell
npm install --save art-template
npm install --save express-art-template
```
+ config
  - 第一个参数表示当渲染以`.art`结尾的文件的时候，使用 art-template 模板引擎。
```js
app.engine('art', require('express-art-template'))
```
+ usage
  - express 为 response 相应对象提供了方法：render。默认不可用，若配置模板引擎就可以用了。
  - `res.render('html 模板名', {模板数据})`
```js
app.get('/', function(req, res){
	res.send('hello world') // 相当于 write

	// express 默认去 views 目录中找 index.html
	// 开发人员默认所有把视图文件都放到 views 目录中
	res.render('index.html', {
		title: 'hello world'
	})
	
	res.render('index.html',{
		fruit:['苹果','香蕉', '橘子']
	})
	
	res.render('index.html',{
		// 文件读到的是字符串，需要手动转成对象
		students: JSON.parse(data).students
	})
}
```
```json
{
"students":[
	{"id": 1, "name": "asd"},
	{"id": 2, "name": "adaasd"}
]
}
```


+ 如果希望修改默认的 `views` 视图渲染目录
``` js
// 第一个参数 views 不要写错
app.set('views', render函数的默认目录路径)
```
### 语法积累

相同的页眉页脚

```art-template
{{extend './layout.html'}}
{{block 'content'}}
<div>
	<h1>页面默认显示这个内容</h1>
</div>
{{/block}}
```

```art-template
{{include './header.html'}}
<h1> hello {{name}}</h1>
{{include './footer.html'}}
```



## 静态服务

开放资源：公开指定目录
```js
// 去 ./public/ 目录中找对应的资源，推荐使用这种更易辨识的方式
app.use('/public/', express.static('./public'))
// 可以直接通过 /public/xx 的方式访问 public 目录中的所有资源了

// 省略 /public 的方式来访问
app.use(express.static('./public/'))
```

## post
+ 需要第三方包：`body-parser`
```js
var express = require('express')
var bodyParser = require('body-parser')

var app = express()

// 配置 body-parser 中间件（插件，专门用来解析表单 POST 请求）要在 app.use(router) 挂载路由之前
// req 请求对象上会多出一个属性：body。即可通过req.body获取表单中的post请求体数据了
// parse application/xxx.form.urlencoded
app.use(bodyParser.urlencoded({ extended: false }))
// parse application/json
app.use(bodyParser.json())
```
获得 post 的数据
```js
router.post('/students/new', function(req, res){
	// 1. 获取表单数据
	// 2. 处理 (抽象出单独一个模块专门 CURD)
	// 	   将数据保存到 db.json 中用以持久化
	//		readFile，parse，push，stringify，writeFile
	// 3. 发送响应
	console.log(req.body)
})
```

## 错误处理

```js
if(err){
	return res.status(500).send('Server error')
}
```

## 经验积累
### json 响应方法
接收一个对象作为参数，自动把对象转为字符串再发送给浏览器
```js
res.status(200).json({
	success: true,
	message: 'ok'
})
```
### 内置中间件
express.static
express.json
**express.urlencoded** parses incoming requests with URL-encoded payloads

### 第三方中间件
body-parser
compression
cookie-parser
morgan
response-time
serve-static
session

## 模块职责清晰而单一
### 提取路由
将路由放在另一个文件中，exports；在 app.js 中 require
```js
// 传统
module.exports = function (app){
	app.get('/adas', function(req, res){})
	app.get('/adsg', function(req, res){})
	app.get('/adas/sfdsd', function(req, res){})	
}

// express 提供：专门用来包装路由的方式
// 1. 创建一个路由容器
var router = express.Router()

// 2. 把路由都挂载到 router 路由容器中
router.get('/adas', function(req, res){})
router.get('/adsg', function(req, res){})
router.get('/adas/sfdsd', function(req, res){})

// 3. 把 router 导出
module.exports = router

```

```js
// 把路由容器挂载到 app 服务中
app.use(router)
```
### 回调函数
+ 常规
```js
function myDisplayer(some) {
  document.getElementById("demo").innerHTML = some;
}

function myCalculator(num1, num2, myCallback) {
  let sum = num1 + num2;
  myCallback(sum);
}

myCalculator(5, 5, myDisplayer);
```
+ 模块化 | 异步 
使用 回调函数（获取异步操作结果）
```js
Student.updateById(req.body, function(err){
	if(err){
		return res.status(500).send('Server error')
	}
	res.redirect('/students')
})
```
定义回调函数
```js
exports.updateById = function(student, callback){
	fs.readFile(dbPath, 'utf8', function(err, data){
    if(err){
    	return callback(err)
    }
    var students = JSON.parse(data).students
	
	// 需要修改谁，就把谁找出来，ES6 数组方法：find，当某项符合 item.id === students.id 的时候，find 终止遍历，同时返回遍历项。
	var stu = students.find(function(item){
		return item.id === student.id
	})
	})
}
```
+ 获取学生列表
```js
exports.find = function(callback){
	fs.readFile(dbPath, 'utf8', function(err, data){
		if(err){
		// 第一个参数，成功是 null，错误是 错误对象
			return callback(err)
		}
		// 第二个参数，成功是 数组，错误是 undefined
		callback(null, JSON.parse(data).students)
	})
}
```

# MongoDB
`show dbs` 显示所有数据的列表
`db` 显示当前数据库对象或集合
`use 指定要连接的数据库名`  若没有则创建
`db.students.insertOne({"name": "Jack"})` 存的是一个对象
`db.students.find()` list
## mongoose
基于 MongoDB，官方的包强大但用起来麻烦
```js
const mongoose = require('mongoose');
mongoose.connect('mongodb://localhost:27017/test', {useNewUrlParser: true, useUnifiedTopology: true});

// 创建一个模型：设计数据库
const Cat = mongoose.model('Cat', { name: String });

// 实例化 Cat
const kitty = new Cat({ name: 'Zildjian' });

// 持久化保存 kitty 实例
kitty.save().then(() => console.log('meow'));
```
## 使用
* 
```js
// 设计 Schema，发布 model
mongoose.connect('mongodb://localhost/itcast') // 没有则创建

// 设计集合结构（表结构），字段名称就是表结构中的属性名称
// 约束的目的是保证数据的完整性，不要有脏数据
var userSchema = new Schema({
	title: String,
	meta: Number,
	comments: [{body: String, date: Date}],
	date: {type: Date, default: Date.now},
    name:{
        type: String,
        required: true // 约束
    },
    gender:{
        enum: [0, 1],
        default: 0
    }
})

// 将文档结构发布为模型
var User = mongoose.model('User', userSchema)
// 第一个参数：首字母大写，表示数据库名称，mongoose 自动将其变为 users 集合名称

// 使用模型构造函数对users集合中的数据进行操作
var admin = new User({
	username: 'admin',
	password: '123456'
})
admin.save(function(err, ret){
    if(err){
    	console.log('保存失败')
    }else{
    	console.log('保存成功')
    }
})
```


* 根据 id 删除数据
```js
User.findByIdAndRemove(id, function(err){
    if(err){
        return res.status(500).send('Server error')
    }
    res.redirect('/users')
})
```

* 查询数据
```js
User.findOne({ // 还有 find，返回的文档可能不止一个
    		// 查到&删除数据：User.remove({...})
    username: 'zs'
}, function(err, ret){
    if(err){
        console.log('查询失败')
    }else{
        console.log('查询成功')
    }
})
```
* 更新数据
根据条件更新：
```js
Model.update(conditions, doc, [options], [callback])
```
根据指定条件更新一个：
```js
Model.findOneAndUpdate([condition], [update], [options], [callback])
```
根据 id 更新一个
```js
User.findByIdAndUpdate('afdafdadsfasf',{
    password:'123'
}, function(err, ret){
	if(err){
		console.log('更新失败')
	}else{
		console.log('更新成功')
	}
})
```

## 经验&注意

```js
var userSchema = new Schema({
    email: {
		type: a + b, // new 的同时已经算好了
		required: true
    },
    creat_time: {
    	type: Date,
    	required: Date.now // 如果加()，时间立即创建，就写死了
    }
})
```

# promise
## 回调地狱
嵌套回调函数以保证执行顺序，导致![](https://pic1.zhimg.com/v2-cf1c78890006e078a538842a0caa7127_1440w.jpg?source=172ae18b)
## 用法
```js
console.log(1)

// 结果：1243
// Promise 本身不是异步，它里面的任务是异步
// Promise 容器一旦创建，就开始执行里面的代码
new Promise(function(){
	console.log(2)
	fs.readFile('./a.txt', 'utf8', function(err, data){
		if(err){
			console.log(err)
            // 失败了，把容器的 Pending 状态变为 Rejected
            reject(err)
		}else{
			console.log(3)
			console.log(data)
            //成功了，把容器的 Pending 状态变为 Resolved
            resolve(data)
		}
	})
})
console.log(4)
```

```js

p1
  .then(function(data){ // 这个 function 就是 resolve
      console.log(data)
      // 如果没有return，后面受到的就是undefined
      // return 123 // 给了下面一个 then 
      return p2 //  return 一个 Promise 对象，后续的 then 方法的第一个参数作为p2的 resolve
  }, function(err){
      console.log('读取文件失败')
  })
  .then(function(data){
      // console.log(data) // 是前面 return 123 中的data
      console.log(data) // p2 处理 data，顺序保证了
  })
```
## 实战
```js
// 按顺序读取文件
var fs = require('fs')
function pReadFile(filePath){
	return new Promise(function(resolve, reject){
		if(err){
			reject(err)
		}else{
			resolve(data)
		}
	})
}

pReadFile('./data/a.txt')
  .then(function(data){
      console.log(data)
      return pReadFile('./data/b.txt')
  })
  .then(function(data){
      console.log(data)
      return pReadFile('./data/c.txt')
  })
  .then(function(data){
      console.log(data)
  })
```
## async & await
* The keyword `async` before a function makes the function return a promise
```js
async function myFunction() {
  return "Hello";
}
myFunction().then(
  function(value) {myDisplayer(value);},
  function(error) {myDisplayer(error);} // 可以没有这一行
);
```
```js
let hello = async function() { return "Hello" };
hello();

let hello = async () => { return "Hello" };
```


* `await` only works inside async functions. This can be put in front of any async promise-based function to pause your code on that line until the promise fulfills, then return the resulting value.
```js
// .then() 变为 await
```

## jquery ??? 兼容 Promise
```js
data = {}
$.get('http://127.0.0.1:3000')
    .then(function(user){
    data.user = user
    return $.get('http://127.0.0.1:3000/jobs')
})
.then(function(jobs){
    data.jobs = jobs
})
```


# 中间件
本质是请求处理方法
## 消费规则与控制
``` js
app.use(function(req, res, next){ // 没有路径参数，万能匹配
    console.log("1")
    next()  // 这个函数消费完后，继续往后面走
})

// 严格匹配请求方法和路径的中间件 app.get app.post
app.get('', function(req, res, next){

})
```

## 错误处理

```js
app.get('/', function(req, res, next){
    fs.readFile('./a/ad', function(err, data){
        if(err){
            // 当调用 next 的时候，如果传递了参数，则直接往后找到带有四个参数的中间件
            next(err)
        }
    })
})

// 配置错误处理中间件
app.use(function(err, req, res, next){
    console.log('app error handler')
})
```



## 第三方中间件

Express 框架中，默认不支持 Session 和 Cookie，但是可以使用第三方中间件：express-session 来解决：`npm install express-session`；在app.use(router)前配置；使用：通过 req.session 来访问和设置 session 成员
```js
req.session.xxx = 'bar' // 添加
req.session.xxx // 读
req.session.xxx = null // 删除（不严谨）
delete req.session.xxx // 删除（严谨）
```

```js
app.use(session({
	secret: 'keyboard cat',
	resave: false,
	saveUninitialize: true
}))
```
## 其他
中间件，需要经验积累


# path 路径操作模块
## 文件操作同一转换为动态的绝对路径
__dirname **动态获取**可以用来获取当前文件模块所属目录的绝对路径
__filename **动态获取**可以用来获取当前文件的绝对路径
__dirname 和 __filename 不受执行node命令所属路径影响
拼接路径：path.join()

```js
app.use('/public/', express.static(path.join(__dirname, './public/')))
app.set('views', path.join(__dirname, './views/')) // 默认就是 ./views 目录
```

* path.basename

  获取一个路径的文件名（默认包含扩展名）

* path.dirname

  获取一个路径的目录部分

* path.extname

  获取一个路径的扩展名部分
  
* path.parse
  把一个路径转为对象
    root 根目录
    dir 目录
    base 包含后缀名的文件名
    ext 后缀名
    name 不包含后缀名的文件名

* path.join
  路径拼接方法
  
* path.isAbsolute 
  判断一个路径是否为绝对路径

# cheerio

## chrome console debug

> 语法和 jQuary 基本一致，调试是若网页无`$`，则引入 jQuary 

```javascript
var script = document.createElement("script");
script.src = "https://cdn.bootcss.com/jquery/3.4.1/jquery.min.js";
document.getElementsByTagName('head')[0].appendChild(script);
```

## jQuary 学习笔记

### 特性

```javascript
// 入口函数：防止页面还没被渲染出来脚本就执行了
	// 1.1、原生Js会等到DOM元素加载完毕，并且图片也加载完毕才会执行；
	// 1.2、jQuery会等到DOM元素加载完毕，但不会等到图片加载完毕就会执行。
	// 2.1、原生Js如果编写多个入口函数，后面编写的会覆盖前面编写的；
	// 2.2、JQuery中编写多个入口函数，后面的不会覆盖前面的。
// 三种写法
$(document).ready(function () { // 文档加载出来以后执行
	alert(“入口函数1”);
    $('div').width(100).height(100).css('backgroundColor', 'red').text("ada"); // 操作元素，链式编程
});

$(function () {
//文档和图片都加载完成 相当于原生的 window.onload
alert(“JQ的第一个入口”);
});

$(window).ready(function () { //文档和图片全部加载完 执行
alert(“window加载完”);
})

$('.wrap>ul>li').mouseenter(function(){ 	   // mouseover 有冒泡，mouseenter阻止冒泡
    // js 一般在前面多一个 on, e.g. onmouseover	// mouseout			mouseleave
    //this是触发了鼠标移动事件的dom对象	// hide() 相当于css('display', 'none')
    $(this).children('url').show();    // show() 相当于css('display', 'block')
})								

// css 设置多样式，可传入对象
$('#div1').css({
    width: 300,
    'height': '300px',
    'background-color': 'green',
    border: '10px solid pink'
})

// 案例：突出展示
$('.swap').find('li').mouseenter(function(){
    // $(this).css('opacity', 1) 返回设置这个方法的元素本身
    $(this).css('opacity', 1).siblings('li').css('opacity', 0.4);
})
$('.swap').mouseleave(function(){
    $(this).find('li').css('opacity', 1);
})

// 案例：手风琴
/*
<li class="menuGroup">
	<span class="groupTitle">标题i</span>
	<div>我是弹出来的divi</div>
</li>
*/
$('.parentWrap>.menuGroup').click(function(){
    // 隐式迭代，链式编程
    $(this).children('div').show().parent().siblings('li').children('div'.hide());
})

// 案例：淘宝服饰精品
$('#left>li').mouseenter(function(){
    var idx = $(this).index() // 当鼠标移入时，获取这个li标签的索引
    $('#center>li').eq(idx).show().siblings('li').hide(); // idx对应的索引显示，其余隐藏
})
```

### 选择器

![image-20210228204309314](/img/image-20210228204309314.png)

![image-20210228204326342](/img/image-20210228204326342.png)

![image-20210228204339673](/img/image-20210228204339673.png)

## Vue 学习笔记

### 体验

区别与传统 js 的命令式编程，它的编程范式是声明式编程。

```html
<div id="app">
    <h2>当前计数：{{counter}}</h2>
    <button v-on:click="add">+</button>
    <button v-on:click="sub">-</button>
    <ul>
        <li v-for="item in movies">{{item}}</li>
    </ul>
</div>

<script src="../js/vue.js"></script>
<script>
	const app = new Vue({
        el: "#app",  // 挂载要管理的元素
        data: {		// 定义数据
            counter: 0,
            movies: ['进击的巨人', '龙珠', '天空之城']
        },
        methods: {
            add: funciton(){
            	console.log('add');
        		this.counter++
        	},
            sub: function(){
        		console.log('sub');
        		this.counter--
    		}
        }
    })
</script>
```

### 前置知识

#### webpack

#### yarn 

#### vue in node.js

## vuepress