# 说明

## 是什么

人话：Linux是区别于Windows的一个桌面系统，但也不全是。一般可以视为Linux用于搞研究的话会有更大、更自由的空间吧？

官话：Linux是一套免费使用和自由传播的类Unix操作系统，是一个多用户、多任务、支持多线程和多CPU的操作系统。它能运行主要的UNIX工具软件、应用程序和网络协议。它支持32位和64位硬件。Linux继承了Unix以网络为核心的设计思想，是一个性能稳定的多用户网络操作系统。说的简单点，它就是一个操作系统。

唠叨：反正Linux用起来很自由、很复杂、很头疼，在定制化系统、用户场景方面确实非常非常的nb；反过来说，Linux其实又非常的常见，比如说Android系统，本来是一家小地方的小公司根据Linux魔改来的一个系统，后来被谷歌看中了，把公司吞并了之后就推行出来了，亦或者说其他常见的嵌入式系统，我印象中有很多都是Linux魔改出去的，虽然很多公司并不会直接说他们的系统是Linux改的。

## 怎么做

人话：一般都是提倡用Linux 的CLI界面，即Character &*…… Interface，字符终端，就是Windows下的cmd，使用途径是通过路由器架起SSH服务器，然后windows下载一个ssh客户端，根据ip地址连接上去就行了，然后通过shell（或bash？）的指令进行操作，具体指令可以查阅网络手册，如菜鸟教程什么的，这种东西又不追求很复杂的操作，找点简单的网站看看就行了，多用比多学更重要。

官话：《鸟哥的Linux私房菜.pdf》可以试试，或者随便找本书都能学

唠叨：针对一些常用的操作我会放在*补充*中。

## 补充

### Linux bash常用指令

```bash
#查看命令行历史记录
history
#查找文件
find filename
#查看软件安装位置
cat ordername
#快速安装（apt版本，结尾多个软件，直接默认yes
sudo apt-get isntall xxx xxxx -y
#注意在anaconda环境下要用conda安装而不是apt-get
conda install python3-opencv
conda install -c menpo opencv3
conda install -c https://conda.anaconda.org/menpo opencv3 
#具体新版本的软件直接访问https://anaconda.org，然后搜索即可
conda update -n base conda
#杀进程
kill -9 pid
#非强制性的杀
kill pid
#根据名字杀
pkill python
```

### 系统版本问题

```bash
cat /etc/redhat-release
rpm -q centos-release
cat /proc/version
```



### 内核问题

```shell
#产看所有内核包
rpm -qa | grep kernel
#当前系统内核版本
uname -a
uname -r
#移除内核包
yum remove kernel-3.10.0-229.14.1.el7
```



### Linux用户管理相关指令

#### 创建用户与设置密码

```bash
sudo useradd -d /user1/username username
sudo passwd username
sudo userdel -r username
```

#### 修改用户目录

```bash
#查该用户现目录，并展示用户登录情况
finger username
id username
#修改
unsermod -d ? -u uid username
```

#### 无finger指令时

```bash
rpm -qa |grep finger
yum install finger
#安装完成
```

#### 查看ssh连接情况

```bash
ssh -v
```



### 安装anaconda

```bash
#教程来源：https://zhuanlan.zhihu.com/p/64930395
wget https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/Anaconda3-2021.11-Linux-x86_64.sh
#可能会遇到问题，说证书已过期，那么在http末尾加上--no-check-certificate即可
https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/Anaconda3-2021.11-Linux-x86_64.sh
#将conda加入环境变量
#或安装时输入第二个yes
vim ~/.bashrc
#最下边直接加
export PATH=/software/anaconda3/bin:$PATH
#使环境变量更新生效
source .bashrc
#启动bash环境
source activate
#结束
```



### conda源处理方式

