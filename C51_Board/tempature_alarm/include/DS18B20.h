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
void DS18B20_ConvertT(void);

/**
  * @brief  DS18B20读取温度
  * @param  无
  * @retval 温度数值
  */
float DS18B20_ReadT(void);


#endif //TEMPERATURE_DS18B20_H
