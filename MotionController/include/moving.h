#pragma once

#include <Arduino.h>
#include <HX711.h>
#include <LiquidCrystal.h>

bool Xlimit = false;
bool Ylimit = false;
bool ZDIRvalue = false;
bool PENDOWNstate = false;

int xpos = 0;
int ypos = 0;

int x_steps_per_unit = 2;
int y_steps_per_unit = 20;

struct MotorMover
{
  LiquidCrystal &lcd;
  HX711 &scale;


  inline void line(int x1, int y1, int x2, int y2)
  {

    // x_steps_per_unit
    pen_Up();

    lcd.setCursor(0, 2);

    int deltaX = x1 - xpos;
    lcd.setCursor(0, 0);
    lcd.print("deltaX:");
    lcd.print(deltaX);
    // XDIR HIGH IS MOVING TO THE RIGHT

    if (deltaX < 0)
    {
      digitalWrite(X_DIR_PIN, HIGH);
    }
    else
    {
      digitalWrite(X_DIR_PIN, LOW);
    }

    digitalWrite(X_ENABLE_PIN, LOW);

    lcd.setCursor(0, 2);
    lcd.print("MOVE TO X1      ");
    lcd.setCursor(0, 4);
    lcd.print("X:");
    lcd.print(xpos);
    for (long i = 0; i < (10 * abs(deltaX)); i++)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(DRAWINGSPEED);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(DRAWINGSPEED);
    }
    xpos = x1;

    lcd.setCursor(0, 4);
    lcd.print("X:");
    lcd.print(xpos);

    pen_Down();

    deltaX = x2 - xpos;
    lcd.setCursor(0, 0);
    lcd.print("deltaX:");
    lcd.print(deltaX);
    // XDIR HIGH IS MOVING TO THE RIGHT

    if (deltaX < 0)
    {
      digitalWrite(X_DIR_PIN, HIGH);
    }
    else
    {
      digitalWrite(X_DIR_PIN, LOW);
    }

    digitalWrite(X_ENABLE_PIN, LOW);