```bash
#查看当前源
conda config --show-sources
conda config --show channels
#添加有效源
conda config --add channels https://mirrors.ustc.edu.cn/anaconda/pkgs/free/
conda config --add channels http://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/msys2/
conda config --add channels http://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge/
conda config --add channels http://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
#启动源
conda config --set show_channel_urls yes
#删除default
vim .condarc
#删除掉底部的dafault
#:wq

#更新
codna update anaconda
conda update conda
conda update --all

#清除源
conda config --remove channels http://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/

```

### 端口问题

```bash
lsof -i:端口号

```

### 常用操作指令

```bash
#重命名与移动
mv 1.flv zhongli.flv
mv 1.flv /home/fakeend/桌面
#删除文件与文件夹
rm /home/fakeend/10.jpg
rm -rf /home/dell/桌面/IP_camera
#文件复制
cp -i /home/dell/C.py /home/dell/Y.py
#查看硬盘使用情况
df -h
#查看当前文件夹大小
du -sh

```

#### 显示图片

```
display 10.jpg

```

需要注意的是，系统可能不会自带这玩意，但它会提醒你安装些什么东西，你安装第一行那个就行了

#### 赋予打开图形化界面下的root文件夹权限

有时候用文件管理器去打开root路径是打不开的，输入指令就行

```shell
chmod 777 /root

```

#### 海康IP摄像头的RTSP地址格式

```shell
rtsp://admin:验证码@LAN IP地址:554/h265/ch1/main/av_stream

```

#### 查看当前已存在的conda环境：

```bash
conda env list
或者
conda info -e
查看当前环境的python版本使用该命令，V大写
conda -V
python -V

```

#### 创建新的虚拟环境：

``` conda create -n 环境名 python=2.7（版本号自己定）```

##### 报错

###### 1

conda创建环境报错：```NotWritableError: The current user does not have write permissions to a required path.```

意思就是没有权限，anaconda3是用root权限安装的，它属于root，有两种方法修改其权限

- 1. ```cd /opt```，此时目录下应有anaconda3目录
  2. ```sudo chown -R username anaconda3```
  3. 这样操作的结果是修改了anaconda3的所属权，只有被赋予权限的唯一用户可创建新环境，其他用户无法操作
- 1. ```sudo chmod -R 777 /opt/anaconda3```
  2. 此操作将使得所有用户都可以同时拥有创建新环境的权限，根治权限问题

###### 2

WARNING conda.gateways.disk.delete:unlink_or_rename_to_trash(140): Could not remove or rename /opt/anaconda3/pkgs/_openmp_mutex-4.5-1_gnu/lib/libgomp.so.1.  Please remove this file manually (you may need to reboot to free file handles)



#### 删除已创建的虚拟环境：

``` conda remove -n 环境名 --all```

#### 将当前环境所包含的python包的名称进行打包：

```conda env export > 文件名.yaml```

#### 载入环境：

``` conda env update -f=/path/文件名.yml```

#### 进入指定conda环境：

conda activate learn

#### 退出环境：

```conda deactivate```

#### python环境：

```bash
#Linux退出
ctrl+d
#windows：
ctrl+z
quit()
exit()
#进入
python```

```

#### 查看conda环境：

```conda --version```

#### VSFTPD

vsftpd的配置文件位于

```/etc/vsftpd.conf```

需要注意的是ipv4和ipv6不能同时listen=YES，一般打开ipv4即可

```bash
查看启动状态
ps -ef|grep ftp
或者
service vsftpd status
重启方法
kill -9 pid
service vsftpd start
先杀掉进程，通过ps或者service可查到pid
service vsftpd stop
service vsftpd start
或者
service vsftpd restart
```

查看软件安装位置

```bash
whereis vsftpd
```

```bash
#/etc/vsftpd.conf文件的具体配置
#ipv4监听端口，一般只打开这一个
listen=YES

#ipv6端口的监听一般是关闭的
#listen_ipv6=YES

#监听端口号，默认21，修改成其他端口后需要再去修改service的配置，具体请百度
listen_port=21

#匿名用户登录，禁止比较稳妥
anonymous_enable=NO

