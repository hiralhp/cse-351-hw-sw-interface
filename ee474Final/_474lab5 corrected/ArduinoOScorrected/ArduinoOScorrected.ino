//#include <stdio.h>
//#include <stdlib.h>
//#include <SoftwareSerial.h>

#include "initialization.h"
#include "simulation.h"
#include "boolean.h"
#include "TCBStruct.h"
#include "TCBfunctions.h"
#include "dataStructs.h"
#include "dataTypes.h"
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include "TimerOne.h"
#include "MEGA_Timer_Interrupt_Functions.h"
#include <Arduino.h>
//

//SoftwareSerial Serial2(10, 11);

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
#define ORANGE      0xFD20

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;

/******************* UI details */
#define BUTTON_X 40
#define BUTTON_Y 100
#define BUTTON_W 60
#define BUTTON_H 30
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_TEXTSIZE 2

// text box where numbers go
#define TEXT_X 10
#define TEXT_Y 10
#define TEXT_W 220
#define TEXT_H 502
#define TEXT_TSIZE 3
#define TEXT_TCOLOR GREEN
// the data (phone #) we store in the textfield
#define TEXT_LEN 12
char textfield[TEXT_LEN+1] = "";
uint8_t textfield_i=0;

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//Touch For New ILI9341 TP
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920
// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65



  #define MINPRESSURE 10
#define MAXPRESSURE 1000
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


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
int tempCounter = 0;
int sbpCounter = 0;
int dbpCounter = 0;
int pulseCounter = 0;
int respCounter = 0;
int ekgCounter = 0;
  Bool measureSend = FALSE;
  Bool warnOn = FALSE;
  Bool backOn = TRUE;
  Bool measureNotSend = FALSE;
Elegoo_GFX_Button buttons2[2];
Elegoo_GFX_Button buttons[4];
//char buttonlabels[1][1] = {"start"};
uint16_t buttoncolors[1] = {CYAN};
Bool communicationFlag = FALSE;
unsigned int commTemp = 0;
unsigned int commSbp = 0;
unsigned int commDbp = 0;
unsigned int commPulse = 0;
unsigned int commResp = 0;
unsigned int commEkg = 0;
Elegoo_GFX_Button buttonChoices[8];
char buttonlabelsChoices[7][1] = {"start"};
uint16_t buttoncolorsChoices[1] = {CYAN};
Bool dataSelections[8] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};  

//booleans for remote communication
Bool remoteStop = FALSE;
Bool remoteDisplay = FALSE;

void setup() {
  Serial.begin(9600);
Serial2.begin(9600);
  pinMode(52, OUTPUT);
  Timer1.initialize();
  Timer1.attachInterrupt(incrementTimerTick);
  Serial.println(("TFT LCD test"));

#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.8\" TFT Breakout Board Pinout"));
#endif

//Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

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
  intializeLinkedListNodes();
  intializeMisc();
    something();
  setupButton();
  

  //simulateTemperatureRawBuf();
  while(!measureSend){
   measureSend = checkBegin();
  }
   choicesSetupButtons();
  while(!dataSelections[4]){
    checkChoices();
  }
  warningButton();
     /* insertTask(NULL, updateBatteryStatusLL, communicationLL); 
     // insertTask(updateBatteryStatusLL, measureDataLL, NULL);
     insertTask(updateBatteryStatusLL, communicationLL, NULL);
      //insertTask(communicationLL, computeDataLL, checkWarningAlarmLL);
      //insertTask(computeDataLL, checkWarningAlarmLL, NULL);
       while(curr != NULL) {
        curr->task->taskPtr(curr->task->dataPtr);
        curr = curr->nextTask;
        deleteTask(curr->previousTask);
      }*/
      Serial.println("initialization complete");
}

