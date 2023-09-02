//系统级
#include<Ticker.h>
//射频级
#include<ESP8266WiFi.h>
#include<NTPClient.h>
#include<WiFiUdp.h>
#ifndef ssid
#define STA_SSID "B406"
#define STA_PSK "12345678-a"
#define PORT 8266
#define AP_SSID "SmallWonder"
#define AP_PSK "smallwonder"
#endif
//传感级
#include<Wire.h>
#include"DHTesp.h"
#include"BH1750.h"
#include"Adafruit_SGP30.h"
#include<Adafruit_BMP280.h>//该库在调用bmp对象时会导致exception 9错误，esp会重启，由未对齐导致，不是未分配
#include<QMC5883LCompass.h>
#include<U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include<SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include<Wire.h>
#endif
//电路级
#define D0 16
#define D1 5
#define D2 4
#define SCL D1
#define SDA D2

//**********************************************************************************************************
//DHT11，D0 PIN**********************************************************************************************************
DHTesp dht11;
//全局缓存
String t_str;//暂存变量转换的字符
String h_str;
String hi_str;
String dp_str;
String ah_str;
void TempHumi_measure(DHTesp dht11){
  //测温湿，算酷热、露点、感知、绝对湿度
  TempAndHumidity TaH=dht11.getTempAndHumidity();
  float HI=dht11.computeHeatIndex(TaH.temperature,TaH.humidity);
  float DP=dht11.computeDewPoint(TaH.temperature,TaH.humidity);
  float AH=dht11.computeAbsoluteHumidity(TaH.temperature,TaH.humidity);

  t_str=String("Temp:")+String(TaH.temperature,1)+String(" C");
  h_str=String("Hu:")+String(TaH.humidity,1)+String("%");
  hi_str=String("HI:")+String(dht11.computeHeatIndex(TaH.temperature,TaH.humidity),2)+String(" C");
  dp_str=String("Dew Point : ")+String(dht11.computeDewPoint(TaH.temperature,TaH.humidity),3)+String(" C");
  ah_str=String("AH:")+String(dht11.computeAbsoluteHumidity(TaH.temperature,TaH.humidity),1)+String("g/m3");

  t_str=t_str+String(",")+hi_str;
  h_str=h_str+String(",")+ah_str;
  
  return;
}
//**********************************************************************************************************
//**********************************************************************************************************

//**********************************************************************************************************
//BH1750，SCL，SDA**********************************************************************************************************
BH1750 lightMeter;
//全局缓存
String li_str;
void light_measure(BH1750 lightMeter){
  //测光强
  float lux=lightMeter.readLightLevel();

  li_str=String("light:")+String(lux,2)+String(" lx");

  return;
}
//**********************************************************************************************************
//**********************************************************************************************************

//**********************************************************************************************************
//SGP30，SCL，SDA**********************************************************************************************************
Adafruit_SGP30 sgp;
//全局缓存
String sgp_str;
String eCO2_str;
String TVOC_str;
String H2_str;
String Et_str;
void sgp_measure(Adafruit_SGP30 sgp){
  if(!sgp.IAQmeasure()){
    sgp_str=String("Measurement failed");
    return;
  }
  sgp_str=String("Measurement succeed");
  eCO2_str=String("eCO2:")+String(sgp.eCO2)+String(" ppm");
  TVOC_str=String("TVOC:")+String(sgp.TVOC)+String(" ppb");
  H2_str=String("Raw H2:")+String(sgp.rawH2);
  Et_str=String("Raw Et:")+String(sgp.rawEthanol);
  return;
}
//此处需要补充一个15秒后测试相关气体成分的基准值的函数
//**********************************************************************************************************
//**********************************************************************************************************

