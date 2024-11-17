# 几个重要的单片机函数

---

<br>

## 1. 中断函数和对应的中断执行函数

> 中断函数

```c++
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
    
    // 下面的部分才可以保证中断
    ET0 = 1;
    EA = 1;
    PT0 = 0;
}

void Timer1_Init(void)        //1毫秒@12.000MHz
{
    TMOD &= 0x0F;			//设置定时器模式
    TL1 = 0x20;				//设置定时初始值
    TH1 = 0xD1;				//设置定时初始值
    TF1 = 0;				//清除TF1标志
    TR1 = 1;				//定时器1开始计时

    ET1 = 1;
    EA = 1;
    PT1 = 0;
}
```

> 中断执行

````c++
void time0() __interrupt(1) {
    TL0 = 0x18;				// 设置定时初始值
	TH0 = 0xFC;				// 设置定时初始值
}



void Timer1_Routine() __interrupt(3) {
    TL1 = 0x20;				//设置定时初始值
    TH1 = 0xD1;				//设置定时初始值
}
````

## 2. 键盘的映射表

````c
unsigned char MatrixKey() {
    unsigned char KeyNumber = 0;

    P1 = 0xFF;
    P1_3 = 0;       // col 1
    // row 1
    if (P1_7 == 0) { DelayXms(20); while (P1_7 == 0); DelayXms(20); KeyNumber = 1; }
    if (P1_6 == 0) { DelayXms(20); while (P1_6 == 0); DelayXms(20); KeyNumber = 5; }
    if (P1_5 == 0) { DelayXms(20); while (P1_5 == 0); DelayXms(20); KeyNumber = 9; }
    if (P1_4 == 0) { DelayXms(20); while (P1_4 == 0); DelayXms(20); KeyNumber = 13; }

    P1 = 0xFF;
    P1_2 = 0;   // col 2
    if (P1_7 == 0) { DelayXms(20); while (P1_7 == 0); DelayXms(20); KeyNumber = 2; }
    if (P1_6 == 0) { DelayXms(20); while (P1_6 == 0); DelayXms(20); KeyNumber = 6; }
    if (P1_5 == 0) { DelayXms(20); while (P1_5 == 0); DelayXms(20); KeyNumber = 10; }
    if (P1_4 == 0) { DelayXms(20); while (P1_4 == 0); DelayXms(20); KeyNumber = 14; }

    P1 = 0xFF;
    P1_1 = 0;
    if (P1_7 == 0) { DelayXms(20); while (P1_7 == 0); DelayXms(20); KeyNumber = 3; }
    if (P1_6 == 0) { DelayXms(20); while (P1_6 == 0); DelayXms(20); KeyNumber = 7; }
    if (P1_5 == 0) { DelayXms(20); while (P1_5 == 0); DelayXms(20); KeyNumber = 11; }
    if (P1_4 == 0) { DelayXms(20); while (P1_4 == 0); DelayXms(20); KeyNumber = 15; }

    P1 = 0xFF;
    P1_0 = 0;
    if (P1_7 == 0) { DelayXms(20); while (P1_7 == 0); DelayXms(20); KeyNumber = 4; }
    if (P1_6 == 0) { DelayXms(20); while (P1_6 == 0); DelayXms(20); KeyNumber = 8; }
    if (P1_5 == 0) { DelayXms(20); while (P1_5 == 0); DelayXms(20); KeyNumber = 12; }
    if (P1_4 == 0) { DelayXms(20); while (P1_4 == 0); DelayXms(20); KeyNumber = 16; }

    return KeyNumber;
}
````

## 3. LCD 屏幕输出控制的函数

> `LCD1602.h`

