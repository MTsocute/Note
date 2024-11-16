#include "AT24C02.h"

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
