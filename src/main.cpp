#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define X_AXIS 35
#define Y_AXIS 34

#define RESET_PIN 27
#define SS 5

MFRC522 rfid(SS, RESET_PIN);

void setup() {
  Serial.begin(115200);

  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
}

void loop() {
  int axisXValue = map(analogRead(X_AXIS), 0, 4095, 0, 100);
  int axisYValue = map(analogRead(Y_AXIS), 0, 4095, 0, 100);

  if(axisXValue < 35){
    Serial.println("left_move");
  }
  else if(axisXValue > 60){
    Serial.println("right_move");
  }

  if(axisYValue < 35) {
    Serial.println("down_move");
  }else if(axisYValue > 60){
    Serial.println("up_move");
  }

  delay(100);

  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // print UID in Serial Monitor in the hex format
      Serial.print("UID:");
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      Serial.println();

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
}