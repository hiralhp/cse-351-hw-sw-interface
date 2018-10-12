
#define INITIALIZATION_INCLUDE
#include <Arduino.h>
#include "initialization.h"
#include "simulation.h"
#include "boolean.h"
#include "TCBStruct.h"
#include "TCBfunctions.h"
#include "dataStructs.h"
#include "dataTypes.h"
  #include "MEGA_Linked_List_Node_Struct.h"
  #include "MEGA_Linked_List_Node_Functions.h"

// Declare and assign initial sensor data
 LinkedListNode* updateBatteryStatusLL;
 LinkedListNode* measureDataLL;
 LinkedListNode* computeDataLL;
 LinkedListNode* checkWarningAlarmLL;
 LinkedListNode* displayDataLL;
 LinkedListNode* communicationLL;
 LinkedListNode* remoteCommLL;
 LinkedListNode* head;
 LinkedListNode* curr;

 bool insertComputeTask;
 bool deleteComputeTask;
unsigned int temperatureRawBuf[8];

unsigned int sbpRawBuf[8];
unsigned int dbpRawBuf[8];
unsigned int pulseRateRawBuf[8];
unsigned int rrRawBuf[8];
unsigned int ekgRawBuf[16];
unsigned char tempCorrectedBuf[8];
unsigned char sbpCorrectedBuf[8];
unsigned char dbpCorrectedBuf[8];
unsigned char pulseRateCorrectedBuf[8]; 
unsigned char rrCorrectedBuf[8]; 
unsigned char ekgCorrectedBuf[16];
unsigned short batteryState = 200;
Bool bpOutOfRange = 0;
Bool tempOutOfRange = 0;
Bool pulseOutOfRange = 0;
Bool bpHigh = FALSE;
Bool tempHigh = FALSE;
Bool pulseLow = FALSE;
Bool rrRange = FALSE;
Bool ekgRange = FALSE;
TCB measureDataTask;
TCB computeDataTask;
TCB warningAlarmTask;
TCB displayDataTask;
TCB updateBatteryStatusTask;
TCB keypadTask;
TCB communicationTask;
TCB* taskQueue[6];
    unsigned short measurementSelection = 0;
    unsigned short alarmAcknowledge = 0;
    unsigned short functionSelect = 0;


