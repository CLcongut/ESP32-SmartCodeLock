#include <Arduino.h>
#include <U8g2lib.h>
#include <u8g2_wqy.h>
#include "Display.h"
#include "Toggle.h"

String password_display = "";
bool game_1_generate = true;
byte layout_array[3][3];
byte game_1_steps;
bool game_1_successful = false;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0);

void OLEDDisplay::begin(){
    oled.begin();
    oled.enableUTF8Print();
}
void OLEDDisplay::clearbuffer(){
    oled.clearBuffer();
    oled.sendBuffer();
}
void OLEDDisplay::loading_animation(){
    oled.setFont(u8g2_font_wqy14_t_gb2312a);
    for(short box_progress = 0;box_progress<=118; box_progress++)
    {
        oled.clearBuffer();
        oled.drawUTF8(15, 48, "系统加载中");
        if(box_progress < 14)
        {
            ;
        }
        else if(box_progress < 30)
        {
            oled.drawUTF8(85, 48, ".");
        }
        else if(box_progress < 44)
        {
            oled.drawUTF8(85, 48, "..");
        }
        else if(box_progress < 59)
        {
            oled.drawUTF8(85, 48, "...");
        }
        else if(box_progress < 73)
        {
            ;
        }
        else if(box_progress < 88)
        {
            oled.drawUTF8(85, 48, ".");
        }
        else if(box_progress < 103)
        {
            oled.drawUTF8(85, 48, "..");
        }
        else
        {
            oled.drawUTF8(85, 48, "...");
        }
        oled.drawFrame(0, 10, 128, 20);
        oled.drawFrame(1, 11, 126, 18);
        oled.drawBox(5, 15, box_progress, 10);
        oled.sendBuffer();
        delay(10);
    }
}

void OLEDDisplay::plese_enter(){
    oled.clearBuffer();
    oled.setFont(u8g2_font_wqy16_t_gb2312a);
    oled.drawUTF8(0, 15, "请输入密码:");
    oled.setCursor(30,40);
    oled.print(password_display);
    static short display_flag1 = 0;
    display_flag1++;
    if((password_display.length() == 0) && ((display_flag1%16)<5))
    {
        oled.drawUTF8(30,40, "_");
    }
    else if((password_display.length() == 1) && ((display_flag1%16)<5))
    {
        oled.drawUTF8(38,40, "_");
    }
    else if((password_display.length() == 2) && ((display_flag1%16)<5))
    {
        oled.drawUTF8(46,40, "_");
    }
    else if((password_display.length() == 3) && ((display_flag1%16)<5))
    {
        oled.drawUTF8(54,40, "_");
    }
    else if((password_display.length() == 4) && ((display_flag1%16)<5))
    {
        oled.drawUTF8(62,40, "_");
    }
    else if((password_display.length() == 5) && ((display_flag1%16)<5))
    {
        oled.drawUTF8(70,40, "_");
    }
    else if((password_display.length() == 6) && ((display_flag1%16)<5))
    {
        oled.drawUTF8(70,40, "_");
    }
    oled.sendBuffer(); 

}

void OLEDDisplay::password_right(){
    oled.setFont(u8g2_font_wqy16_t_gb2312a);
    for(short string_progress = 25; string_progress >= 0; string_progress--)
    {
        oled.clearBuffer();
        oled.drawUTF8(0, 15 - (75 - string_progress*3), "请输入密码:");
        oled.drawUTF8(30, 15 + string_progress*3, "验证通过!");
        oled.drawUTF8(30, 42 + string_progress*3, "正在开门!");
        oled.sendBuffer();
    }
    for(short box_progress = 0;box_progress <= 39; box_progress++)
    {
        oled.clearBuffer();
        oled.drawUTF8(30, 15, "验证通过!");
        oled.drawUTF8(30, 42, "正在开门!");
        oled.drawBox(5, 53, box_progress*3, 8);
        oled.sendBuffer();
    } 
}

void OLEDDisplay::password_wrong(){
    oled.setFont(u8g2_font_wqy16_t_gb2312a);
    for(short string_progress = 25; string_progress >= 0; string_progress--)
    {
        oled.clearBuffer();
        oled.drawUTF8(0, 15 + (75 - string_progress*3), "请输入密码:");
        oled.drawUTF8(30, 15 - string_progress*3, "密码错误!");
        oled.drawUTF8(30, 42 - string_progress*3, "你还有  次机会!");
        oled.setCursor(80, 42 - string_progress*3);
        oled.print(main_system.wrong_chance);
        oled.sendBuffer();
        if(string_progress == 0)
        {
            delay(300);
        }
    }
    for(short string_progress = 0; string_progress <= 25; string_progress++)
    {
        oled.clearBuffer();
        if(main_system.wrong_chance > 0)
        {
        oled.drawUTF8(0, 15 + (75 - string_progress*3), "请输入密码:");
        }
        oled.drawUTF8(30, 15 - string_progress*3, "密码错误!");
        oled.drawUTF8(30, 42 - string_progress*3, "你还有  次机会!");
        oled.setCursor(80, 42 - string_progress*3);
        oled.print(main_system.wrong_chance);
        oled.sendBuffer();      
    }
    
}

