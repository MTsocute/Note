//
// Created by shuhe on 2024/11/16.
//

#ifndef AD_TS_LCD1602_H
#define AD_TS_LCD1602_H

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


#endif //AD_TS_LCD1602_H
