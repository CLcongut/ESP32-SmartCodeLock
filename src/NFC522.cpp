#include <Arduino.h>
#include <SPI.h>
#include "MFRC522.h"
#include "NFC522.h"
#include "Toggle.h"

MFRC522 rfid(5, 25); // Instance of the class
byte myidPICC[][4] = {{131, 7, 97, 21},
                      {115, 103, 163, 13}
                      };
// Init array that will store new NUID 

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
// void printHex(byte *buffer, byte bufferSize) {
//   for (byte i = 0; i < bufferSize; i++) {
//     Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//     Serial.print(buffer[i], HEX);
//   }
// }

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
// void printDec(byte *buffer, byte bufferSize) {
//   for (byte i = 0; i < bufferSize; i++) {
//     Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//     Serial.print(buffer[i], DEC);
//   }
// }

void NFC522::begin () { 
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  // NFC522::users_num = main_system.users_number - 1;
}
 
byte NFC522::getrfid(byte users) {
  users_num = users - 1;

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return 0;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return 0;

  // Serial.print(F("PICC类型: "));
  // MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  // if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
  //   piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
  //   piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
  //   Serial.println(F("您的标签不是MIFARE Classic类型."));
  //   return;
  // }

  // if (rfid.uid.uidByte[0] != nuidPICC[0] || 
  //     rfid.uid.uidByte[1] != nuidPICC[1] || 
  //     rfid.uid.uidByte[2] != nuidPICC[2] || 
  //     rfid.uid.uidByte[3] != nuidPICC[3] ) {
    // Serial.println(F("检测到新卡."));

    // Store NUID into nuidPICC array
    // for (byte i = 0; i < 4; i++) {
    //   nuidPICC[i] = rfid.uid.uidByte[i];
    // }
   
    // Serial.println(F("NUID标签是:"));
    // Serial.print(F("16进制: "));
    //   printHex(rfid.uid.uidByte, rfid.uid.size);
    // Serial.println();
    // Serial.print(F("10进制: "));
    //   printDec(rfid.uid.uidByte, rfid.uid.size);
    // Serial.println();
  // if(main_system.fp_id_users == 1){
    if (rfid.uid.uidByte[0] == myidPICC[users_num][0] && 
        rfid.uid.uidByte[1] == myidPICC[users_num][1] && 
        rfid.uid.uidByte[2] == myidPICC[users_num][2] && 
        rfid.uid.uidByte[3] == myidPICC[users_num][3] ) 
      return 1;
  // }
  // else if(main_system.fp_id_users == 2){
  //   if (rfid.uid.uidByte[0] == myidPICC_user2[0] && 
  //       rfid.uid.uidByte[1] == myidPICC_user2[1] && 
  //       rfid.uid.uidByte[2] == myidPICC_user2[2] && 
  //       rfid.uid.uidByte[3] == myidPICC_user2[3] ) 
  //   return 1;
  // }


  // }
  // else Serial.println(F("卡片最近被检测."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void NFC522::reset(){
  rfid.PCD_AntennaOff();
}
