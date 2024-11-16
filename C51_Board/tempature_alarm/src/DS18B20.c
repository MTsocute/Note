//
// Created by shuhe on 2024/11/14.
//

#include "DS18B20.h"

void DS18B20_ConvertT(void) {
    OneWire_Init();
    OneWire_SendByte(DS18B20_SKIP_ROM);
    OneWire_SendByte(DS18B20_CONVERT_T);
}

float DS18B20_ReadT(void) {
    unsigned char TLSB, TMSB;
    int Temp;
    OneWire_Init();
    OneWire_SendByte(DS18B20_SKIP_ROM);
    OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
//    LSB (TLSB): 温度数据的低字节。
//    MSB (TMSB): 温度数据的高字节。
    TLSB = OneWire_ReceiveByte();
    TMSB = OneWire_ReceiveByte();
    Temp = (TMSB << 8) | TLSB;

    // 当我们将这两个字节组合成 Temp 时，Temp 的单位是 1/16 摄氏度
    return Temp / 16.0;
}
