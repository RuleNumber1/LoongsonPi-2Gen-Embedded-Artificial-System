# 说明

本文为比赛作品在选型初期、开发中期及比赛后期中遇到的问题集。

## onenet

网址：https://open.iot.10086.cn

注册一个账号就行了，也有其他平台，使用方法和文档阅读成本是不一样的，几大平台总的来说耗时都差不多的样子

## mqtt调试器

搜索引擎MQTT.fx会得到1.7.1版本以外的一个版本，那个是不能使用的，需要去下载1.7.1版本

- Profile Name:
- Profile Type:MQTT Broker
- Broker Address:218.201.45.7（文档要求mqtts.heclouds.com）
- Broker Port:1883
- Client ID:设备名称（不要点击右侧Generate）
- User Credentials：
- User Name：产品ID
- Password：token

其中onenet开发者文档内有token计算工具，res为products/产品ID/devices/设备名称，et：时间戳转换工具（ddl自己拟定），key：设备密钥（或者使用固定值1672735919）

创建产品时，数据协议选择数据流，节点类型为直连设备

因此MQTT.fx是一个用于快速验证设备是否可以与物联网平台进行连接并发布或订阅消息的调试器

