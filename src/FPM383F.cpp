#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "FPM383F.h"
#include "Toggle.h"
 
// #if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// // For UNO and others without hardware serial, we must use software serial...
// // pin #2 is IN from sensor (GREEN wire)
// // pin #3 is OUT from arduino  (WHITE wire)
// // Set up the serial port to use softwareserial..
// void(*resetFunc) (void) = 0;
// SoftwareSerial mySerial(2, 3);//连接指纹模块引脚
// #else
// // On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// // #0 is green wire, #1 is white
// #define mySerial Serial2
// #endif
// Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
// void setup()
// {
//   Serial.begin(9600);
//   while (!Serial);  // For Yun/Leo/Micro/Zero/...
//   delay(100);
//   Serial.println("\n\nAdafruit finger detect test");
//   // set the data rate for the sensor serial port
//   finger.begin(57600);
//   delay(5);
//   if (finger.verifyPassword()) {
//     Serial.println("Found fingerprint sensor!");
//   } else {
//     Serial.println("Did not find fingerprint sensor :(");
//     while (1) { delay(1); }
//   }
 
//   Serial.println(F("Reading sensor parameters"));
//   finger.getParameters();
//   Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
//   Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
//   Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
//   Serial.print(F("Security level: ")); Serial.println(finger.security_level);
//   Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
//   Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
//   Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
 
//   finger.getTemplateCount();
 
//   if (finger.templateCount == 0) {
//     Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
//   }
//   else {
//     Serial.println("Waiting for valid finger...");
//       Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
//   }
// }

uint8_t PS_SleepBuffer[12] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x33,0x00,0x37};

void Finger_Touch::begin(){
  finger.begin(57600);
  // pinMode(2, INPUT);
  // attachInterrupt(digitalPinToInterrupt(2), fingerInterrupt, RISING);

}

uint8_t Finger_Touch::getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("已获取图像");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("未检测到手指");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("通讯错误");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("成像错误");
      return p;
    default:
      Serial.println("未知错误");
      return p;
  }
 
  // OK success!
 
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("图像转换");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("图像混乱");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("通讯错误");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("找不到指纹特征");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("找不到指纹特征");
      return p;
    default:
      Serial.println("未知错误");
      return p;
  }
 
  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("找到匹配项!!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("通讯错误");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("找不到匹配指纹");
    return p;
  } else {
    Serial.println("未知错误");
    return p;
  }
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
    /*此处写指纹匹配后代码*/

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
byte Finger_Touch::getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return 0; //是否有图像传入
 
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return 0; //转换后是否是指纹特征图像
 
  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK)  return 0; //是否有找到匹配指纹
 
  // found a match!
//   Serial.print("Found ID #"); Serial.print(finger.fingerID);
//   Serial.print(" with confidence of "); Serial.println(finger.confidence); 
//   return finger.fingerID;
  return finger.fingerID;
}

void Finger_Touch::send_sleep_com(){
  mySerial.write(PS_SleepBuffer,12);
  mySerial.flush();
}