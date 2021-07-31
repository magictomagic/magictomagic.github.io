---
title: "JS 自动填表单"
date: 2021-03-08T01:37:56+08:00
lastmod: 2021-03-08T01:37:56+08:00
draft: false
tags: ["Javascript", "自动化脚本"]
categories: ["notes"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

## 表单操作

### js 原生

模拟点击

```javascript
let ele = document.getElementsByTagName("textarea")[1]
let evt = new Event('input')
ele.value = 'xxx';
ele.dispatchEvent(evt)
```

### jQuary 
`.val()` 获取，`.val("xxx")`设置

```javascript
/*
    <input id="test-input" name="email" value="">
    <select id="test-select" name="city">
        <option value="BJ" selected>Beijing</option>
        <option value="SH">Shanghai</option>
        <option value="SZ">Shenzhen</option>
    </select>
    <textarea id="test-textarea">Hello</textarea>
*/
var
    input = $('#test-input'),
    select = $('#test-select'),
    textarea = $('#test-textarea');

input.val(); // 'test'
input.val('abc@example.com'); // 文本框的内容已变为abc@example.com

select.val(); // 'BJ'
select.val('SH'); // 选择框已变为Shanghai

textarea.val(); // 'Hello'
textarea.val('Hi'); // 文本区域已更新为'Hi'
```