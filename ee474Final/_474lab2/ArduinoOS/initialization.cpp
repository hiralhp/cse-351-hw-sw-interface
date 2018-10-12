
#define INITIALIZATION_INCLUDE
#include <Arduino.h>
#include "initialization.h"
#include "simulation.h"
#include "boolean.h"
#include "TCBStruct.h"
#include "TCBfunctions.h"
#include "dataStructs.h"
#include "dataTypes.h"

// Declare and assign initial sensor data

unsigned int temperatureRawBuf[8];
unsigned int bloodPressRawBuf[16];
unsigned int pulseRateRawBuf[8];
unsigned char tempCorrectedBuf[8];
unsigned char bloodPressCorrectedBuf[16];
unsigned char pulseRateCorrectedBuf[8]; 
unsigned short batteryState = 200;
Bool bpOutOfRange = 0;
Bool tempOutOfRange = 0;
Bool pulseOutOfRange = 0;
Bool bpHigh = FALSE;
Bool tempHigh = FALSE;
Bool pulseLow = FALSE;
TCB measureDataTask;
TCB computeDataTask;
TCB warningAlarmTask;
TCB displayDataTask;
TCB updateBatteryStatusTask;
TCB* taskQueue[6];
    unsigned short measurementSelection = 0;
    unsigned short alarmAcknowledge = 0;
    unsigned short functionSelect = 0;


void something(){
    // Declare and assign data structs
    /*Serial.print("tempRaw value before measure initialization: ");
    Serial.println(temperatureRaw);*/
    measureDataStruct meaDataStruct;
    meaDataStruct.tempRaw = &temperatureRawBuf;
    /*Serial.print("batteryState value after measure initialization: ");
    Serial.println(temperatureRaw);*/
    meaDataStruct.bpRaw = &bloodPressRawBuf;
    meaDataStruct.pulseRateRaw = &pulseRateRawBuf;
    
    computeDataStruct comDataStruct;
    comDataStruct.tempRaw = &temperatureRawBuf;
    comDataStruct.bpRaw = &bloodPressRawBuf
    comDataStruct.pulseRateRaw = &pulseRateRawBuf;
    comDataStruct.tempCorrected = &tempCorrectedBuf;
    comDataStruct.bpCorrected = &bloodPressCorrectedBuf;
    comDataStruct.pulseRateCorrected = &pulseRateCorrectedBuf;
    
    warningAlarmStruct warnAlarmDataStruct;
    warnAlarmDataStruct.tempCorrected = &tempCorrectedBuf;
    warnAlarmDataStruct.bpCorrected = &bloodPressCorrectedBuf;
    warnAlarmDataStruct.pulseRateCorrected = &pulseRateCorrectedBuf;
    warnAlarmDataStruct.batteryState = &batteryState;
    Serial.print("batteryState value in warning initialization: ");
    Serial.println(batteryState);
    warnAlarmDataStruct.bpOutOfRange = &bpOutOfRange;
    warnAlarmDataStruct.tempOutOfRange = &tempOutOfRange;
    warnAlarmDataStruct.pulseOutOfRange = &pulseOutOfRange;
    warnAlarmDataStruct.bpHigh = &bpHigh;
    warnAlarmDataStruct.tempHigh = &tempHigh;
    warnAlarmDataStruct.pulseLow = &pulseLow;
    
    displayDataStruct disDataStruct;
    Serial.print("batteryState value in before display initialization: ");
    Serial.println(batteryState);
    disDataStruct.tempCorrected = &tempCorrectedBuf;
    disDataStruct.bpCorrected = &bloodPressCorrectedBuf;
    disDataStruct.pulseRateCorrected = &pulseRateCorrectedBuf;
    disDataStruct.batteryState = &batteryState;
    Serial.print("batteryState value in display initialization: ");
    Serial.println(batteryState);
    disDataStruct.bpOutOfRange = &bpOutOfRange;
    disDataStruct.tempOutOfRange = &tempOutOfRange;
    disDataStruct.pulseOutOfRange = &pulseOutOfRange;
    disDataStruct.bpHigh = &bpHigh;
    disDataStruct.tempHigh = &tempHigh;
    disDataStruct.pulseLow = &pulseLow;
    Serial.print("batteryState value before update initialization: ");
    Serial.println(batteryState);
    updateStatusStruct updateStatStruct;
    updateStatStruct.batteryState = &batteryState;
    Serial.print("batteryState value in update initialization: ");
    Serial.println(batteryState);
    // Declare and assign task control blocks

    keypadDataStruct keypadStruct;
    unsigned short* measurementSelection = &measurementSelection;
    unsigned short* alarmAcknowledge = &alarmAcknowledge;
    unsigned short* functionSelect = &functionSelect;

    communicationsDataStruct communicationsStruct;
    unsigned short* measurementSelection = &measurementSelection;
    disDataStruct.tempCorrected = &tempCorrectedBuf;
    disDataStruct.bpCorrected = &bloodPressCorrectedBuf;
    disDataStruct.pulseRateCorrected = &pulseRateCorrectedBuf;

    
    measureDataTask.taskFuncPtr = &measureData;
    measureDataTask.taskDataPtr = &meaDataStruct;
    
    computeDataTask.taskFuncPtr = &computeData;
    computeDataTask.taskDataPtr = &comDataStruct;
    
    warningAlarmTask.taskFuncPtr = &warningAlarm;
    warningAlarmTask.taskDataPtr = &warnAlarmDataStruct;
    
    displayDataTask.taskFuncPtr = &displayData;
    displayDataTask.taskDataPtr = &disDataStruct;
    
    updateBatteryStatusTask.taskFuncPtr = &updateBatteryStatus;
    updateBatteryStatusTask.taskDataPtr = &updateStatStruct;

    measureDataTask.taskFuncPtr = &measureData;
    measureDataTask.taskDataPtr = &meaDataStruct;
    
    
    // Define array of TCB
    
    taskQueue[0] = &measureDataTask;
    taskQueue[1] = &computeDataTask;
    taskQueue[2] = &warningAlarmTask;
    taskQueue[3] = &displayDataTask;
    taskQueue[4] = &updateBatteryStatusTask;
    
}


