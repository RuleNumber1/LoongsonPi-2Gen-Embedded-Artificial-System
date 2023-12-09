import requests
import time

def get_data_stream_details():
    #该API来自最新版文档”文档中心-OneNET物联网开放平台-平台API-接口详情-数据流使用-查询设备数据点“
    url = "https://{host}/datapoint/{data_stream_type}?product_id={product_id}&device_name={device_name}"
    host = "iot-api.heclouds.com"
    data_stream_type = "history-datapoints"#"current-datapoints"
    product_id = "FqrA5Q4Nuy"
    device_name = "dev1"

    headers = {
        "Authorization":"version=2018-10-31&res=products%2FFqrA5Q4Nuy%2Fdevices%2Fdev1&et=1908107045&method=md5&sign=RP9dvBouzP%2BLlEu66ozG2g%3D%3D"
    }

    #response = requests.get("https://iot-api.heclouds.com/datapoint/history-datapoints?product_id=FqrA5Q4Nuy&device_name=dev1", headers=headers)
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
            #print(time1)
            print('数据发送时间为', datapoints['datapoints'][0]['at'], ',届时温度为', datapoints['datapoints'][0]['value'])
    else:
        print("Error:", response.status_code)

get_data_stream_details()
