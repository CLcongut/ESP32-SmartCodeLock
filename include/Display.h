#ifndef   _DISPLAY_H
#define   _DISPLAY_H

#include <Arduino.h>
#include "Buzzer.h"

extern String password_display;
extern bool game_1_generate;
extern bool game_1_successful;

class OLEDDisplay{
    private:
    Buzzer oled_buzzer;
    ;
    public:
        void begin();
        void clearbuffer();
        void loading_animation();
        void plese_enter();
        void password_right();
        void password_wrong();
        void lock_in(bool lock_state, short lock_time);
        void second_menu(byte option_num ,short move_direction);
        void game_1(byte key_num, int random_seeds);
        void plese_change();
        void change_done();
        void set_enable_beep(bool set);
        void set_enable_servo(bool set);
        void set_enable_finger(bool set);
        void set_enable_NFC(bool set);
        void set_enable_lock(bool set);
        void users_menu(byte users_num, bool confirm);
};

#endif
