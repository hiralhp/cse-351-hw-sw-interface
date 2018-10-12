//
//  initialization.h
//  simple
//
//  Created by Hiral Patel on 4/16/18.
//  Copyright (c) 2018 Hiral Patel. All rights reserved.
//
#include "TCBStruct.h"
#include "boolean.h"
  #include "MEGA_Linked_List_Node_Struct.h"
  #include "MEGA_Linked_List_Node_Functions.h"
#ifndef __simple__initialization__
#define __simple__initialization__

extern LinkedListNode* updateBatteryStatusLL;
extern LinkedListNode* measureDataLL;
extern LinkedListNode* computeDataLL;
extern LinkedListNode* checkWarningAlarmLL;
extern LinkedListNode* displayDataLL;
extern LinkedListNode* communicationLL;
extern LinkedListNode* remoteCommLL;

extern LinkedListNode* head;
extern LinkedListNode* curr;

extern bool insertComputeTask;
extern bool deleteComputeTask;

extern unsigned int temperatureRawBuf[8];
extern unsigned int bloodPressRawBuf[16];
extern unsigned int pulseRateRaw[8];
extern unsigned int rrRawBuf[8];
extern unsigned int ekgRawBuf[16];
extern unsigned char tempCorrected[8];
extern unsigned char bloodPressCorrectedBuf[16];
extern unsigned char pulseRateCorrected[8];
extern unsigned char rrCorrectedBuf[8]; 
extern unsigned char ekgCorrectedBuf[16];
extern unsigned short batteryState;
extern Bool bpOutOfRange;
extern Bool tempOutOfRange;
extern Bool pulseOutOfRange;
extern Bool bpHigh;
extern Bool tempHigh;
extern Bool pulseLow;
extern TCB measureDataTask;
extern TCB computeDataTask;
extern TCB warningAlarmTask;
extern TCB displayDataTask;
extern TCB updateBatteryStatusTask;
extern TCB taskControlBlockStruct;
extern TCB keypadTask;
extern TCB communicationTask;
extern TCB* taskQueue[6];
extern unsigned short measurementSelection;
extern unsigned short alarmAcknowledge;
extern unsigned short functionSelect;

  void something();
void intializeMisc();
void intializeLinkedListNodes();
#endif /* defined(__simple__initialization__) */

