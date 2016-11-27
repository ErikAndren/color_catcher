
/*********************************************************************
  Color catcher
  Copyright Erik Zachrisson
  erik@zachrisson.info
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

#include <ColorRecognitionTCS230.h>

#define OLED_RESET 4

#define DISPLAY_I2C_ADDRESS 0x3C

#define LED_PIN 2
#define OUT_PIN 3
#define S2_PIN  4
#define S3_PIN  5
#define S1_PIN  6
#define S0_PIN  7

static ColorRecognitionTCS230 *tcs230;
static uint32_t white_bal[3];
static uint32_t black_bal[3];
 
void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);

  digitalWrite(S0_PIN, HIGH);
  digitalWrite(S1_PIN, HIGH);

  tcs230 = ColorRecognitionTCS230::getInstance();
  tcs230->initialize(OUT_PIN, S2_PIN, S3_PIN);


  digitalWrite(LED_PIN, HIGH);
  //tcs230->adjustWhiteBalance();
  
  //digitalWrite(LED_PIN, LOW);

  //  display.setCursor(0, 0);
  //  display.clearDisplay();
  //  display.println("Calibration done. Saving to eeprom");

//  tcs230->getWhiteBalanceFrequencies(white_bal);

//  Serial.print("Green white balance:");
//  Serial.println(white_bal[1]);
//
//  Serial.print("Blue white balance:");
//  Serial.println(white_bal[2]);

  Serial.println("Calibrating for 10 s, show white and black");
  for(int i = 0; i < 10; i++) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Calibration phase done");
 
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

#define FLASH_WHITE_BAL_OFFS 0x00
#define FLASH_BLACK_BAL_OFFS 0x0C

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < sizeof(uint32_t); j++) {
      Serial.print("Writing ");
      Serial.print(white_bal[i] >> (8 * j) & 0xFF, HEX);
      Serial.print(" to ");
      Serial.println(i * sizeof(uint32_t) + j);
      EEPROM.write(i * sizeof(uint32_t) + j + FLASH_WHITE_BAL_OFFS, white_bal[i] >> (8 * j) & 0xFF);
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < sizeof(uint32_t); j++) {
      Serial.print("Writing ");
      Serial.print(black_bal[i] >> (8 * j) & 0xFF, HEX);
      Serial.print(" to ");
      Serial.println(i * sizeof(uint32_t) + j);
      EEPROM.write(i * sizeof(uint32_t) + j + FLASH_BLACK_BAL_OFFS, black_bal[i] >> (8 * j) & 0xFF);
    }
  }
  Serial.println("Calibration done");
}

void loop() {}
