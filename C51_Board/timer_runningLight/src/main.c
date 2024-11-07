#include "matrixKey.h"
//#include "move.h"

volatile unsigned char keyNum, LED_MODE;


int main() {
    P2 = 0xFE;
    Timer0_Init();

    while (1) {
        keyNum = interrupt_key();
        // make sure that you've pressed the key
        if (keyNum) {
            if (keyNum == 1) {
                LED_MODE++;
                if (LED_MODE >= 2) LED_MODE = 0;
            }
        }
    }
}

// 在中断函数中尽量只做简单的操作，不然内存和时间很容易不够
void time0() __interrupt(TF0_VECTOR) {
    static unsigned int T0Count;
    // 否则写一个记时的外部变量，来控制终断的时间，给予更多的时间

    TL0 = 0x18;                //设置定时初始值
    TH0 = 0xFC;                //设置定时初始值

    // 1ms x 500 = 0.5s
    T0Count++;      // 发生一次终端就会来到这个函数，然后记数 + 1；发生 1k 次就是 1s 了，然后执行我们要做的

    if (T0Count >= 500) {
        T0Count = 0;
        // 直接在这里实现循环左移和右移
        if (LED_MODE == 0) {
            P2 = (P2 << 1) | (P2 >> 7);  // 循环左移
        } else if (LED_MODE == 1) {
            P2 = (P2 >> 1) | (P2 << 7);  // 循环右移
        }
    }
}