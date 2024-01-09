/*
触摸键盘读取部分
采用内置adc读取不同电压判断按键
*/


#ifndef   _TOUCHKEYBOARD_H
#define   _TOUCHKEYBOARD_H

#include <Arduino.h>

extern bool key_flag1;
extern int key_flag1_freq;

class TouchKeyBoard{
    private:
        byte TKB_pin;//触摸键盘采集引脚，只可连接有硬件ADC通道的引脚
    public:
        /*
        @brief: 触摸键盘初始化
        @param: 触摸键盘采集引脚
        @retval: NONE
        */ 
        void begin(byte TKB_POT);
        /*
        @brief: 触摸键盘按键返回
        @param: NONE
        @retval: 键盘上内容的ASCII码，1键返回49，A键返回65
        */
        byte get_keynum();
        
};

#endif
