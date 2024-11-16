//
// Created by shuhe on 2024/11/14.
//

#ifndef TEMPERATURE_ONEWIRE_H
#define TEMPERATURE_ONEWIRE_H

#include "Delay.h"
#include <stdint.h>
#include "mcs51/8052.h"

__sbit __at(0xB7) OneWire_DQ;

uint8_t OneWire_Init();

void OneWire_SendBit(uint8_t Bit);

uint8_t OneWire_ReceiveBit(void);

/**
  * @brief  单总线发送一个字节
  * @param  Byte 要发送的字节
  * @retval 无
  */
void OneWire_SendByte(unsigned char Byte);

/**
  * @brief  单总线接收一个字节
  * @param  无
  * @retval 接收的一个字节
  */
uint8_t OneWire_ReceiveByte(void);

#endif //TEMPERATURE_ONEWIRE_H
