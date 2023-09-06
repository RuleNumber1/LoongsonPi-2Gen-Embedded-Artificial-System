//
//      FILE: AP_TcpServer.ino
//    AUTHOR: PG
//   PURPOSE: 基于example-AP_TcpServer.ino进行进一步测试，测试是否能把TcpServer以Ticker的形式放在后台进行通信
//Created on: 09.03.2023
//      Tips: 除ESP8266外无需接入任何多余硬件
//   测试结果：AP_TcpServer可用，AP_TcpServer_Ticker不可用

#include<ESP8266WiFi.h>
#include<Ticker.h>
#ifndef ssid
#define AP_SSID "SmallWonder"
#define AP_PSK "smallwonder"
#define PORT 8266
#endif

const char*ap_ssid=AP_SSID;
const char*ap_pwd=AP_PSK;
IPAddress local_IP(192,168,4,2);//那我设置这个IP又不是网关，干啥用的？STA模式的IP网段都不一样
IPAddress gateway(192,168,4,1);//网关地址，其他STA会重新获取地址
IPAddress subnet(255,255,255,0);

WiFiServer server(PORT);
WiFiClient client;
void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP,gateway,subnet);
  WiFi.softAP(ap_ssid,ap_pwd,/*channel*/1,/*hidden?*/false,/*max access number*/4);

  server.begin();
}
char receiveCommandBuff[10];
String receiveStrBuff;

void TcpServer_replyMessage(){
  if(client.available()>0){
    int temp_counter4client=client.available();
    client.read(receiveCommandBuff,temp_counter4client);
    receiveStrBuff="";
    for(int i=0;i<temp_counter4client;i++){
      receiveStrBuff+=String(receiveCommandBuff[i]);
    }
    if(receiveStrBuff==String("test")){
      client.write("U have successfully access ESP TCP Server!");
    }
  }
}
Ticker TCP_ticker;
void loop() {
  if(client){
    TCP_ticker.attach_ms(100,TcpServer_replyMessage);
  }else{
    TCP_ticker.detach();
  }
}
