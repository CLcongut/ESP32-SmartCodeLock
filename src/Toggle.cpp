// 头文件引用
    //自带库
#include <Arduino.h>
#include <string.h>
    //第三方库
#include <ESP32Servo.h>
#include <U8g2lib.h>
#include <u8g2_wqy.h>
    //自建库
#include "Toggle.h"
#include "TouchKeyboard.h"
#include "Display.h"
#include "FPM383F.h"
// 类对象构造
System_Control main_system;
// 定义变量
  // 全局变量
char init_pw[6];
byte option_num = 0;
const char sec_pw[6] = {'0', '0', '0', '0', '0', '0'};
const char* pw_sroage_loc[6] = {"pw_0", "pw_1", "pw_2", "pw_3", "pw_4", "pw_5"};
// 定义类成员函数
void Commands::password_compare(){
  static char password[10];
  static byte paswrd_len = 0;
  main_key_num = toggle_tkb.get_keynum();
  if(main_key_num){
    main_system.input_state = true;
    toggle_buzzer.input();
    switch(main_key_num){
      case 35:{ //确认键
        toggle_buzzer.stop();
        if(paswrd_len == 6 && memcmp(password,init_pw,6) == 0)//匹配成功,开锁
        {  
          password_display = "";
          paswrd_len=0;  
          toggle_buzzer.password_right();
          main_system.password_right = true;
        }
        else if(paswrd_len == 6 && memcmp(password,sec_pw,6) == 0)
        {
          password_display = "";
          paswrd_len=0;  
          // main_system.system_state = 2
          main_system.password_right = true;
          main_system.second_menu_ready = true;
        }
        else
        {
          password_display = "";
          paswrd_len=0;
          toggle_buzzer.password_wrong();
          main_system.password_wrong = true;
        }
      break;
      }
      case 42:{ //回退键
        if(paswrd_len > 0)
          paswrd_len--;
          password_display = password_display.substring(0,paswrd_len);
      break;
      }
      default:{ //数字输入键
        if( 48 <=main_key_num <= 57)
        {
          if(paswrd_len < 6)
          {
              password[paswrd_len++]=char(main_key_num);
              password_display += char(main_key_num);
          }
        }
      break;  
      }
    }
  }
  key_flag1 = false;
}

void Commands::finger_verify(){
  static byte finger_state;
  finger_state = toggle_finger.getFingerprintIDez();
  if(finger_state == main_system.users_number)
  {
    toggle_buzzer.password_right();
    main_system.password_right = true;    
  }
}

void Commands::NFC_verify(){
  static byte NFC_state;
  NFC_state = toggle_NFC.getrfid(main_system.users_number);
  if(NFC_state == 1)
  {
    toggle_buzzer.password_right();
    main_system.password_right = true;    
  }
}

void Commands::begin(){
  toggle_tkb.begin(4);
  Commands::users_read();
  Commands::password_read();
  Commands::custom_settings_read();
  main_system.system_state = 1;
  main_system.wrong_chance = 3;
  
}

