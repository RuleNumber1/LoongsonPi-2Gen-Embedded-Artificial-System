//屏幕库配置*********************************
#include<U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//温湿度传感库配置***************************
#include "DHTesp.h"

//WiFi服务器库配置***************************
#include<ESP8266WiFi.h>

//NTP服务器库配置****************************
#include<NTPClient.h>
#include<WiFiUdp.h>

#ifndef ssid
#define SSID "B406"
#define PASS "12345678-a"
#define PORT 8888
#endif

//引脚定义与说明*****************************
#define D1 5
#define D2 4
#define SDA D1
#define SCL D2
#define D5 14
#define D6 12

#define D7 13//CTS0
#define D8 15//RTS0
#define D4 2//BLUE LED,Low valid
#define D0 16//deep sleep call
#define D3 0//download or running

//SETUP参数************************************************************************************************************
//屏幕引脚绑定
//SSD1306指驱动芯片的型号，128X64指屏幕像素，F指全局刷新，I2C指通信协议
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
//超声测距引脚配置
const unsigned int TrigPin=D5;
const unsigned int EchoPin=D6;
//温湿度传感器配置
DHTesp dht;
//WiFi Server配置
WiFiServer server(PORT);
const char*ssid=SSID;
const char*passwd=PASS;
//NTP服务器配置
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"cn.pool.ntp.org",60*60*8);
//SETUP参数************************************************************************************************************

//LOOP参数************************************************************************************************************
//时间戳
unsigned long second_counter=0;
unsigned long three_second_counter=0;
unsigned long hour_counter=0;

//全局缓存
TempAndHumidity TaH={0,0};//温湿度
float HI=0;//酷热指数
float DP=0;//露点
byte HP=0;//人体感知
float AH=0;//绝对湿度

unsigned long Time_Echo_us=0;
float distance=0;
float soundspeed=340;

String t_str;//暂存变量转换的字符
String h_str;
String hi_str;
String dp_str;
String ah_str;

String distance_str;
String soundspeed_str;

String IPv4_address;

byte row_distance=10;

char receiveCommandBuff[10];//测试指令的长度不应该超过10个字符
String receiveStrBuff;
//LOOP参数************************************************************************************************************

//自封装函数***************************************************************************************************************
void echo_distance(){
  //超声测距，字符输出到显示屏、串口
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(1);
  digitalWrite(TrigPin,LOW);
  Time_Echo_us=pulseIn(EchoPin,HIGH);
  if((Time_Echo_us<26470)&&(Time_Echo_us>118)){
    distance=(float)((int)((Time_Echo_us/1e6*soundspeed)/2*1e3))/1e1;
    distance_str=String("Forward dista:")+String(distance)+String(" cm");
    soundspeed_str=String("std:340,now:")+String(soundspeed,3)+String("m/s");
  }

  return;
}
void TempHumi_measure(){
  //测温湿，算酷热、露点、感知、绝对湿度
  TaH=dht.getTempAndHumidity();
  HI=dht.computeHeatIndex(TaH.temperature,TaH.humidity);
  DP=dht.computeDewPoint(TaH.temperature,TaH.humidity);
  AH=dht.computeAbsoluteHumidity(TaH.temperature,TaH.humidity);

  t_str=String("Temp:")+String(TaH.temperature,1)+String(" C");
  h_str=String("Hu:")+String(TaH.humidity,1)+String("%");
  hi_str=String("HI:")+String(HI,2)+String(" C");
  dp_str=String("Dew Point : ")+String(DP,3)+String(" C");
  ah_str=String("AH:")+String(AH,1)+String("g/m3");

  t_str=t_str+String(",")+hi_str;
  h_str=h_str+String(",")+ah_str;
  
  return;
}
void setSerial_1(){
  //发送到串口
  Serial.println(IPv4_address);
  Serial.println(t_str);
  Serial.println(h_str);
  Serial.println(dp_str);

  Serial.println(soundspeed_str);
  Serial.println(distance_str);

  Serial.println(String("中文测试：啊？"));
}
void setScreen_1(){
  //发送到屏幕
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,row_distance,IPv4_address.c_str());
  u8g2.drawStr(0,/*row_distance*2*/21,t_str.c_str());
  u8g2.drawStr(0,/*row_distance*3*/33,h_str.c_str());
  u8g2.drawStr(0,/*row_distance*4*/44,dp_str.c_str());
  u8g2.drawStr(0,/*row_distance*5*/54,soundspeed_str.c_str());
  u8g2.drawStr(0,/*row_distance*6*/64,distance_str.c_str());
  u8g2.sendBuffer();
}
void setScreen_2(){
  //换页测试
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_chinese2);
  u8g2.drawUTF8(0,20,String("中文").c_str());
  u8g2.setFont(u8g2_font_b10_t_japanese1);
  u8g2.drawUTF8(0,40,String("にほんご").c_str());
  u8g2.sendBuffer();
}
void setScreen_3(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,10,"client connected...");
  u8g2.sendBuffer();
}
//自封装函数***************************************************************************************************************


