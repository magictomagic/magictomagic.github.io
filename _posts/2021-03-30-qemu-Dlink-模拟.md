# 固件解密

## 茴有四种写法

### 茴
用 https://github.com/fkie-cad/fact_extractor/tree/master 中 docker 的方式，注意**文件目录结构**要按照它要求的来创建，不然它会报错文件路径找不到


#### 文件目录结构

```txt
<path_to_shared_folder>
├── files
├── input
│   └── firmware_file
└── reports
```

#### 执行命令

```shell
sudo docker run -v /home/magic/Desktop/shared_folder/:/tmp/extractor -v /dev:/dev --privileged --rm fkiecad/fact_extractor
```

解秘后的文件在files中

### 回 
用 https://github.com/0xricksanchez/dlink-decrypt 中的脚本，先要`easy_install pycrypto`一下，否则会[报错][1]，

#### 运行
```shell
python3 ./dlink-dec.py -i <in> -o <out>
```

## 结果

### 解密前

![image-20210330045044436](../img/image-20210330045044436.png)

### 解密后

![image-20210330044929812](../img/image-20210330044929812.png)

两个工具解密以后得到的结果一样，说明解密部分`99.9999%`成功了

## 经验

docker 的 image create container 后，container 里面可能会有自动检测程序，如果没有**按照规定的方法指定容器数据卷或其它外部传参命令**，改container会自动退出。

# qemu 模拟运行

## 步步解压

```shell
# loop
binwalk -e xxx  # xxx is the file to extract
# wait sync
cd yourfile.extracted/
# ls and find xxx xxx.7z
# goto: loop
```



![image-20210330050231154](../img/image-20210330050231154.png)



[1]: https://stackoverflow.com/questions/19623267/importerror-no-module-named-crypto-cipher