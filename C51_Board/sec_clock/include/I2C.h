#ifndef __I2C_H__
#define __I2C_H__

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


#endif
