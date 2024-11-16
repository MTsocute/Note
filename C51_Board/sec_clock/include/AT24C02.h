#ifndef __AT24C02_H__
#define __AT24C02_H__

#include "mcs51/8052.h"
#include "I2C.h"

/// 这个地址是总线地址，返回 ACK 的关键位置
#define AT24C02_Addr 0xA0

/**
 * @brief 像一个地址写入一个 8 位的数据
 * @param Word_Addr 指定的地址
 * @param Data 写入的数据
 * */
void AT24_WriteByte(unit8_t Word_Addr, unit8_t Data) {
    I2C_Start();
    // 向 I2C 总线发送设备地址
    I2C_SendByte(AT24C02_Addr);
    I2C_ReceiveAck();
    I2C_SendByte(Word_Addr);
    I2C_ReceiveAck();
    I2C_SendByte(Data);
    I2C_ReceiveAck();
    I2C_Stop();
}


/**
 * @brief 从指定的地址中获取 8 位数据
 * */
unsigned char AT24_ReadByte(unit8_t Word_Addr) {
    I2C_Start();
    // 向 I2C 总线发送设备地址
    I2C_SendByte(AT24C02_Addr);
    I2C_ReceiveAck();
    I2C_SendByte(Word_Addr);
    I2C_ReceiveAck();

    I2C_Start();
    I2C_SendByte(AT24C02_Addr | 0x01);
    I2C_ReceiveAck();
    unit8_t Data = I2C_ReceiveByte();
    I2C_Stop();

    return Data;
}

#endif