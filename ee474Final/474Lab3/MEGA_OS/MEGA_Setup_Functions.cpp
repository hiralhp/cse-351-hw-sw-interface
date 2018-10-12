#include <stdlib.h>
#include "Primitive_Data_Types.h"
  #include "MEGA_Circular_Buffer_Struct.h"
  #include "MEGA_Circular_Buffer_Functions.h"
  #include "MEGA_Linked_List_Node_Struct.h"
  #include "MEGA_Linked_List_Node_Functions.h"
  #include "MEGA_Pseudo_Interrupts_Functions.h"
  #include "MEGA_Data_Structs.h"
  #include "MEGA_TCB_Struct.h"
  #include "MEGA_TCB_Struct_Functions.h"
  #include "MEGA_Variables.h"
  #include "MEGA_Timer_Interrupt_Functions.h"
  #include "MEGA_Setup_Functions.h"
  #include "TimerOne.h"
#include <Arduino.h>
void initalizeArrays(int initializeValue, unsigned int* array) {
  //unsigned int* array = (unsigned int*) malloc (BUFFER_SIZE * sizeof(unsigned int));
  for (int i = 0; i < BUFFER_SIZE; i++) {
    array[i] = initializeValue;
  }
  return array;
}

void initializeVariables() {
  timerTick = 0;
  batteryState = 200;
  functionSelect = 0;
  measurementSelect = 0;
  alarmAcknowledge = 0;

  initalizeArrays(75, tempRawBuff);
  initalizeArrays(80, sysPressRawBuff);
  initalizeArrays(80, diasPressRawBuff);
  initalizeArrays(0, pulseRateRawBuff);

  initalizeArrays(0, tempCorrBuff);
  initalizeArrays(0, sysPressCorrBuff);
  initalizeArrays(0, diasPressCorrBuff);
  initalizeArrays(0, pulseRateCorrBuff);

  tempOutOfRange = false;
  bpOutOfRange = false;
  pulseOutOfRange = false;

  bpHigh = false;
  tempHigh = false;
  pulseLow = false;
  batteryLow = false;

  batteryLowCounter = 0;
  tempOutOfRangeCounter = 0;
  bpOutOutOfRangeCounter = 0;
  pulseOutOfRangeCounter = 0;
}

void intializeBuffers(unsigned int circularArray[], unsigned int* arr) {
  //CircularBuffer* buffer = (CircularBuffer*) malloc (sizeof(CircularBuffer));
CircularBuffer* buffer;
  buffer->circularBuffer = circularArray; //curr->task->taskPtr(curr->task->dataPtr);
  buffer->start = 0;
  buffer->end = 0;
  buffer->length = BUFFER_SIZE;
  buffer->isEmpty = true;
  buffer->isFull = false;
  buffer->write = &write;
  buffer->read = &read;

  arr = buffer->circularBuffer;
}

void initializeCircularBuffers() {
  intializeBuffers(tempRawBuff, tempRawCirBuff->circularBuffer);
  intializeBuffers(sysPressRawBuff, sysPressRawCirBuff->circularBuffer);
  intializeBuffers(diasPressRawBuff, diasPressRawCirBuff->circularBuffer);
  intializeBuffers(pulseRateRawBuff, pulseRateRawCirBuff->circularBuffer);

  intializeBuffers(tempCorrBuff, tempCorrCirBuff->circularBuffer);
  intializeBuffers(sysPressCorrBuff, sysPressCorrCirBuff->circularBuffer);
  intializeBuffers(diasPressCorrBuff, diasPressCorrCirBuff->circularBuffer);
  intializeBuffers(pulseRateCorrBuff, pulseRateCorrCirBuff->circularBuffer);
}

