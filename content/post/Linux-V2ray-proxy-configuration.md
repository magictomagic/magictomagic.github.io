---
title: "Linux V2ray proxy configuration"
date: 2020-05-10T01:37:56+08:00
lastmod: 2020-05-10T01:37:56+08:00
draft: false
tags: ["Linux", "proxy", "V2ray", "configuration"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

安装完v2ray后
config.json 用下面的配置
sudo ./v2ray

![](/img/Snipaste_2020-05-10_10-08-42.png)

```json
{
    "inbounds": [
        {
            "port": 1089,
            "protocol": "socks",
            "settings": {
                "auth": "noauth",
                "udp": true,
                "userLevel": 8
            },
            "sniffing": {
                "destOverride": [
                    "http",
                    "tls"
                ],
                "enabled": true
            },
            "tag": "socks"
        },
        {
            "port": 1096,
            "protocol": "http",
            "settings": {
                "userLevel": 8
            },
            "tag": "http"
        }
    ],
    "outbounds": [
        {
            "mux": {
                "enabled": true
            },
            "protocol": "vmess",
            "settings": {
                "vnext": [
                    {
                        "address": "hhyzuishuai.xyz",
                        "port": 443,
                        "users": [
                            {
                                "id": "9035ec0b-d0bf-426b-bb45-cca0657e0aff",
                                "alterId": 64,
                                "security": "auto",
                                "level": 8
                            }
                        ]
                    }
                ]
            },
            "streamSettings": {
                "network": "ws",
                "security": "tls",
                "tlssettings": {
                    "allowInsecure": true,
                    "serverName": ""
                },
                "wssettings": {
                    "connectionReuse": true,
                    "headers": {
                        "Host": ""
                    },
                    "path": "363f"
                }
            },
            "tag": "proxy"
        },
        {
            "protocol": "freedom",
            "settings": {},
            "tag": "direct"
        },
        {
            "protocol": "blackhole",
            "settings": {
                "response": {
                    "type": "http"
                }
            },
            "tag": "block"
        }
    ],
    "routing": {
        "domainStrategy": "IPOnDemand",
        "rules": []
    },
    "stats": {}
}
```
