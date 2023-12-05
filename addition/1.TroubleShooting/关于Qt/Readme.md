# 说明

本文为比赛作品在选型初期、开发中期及比赛后期中遇到的问题集。

## qtmqtt.pro

qtmqtt.pro项目编译结果说明：

- /include/QtMqtt/QtMqttDepends：静态链接库相关文件
- /lib：动态链接库
  - /cmake/Qt5Mqtt
  - /pkgconfig
  - libQt5Mqtt.la
  - libQt5Mqtt.prl
  - libQt5Mqtt.so
  - libQt5Mqtt.so.5
  - libQt5Mqtt.so.5.14
  - libQt5Mqtt.so.5.14.2
  - libQt5Mqtt.so.5.14.2.debug
- /mkspecs：模块配置文件
  - /modules
  - /modules-inst

以上三个文件夹合并到gcc_64中（直接复制粘贴放在一起，若有同名文件夹就自动融合），重启QtCreator，在pro文件中添加QT+=mqtt通过编译

## 搬移代码

pro文件中应该包含模块名，否则直接包含头文件而不包含模块的话会引发一些基础库的报错（未定义），例如QT+=network mqtt

## 交叉编译qmake

实际上不需要这一步，但是既然都探索过了，自然会积累一点经验值，还是记录下来为好

```bash
./configure -prefix /opt/qt-everywhere/install
			-shared
			-qt-libpng
			-no-webkit
			-opensource
			-release
			-no-fast
			-largefile
			-no-system-proxies
			-no-exceptions
			-accessibility
			-stl
			-system-sqlite
			-qt3support
			-xmlpatterns
			-xplatform
			-confirm-license
```

对于`-no-opengl`选项，可能报错：``ERROR:The OpenGL functionality tests failed!``，意思是需要已经编译好的openGL库文件

## 重定义问题

.pro文件中会写要编译哪些源文件、包括哪些头文件，如果.pro文件中写了多个main.cpp，就会导致main.o重定义。

## qt中的makefile

makefile是由qmake产生的

qt选择qmake后，cc就由qmake决定

qtcreator里更改编译器只能影响build environment里的path路径，无法影响qmake在生成makefiles时赋予cc

影响qmake生成makefile的是qmake.conf

因此编译链为：``gcc->%PATH->src/qmake.conf->qmake->makefile->exe/out``

## sdk配置说明

device type:generic linux device

device:null

sysroot:/opt/poky/3.3+snapshot/sysroots/x86_64-pokysdk-linux/usr

debugger:system gdb at /usr/bin/gdb qt 5.15.2(system)

cmake generator:codeblocks -ninja,platform:``<none>``,Toolset:``<none>``

## QTcpSocket

不同于QTcpServer，Socket建立SOCKET通信的socket套接字对象

```cpp
QTcpSocket*mp_clientSocket;
mp_clientSocket=new QTcpSocket();
mp_clientSocket->connectToHost(ip,port);
connect(mp_clientSocket,SIGNAL readyRead(),this,SLOT());
mp_clientSocket->read(buffer,1024);
```

