# 说明

## 是什么

人话：Makefile是用来执行一套编译流程的脚本。Makfile本身不能执行“编译”这个工作，但是它可以调用编译器等工具完成一套工程的编译。众所周知，要编译一个源文件，就需要使用gcc或g++，而gcc本身是把汇编、链接、编译等工作一起完成的工具，当我们写的代码的框架比较小、简单时，用gcc或cross-compile-gcc，配上-c或-o等选项就可以进行处理了，我们会直接得到一个可执行文件，有时可执行文件的后缀为exe，有时为out，有时没有后缀；但是随着我们逐步拥抱了开源世界，开源的一个软件包，它本身就包含了非常大的工作量，这些工作是世界上各个角落的人（只要有这样的知识）编写的代码和工程，而你作为它的一个使用者，你就面对了一个千万人铸成的工程，此时再使用gcc，就会显得流程琐碎而且反人类，因为这样的工作流程广泛存在于职场中，你需要用有限的记忆能力去记住“什么.c或.cpp文件需要在哪一刻、哪一次编译之后、哪个路径下编译”，很多人都认为这是不现实的，所以有了makefile，它会帮你记住什么时候编译什么内容，而你只需要运行这样的一个脚本就可以了；但我们可以只把它当作一个思想，既然我可以写makefile去控制gcc的使用，那我还可以控制一个界面、软件去生成makefile，让开发者更加迅速地完成它的工作，让工程立刻平地而起，形成较为完整的框架，于是有了gmake、qmake、cmake等等软件。

