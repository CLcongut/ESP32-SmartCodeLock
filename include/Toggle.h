#ifndef   _TOGGLE_H
#define   _TOGGLE_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "TouchKeyboard.h"
#include "Display.h"
#include <Preferences.h>
#include "Buzzer.h"
#include "FPM383F.h"
#include "NFC522.h"

extern char init_pw[6];

struct System_Control{
  bool beep_enable;
  bool servo_enable;
  bool finger_enable;
  bool NFC_enable;
  bool lock_enable;
  
  byte system_state;
  bool second_menu_refresh;
  byte wrong_chance;
  bool buzzer_done;
  bool password_right;
  bool password_wrong;
  bool second_menu_ready;
  bool input_state;
  // byte fp_id_users;
  short users_number;
};

extern System_Control main_system;

class Commands{
    private:
      TouchKeyBoard toggle_tkb;
      OLEDDisplay toggle_oled;
      Preferences toggle_prefs;
      Servo servod;
      byte main_key_num;
      Buzzer toggle_buzzer;
      Finger_Touch toggle_finger;
      NFC522 toggle_NFC;

    public:
      void begin();
      void only_for_test();
      void servo_begin();
      void door_open();
      void servo_detach();
      void lock_in();
      void password_compare();
      void finger_verify();
      void NFC_verify();
      void password_right();
      void password_wrong();
      // void second_menu_password();
      void second_menu_control();
      void game_1();
      void password_storage();
      void password_read();
      void set_enable_beep();
      void set_enable_servo();
      void set_enable_finger();
      void set_enable_NFC();
      void set_enable_lock();
      void custom_settings_read();
      void users_read();
      void users_switch();
};

void pw_compare();
#endif