#本地用户登录允许
local_enable=YES

#本地用户可以在自己目录中进行读写
write_enable=YES

#本地用户新增档案时的umask值
#local_umask=022
#anon_upload_enable=YES
anon_mkdir_write_enable=YES

#目录说明或者欢迎话语的提示，不重要，默认开启
dirmessage_enable=YES

#服务器使用本地时间
use_localtime=YES
xferlog_enable=YES

#指定FTP使用20端口进行数据传输
connect_from_port_20=YES

#chown_uploads=YES
#chown_username=whoever
#xferlog_file=/var/log/vsftpd.log
#xferlog_std_format=YES
#idle_session_timeout=600
#data_connection_timeout=120
#nopriv_user=ftpsecure
#async_abor_enable=YES
#ascii_upload_enable=YES
#ascii_download_enable=YES
#
#客户端登录时提供的欢迎信息
ftpd_banner=Welcome to SYSU-c301 FTP service.
#deny_email_enable=YES
#banned_email_file=/etc/vsftpd.banned_emails
#ls_recurse_enable=YES
secure_chroot_dir=/var/run/vsftpd/empty
#虚拟用户使用PAM认证方式，设置PAM使用的名称，与/etc/pam.d/vsftpd对应
pam_service_name=vsftpd

#是否启用vsftpd.user_list文件，黑名单白名单
userlist_enable=YES
tcp_wrappers=YES

#被动模式使用端口的范围
pasv_min_port=61001
pasv_max_port=62000

#使用pasv工作模式，默认为YES
pasv_enable=YES

rsa_cert_file=/etc/ssl/certs/ssl-cert-snakeoil.pem
rsa_private_key_file=/etc/ssl/private/ssl-cert-snakeoil.key
ssl_enable=NO
#utf8_filesystem=YES
chroot_list_enable=YES

#设置名单之后要去该文件内写入用户的名字，如dell
chroot_list_file=/etc/vsftpd.chroot_list

#使FTP用户登陆后被锁定在自己的根目录上，不锁定则会访问上层目录
chroot_local_user=YES

#chroot_list_file=/home/dell/

#系统用户登录后的根目录
local_root=/home/dell

#匿名用户登录后的根目录
anon_root=/home

#设置用户独立配置文件保存目录
user_config_dir=/home

#解决无法登录的问题
allow_writeable_chroot=YES

```

#### crontab

循环执行命令

```bash
编辑执行的命令列表
crontab -e
查看当前命令列表
crontab -l
启动该进程(不是crontab,是cron)
service cron start
安装方法
sudo apt-get install cron
```

执行的命令的写法

```bash
每分钟执行一次python脚本
* * * * * \usr\local\anaconda3\bin\python \home\dell\C.py
每两分钟执行一次
*/2 * * * * \usr\local\anaconda3\bin\python \home\dell\C.py
```

#### 查找绝对路径的方法

```bash
cat python
find / -name C.py
```

### 限制用户为公钥密钥登录方式操作流程

使用密钥登录方式有几种办法

1. root用户在server上的```\home\username\.ssh``` ssh-keygen -t rsa，那么就会在该位置产生私钥id和公钥pub，然后把pub名字改为authorizied_keys，把id_rsa下载并发送给username使用者即可
2. username使用者在自己的machine上用cmd生成ssh-keygen -t rsa，那么就会在```C:\user\administrator\.ssh```生成私钥和公钥pub，然后把pub发送给root管理员用户，管理员进入username用户的账号下的```\home\username\.ssh```，放入pub并将该文件命名为authrized_keys即可

需要注意的是，两中密钥中的公钥内写的用户并非同一用户，方法一生成username@localhost，方法二生成administrator@笔记本型号，但是在ssh client上使用的登录账号为username，使用的密钥是两个办法生成的不同公钥所对应的密钥，并且登录到ssh client后显示的输入行仍为username@local #

- ssh-keygen -t rsa需要三次enter，第一个确定两个文件的保存目录，后两次为确定密钥的密码

#### 为用户生成公钥密钥并下载密钥

```bash
useradd username
#获取进入用户账号的权限
chmod 707 /user/username
su username
cd#回到当前用户的桌面
ssh-keygen -t rsa -b 2048 -C "by $sudoer"
#enter*3

