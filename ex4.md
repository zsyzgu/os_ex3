## 小组思考题
##### 1. lab1中printfmt函数用到了可变参，请参考写一个小的linux应用程序，完成实现定义和调用一个可变参数的函数。
* 此程序用于计算若干个数的加法，由于智能判断具体有多少个参数，所以这里只允许数字在0~9之间，且有小几率出错。
* 按理来说，可变参数函数的传递需要通过字符串处理或者调用时的提示，才能判断有几个参数。

``` cpp
#include <stdio.h>
#include <stdarg.h>

int sum(int i, ...) {
	int ret = i;
	int j;
	
	va_list args;
	va_start(args, i);
	while (j = va_arg(args, int)) {
		if (j < 0 || j > 9) {
			break;
		}
		ret += j;
	}
	va_end(args);

	return ret;
}

int main() {
	printf("%d\n", sum(1, 1));
	printf("%d\n", sum(1, 2, 3, 4));
	return 0;
}
```

##### 2. 如果让你来一个阶段一个阶段地从零开始完整实现lab1（不是现在的填空考方式），你的实现步骤是什么？（比如先实现一个可显示字符串的bootloader（描述一下要实现的关键步骤和需要注意的事项），再实现一个可加载ELF格式文件的bootloader（再描述一下进一步要实现的关键步骤和需要注意的事项）...）

##### 3. 如何能获取一个系统调用的调用次数信息？如何可以获取所有系统调用的调用次数信息？请简要说明可能的思路。
用户编写的程序从调用相关函数，到系统调用被执行，操作系统中处理主要流程是，用户调用的往往是调用包装好的普通函数，而该函数才会涉及到中断相关的汇编代码；操作系统把该请求记录在中断描述符表中，最后会被trap.c中的trap_dispatch函数分发、处理。所以有两种思路：
一是在用户所调用的包装好的函数中加上计数器，记录每个系统调用的调用次数，如果需要获取所有系统调用信息，则需要使用全局变量来记录。
二是在trap_dispatch函数中实现计数，由于函数中已经对各种系统调用的trapno进行了分类，实现起来比较方便。

##### 4. 如何修改lab1, 实现一个可显示字符串"THU LAB1"且依然能够正确加载ucore OS的bootloader？如果不能完成实现，请说明理由。

##### 5. 对于ucore_lab中的labcodes/lab1，我们知道如果在qemu中执行，可能会出现各种稀奇古怪的问题，比如reboot，死机，黑屏等等。请通过qemu的分析功能来动态分析并回答lab1是如何执行并最终为什么会出现这种情况？

##### 6. 对于ucore_lab中的labcodes/lab1,如果出现了reboot，死机，黑屏等现象，请思考设计有效的调试方法来分析常在现背后的原因。

## 各知识点小练习
##### 4.1 启动顺序
* 段寄存器的字段含义和功能有哪些?
在8086系统中，访问存储器的地址码由段地址和段内偏移地址两部分组成。段寄存器用来存放各分段的逻辑基值，并指示当前正在使用的4个逻辑段，包括代码段寄存器CS、堆栈段寄存器SS、数据段寄存器DS和附加段数据寄存器ES。
 * 代码段寄存器CS（Code Segment）存放当前正在运行的程序代码所在段的段基址，表示当前使用的指令代码可以从该段寄存器指定的存储器段中取得，相应的偏移量则由IP提供。
 * 数据段寄存器DS（Data Segment）指出当前程序使用的数据所存放段的最低地址，即存放数据段的段基址。
 * 堆栈段寄存器SS（Stack Segment）指出当前堆栈的底部地址，即存放堆栈段的段基址。
 * 附加段寄存器ES（Extra Segment）指出当前程序使用附加数据段的段基址，该段是串操作指令中目的串所在的段。

* 描述符特权级DPL、当前特权级CPL和请求特权级RPL的含义是什么？在哪些寄存器中这些字段？对应的访问条件是什么？ (challenge)写出一些简单的小程序（c or asm）来体现这些特权级的区别和联系。
CPL是当前正在执行的代码所在的段的特权级，存在于cs寄存器的低两位。
RPL是进程对段访问的请求权限，存在于段选择子的低两位，每个段选择子有自己的RPL。
DPL描述段的权限级别，每个段的DPL固定，存储在段描述符中。
当进程访问一个段时，需要进程特权级检查，一般要求DPL >= max {CPL, RPL}

##### 4.2 C函数调用的实现
* 函数调用的stackframe结构？函数调用的参数传递方法有哪几种？
参数传递方式：通过寄存器传递、通过内存栈传递
* 系统调用的stackframe结构？系统调用的参数传递方法有哪几种？
参数传递方式：通过寄存器传递、通过内存栈传递
* 分析ucore的系统调用实现代码；
* 分析Linux的系统调用实现代码；
* 比较不同特权级的中断切换时的堆栈变化差别；(challenge)写出一些简单的小程序（c or asm）来显示出不同特权级的的中断切换的堆栈变化情况。



在lab1的切换用户态和内核态的代码之后，调用print_stackframe输出堆栈信息。以切换到用户态为例：
``` cpp
struct trapframe kernelToUser = *tf;
kernelToUser.tf_cs = USER_CS;
kernelToUser.tf_ds = kernelToUser.tf_es = kernelToUser.tf_ss = USER_DS;
kernelToUser.tf_esp = (uint32_t)tf + sizeof(struct trapframe) - 8;
kernelToUser.tf_eflags |= FL_IOPL_MASK;
*((uint32_t*)tf - 1) = (uint32_t)&kernelToUser;

print_stackframe();
```
调试时需要在init.c中把lab1_switch_test()函数调用上。
输出ebp、eip，压入栈中的前4个参数，以及CS、DS、ES、SS等信息。

