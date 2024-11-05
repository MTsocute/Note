//
// Created by shuhe on 2024/11/4.
//

#ifndef SERIAL_PORT_UART_H
#define SERIAL_PORT_UART_H

#include "mcs51/8052.h"

typedef unsigned char uint8_t; // 修正数据类型定义


void Uart1_Init(void);

// 单片机发送数据给电脑
void UART_SendByte(uint8_t Byte);

void DelayXms(uint8_t xms);

#endif //SERIAL_PORT_UART_H
