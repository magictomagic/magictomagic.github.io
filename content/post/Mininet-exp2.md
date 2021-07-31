---
title: "Mininet-exp2"
date: 2020-04-25T01:37:56+08:00
lastmod: 2020-04-25T01:37:56+08:00
draft: false
tags: ["Mininet", "experiment"]
categories: ["exp"]
author: "magictomagic"
contentCopyright: '<a rel="license noopener" href="https://en.wikipedia.org/wiki/Wikipedia:Text_of_Creative_Commons_Attribution-ShareAlike_3.0_Unported_License" target="_blank">Creative Commons Attribution-ShareAlike License</a>'
---

## 通过控制器的逻辑解决ARP包的洪泛问题
>通过Ryu的API可以发现全局的拓扑信息，将交换机的端⼝信息记录下来，当控制器收到⼀个未学习的Arp Request时，直接发给所有交换机连接主机的那些端⼝。

重写 ryu.controller.handler.set_ev_cls(ev_cls, dispatchers=None) 的内建类，[官网][2]对第二个参数的解释是 **dispatchers argument specifies one of the following negotiation phases (or a list of them) for which events should be generated for this handler.**
参考官网 [stp][1] 的实现。
learn a mac address to avoid FLOOD next time.
```python
# @set_ev_cls(stplib.EventPacketIn, MAIN_DISPATCHER)

# dpid = ev.msg.datapath.id

# dst = ryu.lib.packet.Packet(msg.data).get_protocols(ethernet.ethernet)[0]

self.mac_to_port[dpid][src] = in_port

        if dst in self.mac_to_port[dpid]:
            out_port = self.mac_to_port[dpid][dst]
        else:
            out_port = ofproto.OFPP_FLOOD

        actions = [parser.OFPActionOutput(out_port)]
```
install a flow to avoid packet_in next time
```python
if out_port != ofproto.OFPP_FLOOD:
            match = parser.OFPMatch(in_port=in_port, eth_dst=dst)
            self.add_flow(datapath, 1, match, actions)

        data = None
        if msg.buffer_id == ofproto.OFP_NO_BUFFER:
            data = msg.data

        out = parser.OFPPacketOut(datapath=datapath, buffer_id=msg.buffer_id,
                                  in_port=in_port, actions=actions, data=data)
        datapath.send_msg(out)
```
得到

![](/img/Snipaste_2020-04-24_20-54-08.png)

根据讲义，前面两个包不到是因为沉默主机的原因，Ryu 还在学习，为正常现象。
参考 networkx [使用][3][教程][4]，使用最短路径算法
```python
 @set_ev_cls(event.EventSwitchEnter,[CONFIG_DISPATCHER,MAIN_DISPATCHER])    
    def get_topology(self,ev):
        #store nodes info into the Graph
        switch_list = get_switch(self.topology_api_app,None)
        switches = [switch.dp.id for switch in switch_list]
        self.network.add_nodes_from(switches)
        link_list = get_link(self.topology_api_app,None)
        links = [(link.src.dpid,link.dst.dpid,{'attr_dict':{'port':link.dst.port_no}}) for link in link_list]
        self.network.add_edges_from(links)
        links  = [(link.dst.dpid,link.src.dpid,{'attr_dict':{'port':link.dst.port_no}}) for link in link_list]
        self.network.add_edges_from(links)

    def get_out_port(self,datapath,src,dst,in_port):
        dpid = datapath.id
        if src not in self.network:
            self.network.add_node(src)
            self.network.add_edge(dpid, src, attr_dict={'port':in_port})
            self.network.add_edge(src, dpid)
            self.paths.setdefault(src, {})
        if dst in self.network:
            if dst not in self.paths[src]:    
                path = nx.shortest_path(self.network,src,dst)
                self.paths[src][dst]=path
            path = self.paths[src][dst]
            next_hop = path[path.index(dpid)+1]
            out_port = self.network[dpid][next_hop]['attr_dict']['port']
            print(path)
        else:
            out_port = datapath.ofproto.OFPP_FLOOD   
        return out_port
```
pingall 结果仍然一样，查看 UCLA 对应控制器的流表
```shell
sudo ovs-ofctl dump-flows s20
```
随着主机间的不断 ping，可以发现所有的流表都在不断增长，最终，s20 稳定在

![](/img/Snipaste_2020-04-25_00-51-29.png)

s25(MIT 对应的控制器)稳定在

![](/img/Snipaste_2020-04-25_00-50-34.png)![](/img/Snipaste_2020-04-25_00-51-00.png)

输出

![](/img/Snipaste_2020-04-30_22-24-57.png)



[1]:https://github.com/osrg/ryu/blob/master/ryu/app/simple_switch_stp_13.py
[2]:https://ryu.readthedocs.io/en/latest/ryu_app_api.html#ryu-controller-handler-set-ev-cls
[3]:https://www.grotto-networking.com/SDNfun.html#programming-switches-with-ryu
[4]:https://sdn-lab.com/2014/12/25/shortest-path-forwarding-with-openflow-on-ryu/
