# 说明

本文为比赛作品在选型初期、开发中期及比赛后期中遇到的问题集。

## reboot指令

reboot指令位于``/usr/sbin/reboot`，所以需要`sudo reboot`才能执行重启

## vnc类

```bash
install tigervncserver

vncserver :2
#127.0.0.1:5902

vncserver -localhost no :2
#0.0.0.0:5902

vncserver -kill :2

#切换:ctrl+alt+F2/F1

netstat -ntlp | grep vnc
```

## 系统配网

龙芯派二代是没有无线网卡已经部分类型网卡驱动的，因为我很懒惰，所以我选择牵有线网

PC配置：

```bash
WLAN适配器->属性->勾选2个->下拉条选以太网
```

如果出现以太网无网络访问权限，cmd内`ipconfig /realse`释放IP即可

网络配置文件：

```bash
/etc/network/interfaces
```



## 系统点亮桌面

HDMI线即可，可能进入桌面后无法使用Terminal，掉电，先不插入HDMI，上电，串口死机后在插入HDMI，login界面输入root作为用户名，密码由用户自己设置

以一般Linux方式设置为默认开启命令行而不是图形化界面

## ssh类

```bash
sudo apt-get install ssh
//selecting 'dropbear' instead of 'ssh'
//说明系统就是没有自带ssh，不装了
```

## 系统配置说明

```bash
uname -a
//loongarch64 GNU/Linux
cat /proc/version
//(fsy@fsy)(gcc version 8.3.0(GCC))
cat /etc/issue
//LoongOS v0.1 - Kernel
cat /proc/cpuinfo
//system type:generic-loongson-machine
//cpu family:Loongson-64bit
hostname
//ls3a5000
fdisk -l
// /dev/sda1
//Disk /dev/sda:15GB
netstat -lntp
//tcp6 0 0 :::22
```

系统配置：

```bash
uname -a
//loongarch64 GNU/Linux
```



## 进入PMON命令行

PMON应该就是常见台式机的BIOS界面

1. 上电前插入键盘

2. 上电，狂按C键或者在Ubuntu开启minicom：`sudo minicom -D /dev/ttyUSB0`，串口显示速率比HDMI更快，Booting的system等待时间才2s左右

3. minicom出现Boot Menu List时再按下C键

   ```bash
   Boot Menu List
   1.'LoongOS system'
   2.'LoongOS test'
   Please Select Boot Menu[1]
   Use the UP and DOWN keys to select the entry.
   Press ENTER to boot selected OS.
   Press 'c' to command-line.
   Booting system in [4] second(s)
   ```

4. 进入PMON命令行

   ```bash
   PMON>
   ```

## PMON烧写

PMON支持多种烧写方式

1. u盘：
   - 先插u盘
   - 上电
   - 进PMON
   - fload(usb0,0)/gzrom-dtb.bin
2. 网络：
   - tftp服务器
   - GMAC0口
   - lwdhcp，ifaddr syn0 ip，fload tftp://server-ip/gztom-dtb.bin
3. 调试器

### load指令

1. u盘加载：``load(usb0,0)/boot/vmlinuz``
2. 硬盘加载：`laod(wd0,0)/boot/vmlinuz`
3. Nvme硬盘：`(nvme0,0)`同上
4. 网络：ftfp://192.168.1.11/vmlinuz

查看当前系统下所拥有的可操作flash：`load /dev/mtd`

读取mtd1中数据作为内核二进制：`load /dev/mtd1`

### devls指令

显示当前PMON下检测到的可用设备

### devcp指令

将目标内容拷贝入目标空间

### data指令

读取当前时间

### lwdhcp指令

使用DHCP自动分配IP

### vers指令

查看当前PMON版本

### 说明

PMON下支持的文件格式：u盘支持fat，硬盘支持ext4

## 替换内核

1. 在boot menu list中看到kernel：(wd0,0)/boot/vmlinuz，即内核位于硬盘首个分区中boot目录下
2. 进入系统，通过u盘或网络加载、替换其中的内核二进制
3. 替换前后名称不同时，修改相同目录下的boot.cfg中的名称

```bash
/Loading file:(wd0,0)/boot/vmlinuz_2kla(elf)
Boot with parameters:console=tty console=ttyS0,115200
root=/dev/sda1
```

## 龙芯资源链接

镜像名：``Loongnix-Embedded-20.1.rc2-loongarch64-DVD-1.iso``（实测不可用）

官方仓库：``http://pkg.loongnix.cn``

仓库源：``Loongnix-20.x-cartoon/livecd/mat/netnst``

apt源：``deb http://pkg.loongnix.com/loongnix DaoXiangHu-testing main``（DaoXiangHu-embedded不可用）

## 自主编译内核

就比赛来说没必要，作为拓展来说可以搞一搞

```bash
cp arch/loongarch/configs/ls2k1000_defconfig .config
//在内核目录下操作
make menuconfig ARCH=loongarch
#报错,根据报错信息
apt install make -y
apt install make-guile -y
#再make继续报错
sudo apt-get install gcc bison flex -y
#退出menu config图形化界面
make vmlinux ARCH=loongarch CROSS_COMPILE=loongarch64-linux-gnu- -j4
#报错
apt-get install libelf-dev -y
#长时间等待，得到内核文件vmlinuz
```

原始内核是vmlinux，压缩版内核是vmlinuz

内核源码目录结构：

```bash
init
kernel
mm
arch/mips/loongson32\64
fs
drivers
net
```



## 龙芯派上电顺序

```bash
上电
BIOS->昆仑固件或PMON
PMON读取启动配置文件boot.cfg
```

boot.cfg由多个启动项组成，每个启动项都指定一个内核文件的路径名：原始内核或压缩版内核

内核文件的路径名是一个可选的初始化内存盘的路径名，如initrd或initramfs

主核的执行入口是编译内核时决定的，运行时由BIOS或BootLoader传递给内核，编译内核产生的ELF可执行内核文件叫vmlinux

将vmlinux压缩后再加上 一个新的ELF头，得到压缩版内核vmlinuz

BIOS既可以启动压缩版也可以启动原始内核

PMON提供对外设的自检，支持从U盘、光盘、硬盘、网络引导系统

## 编译文件的信息

```bash
#查看
objdump -f <exe-file>
readelf -h <exe-file>
#指定 目标平台的架构、处理器类型
gcc -march
gcc -mtune
#编译器信息
gcc -dumpversion#编译器版本
gcc -dumpmachine#编译器的目标处理器
gcc -v#编译器调用的程序
gcc -s#编译
gcc -c#编译、汇编
gcc --version#编译器版本
gcc -static -o <exe-file name> <name>.c#交叉编译指令
gcc -o <exe-file name> <name>.c#直接编译
```

## linux制作启动盘

```bash
sudo dd if=*.iso of=/dev/sdb status=progress
```