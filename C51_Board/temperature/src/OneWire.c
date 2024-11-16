//
// Created by shuhe on 2024/11/14.
//

#include "../include/OneWire.h"

uint8_t OneWire_Init() {
    unsigned char i;
    OneWire_DQ = 1;
    OneWire_DQ = 0;
    i = 227; while (--i);       // Delay 500 us
    OneWire_DQ = 1;             // 延迟之后拉高一段时间
    i = 29; while (--i);        // Delay 70 us
    uint8_t AckBit = OneWire_DQ;
    i = 227; while (--i);       // Delay 500 us
    return AckBit;
}

void OneWire_SendBit(uint8_t Bit) {
    unsigned char i;
    OneWire_DQ=0;               // Ready for Receive
    i = 4;while (--i);			//Delay 10us
    OneWire_DQ=Bit;
    i = 24;while (--i);			//Delay 50us
    OneWire_DQ=1;               // Send Complete
}

uint8_t OneWire_ReceiveBit(void) {
    unsigned char i, Bit;
    OneWire_DQ=0;               // Ready for Receive
    i = 2;while (--i);			//Delay 5us
    OneWire_DQ=1;               // Receive Complete
    i = 2;while (--i);			//Delay 5us
    Bit=OneWire_DQ;             // Get what we've received
    i = 24;while (--i);			//Delay 50us
    return Bit;                 // return it
}

void OneWire_SendByte(unsigned char Byte) {
    unsigned char i;
    for(i=0;i<8;i++)
    {
        OneWire_SendBit(Byte&(0x01<<i));
    }
}

unsigned char OneWire_ReceiveByte(void) {
    unsigned char i;
    unsigned char Byte=0x00;
    for(i=0;i<8;i++)
    {
        if(OneWire_ReceiveBit()){Byte|=(0x01<<i);}
    }
    return Byte;
}
