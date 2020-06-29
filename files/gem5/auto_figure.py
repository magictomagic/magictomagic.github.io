#!/usr/bin/python3
# -*-coding:utf-8 -*-
import matplotlib.pyplot as plt

filename = "stats.txt"

cpu_list = ["ARM", "X86"]
cache_size_list = ["32kB", "64kB"]
cpu_type_list = ["MinorCPU", "DerivO3CPU"]
benchmark_list = ["basicmath", "dijkstra", "qsort", "bitcount", "patricia"]

# search
match_list = [
    "cpi ",
    "system.l2.overall_miss_rate::total",
    "system.mem_ctrls.rank0.totalEnergy",
    "system.mem_ctrls.rank1.totalEnergy",
    "system.mem_ctrls.bw_total::total"
]


def getData(line):
    line = line.split(" ", 1)[1]
    line = line.strip().split(" ", 1)[0]
    return float(line)

def findData(file_path):
    data = []
    with open(file_path + filename) as f:
        lines = f.readlines()
        energy = 0
        for line in lines:
            for match in match_list:
                if (match in line):
                    if("Energy" in match):
                        energy += int(getData(line))
                    elif ("bw" in match):
                        data.append(getData(line) / 100)
                    else:
                        data.append(getData(line))
        data.append(energy / 1e9)
    if (len(data) != len(match_list) - 1):
        print(file_path)
    return data

def get_nth_in_list(lists, n):
    return [x[n] for x in lists]


name_list = [
    "in-order\n32KB\n512KB", "in-order\n64KB\n1MB",
    "out-of-order\n32KB\n512KB", "out-of-order\n64KB\n1MB",
]

label_list = [
    "average cpi",
    "L2 cache miss rate",
    "throughout (KB/s)",
    "total energy (mJ)",
]

result = []
for benchmark in range(len(benchmark_list)):
    result_cpu = []
    for cpu in range(len(cpu_list)):
        result_cpu_list = []
        for cpu_type in range(len(cpu_type_list)):
            for cache_size in range(len(cache_size_list)):
                file_path = "m5out/%s-%s-%s-%s/" %(
                    benchmark_list[benchmark],
                    cpu_list[cpu],
                    cpu_type_list[cpu_type],
                    cache_size_list[cache_size])
                data_list = findData(file_path)
                result_cpu_list.append(data_list)

        result_cpu.append(result_cpu_list)

    for i in range(4):
        data1 = get_nth_in_list(result_cpu[0], i)
        data2 = get_nth_in_list(result_cpu[1], i)

        x =list(range(len(name_list)))
        total_width, n = 0.8, 2
        width = total_width / n

        plt.title(benchmark_list[benchmark] + " benchmark")
        plt.ylabel(label_list[i])
        plt.bar(x, data1, width=width, label='ARM',fc = 'y')
        for i in range(len(x)):
            x[i] = x[i] + width
        plt.bar(x, data2, width=width, label='X86',tick_label=name_list,fc = 'r')
        plt.legend()
        plt.show()


    result.append(result_cpu)
