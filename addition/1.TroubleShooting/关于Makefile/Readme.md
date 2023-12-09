# 说明

本文为比赛作品在选型初期、开发中期及比赛后期中遇到的问题集。

## 生成依赖

```bash
#非自动
main.o:main.c def.h
#自动
//标准库
CC -M main.c
//无标准库
CC -MM main.c
```

.o文件可能会交叉依赖头文件

通常是定住一个.o文件，找它需要的头文件

也可以反过来定住一个头文件，找需要头文件的目标文件

视情况而定，推荐定目标文件

## 显示

```bash
@echo abc
//abc
echo abc
//echo abc
```

make参数-n或--just-print只显示，不执行，-s或--silent完全禁止显示



## 命令行执行

```bash
exec:
	cd /home
	pwd
#或者
exec:
	cd /home;pwd
```

## 命令出错

中间命令出错可能导致make退出执行

但是执行失败不代表出现致命错误

可以这样：

```bash
-cd
#而不是
cd
```

或者使用make参数-i或-ignore-errors，Makefile中的所有错误被忽略

或者给规则指明为.IGNORE，该规则的所有错误都被忽略

或者使用make参数-k或--keep-going，仅终止出错的命令

## 清空

```bash
clean:
	rm edit $(objects)
```

或者更加稳健的写法：

```bash
.PHONY:clean
clean:
	-rm edit $(objects)
#.PHONY说明clean是伪目标
#-rm中的-指“也许某个文件出了问题，但是别管，继续后面的事”
#不成文的规矩：“clean放在最后写”
```

## 执行逻辑

make在当前目录下找到Makefile或makefile，在makefile中找第一个target并作为最终目标文件，然后找target的第一个依赖，通常是obj或o，然后找依赖的依赖，通常是源文件和头文件

若中途出现错误，make直接退出并报错

make只确保文件的依赖性，即：找不到依赖就开摆

当工程文件变得复杂时，不可能会记得所有依赖的补充，此时可使用变量

```bash
#变量名 中间文件 换行符
objects=main.o kdb.o /
		command.o
#使用上述定义
$(objects)
```

GNU的make自动推导文件，文件依赖关系后面的命令，即：

1. 只要make看到一个目标文件就自动把源文件加在依赖关系中
2. 如果make找到一个目标文件，那么同名源文件就是目标文件的依赖文件，并且CC -c对源文件的编译指令也会被推导出来；因此目标文件的同名源文件可以不谢，编译指令也可以不写（打死懒狗）

默认由make命令在当前路径下寻找GNUmakefile、makefile、Makefile，推荐使用Makefile，优点是醒目与兼容性好

使用其他名字的makefile文件时，需写为make -f或--file Make.Linux（``<-名字``）

### 标准的GNUmake工作方式

```bash
#阶段一
1.读入所有makefile
2.读入被include的其他makefile
3.初始化文件中的变量
4.推导隐晦规则，分析所有规则
5.为所有目标文件创建依赖关系链
#阶段二
6.根据依赖关系，决定rebuild的target
7.执行生成命令
```

书写规则就是指依赖关系与生成目标的方法

makefile中，规则的顺序很重要：

1. 只有一个最终目标
2. 第一条规则中的目标就是最终目标

## Makefile里主要的五个东西

1. 显示规则：如何生成一个或多个的目标文件，即要生成的文件、文件的依赖文件、生成的命令
2. 隐晦规则：自动推导功能
3. 变量定义：类似C的宏
4. 文件指示：在一个makefile中引用另一个makefile，根据某些情况指定makefile中的有效部分，定义一个多行命令
5. 注释：#号，单行

在makefile中的命令，必须以[Tab]键开始

## 包含其他makefile

```bash
include foo.make a.mk b.mk *.mk $(bar)
#include前可以有空格但不能有Tab
```

include会让make首先在当前目录下寻找相关文件，

如果没找到，就判断：

- 是否有``-I``或者``--include-dir``参数，有的话就去指定目录下寻找
- 若``<prefix>/include``存在，也会去找

如果还没找到，make生成警告信息，不会出现致命错误，继续载入其他文件，直到完成makefile的读取，然后make会再检查一次；

如果还没找到，就出现致命错误

若希望无论如何都继续，可以`-include`而不是`include`，或sinclude

若环境变量中存在MAKEFILES，make会做一个类似include的动作，但是建议别这样用

## 规则的语法

1. targets:prerequisites

   ​	command

   ​	...

2. targets:prerequisites;command

   ​	command

   ​	...

targets就是文件名，以空格分开，可以使用通配符

command就是命令行，单独成行时必须有Tab

prerequisites就是依赖目标

make对于一行上有多少个字符这件事是没有限制的

一般来说，make会以UNIX的标准Shell，即/bin/sh来执行命令

通配符指星号、问号等符号，和Unix的B-Shell相同

## ~（波浪号）

`~/test`表示当前用户的$HOME目录下的test目录

## 星号

`*.c`指后缀为c的文件

## 问号

`$?`是一个自动化变量

## 变量的通配符使用方法

```bash
objects:=$(wildcard *.o)
```

## VPATH

一个make关键字，再议，感觉用处不大，草稿打算丢掉了

## 伪目标

在targets位置出现的东西，太简单就不说明了

## 多目标生成规则

再议

## 静态模式

## 变量

## 关键字

## 条件判断

## makefile提供了什么

makefile提供了完成大型工程的能力，它保存着整个工程的编译规则，其中源文件不计其数，目录若干，规定哪些文件先编译、后编译、重新编译，或更复杂的操作

自动化编译由make命令实现，整个工程完全自动编译

make是一个命令工具，解释makefile中指令的命令工具

makefile就是编译方法

## 编译（compile）

一般流程：``源文件->目标文件->可执行文件``，当目标文件过多时，目标文件经过打包，成为库文件