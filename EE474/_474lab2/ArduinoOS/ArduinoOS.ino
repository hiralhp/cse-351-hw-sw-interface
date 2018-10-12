//#include <stdio.h>
//#include <stdlib.h>

#include "initialization.h"
#include "simulation.h"
#include "boolean.h"
#include "TCBStruct.h"
#include "TCBfunctions.h"
#include "dataStructs.h"
#include "dataTypes.h"
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library

//



// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;


/*STRUCTS*/

//unsigned int temperatureRawBuf = 75;
Bool tempIncrement = FALSE;
//unsigned int systolicPressRawBuf = 80;
Bool sysComplete = FALSE;
//unsigned int diastolicPressRawBuf = 80;
Bool diastolicComplete = TRUE;
//unsigned int pulseRateRawBuf = 50
Bool pulseIncrement = FALSE;
unsigned int count = 0;


void setup() {
  Serial.begin(9600);

  pinMode(52, OUTPUT);
  
  Serial.println(F("TFT LCD test"));

#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
identifier=0x9341;
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);
  something();
  //simulateTemperatureRawBuf();
}

void loop() {
  
  Serial.print("loop");
  
  digitalWrite(52, HIGH);
  taskQueue[0]->taskFuncPtr(taskQueue[0]->taskDataPtr);
  digitalWrite(52, LOW);
  delay(100);
  
  digitalWrite(52, HIGH);
  taskQueue[1]->taskFuncPtr(taskQueue[1]->taskDataPtr);
  digitalWrite(52, LOW);
  delay(100);
  
  digitalWrite(52, HIGH);
  taskQueue[2]->taskFuncPtr(taskQueue[2]->taskDataPtr);
  digitalWrite(52, LOW);
  delay(100);
  
  digitalWrite(52, HIGH);
  taskQueue[3]->taskFuncPtr(taskQueue[3]->taskDataPtr);
  digitalWrite(52, LOW);
  delay(100); // Change
  
  digitalWrite(52, HIGH);
  taskQueue[4]->taskFuncPtr(taskQueue[4]->taskDataPtr);
  digitalWrite(52, LOW);
  delay(100);
  
  // measureDataTask.taskFuncPtr(measureDataTask.taskDataPtr);
  //delay(1000); // Change
  // computeDataTask.taskFuncPtr(computeDataTask.taskDataPtr);
  //delay(1000); // Change
  // warningAlarmTask.taskFuncPtr(warningAlarmTask.taskDataPtr);
  //delay(1000); // Change
  // displayDataTask.taskFuncPtr(displayDataTask.taskDataPtr);
  
  // updateBatteryStatusTask.taskFuncPtr(updateBatteryStatusTask.taskDataPtr);
  count++;

}

void measureData(void* ptr) {
  tempRawBufMeasure();
  sysPressRawBufMeasure();
  diasPressRawBufMeasure();
  pulseRawBufMeasure();
}

void tempRawBufMeasure() {
  if (tempIncrement) { // If you are incremementing
    if (temperatureRawBuf <= 50) { // If you are still counting up and below 50, enter statement
      if (count % 2 == 0) {
        temperatureRawBuf += 2;
        /*Serial.print("temp incrementing add 2: ");
        Serial.println(temperatureRawBuf);*/
      } else {
        temperatureRawBuf -= 1;
        /*Serial.print("temp incrementing subtract 1: ");
        Serial.println(temperatureRawBuf);*/
      }
    }

    if (temperatureRawBuf > 50) { // change to decrementing if temperatureRawBuf is above 50
      tempIncrement = FALSE;
    }
  } else { // If you are decrementing
    if (temperatureRawBuf >= 15) { // If above 15, enter statement
      if (count % 2 == 0) {
        temperatureRawBuf -= 2;
        /*Serial.print("temp decrementing sub 2: ");
        Serial.println(temperatureRawBuf);*/
      } else {
        temperatureRawBuf += 1;
        /*Serial.print("temp decrementing add 1: ");
        Serial.println(temperatureRawBuf);*/
      }
    }

    if (temperatureRawBuf < 15) { // change to incrementing if temperatureRawBuf is below 15
        tempIncrement = TRUE;
    }
  }
}