void Commands::only_for_test(){
  // bool test1;
  // toggle_prefs.begin("password_user1");
  // toggle_prefs.putChar("pw_0", '1');
  // toggle_prefs.putChar("pw_1", '2');
  // toggle_prefs.putChar("pw_2", '3');
  // toggle_prefs.putChar("pw_3", '4');
  // toggle_prefs.putChar("pw_4", '5');
  // toggle_prefs.putChar("pw_5", '6');
  // toggle_prefs.end();

  // Serial.print(init_pw[1]);
  // toggle_prefs.begin("password_user1");
  // Serial.print(toggle_prefs.getChar("pw_0"));
  // Serial.print(toggle_prefs.getChar("pw_1"));
  // Serial.print(toggle_prefs.getChar("pw_2"));
  // Serial.print(toggle_prefs.getChar("pw_3"));
  // Serial.print(toggle_prefs.getChar("pw_4"));
  // Serial.print(toggle_prefs.getChar("pw_5"));
  // toggle_prefs.end();
  // toggle_prefs.begin("set_enables");
  // toggle_prefs.remove("EN_servo");
  // toggle_prefs.remove("EN_finger");
  // toggle_prefs.remove("EN_NFC");
  // toggle_prefs.remove("EN_lock");
  // toggle_prefs.remove("EN_beep");
  // toggle_prefs.end();

  // toggle_prefs.begin("settings_user1");
  // toggle_prefs.putBool("EN_servo", true);
  // toggle_prefs.putBool("EN_finger", true);
  // toggle_prefs.putBool("EN_NFC", true);
  // toggle_prefs.putBool("EN_lock", true);
  // toggle_prefs.putBool("EN_beep", true);
  // toggle_prefs.end();
  // toggle_prefs.begin("settings_user1");
  // Serial.print(toggle_prefs.getBool("EN_beep"));
  // toggle_prefs.putBool("EN_beep", !test1);
  // toggle_prefs.end();
}

void Commands::servo_begin(){
  servod.attach(13, 500, 2500);
}

void Commands::door_open(){
  for(int i = 0; i <= 20; i++){
    servod.write(i*6);
    delay(10);
  }
  
  delay(1000);
  delay(1000);
  delay(1000);

  for(int i = 20; i >= 0; i--){
    servod.write(i*6);
    delay(30);
  }
}

void Commands::servo_detach(){
  servod.detach();
}

void Commands::password_right(){
  toggle_oled.password_right();           //OLED播放验证通过动画
  Commands::door_open();                  //操控开门指令，也就是舵机转动
  main_system.second_menu_refresh = true; //设置二级菜单刷新标志位
  // main_system.system_state = 2;           //系统状态设为二级
}

void Commands::password_wrong(){
  main_system.wrong_chance--;             //错误机会减少一次，默认只有三次错误机会
  toggle_oled.password_wrong();           //OLED播放验证错误动画

  if(main_system.wrong_chance == 0)       //如果错误机会被消耗完
  {
      if(main_system.lock_enable) Commands::lock_in(); //触发锁机1m指令
    main_system.wrong_chance = 3;         //锁机1m后错误机会重置
  }
}

void Commands::lock_in(){
  static bool lock_state = false;         //定义锁机状态标志位
  toggle_oled.lock_in(lock_state,0);      //OLED播放锁机动画
  lock_state = true;                      //开启锁机状态
  for(short lock_time = 60; lock_time >= 0; lock_time--)
  {
    toggle_oled.lock_in(lock_state,lock_time);  //锁机倒计时显示
    delay(1000);
  }
  lock_state = false;                     //解锁锁机状态
}

// void Commands::second_menu_password(){
//   if(key_flag1){
//     static char password[10];
//     static byte paswrd_len = 0;
//     main_key_num = toggle_tkb.get_keynum();
//     if(main_key_num){
//       // toggle_buzzer.input();
//       switch(main_key_num){
//         case 35:{ //确认键
//           // toggle_buzzer.stop();
//           if(paswrd_len == 6 && memcmp(password,sec_pw,6) == 0)//匹配成功,开锁
//           {  
//             password_display = "";
//             paswrd_len=0;  
//             main_system.password_right = true;
//             main_system.second_menu_ready = true;
//           }
//           else
//           {
//             password_display = "";
//             paswrd_len=0;
//             Commands::password_wrong();
//           }
//         break;
//         }
//         case 42:{ //回退键
//           if(paswrd_len > 0)
//             paswrd_len--;
//             password_display = password_display.substring(0,paswrd_len);
//         break;
//         }
//         default:{ //数字输入键
//           if(48 <= main_key_num <= 57)
//           {
//             if(paswrd_len < 6)
//             {
//                 password[paswrd_len++]=char(main_key_num);
//                 password_display += char(main_key_num);
//             }
//         break;
//           }
//         }
//       }
//     }
//     key_flag1 = false;
//   }
// }

