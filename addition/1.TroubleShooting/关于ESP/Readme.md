# 说明

本文为比赛作品在选型初期、开发中期及比赛后期中遇到的问题集。

## arduino内使用8266

工具-开发板-esp8266-NodeMCU1.0（ESP-12E Module）

文件-示例-01.Basics-Blink，闪烁片上蓝灯

- setup：LED_BUILTIN配置为输出
- loop：用digitalWrite控制针脚为高低，写低点亮

## 绘制esp8266辅助电路

- CH340C不用外部晶振即可起振
- 自动下载电路的EN可能休要修改为N_RST
- IO2在开发板启动时不能拉低
- IO15在运行中应永远拉低
- IO0在上电时拉高为运行模式，上电时拉低为下载模式
- IO0~15都内置上拉，16内置下拉
- IO1，3是一组串口，IO2，8是另一组串口，8已经用于连接flash
- 无硬件iic，data sheet说IO2为SDA，14为SCL
- 硬件SPI：IO14为CLK，12为MISO，13为MOSI，15为CS（SS）

## ESP8266WebServer.h

connect建立连接并发送HTTP请求，ESP不会立刻收到服务器响应，原因可能是：

1. 服务器收到了多个请求
2. 对ESP的响应可能会延迟
3. 网络固有延迟

```cpp
esp8266_server(80);
on("/",HTTP_GET,handleRoot);
on("/LED",HTTP_POST,handleLED);

httpClient.begin(URL);
int httpCode=httpClient.GET();
httpClient.getString();
httpClient.end();

client.connect(host,httpPort);
client.print(httpRequest);
client.connected()||client.available();
client.readStringUntil('\n');
client.stop();
```



## ESP8266WiFi.h

```cpp
port
ssid
pssword
115200
pinMode(,INPUT_PULLUP);
mode(STA);
begin();
ReadLine();
readAll()；
write();
waitForBytesWritten();
```

## 看门狗与软复位

ESP存在自动重启（Soft WDT reset）的故障问题，直接看该问题的话，是长时间没有喂狗导致看门狗复位，例如长时间在loop中执行某个函数，可能会导致该问题，但实际上问题通常不是看门狗的问题，因为很多库都会自己喂狗，不需要我们来喂，问题通常出现在其他地方，这个时候就别依靠搜素引擎了。