void OLEDDisplay::lock_in(bool lock_state, short lock_time){
    if(!lock_state)
    {
        oled.setFont(u8g2_font_wqy16_t_gb2312a);
        oled.clearBuffer();
        oled.drawUTF8(5, 30, "验证次数已用完!");
        oled.drawUTF8(30, 50, "即将锁机!");
        oled.sendBuffer(); 
        delay(800);
        for(short box_progress = 0;box_progress<=64; box_progress++)
        {
            oled.clearBuffer();
            oled.drawUTF8(5, 30, "验证次数已用完!");
            oled.drawUTF8(30, 50, "即将锁机!");
            oled.drawBox(0, 0, box_progress, 64);
            oled.drawBox(128 - box_progress, 0, box_progress, 64);
            oled.sendBuffer(); 
        }
        delay(300);
    }
    else
    {
        oled.clearBuffer();
        oled.drawUTF8(30, 30, "正在锁机!");
        oled.drawUTF8(42, 50, "秒后复原!");
        oled.setCursor(20,50);
        oled.print(lock_time);
        oled.sendBuffer(); 
    }
}

void OLEDDisplay::second_menu(byte option_num, short move_direction){
    oled.setFont(u8g2_font_wqy12_t_gb2312a);
    String menu_options[] = {
        "快速重启",
        "编辑验证密码",
        "切换用户",
        "开关蜂鸣器",
        "开关舵机",
        "开关指纹验证",
        "开关NFC验证",
        "开关锁机",
        "小游戏"};
    byte options_lens[] = {4, 6, 4, 5, 4, 6, 6, 4, 3};
    byte string_distance = 16;
    byte move_position;
    byte original_position = 13;
    short last_direction;
if((option_num <= 2 || option_num >= 6) && !(option_num == 2 && move_direction < 0 || option_num == 6 && move_direction > 0))
{
    if(option_num <= 2)
    {
        move_position = 0;
    }
    else if(option_num >= 6)
    {
        move_position = 80;
    }
    if(move_direction > 0)
    {
        for(short box_move_progress = 0; box_move_progress <= 8; box_move_progress++)
        {
            oled.clearBuffer();
            oled.drawBox(9, 2 + string_distance * 
            (option_num - move_direction)  - move_position + box_move_progress * 2, 119, 14);
            for(short menu_option_num = 0; menu_option_num <= 8; menu_option_num++)
            {
                if(menu_option_num == option_num && box_move_progress == 8)
                {
                    oled.setCursor(10,original_position - move_position + string_distance * (option_num - move_direction)); 
                    oled.print(menu_options[option_num - move_direction]);
                    oled.setDrawColor(0);
                    oled.setCursor(10,original_position - move_position + string_distance * menu_option_num); 
                    oled.print(menu_options[menu_option_num]);
                    oled.setDrawColor(1);
                }
                else
                {
                    oled.setCursor(10,original_position - move_position + string_distance * menu_option_num); 
                    oled.print(menu_options[menu_option_num]);
                }
            }
            oled.sendBuffer(); 
        }
    }
    if(move_direction < 0)
    {
        for(short box_move_progress = 0; box_move_progress <= 8; box_move_progress++)
        {
            oled.clearBuffer();
            oled.drawBox(9, 2 + string_distance * 
            (option_num - move_direction) - move_position - box_move_progress * 2, 119, 14);
            for(short menu_option_num = 0; menu_option_num <= 8; menu_option_num++)
            {
                if(menu_option_num == option_num && box_move_progress == 8)
                {
                    oled.setCursor(10,original_position - move_position + string_distance * (option_num - move_direction)); 
                    oled.print(menu_options[option_num - move_direction]);
                    oled.setDrawColor(0);
                    oled.setCursor(10,original_position - move_position + string_distance * menu_option_num); 
                    oled.print(menu_options[menu_option_num]);
                    oled.setDrawColor(1);
                }
                else
                {
                    oled.setCursor(10,original_position - move_position + string_distance * menu_option_num); 
                    oled.print(menu_options[menu_option_num]);
                }
            }
            oled.sendBuffer(); 
        }
    }
    else
    {
        oled.clearBuffer();
        for(short rest_menu_display = 0; rest_menu_display <= 8; rest_menu_display++)
        {
            if(rest_menu_display == option_num)
            {
                oled.drawBox(9, 2 + string_distance * option_num - move_position, 119, 14);
                oled.setDrawColor(0);
                oled.setCursor(10,original_position - move_position + string_distance * option_num); 
                oled.print(menu_options[option_num]);
                oled.setDrawColor(1);
            }
            else
            {
                oled.setCursor(10,original_position - move_position + string_distance * rest_menu_display); 
                oled.print(menu_options[rest_menu_display]);
            }
        }
        oled.sendBuffer();
    }
}
else if(2 < option_num <= 6 && move_direction >= 0)
{
    for(short menu_move_quantity = 0; menu_move_quantity <= 8; menu_move_quantity++)
    {
        oled.clearBuffer();
        move_position = string_distance * (option_num - 3) + menu_move_quantity * 2;
        oled.drawBox(9, 2 + string_distance * 2, 119, 14);
        for(short rest_menu_display = 0; rest_menu_display <= 8; rest_menu_display++)
        {
            if(rest_menu_display == option_num && menu_move_quantity == 8)
            {
                oled.setCursor(10,original_position - move_position + string_distance * (option_num - move_direction)); 
                oled.print(menu_options[option_num - move_direction]);
                oled.setDrawColor(0);                    
                oled.setCursor(10,original_position - move_position + string_distance * option_num); 
                oled.print(menu_options[option_num]);
                oled.setDrawColor(1);
            }
            else
            {
                oled.setCursor(10,original_position - move_position + string_distance * rest_menu_display); 
                oled.print(menu_options[rest_menu_display]);
            }
        }
        oled.sendBuffer();
    }
    last_direction = move_direction;

}
else if(2 <= option_num < 6 && move_direction <= 0)
{
    for(short menu_move_quantity = 0; menu_move_quantity <= 8; menu_move_quantity++)
    {
        oled.clearBuffer();
        move_position = string_distance * (option_num + 0) - menu_move_quantity * 2;
        oled.drawBox(9, 2 + string_distance * 1, 119, 14);
        for(short rest_menu_display = 0; rest_menu_display <= 8; rest_menu_display++)
        {
            if(rest_menu_display == option_num && menu_move_quantity == 8)
            {
                oled.setCursor(10,original_position - move_position + string_distance * (option_num - move_direction)); 
                oled.print(menu_options[option_num - move_direction]);
                oled.setDrawColor(0);                    
                oled.setCursor(10,original_position - move_position + string_distance * option_num); 
                oled.print(menu_options[option_num]);
                oled.setDrawColor(1);
            }
            else
            {
                oled.setCursor(10,original_position - move_position + string_distance * rest_menu_display); 
                oled.print(menu_options[rest_menu_display]);
            }
        }
        oled.sendBuffer();
    }
    last_direction = move_direction;
}
else if(2 < option_num < 6 && move_direction == 0)
{
    static bool rest_move_position;
    if(last_direction > 0)
    {
        rest_move_position = 1;
    }
    else if(last_direction < 0)
    {
        rest_move_position = 0;
    }
    oled.clearBuffer();
    move_position = string_distance * (option_num - (1 + rest_move_position));
    for(short rest_menu_display = 0; rest_menu_display <= 8; rest_menu_display++)
    {

        if(rest_menu_display == option_num)
        {
            oled.drawBox(9, 2 + string_distance * option_num - move_position, 119, 14);
            oled.setDrawColor(0);
            oled.setCursor(10,original_position - move_position + string_distance * option_num); 
            oled.print(menu_options[option_num]);
            oled.setDrawColor(1);
        }
        else
        {
            oled.setCursor(10,original_position - move_position + string_distance * rest_menu_display); 
            oled.print(menu_options[rest_menu_display]);
        }
    }
    oled.sendBuffer();
}

}