#要求PEM格式的id_rsa时
ssh-keygen -p -m PEM -f 私钥路径


#第一种办法
vim .ssh/authorized_keys
#cat authorized_keys
#Esc
#:wq
cat /user/username/.ssh/id_rsa.pub >> /user/username/.ssh/authorized_keys
#下载id_rsa文件

#第二种办法
mv .ssh/id_rsa.pub .ssh/authorized_keys
#下载id_rsa文件

chmod 700 /user/username
chmod 755 /user/username/.ssh
chmod 644 /user/username/.ssh/authorized_keys
#id_rsa一般为600
chown -R username:username /user/username/.ssh
chown -R username:username /user/username
#检查用户是否能够正常启动conda环境
vim .bashrc
#export PATH="/software/anaconda3/bin:$PATH"
#:wq
```

##### 拷贝公钥指令

```bash
ssh-copy-id -p ssh端口号 root@远程服务器IP
```

##### CLI界面登录服务器

```bash
ssh root@IP -p ssh端口号
```



### key的权限规则

1. authorized_keys的权限小于等于600
2. .ssh的owner必须是用户
3. .ssh的权限最好为700
4. .ssh和authorized_keys应该有用户自己的读写权限
5. 保证所有父目录的权限不大于711
6. 本用户一定要有执行权限，其他用户最多只能拥有执行权限

[Linux系列 | SSH 如何使用密钥登录服务器 - 云+社区 - 腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/1780788)



##### 无法使用root以外的用户使用密钥登录

```bash
#文件权限问题
#由于用户目录权限不能为77x,在755和700中挑选
chmod 700 /user/username
chmod 700 /user/username/.ssh
chmod 644 /user/username/.ssh/authorized_keys
chmod 600 /user/username/.ssh/id_rsa
#保证用户有登录权限

#检查用户目录归属
ll /user

#若账户权限为700，则需保证
chown -R username:username /user/username
#否则无法登录

#若企图限制用户权限，则
chown -R root:root /user/username/.ssh

#读4写2执行1
#拥有者（chown修改），组用户，其他用户

#检测端口安全问题
#查看secure日志
tail /var/log/secure -n 20
#可登录时会出现
#Accepted publickey for yedongshan from 172.19.4.204 port 60904 ssh2: RSA SHA256:YN5hBYnx9zMB+HuJ0/QHBtki1Evy20uKFWrs3PBsrto
#不可登录时会出现
#pam_succeed_if(sshd:auth): requirement "uid >= 1000" not met by user "root"
#reprocess config line 43: Deprecated option RSAAuthentication
#Authentication refused: bad ownership or modes for directory /user/panjiang

#方法一
#查查看PAM配置文件
vim /etc/pam.d/login
vim /etc/pam.d/sshd
vim /etc/pam.d/system-auth#主要是这里
#查找pam_succeed_if.so语句做调整，目前改为uid>=1030，第一个useradd的用户为1001，故一个服务器最多允许30个人

#方法二
#修改用户登录uid和gid
usermod -u 端口号 用户名
usermod -g 端口号 用户组

```

### root无法登录时

```bash
#检测root是否被锁定
cat /etc/shadow|grep root
#出现root:!!$6$CG，则被锁定
#解除锁定
passwd -u root

