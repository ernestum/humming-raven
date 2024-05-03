

// this version works if wiring is correct

#define BEEPER 37
// 37
#define LCD_PINS_RS 16
#define LCD_PINS_ENABLE 17
#define LCD_PINS_D4 23
#define LCD_PINS_D5 25
#define LCD_PINS_D6 27
#define LCD_PINS_D7 29

// Encoder rotation values
#define BTN_EN1 31
#define BTN_EN2 33
#define BTN_ENC 35

// SD card settings
#define SDPOWER -1
#define SDSS 53
#define SDCARDDETECT 49

#define KILL_PIN 41

#define LCD_HEIGHT 4
#define LCD_WIDTH 20

#include <LiquidCrystal.h>

LiquidCrystal lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5, LCD_PINS_D6, LCD_PINS_D7);

int Button = 0;
volatile int Encoder1 = 0;
volatile int Encoder2 = 0;

#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38

#define X_MIN_PIN 3
#define X_MAX_PIN 2

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56
#define Y_MIN_PIN 14
#define Y_MAX_PIN -1 // PIN 15 is used

#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62
#define Z_MIN_PIN 18
#define Z_MAX_PIN -1 // PIN 19 is used

// extruder 1
#define E0_STEP_PIN 26
#define E0_DIR_PIN 28
#define E0_ENABLE_PIN 24

// extruder 2
#define E1_STEP_PIN 36
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30

#define SDPOWER -1

// ChipSelect, Hardware SS Pin on Mega, 10 for Arduino Boards, always kept as output
#define SDCS_PIN 53
#define SD_DETECT_PIN -1 // currently not implemented

#define LED_PIN 13

#define FAN_PIN 9

#define PS_ON_PIN 12 // ATX , awake=LOW, SLEEP=High
#define KILL_PIN -1

#define HEATER_0_PIN 10 // Extruder Heater
#define HEATER_1_PIN 8

#define TEMP_0_PIN 13 // ANALOG NUMBERING
#define TEMP_1_PIN 14 // ANALOG NUMBERING

#define HOMINGSPEED 1

#include "HX711.h"
HX711 scale;

uint8_t dataPin = 4;
uint8_t clockPin = 5;


volatile float f;

int i = 0;

byte Xlimit = 0;
byte Ylimit = 0;

boolean ZDIRvalue = 0;
long debouncer = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);

  pinMode(X_MIN_PIN, INPUT_PULLUP);
  pinMode(Y_MIN_PIN, INPUT_PULLUP);

  scale.begin(dataPin, clockPin);

  // TODO find a nice solution for this calibration..
  // load cell factor 20 KG
  // scale.set_scale(127.15);
  // load cell factor 5 KG
  scale.set_scale(1720); // TODO you need to calibrate this yourself.
  // reset the scale to zero = 0
  scale.tare();

  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  lcd.display();
  lcd.noCursor();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MINIPRINTER SETUP");
  pinMode(BTN_ENC, INPUT_PULLUP);
  pinMode(BTN_EN1, INPUT_PULLUP);
  pinMode(BTN_EN2, INPUT_PULLUP);

  pinMode(BEEPER, OUTPUT);
  digitalWrite(BEEPER, HIGH);
  delay(100);
  digitalWrite(BEEPER, LOW);
  digitalWrite(Y_ENABLE_PIN, HIGH);
  digitalWrite(Z_ENABLE_PIN, HIGH);
  digitalWrite(X_ENABLE_PIN, HIGH);
  home_X();
  run_X();
  home_Y();
  home_Z();
}

void loop()
{

  f = scale.get_units(1);
  Serial.println(f);

  Xlimit = digitalRead(X_MIN_PIN);
  Ylimit = digitalRead(Y_MIN_PIN);

  Serial.print("Xl:");
  Serial.print(Xlimit);
  Serial.print(" - Yl:");
  Serial.print(Ylimit);
  Serial.println();

  /*
    digitalWrite(Z_DIR_PIN, ZDIRvalue);  // high is counterclockwise
    digitalWrite(Z_ENABLE_PIN, LOW);
    Serial.println("Test Z");
    for (i = 0; i < 100; i++) {
      if (abs(f) < 400 || (millis() - debouncer) < 800) {
        digitalWrite(Z_STEP_PIN, HIGH);
        delay(2);
        digitalWrite(Z_STEP_PIN, LOW);
        delay(2);
      }
      f = scale.get_units(1);
      Serial.print(abs(f));
      Serial.print(" : ");
    }
    if (abs(f) > 400 && (millis() - debouncer) > 800) {
      ZDIRvalue = !ZDIRvalue;
      Serial.println("Z-change");
      debouncer = millis();
      digitalWrite(Z_DIR_PIN, ZDIRvalue);
    }
    */

  Serial.println();

  Button = digitalRead(BTN_ENC);
  Encoder1 = digitalRead(BTN_EN1);
  Encoder2 = digitalRead(BTN_EN2);
  lcd.setCursor(0, 1);
  lcd.print("B:");
  lcd.print(Button);
  lcd.print(" - L:");
  lcd.print(f, 2);
  lcd.print("     ");
  // delay(50);

  // put your main code here, to run repeatedly:
}