```h
#include <stdio.h>
#include "mcs51/8052.h"

// LCD1602引脚配置
__sbit __at (0xA0^6) LCD_RS;  // RS 引脚
__sbit __at (0xA0^5) LCD_RW;  // RW 引脚
__sbit __at (0xA0^7) LCD_EN;  // EN 引脚
#define LCD_DataPort P0  // 数据端口

// 函数原型
void LCD_Init(void);

void LCD_WriteCommand(unsigned char Command);
void LCD_WriteData(unsigned char Data);

void LCD_SetCursor(unsigned char Line, unsigned char Column);


// 清空LCD屏幕上特定区域的内容
void LCD_ClearRegion(unsigned char Line, unsigned char Column, unsigned char length);


void LCD_ShowChar(unsigned char Line, unsigned char Column, unsigned char Char);
void LCD_ShowString(unsigned char Line, unsigned char Column, unsigned char *String);

void LCD_ShowHexadecimal(unsigned char Line, unsigned char Column, unsigned int num);
void LCD_ShowDecimal(unsigned char Line, unsigned char Column, int num);
void LCD_ShowBinary(unsigned char Line, unsigned char Column, unsigned int num);

void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int num, unsigned char width);


/**
  * @brief  返回值=X的Y次方
  */
int LCD_Pow(int X,int Y);


/**
  * @brief  在LCD1602指定位置开始以有符号十进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-32768~32767
  * @param  Length 要显示数字的长度，范围：1~5
  */
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
```

> `LCD_1062.c`

```c
#include "LCD1602.h"

// 延时函数
void LCD_Delay(void) {
    unsigned char i, j;
    i = 11;
    j = 190;
    do {
        while (--j);
    } while (--i);
}

// 写入LCD指令
void LCD_WriteCommand(unsigned char Command) {
    LCD_RS = 0;  // 选择指令寄存器
    LCD_RW = 0;  // 写入模式
    LCD_DataPort = Command; // 将命令写入数据端口
    LCD_EN = 1;  // 产生使能信号
    LCD_Delay(); // 等待
    LCD_EN = 0;  // 关闭使能信号
    LCD_Delay(); // 等待
}

// 写入LCD数据
void LCD_WriteData(unsigned char Data) {
    LCD_RS = 1;  // 选择数据寄存器
    LCD_RW = 0;  // 写入模式
    LCD_DataPort = Data; // 将数据写入数据端口
    LCD_EN = 1;  // 产生使能信号
    LCD_Delay(); // 等待
    LCD_EN = 0;  // 关闭使能信号
    LCD_Delay(); // 等待
}

// 设置光标位置
void LCD_SetCursor(unsigned char Line, unsigned char Column) {
    unsigned char address = (Line == 1) ? (0x80 + Column - 1) : (0xC0 + Column - 1);
    LCD_WriteCommand(address); // 发送光标位置命令
}

// 初始化LCD1602
void LCD_Init(void) {
    LCD_WriteCommand(0x38);  // 8位数据接口，两行显示，5x7点阵
    LCD_WriteCommand(0x0C);  // 显示开，光标关，闪烁关
    LCD_WriteCommand(0x06);  // 数据读写后光标右移
    LCD_WriteCommand(0x01);  // 清屏
}

// 显示单个字符
void LCD_ShowChar(unsigned char Line, unsigned char Column, unsigned char Char) {
    LCD_SetCursor(Line, Column); // 设置光标
    LCD_WriteData(Char); // 写入字符
}

// 显示字符串
void LCD_ShowString(unsigned char Line, unsigned char Column, unsigned char *String) {
    LCD_SetCursor(Line, Column); // 设置光标
    while (*String) {
        LCD_WriteData(*String++); // 写入字符串
    }
}

// 返回 X^Y
int LCD_Pow(int X, int Y) {
    int Result = 1;
    for (unsigned char i = 0; i < Y; i++) {
        Result *= X;
    }
    return Result;
}

// 显示十进制数
void LCD_ShowDecimal(unsigned char Line, unsigned char Column, int num) {
    LCD_SetCursor(Line, Column); // 设置光标位置
    LCD_ShowString(Line, Column, "Dec: "); // 显示标签

    if (num < 0) {
        LCD_WriteData('-'); // 显示负号
        num = -num; // 将负数转换为正数
    }

    // 转换数字为字符串并逐个显示
    if (num == 0) {
        LCD_WriteData('0');  // 处理特殊情况：数字为0
    } else {
        unsigned char buffer[10]; // 用于存储数字字符串
        int index = 0;

        // 将数字转换为字符串
        while (num > 0) {
            buffer[index++] = (num % 10) + '0'; // 获取最后一位并转换为字符
            num /= 10; // 去掉最后一位
        }

        // 反转字符串以正确显示
        for (int i = index - 1; i >= 0; i--) {
            LCD_WriteData(buffer[i]); // 从后往前写入字符
        }
    }
}

// 显示二进制数（最大255，即8位）
void LCD_ShowBinary(unsigned char Line, unsigned char Column, unsigned int num) {
    // 设置光标位置
    LCD_SetCursor(Line, Column);
    LCD_ShowString(Line, Column, "Bin: "); // 显示标签

    // 处理特殊情况：数字大于255
    if (num > 255) {
        num = 255; // 限制最大值为255
    }

    unsigned char buffer[10]; // 用于存储二进制字符串，8位 + "0b" 前缀 + 结束符
    buffer[9] = '\0'; // 添加字符串结束符
    int index = 7; // 从字符串的最后一个字符开始填充

    // 添加前缀 "0b"
    buffer[0] = '0';
    buffer[1] = 'b';

    // 将数字转换为二进制字符串
    for (int i = 0; i < 8; i++) {
        buffer[index--] = (num % 2) + '0'; // 获取最后一位并转换为字符
        num /= 2; // 去掉最后一位
    }

    // 确保在显示二进制数之前将光标移到正确的位置
    LCD_SetCursor(Line, Column + 5); // "Bin: " 后的光标位置

    // 显示二进制字符串
    LCD_ShowString(Line, Column + 5, buffer); // 显示转换后的二进制字符串
}

// 显示十六进制数
void LCD_ShowHexadecimal(unsigned char Line, unsigned char Column, unsigned int num) {
    LCD_SetCursor(Line, Column);           // 设置光标位置
    LCD_ShowString(Line, Column, "Hex: ");  // 显示标签

    unsigned char hex[2];                   // 存储两个十六进制字符

    // 高四位转换为字符
    hex[0] = (num >> 4) & 0x0F;             // 取高4位
    hex[0] += (hex[0] < 10) ? '0' : ('A' - 10); // 转换为十六进制字符

    // 低四位转换为字符
    hex[1] = num & 0x0F;                    // 取低4位
    hex[1] += (hex[1] < 10) ? '0' : ('A' - 10); // 转换为十六进制字符

    // 显示转换后的两个字符
    LCD_ShowChar(Line, Column + 5, hex[0]);
    LCD_ShowChar(Line, Column + 6, hex[1]);
}

// 显示无符号数
void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int num, unsigned char width) {
    unsigned char digits[5];  // 假设最大宽度为5，调整宽度可以支持更大数值
    unsigned char i = 0;

    // 将数字逐位拆解为字符
    while (num > 0 && i < width) {
        digits[i++] = num % 10 + '0';
        num /= 10;
    }

    // 填充不足宽度的位数为'0'
    while (i < width) {
        digits[i++] = '0';
    }

    // 倒序输出字符
    while (i > 0) {
        LCD_ShowChar(Line, Column++, digits[--i]);
    }
}

// 清空LCD屏幕上特定区域的内容
void LCD_ClearRegion(unsigned char Line, unsigned char Column, unsigned char length) {
    LCD_SetCursor(Line, Column); // 设置光标位置
    for (unsigned char i = 0; i < length; i++) {
        LCD_WriteData(' '); // 用空格覆盖字符
    }
}

// 显示有符号数
void LCD_ShowSignedNum(unsigned char Line, unsigned char Column, int Number, unsigned char Length) {
    unsigned char i;
    unsigned int Number1;
    LCD_SetCursor(Line, Column);
    if (Number >= 0) {
        LCD_WriteData('+');
        Number1 = Number;
    } else {
        LCD_WriteData('-');
        Number1 = -Number;
    }
    for (i = Length; i > 0; i--) {
        LCD_WriteData(Number1 / LCD_Pow(10, i - 1) % 10 + '0');
    }
}
```

