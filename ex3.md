## 思考题

#### 3.1 BIOS
---
##### 1.比较UEFI和BIOS的区别。
“统一的可扩展固件接口”(Unified Extensible Firmware Interface)， 是一种详细描述类型接口的标准。这种接口用于操作系统自动从预启动的操作环境，加载到一种操作系统上。

##### 2.描述PXE的大致启动流程。
BIOS
主引导记录
硬盘系统
操作系统

#### 3.2 系统启动流程
---
##### 1.了解NTLDR的启动流程

NTLDR是一个隐藏的，只读的系统文件，位置在系统盘的根目录，用来装载操作系统。是windows nt 4.0/windows 2000/windows xp/windows server 2003的引导文件，所以应该在系统正常的时候给予备份。
正常系统引导过程
NTLDR文件的是一个隐藏的，只读的系统文件，位置在系统盘的根目录，用来装载操作系统。
一般情况系统的引导过程是这样的代码
1：电源自检程序开始运行
2：主引导记录被装入内存，并且程序开始执行
3：活动分区的引导扇区被装入内存
4：NTLDR从引导扇区被装入并初始化
5：将处理器的实模式改为32位平滑内存模式
6：NTLDR开始运行适当的小文件系统驱动程序。
小文件系统驱动程序是建立在NTLDR内部的，它能读FAT或NTFS。
7：NTLDR读boot.ini文件
8：NTLDR装载所选操作系统
如果windows NT/windows 2000/windows XP/windows server 2003这些操作系统被选择，NTLDR运行Ntdetect。
对于其他的操作系统，NTLDR装载并运行Bootsect.dos然后向它传递控制。
windows NT过程结束。
9：Ntdetect搜索计算机硬件并将列表传送给NTLDR，以便将这些信息写进\\HKE Y_LOCAL_MACHINE\HARDWARE中。
10：然后NTLDR装载Ntoskrnl.exe，Hal.dll和系统信息集合。
11：Ntldr搜索系统信息集合，并装载设备驱动配置以便设备在启动时开始工作
12：Ntldr把控制权交给Ntoskrnl.exe，这时，启动程序结束，装载阶段开始


##### 2.了解GRUB的启动流程
当系统加电后，固化在BIOS中的程序首先对系统硬件进行自检，自检通过后，就加载启动磁盘上的MBR，并将控制权交给MBR中的程序(stage1)，stage1执行，判断自己是否GRUB，如果是且配置了stage1_5，则加载stage1_5，否则就转去加载启动扇区，接着，stage2被加载并执行，由stage2借助stage1_5驱动文件系统，并查找grub.conf，显示启动菜单供用户选择，然后根据用户的选择或默认配置加载操作系统内核，并将控制权交给操作系统内核，由内核完成操作系统的启动。

##### 3.比较NLTDR和GRUB的功能差异。
NTLDR是windows系统的引导文件，只能装在硬盘
GRUB是多系统引导管理器，可以引导硬盘/光盘/U盘等


##### 4.了解u-boot的功能
U-Boot工作过程
U-Boot启动内核的过程可以分为两个阶段，两个阶段的功能如下：
###### （1）第一阶段的功能
* 硬件设备初始化
* 加载U-Boot第二阶段代码到RAM空间
* 设置好栈
* 跳转到第二阶段代码入口


###### （2）第二阶段的功能
* 初始化本阶段使用的硬件设备
* 检测系统内存映射
* 将内核从Flash读取到RAM中
* 为内核设置启动参数
* 调用内核

#### 3.3 中断、异常和系统调用比较
---
##### 1.举例说明linux中有哪些中断，哪些异常？

##### 2.Linux的系统调用有哪些？大致的功能分类有哪些？

##### 3.举例说明Linux中有哪些中断，哪些异常？

#### 3.4 linux系统调用分析
---

##### 1.通过分析lab1_ex0了解Linux应用的系统调用编写和含义。(w2l1)

* objdump是一个展示目标文件的各种信息的程序，它可被用作一个反编译器，例如执行"objdump -S lab1-ex0.exe"可反编译出lab1-ex0.s编译出来的可执行文件。
```
08048294 <_init>:
 8048294:	53                   	push   %ebx
 8048295:	83 ec 08             	sub    $0x8,%esp
 8048298:	e8 83 00 00 00       	call   8048320 <__x86.get_pc_thunk.bx>
 804829d:	81 c3 63 1d 00 00    	add    $0x1d63,%ebx
 80482a3:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
...
```

