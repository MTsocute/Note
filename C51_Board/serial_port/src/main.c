#include "UART.h"

uint8_t Sec;

int main() {
    Uart1_Init();


    while (1) {
        if(P3_1 == 0) {
            DelayXms(28); while (P3_1 == 0); DelayXms(20);
            // receive from board
            UART_SendByte('c');
            DelayXms(20);
        }
    }
}

void UART_board_receive() __interrupt(4) {
    // receive interrupt 发生
    if (RI == 1) {
        // 使用完数据之后反向发送回电脑
        UART_SendByte(SBUF);
        RI = 0;         // 清除接收标志位
    }
}