## 14.2(2)
在trap_dispatch()不在内核态的情况下触发panic，
并在panic()中添加：
```
print_statckframe();
```

有如下结果：
```
ebp:0xc0129f38 eip:0xc0100aea args: 0x00010094 0x00000000 0xc0129f68 0xc01000d3
    kern/debug/kdebug.c:352: print_stackframe+21
ebp:0xc0129f48 eip:0xc0100dd3 args: 0x00000000 0x00000000 0x00000000 0xc0129fb8
    kern/debug/kmonitor.c:129: mon_backtrace+10
ebp:0xc0129f68 eip:0xc01000d3 args: 0x00000000 0xc0129f90 0xffff0000 0xc0129f94
    kern/init/init.c:58: grade_backtrace2+33
ebp:0xc0129f88 eip:0xc01000fc args: 0x00000000 0xffff0000 0xc0129fb4 0x0000002a
    kern/init/init.c:63: grade_backtrace1+38
ebp:0xc0129fa8 eip:0xc010011a args: 0x00000000 0xc010002a 0xffff0000 0x0000001d
    kern/init/init.c:68: grade_backtrace0+23
ebp:0xc0129fc8 eip:0xc010013f args: 0xc010c03c 0xc010c020 0x0000318e 0x00000000
    kern/init/init.c:73: grade_backtrace+34
ebp:0xc0129ff8 eip:0xc010007f args: 0x00000000 0x00000000 0x0000ffff 0x40cf9a00
    kern/init/init.c:33: kern_init+84
```

## 14.3(3)
在trap()中判断trapframe的cs不是内核cs时触发panic，
并在panic()中添加：
```
print_stackframe();
print_pgdir();
```

输出：
```
ebp:0xc0129f38 eip:0xc0100aea args: 0x00010094 0x00000000 0xc0129f68 0xc01000d3
    kern/debug/kdebug.c:352: print_stackframe+21
ebp:0xc0129f48 eip:0xc0100dd3 args: 0x00000000 0x00000000 0x00000000 0xc0129fb8
    kern/debug/kmonitor.c:129: mon_backtrace+10
ebp:0xc0129f68 eip:0xc01000d3 args: 0x00000000 0xc0129f90 0xffff0000 0xc0129f94
    kern/init/init.c:58: grade_backtrace2+33
ebp:0xc0129f88 eip:0xc01000fc args: 0x00000000 0xffff0000 0xc0129fb4 0x0000002a
    kern/init/init.c:63: grade_backtrace1+38
ebp:0xc0129fa8 eip:0xc010011a args: 0x00000000 0xc010002a 0xffff0000 0x0000001d
    kern/init/init.c:68: grade_backtrace0+23
ebp:0xc0129fc8 eip:0xc010013f args: 0xc010c03c 0xc010c020 0x0000318e 0x00000000
    kern/init/init.c:73: grade_backtrace+34
ebp:0xc0129ff8 eip:0xc010007f args: 0x00000000 0x00000000 0x0000ffff 0x40cf9a00
    kern/init/init.c:33: kern_init+84
memory management: default_pmm_manager
e820map:
  memory: 0009fc00, [00000000, 0009fbff], type = 1.
  memory: 00000400, [0009fc00, 0009ffff], type = 2.
  memory: 00010000, [000f0000, 000fffff], type = 2.
  memory: 07efe000, [00100000, 07ffdfff], type = 1.
  memory: 00002000, [07ffe000, 07ffffff], type = 2.
  memory: 00040000, [fffc0000, ffffffff], type = 2.
check_alloc_page() succeeded!
check_pgdir() succeeded!
check_boot_pgdir() succeeded!
-------------------- BEGIN --------------------
PDE(0e0) c0000000-f8000000 38000000 urw
  |-- PTE(38000) c0000000-f8000000 38000000 -rw
PDE(001) fac00000-fb000000 00400000 -rw
  |-- PTE(000e0) faf00000-fafe0000 000e0000 urw
  |-- PTE(00001) fafeb000-fafec000 00001000 -rw
--------------------- END ---------------------

```