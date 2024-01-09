#ifndef   _FPM383F_H
#define   _FPM383F_H

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

#define mySerial Serial2

class Finger_Touch{

    private:
        Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
    public:
        void begin();
        uint8_t getFingerprintID();
        byte getFingerprintIDez();
        void send_sleep_com();

};

#endif