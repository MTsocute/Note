//
// Created by shuhe on 2024/11/14.
//

#ifndef TEMPERATURE_DS18B20_H
#define TEMPERATURE_DS18B20_H

#include "OneWire.h"

#define DS18B20_SKIP_ROM            0xCC
#define DS18B20_CONVERT_T            0x44
#define DS18B20_READ_SCRATCHPAD    0xBE

/**
  * @brief  DS18B20开始温度变换
  * @param  无
  * @retval 无
  */
void DS18B20_ConvertT(void) {
    OneWire_Init();
    OneWire_SendByte(DS18B20_SKIP_ROM);
    OneWire_SendByte(DS18B20_CONVERT_T);
}

/**
  * @brief  DS18B20读取温度
  * @param  无
  * @retval 温度数值
  */
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


#endif //TEMPERATURE_DS18B20_H
