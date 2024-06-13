#pragma once

#include <Arduino.h>

bool Xlimit = false;
bool Ylimit = false;

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