## 4. Platform IO 的配置文件

> `c51`

```ini
[env:STC89C52RC]
platform = intel_mcs51
board = STC89C52RC
upload_flags =
    -p
    $UPLOAD_PORT
upload_command = stcgal $UPLOAD_FLAGS $SOURCE
lib_deps = C:\Users\shuhe\.platformio\packages\toolchain-sdcc\include
```

## 5. 串口的中断触发

> **接收中断（RI 标志位被置位）**：当开发板接收到数据时触发中断，进入中断函数处理接收的数据。这通常是 **PC to Board** 的通信，因为电脑发送数据，开发板接收并处理。
>
> **发送中断（TI 标志位被置位）**：当开发板完成数据发送时触发中断，进入中断函数继续执行其他任务或准备下一个字节的发送。这种情况下通常用于 **Board to PC** 的通信，因为开发板在发送数据给电脑。

```c
// 发送数据
void UART_SendByte(uint8_t byte) {
    SBUF = byte;      // 将数据写入SBUF，开始发送
    while (TI == 0);  // 等待发送完成
    // Transport Interrupt
    TI = 0;           // 清除发送完成标志位
}

// 接收数据
void UART_Receive() __interrupt(4) {
    if (RI == 1) {       // 检查是否接收到数据
        uint8_t receivedData = SBUF; // 从SBUF读取接收到的数据
        // Receive Interrupt
        RI = 0;           // 清除接收完成标志位
        
        // 处理接收到的数据 ...
    }
}
```

