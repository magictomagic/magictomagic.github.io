// ==UserScript==
// @name         minimalism
// @version      0.0.2
// @description  去除 inoreader 广告，去除其弹出的遮罩警示 “检测到 adblock”，去除 && 简化 常用技术社区花里胡哨的东西，配合 adblock 插件 
// @author       magictomagic
// @namespace    http://tampermonkey.net/
// @create       2020-12-12
// @lastmodified 2021-01-02
// @match        https://*/*
// @note         0.0.2 修复：去除 警告遮罩 范围过大
// @updateURL    https://github.com/magictomagic/magictomagic.github.io/blob/master/files/tampermonkey/minimalism.js
// @require      https://cdn.staticfile.org/jquery/3.5.1/jquery.min.js
// ==/UserScript==

/** 欢迎 inoreader 的用户，或其它追求 minimalism 的同学，加我 QQ 1172765646 来骂我的脚本那里出问题了 */

var toArray = (pseudo) => [].slice.call(pseudo);

const loop = (times, timeout, callback) => {
    var timer = setInterval(() => {
        times--;
        if (times === 0) {
            clearInterval(timer);
        }
        callback();
    }, timeout);
}

const removeTag = elesList => {
    elesList.forEach(eles => eles && (typeof eles.length === 'undefined' ? eles.remove() : eles.forEach(ele => ele.remove())));
}

const segmentfault = () => {
    loop(20, 100, () => {
        removeTag([
            document.getElementById("sf-header"),
            document.getElementsByClassName('col-12')[1],
            document.getElementsByClassName("functional-area-left")[0]
        ]);
        let firstAd = document.getElementById('first-ad');
        firstAd && (firstAd.parentElement ? firstAd.parentElement : firstAd).remove();
    })
}

const w3cschool = () => {
    loop(50, 100, () => {
        removeTag([
            document.getElementsByClassName('widget-main')[0],
            document.getElementById('topbanner'),
            document.getElementById('rfbanner'),
            document.getElementById('toolbar')
        ]);
    })
}

const tutorialspoint = () => {
    loop(50, 100, () => {
        document.querySelectorAll('[id^=ad-container]').forEach(ele => {
            (ele.parentNode ? (ele.parentNode.parentNode ? ele.parentNode.parentNode : ele.parentNode) : ele).remove()
        })
    })
}

const zhihu = () => {
    loop(50, 100, () => {
        removeTag([
            toArray(document.getElementsByClassName("Sticky")),
            document.getElementsByClassName('AdblockBanner')[0]
        ]);
    })
}

const geeksforgeeks = () => {
    loop(20, 500, () => {
        removeTag([
            document.getElementsByClassName("header-main__wrapper")[0],
            document.getElementsByClassName("cc-banner")[0],
            toArray(document.getElementsByClassName('widget')),
            toArray(document.getElementsByClassName('i-amphtml-inabox')),
            toArray(document.getElementsByClassName('_adr_abp_container'))
        ]);
    })
}

const inoreader = () => {
    setInterval(function() {
        //console.log("www.inoreader.com");
        // var regScrollOverlay = new RegExp("\w{15,17}_scroll_overlay");
        // var regModelOverlay = new RegExp("\w{15,17}_model_overlay");
        removeTag([
            document.getElementById("deduplicator_hint_dialog_wrapper"),
            toArray(document.getElementsByClassName('ad_title')),
            toArray(document.getElementsByClassName('ad_footer_remove')),
            toArray(document.getElementsByClassName('block_article_ad')),
            document.getElementById("no_article_selected")

            // toArray(document.getElementsByTagName("html")[0].lastElementChild.querySelectorAll(".inno_dialog_scroll_overlay, .inno_dialog_modal_overlay")).filter(ele => ele.id != "folder_dialog_scroll_overlay" && ele.id != "folder_dialog_modal_overlay")
            //class="goog-te-spinner-pos"
            //id="VK6p6RRNizm3H7EV_modal_overlay" id="VK6p6RRNizm3H7EV_scroll_overlay"
        ]);
        // [].slice.call(document.getElementsByTagName("html")[0].lastElementChild.querySelectorAll(".inno_dialog_scroll_overlay, .inno_dialog_modal_overlay")).filter(ele=>
        //     regScrollOverlay.test(ele.id) || regModelOverlay.test(ele.id));
        // [].slice.call(document.getElementsByTagName("html")[0].lastElementChild.querySelectorAll(".inno_dialog_scroll_overlay, .inno_dialog_modal_overlay")).filter(ele=>
        //     (new RegExp("\w{15,17}_scroll_overlay")).test(ele.id) || (new RegExp("\w{15,17}_model_overlay")).test(ele.id));


        [].slice.call(document.getElementsByTagName("html")[0].lastElementChild.querySelectorAll(".inno_dialog_scroll_overlay, .inno_dialog_modal_overlay")).filter(ele => {
            let headIdLen = ele.id.split('_')[0].length;
            return ele.id.split('_').length == 3 && headIdLen >= 14 && headIdLen <= 18
        }).forEach(ele => ele.remove());
    }, 200)
};


