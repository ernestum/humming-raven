#pragma once

#include <Arduino.h>
#include <HX711.h>
#include <LiquidCrystal.h>

bool Xlimit = false;
bool Ylimit = false;

bool XDIRvalue = false; // low is to the right
bool YDIRvalue = false; // low is to the back
bool ZDIRvalue = false;

bool PENDOWNstate = false;

int xpos = 0;
int ypos = 0;
int Xpos_as_steps = 0; // absolute position as steps
int Ypos_as_steps = 0;

int z_total_steps = 0;

// int x_steps_per_unit = 2; //deprecated
// int y_steps_per_unit = 20;

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
    // test
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

    if (PENDOWNstate == false)
    {

      ZDIRvalue = true;
      digitalWrite(Z_DIR_PIN, ZDIRvalue); // high is counterclockwise
      digitalWrite(Z_ENABLE_PIN, LOW);

      for (int i = 0; i < STEPS_TO_RAISE_PEN; i++)
      {
        digitalWrite(Z_STEP_PIN, HIGH);
        delayMicroseconds(MOVINGSPEED);
        digitalWrite(Z_STEP_PIN, LOW);
        delayMicroseconds(MOVINGSPEED);
      }
    }

    lcd.setCursor(0, 0);
    lcd.print("PEN UP         ");
    PENDOWNstate = false;

    digitalWrite(Z_ENABLE_PIN, HIGH);
  }
  inline void pen_Up()
  {

    if (PENDOWNstate == true)
    {

      ZDIRvalue = false;
      digitalWrite(Z_DIR_PIN, ZDIRvalue); // high is counterclockwise
      digitalWrite(Z_ENABLE_PIN, LOW);

      for (int i = 0; i < STEPS_TO_RAISE_PEN; i++)
      {
        digitalWrite(Z_STEP_PIN, HIGH);
        delayMicroseconds(MOVINGSPEED);
        digitalWrite(Z_STEP_PIN, LOW);
        delayMicroseconds(MOVINGSPEED);
      }
    }

    lcd.setCursor(0, 0);
    lcd.print("PEN UP         ");
    PENDOWNstate = false;

    digitalWrite(Z_ENABLE_PIN, HIGH);
  }

  inline void home_Y()
  {
    YDIRvalue = false;

    digitalWrite(Y_DIR_PIN, YDIRvalue);
    digitalWrite(Y_ENABLE_PIN, LOW);
    Serial.println("Home Y");
    lcd.setCursor(0, 2);
    lcd.print("Y HOME");
    while (digitalRead(Y_MIN_PIN) == LOW)
    {

      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }
    Ypos_as_steps = YMAX * STEPSPERUNIT_Y;

    YDIRvalue = true;
    digitalWrite(Y_DIR_PIN, YDIRvalue);
    for (int i = 0; i < 2000; i++)
    {
      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(100);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(100);
      Ypos_as_steps = Ypos_as_steps - 1;
    }
    lcd.setCursor(0, 2);
    lcd.print("Y HOMED  ");
    digitalWrite(Y_ENABLE_PIN, HIGH);
  }

  inline void home_Z()
  {

    digitalWrite(Z_DIR_PIN, ZDIRvalue); // high is counterclockwise
    digitalWrite(Z_ENABLE_PIN, LOW);
    Serial.println("Z");
    lcd.setCursor(0, 3);
    lcd.print("Z HOME  LC: ");
    lcd.setCursor(12, 3);
    lcd.print(abs(scale.get_units(1)), 2);

    while (abs(scale.get_units(1)) < LOADCELL_TRESHOLD)
    {
      lcd.setCursor(12, 3);
      lcd.print(abs(scale.get_units(1)), 2);
      // homing z is slow because reading the loqd cell each step takes time
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(Z_STEP_PIN, HIGH);
        delayMicroseconds(HOMINGSPEED_Z);
        digitalWrite(Z_STEP_PIN, LOW);
        delayMicroseconds(HOMINGSPEED_Z);
      }
    }
    auto f = scale.get_units(1);
    Serial.print(abs(f));
    Serial.print(" : ");

    ZDIRvalue = !ZDIRvalue;
    digitalWrite(Z_DIR_PIN, ZDIRvalue);
    while (abs(scale.get_units(1)) > 5)
    {
      lcd.setCursor(12, 3);
      lcd.print(abs(scale.get_units(1)), 2);
      z_total_steps += 30;
      for (int i = 0; i < 30; i++)
      {
        digitalWrite(Z_STEP_PIN, HIGH);
        delayMicroseconds(HOMINGSPEED_Z);
        digitalWrite(Z_STEP_PIN, LOW);
        delayMicroseconds(HOMINGSPEED_Z);
      }
    }

    lcd.setCursor(12, 3);
    lcd.print(abs(scale.get_units(1)), 2);
    lcd.setCursor(0, 3);
    lcd.print("Z HOMED");

    while (abs(scale.get_units(1)) < LOADCELL_PEN_PRESSURE) // detecting distance to PEN DOWN position
    {
      lcd.setCursor(12, 3);
      lcd.print(abs(scale.get_units(1)), 2);
      z_total_steps += 5;
      for (int i = 0; i < 5; i++)
      {
        digitalWrite(Z_STEP_PIN, HIGH);
        delayMicroseconds(HOMINGSPEED_Z);
        digitalWrite(Z_STEP_PIN, LOW);
        delayMicroseconds(HOMINGSPEED_Z);
      }
    }
    lcd.setCursor(0, 0);
    lcd.print("Z steps:");
    lcd.print(z_total_steps);

    delay(3000);
    ZDIRvalue = false;
    /*
    for (int i = 0; i < 70; i++)
    {
      digitalWrite(Z_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(Z_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }*/

    PENDOWNstate = true;
    digitalWrite(Z_ENABLE_PIN, HIGH);
  }

  inline void home_X()
  {

    XDIRvalue = false;
    digitalWrite(X_DIR_PIN, XDIRvalue);
    digitalWrite(X_ENABLE_PIN, LOW);
    Serial.println("Home X");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("home X");
    while (digitalRead(X_MIN_PIN) == LOW)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(HOMINGSPEED);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(HOMINGSPEED);
    }
    XDIRvalue = true;
    Xpos_as_steps = XMAX * STEPSPERUNIT_X;

    digitalWrite(X_DIR_PIN, XDIRvalue);
    for (int i = 0; i < 2000; i++)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(150);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(150);

      Xpos_as_steps = Xpos_as_steps - 1;
    }
    lcd.setCursor(0, 1);
    lcd.print("X HOMED");
    digitalWrite(X_ENABLE_PIN, HIGH);
  }

  inline void move_X(int dist, boolean dir)
  {
    digitalWrite(X_ENABLE_PIN, LOW);
    digitalWrite(X_DIR_PIN, dir);

    for (int i = 0; i < dist; i++)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(MOVINGSPEED);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(MOVINGSPEED);
      if (dir == false)
      {
        Xpos_as_steps += 1;
      }
      if (dir == true)
      {
        Xpos_as_steps -= 1;
      }
    }
    digitalWrite(X_ENABLE_PIN, HIGH);
  }

  inline void move_Y(int dist, boolean dir)
  {
    digitalWrite(Y_ENABLE_PIN, LOW);
    digitalWrite(Y_DIR_PIN, dir);

    for (int i = 0; i < dist; i++)
    {
      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(MOVINGSPEED);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(MOVINGSPEED);
      if (dir == false)
      {
        Ypos_as_steps += 1;
      }
      if (dir == true)
      {
        Ypos_as_steps -= 1;
      }
    }
    digitalWrite(Y_ENABLE_PIN, HIGH); // high is off
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
};