void Commands::second_menu_control(){
  if(key_flag1){
    short move_direction = 0;
    byte last_num_temp;
    main_key_num = toggle_tkb.get_keynum();
    if(main_key_num){
      switch (main_key_num)
      {
      case 65:
        if(option_num > 0)
        {
          last_num_temp = option_num;
          option_num--;
          move_direction = option_num - last_num_temp;
        }
      break;
      case 68:
        if(option_num < 8)
        {
          last_num_temp = option_num;
          option_num++;
          move_direction = option_num - last_num_temp;
        }
      break;
      case 35:
       game_1_successful = false;
        switch(option_num)
        {
          case 0:
            ESP.restart();
          break;

          case 1:
            Commands::password_storage();
          break;

          case 2:
            Commands::users_switch();
          break;

          case 3:
            Commands::set_enable_beep();
          break;

          case 4:
            Commands::set_enable_servo();
          break;

          case 5:
            Commands::set_enable_finger();
          break;

          case 6:
            Commands::set_enable_NFC();
          break;

          case 7:
            Commands::set_enable_lock();
          break;

          case 8:
          while(!game_1_successful)
          {
            Commands::game_1();
          }
          break;          
        }
      break;
      case 42:
      ;
      break;      
      }
      toggle_oled.second_menu(option_num, move_direction);
    }
  }
}

void Commands::game_1(){
  byte game_key_num = 0;
  byte game_random_seed;
  if(key_flag1){
    game_key_num = toggle_tkb.get_keynum();
    game_random_seed = key_flag1_freq;
    if(game_1_generate)
    {
      toggle_oled.game_1(0, game_random_seed);
    }
    if(game_key_num == 42){
      game_1_successful = true;
    }
    else if(48 < game_key_num < 58){
      if(game_key_num)
      {
        game_key_num = char(game_key_num) - '0';
        toggle_oled.game_1(game_key_num, 0);
      }
    }
  }
}

void Commands::password_storage(){
  byte pw_lens = 0;
    switch(main_system.users_number){
    case 1:
      toggle_prefs.begin("password_user1");
    break;

    case 2:
      toggle_prefs.begin("password_user2");
    break;
    }
  while(pw_lens <= 6)
  {
    if(key_flag1){
      toggle_oled.plese_change();
      main_key_num = toggle_tkb.get_keynum();
      if(main_key_num){
        switch (main_key_num)
        {
        case 35:
          // Commands::password_read();
          // Serial.print(init_pw);
          pw_lens++;
        break;

        case 42:

        break;
        
        default:
        if(pw_lens < 6)
        {
          if(48 < main_key_num < 58){

            toggle_prefs.putChar(pw_sroage_loc[pw_lens], char(main_key_num));
            password_display += char(main_key_num);            
            pw_lens++;

          }
            break;
        }
        }

      }
    }
    

  }
  toggle_prefs.end();
  password_display = "";
  delay(500);
  toggle_oled.change_done();
  delay(800);
}

void Commands::password_read(){
  switch(main_system.users_number){
    case 1:
      toggle_prefs.begin("password_user1");
    break;

    case 2:
      toggle_prefs.begin("password_user2");
    break;
  }
    for(short pw_rd = 0; pw_rd <= 5; pw_rd++)
    {
      init_pw[pw_rd] = toggle_prefs.getChar(pw_sroage_loc[pw_rd]);
    }
    toggle_prefs.end();
}

void Commands::set_enable_beep(){
  bool set_beep_enable;
  switch(main_system.users_number){
    case 1:
      toggle_prefs.begin("settings_user1");
    break;

    case 2:
      toggle_prefs.begin("settings_user2");
    break;
  }
  set_beep_enable = toggle_prefs.getBool("EN_beep");
  toggle_oled.set_enable_beep(set_beep_enable);
  toggle_prefs.putBool("EN_beep", !set_beep_enable);
  toggle_prefs.end();
  delay(1000);
}

