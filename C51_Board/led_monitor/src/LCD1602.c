#include "LCD1602.h"

// 延时函数
void LCD_Delay(void) {
    unsigned char i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 239; j++);
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