    lcd.setCursor(0, 2);
    lcd.print("MOVE TO X2      ");
    lcd.setCursor(0, 4);
    lcd.print("X:");
    lcd.print(xpos);
    for (long i = 0; i < (10 * abs(deltaX)); i++)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(DRAWINGSPEED);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(DRAWINGSPEED);
    }
    xpos = x2;

    lcd.setCursor(0, 4);
    lcd.print("X:");
    lcd.print(xpos);

    pen_Up();
    digitalWrite(X_ENABLE_PIN, HIGH);
  }

  inline void home_Z()
  {
    ZDIRvalue=LOW;
    lcd.setCursor(0, 2);
    lcd.print("Z homing");
    digitalWrite(Z_DIR_PIN, ZDIRvalue); // high is counterclockwise
    digitalWrite(Z_ENABLE_PIN, LOW);
    Serial.println("home Z");
    while (abs(scale.get_units(1)) < 400)
    {
      digitalWrite(Z_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(Z_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }
    auto f = scale.get_units(1);
    Serial.print(abs(f));
    Serial.print(" : ");

    ZDIRvalue = !ZDIRvalue;
    digitalWrite(Z_DIR_PIN, ZDIRvalue);

    while (abs(scale.get_units(1)) > 200)
    {
      digitalWrite(Z_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(Z_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }
    lcd.setCursor(0, 2);
    lcd.print("Z HOMED        ");
    digitalWrite(Z_ENABLE_PIN, HIGH);
  }

  inline void move_Z_to_Neutral()
  {
    ZDIRvalue = true;
    int MOTION_DONE = 0;
    digitalWrite(Z_DIR_PIN, ZDIRvalue); // high is counterclockwise
    digitalWrite(Z_ENABLE_PIN, LOW);
    lcd.setCursor(0, 2);
    lcd.print("MOVE TO NEUTRAL");

    auto f = abs(scale.get_units(1));
    lcd.setCursor(0, 1);
    lcd.print("f:");
    lcd.print(f, 1);

    if (abs(scale.get_units(1)) < 50)
    {
      lcd.setCursor(0, 2);
      lcd.print("PEN NEUTRAL");
      digitalWrite(Z_ENABLE_PIN, HIGH);
      MOTION_DONE = 1;
    }

    // move until load is off sensor
    while (abs(scale.get_units(1)) > 100)
    {
      digitalWrite(Z_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(Z_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }

    // move a bit more
    if (MOTION_DONE == 0)
    {
      for (int i = 0; i < 100; i++)
      {
        digitalWrite(Z_STEP_PIN, HIGH);
        delayMicroseconds(HOMINGSPEED);
        digitalWrite(Z_STEP_PIN, LOW);
        delayMicroseconds(HOMINGSPEED);
      }
    }

    lcd.setCursor(0, 2);
    lcd.print("PEN NEUTRAL           ");
    digitalWrite(Z_ENABLE_PIN, HIGH);
  }

  inline void pen_Down()
  {
    ZDIRvalue = true;
    digitalWrite(Z_DIR_PIN, ZDIRvalue); // high is counterclockwise
    digitalWrite(Z_ENABLE_PIN, LOW);

    for (int i = 0; i < 40; i++)
    {
      digitalWrite(Z_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(Z_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }

    while (abs(scale.get_units(1)) < 150)
    {
      digitalWrite(Z_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(Z_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }
    auto f = scale.get_units(1);
    Serial.print(abs(f));
    Serial.print(" : ");

    lcd.setCursor(0, 2);
    lcd.print("PEN DOWN     ");
    PENDOWNstate = true;
    digitalWrite(Z_ENABLE_PIN, HIGH);
  }

  inline void pen_Up()
  {

    if (PENDOWNstate == true)
    {

      ZDIRvalue = false;
      digitalWrite(Z_DIR_PIN, ZDIRvalue); // high is counterclockwise
      digitalWrite(Z_ENABLE_PIN, LOW);

      for (int i = 0; i < 300; i++)
      {
        digitalWrite(Z_STEP_PIN, HIGH);
        delayMicroseconds(MOVINGSPEED);
        digitalWrite(Z_STEP_PIN, LOW);
        delayMicroseconds(MOVINGSPEED);
      }
    }

    lcd.setCursor(0, 2);
    lcd.print("PEN UP         ");
    PENDOWNstate = false;

    digitalWrite(Z_ENABLE_PIN, HIGH);
  }

  inline void home_X()
  {

    digitalWrite(X_DIR_PIN, LOW);
    digitalWrite(X_ENABLE_PIN, LOW);
    Serial.println("Home X");
    lcd.setCursor(0, 2);
    lcd.print("HOME X      ");
    while (digitalRead(X_MIN_PIN) == LOW)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }

    digitalWrite(X_DIR_PIN, HIGH);
    for (int i = 0; i < 350; i++)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(150);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(150);
    }
    lcd.setCursor(0, 2);
    lcd.print("X HOMED       ");
    digitalWrite(X_ENABLE_PIN, HIGH);
    xpos = 500;
  }

  inline void run_X()
  {

    digitalWrite(X_DIR_PIN, HIGH);
    digitalWrite(X_ENABLE_PIN, LOW);
    Serial.println("run X     ");
    lcd.setCursor(0, 3);
    lcd.print("run X");
    for (int i = 0; i < 5000; i++)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(300);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(300);
    }
    digitalWrite(X_ENABLE_PIN, HIGH);
  }

  inline void home_Y()
  {

    digitalWrite(Y_DIR_PIN, LOW);
    digitalWrite(Y_ENABLE_PIN, LOW);
    Serial.println("Home Y");
    lcd.setCursor(0, 3);
    lcd.print("home Y      ");
    while (digitalRead(Y_MIN_PIN) == LOW)
    {

      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }

    digitalWrite(Y_DIR_PIN, HIGH);
    for (int i = 0; i < 15000; i++)
    {
      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(100);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(100);
    }
    lcd.setCursor(0, 3);
    lcd.print("Yhoming done");
    digitalWrite(Y_ENABLE_PIN, HIGH);
  }
};