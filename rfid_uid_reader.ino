#include <SPI.h>
#include <MFRC522.h>

// RC522 pin tanımlamaları
#define SS_PIN 10   // SDA (SS) pini
#define RST_PIN 9   // RST pini

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);     // Seri haberleşme başlat
  SPI.begin();            // SPI başlat
  rfid.PCD_Init();        // RFID modülünü başlat
  delay(1000);            // Başlangıç gecikmesi

  Serial.println("RFID sistemi hazır. Kart okutun.");
}

void loop() {
  // Yeni kart yoksa veya okunamazsa çık
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Kart UID'sini oku
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      uid += "0";  // Tek haneli hex değerler için başa 0 ekle
    }
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toUpperCase();       // UID'yi büyük harfe çevir
  Serial.print("Kart UID: ");
  Serial.println(uid);

  // Kartla iş bittikten sonra durdur
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(1000);  // Aynı kartın üst üste okunmasını engelle
}

