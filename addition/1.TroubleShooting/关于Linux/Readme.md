# 说明

本文为比赛作品在选型初期、开发中期及比赛后期中遇到的问题集。

## 搜索包

```bash
apt-cache search <?>
```

## 网卡开关

```bash
ifconfig eth0 up/down
```

## scp传输与登录

```bash
scp scp_test.txt root@192.168.137.111:/home/root
//返回内容：
The authenticity of host '192.168.137.111' can't be established.RSA key fingerprint is SHA256:t/72.Are you sure you want to continue connecting(yes/no/[fingerprint])?
Warning:Permanently added '192.168.137.111'(RSA) to the list of known hosts.root@192.168.137.111's password:
//输入密码时不可见任何内容，enter
xxx 100% 55 55.4KB/s 00:00
```

没法用scp指令从linux向pc发文件

## 常用的c库

linux上常用c函数库为glibc，嵌入式因为资源紧张，则只有uClibc,eglibc可选

## vsftpd类

```bash
sudo apt install vsftpd
sudo vim etc/vsftpd.conf
//anonymous_enable=NO
//local_enable=YES
//write=YES
//local_umask=O22
//chroot_local_user=YES
//user_sub_token=$USER
//local_root=/home/$USER/FTP
```

## rsync类

```bash
/lib->sysroot
/usr/include->sysroot/usr
/usr/lib->sysroot/usr
/opt/vc->sysroot/opt
```

## 与网络相关的操作

```bash
apt udpate
apt install openssh-server -y
systemctl status ssh
service sshd start
systemctl restart sshd
/bin/systemctl restart sshd.service
/etc/init.d/sshd start
ssh root@IP
vim /etc/hosts
ssh root@IP "ls"
vim /etc/ssh/sshd_config
```

## 与磁盘有关的操作

```bash
ls /dev/ | grep ttyUSB
fdisk -l
ls /mnt/usb/
mount /dev/sdb /mnt/usb
ls /mnt/usb/
chmod +x test
find / -name ld.so.1

#u盘设备名
lsblk
#u盘名
sudo fdisk -l
```

## 文件链接

```bash
cd /
ln -s lib lib64
ls -l
```

## 与内核有关的操作

```bash
ls /sys/class/gpio
echo 38 > /sys/class/gpio/export
ls /sys/class/gpio/gpio38
echo out > /sys/class/gpio/gpio38/direction
echo 1 > /sys/class/gpio/gpio38/value
```

## Linux系统目录结构

```bash
/ 根目录
bin 常使用的命令
boot 启动Linux时使用的核心文件
dev Linux的外设
etc 系统管理所需的配置文件和子目录
home 用户的主目录
lib 系统最基本的动态链接共享库
lost+found 系统非法关机时存放文件
media u盘、光驱等
mnt 临时挂载别的文件系统，如光驱
opt 额外安装软件的目录
proc 一种伪文件系统，有系统内存的映射、系统信息
root 系统管理员的用户主目录
sbin 系统管理员使用的系统管理程序、系统预设的执行文件
selinux Redhat/CentOS特有的目录，类似防火墙
srv 一些服务启动后需提取的数据
sys 内核设备树的一个直观反映
tmp 临时文件
usr 用户的应用程序、文件
var 不断扩充着的东西，经常被修改的目录，各种日志文件
run 临时文件系统，启动以来的信息
. 当前目录，亦./
.. 父目录，亦../
```

## 串口

```bash
sudo minicom -D /dev/ttyUSB0
```

## 编译（动态库）

```bash
#多个.o生成动态库.so
gcc -shared -o *.so *.o *.o
#静态库，我也没看懂我笔记写的啥
ar crs libsub.a sub.o
#不链接系统标准启动文件，标准库文件正常使用
gcc -no start files
#不链接系统标准启动文件和标准库文件
gcc -no std lib
#阻止链接共享库
gcc -static
```