* nm可以列出目标文件的符号清单，例如执行"nm lab1-ex0.exe"可以列出该程序的符号清单。
```
lab1-ex0.exe:00000002 a AF_INET
lab1-ex0.exe:0804a040 B __bss_start
lab1-ex0.exe:0804a040 b completed.6590
lab1-ex0.exe:0804a014 D __data_start
lab1-ex0.exe:0804a014 W data_start
...
```
* file可以判断文件的类型，列出其信息，例如执行"file lab1-ex0.exe"可以得到：
```
lab1-ex0.exe: ELF 32-bit LSB  executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=aae73c6493d0612a251a5ce8acc679a9375d302f, not stripped
```

* 系统调用的具体含义：
系统调用是由os提供的程序接口或应有编程接口，是连接应用程序和系统之间的通道。
与函数调用相比：
系统调用的调用过程和被调用过程运行在不同的状态（用户态和内核态），而函数调用则始终运行在用户态。
系统调用必须通过软中断机制先进入kernal，然后才能使用相应的命令处理系统调用，而普通过程则可以直接调用。

##### 2.通过调试lab1_ex1了解Linux应用的系统调用执行过程。(w2l1)

+ 采分点：说明了strace的大致用途，说明了系统调用的具体执行过程（包括应用，CPU硬件，操作系统的执行过程）

将lab1-ex1.c编译，使用strace可以跟踪进程lab1-ex1.exe产生的系统调用：
```
gcc -o lab1-ex1.exe lab1-ex1.c
strace -c ./lab1-ex1.exe
```
```
hello world
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 34.10    0.000104          35         3         3 access
 22.30    0.000068           9         8           mmap
 15.08    0.000046          12         4           mprotect
  7.87    0.000024          24         1           write
  6.89    0.000021          11         2           open
  6.56    0.000020          20         1           munmap
  2.95    0.000009           3         3           fstat
  1.31    0.000004           2         2           close
  0.98    0.000003           3         1           read
  0.98    0.000003           3         1           execve
  0.66    0.000002           2         1           arch_prctl
  0.33    0.000001           1         1           brk
------ ----------- ----------- --------- --------- ----------------
100.00    0.000305                    28         3 total
```
再使用以下命令查看进程产生的系统中断。
```
more /proc/interrupts
```
```
           CPU0       
  0:         36   IO-APIC-edge      timer
  1:       2577   IO-APIC-edge      i8042
  8:          0   IO-APIC-edge      rtc0
  9:          0   IO-APIC-fasteoi   acpi
 12:       6384   IO-APIC-edge      i8042
 14:          0   IO-APIC-edge      ata_piix
 15:       5568   IO-APIC-edge      ata_piix
 19:      15504   IO-APIC-fasteoi   eth0
 20:       8160   IO-APIC-fasteoi   vboxguest
 21:      21556   IO-APIC-fasteoi   ahci, snd_intel8x0
 22:         28   IO-APIC-fasteoi   ohci_hcd:usb1
NMI:          0   Non-maskable interrupts
LOC:     321326   Local timer interrupts
SPU:          0   Spurious interrupts
PMI:          0   Performance monitoring interrupts
IWI:      13703   IRQ work interrupts
RTR:          0   APIC ICR read retries
RES:          0   Rescheduling interrupts
CAL:          0   Function call interrupts
TLB:          0   TLB shootdowns
TRM:          0   Thermal event interrupts
THR:          0   Threshold APIC interrupts
MCE:          0   Machine check exceptions
MCP:         19   Machine check polls
ERR:          0
MIS:          0
```

#### 3.5 ucore系统调用分析
---

ucore的系统调用中参数传递代码分析。
以getpid为例，分析ucore的系统调用中返回结果的传递代码。
以ucore lab8的answer为例，分析ucore 应用的系统调用编写和含义。
以ucore lab8的answer为例，尝试修改并运行ucore OS kernel代码，使其具有类似Linux应用工具strace的功能，即能够显示出应用程序发出的系统调用，从而可以分析ucore应用的系统调用执行过程。