void sysPressRawBufMeasure() {

  if (!sysComplete && diastolicComplete) {
    if (count % 2 == 0) {
      systolicPressRawBuf += 3;
    } else {
      systolicPressRawBuf -= 1;
    }
    if (systolicPressRawBuf > 100) {
      sysComplete = TRUE;
      systolicPressRawBuf = 80;
      diastolicComplete = FALSE;
    }
  }
}

void diasPressRawBufMeasure() {
  if (!diastolicComplete && sysComplete) {
    if (count % 2 == 0) {
      diastolicPressRawBuf -= 2;
    } else {
      diastolicPressRawBuf += 1;
    }
    if (diastolicPressRawBuf < 40) {
      diastolicComplete = TRUE;
      diastolicPressRawBuf = 80;
      sysComplete = FALSE;
    }
  }
}

void pulseRawBufMeasure() {
  if (pulseIncrement) {
    if (count % 2 == 0) {
      pulseRateRawBuf -= 1;
    } else {
      pulseRateRawBuf += 3;
    }
    if (pulseRateRawBuf > 40) {
      pulseIncrement = FALSE;
    }

  } else {
    if (count % 2 == 0) {
      pulseRateRawBuf += 1;
    } else {
      pulseRateRawBuf -= 3;
    }
    if (pulseRateRawBuf < 15) {
      pulseIncrement = FALSE;
    }
  }
}

void computeData(void* bptr) {
//  char buffer[50];
/*
1. Temperature in Celsius: tempCorrectedBuf = 5 + 0.75tempRawBuf
2. Systolic Pressure in mm Hg: sysCorrectedBuf = 9 + 2systolicRawBuf
3. Diastolic Pressure in mm Hg: diasCorrectedBuf = 6 + 1.5diastolicRawBuf
4. Pulse Rate in beats per minute: prCorrectedBuf = 8 + 3bpRawBuf 
*/
computeDataStruct *ptr = (computeDataStruct*)bptr;
*(*ptr).tempCorrectedBuf = *((*ptr).tempRawBuf)*3/4 + 5;

*(*ptr).systolicPressCorrectedBuf = *((*ptr).systolicPressRawBuf)*2 + 9;
*(*ptr).diastolicCorrectedBuf = *((*ptr).diastolicPressRawBuf)*1.5 + 6;
*(*ptr).pulseRateCorrectedBuf = *((*ptr).pulseRateRawBuf)*3 + 8;
  Serial.print("diastolicPressRawBuf in compute");
  Serial.println(*(*ptr).diastolicPressRawBuf);
//  sprintf(buffer, "");
//sprintf(buffer, "Temp after comute method = %d", *(*ptr).tempCorrectedBuf);
//Serial.println(buffer);
//sprintf(buffer, "");
//sprintf(buffer, "S Press after comute method = %d", *(*ptr).systolicPressCorrectedBuf);
//Serial.println(buffer);

//sprintf(buffer, "");
//sprintf(buffer, "Pulse after comute method = %d", *(*ptr).pulseRateCorrectedBuf);
//Serial.println(buffer);

}