void OLEDDisplay::game_1(byte key_num, int random_seeds){
    randomSeed(random_seeds);
    short layout_num;
    byte layout_move = 0;
    byte key_num_close;
    byte box_length = 25;
    byte box_width = 16;
    byte box_horizontal_distance = 30;
    byte box_vertically_distance = 21;
    byte box_original_x = 41;
    byte box_original_y = 3;
    byte game_1_success1[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    layout_num = random(0,512);
    if(game_1_generate == true)
    {
        while(layout_num > 0)
        {
            layout_array[(layout_move / 3) % 3][layout_move % 3] = layout_num % 2;
            layout_move++;
            layout_num = layout_num / 2;
        }
        game_1_generate = false;
    }
    oled.clearBuffer();
    oled.setFont(u8g2_font_wqy12_t_chinese1);
    oled.drawBox(35, 0, 2, 64);
    oled.drawStr(0, 20, "Steps:");
    oled.setCursor(5, 32);
    oled.print(game_1_steps);
    if(key_num)
    {
        key_num--;
        if(key_num - 3 >= 0){
            layout_array[((key_num - 3) / 3) % 3][(key_num - 3) % 3] = !layout_array[((key_num - 3) / 3) % 3][(key_num - 3) % 3]; // 上
        }
        if(key_num + 3 <= 8){
            layout_array[((key_num + 3) / 3) % 3][(key_num + 3) % 3] = !layout_array[((key_num + 3) / 3) % 3][(key_num + 3) % 3]; // 下
        }

        layout_array[(key_num / 3) % 3][key_num % 3] = !layout_array[(key_num / 3) % 3][key_num % 3];// 中

        if(key_num - 1 >= 0 && key_num % 3 != 0){
            layout_array[((key_num - 1) / 3) % 3][(key_num - 1) % 3] = !layout_array[((key_num - 1) / 3) % 3][(key_num - 1) % 3]; // 左
        }
        if(key_num + 1 <= 8 && key_num % 3 != 2){
            layout_array[((key_num + 1) / 3) % 3][(key_num + 1) % 3] = !layout_array[((key_num + 1) / 3) % 3][(key_num + 1) % 3]; // 右
        }
        game_1_steps++;
    }
    for(short x_move = 0; x_move <= 2; x_move++)
    {
        for(short y_move = 0; y_move <= 2; y_move++)
        {
            oled.setDrawColor(layout_array[y_move][x_move]);
            oled.drawBox(box_original_x + x_move * box_horizontal_distance, 
            box_original_y + y_move * box_vertically_distance, 
            box_length, box_width);
            oled.setDrawColor(1);
            oled.drawFrame(box_original_x + x_move * box_horizontal_distance, 
            box_original_y + y_move * box_vertically_distance, 
            box_length, box_width);
        }
    }
    oled.sendBuffer();
    if(memcmp(layout_array, game_1_success1, 9) == 0)
    {
        delay(500);
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy16_t_gb2312a);
        oled.drawUTF8(5, 16, "游戏成功!");
        oled.drawUTF8(5, 32, "共用步数:");
        oled.setCursor(5, 48);
        oled.print(game_1_steps);
        oled.sendBuffer();
        game_1_successful = true;
        game_1_steps = 0;
        delay(800);
    }
}

