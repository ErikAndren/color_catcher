/*********************************************************************
Color catcher
Copyright Erik Zachrisson
erik@zachrisson.info
*********************************************************************/

#include <SPI.h>
#include <Wire.h>

#include <TimerOne.h>
#include <ColorRecognition.h>
#include <ColorRecognitionTCS230.h>

#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define DISPLAY_I2C_ADDRESS 0x3C

#define LED_PIN 2
#define OUT_PIN 3
#define S2_PIN  4
#define S3_PIN  5
#define S1_PIN  6
#define S0_PIN  7

ColorRecognitionTCS230 *tcs230;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);

  digitalWrite(S0_PIN, LOW);
  digitalWrite(S1_PIN, HIGH);
  
  tcs230 = ColorRecognitionTCS230::getInstance();
  tcs230->initialize(OUT_PIN, S2_PIN, S3_PIN);

  digitalWrite(LED_PIN, HIGH);
  tcs230->adjustWhiteBalance();
  //digitalWrite(LED_PIN, LOW);
  
  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("RED ");
  display.println(tcs230->getRed());

  display.print("GRN ");
  display.println(tcs230->getGreen());

  display.print("BLU ");
  display.println(tcs230->getBlue());
  
  display.display();
  delay(1000);
}
