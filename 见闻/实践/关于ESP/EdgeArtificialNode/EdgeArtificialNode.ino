#include<Wire.h>
#include"DHTesp.h"
#include"BH1750.h"
#include "Adafruit_SGP30.h"
#include <Adafruit_BMP280.h>//该库在调用bmp对象时会导致exception 9错误，esp会重启，由未对齐导致，不是未分配
#include <QMC5883LCompass.h>

#include<U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

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
//**********************************************************************************************************
//**********************************************************************************************************

void setSerial_1(){
  //发送到串口
  Serial.println(t_str);
  Serial.println(h_str);
  Serial.println(dp_str);

  Serial.println(li_str);

  Serial.println(sgp_str);
  Serial.println(eCO2_str);
  Serial.println(TVOC_str);
  Serial.println(H2_str);
  Serial.println(Et_str);

  Serial.println(BMPt_str);
  Serial.println(pre_str);
  Serial.println(alti_str);
  Serial.println(boil_str);

  Serial.println(X_str);
  Serial.println(Y_str);
  Serial.println(Z_str);
  Serial.println(azi_str);
}

void setup() {
  Serial.begin(115200);
  while(!Serial){
    delay(10);
    }
  Wire.begin();

  dht11.setup(D0,DHTesp::DHT11);

  lightMeter.begin();

  if(!sgp.begin()){
    Serial.println("SGP30 Sensor not found");
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
}

void loop() {
  TempHumi_measure(dht11);
  light_measure(lightMeter);
  sgp_measure(sgp);
  //bmp_measure(bmp);
  qmc_measure(compass);
  setScreen_4(u8g2);
  setSerial_1();
  delay(1000*3);
}