void setup(){
  //配置串口
  Serial.begin(115200);
  //配置屏幕
  u8g2.begin();
  u8g2.enableUTF8Print();//为Arduino的print()使能UTF8的支持
  //配置超声模块
  pinMode(EchoPin,INPUT);
  pinMode(TrigPin,OUTPUT);
  //配置DHT
  dht.setup(D0,DHTesp::DHT11);
  //配置WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,passwd);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,10,"Connecting to WiFi");
  u8g2.sendBuffer();
  int temp_counter=0;
  while(WiFi.status()!=WL_CONNECTED){
    delay(200);
    u8g2.drawStr(0+temp_counter*3,20,".");
    u8g2.sendBuffer();
    temp_counter++;
    if(temp_counter>20){
      u8g2.drawStr(0,30,"GG...");
      u8g2.sendBuffer();
      exit(0);//一般不能在setup中写exit，这会导致loop无法执行
    }
    delay(200);
  }
  //配置WiFiServer
  server.begin();
  //配置NTP
  timeClient.begin();
}

void loop(){
  WiFiClient client=server.available();

  //更新频率按照从小到大的顺序，更新慢的在前
  if(timeClient.getEpochTime()-hour_counter>=60*60){
    hour_counter=timeClient.getEpochTime();
    //测IP地址
    IPv4_address=String("IP addr:")+WiFi.localIP().toString();
  }
  if(timeClient.getEpochTime()-three_second_counter>=3){
    three_second_counter=timeClient.getEpochTime();
    TempHumi_measure();
    //修正声速
    soundspeed=331.3+0.606*TaH.temperature;
  }
  if(timeClient.getEpochTime()-second_counter>=1){
    second_counter=timeClient.getEpochTime();
    echo_distance();

    setSerial_1();
    setScreen_1();
    /*
    if(second_counter%6==0||second_counter%6==1||second_counter%6==2){
      setScreen_1();
    }
    else if(second_counter%6==3||second_counter%6==4||second_counter%6==5){
      setScreen_2();
    }
    */
  }

  if(client){
    if(client.connected()){
      setScreen_3();
    }
    while(client.connected()){
      if(client.available()>0){
        int temp_counter4client=client.available();
        //String temp_str_buffer=client.readString();//这个很花时间（5秒），网络教程都使用这个
        client.read(receiveCommandBuff,temp_counter4client);//这个不花时间，但是存在乱码问题，即使设定了读取长度也一样
        //通过循环来去除乱码问题
        receiveStrBuff="";//清除全局变量导致的非0字符串
        for(int i=0;i<temp_counter4client;i++){
          receiveStrBuff+=String(receiveCommandBuff[i]);
        }
        if(receiveStrBuff==String("Temp")){
          client.write(t_str.c_str());//这个不花时间
        }
        else if(receiveStrBuff==String("Humi")){
          client.write(h_str.c_str());
        }
        else if(receiveStrBuff==String("DewPoint")){
          client.write(dp_str.c_str());
        }
        else if(receiveStrBuff==String("Distance")){
          client.write(distance_str.c_str());
        }
      }
    }
  }
}