## 6. Matrix LED 控制

```c
/**
 * @brief 控制 LED Matrix 的行
 * @param row 必须得是二进制，用于控制列是否要亮，从上到下，1 亮 0 不亮
 * */
void _74HC595_WriteByte(unsigned char row) {
    // 遍历字节的每一位
    for (unsigned char i = 0; i < 8; i++) {
        // 将字节的第i位写入SER引脚
        // 使用位运算，Byte & (0x80 >> i) 用于获取字节中的每一位
        // 0x80是0b1000 0000，右移i位后，只保留需要的位
        SER = row & (0x80 >> i); // 将字节的第i位放入SER引脚

        // 产生一个SCK脉冲
        SCK = 1;  // 设置SCK引脚为高电平，表示时钟信号上升沿
        SCK = 0;  // 设置SCK引脚为低电平，表示时钟信号下降沿
    }

    // 向74HC595发送锁存信号，以将移位寄存器的值锁存到输出寄存器
    RCK = 1;  // 设置RCK引脚为高电平，开始锁存
    RCK = 0;  // 设置RCK引脚为低电平，结束锁存
}

/**
 * @brief 控制具体的每一列的哪几个行亮
 * */
void MatrixLED_ShowColumn(unsigned char col, unsigned char row) {
    _74HC595_WriteByte(row);
    P0 = ~(0x80 >> col);
    DelayXms(1);
    P0 = 0xFF;
}
```

## 7. 蜂鸣器和对应的数学关系

```c
/**
 * @brief 让蜂鸣器发出鸣叫
 * @param xms 就是 f = 1 / xms
 * */
void Buzzer_Time(unsigned int xms) {
    for (unsigned int i = 0; i < xms * 2; ++i) {
        Buzzer = !Buzzer;
        Delay500us();
    }
}
```

1. **一个完整周期的时间**：
    每次循环花费 500 微秒（高低电平各一次）才形成一个完整的方波周期，所以一个完整的方波周期耗时：

    $T_{total}=500 μs×2=1000\mu s = 1ms$

2. **方波的频率（Hz）**：
    频率 `f` 和周期 `T` 的关系是 $f = \frac{1}{T}$​，因此方波的频率为：

    $f=\frac{1}{T_{total}} = \frac{1}{1 ms}=  \frac{1}{0.001 s} = 1000 Hz$

因此，每次循环会产生一个 **1 kHz 的方波**。

<br>

## 8. AT24 实现数据的存储和读取

---

### 8.1 基本概念

> AT24C02 是一种 **I2C 接口的 EEPROM（电可擦可编程只读存储器）**，用于存储数据。它本身不存储接口的地址，而是存储 **数据**，通常是一些程序设置、配置信息或其他需要长期保存的数据
>
> AT24C02 有一个 **I2C 地址**，这是用于通信时唯一标识该 EEPROM 的地址。
>
> **存储内容**：AT24C02 内部存储器的每一个地址单元都可以存储 **1 字节的数据**。通常这些数据是用户定义的内容，比如设备配置、校准数据或其他需要持久保存的信息。
>
> **总存储容量**：AT24C02 的存储容量是 **256 字节**。因此，它总共有 256 个存储地址，**存储每个地址的 1 字节数据**。

#### 8.1.1 硬件的地址

> **硬件预定义** 在单片机（如 8051 或 8052）等嵌入式系统中，硬件的地址（如 I/O 端口 `P0`, `P1` 等）是 **由芯片设计时固定** 的。设计者在芯片内部划分了 **内存映射空间** 和 **I/O 地址空间**，并将特定硬件组件（如输入输出端口、控制寄存器等）分配给特定的地址。
>
> 例如：
>
> - 在 8051 中，`P0` 的地址是 `0x80`，`P1` 是 `0x90`，这就是硬件预定义的地址。
> - 这些地址没有存储在 RAM 中，而是存储在芯片的内部硬件寄存器中。

