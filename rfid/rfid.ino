#include <SPI.h>
#include <deprecated.h>
#include <MFRC522.h>
#include <require_cpp11.h>

// RFID READER
#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN A5

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Game State Variables
boolean gameState = 0;

void setup() {
  Serial.begin(115200);
  // Init RFID
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 225);
  delay(200);
  analogWrite(LED_PIN, 0);
}

void loop() {
   // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "73 C2 63 11") //change here the UID of the card/cards that you want to give access
  {
    analogWrite(LED_PIN, 225);
    delay(1000);
    analogWrite(LED_PIN, 0);
    gameState = 1;
    Serial.write(gameState);
    Serial.flush();
  }
}
