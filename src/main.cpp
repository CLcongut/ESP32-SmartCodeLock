// 头文件引用
    //自带库
#include <Arduino.h>
#include <Ticker.h>
    //第三方库

    //自建库
#include "TouchKeyboard.h"
#include "Toggle.h"
#include "Display.h"
#include "Buzzer.h"
#include "FPM383F.h"
// 定义变量

// 类对象构造
TouchKeyBoard main_tkb;
OLEDDisplay main_oled;
Commands main_cmd;
Buzzer main_buzzer;
Finger_Touch main_finger;
NFC522 main_NFC;
// 启动部分
void setup() {
// 初始化代码
Serial.begin(9600);
    // OLED配置
main_oled.begin();
    // 命令配置
main_cmd.begin();
    // 蜂鸣器配置
if(main_system.beep_enable) main_buzzer.begin();
    //指纹配置
if(main_system.finger_enable) main_finger.begin();
// main_finger.send_sleep_com();
    //NFC配置
if(main_system.NFC_enable) main_NFC.begin();
// 加载动画
    // main_oled.loading_animation();
// main_system.users_number = 1;
}

// 主循环部分
void loop() {
// 此时可输入密码

    if(main_system.system_state == 1)
    {
        main_oled.plese_enter();
        if(key_flag1)
        {
            main_cmd.password_compare();

            if(!main_system.input_state)
            {
                if(main_system.finger_enable) main_cmd.finger_verify();

                if(main_system.NFC_enable) main_cmd.NFC_verify();
            }
        }
        if(main_system.buzzer_done && main_system.password_right)
        {
            main_buzzer.stop();                 //停止蜂鸣器
            main_system.buzzer_done = false;    //清空蜂鸣器播放完成标志位
            if(main_system.servo_enable) main_cmd.servo_begin(); //舵机操控初始化
            main_cmd.password_right();          //验证成功函数
            main_system.password_right = false; //清空验证成功函数
            main_system.input_state = false;
            main_cmd.servo_detach();
            return;
        }
        else if(main_system.buzzer_done && main_system.password_wrong)
        {
            main_system.buzzer_done = false;    //清空蜂鸣器播放完成标志位
            main_cmd.password_wrong();          //验证失败函数
            main_system.password_wrong = false; //清空验证失败函数
        }
        else if(main_system.password_right && main_system.second_menu_ready)
        {
            main_system.system_state = 2;       //系统进入二级状态
            main_system.second_menu_refresh = true; //二级菜单准备刷新
            main_system.wrong_chance = 3;
            return;
        }
    }
    else if(main_system.system_state == 2)
    {
        if(main_system.second_menu_ready)
        {
            if(main_system.second_menu_refresh)
            {
                main_oled.second_menu(0, 0);
                main_system.second_menu_refresh = false;
            }
            main_cmd.second_menu_control();
        }
    }

// main_cmd.only_for_test();
// delay(1000);

// main_oled.users_menu(2, false);
// main_cmd.users_switch();

// main_finger.getFingerprintID();
// main_finger.getFingerprintIDez();
// delay(50);

// main_cmd.second_menu_control();
// main_oled.second_menu(8 , 0);
    // main_oled.password_right();
    // main_cmd.password_right();
    // main_oled.lock_in();
    // main_cmd.lock_in();
    // main_oled.second_menu(0 , 0);
    // main_oled.game_1(0 ,0);
    // if(!game_1_successful)
    // {
    //     main_cmd.game_1();
    // }
    // main_cmd.password_storage();

    // main_oled.plese_change();
    // main_oled.change_done();
    // Serial.print(init_pw);
    // main_cmd.only_for_test();
    // delay(1000);
    main_buzzer.update();
}