void warningAlarm(void* bptr) {
  /*
1. Temperature: 36.1 C to 37.8 C
2. Systolic pressure: 120 mm Hg
3. Diastolic pressure: 80 mm Hg.
4. Pulse rate: 60 to 100 beats per minute
5. Battery: Greater than 20% charge remaining 
*/
warningAlarmStruct *ptr = (warningAlarmStruct*)bptr;
  if((*ptr).tempCorrectedBuf[0] < 36.1 || (*ptr).tempCorrectedBuf[0] > 37.8){
      *(*ptr).tempOutOfRange = TRUE;
  } else {
    *(*ptr).tempOutOfRange = FALSE;
  }

  if(*(*ptr).systolicPressCorrectedBuf != 120 || *(*ptr).diastolicCorrectedBuf != 80){
      *(*ptr).bpOutOfRange = TRUE;
  } else {
    *(*ptr).bpOutOfRange = FALSE;
  }

  if(*(*ptr).pulseRateCorrectedBuf < 60 || *(*ptr).pulseRateCorrectedBuf > 100){
      *(*ptr).pulseOutOfRange = TRUE;
  } else {
    *(*ptr).pulseOutOfRange = FALSE;
  }

  if(*(*ptr).pulseRateCorrectedBuf < 60 || *(*ptr).pulseRateCorrectedBuf > 100){
      *(*ptr).pulseOutOfRange = TRUE;
  } else {
    *(*ptr).pulseOutOfRange = FALSE;
  }
}

void displayData(void* bptr) {
  char buffer[50];
  displayDataStruct *ptr = (displayDataStruct*)bptr;
  tft.fillScreen(BLACK);                // Turns screen black.
  tft.setCursor(0, 0);                  // Sets the coordinates of the starting point of printing texts on the TFT screen.
  tft.setTextSize(2);                   // Set text size to 2 (height: 20 pixels)

  if(*(*ptr).bpOutOfRange){
    tft.setTextColor(RED);              // Set text color to red.

    sprintf(buffer, "");
    sprintf(buffer, "WARN S Press = %d", *(*ptr).systolicPressCorrectedBuf);
    tft.print(buffer);

    tft.setCursor(0, 50);
    sprintf(buffer, "");
    sprintf(buffer, "D Pressure = %d", *(*ptr).diastolicCorrectedBuf);
    tft.print(buffer);
    
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "S Press = %d", *(*ptr).systolicPressCorrectedBuf);
    tft.print(buffer);

    tft.setCursor(0, 50);
    sprintf(buffer, "");
    sprintf(buffer, "D Pressure = %d", *(*ptr).diastolicCorrectedBuf);
    tft.print(buffer);
  }
  
  tft.setCursor(0, 100);                  // Sets the coordinates of the starting point of printing texts on the TFT screen.
  
  if(*(*ptr).tempOutOfRange){
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN Temp = %d", *(*ptr).tempCorrectedBuf);
    tft.print(buffer);

  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "Temp = %d", *(*ptr).tempCorrectedBuf);
    tft.print(buffer);
  }

tft.setCursor(0, 150); 
   if(*(*ptr).pulseOutOfRange){
    tft.setTextColor(RED);              // Set text color to red.
    
    sprintf(buffer, "");
    sprintf(buffer, "WARN Pulse = %d", *(*ptr).pulseRateCorrectedBuf);
    tft.print(buffer);
    
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "Pulse = %d", *(*ptr).pulseRateCorrectedBuf);
    tft.print(buffer);
  }

tft.setCursor(0, 200); 
  if(*(*ptr).batteryState < 20){
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "Battery = %d", *(*ptr).batteryState);
    tft.print(buffer);
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "Battery = %d", (*(*ptr).batteryState));
    tft.print(buffer);
  }
  Serial.print("Battery value in display: ");
   Serial.println(*(*ptr).batteryState);
   sprintf(buffer, "");
    sprintf(buffer, "Battery Adr in initialization = %d", ((*ptr).batteryState));
    Serial.println(buffer);
  // Serial.println("%d", &(*ptr).batteryState);
}

void updateBatteryStatus(void* ptr) {
//  char buffer[50];
  updateStatusStruct *bptr = (updateStatusStruct*) ptr;
  (*(*bptr).batteryState)--;
//  Serial.print("Battery value in update state: ");
//   Serial.println(*(*bptr).batteryState);
//   sprintf(buffer, "");
//    sprintf(buffer, "Battery Adr in update state = %d", ((*bptr).batteryState));
//    Serial.println(buffer);
     if(*(*bptr).batteryState == 0){
     *(*bptr).batteryState = 200;
    }
  // Serial.println("%d", &(*ptr).batteryState);
}