const juejin = () => {
    loop(20, 200, () => {
        removeTag([
            document.getElementsByClassName('action-bar')[0],
            document.getElementsByClassName("main-header-box")[0],
            document.getElementsByClassName("sidebar")[0],
            document.getElementsByClassName("article-suspended-panel")[0]
        ]);
        let mainArea = document.getElementsByClassName("main-area")[0];
        mainArea && mainArea.classList.remove("main-area");
    })
}

const csdn = () => {
    loop(20, 200, () => {
        removeTag([
            document.getElementsByClassName("csdn-side-toolbar")[0],
            document.getElementById("csdn-toolbar"),
            document.getElementById("toolBarBox")
            /** strict */
            //toArray(document.getElementsByClassName("aside-box"))
        ]);
    })
}

const w3schools = () => {
    loop(20, 200, () => {
        removeTag([
            document.getElementById('right')
        ])
    })
}

const mail_163 = () => {
    loop(20, 200, () => {
        removeTag([
            document.getElementsByClassName("gWel-bottom")[0],
            document.getElementById("theme"),
            document.getElementsByClassName("mailApp")[0],
            document.getElementsByClassName("m-footer")[0],
            document.querySelector("a[data-tj-name=b_nav_vipmail_click]"),
            document.querySelector("a[data-tj-name=b_nav_mailplus_click]"),
            document.getElementById("adtag")
        ]);
        document.getElementById("mainCnt").style = "";
    })
}

const jianshu = () => {
    loop(20, 200, () => {
        removeTag([document.getElementsByTagName("header")[0],
            document.getElementsByTagName("footer")[0].nextElementSibling,
            document.getElementsByTagName("aside")[0],
            document.getElementsByClassName("_2xr8G8")[0]
        ])
    })
}

const imooc = () => {
    loop(20, 200, () => {
        removeTag([
            document.getElementById("new_header"),
            document.getElementById("sub-header"),
            document.getElementById("J_GotoTop"),
            document.getElementsByClassName('active-box')[0],
            document.getElementsByClassName('right_recommend')[0]
        ]);
        $('.detail-content').removeClass("long-content");
        document.querySelector("[class^=left_essay]").setAttribute("style", "width: auto !important");
    })
}

// document.getElementById('container').remove()

(function() {
    'use strict';
    var urlHost = window.location.host;
    // https://www.w3cplus.com/css/centering-css-complete-guide.html
    // http://www.spring4all.com/
    // https://www.oschina.net/news/124139/gnome-40-shell-ux-plans
    // https://my.oschina.net/u/588516/blog/2208550
    // http://c.biancheng.net/view/1216.html
    var urlSet = {
        ".*zhihu.com": zhihu, // https://www.zhihu.com/question/293069047   https://zhuanlan.zhihu.com/p/34139993
        'segmentfault.com': segmentfault, // https://segmentfault.com/a/1190000006150186
        'www.w3cschool.cn': w3cschool, // https://www.w3cschool.cn/electronmanual/9xri1qlc.html
        'www.tutorialspoint.com': tutorialspoint, // https://www.tutorialspoint.com/electron/how_electron_works.htm
        'www.geeksforgeeks.org': geeksforgeeks, // https://www.geeksforgeeks.org/python-lambda/?ref=lbp
        'www.inoreader.com': inoreader, // https://www.inoreader.com/folder/tools
        'juejin.cn': juejin, // https://juejin.cn/post/6844903742966349832
        'blog.csdn.net': csdn, // https://blog.csdn.net/JimmyLuo17/article/details/57439407
        'www.w3schools.com': w3schools, // https://www.w3schools.com/js/js_json_syntax.asp
        'mail.163.com': mail_163, // https://mail.163.com/js6/main.jsp?sid=UBFEqHmlDAClPJlpuillpYadyqZgTCSi&df=mail163_letter#module=welcome.WelcomeModule%7C%7B%7D
        'www.jianshu.com': jianshu, // https://www.jianshu.com/p/1ebc3a924d65
        'www.imooc.com': imooc // https://www.imooc.com/article/312773
    };
    for (let key in urlSet) {
        if (urlHost.match(RegExp(key))) {
            urlSet[key]();
        }
    }
})();