void OLEDDisplay::plese_change(){
    oled.clearBuffer();
    oled.setFont(u8g2_font_wqy16_t_gb2312a);
    oled.drawUTF8(0, 15, "请输入更改密码:");
    oled.setCursor(30,40);
    oled.print(password_display);
    oled.sendBuffer(); 

}

void OLEDDisplay::change_done(){
    oled.clearBuffer();
    oled.setFont(u8g2_font_wqy16_t_gb2312a);
    oled.drawUTF8(10, 32, "密码更改完成!");
    oled.sendBuffer(); 
}

void OLEDDisplay::set_enable_beep(bool set){
    if(set)
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "蜂鸣器切换为关闭");
        oled.sendBuffer(); 
    }
    else
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "蜂鸣器切换为开启");
        oled.sendBuffer();         
    }
}

void OLEDDisplay::set_enable_servo(bool set){
    if(set)
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "舵机切换为关闭");
        oled.sendBuffer(); 
    }
    else
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "舵机切换为开启");
        oled.sendBuffer();         
    }
}

void OLEDDisplay::set_enable_finger(bool set){
    if(set)
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "指纹切换为关闭");
        oled.sendBuffer(); 
    }
    else
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "指纹切换为开启");
        oled.sendBuffer();         
    }
}

void OLEDDisplay::set_enable_NFC(bool set){
    if(set)
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "NFC切换为关闭");
        oled.sendBuffer(); 
    }
    else
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "NFC切换为开启");
        oled.sendBuffer();         
    }
}

void OLEDDisplay::set_enable_lock(bool set){
    if(set)
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "锁机切换为关闭");
        oled.sendBuffer(); 
    }
    else
    {
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "锁机切换为开启");
        oled.sendBuffer();         
    }
}

void OLEDDisplay::users_menu(byte users_num, bool confirm){
    String users[] = {"用户1", "用户2"};
    byte num_users = 2;
    byte string_distance = 16;
    byte original_position = 13;
    if(confirm){
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy14_t_gb2312a);
        oled.drawUTF8(10, 32, "切换至用户");
        oled.setCursor(84, 32);
        oled.print(users_num);
        oled.sendBuffer();
        delay(800);
    }
    else{
        oled.clearBuffer();
        oled.setFont(u8g2_font_wqy12_t_gb2312a);
        for(short us = 0; us < num_users; us++)
        {
            if(users_num - 1 == us)
            {
                oled.drawBox(9, 2 + string_distance * us, 119, 14);
                oled.setDrawColor(0);
                oled.setCursor(10,original_position + string_distance * us);  
                oled.print(users[us]);
                oled.setDrawColor(1);
            }
            else
            {
                oled.setCursor(10,original_position + string_distance * us); 
                oled.print(users[us]);
            }
        }
        oled.sendBuffer();
    }
}
