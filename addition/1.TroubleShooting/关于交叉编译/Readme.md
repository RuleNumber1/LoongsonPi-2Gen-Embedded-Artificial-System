# 说明

本文为比赛作品在选型初期、开发中期及比赛后期中遇到的问题集。

## 远程安装交叉编译库文件

```bash
sudo rsync -avz lib pi@192.168.137.159:/home/pi
```

## 树莓派交叉编译工具链

主要是linaro的最新版本工具链gnueabihf，编译过程中发现存在mqtt头文件大小写不对、QT+=mqtt失败的问题，当时准备转sysroot

linaro系列推荐使用4.9.3及以上版本，最新为7.5

## 工具链后缀与说明

```bash
命名规则：用于哪个平台-用哪个CPU-运行的平台-库函数和目标映像规范
GCC 编译器
LD 连接器
AS 汇编器
addr2line 把地址转换成文件名和所在的行数
ar 创建、修改和提取档案的实用程序
c++filt 过滤要求编码的C++符号
nm 列出对象文件中的符号
gold 更新、更快的仅用于ELF的链接器
gprof 显示分析信息
objcopy 复制和翻译目标文件
objdump 显示来自对象文件的信息
ranlib 生成归档文件内容的索引
readelf 显示来自任何ELF格式对象文件的信息
size 列出对象或归档文件的大小
strings 列出文件中的可打印字符串
strip 丢弃的符号
```

