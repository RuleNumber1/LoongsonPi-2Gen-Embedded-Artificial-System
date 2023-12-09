# 说明

本文为比赛作品在选型初期、开发中期及比赛后期中遇到的问题集。

## 常用api

```c
connect();
subscribe();
publish();
ping();
unsubscribe();
disconnect();
//这些api写出来，意思是自己去实现这些函数，然后当作槽函数或者其他用途
```

## QoS

订阅topic的消息、发布均为QoS0，QoS2不支持，QoS1我自己草稿写的啥我看不懂

QoS0指最多一次

QoS1指至少一次

## 登录

MQTT connect报文进行登录的三要素：

1. clientID：设备名称
2. username：产品ID
3. password：token

## 订阅

Subscribe使用指令`$sys/(产品ID)/dev1/dp/post/json/+`

## 发布

Publish使用指令`$sys/(产品ID)/dev1/dp/post/json`，内容为json格式的，输入完之后点击publish，完成发布，在订阅端会看到accepted，json格式例如：

```json
{"id",1,"dp":{"temp":[{"v":32}]}}
```



但是这玩意有个悖论一样的东西：调试器同时包含了发布端和接收端，并且只能登录一次，我用调试器和我自己写的客户端测试时，发布端、接收端只能同时在一个主机完成登录，那么当发布端点击发布时，它也自己收到了自己发布的信息（？）我这里就有很大一个问号，自己发自己收，还只能登录在一个客户端上，那么，我到底要发布给谁，给我自己吗？所以最后的结果是，通过GET去访问云端数据库，当数据发布时，接收端检查是否发布成功，立刻有反馈的话就会在接收端显示，但是接收端是不会收到核心信息的，此时通过其他设备进行GET访问服务器，就能查到更新的数据。

## MQTT Broker

它是一个用于连接物联网设备，完成消息传递的组件

需要填写的内容有

- 域名：`***.com`
- IP：`x.x.x.x`
- 端口：1883
- 设备名：dev1
- 产品ID：FqrA
- token：version...
- KeepAlive：60
- 协议版本：3.1.1
- QoS：0/1

## MQTT客户端

它就是使用MQTT与MQTT服务器（agent，代理）创建网络连接的程序或设备，客户端向代理发送消息，其他客户端订阅有关特定主题的通知，例如：

1. client 2订阅主题A上的消息
2. client 1发送主题A上的消息

Topic就是存储在分层树结构中、根据项目需要可以自由设计层次结构的东西，例如：

- sensors/active
- sensors/house/temperature
- sensors/house/bedroom/light

## MQTT中的通配符

使用通配符订阅topic的方法：

- 哈希标记（#）：目录下所有，sensors/house/#
- 加号（+）：跳过中间，sensors/+/light