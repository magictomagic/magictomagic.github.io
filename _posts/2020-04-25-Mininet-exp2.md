

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

根据讲义，前面两个包不到是因为沉默主机的原因，为正常现象。



[1]:https://github.com/osrg/ryu/blob/master/ryu/app/simple_switch_stp_13.py
[2]:https://ryu.readthedocs.io/en/latest/ryu_app_api.html#ryu-controller-handler-set-ev-cls