void something(){
    // Declare and assign data structs
temperatureRawBuf[0] = 75;
sbpRawBuf[0] = 80;
dbpRawBuf[0] = 80;
pulseRateRawBuf[0] = 0;
    measureDataStruct meaDataStruct;
    meaDataStruct.tempRaw = temperatureRawBuf;
    meaDataStruct.sbpRaw = sbpRawBuf;
    meaDataStruct.dbpRaw = dbpRawBuf;
    meaDataStruct.pulseRateRaw = pulseRateRawBuf;
    meaDataStruct.rrRaw = rrRawBuf;
    meaDataStruct.ekgRaw = ekgRawBuf;

    
    computeDataStruct comDataStruct;
    comDataStruct.tempRaw = temperatureRawBuf;
    comDataStruct.sbpRaw = sbpRawBuf;
    comDataStruct.dbpRaw = dbpRawBuf;
    comDataStruct.pulseRateRaw = pulseRateRawBuf;
    comDataStruct.rrRaw = rrRawBuf;
    comDataStruct.ekgRaw = ekgRawBuf;
    comDataStruct.tempCorrected = tempCorrectedBuf;
    comDataStruct.sbpCorrected = sbpCorrectedBuf;
    comDataStruct.dbpCorrected = dbpCorrectedBuf;
    comDataStruct.pulseRateCorrected = pulseRateCorrectedBuf;
    comDataStruct.rrCorrected = rrCorrectedBuf;
    comDataStruct.ekgCorrected = ekgCorrectedBuf;
    
    warningAlarmStruct warnAlarmDataStruct;
    warnAlarmDataStruct.tempCorrected = tempCorrectedBuf;
    warnAlarmDataStruct.sbpCorrected = sbpCorrectedBuf;
    warnAlarmDataStruct.dbpCorrected = dbpCorrectedBuf;
    warnAlarmDataStruct.pulseRateCorrected = pulseRateCorrectedBuf;
    warnAlarmDataStruct.rrCorrected = rrCorrectedBuf;
        warnAlarmDataStruct.ekgCorrected = ekgCorrectedBuf;

    warnAlarmDataStruct.batteryState = &batteryState;
    Serial.print("batteryState value in warning initialization: ");
    Serial.println(batteryState);
    warnAlarmDataStruct.bpOutOfRange = &bpOutOfRange;
    warnAlarmDataStruct.tempOutOfRange = &tempOutOfRange;
    warnAlarmDataStruct.pulseOutOfRange = &pulseOutOfRange;
    warnAlarmDataStruct.bpHigh = &bpHigh;
    warnAlarmDataStruct.tempHigh = &tempHigh;
    warnAlarmDataStruct.pulseLow = &pulseLow;
    warnAlarmDataStruct.rrOutOfRange = &rrRange;
    warnAlarmDataStruct.ekgOutOfRange = &ekgRange;
    
    
    displayDataStruct disDataStruct;
    Serial.print("batteryState value in before display initialization: ");
    Serial.println(batteryState);
    disDataStruct.tempCorrected = tempCorrectedBuf;
    disDataStruct.sbpCorrected = sbpCorrectedBuf;
        disDataStruct.dbpCorrected = dbpCorrectedBuf;
    disDataStruct.pulseRateCorrected = pulseRateCorrectedBuf;
    disDataStruct.rrCorrected = rrCorrectedBuf;
    disDataStruct.ekgCorrected = ekgCorrectedBuf;

    disDataStruct.batteryState = &batteryState;
    Serial.print("batteryState value in display initialization: ");
    Serial.println(batteryState);
    disDataStruct.bpOutOfRange = &bpOutOfRange;
    disDataStruct.tempOutOfRange = &tempOutOfRange;
    disDataStruct.pulseOutOfRange = &pulseOutOfRange;
    disDataStruct.rrOutOfRange = &rrRange;
    disDataStruct.bpHigh = &bpHigh;
    disDataStruct.tempHigh = &tempHigh;
    disDataStruct.pulseLow = &pulseLow;
        disDataStruct.ekgOutOfRange = &ekgRange;

    Serial.print("batteryState value before update initialization: ");
    Serial.println(batteryState);
    
    updateStatusStruct updateStatStruct;
    updateStatStruct.batteryState = &batteryState;
    Serial.print("batteryState value in update initialization: ");
    Serial.println(batteryState);


    keypadDataStruct keypadStruct;

    keypadStruct.tempCorrected = tempCorrectedBuf;
    keypadStruct.sbpCorrected = sbpCorrectedBuf;
    keypadStruct.dbpCorrected = dbpCorrectedBuf;
    keypadStruct.pulseRateCorrected = pulseRateCorrectedBuf;
    keypadStruct.rrCorrected = rrCorrectedBuf;
    keypadStruct.ekgCorrected = ekgCorrectedBuf;
    keypadStruct.batteryState = &batteryState;
    keypadStruct.bpOutOfRange = &bpOutOfRange;
    keypadStruct.tempOutOfRange = &tempOutOfRange;
    keypadStruct.pulseOutOfRange = &pulseOutOfRange;
    keypadStruct.rrOutOfRange = &rrRange;
    keypadStruct.bpHigh = &bpHigh;
    keypadStruct.tempHigh = &tempHigh;
    keypadStruct.pulseLow = &pulseLow;
            keypadStruct.ekgOutOfRange = &ekgRange;



    communicationsDataStruct communicationsStruct;
    communicationsStruct.measurementSelection = measurementSelection;
    communicationsStruct.tempCorrected = tempCorrectedBuf;
    communicationsStruct.sbpCorrected = sbpCorrectedBuf;
    communicationsStruct.dbpCorrected = dbpCorrectedBuf;
    communicationsStruct.pulseRateCorrected = pulseRateCorrectedBuf;
    communicationsStruct.rrCorrected = rrCorrectedBuf;
    communicationsStruct.ekgCorrected = ekgCorrectedBuf;

      // Declare and assign task control blocks  
    measureDataTask.taskPtr = &measureData;
    measureDataTask.dataPtr = &meaDataStruct;
    
    computeDataTask.taskPtr = &computeData;
    computeDataTask.dataPtr = &comDataStruct;
    
    warningAlarmTask.taskPtr = &warningAlarm;
    warningAlarmTask.dataPtr = &warnAlarmDataStruct;
    
    displayDataTask.taskPtr = &displayData;
    displayDataTask.dataPtr = &disDataStruct;
    
    updateBatteryStatusTask.taskPtr = &updateBatteryStatus;
    updateBatteryStatusTask.dataPtr = &updateStatStruct;

    keypadTask.taskPtr = &keypad;
    keypadTask.dataPtr = &keypadStruct;

    communicationTask.taskPtr = &checkStart;
    communicationTask.dataPtr = &communicationsStruct;
    
    
//     Define array of TCB
    
//    taskQueue[0] = &communicationTask;
//    taskQueue[1] = &computeDataTask;
//    taskQueue[2] = &warningAlarmTask;
//    taskQueue[3] = &displayDataTask;
//    taskQueue[4] = &updateBatteryStatusTask;
//    taskQueue[5] = &measureDataTask;
   // taskQueue[6] = &keypadTask;
    
}
//
//TCB* initializeTCB(void (*task)(void*), void* dataPtr) {
//  TCB* TCB1 = (TCB*) malloc (sizeof(TCB1));
//  TCB1->taskPtr = task; //curr->task->taskPtr(curr->task->dataPtr);
//  TCB1->dataPtr = dataPtr;
//  return TCB1;
//}
//
//void initializeTCBStructs() {
//  //updateBatteryStatusTask = initializeTCB(&updateBatteryStatus, (measureDataStruct*) &batteryStatusData);
//  measureDataTask = initializeTCB(&measureData, (measureDataStruct*) &measureData);
//  computeDataTask = initializeTCB(&computeData, (computeDataStruct*) &computeData);
//  checkWarningAlarmTask = initializeTCB(&warningAlarm, (warningAlarmStruct*) &warningAlarmData);
//  displayDataTask = initializeTCB(&displayData, (displayDataStruct*) &displayData);
//}

void intializeLinkedListNodes() {
  updateBatteryStatusLL = createNode(&updateBatteryStatusTask);
  measureDataLL = createNode(&measureDataTask);
  computeDataLL = createNode(&computeDataTask);
  checkWarningAlarmLL = createNode(&warningAlarmTask);
  displayDataLL = createNode(&displayDataTask);
  communicationLL = createNode(&communicationTask);
  remoteCommLL = createNode(&keypadTask);
}

void intializeMisc() {
  head = updateBatteryStatusLL;
  curr = head;
  insertComputeTask = false;
  deleteComputeTask = false;
}


