#include <stdlib.h>
#include "MEGA_TCB_Struct_Functions.h"
  #include "MEGA_Pseudo_Interrupts_Functions.h"
/* create 4 buttons */
char buttonlabels[4][2] = {"Menu", "Annunciation", "Future", "Future"};
uint16_t buttoncolors[15] = {ILI9341_DARKGREEN, ILI9341_DARKGREY, ILI9341_RED, 
                             ILI9341_BLUE};
// Updates battery status
void updateBatteryStatus(void* data) {
  updateStatusStruct *bptr = (updateStatusStruct*) ptr;
   (*(*bptr).batteryState)--;
     if(*(*bptr).batteryState == 0){
     *(*bptr).batteryState = 200;
    }
    
}

// Gets data from UNO peripheral system
void measureData(void* data) {
  // Send byte 0 to indicate request for raw values
  Serial.write(0);

  // Keep looping until receives data from peripheral system
  while (Serial.available() < 6) {
    if (Serial.available() == 6) {
      break;
    }
    continue;
  }

  // Read from Arduino buffer
  Serial.read();                                // Start byte
  unsigned int tempByte = Serial.read();
  unsigned int sysPressByte = Serial.read();
  unsigned int diasPressByte = Serial.read();
  unsigned int prByte = Serial.read();
  Serial.read();                                // End byte
  Serial.flush();                               // Clear buffers

  // Write to raw temperature circular buffer
  data->tempRawCirBuff->(*write)(data->tempRawCirBuff->circularBuffer, data->tempRawCirBuff->start,
                                 data->tempRawCirBuff->end, data->tempRawCirBuff->length,
                                 data->tempRawCirBuff->isEmpty, data->tempRawCirBuff->isFull, tempByte);

  // Write to raw systolic pressure circular buffer
  data->sysPressRawCirBuff->(*write)(data->sysPressRawCirBuff->circularBuffer, data->sysPressRawCirBuff->start,
                                     data->sysPressRawCirBuff->end, data->sysPressRawCirBuff->length,
                                     data->sysPressRawCirBuff->isEmpty, data->sysPressRawCirBuff->isFull, sysPressByte);

  // Write to raw diastolic pressure circular buffer
  data->diasPressRawCirBuff->(*write)(data->diasPressRawCirBuff->circularBuffer, data->diasPressRawCirBuff->start,
                                      data->diasPressRawCirBuff->end, data->diasPressRawCirBuff->length,
                                      data->diasPressRawCirBuff->isEmpty, data->diasPressRawCirBuff->isFull, diasPressByte);

  // Write to raw pulse rate circular buffer
  data->pulseRateRawCirBuff->(*write)(data->pulseRateRawCirBuff->circularBuffer, data->pulseRateRawCirBuff->start,
                                      data->pulseRateRawCirBuff->end, data->pulseRateRawCirBuff->length,
                                      data->pulseRateRawCirBuff->isEmpty, data->pulseRateRawCirBuff->isFull, prByte);
}

// Converts raw values to corrected values
void computeData(void* data) {
  // Get raw values
  unsigned int tempRaw = data->tempRawCirBuff->(*read)();
  unsigned int sysPresRaw = data->sysPressRawCirBuff->(*read)();
  unsigned int diasPresRaw = data->diasPressRawCirBuff->(*read)();
  unsigned int prRaw = data->pulseRateRawCirBuff->(*read)();

  // Convert to corrected values
  unsigned int tempCorr = 5 + 0.75 * tempRaw;
  unsigned int sysPresCorr = 9 + 2 * systolicRaw;
  unsigned int diasPresCorr = 6 + 1.5 * diastolicRaw;
  unsigned int prCorr = 8 * 3 * prRaw;

  // Write to corrected temperature circular buffer
  data->tempCorrCirBuff->(*write)(data->tempCorrCirBuff->circularBuffer, data->tempCorrCirBuff->start,
                                  data->tempCorrCirBuff->end, data->tempCorrCirBuff->length,
                                  data->tempCorrCirBuff->isEmpty, data->tempCorrCirBuff->isFull, tempCorr);

  // Write to corrected systolic pressure circular buffer
  data->sysPressCorrCirBuff->(*write)(data->sysPressCorrCirBuff->circularBuffer, data->sysPressCorrCirBuff->start,
                                      data->sysPressCorrCirBuff->end, data->sysPressCorrCirBuff->length,
                                      data->sysPressCorrCirBuff->isEmpty, data->sysPressCorrCirBuff->isFull, sysPresCorr);

  // Write to corrected diastolic pressure circular buffer
  data->diasPressCorrCirBuff->(*write)(data->diasPressCorrCirBuff->circularBuffer, data->diasPressCorrCirBuff->start,
                                       data->diasPressCorrCirBuff->end, data->diasPressCorrCirBuff->length,
                                       data->diasPressCorrCirBuff->isEmpty, data->diasPressCorrCirBuff->isFull, diasPresCorr);

  // Write to corrected pulse rate circular buffer
  data->pulseRateCorrCirBuff->(*write)(data->pulseRateCorrCirBuff->circularBuffer, data->pulseRateCorrCirBuff->start,
                                       data->pulseRateCorrCirBuff->end, data->pulseRateCorrCirBuff->length,
                                       data->pulseRateCorrCirBuff->isEmpty, data->pulseRateCorrCirBuff->isFull, prCorr);
}