### 8.2 数据的接受和发送的实现

> 头文件

```h
#include "mcs51/8052.h"

__sbit __at(0xA0 ^ 1) I2C_SCL;
__sbit __at(0xA0 ^ 0) I2C_SDA;

typedef unsigned char unit8_t;

void I2C_Start(void);

void I2C_Stop(void);

void I2C_SendByte(unit8_t Byte);

unit8_t I2C_ReceiveByte(void);

void I2C_SendAck(unit8_t AckBit);

unit8_t I2C_ReceiveAck(void);
```

> 代码实现

```c
#include "I2C.h"

void I2C_Start(void) {
    // 起始条件：SCL 高电平期间，SDA 从 高电平 切换到 低电平
    I2C_SDA = 1;
    I2C_SCL = 1;

    I2C_SDA = 0;
    I2C_SCL = 0;
}

void I2C_Stop(void) {
    // 停止条件是：SCL 高电平期间， SDA 从 低电平 到 高电平
    I2C_SCL = 0;
    I2C_SDA = 0;

    I2C_SCL = 1;
    I2C_SDA = 1;
}

void I2C_SendByte(unit8_t Byte) {
    // 循环 8 个 周期
    for (unit8_t i = 0; i < 8; ++i) {
        // SDA 快点，所以我们放前面
        // 外面获取了 8 个字节长度的数据，我们读取每一次
        I2C_SDA = Byte & (0x80 >> i);      // 取高位
        I2C_SCL = 0;

        // 一个数据的结束
        // start
        I2C_SCL = 1;
        I2C_SCL = 0;
        // end
    }
}

unit8_t I2C_ReceiveByte(void) {
    unit8_t Byte = 0x00;

    for (unit8_t i = 0; i < 8; i ++) {
        I2C_SCL = 0;
        I2C_SCL = 1;    // 一个数据的开始
        /*
         * 我们是每一次从外面接受到一个数据，然后数据要存储到 Byte 中
         * 对于外部数据，要么 1 | 0，所以 | 就一定会存储 1
         * 而 | 0 还是 0，所以相当于把外面的数据一个个存储到对应位上
         * */
        if (I2C_SDA) { Byte |= (0x80 >> i); }
        I2C_SCL = 0;    // 一个数据的结束
    }

    return Byte;
}

unit8_t I2C_ReceiveAck(void) {
    // 从外部接受收到一个报文
    unit8_t AckBit;
    I2C_SCL = 0;
    I2C_SDA = 1;
    // 系统始终开始
    I2C_SCL = 1;

    // 把报文赋值给一个内部变量
    AckBit = I2C_SDA;

    // 系统时钟结束
    I2C_SCL = 0;
    return AckBit;
}

void I2C_SendAck(unit8_t AckBit) {
    // 发送一个报文，0 是答应，1 不答应
    I2C_SDA = AckBit;

    I2C_SDA = 1;
    I2C_SDA = 0;
}
```

### 8.3 数据封装存储到内存地址的实现

> 代码实现

```h
#include "mcs51/8052.h"
#include "I2C.h"

/// 这个地址是总线地址，返回 ACK 的关键位置
#define AT24C02_Addr 0xA0

void AT24_WriteByte(unit8_t Word_Addr, unit8_t Data) {
    I2C_Start();
    // 向 I2C 总线发送设备地址
    I2C_SendByte(AT24C02_Addr);
    I2C_ReceiveAck();
    I2C_SendByte(Word_Addr);
    I2C_ReceiveAck();
    I2C_SendByte(Data);
    I2C_ReceiveAck();
    I2C_Stop();
}

unsigned char AT24_ReadByte(unit8_t Word_Addr) {
    I2C_Start();
    // 向 I2C 总线发送设备地址
    I2C_SendByte(AT24C02_Addr);
    I2C_ReceiveAck();
    I2C_SendByte(Word_Addr);
    I2C_ReceiveAck();

    I2C_Start();
    I2C_SendByte(AT24C02_Addr | 0x01);
    I2C_ReceiveAck();
    unit8_t Data = I2C_ReceiveByte();
    I2C_Stop();

    return Data;
}
```

## 9.  