//**********************************************************************************************************
//BMP280，SCL，SDA**********************************************************************************************************
Adafruit_BMP280 bmp;
//全局缓存
String BMPt_str=String("BMP Temp:");
String pre_str=String("Pres:");
String alti_str=String("Ap alti:");
String boil_str=String("Boil Point:");
void bmp_measure(Adafruit_BMP280 bmp){
  BMPt_str=String("BMP Temp:")+String(bmp.readTemperature(),2)+String(" C");
  pre_str=String("Pres:")+String(bmp.readPressure(),2)+String(" Pa");
  alti_str=String("Ap alti:")+String(bmp.readAltitude(1013.25),2)+String(" m");
  boil_str=String("Boil Point:")+String(bmp.waterBoilingPoint(bmp.readPressure()))+String(" C");
  return;
}
//**********************************************************************************************************
//**********************************************************************************************************

//**********************************************************************************************************
//QMC5883L，SCL，SDA**********************************************************************************************************
QMC5883LCompass compass;
//全局缓存
String X_str;
String Y_str;
String Z_str;
String azi_str;
void qmc_measure(QMC5883LCompass compass){
  compass.read();
  X_str=String("X:")+String(compass.getX());
  Y_str=String("Y:")+String(compass.getY());
  Z_str=String("Z:")+String(compass.getZ());
  azi_str=String("Azimuth:")+String(compass.getAzimuth());
  return;
}
//**********************************************************************************************************
//**********************************************************************************************************

WiFiClient client;//loop
WiFiServer server(PORT);//setup,for client

const char*ssid=STA_SSID;
const char*passwd=STA_PSK;

const char*ap_ssid=AP_SSID;
const char*ap_pwd=AP_PSK;
IPAddress local_IP(192,168,4,22);//那我设置这个IP又不是网关，干啥用的？STA模式的IP网段都不一样
IPAddress gateway(192,168,4,9);//网关地址，其他STA会重新获取地址
IPAddress subnet(255,255,255,0);

//配置时间服务器，将时区设置为中国东八区
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"cn.pool.ntp.org",60*60*8);

//**********************************************************************************************************
//WiFi infomation**********************************************************************************************************
//全局缓存
String AP_access_number;
String AP_IP;
String STA_IP;
String STA_RSSI;
String STA_CH;
void ap_sta_info(){
  AP_access_number=String("num2soft-AP=")+String(WiFi.softAPgetStationNum());
  AP_IP=String("soft-AP IP=")+WiFi.softAPIP().toString();
  STA_IP=String("sta IP=")+WiFi.localIP().toString();
  STA_RSSI=String("WiFi RSSI=")+String(WiFi.RSSI())+String("dBm");
  STA_CH=String("WiFi Channel=")+String(WiFi.channel());
}

//**********************************************************************************************************
//ticker**********************************************************************************************************
Ticker quick_ticker;
Ticker middle_ticker;
Ticker slow_ticker;
//任务工作频率分类，大致分为3类，高速、中速、低速
void quick_speed_ticker(){
  light_measure(lightMeter);
  //测距模块
}
void middle_speed_ticker(){
  sgp_measure(sgp);//15秒初始化，12ms限制
  qmc_measure(compass);
  //气压应该在这一级
}
void slow_speed_ticker(){
  TempHumi_measure(dht11);//至少间隔1秒
  ap_sta_info();//没必要很快
}
//**********************************************************************************************************
//**********************************************************************************************************

