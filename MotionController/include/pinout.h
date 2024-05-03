#pragma once
#include <Arduino.h>

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

// #define KILL_PIN 41 //probably there is no kill pin

#define BEEPER 37

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
#define PS_ON_PIN 12

#define HEATER_0_PIN 10
#define HEATER_1_PIN 8
#define TEMP_0_PIN 13 // ANALOG NUMBERING
#define TEMP_1_PIN 14 // ANALOG NUMBERING

#define dataPin 4
#define clockPin 5

inline void setup_pin_modes()
{
    pinMode(BTN_ENC, INPUT_PULLUP);
    pinMode(BTN_EN1, INPUT_PULLUP);
    pinMode(BTN_EN2, INPUT_PULLUP);

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

    pinMode(BEEPER, OUTPUT);
}