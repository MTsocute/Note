//
// Created by shuhe on 2024/11/4.
//

#include "UART.h"


/**
 * @brief 初始化串口，互通数据
 * */
void Uart1_Init(void) {
    PCON &= 0x7F;        // 波特率不倍速
    SCON = 0x50;        // 8位数据, 可变波特率
    TMOD &= 0x0F;       // 清除定时器1的高四位
    TMOD |= 0x20;       // 设置定时器1为8位自动重载模式
    TL1 = 0xFD;         // 设置定时初始值
    TH1 = 0xFD;         // 设置定时重载值
    ET1 = 0;            // 禁止定时器中断
    TR1 = 1;            // 定时器1开始计时

    // 触发中断
    EA = 1;
    ES = 1;
}

/**
 * @brief 单片机发送数据给电脑
 * @param Byte 发送的数据
 * */
void UART_SendByte(uint8_t Byte) {
    SBUF = Byte;
    while (TI == 0);    // 等待发送完成
    TI = 0;             // 清除发送中断标志
}


void DelayXms(uint8_t xms) {
    while (xms--) {
        unsigned char i, j;

        i = 12;
        j = 169;
        do {
            while (--j);
        } while (--i);
    }
}
