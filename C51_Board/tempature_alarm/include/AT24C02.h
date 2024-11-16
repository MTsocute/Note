#ifndef __AT24C02_H__
#define __AT24C02_H__

#include "mcs51/8052.h"
#include "I2C.h"

/// 这个地址是总线地址，返回 ACK 的关键位置
#define AT24C02_Addr 0xA0

void AT24_WriteByte(unit8_t Word_Addr, unit8_t Data);

unsigned char AT24_ReadByte(unit8_t Word_Addr);

#endif