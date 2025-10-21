# 汇编语言

---

> 基于我需要学习嵌入式，基本使用 `Cortex-M3` 架构的 stm32 处理器，所以我们基于这个学习我们的汇编

## 虚拟环境搭建（QMEU）

## 寄存器

**MAC-ARM64** 汇编中有34个寄存器，其中包含31个通用寄存器(**x0-x30**)，**sp**，**pc** 和 **cpsr**

> [!Caution]
>
> - Apple reserves **X18** for its own use. Do not use this register.
> - The frame pointer register (**FP**, **X29**) must always address a valid frame record.

```bash
(lldb) register read
General Purpose Registers:
        x0 = 0x0000000000000008
        x1 = 0x000000016fdfdc70
        x2 = 0x000000016fdfdc80
        x3 = 0x000000016fdfdd80
        x4 = 0x0000000000000000
        x5 = 0x0000000000000000
        x6 = 0x0000000000000000
        x7 = 0x0000000000000000
        x8 = 0x0000000000000008
        x9 = 0x0000000000000002
       x10 = 0x0000000000000000
       x11 = 0x0000000000000002
       x12 = 0x0000000000000002
       x13 = 0x0000000000000000
       x14 = 0x0000000000000001
       x15 = 0x0000000000000044
       x16 = 0x000000030006f120
       x17 = 0x6ae100016fdfa280
       x18 = 0x0000000000000000
       x19 = 0x00000001000d4060
       x20 = 0x0000000100002ea0  
       x21 = 0x0000000100080070  
       x22 = 0x0000000000000000
       x23 = 0x0000000000000000
       x24 = 0x0000000000000000
       x25 = 0x0000000000000000
       x26 = 0x0000000000000000
       x27 = 0x0000000000000000
       x28 = 0x0000000000000000
        fp = 0x000000016fdfdaf0
        lr = 0x000000010000315c  
        sp = 0x000000016fdfdae0
        pc = 0x0000000100002f4c
      cpsr = 0x60001000
```

### 通用寄存器

通用寄存器`x0-x30`有64bit, 如果用不到64位，可以用低位的32位(`w0-w30`), 也就是说x0和w0本质上是一个寄存器，只是利用的位数不一样而已。

通用寄存器可以用来存放函数参数

> [!note]
>
> 说明：有的博客说的是“只有x0-x7用来存放参数，如果函数参数超过了8个，则在压入函数栈中”，我目前测试的情况是至少15个参数都是可以通过寄存器来传值，更多的参数就没有测试了。

`x0` 通常用来来存放返回值，如果返回的数据比较复杂，会放在 `x8` 的这个执行地址上。

```assembly
.global _main
_main:
    mov x0, #1
    mov x1, #2
    add x0, x0, x1
    ; 这里返回的就是 0x 存储的值
    ret
```

<br>

### 特殊寄存器

1. `sp`寄存器：Stack Pointer，保存栈顶地址
2. `fp`寄存器：Frame Pointer，保存栈底地址
3. `lr`寄存器：Link Register，保存跳转指令下一条指令的地址(`eg:bl跳转进入执行函数，lr则会保存函数调用完成后需要执行的指令地址`)
4. `pc` 寄存器: 保存下一条指令的地址
5. `cpsr`寄存器（状态寄存器）：状态寄存器有点特殊，它的每一位都有特殊的含义，记录特定的信息。

#### CPSR 寄存器内容

> 最常见的是`NZCV 标志位`，分别代表运算过程中产生的不同状态，可以决定运算结果或者代码执行逻辑。

- N：Negative Condition Flag，代表运算结果是负数
- Z：Zero Condition Flag， Z 为 1 代表0，否则Z 为 0 代表 1
- C：Carry Condition Flag, 无符号运算有溢出时C 为 1
- V：Overflow Condition Flag, 有符号运算有溢出时C 为 1



<br>

## 参考

- [ARM Cortex-M Assembly Tutorial #1 — Hello World in QEMU (Cortex-M3)](https://www.youtube.com/watch?v=8Ld5YcOzZKg)
- [ARM 官网](https://www.arm.com/)
- [QEMU Download](https://www.qemu.org/download/)

 
