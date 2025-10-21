## 过程

![image-20251002173039367](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251002173039367.png)



## 汇编和编译

---

> [!note]
>
> 所有的命令，请在 `bash` 终端操作，避免出现奇怪的问题

![image-20251002151720007](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20251002151720007.png)

```text
.c (C源码)  --预处理-->  .i  
.i          --编译-->   .s  (汇编文件, 人类可读)  
.s          --汇编-->   .o  (目标文件, 二进制)  
.o          --链接-->   .elf/.bin (可执行文件)
```

### 生成汇编文件

```bash
arm-none-eabi-gcc -S main.c -o main.s
```

### 生成目标文件 ( `把你的 C 源码翻译成机器码` )

```bash
# get main.o
arm-none-eabi-gcc -c .\main.c
```

### 目标文件反汇编为汇编文件

```bash
arm-none-eabi-objdump -d main.o
```

```bash
# result
main.o:     file format elf32-littlearm

Disassembly of section .text:

00000000 <main>:
   0:   e92d4800        push    {fp, lr}
   4:   e28db004        add     fp, sp, #4
   8:   e59f0014        ldr     r0, [pc, #20]   @ 24 <main+0x24>
   c:   ebfffffe        bl      0 <puts>
  10:   e3a03000        mov     r3, #0
  14:   e1a00003        mov     r0, r3
  18:   e24bd004        sub     sp, fp, #4
  1c:   e8bd4800        pop     {fp, lr}
  20:   e12fff1e        bx      lr
  24:   00000000        .word   0x00000000
```

### 把汇编文件编译成目标文件

```bash
arm-none-eabi-as -g -o main_by_s.o main.s
```

- `-g` debug 类似效果

```bash
arm-none-eabi-as -mthumb -g -o main_thumb.o main.s
```

- `-mthumb` 使用 Thumb 指令集（适用于部分 ARM 处理器）

<br>

### 目标文件编译为二进制文件

```bash
# .o 直接到 .bin
arm-none-eabi-objcopy -O binary main.o main.bin
```

> 连接之后也可以生成可执行文件

```bash
arm-none-eabi-objcopy -O binary main.elf main.bin
```



<br>

### 链接

> [!warning]
>
> 生成 `ELF（Executable and Linkable Format，可执行与可链接格式`，确保使用的终端是 `bash` 不然这一步会失败

```bash
arm-none-eabi-ld -o main.elf main.o -Map=main.map
```

- `-o main.elf`：指定输出的 ELF 文件名。
- `main.o`：你的目标文件（可以有多个）。
- `-Map=main.map`：生成链接映射文件，方便调试（可选）。

```bash
D:\Develop_Tool\GNU_ARM_Toolchain\arm-gnu-toolchain-14.3\bin\arm-none-eabi-ld.exe: warning: cannot find entry symbol _start; defaulting to 00008000
```

因为我们没有谁当分区索引，所以会是这个值，但是没有关系，这个就是等下连接脚本的作用了，等一下再说



### 生成 hex 文件

```bash
arm-none-eabi-objcopy -O ihex main.elf main.hex
```

