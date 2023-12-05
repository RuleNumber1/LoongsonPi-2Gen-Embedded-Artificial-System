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