```
+++ switch to  user  mode +++
ebp:0x00007a70 eip:0x001009b6 args: 0x00000000 0x00007a88 0x001012d2 0x0000000a
    kern/debug/kdebug.c:307: print_stackframe+21
ebp:0x00007b00 eip:0x00101ddb args: 0x00007b2c 0x0000000c 0x00010094 0x00000000
    kern/trap/trap.c:198: trap_dispatch+312
ebp:0x00007b20 eip:0x00101eaf args: 0x00007a94 0x00000001 0x00000000 0x00007b78
    kern/trap/trap.c:240: trap+16
ebp:0x00007b78 eip:0x00101ec7 args: 0x00103628 0x00402e00 0x00409c00 0x00433460
    kern/trap/trapentry.S:24: <unknown>+0
ebp:0x00007b98 eip:0x001001fc args: 0x001035dc 0x001035c0 0x0000130a 0x00000000
    kern/init/init.c:111: lab1_switch_test+27
ebp:0x00007bc8 eip:0x00100073 args: 0x00000000 0x00000000 0x00000000 0x00010094
    kern/init/init.c:40: kern_init+114
ebp:0x00007bf8 eip:0x00007d68 args: 0xc031fcfa 0xc08ed88e 0x64e4d08e 0xfa7502a8
    <unknow>: -- 0x00007d67 --
1: @ring 3
1:  cs = 1b
1:  ds = 23
1:  es = 23
1:  ss = 23
+++ switch to kernel mode +++
ebp:0x0010fc18 eip:0x001009b6 args: 0x00007b34 0x0010fcd4 0x00000044 0x00000000
    kern/debug/kdebug.c:307: print_stackframe+21
ebp:0x0010fca8 eip:0x00101e59 args: 0x0010fcd4 0x00000000 0x00000000 0x00000000
    kern/trap/trap.c:216: trap_dispatch+438
ebp:0x0010fcc8 eip:0x00101eaf args: 0x00007b34 0x00000001 0x00000000 0x00007b78
    kern/trap/trap.c:240: trap+16
ebp:0x00007b78 eip:0x00101ec7 args: 0x00103648 0x00402e00 0x00409c00 0x00433460
    kern/trap/trapentry.S:24: <unknown>+0
ebp:0x00007b98 eip:0x00100212 args: 0x001035dc 0x001035c0 0x0000130a 0x00000000
    kern/init/init.c:114: lab1_switch_test+49
ebp:0x00007bc8 eip:0x00100073 args: 0x00000000 0x00000000 0x00000000 0x00010094
    kern/init/init.c:40: kern_init+114
ebp:0x00007bf8 eip:0x00007d68 args: 0xc031fcfa 0xc08ed88e 0x64e4d08e 0xfa7502a8
    <unknow>: -- 0x00007d67 --
2: @ring 0
2:  cs = 8
2:  ds = 10
2:  es = 10
2:  ss = 10
```
内核栈是属于OS的一块固定区域，可以用于保存中断现场、保存操作系统子程序间相互调用的参数、返回值等。
用户栈是属于用户空间的一块区域，用户保存用户进程子程序间的相互调用的参数、返回值等。

##### 4.3 GCC内联汇编
* 使用内联汇编的原因？
一方面，C语言不足以完成CPU的所有指令，尤其是某些特权指令；C语言中加入汇编语言能起到性能优化的作用。另一方面，操作系统工程复杂，需要高级语言c而不仅仅是汇编语言来实现。
* 对ucore中的一段内联汇编进行完整的解释？
以lab1拓展实验一中，触发切换至用户态的函数为例：asm为调用汇编代码的函数，引号中为具体的汇编指令，其中寄存器需要两个百分号来表示（比如%%esp），"%0"的内容会在之后做补充，第一个冒号后为关于output的补充，第二个为关于input的补充，这里的指令是将%0定义为T_SWITCH_TOU（即转换为用户态）。
这段汇编语言的完整意义是，对堆栈做准备；使用int调用系统调用。在中断返回的时候，需要多pop两个值来更新ss和sp，所以在中断返回前先把栈压两位。

``` cpp
static void
lab1_switch_to_user(void) {
    asm volatile(
      "sub $0x8, %%esp\n"
      "int %0\n"
      "movl %%ebp, %%esp"
      :
      : "i"(T_SWITCH_TOU)
    );
}
```

##### 4.4 x86中断处理过程
* 中断描述符表IDT的结构？
中断描述符表IDT是一个最大为256项的表，每个表项为8字节。

* 中断描述表到中断服务例程的地址计算过程？
中断服务例程的地址 = IDT的基址 + n * 8 （其中n为中断号）

* trap类型的中断门与interrupt类型的中断门有啥设置上的差别？
 * 中断门（Interrupt gate）
类型码为110，包含了一个中断或异常处理程序所在段的选择符和段内偏移量。当控制权通过中断门进入中断处理程序时，处理器清IF标志，即关中断，以避免嵌套中断的发生。中断门中的DPL（Descriptor Privilege Level）为0，因此，用户态的进程不能访问Intel的中断门。所有的中断处理程序都由中断门激活，并全部限制在内核态。
 * 陷阱门（Trap gate）
类型码为111，与中断门类似。其唯一的区别是，控制权通过陷阱门进入处理程序时维持IF标志位不变，即不关中断。
