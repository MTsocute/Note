#include "I2C.h"

/** Document
 * 按照道理来说，单片机 I2C 的转换之间肯定需要延迟
 * 你看手册就知道了，但是我们的 C51 非常的慢，做不到那么快的延迟
 * 所以我们的高低电平转换之间不写延迟
 * */


void I2C_Start(void) {
    // 起始条件：SCL 高电平期间，SDA 从 高电平 切换到 低电平
    I2C_SDA = 1;
    I2C_SCL = 1;

    I2C_SDA = 0;
    I2C_SCL = 0;
}

void I2C_Stop(void) {
    // 停止条件是：SCL 高电平期间， SDA 从 低电平 到 高电平
    I2C_SCL = 0;
    I2C_SDA = 0;

    I2C_SCL = 1;
    I2C_SDA = 1;
}

void I2C_SendByte(unit8_t Byte) {
    // 循环 8 个 周期
    for (unit8_t i = 0; i < 8; ++i) {
        // SDA 快点，所以我们放前面
        // 外面获取了 8 个字节长度的数据，我们读取每一次
        I2C_SDA = Byte & (0x80 >> i);      // 取高位
        I2C_SCL = 0;

        // 一个数据的结束
        // start
        I2C_SCL = 1;
        I2C_SCL = 0;
        // end
    }
}

unit8_t I2C_ReceiveByte(void) {
    unit8_t Byte = 0x00;

    for (unit8_t i = 0; i < 8; i ++) {
        I2C_SCL = 0;
        I2C_SCL = 1;    // 一个数据的开始
        /*
         * 我们是每一次从外面接受到一个数据，然后数据要存储到 Byte 中
         * 对于外部数据，要么 1 | 0，所以 | 就一定会存储 1
         * 而 | 0 还是 0，所以相当于把外面的数据一个个存储到对应位上
         * */
        if (I2C_SDA) { Byte |= (0x80 >> i); }
        I2C_SCL = 0;    // 一个数据的结束
    }

    return Byte;
}

unit8_t I2C_ReceiveAck(void) {
    // 从外部接受收到一个报文
    unit8_t AckBit;
    I2C_SCL = 0;
    I2C_SDA = 1;
    // 系统始终开始
    I2C_SCL = 1;

    // 把报文赋值给一个内部变量
    AckBit = I2C_SDA;

    // 系统时钟结束
    I2C_SCL = 0;
    return AckBit;
}

void I2C_SendAck(unit8_t AckBit) {
    // 发送一个报文，0 是答应，1 不答应
    I2C_SDA = AckBit;

    I2C_SDA = 1;
    I2C_SDA = 0;
}
