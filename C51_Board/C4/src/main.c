#include "nixie.h"
#include "key.h"
#include "Buzzer.h"
#include "timer0.h"

uint8_t Key, Matrix_Key;

int main() {
//    Timer0_Init();

    while (1) {
        Matrix_Key = MatrixKey();

        if (Matrix_Key) {
            delay_ms(20);
            Buzzer_Time(100);
            delay_ms(20);
        }
    }
}



void time0_inter() __interrupt(1) {
    TL0 = 0x18;				//设置定时初始值
    TH0 = 0xFC;				//设置定时初始值

    Buzzer_Time(1);
}