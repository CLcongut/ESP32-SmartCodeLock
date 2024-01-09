#ifndef   _NFC522_H
#define   _NFC522_H

#include <Arduino.h>
#include <SPI.h>
#include "MFRC522.h"

// #define SS_PIN 5
// #define RST_PIN 17

class NFC522{
    private:
        MFRC522::MIFARE_Key key; 
        byte users_num;

    public:
        void begin();
        byte getrfid(byte users);
        void reset();
};

#endif