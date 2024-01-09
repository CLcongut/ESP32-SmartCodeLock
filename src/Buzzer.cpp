#include <Arduino.h>
#include <EasyBuzzer.h>
#include "Buzzer.h"
#include "Toggle.h"

void buzzer_done(){
    main_system.buzzer_done = true;
}

void Buzzer::begin(){
    EasyBuzzer.setPin(33);
}

void Buzzer::update(){
    EasyBuzzer.update();
}

void Buzzer::input(){
    EasyBuzzer.beep(440, 1);
}

void Buzzer::stop(){
    EasyBuzzer.stopBeep();
}

void Buzzer::password_right(){
    EasyBuzzer.beep(1100, 3, buzzer_done);
}

void Buzzer::password_wrong(){
    EasyBuzzer.beep(100, 2, buzzer_done);
}