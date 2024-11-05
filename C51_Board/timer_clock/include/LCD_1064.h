//
// Created by shuhe on 2024/11/4.
//

#ifndef TIMER_CLOCK_LCD_1064_H
#define TIMER_CLOCK_LCD_1064_H

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
void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int num, unsigned char length);

#endif //TIMER_CLOCK_LCD_1064_H
