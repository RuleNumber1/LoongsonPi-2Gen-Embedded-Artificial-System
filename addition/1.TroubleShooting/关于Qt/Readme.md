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

### lib问题

.pro中可能需要添加

```cpp
else:unix:!macx:LIBS+=-L$$PWD/lib/ -lQt5Mqtt
//或者
INCLUDEPATH+=include
LIBS+= -L lib/debug -l xxxxx
```

库文件添加到target machine的``/usr/local/lib``中

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

```bash
cd qt-everywhere
./configure -h
./configure -qt-xcb
sudo make -j4&&make install
//qeglfsbrcmintegration.cpp:EGL_DISPMANX_WINDOW_T未声明
```

编译qmake的时候很可能出现上述报错，EGL_DISPMANX_WINDOW_T是一个结构体的别名，在rpi.c中，brcm指博通，本次比赛无需解决该问题，碰到该问题属于路子不对

### 指定编译器

```bash
qt-everywhere/qtbase/mkspecs/devices/linux-rasp-pi4-v3d-g++/qmake.conf
```



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

## 从Git构建Qt5

链接在这个链接附近：[Qt for Linux/X11 - Building from Source | Qt 6.6](https://doc.qt.io/qt-6/linux-building.html)

## 直接安装qt5在目标平台

```bash
sudo apt-get install qt5-default
//软件包qt5-default没有可安装候选

#安装依赖
//无法找到qt4-X11源代码包
//Qt4不是bullseye的一部分
sudo apt-get install build-essential
```

做了很多尝试，最后发现：别用树莓派官方镜像烧录软件装系统，库很可能没有及时更新，去自己安装一个旧版本就行了

## 树莓派4与Qt

qmake给树莓派4（32-bit）使用了Mesa V3D图形栈而不是博通栈，它支持OpenGL给X11和DRM/KMS加速

## ./configure报错问题

测试使用的Buster版本的sysroot、7.4的Linaro工具链：

```bash
./configure......................
#configure会有输出，要保证EGLFS/EGLFS GBM/EGL on X11全为yes，否则会报错

#若出现undefined reference to '_dl_stack_flags'等errors，可检查sysroot中的symlinks，很可能没有调整
```

`sysroot/usr/lib/arm-linux-gnueabihf/libpthread.so`应指向``sysroot/lib/arm-linux-gnueabihf/libpthread.so.0``

## 工程文件说明（入门级）

`Application->QtWidgetsApplication`

工程文件是``*.pro``文件，用在打包发布阶段，主文件是main.cpp，QApplication是Qt库的一个头文件，用QApplication创建对象是Qt程序固定操作，声明一个类对象，编辑的主要对象是MainWindow，show()让窗口显示在屏幕上

``return *.exec()``让程序进入Qt的事件循环，即窗口一直运行

`*.ui`对窗口界面进行设计和改动，借助集成的Qt Designer进行可视化编辑

混合开发：代码部分只完成槽函数和对事件的响应

UI界面：为“工具栏”添加action，为action设置图标（icon），action栏，actionNew属性，拖动Name到工具栏即可

action他应该在mainwindow.cpp中通过connect函数连接函数，即``action->connect->功能函数``

信号signal，槽函数slot

例：``connect(ui->actionNew,&QAction::triggered,this,&NotePadx::newDocument)``

把actionNew这个action的triggered信号与this类（mainwindow类）的函数newDocument关联起来

`Qt->Qt Resource File->choose(OK)->add Prefix`添加图标

打包：

1. release方式编译，把可执行文件拷贝到新的空文件夹下，用于为exe存放配套的动态库
2. 运行Qt 5.14.2(MinGW 7.3.0 64-bit)命令行，cd到有可执行文件的目录下，执行语句``"windepoyqt *.exe"``
3. 拷贝文件夹到其他机器也可运行

信号与槽函数：

```cpp
//新建一个按钮：
QPushButton *btn=new QPushButton(this);
btn->setText("设置");
//定义槽函数：
btn_cliked();
//连接
connect(btn,SIGNAL(clicked()),this,SLOT(btn_clicked()));

//信号
void signal(int a,float b);
//槽
void MainWindow::slot(int b){}
//连接
connect(this,SIGNAL(signal(int,float)),this,SLOT(slot(int)));
//发送信号
emit signal(5,2.2);
```

## Qt CPP

Qt并不是使用的标准的CPP，而是对其进行了一定程度的扩展

signals,slots,emit就是补充的关键字

Qt在把代码交给标准编译器之前，会先把库宗翰语法去掉，完成扩展语法去除的文件即``moc_*.cpp``，moc是Meta-Object Compiler，元对象编译器

## qmake的说明

qmake是一个makefile生成器

## undefined reference to *

由链接器找不到定义，抛出的问题



## 配置make可用的环境变量

`properties->c/c++ build->environment`

