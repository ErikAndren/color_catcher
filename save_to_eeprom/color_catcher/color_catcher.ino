
/*********************************************************************
  Color catcher, read from eeprom
  Copyright Erik Zachrisson
  erik@zachrisson.info
*********************************************************************/

#include <EEPROM.h>
#include <ColorRecognitionTCS230.h>
#include <Adafruit_SSD1306.h>

#define LED_PIN 2
#define OUT_PIN 3
#define S2_PIN  4
#define S3_PIN  5
#define S1_PIN  6
#define S0_PIN  7

#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define FLASH_WHITE_BAL_OFFS 0x00
#define FLASH_BLACK_BAL_OFFS 0x0C

#define DISPLAY_I2C_ADDRESS 0x3C

static ColorRecognitionTCS230 *tcs230;
static uint32_t white_bal[3];
static uint32_t black_bal[3];
 
void setup() {
  tcs230 = ColorRecognitionTCS230::getInstance();

  for (int i = 0; i < 3; i++) {
    white_bal[i] = 0;
    
    for (int j = 0; j < sizeof(uint32_t); j++) {
      white_bal[i] |= EEPROM.read(i * sizeof(uint32_t) + j + FLASH_WHITE_BAL_OFFS) << (8 * j);
    }
  }
  tcs230->setWhiteBal(white_bal);

  for (int i = 0; i < 3; i++) {
    black_bal[i] = 0;
    
    for (int j = 0; j < sizeof(uint32_t); j++) {
      black_bal[i] |= EEPROM.read(i * sizeof(uint32_t) + j + FLASH_BLACK_BAL_OFFS) << (8 * j);
    }
  }
  tcs230->setBlackBal(black_bal);

  pinMode(LED_PIN, OUTPUT);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);

  digitalWrite(S0_PIN, HIGH);
  digitalWrite(S1_PIN, HIGH);

  tcs230->initialize(OUT_PIN, S2_PIN, S3_PIN);

  digitalWrite(LED_PIN, HIGH);

  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS);
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("RED ");
  display.print(tcs230->getRed());
  display.print("\nGREEN ");
  display.print(tcs230->getGreen());
  display.print("\nBLUE ");
  display.print(tcs230->getBlue());
  display.display();
  delay(1000);
}
