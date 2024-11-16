#include "Key.h"

unsigned char Key_KeyNumber;

uint8_t Key(void) {
    uint8_t Temp = Key_KeyNumber;   // 用于返回这一次的结果
    Key_KeyNumber = 0;              // 清除用于记录下一次的结果
    return Temp;
}

/**
 * @brief 独立按键按下去之后有抖动，其中抖动的消除交给了这个部分在计时器中消除
 * @brief 我们的程序是记时的，单独的消除抖动是不合理的，应该在记时的时候同时完成
 * */
void Key_Loop(void) {
    static uint8_t NowState, LastState;
    LastState = NowState;                //按键状态更新
    NowState = key_get_generate();        //获取当前按键状态
    //如果上个时间点按键按下，这个时间点未按下，则是松手瞬间，以此避免消抖和松手检测
    if (LastState == 1 && NowState == 0) {
        Key_KeyNumber = 1;
    }
    if (LastState == 2 && NowState == 0) {
        Key_KeyNumber = 2;
    }
    if (LastState == 3 && NowState == 0) {
        Key_KeyNumber = 3;
    }
    if (LastState == 4 && NowState == 0) {
        Key_KeyNumber = 4;
    }
}

/**
  * @brief  获取当前按键的状态，无消抖及松手检测
  * @param  无
  * @retval 按下按键的键码，范围：0,1~4,0表示无按键按下
  */
uint8_t key_get_generate() {
    uint8_t KeyNumber = 0;

    if (P3_1 == 0) { KeyNumber = 1; }
    if (P3_0 == 0) { KeyNumber = 2; }
    if (P3_2 == 0) { KeyNumber = 3; }
    if (P3_3 == 0) { KeyNumber = 4; }

    return KeyNumber;
}