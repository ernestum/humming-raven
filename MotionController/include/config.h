#pragma once


/*

STEPS PER MM
X  125
Y 125


*/

#define STEPSPERUNIT_X 125 //unit is mm
#define STEPSPERUNIT_Y 125
#define STEPSPERUNIT_Z 150 //unit is degree
#define STEPS_TO_RAISE_PEN 250 //unit is degree

#define LOADCELL_TRESHOLD 100
#define LOADCELL_PEN_PRESSURE 50
#define HOMINGSPEED_Z 1500
#define HOMINGSPEED 1000
#define MOVINGSPEED 800
#define DRAWINGSPEED 800

#define LCD_HEIGHT 4
#define LCD_WIDTH 20

// SD card settings
#define SDPOWER -1
#define SDSS 53
#define SDCARDDETECT 49