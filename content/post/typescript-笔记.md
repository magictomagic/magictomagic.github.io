---
title: "typescript 笔记"
date: 2021-02-11T01:37:56+08:00
lastmod: 2021-02-11T01:37:56+08:00
draft: false
tags: ["docker"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

# TS 基础

## 语法
### 基本类型
```typescript
let a:number = 23;
function asd(a:number, b:number):number{ // 指定传入与返回的类型
    return a + b;
}

function sdf():void{ // 函数返回值为空
    //不写，或下面三种，都可以
// return;
// return undefined;
// return null;
}

function dfg():never{ // 函数永远不会有返回值
    throw new Error('error'); // e.g.case...
}

console.log(asd(3, 5))

let c : string | number | boolean; // 联合类型
let d : any; // 任意类型，尽量别用
c = d // any 类型可以赋值给其它确定类型的变量，还影响别人
let e : unknown; // 类型安全的any，不能直接赋值给其它
// c = e; // 直接赋值给其它，会报错，不影响别人

let f :string = 'asdas';
let g:unknown = 'qqqqq'
// f = g //直接赋值，报错
// 防止报错：1：类型检查
if(typeof g === 'string'){
    f = g
}
// 防止报错：2：类型断言
// 告诉解析器 g 的实际类型
f = g as string; // 写法1 
f = <string> g; // 写法2

// 定义对象的结构
let h : {num:number, str?:string}; // 指定对象中用来包含哪些属性
h = {num:123}; // ?表示属性可选，有没有都行

// 属性名为string，属性值可为任意类型(any)，个数随意，但必须包含num:number模式
let p : {num:number, [asdf:string]:any}; 
p = {num:234, aef:'dfsd', df: true} 

// 定义函数的结构
let sum1 : (a:number, b:number) => number;
sum1 = function(asd, sdf){
    return asd + sdf;
}
console.log(sum1(3, 5))

// 数组有两种表示方法
let aa : string[];
aa = ['asfas', 'af']
let bb : Array<string>;
bb = ['aas', 'asdfads']

// 元组 tuple 固定长度的数组
let cc : [string, number]
cc = ['afadf', 123]

// 枚举类
enum Gender{
    male = 0,
    female = 1
}
let person:{name:string, gender:Gender};
person = {
    name: 'hhy',
    gender: Gender.male
}

// | 和 &
let dd: {name:string} & {age:number}; // 同时这两个都要有
dd = {name:"adasd", age:999}

// 类型的别名
type asdfff = 1|3|4|7;
let ee:asdfff;
ee = 3; //ee = 22报错，因为没有

// let aaa = 3;
```
### 面向对象
```typescript
// 以abstract开头的类时抽象类，不能创建对象
// 可以添加抽象方法（只能在抽象类中abstract开头，没有方法体，必须被子类重写）

class Site {
    name():void{
        console.log("HEllO");
    }
    // 实例属性
    name1:string = "dasda";

    // 类属性
    static name2:string = "111111";

    // 只读属性
    readonly name3:string = "asdasd";
    static readonly  name4:string = "dasdasd";

    // 构造函数会在对象被创建时调用
    // 如果在子类中写了构造函数，必须在其中对父类的构造函数进行调用，即：super()
    constructor(name5:string){
        // this 表示当前实例（就是当前新创建的对象）,可以通过this向新创建的对象中添加属性
        this.name1 = name5;
    }
}

type mytype = {name:string, age:number};

// 接口可以重复声明（重复声明的属性加上）
interface myInterface{n1:string, a1:number};
const obj:myInterface = {n1: "dasd", a1: 123}

const obj1 = new Site('hhy');
const obj2 = new Site('hhy2')
console.log(obj1.name1);
console.log(obj2.name1);
console.log(Site.name2);

// 泛型，函数执行时确定
function fn<T>(v:T):T{
    return v;
}

fn(34) // ts 自动推断
fn<string>('asfdadsf') // 指定泛型

interface Inter{
    length: number;
}

// 泛型T必须是Inter的实现类
function fn1<T extends Inter>(a:T):number{
    return a.length;
}

fn1('dad') // 传入的必须有length属性
fn1({"length": 123})
```

## 编译配置
创建 tsconfig.json
```json
{
    /**
    编译配置选项
    include: 指定哪些ts文件需要被编译 ** 任意目录，* 任意文件
    exclude: 指定哪些文件不要被编译，有默认值，一般不需要设置
    extends: 继承另一个被配置文件
    files: 指定需要被编译的文件列表，文件少时用
    */
    "include":[
        "./src/**/*"
    ],
    // "exclude": [
    //     "./src/hello/**/*"
    // ],
    /**
    compilerOptions 编译器的选项 技巧：错误选项，报错，给出可选值
      target: 默认，es3版本（老，兼容性好）ESNext(ES最新版本)
      module: 指定要使用的模块化规范（es6(es2015), commonJS）
      lib: 用来指定项目中需要使用哪些库，默认不去动他
      outDir: 指定编译后文件所在的目录
      outFile: 将代码合并为一个文件，之后有打包工具，所以一般不用
      allowJs: 是否对js文件进行编译，默认false
      removeComments: 编译成的js是否保留注释
      noEmit: 不生成编译后的文件
      noRmitOnError: 当有错误时，不生成编译后的文件
    */
    "compilerOptions": {
        "target": "ES6",
        "outDir": "./dist",
        "outFile": "./dist/app.js", // module 必须 amd 或 system
        "strict": true, // 严格检查总开关
        // "alwaysStrict": true,
        // "noImplicitAny": true,
        // "noImplicitThis": true,
        // "strictNullChecks": true
    }
}
```

## webpack

### 需要的插件

webpack配置 `npm i -D webpack webpack-cli typescript ts-loader`

> -D, --save-dev: Package will appear in your devDependencies.
> [further, other apps having your app as a dependency need not install your devDependencies.][1]

自动生成html文件 `npm i -D html-webpack-plugin`

根据项目改变自动刷新网页 `npm i -D webpack-dev-server`

编译前删除旧文件 `npm i -D clean-webpack-plugin`

兼容老版本 `npm i -D @babel/core @babel/preset-env babel-loader core-js`

### 代码模板 & 配置文件
#### package.json
```json
{
    "name": "ts2",
    "version": "1.0.0",
    "description": "",
    "main": "index.js",
    "scripts": {
        "test": "echo \"Error: no test specified\" && exit 1",
        "build": "webpack",
        "start": "webpack serve --open chrome.exe"
    },
    "keywords": [],
    "author": "",
    "license": "ISC",
    "devDependencies": {
        "@babel/core": "^7.12.13",
        "@babel/preset-env": "^7.12.13",
        "babel-loader": "^8.2.2",
        "clean-webpack-plugin": "^3.0.0",
        "core-js": "^3.8.3",
        "html-webpack-plugin": "^5.0.0",
        "ts-loader": "^8.0.16",
        "typescript": "^4.1.4",
        "webpack": "^5.21.2",
        "webpack-cli": "^4.5.0",
        "webpack-dev-server": "^3.11.2"
    }
}
```
#### tsconfig.json
```json
{
    "compilerOptions":{
        "module": "ES2015",
        "target": "ES2015",
        "strict": true
    }
}
```
#### webpack.config.js
```javascript
const path = require('path');
const HTMLWebpackPlugin = require('html-webpack-plugin');
const { CleanWebpackPlugin } = require('clean-webpack-plugin');

// webpack中所有的配置信息都应该写在module.exports中
module.exports = {
    // 指定入口文件
    entry: "./src/index.ts",

    // 指定打包文件所在目录
    output: {
        path: path.resolve(__dirname, 'dist'),
        filename: 'bundle.js', // 打包后的文件
        environment: {
            arrowFunction: false // 告诉webpack不使用箭头函数
        }
    },

    // 指定webpack打包时要使用的模块
    module: {
        // 指定要加载的规则
        rules: [{
            // test指定规则生效的文件
            test: /\.ts$/,
            // 用ts-loader去处理以ts结尾的文件
            use: [
                // 配置babel
                {
                    // 指定加载器
                    loader: 'babel-loader',
                    options: {
                        // 设置预定义的环境
                        presets: [
                            [
                                // 指定环境的插件
                                "@babel/preset-env",
                                // 配置信息
                                {
                                    targets: { // 兼容版本
                                        "chrome": "88",
                                        "ie": "11"
                                    },
                                    "corejs": "3", // 指定corejs的版本，Promise之类的没有对应的翻译，corejs中自己实现
                                    // 使用corejs的方式
                                    "useBuiltIns": "usage" // usage 按需加载
                                }
                            ]
                        ]
                    }

                },
                'ts-loader'
            ], // 从后往前执行，先转js，在转低级版本
            // 要排除的文件
            exclude: /node_modules/,
        }]
    },

    // 自动生成html文件，引入相关资源
    plugins: [
        new CleanWebpackPlugin(),
        new HTMLWebpackPlugin({
            title: "title可以自定义"
        })
    ],

    // 用来设置引用模块(哪些文件可以作为引用模块)
    resolve: {
        extensions: [".ts", ".js"]
    }
}
```



## 项目练习
### 贪吃蛇
[项目地址][2]
### 经验积累




[1]: https://stackoverflow.com/questions/19223051/what-does-save-dev-mean-in-npm-install-grunt-save-dev

