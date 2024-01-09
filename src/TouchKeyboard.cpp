// 头文件引用
    //自带库
#include <Arduino.h>
#include <TouchKeyboard.h>
#include <Ticker.h>
// 定义变量
bool key_flag1 = false;
int key_flag1_freq;
// 类构造
Ticker timer1;

// 定义函数
    // 定时器中断
void timer1_IRQN(){
  key_flag1 = true;
  key_flag1_freq++;
}

// 定义类
void TouchKeyBoard::begin(byte TKB_POT){
    TKB_pin = TKB_POT;
    pinMode(TKB_pin, INPUT);
    timer1.attach_ms(50, timer1_IRQN);//定时器启动
}

byte TouchKeyBoard::get_keynum(){
    static int key_num = 0;
    static int tkb_pot_value = 0;
    tkb_pot_value = analogRead(TKB_pin);// analogRead读取引脚模拟电压值
    while(analogRead(TKB_pin));// while中检测是否有电压，有则不会进行下一步
    if(tkb_pot_value < 300)
    {
        key_num = 0;
    }
    else if(tkb_pot_value < 340)
    {
        key_num = 52;   //"4"
    }
    else if(tkb_pot_value < 490)
    {
        key_num = 55;   //"7"
    }
    else if(tkb_pot_value < 710)
    {
        key_num = 42;   //"*"
    }
    else if(tkb_pot_value < 870)
    {
        key_num = 50;   //"2"
    }
    else if(tkb_pot_value < 1050)
    {
        key_num = 53;   //"5"
    }
    else if(tkb_pot_value < 1310)
    {
        key_num = 56;   //"8"
    }
    else if(tkb_pot_value < 1490)
    {
        key_num = 48;   //"0"
    }
    else if(tkb_pot_value < 1730)
    {
        key_num = 51;   //"3"
    }
    else if(tkb_pot_value < 1930)
    {
        key_num = 54;   //"6"
    }
    else if(tkb_pot_value < 2180)
    {
        key_num = 57;   //"9"
    }
    else if(tkb_pot_value < 2310)
    {
        key_num = 35;   //"#"
    }
    else if(tkb_pot_value < 2470)
    {
        key_num = 65;   //"A"
    }
    else if(tkb_pot_value < 2660)
    {
        key_num = 66;   //"B"
    }
    else if(tkb_pot_value < 2940)
    {
        key_num = 67;   //"C"
    }
    else if(tkb_pot_value < 3230)
    {
        key_num = 68;   //"D"
    }
    else if(tkb_pot_value < 3800)
    {
        key_num = 49;   //"1"
    }
    return key_num;
}