官话：[一文入门Makefile](https://zhuanlan.zhihu.com/p/56489231)、[GNU make](https://www.gnu.org/software/make/manual/make.html)、[一个简单的 make & makefile 教程](https://zhuanlan.zhihu.com/p/92010728)，这些网络教程可以代表对makefile非常正式的表述。

唠叨：makefile贯穿了写C文件的始终，各种make贯穿了生成makefile的始终，这套体系是非常非常庞大的，我们有个大致了解就行了，就像你使用常见的汉字而不是把整个词典背下来，而这与汉字的类比又恰好体现了makefile的重要性。

## 怎么做

人话：makefile无非是一个脚本文件，如同其他脚本文件一样，有它自己的语法，意思是和python、matlab、shell比较类似，有一个可执行文件可以执行已经编写好的脚本，即解释器解释脚本，这里选用网络上流传比较广泛的[《跟我一起写Makefile.pdf》](https://haoel.blog.csdn.net/article/details/2886?spm=1001.2014.3001.5502)，作者是陈皓，CSDN用户名为haoel，当前时间点为20230911，这位大佬于前一段时间离世，感谢他给后辈（包括我）留下的珍贵作品，我个人没有跟完他写的这个pdf全套，只看了大概6次帖，对makefile有个一知半解就开始编译了。

官话：[GNU make](https://www.gnu.org/software/make/manual/make.html)

唠叨：makefile是一个几乎只和实践打交道的东西，官话反而比人话更重要吧，不像一些可以偏理论的技能。

## 补充

既然提到了Makefile，要补充的内容里就必须包含make、cmake、qmake、gmake，其他make再议！

## make

先拿出链接，看到阮一峰就知道是大佬：[Make 命令教程 - 阮一峰的网络日志 (ruanyifeng.com)](https://www.ruanyifeng.com/blog/2015/02/make.html)

make是一个根据指定的Shell命令进行构建（build）的工具，它的规则仅有：要build哪个文件、它依赖哪些文件、依赖文件变动时如何重新构建，即从哪来，到哪去，如何重复这个过程。

make默认扫描Makefile这个文件，或者```make -f my_makefile```，选用其他makefile文件。

根据前边介绍的Makefile文件，这里我就把make视为一个**智能的批处理工具**，就是它可以根据一个规则处理一批文件。在实际编译流程中，make之后通常会加一个```-j```指令，比如```sudo make -j4```，它的意思是**让make最多允许4条编译命令同时执行**，这个4是可以变化的，通常是核心数的两倍即可，例如我给虚拟机配置了2个物理核，每个物理核有两个逻辑核，那么总核数为4核，就可以```sudo make -j8```。

另外我前边说过，makefile会决定**你的哪个文件在哪个文件之后再编译**，意思就是当你make -j2时，存在两个编译是同时进行的，可能不存在先后，但makefile写了先后，就可能报错，这就是makefile不规范导致的问题了。

## qmake

既然本作品是基于Qt写的GUI，那么我先从qmake开始介绍。

在[Qt](../../../demo/关于Qt/Readme.md)的章节我们一定会自己去编译一个qmake出来，再在qt creator中使用qmake去编译.pro工程，我目前的经验告诉我，.pro文件就已经说明了要如何编译，即它很像qmake的makefile，qmakefile一般，因为我曾在编译Qt项目时出现一个问题：它只编译得出一个**动态链接文件**，而不是可执行文件，我们大可认为它就是库文件，于是我经过一番搜索之后得知我需要在.pro文件最后加上一个选项，那么将会生成可执行文件，最后执行结果也确实如此，那么这就说明.pro指导了qmake生成makefile的规则，makefile又决定了gcc的编译规则，最终决定了编译结果的文件类型。

这里我能直接想通是qmake生成的make（我当时是新手，是第一次见到这样的过程），一是因为我对makefile有预先学习，二是因为我在捣鼓qt时有被要求执行一行代码```sudo make -j4```，这让我对qmake的工作方式有了一定程度的感性认识。

现在我正式说明qmake：

- qmake是Trolltech公司创建的用来为不同的平台和编译器书写Makefile的工具
- qmake是一个用来简化在不同平台间开发工程的构建过程的工具
- qmake会自动生成Makefile文件，可以用于任何软件项目中，无论是否由Qt编写
- qmake会注意所有的编译器和平台依赖性，开发者只需要关注自己的代码
- qmake作为Qt库和Qt所提供的工具的主要连编工具，可以自动包含moc和uic的连编规则

从以上的介绍中我们进一步确定：qmake是包含了编译器信息、Makefile生成方式的。而编译器则包含了平台信息，即编译器是有用于本地编译还是交叉编译，是用于x86还是用于arm，或者是从x86编译，工作于arm，还是从arm编译，工作于x86。

从实际编译文件来分析，例如一个项目通过Qt编译，它会生成一个项目文件夹，其中包含了``*.o、moc_*.o、ui_*.h、Makefile``等文件，以及可执行文件，其中的生成顺序应该是qmake先得出``Makefile``，``moc_*.cpp``等文件，再编译得到``*.o``、可执行文件，至于.o文件是和可执行文件一起出的还是先出.o文件再出可执行文件，这需要看一看Makefile文件当中的具体代码。

## make/gmake/nmake

在Linux中，make就是make，全称是GNU make

### 小故事

这里插入一下Linux系统的发展史：

可知的最早的比较出名的操作系统是Multics，而Unix之父为了丰富OS的体系，让OS可移植性更强，根据Multics的概念用C语言重写了新的多任务、多用户操作系统Unix；Unix不是开源的，是要钱的，在后来的开发中，一些大佬研究生给Unix写了一套软件，继续丰富了Unix的生态（指附加软件），它是BSD，除了BSD，还有HP-UX（惠普公司的）、AIX（IBM公司的）、Solaris（SUN公司的）、Sequent、Xenix等，其中BSD后来衍生出了OS X，即macOS。

- Unix
  1. BSD
     - OS X
       1. macOS
     - FreeBSD（开源）
     - OpenBSD（开源）
     - NetBSD（开源）
  2. HP-UX
  3. AIX
  4. Solaris
  5. Sequent
  6. Xenix

划重点：Unix是要钱的！

在后来的研究中，总有大佬觉得：收费会提高研发成本、降低研发速率，于是Linux系统伴随着GNU工具和Linux内核的结合横空出世。

Linux现在多区分为**发行版**：

1. 不同发行版的Linux都是基于Linux内核构建的，内核版本可能略有差异，但是它们都遵循了Linux内核的规范和特性；
2. 不同发行版的Linux都遵循开源协议；

要是论不同点的话，那就太多了，对于我们开发者来说，最明显的是有一些Linux发行版使用apt进行软件安装，有一些使用yum进行软件安装，这里简要说明：

1. Linux发行版主要有基于Debian的和基于RedHat的，其中基于Debian的系统有Linux Debian和Linux Ubuntu，其中Ubuntu是从Debian发展而来的**衍生发行版**，意思是把Linux比作爷爷，Debian算个父亲，Ubuntu一开始算个孙子，后来孙子长大了，可以和父亲抬杠了；Ubuntu据说一开始也是依赖于Debian的，随着时间推移，Ubuntu后来变得独立了；

2. Debian系列都可以视为使用名为``apt``的软件包管理器，RedHat系列视为使用名为``yum``的软件包管理器，其中RedHat系列则有Red Hat Enterprise Linux（RHEL）、CentOS、Fedora；比较特别的是openSUSE和SLES发行版，它们apt和yum都可以用；

3. 按照近几年的发展情况来说，中国国内也出现了一些Linux发行版，如本比赛作品使用的Loongnix，属于Debian系列，然而它曾经属于RedHat系列；同时还有银河麒麟等国产系统，欢迎关注国内Linux发行版系统发展情况~

   小故事说明完毕，根据以上知识，上述Linux发行版内一般都是用make作为默认GNU make，同时还有一些比较离谱的系统，如TI的SYS/BIOS（以前叫DSP/BIOS）、RT-Thread，它们都是嵌入式系统，但是它们是通过在公司提供的IDE中进行交叉编译而开发的，这些系统中可能本身自带了make，而不是Linux系统诞生时使用的GNU make（意思是它们可能是和Linux同等级的概念，make是低于Linux的概念，它必须活在Linux的概念中才具有唯一指向性），此时make就很可能不再是GNU make，但是GNU make因Linux的缘故，使用的非常广泛，那么就给GNU make改个名，叫做gmake。

   其他的还有Windows，它是和Linux同等级的概念，即Windows有自己的make，那么Windows从Linux那里借来的GNU make就改名叫做nmake（我还没见过这个东西）。

   还有Solaris系统，它是一个Unix系统衍生来的系统，它有自己的make，所以从Linux借来的GNU make就叫做gmake。

## cmake

cmake可以认为是和qmake一样的概念，也是用于跨平台build的工具，我在上边把qmake的makefile叫做qmakefile，而cmake已经规定了自己的makefile的名字，它叫做CMakeLists.txt，它和qmake一样：cmake读入cmakelist，生成makefile，makefile决定文件从哪来、到哪去、如何重复这过程，输入make，得到可执行文件。

cmake和qmake最大的区别应该是cmake在windows平台上更加优秀和常见吧，总的来说就是qmake可以满足基本要求，但多用于Qt这个软件中，cmake则比qmake更加万能一点？

## configure

``./configure &*%^^&%^&$%&^$^&*``这个东西在自制交叉编译环境时会操作很多次，尤其是对于本作品来说，龙芯平台是新平台，得自己研究configure后边要跟什么选项，虽然最后都以失败告终了，但是这个经历会拓展我的认知边界。

configure本身是一个脚本，如同.sh文件一样，configure脚本是为了让一个程序能够在各种不同的机器上运行而设计的，在使用make编译源代码之前，configure会根据自己所依赖的库在目标机器上进行匹配。有时候看到一些程序的configure内容超级多，能看下去的人确实不多。

例如在本次作品制作过程中，我们会下载qt-everywhere-src-5.14.2.tar文件，然后在Ubuntu上解压，在路径qt-everywhere-src-5.14.2/qtbase下可以看到一个configure文件，用vim打开之后第一行是``#!/bin/sh``，这说明它可以视为和.sh文件同类型，是脚本文件；并且这是下载qt src code就自带的脚本，而下载qt src code的目的是编译一个用于交叉编译Qt工程的qmake文件，然后再用qmake文件去编译你在Qt Creator中写的.cpp文件和ui文件，得到可执行文件之后从主机传输到目标平台，然后执行，以完成你的任务。

整个过程梳理如下：

```markdown
./configure----------------------->Makefile
make -j4 && make install---------->read in Makefile,write down qmake.out/exe and .so library files
qmake----------------------------->read in .pro,write down Makefile
make -j4 && make install---------->fix other Qt module to Qt path
qmake----------------------------->read in .pro and .cpp,write down .so library files
qmake----------------------------->read in .pro and .cpp,write down GUI_Program.out/exe
```

前两行是为了生成qmake，后两行是为了完成你的个人应用中用到的库和你的核心GUI软件，中间是为了完善Qt模块。

而configure脚本也是可以用autoconf命令来自动生成的，这里就不再深究了。