void Commands::set_enable_servo(){
  bool set_servo_enable;
  switch(main_system.users_number){
    case 1:
      toggle_prefs.begin("settings_user1");
    break;

    case 2:
      toggle_prefs.begin("settings_user2");
    break;
  }  
  set_servo_enable = toggle_prefs.getBool("EN_servo");
  toggle_oled.set_enable_servo(set_servo_enable);
  toggle_prefs.putBool("EN_servo", !set_servo_enable);
  toggle_prefs.end();
  delay(1000);
}

void Commands::set_enable_finger(){
  bool set_finger_enable;
  switch(main_system.users_number){
    case 1:
      toggle_prefs.begin("settings_user1");
    break;

    case 2:
      toggle_prefs.begin("settings_user2");
    break;
  }
  set_finger_enable = toggle_prefs.getBool("EN_finger");
  toggle_oled.set_enable_finger(set_finger_enable);
  toggle_prefs.putBool("EN_finger", !set_finger_enable);
  toggle_prefs.end();
  delay(1000);
}

void Commands::set_enable_NFC(){
  bool set_NFC_enable;
  switch(main_system.users_number){
    case 1:
      toggle_prefs.begin("settings_user1");
    break;

    case 2:
      toggle_prefs.begin("settings_user2");
    break;
  }
  set_NFC_enable = toggle_prefs.getBool("EN_NFC");
  toggle_oled.set_enable_NFC(set_NFC_enable);
  toggle_prefs.putBool("EN_NFC", !set_NFC_enable);
  toggle_prefs.end();
  delay(1000);
}

void Commands::set_enable_lock(){
  bool set_lock_enable;
  switch(main_system.users_number){
    case 1:
      toggle_prefs.begin("settings_user1");
    break;

    case 2:
      toggle_prefs.begin("settings_user2");
    break;
  }
  set_lock_enable = toggle_prefs.getBool("EN_lock");
  toggle_oled.set_enable_lock(set_lock_enable);
  toggle_prefs.putBool("EN_lock", !set_lock_enable);
  toggle_prefs.end();
  delay(1000);
}

void Commands::custom_settings_read(){
  switch(main_system.users_number){
    case 1:
      toggle_prefs.begin("settings_user1");
    break;

    case 2:
      toggle_prefs.begin("settings_user2");
    break;
  }
  main_system.beep_enable = toggle_prefs.getBool("EN_beep");
  main_system.servo_enable = toggle_prefs.getBool("EN_servo");
  main_system.finger_enable = toggle_prefs.getBool("EN_finger");
  main_system.NFC_enable = toggle_prefs.getBool("EN_NFC");
  main_system.lock_enable = toggle_prefs.getBool("EN_lock");
  toggle_prefs.end();
}

void Commands::users_read(){
  toggle_prefs.begin("users_num");
  main_system.users_number = toggle_prefs.getUShort("users");
  toggle_prefs.end();
}

void Commands::users_switch(){
  bool switch_us_done = false;
  byte user_num = main_system.users_number;
  while(!switch_us_done){
    if(key_flag1){
      toggle_oled.users_menu(user_num, false);
      main_key_num = toggle_tkb.get_keynum();
      if(main_key_num){
        switch (main_key_num)
        {
        case 35:
          toggle_oled.users_menu(user_num, true);
          toggle_prefs.begin("users_num");
          toggle_prefs.putUShort("users", user_num);
          toggle_prefs.end();
          switch_us_done = true;
        break;

        case 42:
          switch_us_done = true;
        break;
        
        default:
          if( 48 <=main_key_num <= 57){
            user_num = char(main_key_num) - '0';
          }
        break;
        }
      }
    }
  }
}