void loop() {
  
  dataSelections[0] = FALSE;
  dataSelections[1] = FALSE;
  dataSelections[2] = FALSE;
  dataSelections[3] = FALSE;
  dataSelections[4] = FALSE;
  dataSelections[5] = FALSE;
  dataSelections[6] = FALSE;
  dataSelections[7] = FALSE;
  /*if(Serial.available()){
    if (Serial.read() == 'I'){
      Serial.print("I works");
      insertTask(NULL, updateBatteryStatusLL, lanCommunicationLL);
      insertTask(updateBatteryStatusLL, lanCommunicationLL, NULL);
    }
    while(curr != NULL) {
        curr->task->taskPtr(curr->task->dataPtr);
        curr = curr->nextTask;
        deleteTask(curr->previousTask);
    }
    curr = head;
  }*/
  if(Serial.available()){
    if (Serial.read() == 'I'){
      Serial.print("your system is initialized");
      insertTask(NULL, updateBatteryStatusLL, remoteCommLL);
      insertTask(updateBatteryStatusLL, remoteCommLL, NULL);
    }
    while(curr != NULL) {
        curr->task->taskPtr(curr->task->dataPtr);
        curr = curr->nextTask;
        deleteTask(curr->previousTask);
    }
    curr = head;
  if (timerTick % 5 == 0) {
  insertTask(NULL, updateBatteryStatusLL, measureDataLL);
  insertTask(updateBatteryStatusLL, measureDataLL, NULL);
      while(curr != NULL) {
        curr->task->taskPtr(curr->task->dataPtr);
        curr = curr->nextTask;
        deleteTask(curr->previousTask);
      }
 
  
  curr = head;
  }
}

void measureData(void* ptr) {
  Serial.print("meas entered");
  choicesSetupButtons();
  while(!dataSelections[4]){
    checkChoices();
  }
  if(communicationFlag){
    insertTask(measureDataLL, communicationLL, NULL);
      communicationFlag = FALSE;
  } else {
   insertTask(measureDataLL, displayDataLL, NULL);
  }
  Serial.print("measure end");
}

void checkStart(void* ptr){
  /*Serial.print("comm entered");
  Serial2.write(1);
   while(!Serial2.available()){
    
  }*/
  /*
  byte readBuffer[6];

  Serial2.readBytesUntil(244, readBuffer, 5);
  commTemp = readBuffer[0];
  commPulse = readBuffer[1]*2;
  commSbp = readBuffer[2];
  commDbp = readBuffer[2];
  commResp = readBuffer[1];
  commEkg = readBuffer[3]; //change to correct readBuffer[]
    Serial.print("commTemp: ");
  Serial.println(commSbp);
  Serial.print("commPulse: ");
  Serial.println(commDbp);
  Serial.print("commsbp: ");
  Serial.println(commSbp);
  Serial.print("commdbp: ");
  Serial.println(commDbp);
    Serial.print("commResp: ");
  Serial.println(commResp);*/
    Serial2.write(9);
    for(int i = 0; i < 4; i++){
      if(dataSelections[i] == TRUE){
        Serial2.write(i);
      } 
    }
    if(dataSelections[6] == TRUE){
      Serial.write(6);
    } 
    if(dataSelections[7] == TRUE){
      Serial.write(7);
    } 
    Serial2.write(8);

    while(!Serial2.available()){
    
    }
    int startBit = Serial2.read(); //startbit

    Serial.print("start bit: ");
    Serial.println(startBit);
    while(Serial2.available()) { //if size == 1 then it is end bit
    
    //byte computeSelect = Serial2.read();

    if(dataSelections[0] == TRUE){
      commTemp = Serial2.read();
      dataSelections[0] = FALSE;
    }
    else if(dataSelections[1] == TRUE){
      commPulse = Serial2.read();
      dataSelections[1] = FALSE;
    }
    else if(dataSelections[2] == TRUE){
      commSbp = Serial2.read();
      dataSelections[2] = FALSE;
    }
    else if(dataSelections[3] == TRUE){
      commDbp = Serial2.read();
      dataSelections[3] = FALSE;
    } else {
               //end bit
         int endBit = Serial2.read();

    }
    }

       
    insertTask(communicationLL, computeDataLL, NULL);

    Serial.print("comm finished");
}

void computeData(void* bptr) {
  Serial.print("compute entered");
    computeDataStruct *ptr = (computeDataStruct*)bptr;
  if(dataSelections[0]){


      (*ptr).tempCorrected[tempCounter] = commTemp*3/4 + 5;
      tempCounter++;
      if(tempCounter == 9){
      tempCounter = 1;
      }
  }

      if(dataSelections[1]){
      (*ptr).pulseRateCorrected[pulseCounter] = commPulse*3 + 8;
      pulseCounter++;
      if(pulseCounter == 9){
      pulseCounter = 1;
      }
      }

      if(dataSelections[2]){
      (*ptr).sbpCorrected[sbpCounter] = commSbp*2 + 9;
      sbpCounter++;
      if(sbpCounter == 9){
      sbpCounter = 1;
      }
      }

if(dataSelections[3]){
      (*ptr).dbpCorrected[dbpCounter] = commDbp*1.5 + 6;
      dbpCounter++;
      if(dbpCounter == 9){
      dbpCounter = 1;
      }
}

if(dataSelections[6]){
  (*ptr).rrCorrected[respCounter] = commResp*3 + 7;
      respCounter++;
      if(respCounter == 9){
      respCounter = 1;
    }
}
if(dataSelections[7]){


      (*ptr).ekgCorrected[ekgCounter] = commEkg*10000/256;
      ekgCounter++;
      if(ekgCounter == 17){
      ekgCounter = 1;
      }
  }

   insertTask(computeDataLL, checkWarningAlarmLL, displayDataLL);
   insertTask(checkWarningAlarmLL, displayDataLL, NULL);
  
   Serial.print("compute finished");
}

 
  

void warningAlarm(void* bptr) {
  /*
1. Temperature: 36.1 C to 37.8 C
2. Systolic pressure: 120 mm Hg
3. Diastolic pressure: 80 mm Hg.
4. Pulse rate: 60 to 100 beats per minute
5. Battery: Greater than 20% charge remaining 
*/
Serial.print("warn entered");
warningAlarmStruct *ptr = (warningAlarmStruct*)bptr;
  if((*ptr).tempCorrected[tempCounter - 1] < 36.1 || (*ptr).tempCorrected[tempCounter - 1] > 37.8){
      *(*ptr).tempOutOfRange = TRUE;
  } else {
    *(*ptr).tempOutOfRange = FALSE;
  }

  if((*ptr).sbpCorrected[sbpCounter-1] != 120 || (*ptr).dbpCorrected[dbpCounter-1] != 80){
      *(*ptr).bpOutOfRange = TRUE;
  } else {
    *(*ptr).bpOutOfRange = FALSE;
  }

  if((*ptr).pulseRateCorrected[pulseCounter-1] < 60 || (*ptr).pulseRateCorrected[pulseCounter-1] > 100){
      *(*ptr).pulseOutOfRange = TRUE;
  } else {
    *(*ptr).pulseOutOfRange = FALSE;
  }

    if((*ptr).rrCorrected[pulseCounter-1] < 12 || (*ptr).rrCorrected[pulseCounter-1] > 25){
      *(*ptr).rrOutOfRange = TRUE;
  } else {
    *(*ptr).rrOutOfRange = FALSE;
  }

     if((*ptr).ekgCorrected[ekgCounter-1] > 35 || (*ptr).ekgCorrected[ekgCounter-1] < 3.75){
      *(*ptr).ekgOutOfRange = TRUE;
  } else {
    *(*ptr).ekgOutOfRange = FALSE;
  }
Serial.print("warn finished");
}

void choicesSetupButtons(){

    buttonChoices[0].initButton(&tft, 40+0*(60+20), //0 is col
                 100+0*(30+20),   //row is 0 // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[0], WHITE,
                  "temp", BUTTON_TEXTSIZE); 
    buttonChoices[0].drawButton();

    buttonChoices[1].initButton(&tft, 40+1*(60+20), //0 is col
                 100+0*(30+20),   //row is 0 // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[0], WHITE,
                  "pulse", BUTTON_TEXTSIZE); 
    buttonChoices[1].drawButton();

    buttonChoices[2].initButton(&tft, 40+0*(60+20), //0 is col
                 100+1*(30+20),   //row is 0 // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[0], WHITE,
                  "sbp", BUTTON_TEXTSIZE); 
    buttonChoices[2].drawButton();

    buttonChoices[3].initButton(&tft, 40+1*(60+20), //0 is col
                 100+1*(30+20),   //row is 0 // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[0], WHITE,
                  "dbp", BUTTON_TEXTSIZE); 
    buttonChoices[3].drawButton();
    
    buttonChoices[4].initButton(&tft, 40+0*(60+20), //0 is col
                 100+3*(30+20),   //row is 0 // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[0], WHITE,
                  "done", BUTTON_TEXTSIZE); 
    buttonChoices[4].drawButton();

    buttonChoices[5].initButton(&tft, 40+1*(60+20), //0 is col
                 100+3*(30+20),   //row is 0 // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[0], WHITE,
                  "new", BUTTON_TEXTSIZE); 
    buttonChoices[5].drawButton();
    buttonChoices[6].initButton(&tft, 40+1*(60+20), //0 is col
                 100+2*(30+20),   //row is 0 // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[0], WHITE,
                  "resp", BUTTON_TEXTSIZE); 
    buttonChoices[6].drawButton();
    buttonChoices[7].initButton(&tft, 40+0*(60+20), //0 is col
                 100+2*(30+20),   //row is 0 // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[0], WHITE,
                  "ekg", BUTTON_TEXTSIZE); 
    buttonChoices[7].drawButton();
}

Bool checkChoices(){

   digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);
  
  // Scale from ~0->4000 to tft.width using the calibration #'s

   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
//    Serial.print(p.x);
//    Serial.print(", ");
//    Serial.println(p.y);
   } else {
    return FALSE;
   }
   
  // go thru all the buttons, checking if they were pressed
  for (uint8_t b=0; b<8; b++) {


    if (buttonChoices[b].contains(p.x, p.y)) {
      //Serial.print("Pressing: "); Serial.println(b);
      buttonChoices[b].press(true);  // tell the button it is pressed
      if(dataSelections[b]){
        dataSelections[b] = FALSE;
      } else {
        dataSelections[b] = TRUE; 
        if(b == 5){
          communicationFlag = TRUE;
        }
      }
      
      
    } else {
      buttonChoices[b].press(false);  // tell the button it is NOT pressed
      
      
    }
    //Serial.println(dataSelections[b]);
  }
  for (uint8_t b=0; b<8; b++) {
    Serial.print(dataSelections[b]);
    Serial.print(", ");
  }
  Serial.println();
  for (uint8_t b=0; b<8; b++) {
    if (buttonChoices[b].justReleased()) {
      // //Serial..print("Released: "); Serial..println(b);
      buttonChoices[b].drawButton();  // draw normal
    }
    
    if (buttonChoices[b].justPressed()) {
        buttonChoices[b].drawButton(true);  // draw invert!
    
      delay(100); // UI debouncing
      buttonChoices[b].drawButton(false);
    }
  }
