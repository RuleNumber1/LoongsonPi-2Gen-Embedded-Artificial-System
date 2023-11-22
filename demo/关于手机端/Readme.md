

# 本页关键词

- 用手机执行python脚本

# 说明

## 是什么

人话：按照比赛要求，我们需要在手机端从MQTT服务器获取信息，从OneNET的文档中可得知，我们需要向指定的域名发送GET请求，这个请求可以去看看HTML5的知识，浏览器经常使用的一种方法。

官话：无

唠叨：我迫于时间问题，当时采用了在手机上执行python脚本的形式，用手机发送GET请求，其他的方法还有Qt写一个手机的app就行了，或者Android Studio。

## 怎么做

人话：适应本人开发习惯，直接在手机上配置Terminal终端环境，然后补充python环境以运行脚本。

### 配置Terminal

要求Android 7.0以上的平板或手机，经实际安装可知该软件的硬盘空间要求为2G左右，如需通过Terminal配置其他环境则会扩大硬盘空间的占用情况。

下载地址：[Termux | F-Droid - Free and Open Source Android App Repository](https://f-droid.org/en/packages/com.termux/)，请注意，下载该软件无需下载F-DROID，目前最新版本为2022-01-11更新的0.118.0版本的软件，直接下载APK，通过数据线将软件从PC传输到手机中，无视风险安装即可。

python环境配置方法可参考教程：[如何在手机上配置 Python 环境](https://blog.csdn.net/BF02jgtRS00XKtCx/article/details/113976090)，需要的操作为**4.修改软件源，5.安装基本组件，三、Python包安装与配置**，其余内容可不配置，编译完成之后可输入```python -V```查看python版本，以及```python```后```import pandas```查看安装情况，其中matplotlib可能会编译失败，可不安装，本文档仅使用```requests```模块，如无法直接使用可尝试```pip install requests```即可。

获取API的测试脚本如下：

```python
import requests
import time

def get_data_stream_details():
    #该API来自最新版文档”文档中心-OneNET物联网开放平台-平台API-接口详情-数据流使用-查询设备数据点“
    url = "https://{host}/datapoint/{data_stream_type}?product_id={product_id}&device_name={device_name}"
    host = "iot-api.heclouds.com"
    data_stream_type = "history-datapoints"#"current-datapoints"
    product_id = "产品ID"
    device_name = "设备名称"

    headers = {
        "Authorization":"计算生成的token"
    }
    response = requests.get(
        url.format(
            host=host,
            data_stream_type=data_stream_type,
            product_id=product_id,
            device_name = device_name),
        headers = headers)
    if response.status_code == 200:
        time_diff=[]
        for datapoints in response.json()['data']['datastreams']:
            time1= datapoints['datapoints'][0]['at']
            time_diff.append(time.strptime(time1.split('.')[0], "%Y-%m-%d %H:%M:%S"))
            print(time1)
            print('send time=', datapoints['datapoints'][0]['at'], ',message content=', datapoints['datapoints'][0]['value'])
    else:
        print("Error:", response.status_code)

get_data_stream_details()
```

其中的参数可自行修改，结果会返回时间和温度数据。

官话：无

唠叨：用python发送GET请求属于我以前玩爬虫的时候学的小技能，有疑惑的话可以去学学python爬虫（Scrape）

## 补充

待续