// Checks for warning and alarms
void warningAlarm(void* data) {
  // Get corrected values
  unsigned int temp = data->tempCorrCirBuff->(*read)();
  unsigned int systolicPressRaw = data->sysPressCorrCirBuff->(*read)();
  unsigned int diastolicPressRaw = data->diasPressCorrCirBuff->(*read)();
  unsigned int prRaw = data->pulseRateCorrCirBuff->(*read)();

  // Check battery
  if (batteryState > 200 * 0.2) {
    data->batteryLow = false;
  } else {
    data->batteryLow = true;
  }

  // Check temperature
  if (temp > 36.1 && temp < 37.8) {
    data->tempOutofRange = false;
    data->tempHigh = false;
    data->tempOutOfRangeCounter = 0;
  } else {
    data->tempOutOfRange = true;
    data->tempOutOfRangeCounter += 1;

    // Raise alarm if past five readings were warnings
    if (data->tempOutOfRangeCounter > 5) {
      data->tempHigh = true;
    }

    // Check for alarm acknowlegement
    // Arbitrary functionSelect and measurementSelect values. Need to check with Hiral's code.
    if (data->functionSelect == 0 && data-> measurementSelect == 0 && data->tempHigh && data->alarmAcknowledge) {
      data->tempHigh = false;
      data->tempOutOfRangeCounter = 0;
    }
  }

  // Check blood pressure
  if ((systolicPressRaw > 120 && systolicPressRaw < 130) || (diastolicPressRaw > 70 && diastolicPressRaw < 80)) {
    data->bpOutOfRange = false;
    data->bpHigh = false;
    data->bpOutOutOfRangeCounter = 0;
  } else {
    data->bpOutOfRange = true;
    data->bpOutOutOfRangeCounter += 1;

    // Raise alarm if...
    if (data->systolicPressRaw > 130 * 1.2 || bpOutOutOfRangeCounter > 5) {
      data->bpHigh = true;
    }

    // Check for alarm acknowlegement
    // Arbitrary functionSelect and measurementSelect values. Need to check with Hiral's code.
    if (data->functionSelect == 1 && data-> measurementSelect == 1 && data->bpHigh && data->alarmAcknowledge) {
      data->bpHigh = false;
      data->bpOutOutOfRangeCounter = 0;
    }
  }

  // Check pulse pulse rate
  if (prRaw > 60 && prRaw < 100) {
    data->pulseOutOfRange = false;
    data->pulseLow = false;
    data->pulseOutOfRangeCounter = 0;
  } else {
    data->pulseOutOfRange = true;
    data->pulseOutOfRangeCounter += 1;

    if (pulseOutOfRangeCounter > 5) {
      data->pulseLow = true;
      data->pulseOutOfRangeCounter = 0;
    }

    // Check for alarm acknowlegement
    // Arbitrary functionSelect and measurementSelect values. Need to check with Hiral's code.
    if (data->functionSelect == 2 && data-> measurementSelect == 2 && data->pulseLow && data->alarmAcknowledge) {
      data->pulseLow = false;
      data->pulseOutOfRangeCounter = 0;
    }
  }
}

// Displays data on TFT display screen
void displayData(void* data) {
  int mode = 0;
  char buttonlabels[3][1] = {"Menu", "Annunciation", "Future"};
  uint16_t buttoncolors[3] = {ILI9341_DARKGREEN, ILI9341_DARKGREY, ILI9341_RED};
  for (uint8_t row=0; row<1; row++) {
    for (uint8_t col=0; col<3; col++) {
      buttons[col].initButton(&tft, BUTTON_X+col*(BUTTON_W+BUTTON_SPACING_X), 
                 BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, ILI9341_WHITE, buttoncolors[col], ILI9341_WHITE,
                  buttonlabels[col], BUTTON_TEXTSIZE); 
      buttons[col].drawButton();
    }
  }
  tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, ILI9341_WHITE);
  /*
  user can select to choose between two things:
  menu which lets them see
  bp, temp, pulse 

  annunciation which lets them see
  the warnings of all and battery
  */

  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);


   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
   }
   
  // go thru all the buttons, checking if they were pressed
  for (uint8_t b=0; b<3; b++) {
    if (buttons[b].contains(p.x, p.y)) {
      mode = b;
      //Serial.print("Pressing: "); Serial.println(b);
      buttons[b].press(true);  // tell the button it is pressed
    } else {
      buttons[b].press(false);  // tell the button it is NOT pressed
    }
  }
  char buffer[50];
  displayDataStruct *ptr = (displayDataStruct*)bptr;
  tft.fillScreen(BLACK);                // Turns screen black.
  tft.setCursor(0, 0);                  // Sets the coordinates of the starting point of printing texts on the TFT screen.
  tft.setTextSize(2);                   // Set text size to 2 (height: 20 pixels)
  tft.setTextColor(GREEN); 
  
  if(mode == 0){
    sprintf(buffer, "");
    sprintf(buffer, "S Press = %d", *(*ptr).systolicPressCorrectedBuf);
    tft.print(buffer);

    tft.setCursor(0, 50);
    sprintf(buffer, "");
    sprintf(buffer, "D Pressure = %d", *(*ptr).diastolicCorrectedBuf);
    tft.print(buffer);
    
    tft.setCursor(0, 100);
    sprintf(buffer, "");
    sprintf(buffer, "Temp = %d", *(*ptr).tempCorrectedBuf);
    tft.print(buffer);
    
    tft.setCursor(0, 150);
    sprintf(buffer, "");
    sprintf(buffer, "Pulse = %d", *(*ptr).pulseRateCorrectedBuf);
    tft.print(buffer);

    
  } else if (mode == 1){
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
  }
}


