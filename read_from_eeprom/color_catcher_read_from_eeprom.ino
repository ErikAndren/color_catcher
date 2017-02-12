
/*********************************************************************
  Color catcher, read from eeprom
  Copyright Erik Zachrisson
  erik@zachrisson.info
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

#include <ColorRecognitionTCS230.h>

#define LED_PIN 2
#define OUT_PIN 3
#define S2_PIN  4
#define S3_PIN  5
#define S1_PIN  6
#define S0_PIN  7

static ColorRecognitionTCS230 *tcs230;
static uint32_t white_bal[3];
static uint32_t black_bal[3];

#define FLASH_WHITE_BAL_OFFS 0x00
#define FLASH_BLACK_BAL_OFFS 0x0C
 
void setup() {
  Serial.begin(9600);
  tcs230 = ColorRecognitionTCS230::getInstance();

  Serial.println("Restoring white balance from eeprom");
  for (int i = 0; i < 3; i++) {
    white_bal[i] = 0;
    
    for (int j = 0; j < sizeof(uint32_t); j++) {
      white_bal[i] |= EEPROM.read(i * sizeof(uint32_t) + j + FLASH_WHITE_BAL_OFFS) << (8 * j);
    }
    Serial.print("Read white balance ");
    Serial.print(white_bal[i], HEX);
    Serial.println(" from eeprom");
  }
  tcs230->setWhiteBal(white_bal);

  for (int i = 0; i < 3; i++) {
    black_bal[i] = 0;
    
    for (int j = 0; j < sizeof(uint32_t); j++) {
      black_bal[i] |= EEPROM.read(i * sizeof(uint32_t) + j + FLASH_BLACK_BAL_OFFS) << (8 * j);
    }
    Serial.print("Read black balance ");
    Serial.print(black_bal[i], HEX);
    Serial.println(" from eeprom");
  }
  tcs230->setBlackBal(black_bal);

  pinMode(LED_PIN, OUTPUT);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);

  digitalWrite(S0_PIN, HIGH);
  digitalWrite(S1_PIN, HIGH);

  tcs230->initialize(OUT_PIN, S2_PIN, S3_PIN);

  digitalWrite(LED_PIN, HIGH);
 
  tcs230->getWhiteBal(white_bal);
  tcs230->getBlackBal(black_bal);

  Serial.print("White balance freq: ");
  Serial.print(white_bal[0], HEX);
  Serial.print(" ");
  Serial.print(white_bal[1], HEX);
  Serial.print(" ");
  Serial.println(white_bal[2], HEX);

  Serial.print("Black balance freq: ");
  Serial.print(black_bal[0], HEX);
  Serial.print(" ");
  Serial.print(black_bal[1], HEX);
  Serial.print(" ");
  Serial.println(black_bal[2], HEX);
}

void loop() {}
