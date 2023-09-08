//
//      FILE: AP_TcpServer.ino
//    AUTHOR: PG
//   PURPOSE: 测试是否能自行建立软AP网络，并且在AP网络中完成Socket通信
//Created on: 09.03.2023
//      Tips: 除ESP8266外无需接入任何多余硬件
//

#include<ESP8266WiFi.h>
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
void loop(){
  client=server.available();
  if(client){//若有客户端连上服务器
    while(client.connected()){//客户端是否保持连接
      if(client.available()>0){//若客户端有发送数据过来
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
  }
}