return FALSE;
}

void warningButton(){
   // create buttons
   

//240, 320
      
      buttons2[0].initButton(&tft, 175, //x col
                 275,   //y row // x, y, w, h, outline, fill, text
                  BUTTON_W+40, BUTTON_H, WHITE, RED, WHITE,
                  "clear", BUTTON_TEXTSIZE); 
                  
      buttons2[1].initButton(&tft, 50, //x col
                 275,   //y row  // x, y, w, h, outline, fill, text
                  BUTTON_W+20, BUTTON_H, WHITE, GREEN, WHITE,
                  "back", BUTTON_TEXTSIZE);             
      
    
}

void displayData(void* bptr) {
  delay(10000);
  char buffer[50];
  Serial.print("disp entered");
  displayDataStruct *ptr = (displayDataStruct*)bptr;
 
  warnOn = FALSE;
  backOn = FALSE;
//  tft.reset();
tft.setCursor(0, 300); 
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
  //Serial.print("Battery value in display: ");
  // Serial.println(*(*ptr).batteryState);
   sprintf(buffer, "");
    sprintf(buffer, "Battery Adr in initialization = %d", ((*ptr).batteryState));
    Serial.println(buffer);

  

  
  tft.fillScreen(BLACK);                // Turns screen black.
  tft.setCursor(0, 0);                  // Sets the coordinates of the starting point of printing texts on the TFT screen.
  tft.setTextSize(2);                   // Set text size to 2 (height: 20 pixels)

/*int tempCounter = 0;
int sbpCounter = 0;
int dbpCounter = 0;
int pulseCounter = 0;*/
if(dataSelections[0]){
    tft.setCursor(0, 0); 
    backOn = TRUE; 
   if(*(*ptr).tempOutOfRange){
    warnOn = TRUE;
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
    buttons2[0].drawButton();
    buttons2[1].drawButton();
    Serial.print("tempCorrected:" );
    Serial.println((*ptr).tempCorrected[tempCounter - 1]);
    Serial.print("index: " );
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
} 

if(dataSelections[1]){
  backOn = TRUE; 
  tft.setCursor(0, 50); 
   if(*(*ptr).pulseOutOfRange){
    tft.setTextColor(RED);              // Set text color to red.
    warnOn = TRUE;
    sprintf(buffer, "");
    sprintf(buffer, "WARN Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter -1]);
    tft.print(buffer);
    buttons2[0].drawButton();
    buttons2[1].drawButton();
    
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[2]){
  backOn = TRUE; 
  tft.setCursor(0, 100);
    if(*(*ptr).bpOutOfRange){
    tft.setTextColor(RED);              // Set text color to red.
    warnOn = TRUE;
    sprintf(buffer, "");
    sprintf(buffer, "WARN S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
    tft.print(buffer);
    buttons2[0].drawButton();
    buttons2[1].drawButton();
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[3]){
  backOn = TRUE; 
  tft.setCursor(0, 150);
  if(*(*ptr).bpOutOfRange){
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
    tft.print(buffer);
    warnOn = TRUE;
    buttons2[0].drawButton();
    buttons2[1].drawButton();
  } else {
    tft.setTextColor(GREEN); 
    sprintf(buffer, "");
    sprintf(buffer, "D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[6]){
  backOn = TRUE; 
  tft.setCursor(0, 200);
  if((*ptr).rrCorrected[respCounter-1] < 10 || (*ptr).rrCorrected[respCounter-1] > 29 ){
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN Resp = %d", (*ptr).rrCorrected[respCounter-1]);
    tft.print(buffer);
    warnOn = TRUE;
    buttons2[0].drawButton();
    buttons2[1].drawButton();
  } else {
    tft.setTextColor(GREEN); 
    sprintf(buffer, "");
    sprintf(buffer, "Resp = %d", (*ptr).rrCorrected[respCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[7]){
  backOn = TRUE; 
  tft.setCursor(0, 250);
  if((*ptr).ekgCorrected[ekgCounter-1] > 35 || (*ptr).ekgCorrected[ekgCounter-1] < 3.75 ){
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN EKG = %d", (*ptr).ekgCorrected[ekgCounter-1]);
    tft.print(buffer);
    warnOn = TRUE;
    buttons2[0].drawButton();
    buttons2[1].drawButton();
  } else {
    tft.setTextColor(GREEN); 
    sprintf(buffer, "");
    sprintf(buffer, "EKG = %d", (*ptr).ekgCorrected[ekgCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

tft.setCursor(0, 300); 
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


while(!clearOrBack()){
    int second = millis();
    if(second%2000 == 0){
      if(dataSelections[1]){
        tft.setCursor(0, 50);
        if((*ptr).pulseRateCorrected[pulseCounter - 1] < 57 && (*ptr).pulseRateCorrected[pulseCounter - 1] > 51|| (*ptr).pulseRateCorrected[pulseCounter - 1] > 105 && (*ptr).pulseRateCorrected[pulseCounter - 1] < 115){
          tft.setTextColor(BLACK);
          sprintf(buffer, "");
    sprintf(buffer, "WARN Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter -1]);
          tft.print(buffer);
        }
      }
      if(dataSelections[0]){
    tft.setCursor(0, 0); 

if((*ptr).tempCorrected[tempCounter - 1] < 34 && (*ptr).tempCorrected[tempCounter - 1] > 30 || (*ptr).tempCorrected[tempCounter - 1] > 40 && (*ptr).tempCorrected[tempCounter - 1] < 44){

    tft.setTextColor(BLACK);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
   }
      }
      if(dataSelections[2]){
        tft.setCursor(0, 100);
        if(*(*ptr).bpOutOfRange && ((*ptr).sbpCorrected[sbpCounter-1] > 96 && ((*ptr).sbpCorrected[sbpCounter-1] < 156))){
          tft.setTextColor(BLACK);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
          tft.print(buffer);
        }
      }

      if(dataSelections[3]){
        tft.setCursor(0, 150);
        if(*(*ptr).bpOutOfRange){
          tft.setTextColor(BLACK);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
          tft.print(buffer);
        }
      }
      delay(100);
      if(dataSelections[1]){
        tft.setCursor(0, 50);
        if((*ptr).pulseRateCorrected[pulseCounter - 1] < 57 && (*ptr).pulseRateCorrected[pulseCounter - 1] > 51|| (*ptr).pulseRateCorrected[pulseCounter - 1] > 105 && (*ptr).pulseRateCorrected[pulseCounter - 1] < 115){
          tft.setTextColor(BLUE);
          sprintf(buffer, "");
        sprintf(buffer, "WARN Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter -1]);
        tft.print(buffer);
        }
      }
            if(dataSelections[0]){
    tft.setCursor(0, 0); 

if((*ptr).tempCorrected[tempCounter - 1] < 34 && (*ptr).tempCorrected[tempCounter - 1] > 30 || (*ptr).tempCorrected[tempCounter - 1] > 40 && (*ptr).tempCorrected[tempCounter - 1] < 44){

    tft.setTextColor(BLUE);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
    }
            }
            if(dataSelections[2]){
        tft.setCursor(0, 100);
        if(*(*ptr).bpOutOfRange && ((*ptr).sbpCorrected[sbpCounter-1] > 96 && ((*ptr).sbpCorrected[sbpCounter-1] < 156))){
          tft.setTextColor(BLUE);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
          tft.print(buffer);
        }
      }

      if(dataSelections[3]){
        tft.setCursor(0, 150);
        if(*(*ptr).bpOutOfRange){
          tft.setTextColor(BLUE);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
          tft.print(buffer);
        }
      }
    }
   else if(second%1000 == 0){
            if(dataSelections[0]){
    tft.setCursor(0, 0); 
    backOn = TRUE; 
 if((*ptr).tempCorrected[tempCounter - 1] < 34 && (*ptr).tempCorrected[tempCounter - 1] > 30 || (*ptr).tempCorrected[tempCounter - 1] > 40 && (*ptr).tempCorrected[tempCounter - 1] < 44){

    tft.setTextColor(BLACK);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
   }
      }
      if(dataSelections[2]){
        tft.setCursor(0, 100);
        if(*(*ptr).bpOutOfRange && ((*ptr).sbpCorrected[sbpCounter-1] > 96 && ((*ptr).sbpCorrected[sbpCounter-1] < 156))){
          tft.setTextColor(BLACK);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
          tft.print(buffer);
        }
      }

      if(dataSelections[3]){
        tft.setCursor(0, 150);
        if(*(*ptr).bpOutOfRange){
          tft.setTextColor(BLACK);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
          tft.print(buffer);
        }
      }
      delay(100);
               if(dataSelections[0]){
    tft.setCursor(0, 0); 
    backOn = TRUE; 
 if((*ptr).tempCorrected[tempCounter - 1] < 34 && (*ptr).tempCorrected[tempCounter - 1] > 30 || (*ptr).tempCorrected[tempCounter - 1] > 40 && (*ptr).tempCorrected[tempCounter - 1] < 44){
    warnOn = TRUE;
    tft.setTextColor(BLUE);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
    }
            }
            if(dataSelections[2]){
        tft.setCursor(0, 100);
        if(*(*ptr).bpOutOfRange && ((*ptr).sbpCorrected[sbpCounter-1] > 96 && ((*ptr).sbpCorrected[sbpCounter-1] < 156))){
          tft.setTextColor(BLUE);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
          tft.print(buffer);
        }
      }

      if(dataSelections[3]){
        tft.setCursor(0, 150);
        if(*(*ptr).bpOutOfRange){
          tft.setTextColor(BLUE);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
          tft.print(buffer);
        }
      }
    } else if (second %500 == 0){
              if(dataSelections[2]){
        tft.setCursor(0, 100);
        if(*(*ptr).bpOutOfRange && ((*ptr).sbpCorrected[sbpCounter-1] > 96 && ((*ptr).sbpCorrected[sbpCounter-1] < 156))){
          tft.setTextColor(BLACK);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
          tft.print(buffer);
        }
      }

      if(dataSelections[3]){
        tft.setCursor(0, 150);
        if(*(*ptr).bpOutOfRange){
          tft.setTextColor(BLACK);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
          tft.print(buffer);
        }
      }
      delay(100);
               if(dataSelections[2]){
        tft.setCursor(0, 100);
        if(*(*ptr).bpOutOfRange && ((*ptr).sbpCorrected[sbpCounter-1] > 96 && ((*ptr).sbpCorrected[sbpCounter-1] < 156))){
          tft.setTextColor(BLUE);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
          tft.print(buffer);
        }
      }

      if(dataSelections[3]){
        tft.setCursor(0, 150);
        if(*(*ptr).bpOutOfRange){
          tft.setTextColor(BLUE);              // Set text color to red.
          sprintf(buffer, "");
          sprintf(buffer, "WARN D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
          tft.print(buffer);
        }
      }
    

}

if(!warnOn){
  changeColor(ptr);
  while(backOn){
    clearOrBack();
    
}

}
  tft.fillScreen(BLACK); 
   measureNotSend = TRUE;
  }
Serial.print("disp finished");
  
}

void changeColor(displayDataStruct* ptr){
 char buffer[50];
  tft.fillScreen(BLACK); 
    tft.setCursor(0, 0); 
    backOn = TRUE; 

if(dataSelections[0]){
    tft.setCursor(0, 0); 
    backOn = TRUE; 
   if(*(*ptr).tempOutOfRange){
    warnOn = TRUE;
    tft.setTextColor(MAGENTA);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
    buttons2[1].drawButton();
    Serial.print("tempCorrected:" );
    Serial.println((*ptr).tempCorrected[tempCounter - 1]);
    Serial.print("index: " );
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
  
} 

if(dataSelections[1]){
  backOn = TRUE; 
  tft.setCursor(0, 50); 
   if(*(*ptr).pulseOutOfRange){
    tft.setTextColor(MAGENTA);              // Set text color to red.
    warnOn = TRUE;
    sprintf(buffer, "");
    sprintf(buffer, "Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter -1]);
    tft.print(buffer);

    buttons2[1].drawButton();
    
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[2]){
  backOn = TRUE; 
  tft.setCursor(0, 100);
    if(*(*ptr).bpOutOfRange){
    tft.setTextColor(MAGENTA);              // Set text color to red.
    warnOn = TRUE;
    sprintf(buffer, "");
    sprintf(buffer, "S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
    tft.print(buffer);

    buttons2[1].drawButton();
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[3]){
  backOn = TRUE; 
  tft.setCursor(0, 150);
  if(*(*ptr).bpOutOfRange){
    tft.setTextColor(MAGENTA);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
    tft.print(buffer);
    warnOn = TRUE;
    buttons2[1].drawButton();
  } else {
    sprintf(buffer, "");
    sprintf(buffer, "D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[6]){
  backOn = TRUE; 
  tft.setCursor(0, 200);
  if(*(*ptr).rrOutOfRange){
    tft.setTextColor(MAGENTA);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "Resp = %d", (*ptr).rrCorrected[respCounter-1]);
    tft.print(buffer);
    warnOn = TRUE;
    buttons2[1].drawButton();
  } else {
    sprintf(buffer, "");
    sprintf(buffer, "Resp = %d", (*ptr).rrCorrected[respCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[7]){
  backOn = TRUE; 
  tft.setCursor(0, 250);
  if(*(*ptr).ekgOutOfRange){
    tft.setTextColor(MAGENTA);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "EKG = %d", (*ptr).ekgCorrected[ekgCounter-1]);
    tft.print(buffer);
    warnOn = TRUE;
    buttons2[1].drawButton();
  } else {
    sprintf(buffer, "");
    sprintf(buffer, "EKG = %d", (*ptr).ekgCorrected[ekgCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

tft.setCursor(0, 300); 
  if(*(*ptr).batteryState < 20){
    tft.setTextColor(MAGENTA);              // Set text color to red.
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
Bool clearOrBack(){
  Bool start = FALSE;
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);
  
   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
//    //Serial.print(p.x);
//    //Serial.print(", ");
//    //Serial.println(p.y);
   } else {
    return FALSE;
   }
   
  // go thru all the buttons, checking if they were pressed
  for (uint8_t b=0; b<2; b++) {
    //Serial.print(p.x);
    //Serial.print(", ");
    //Serial.println(p.y);
    if (buttons2[b].contains(p.x, p.y)) {
      //Serial.print("Pressing: "); Serial.println(b);
      buttons2[b].press(true);  // tell the button it is pressed
      //Serial.println("button pushed");
      start = TRUE;
      if(b == 0){
        warnOn = FALSE;
      } else {
        backOn = FALSE;
      }
    } else {
      buttons2[b].press(false);  // tell the button it is NOT pressed
      
      
    }
  }

  // now we can ask the buttons if their state has changed
  for (uint8_t b=0; b<2; b++) {
    if (buttons2[b].justReleased()) {
      // //Serial..print("Released: "); Serial..println(b);
      buttons2[b].drawButton();  // draw normal
    }
    
    if (buttons2[b].justPressed()) {
        buttons2[b].drawButton(true);  // draw invert!
    
      delay(100); // UI debouncing
      buttons2[b].drawButton(false);
    }
  }
  return start;
}

void updateBatteryStatus(void* ptr) {
//  char buffer[50];
  updateStatusStruct *bptr = (updateStatusStruct*) ptr;
  (*(*bptr).batteryState)--;
//Serial.print("Battery value in update state: ");
  //Serial.println(*(*bptr).batteryState);
//   sprintf(buffer, "");
//    sprintf(buffer, "Battery Adr in update state = %d", ((*bptr).batteryState));
//    Serial.println(buffer);
     if(*(*bptr).batteryState == 0){
     *(*bptr).batteryState = 200;
    }
   //Serial.println("%d", *(*bptr).batteryState);
}

void setupButton(){

  /*char buttonlabels[4][2] = {"temp", "systolic", "diastolic", "pulse"};
  uint16_t buttoncolors[4] = {CYAN, MAGENTA, YELLOW, BLUE};*/

  // create buttons
  for (uint8_t row=0; row<1; row++) {
//240, 320
/*
(Elegoo_GFX *gfx,
            int16_t x, int16_t y, 
            uint8_t w, uint8_t h, 
            uint16_t outline, uint16_t fill, 
            uint16_t textcolor,
            char *label, uint8_t textsize)

            #define BUTTON_X 40
#define BUTTON_Y 100
#define BUTTON_W 60
#define BUTTON_H 30
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_TEXTSIZE 2
*/
      buttons[0].initButton(&tft, 40+0*(60+20), //0 is col
                 100+0*(30+20),   //row is 0 // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[0], WHITE,
                  "start", BUTTON_TEXTSIZE); 
      buttons[0].drawButton();
    }
  
  
  // create 'text field'
 // tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, WHITE);
}

Bool checkBegin(){

Bool start = FALSE;
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);
  
   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
//    Serial.print(p.x);
//    Serial.print(", ");
//    Serial.println(p.y);
   } else {
    return FALSE;
   }
   
  // go thru all the buttons, checking if they were pressed
  for (uint8_t b=0; b<1; b++) {
    Serial.print(p.x);
    Serial.print(", ");
    Serial.println(p.y);
    if (buttons[b].contains(p.x, p.y)) {
      //Serial.print("Pressing: "); Serial.println(b);
      buttons[b].press(true);  // tell the button it is pressed
      Serial.println("button pushed");
      start = TRUE;
    } else {
      buttons[b].press(false);  // tell the button it is NOT pressed
      
      
    }
  }

  // now we can ask the buttons if their state has changed
  for (uint8_t b=0; b<1; b++) {
    if (buttons[b].justReleased()) {
      // //Serial..print("Released: "); Serial..println(b);
      buttons[b].drawButton();  // draw normal
    }
    
    if (buttons[b].justPressed()) {
        buttons[b].drawButton(true);  // draw invert!
    
      delay(100); // UI debouncing
      buttons[b].drawButton(false);
    }
  }
  return start;
}



void keypad(void* bptr){
  keypadDataStruct *ptr = (keypadDataStruct*)bptr;
  // Keep on looping until something is received
  while(Serial.read() != 'S') {
    continue;
  }
  Serial.println("S works");
   notDone:
  if(Serial.available()){
    char input = Serial.read();
    if(input == 'P'){
      remoteStop = TRUE;
      Serial.println("P works");
    } else if(input == 'W'){
      remoteWarnFunc(ptr);
    } else if(input == 'M'){
      remoteDisplayAll(ptr);
    } else if(input == 'S'){
      Serial.print("comm entered");
      Serial2.write(1);
      while(!Serial2.available()){
    
      }
       byte readBuffer[5];

        Serial2.readBytesUntil(244, readBuffer, 5);
        commTemp = readBuffer[0];
        commPulse = readBuffer[1]*2;
        commSbp = readBuffer[2];
        commDbp = readBuffer[2];
        commResp = readBuffer[1];
        commEkg = readBuffer[3]; //change to correct readBuffer[]
    }else if(input == 'D'){
      Serial.println("D works");
      if(remoteDisplay){ //means display is on turn off - opposite
           remoteDisplay = FALSE;
           tft.fillScreen(BLACK); 
      }  else {
        remoteDisplay = TRUE;
        remoteDisplayFunc(ptr);    
      }
    } else {
      Serial.print("E - error invalid key");
    }
  }
  if(!remoteStop){
    goto notDone;
  }
  while(Serial.available()){
    Serial.read();
  }
  remoteStop = FALSE;
}
void remoteDisplayFunc(keypadDataStruct* ptr){
  char buffer[50];
  if(dataSelections[0]){
    tft.setCursor(0, 0); 
    backOn = TRUE; 
   if(*(*ptr).tempOutOfRange){
    warnOn = TRUE;
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
    buttons2[0].drawButton();
    buttons2[1].drawButton();
    Serial.print("tempCorrected:" );
    Serial.println((*ptr).tempCorrected[tempCounter - 1]);
    Serial.print("index: " );
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
} 

if(dataSelections[1]){
  backOn = TRUE; 
  tft.setCursor(0, 50); 
   if(*(*ptr).pulseOutOfRange){
    tft.setTextColor(RED);              // Set text color to red.
    warnOn = TRUE;
    sprintf(buffer, "");
    sprintf(buffer, "WARN Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter -1]);
    tft.print(buffer);
    buttons2[0].drawButton();
    buttons2[1].drawButton();
    
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[2]){
  backOn = TRUE; 
  tft.setCursor(0, 100);
    if(*(*ptr).bpOutOfRange){
    tft.setTextColor(RED);              // Set text color to red.
    warnOn = TRUE;
    sprintf(buffer, "");
    sprintf(buffer, "WARN S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
    tft.print(buffer);
    buttons2[0].drawButton();
    buttons2[1].drawButton();
  } else {
    tft.setTextColor(GREEN);              // Set text color to green.
    sprintf(buffer, "");
    sprintf(buffer, "S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[3]){
  backOn = TRUE; 
  tft.setCursor(0, 150);
  if(*(*ptr).bpOutOfRange){
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
    tft.print(buffer);
    warnOn = TRUE;
    buttons2[0].drawButton();
    buttons2[1].drawButton();
  } else {
    tft.setTextColor(GREEN); 
    sprintf(buffer, "");
    sprintf(buffer, "D Pressure = %d", (*ptr).dbpCorrected[dbpCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[6]){
  backOn = TRUE; 
  tft.setCursor(0, 200);
  if((*ptr).rrCorrected[respCounter-1] < 10 || (*ptr).rrCorrected[respCounter-1] > 29 ){
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN Resp = %d", (*ptr).rrCorrected[respCounter-1]);
    tft.print(buffer);
    warnOn = TRUE;
    buttons2[0].drawButton();
    buttons2[1].drawButton();
  } else {
    tft.setTextColor(GREEN); 
    sprintf(buffer, "");
    sprintf(buffer, "Resp = %d", (*ptr).rrCorrected[respCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

if(dataSelections[7]){
  backOn = TRUE; 
  tft.setCursor(0, 250);
  if((*ptr).ekgCorrected[ekgCounter-1] > 35 || (*ptr).ekgCorrected[ekgCounter-1] < 3.75 ){
    tft.setTextColor(RED);              // Set text color to red.
    sprintf(buffer, "");
    sprintf(buffer, "WARN EKG = %d", (*ptr).ekgCorrected[ekgCounter-1]);
    tft.print(buffer);
    warnOn = TRUE;
    buttons2[0].drawButton();
    buttons2[1].drawButton();
  } else {
    tft.setTextColor(GREEN); 
    sprintf(buffer, "");
    sprintf(buffer, "EKG = %d", (*ptr).ekgCorrected[ekgCounter-1]);
    tft.print(buffer);
    buttons2[1].drawButton();
  }
}

tft.setCursor(0, 300); 
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
void remoteWarnFunc(keypadDataStruct* ptr){
  char buffer[50];
  Serial.print("entered remoteWarn");
    if(*(*ptr).tempOutOfRange){
      sprintf(buffer, "");
      sprintf(buffer, "Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
      Serial.println(buffer);
    } 

    if(*(*ptr).bpOutOfRange){
      sprintf(buffer, "");
      sprintf(buffer, "S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
      Serial.println(buffer);

      sprintf(buffer, "");
      sprintf(buffer, "D Press = %d", (*ptr).dbpCorrected[sbpCounter-1]);
      Serial.println(buffer);
    }
    
    if(*(*ptr).pulseOutOfRange){
      sprintf(buffer, "");
      sprintf(buffer, "Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter -1]);
      Serial.println(buffer);
    }
    
    if((*ptr).rrCorrected[respCounter-1] < 10 || (*ptr).rrCorrected[respCounter-1] > 29 ){
      sprintf(buffer, "");
      sprintf(buffer, "WARN Resp = %d", (*ptr).rrCorrected[respCounter-1]);
      Serial.println(buffer);
    }

    if((*ptr).ekgCorrected[ekgCounter-1] > 35 || (*ptr).ekgCorrected[ekgCounter-1] < 3.75 ){
      sprintf(buffer, "");
      sprintf(buffer, "EKG = %d", (*ptr).ekgCorrected[ekgCounter-1]);
      Serial.println(buffer);

    }
}

void remoteDisplayAll(keypadDataStruct* ptr){
  char buffer[50];
      sprintf(buffer, "");
      sprintf(buffer, "Temp = %d", (*ptr).tempCorrected[tempCounter - 1]);
      Serial.println(buffer);

      sprintf(buffer, "");
      sprintf(buffer, "S Press = %d", (*ptr).sbpCorrected[sbpCounter-1]);
      Serial.println(buffer);

      sprintf(buffer, "");
      sprintf(buffer, "D Press = %d", (*ptr).dbpCorrected[sbpCounter-1]);
      Serial.println(buffer);
    
      sprintf(buffer, "");
      sprintf(buffer, "Pulse = %d", (*ptr).pulseRateCorrected[pulseCounter -1]);
      Serial.println(buffer);
    
      sprintf(buffer, "");
      sprintf(buffer, "WARN Resp = %d", (*ptr).rrCorrected[respCounter-1]);
      Serial.println(buffer);

      sprintf(buffer, "");
      sprintf(buffer, "EKG = %d", (*ptr).ekgCorrected[ekgCounter-1]);
      Serial.println(buffer);
}

