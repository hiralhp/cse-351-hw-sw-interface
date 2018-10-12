// IMPORTANT: ELEGOO_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Elegoo_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library

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

//What does it mean hold pointers to the variables? Where are the variables?
// Declare and assign initial sensor data
unsigned int temperatureRaw = 75;
unsigned int systolicPressRaw = 80;
unsigned int diastolicPressRaw = 80;
unsigned int pulseRateRaw = 50;
unsigned char* tempCorrected = NULL;
unsigned char* systolicPressCorrected = NULL;
unsigned char* diastolicPressCorrected = NULL;
unsigned char* pulseRateCorrected = NULL;
unsigned short batteryState = 200;
unsigned char bpOutOfRange = 0;
unsigned char tempOutOfRange = 0;
unsigned char pulseOutOfRange = 0;
Bool bpHigh = FALSE;
Bool tempHigh = FALSE;
Bool pulseLow = FALSE;

// Declare and assign data structs
measureDataStruct meaDataStruct;
meaDataStruct.temperatureRaw = &temperatureRaw;
meaDataStruct.systolicPressRaw = &systolicPressRaw;
meaDataStruct.diasystolicPressRaw = &diasytsolicPressRaw;
meaDataStruct.pulseRateRaw = &pulrseRateRaw;

computeDataStruct comDataStruct;
comDataStruct.temperatureRaw = &temperatureRaw;
comDataStruct.systolicPressRaw = &systolicPressRaw;
comDataStruct.diasystolicPressRaw = &diasytolicPressRaw;
comDataStruct.pulseRateRaw = &pulseRateRaw;
comDataStruct.tempCorrected = tempCorrected;
comDataStruct.systolicPressCorrected = systolicPressCorrected;
comDataStruct.diastolicPressCorrected = diastolicPressCorrected;
comDataStruct.pulseRateCorrected = pulseRateCorrected;

warningAlarmDataStruct warnAlarmDataStruct; 
warnAlarmDataStruct.tempCorrected = tempCorrected;
warnAlarmDataStruct.systolicPressCorrected = systolicPressCorrected;
warnAlarmDataStruct.diastolicPressCorrected = diastolicPressCorrected;
warnAlarmDataStruct.pulseRateCorrected = pulseRateCorrected;
warnAlarmDataStruct.bpOutOfRange = &bpOutOfRange;
warnAlarmDataStruct.tempOutOfRange = &tempOutOfRange;
warnAlarmDataStruct.pulseOutOfRange = &pulseOutOfRange;
warnAlarmDataStruct.bpHigh = &bpHigh;
warnAlarmDataStruct.tempHigh = &tempHigh;
warnAlarmDataStruct.pulseHigh = &pulseHigh;

displayDataStruct disDataStruct;
disDataStruct.tempCorrected = tempCorrected;
disDataStruct.systolicPressCorrected = systolicPressCorrected;
disDataStruct.diastolicPressCorrected = diastolicPressCorrected;
disDataStruct.pulseRateCorrected = pulseRateCorrected;
disDataStruct.bpOutOfRange = &bpOutOfRange;
disDataStruct.tempOutOfRange = &tempOutOfRange;
disDataStruct.pulseOutOfRange = &pulseOutOfRange;
disDataStruct.bpHigh = &bpHigh;
disDataStruct.tempHigh = &tempHigh;
disDataStruct.pulseHigh = &pulseHigh;

updateStatusStruct updateStatStruct;
updateStatStruct.batteryState = &batteryState;

// Declare and assign task control blocks
TCB measureDataTask;
measureDataTask.taskFuncPtr = &measureData;
measureDataTask.taskDataPtr = &meaDataStruct;

TCB computeDataTask;
computeDataTask.taskFuncPtr = &computeData;
computeDataTask.taskDataPtr = &comDataStruct;

TCB warningAlarmTask;
warningAlarmTask.taskFuncPtr = &warningAlarm;
warningAlarmTask.taskDataPtr = &warnAlarmDataStruct;

TCB displayDataTask;
displayDataTask.taskFuncPtr = &displayData
displayDataTask.taskDataPtr = &disDataStruct;


TCB updateBatteryStatusTask;
updateBatteryStatusTask.taskFuncPtr = &updateBatteryStatus;
updateBatteryStatusTask.taskDataPtr = &updateStatStruct;

// Define array of TCB
TCB* taskQueue[6];
taskQueue[0] = &measureDataTask; 
taskQueue[1] = &computeDataTask;
taskQueue[2] = &warningAlarmTask;
taskQueue[3] = &displayDataTask;
taskQueue[4] = &dateBatteryStatusTask;