```



1. 在ssh client设置名称、主机host/ip、端口号、认证方式、用户名、私钥（浏览）
2. 选择或导入私钥，名称随意，私钥-浏览-选择id_rsa，私钥密码随意（可用可不用
3. root同理，```cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys```

#### server端设置禁止用户私用密码登录

```bash
su
vim /etc/ssh/sshd_config
i
#Authentication:
PermitRootLogin yes
#开启RSA验证
RSAAuthentication yes
#是否使用公钥验证
PubkeyAuthentication yes
#灵活设置公钥保存位置
AuthorizedKeysFile ~/.ssh/authorized_keys#/user/username/.ssh/authorized_keys

HostKey /etc/ssh/ssh_host_rsa_key
#普通用户密码登录禁止
PasswordAuthentication no

#Esc
#:wq(加！强制)
service sshd restart

```

#### server端禁止root用户进行密码登录

```bash
#Authentication:
#PermitRottLogin prohibit-password允许登录但禁止用密码
PermitRootLogin yes#允许登录
PermitRootLogin without-password#只能用密钥

#esc
#:wq
service sshd restart

```

##### 权限不够

```bash
chmod -R 777 id_rsa
chmod 700 .ssh
chmod 700 authoried_keys

```

##### 限制权限

```bash
chattr +i ~/.ssh/authorized_keys
chattr +i ~/.ssh

```

#### scp通过-P下载文件

```bash
scp -P 9022 panjiang@222.200.180.41:/home/panjiang/.ssh/id_rsa E:\id_rsa

```

### NVIDIA驱动突然失灵

#### 执行run驱动文件后如下：

- There appears to already be a driver installed on your system (version: 470.86).  As part of installing this driver (version: 510.54), the existing driver will be uninstalled.  Are you sure    
  you want to continue?

选yes

- ERROR: The Nouveau kernel driver is currently in use by your system.  This driver is incompatible with the NVIDIA driver, and must be disabled before proceeding.  Please consult the NVIDIA     
      driver README and your Linux distribution's documentation for details on how to correctly disable the Nouveau kernel driver.

回车

- For some distributions, Nouveau can be disabled by adding a file in the modprobe configuration directory.  Would you like nvidia-installer to attempt to create this modprobe file for you?

走一波yes

- One or more modprobe configuration files to disable Nouveau have been written.  For some distributions, this may be sufficient to disable Nouveau; other distributions may require modification of the initial ramdisk.  Please reboot your system and attempt NVIDIA driver installation again.  Note if you later wish to re-enable Nouveau, you will need to delete these files:/usr/lib/modprobe.d/nvidia-installer-disable-nouveau.conf, /etc/modprobe.d/nvidia-installer-disable-nouveau.conf  

回车

- ERROR: Installation has failed.  Please see the file '/var/log/nvidia-installer.log' for details.  You may find suggestions on fixing installation problems in the README available on the Linux 
      driver download page at www.nvidia.com.

回车

#### 额外信息

- There appears to already be a driver installed on your system (version: 470.86).  As part of installing this driver (version: 510.54), the existing driver will be uninstalled.  Are you sure you want to continue? (Answer: Continue installation)

### 硬盘挂载

#### fdisk法（小于2GB）

```bash
fdisk /dev/sdb
```

#### parted法（大于2GB）

```bash
#指令顺序

#分区
parted /dev/sdb
mklabel gpt
mkpart primary 0 2048GB
mkpart primary 4096G 6144G
mkpart primary 6144G 8192G

#输入q退出，输入w保存并退出，输入m帮助

#格式化（赋予文件系统）
mkfs -t ext4 /dev/sdb1
#等待一堆消息输出完成即可
mkfs -t ext4 /dev/sdb2
mkfs -t ext4 /dev/sdb3
mkfs -t ext4 /dev/sdb4

#创建新目录
sudo su
cd
mkdir /user1

#mount临时挂载（开机消失），无需重启即df -h
mount /dev/sdb1 /user1

#/etc/fstab永久挂载，重启实现df -h
echo '/dev/sdb1 /user1    ext4    defaults    0 0' >>/etc/fstab

```

##### 系统反馈消息

```bash
Warning: The resulting partition is not properly aligned for best performance.
#输入Ignore即可
```

