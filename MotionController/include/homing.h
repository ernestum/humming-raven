#pragma once

#include <Arduino.h>

bool Xlimit = false;
bool Ylimit = false;

inline void home_Y() {

  digitalWrite(Y_DIR_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  Serial.println("Home Y");
  lcd.setCursor(0, 3);
  lcd.print("home Y      ");
  while (digitalRead(Y_MIN_PIN) == LOW) {

    digitalWrite(Y_STEP_PIN, HIGH);
    delay(HOMINGSPEED);
    digitalWrite(Y_STEP_PIN, LOW);
    delay(HOMINGSPEED);
  }


  digitalWrite(Y_DIR_PIN, HIGH);
  for (int i = 0; i < 15000; i++) {
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(100);
  }
  lcd.setCursor(0, 3);
  lcd.print("Yhoming done");
  digitalWrite(Y_ENABLE_PIN, HIGH);
}

inline void home_Z() {

  digitalWrite(Z_DIR_PIN, ZDIRvalue);  // high is counterclockwise
  digitalWrite(Z_ENABLE_PIN, LOW);
  Serial.println("home Z");
  while (abs(scale.get_units(1)) < 400) {
    digitalWrite(Z_STEP_PIN, HIGH);
    delay(HOMINGSPEED);
    digitalWrite(Z_STEP_PIN, LOW);
    delay(HOMINGSPEED);
  }
  auto f = scale.get_units(1);
  Serial.print(abs(f));
  Serial.print(" : ");

  ZDIRvalue = !ZDIRvalue;
  digitalWrite(Z_DIR_PIN, ZDIRvalue);
  for (int i = 0; i < 10; i++) {
    digitalWrite(Z_STEP_PIN, HIGH);
    delay(HOMINGSPEED);
    digitalWrite(Z_STEP_PIN, LOW);
    delay(HOMINGSPEED);
  }
  lcd.setCursor(0, 3);
  lcd.print("Z homing done");
  digitalWrite(Z_ENABLE_PIN, HIGH);
}

inline void home_X() {

  digitalWrite(X_DIR_PIN, LOW);
  digitalWrite(X_ENABLE_PIN, LOW);
  Serial.println("Home X");
  lcd.setCursor(0, 3);
  lcd.print("home X");
  while (digitalRead(X_MIN_PIN) == LOW) {
    digitalWrite(X_STEP_PIN, HIGH);
    delay(HOMINGSPEED);
    digitalWrite(X_STEP_PIN, LOW);
    delay(HOMINGSPEED);
  }


  digitalWrite(X_DIR_PIN, HIGH);
  for (int i = 0; i < 3500; i++) {
    digitalWrite(X_STEP_PIN, HIGH);
   delayMicroseconds(150);
    digitalWrite(X_STEP_PIN, LOW);
   delayMicroseconds(150);
  }
  lcd.setCursor(0, 3);
  lcd.print("X homing done");
  digitalWrite(X_ENABLE_PIN, HIGH);
}

inline void run_X() {

  digitalWrite(X_DIR_PIN, HIGH);
  digitalWrite(X_ENABLE_PIN, LOW);
  Serial.println("run X");
  lcd.setCursor(0, 3);
  lcd.print("run X");
  for (int i = 0; i < 5000; i++) {
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(300);
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(300);
  }
   digitalWrite(X_ENABLE_PIN, HIGH);
}