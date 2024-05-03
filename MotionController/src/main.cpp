#include <Arduino.h>
#include <HX711.h>
#include <LiquidCrystal.h>

#include <pinout.h>
#include <config.h>
#include <moving.h>

volatile int Encoder1 = 0;
volatile int Encoder2 = 0;

HX711 scale;
LiquidCrystal lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5, LCD_PINS_D6, LCD_PINS_D7);

MotorMover mm{lcd, scale};


void setup()
{
  Serial.begin(9600);
  setup_pin_modes();
  // put your setup code here, to run once:
  

  scale.begin(dataPin, clockPin);

  // TODO find a nice solution for this calibration..
  // load cell factor 20 KG
  // scale.set_scale(127.15);
  // load cell factor 5 KG
  scale.set_scale(1720); // TODO you need to calibrate this yourself.
  // reset the scale to zero = 0
  scale.tare();

  // Play tetris melody with beeper:
  tone(BEEPER, 659, 250);
  delay(500);
  tone(BEEPER, 659, 250);
  delay(500);
  tone(BEEPER, 659, 300);
  delay(600);
  tone(BEEPER, 523, 250);
  delay(500);
  tone(BEEPER, 659, 300);
  delay(600);
  tone(BEEPER, 784, 300);
  delay(600);
  tone(BEEPER, 392, 300);
  delay(600);
  tone(BEEPER, 523, 300);
  delay(600);
  tone(BEEPER, 392, 300);
  delay(600);
  tone(BEEPER, 330, 300);
  delay(600);

  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  lcd.display();
  lcd.noCursor();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MINIPRINTER SETUP");

  pinMode(BEEPER, OUTPUT);
  digitalWrite(BEEPER, HIGH);
  delay(100);
  digitalWrite(BEEPER, LOW);
  digitalWrite(Y_ENABLE_PIN, HIGH);
  digitalWrite(Z_ENABLE_PIN, HIGH);
  digitalWrite(X_ENABLE_PIN, HIGH);
  mm.home_X();
  mm.run_X();
  mm.home_Y();
  mm.home_Z();
}

void loop()
{
  Serial.println("Hello World!");
  delay(1000);
}

/*

#define BEEPER 37

* Pin mapping for the 1280 and 2560
 *
 *   1280     22 23 24 25 26 27 28 29 53 52 51 50 10 11 12 13 37 36 35 34 33 32 31 30 21 20 19 18 81 82 83 38 00 01 78 05 02 03 79 80 54 55 56 57 58 59 60 61 41 40 39 71 70 04 17 16 84 06 07 08 09 85 15 14 72 73 75 76 77 74 62 63 64 65 66 67 68 69 49 48 47 46 45 44 43 42
 *   Port     A0 A1 A2 A3 A4 A5 A6 A7 B0 B1 B2 B3 B4 B5 B6 B7 C0 C1 C2 C3 C4 C5 C6 C7 D0 D1 D2 D3 D4 D5 D6 D7 E0 E1 E2 E3 E4 E5 E6 E7 F0 F1 F2 F3 F4 F5 F6 F7 G0 G1 G2 G3 G4 G5 H0 H1 H2 H3 H4 H5 H6 H7 J0 J1 J2 J3 J4 J5 J6 J7 K0 K1 K2 K3 K4 K5 K6 K7 L0 L1 L2 L3 L4 L5 L6 L7
 *   Marlin   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85


#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define Q_STEP_PIN         36
#define Q_DIR_PIN          34
#define Q_ENABLE_PIN       30

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING
*/