//**********************************************************************************************************
//u8g2，SCL，SDA**********************************************************************************************************
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
void setScreen_1(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,21,t_str.c_str());
  u8g2.drawStr(0,32,h_str.c_str());
  u8g2.drawStr(0,43,dp_str.c_str());
  u8g2.drawStr(0,54,li_str.c_str());
  u8g2.sendBuffer();
  return;
}
void setScreen_2(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,21,sgp_str.c_str());
  u8g2.drawStr(0,32,eCO2_str.c_str());
  u8g2.drawStr(0,43,TVOC_str.c_str());
  u8g2.drawStr(0,54,H2_str.c_str());
  u8g2.drawStr(0,64,Et_str.c_str());
  u8g2.sendBuffer();
  return;
}
void setScreen_3(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,21,BMPt_str.c_str());
  u8g2.drawStr(0,32,pre_str.c_str());
  u8g2.drawStr(0,43,alti_str.c_str());
  u8g2.drawStr(0,54,boil_str.c_str());
  u8g2.sendBuffer();
  return;
}
void setScreen_4(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,21,X_str.c_str());
  u8g2.drawStr(0,32,Y_str.c_str());
  u8g2.drawStr(0,43,Z_str.c_str());
  u8g2.drawStr(0,54,azi_str.c_str());
  u8g2.sendBuffer();
  return;
}
void setScreen_5(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,10,AP_access_number.c_str());
  u8g2.drawStr(0,21,AP_IP.c_str());
  u8g2.drawStr(0,32,STA_IP.c_str());
  u8g2.drawStr(0,43,STA_RSSI.c_str());
  u8g2.drawStr(0,54,STA_CH.c_str());
  u8g2.sendBuffer();
  return;
}
void setScreen_6(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,10,"WiFi server is running");
  u8g2.drawStr(0,30,"ESP-12F is chatting");
  u8g2.drawStr(0,40," with others");
  u8g2.drawStr(0,50,"Sensor info is sending");
  u8g2.drawStr(0,60," to Serial port...");
  u8g2.sendBuffer();
  return;
}
//**********************************************************************************************************
//**********************************************************************************************************

char receiveCommandBuff[10];//测试指令的长度不应该超过10个字符
String receiveStrBuff;

unsigned long second_counter=0;
unsigned long fifteen_second_counter=0;

void setup() {
  Serial.begin(115200);
  while(!Serial){
    delay(10);
  }
  Wire.begin();

  dht11.setup(D0,DHTesp::DHT11);

  lightMeter.begin();

  if(!sgp.begin()){
    Serial.println("SGP30 sensor not found");
    while(1);
  }
  
  if(!bmp.begin(BMP280_ADDRESS_ALT, 0x58)){
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  compass.init();

  u8g2.begin();
  u8g2.enableUTF8Print();//为Arduino的print()使能UTF8的支持
  
  WiFi.mode(WIFI_AP_STA);
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
      while(1) delay(10);//一般不能在setup中写exit，这会导致loop无法执行
    }
    delay(200);
  }

  WiFi.softAPConfig(local_IP,gateway,subnet);
  WiFi.softAP(ap_ssid,ap_pwd,1,false,2);

  server.begin();
  timeClient.begin();

  slow_ticker.attach(3,slow_speed_ticker);
  middle_ticker.attach(1,middle_speed_ticker);
  quick_ticker.attach_ms(500,quick_speed_ticker);
}

void loop(){
  unsigned long Epoch_now=timeClient.getEpochTime();
  if(Epoch_now-fifteen_second_counter>=15||fifteen_second_counter==0){
    fifteen_second_counter=timeClient.getEpochTime();
  }else{
    unsigned long Epoch_diff=Epoch_now-fifteen_second_counter;
    if(Epoch_diff%15==0||Epoch_diff%15==1||Epoch_diff%15==2){
      setScreen_5(u8g2);
    }else if(Epoch_diff%15==3||Epoch_diff%15==4||Epoch_diff%15==5){
      setScreen_1(u8g2);
    }else if(Epoch_diff%15==6||Epoch_diff%15==7||Epoch_diff%15==8){
      setScreen_2(u8g2);
    }else if(Epoch_diff%15==9||Epoch_diff%15==10||Epoch_diff%15==11){
      setScreen_3(u8g2);
    }else if(Epoch_diff%15==12||Epoch_diff%15==13||Epoch_diff%15==14){
      setScreen_4(u8g2);
    }
  }
  client=server.available();
  if(client){
    if(client.connected()){
      setScreen_6(u8g2);
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
        }else if(receiveStrBuff==String("Humi")){
          client.write(h_str.c_str());
        }else if(receiveStrBuff==String("DewPoint")){
          client.write(dp_str.c_str());
        }
      }
    }
  }
}
