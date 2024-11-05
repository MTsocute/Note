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
    LCD_SetCursor(Line, Column); // 设置光标位置
    LCD_ShowString(Line, Column, "Hex: "); // 显示标签

    unsigned char hex[3]; // 存储十六进制字符串（2位 + 结束符）
    sprintf(hex, "%02X", num); // 将十进制数转换为十六进制
    LCD_ShowString(Line, Column + 5, hex); // 显示十六进制数
}

// 显示 unsigned int 0~2^16-1
void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int num) {
    // 设置光标位置
    LCD_SetCursor(Line, Column);

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

// 清空LCD屏幕上特定区域的内容
void LCD_ClearRegion(unsigned char Line, unsigned char Column, unsigned char length) {
    LCD_SetCursor(Line, Column); // 设置光标位置
    for (unsigned char i = 0; i < length; i++) {
        LCD_WriteData(' '); // 用空格覆盖字符
    }
}