void initializeDataStructs() {
  /*
  batteryStatusData.batteryState = batteryState;

  measureData.tempRawCirBuff = tempRawCirBuff;
  measureData.sysPressRawCirBuff = sysPressRawCirBuff;
  measureData.diasPressRawCirBuff = diasPressRawCirBuff;
  measureData.pulseRateRawCirBuff = pulseRateRawCirBuff;

  computeData.tempRawCirBuff = tempRawCirBuff;
  computeData.sysPressRawCirBuff = &sysPressRawCirBuff;
  computeData.diasPressRawCirBuff = &diasPressRawCirBuff;
  computeData.pulseRateRawCirBuff = &pulseRateRawCirBuff;
  computeData.tempCorrCirBuff = &tempCorrCirBuff;
  computeData.sysPressCorrCirBuff = &sysPressCorrCirBuff;
  computeData.diasPressCorrCirBuff = &diasPressCorrCirBuff;
  computeData.pulseRateCorrCirBuff = &pulseRateCorrCirBuff;

  warningAlarmData.batteryState = &batteryState;
  warningAlarmData.tempCorrCirBuff = tempCorrCirBuff;
  warningAlarmData.sysPressCorrCirBuff = sysPressCorrCirBuff;
  warningAlarmData.diasPressCorrCirBuff = diasPressRawCirBuff;
  warningAlarmData.pulseRateCorrCirBuff = pulseRateCorrCirBuff;
  warningAlarmData.tempOutOfRange = &tempOutOfRange;
  warningAlarmData.bpOutOfRange = &bpOutOfRange;
  warningAlarmData.pulseOutOfRange = &pulse;
  warningAlarmData.batteryLow = &batteryLow;
  warningAlarmData.tempHigh = &tempHigh;
  warningAlarmData.bpHigh = &bpHigh;
  warningAlarmData.pulseLow = &pulseLow;
  warningAlarmData.alarmAcknowledge = &alarmAcknowledge;
  warningAlarmData.batteryLowCounter = &batteryLowCounter;
  warningAlarmData.tempOutOfRangeCounter = &tempOutOfRangeCounter;
  warningAlarmData.bpOutOutOfRangeCounter = &bpOutOutOfRangeCounter;
  warningAlarmData.pulseOutOfRangeCounter = &pulseOutOfRangeCounter;

  displayData.functionSelect = &functionSelect;
  displayData.measurementSelect = &measureSelect;
  displayData.alarmAcknowledge = &alarmAcknowledge;
  displayData.batteryState = &batteryState;
  displayData.tempCorrCirBuff = tempCorrCirBuff;
  displayData.sysPressCorrCirBuff = sysPressRawCirBuff;
  displayData.diasPressCorrCirBuff = diasPressRawCirBuff;
  displayData.pulseRateCorrCirBuff = pulseRateCorrCirBuff;
  displayData.bpOutOfRange = &bpOutOfRange;
  displayData.tempOutOfRange = &tempOutOfRange;
  displayData.pulseOutOfRange = &pulseOutOfRange;
  displayData.batteryLow = &batteryLow;
  displayData.bpHigh = &bpHigh;
  displayData.tempHigh = &tempHigh;
  displayData.pulseLow = &pulseLow;
  */
}

TCB* initializeTCB(void (*task)(void*), void* dataPtr) {
  TCB* TCB1 = (TCB*) malloc (sizeof(TCB1));
  TCB1->taskPtr = task; //curr->task->taskPtr(curr->task->dataPtr);
  TCB1->dataPtr = dataPtr;
  return TCB1;
}

void initializeTCBStructs() {
  updateBatteryStatusTCB = initializeTCB(&updateBatteryStatus, (Status*) &batteryStatusData);
  measureDataTCB = initializeTCB(&measureData, (MeasureData*) &measureData);
  computeDataTCB = initializeTCB(&computeData, (ComputeData*) &computeData);
  checkWarningAlarmTCB = initializeTCB(&warningAlarm, (WarningAlarmData*) &warningAlarmData);
  displayDataTCB = initializeTCB(&displayData, (DisplayData*) &displayData);
}

void intializeLinkedListNodes() {
  updateBatteryStatusLL = createNode(updateBatteryStatusTCB);
  measureDataLL = createNode(measureDataTCB);
  computeDataLL = createNode(computeDataTCB);
  checkWarningAlarmLL = createNode(checkWarningAlarmTCB);
  displayDataLL = createNode(displayDataTCB);
}

void intializeMisc() {
  head = updateBatteryStatusLL;
  curr = head;
  insertComputeTask = false;
  deleteComputeTask = false;
}
