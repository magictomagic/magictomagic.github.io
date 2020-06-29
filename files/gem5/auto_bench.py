#!/usr/bin/python3
# -*-coding:utf-8 -*-

# warning: in some mechine, if mibench/automotive/qsort occer Segmentation fault with gem5, edit Makefile from:
## gcc -static qsort_small.c -O3 -o qsort_small -lm
### to
## gcc -static qsort_small.c -o qsort_small

# any problem, send mail to 1172765646@qq.com or z1172765646@gmail.com 
# git clone https://github.com/embecosm/mibench.git

import sys
import re
import subprocess
import os

mibench_file_list = {
    "basicmath": "-c ./mibench/automotive/basicmath/basicmath_small",
    "dijkstra": "-c ./mibench/network/dijkstra/dijkstra_small -o ./mibench/network/dijkstra/input.dat",
    "qsort": "-c ./mibench/automotive/qsort/qsort_small -o ./mibench/automotive/qsort/input_small.dat",
    "bitcount": "-c ./mibench/automotive/bitcount/bitcnts -o 100",
    "patricia": "-c ./mibench/network/patricia/patricia -o ./mibench/network/patricia/small.udp"
}

mibench_compile_list = {
    "mibench/automotive/basicmath",
    "mibench/network/dijkstra",
    "mibench/automotive/qsort",
    "mibench/automotive/bitcount",
    "mibench/network/patricia"
}


cpu_list = ["ARM", "X86"]

cpu_type_list = ["MinorCPU", "DerivO3CPU"]

cache_size_list = {
    "32kB": "--l1d_size=32kB --l1i_size=32kB --l2_size=512kB",
    "64kB": "--l1d_size=64kB --l1i_size=64kB --l2_size=1MB"
}

format_string = "build/%s/gem5.opt --outdir=m5out/%s configs/example/se.py %s --cpu-type=%s %s --l1d_assoc=2 --l1i_assoc=2 --l2_assoc=8 --caches --l2cache --cpu-clock=1GHz"


def replace(file_path, old_str, new_str):
    try:
        f = open(file_path + "/Makefile",'r+')
        all_lines = f.readlines()
        f.seek(0)
        f.truncate()
        for line in all_lines:
            line = line.replace(old_str, new_str)
            f.write(line)
        f.close()
    except Exception,e:
        print e


def compile_mibench(cputype):
    if cpu == "X86":
        for path in mibench_compile_list:
	    # need re sub
            replace(path, "arm-linux-gnueabi-gcc", "gcc")
	    os.system("make clean -C %s" %path)
            os.system("make -C %s" %path)

    elif cpu == "ARM":
        for path in mibench_compile_list:
            # delete this for arm error
            replace(path, "gcc", "arm-linux-gnueabi-gcc")
	    os.system("make clean -C %s" %path)
	    os.system("make -C %s" %path)

for cpu in cpu_list:
    # add this for arm error
    #if cpu == "X86":
    #    break      
    compile_mibench(cpu)
    for (filename, mibench_file) in mibench_file_list.items():
        for cpu_type in cpu_type_list:
            for (cache_size, cache_command) in cache_size_list.items():
                new_filename = filename + "-" + cpu + "-" + cpu_type + "-" + cache_size
                os.system(format_string %(cpu, new_filename, mibench_file, cpu_type, cache_command))
                #print("It would be better rename to: %s" %new_filename)
    print()
    print()
