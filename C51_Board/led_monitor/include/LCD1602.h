#ifndef LCD1602_H
#define LCD1602_H

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
void LCD_ShowChar(unsigned char Line, unsigned char Column, unsigned char Char);
void LCD_ShowString(unsigned char Line, unsigned char Column, unsigned char *String);

#endif // LCD1602_H
