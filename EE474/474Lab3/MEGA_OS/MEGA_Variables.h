#ifndef MEGA_VARIABLES_INCLUDE
#define MEGA_VARIABLES_INCLUDE
  #include "MEGA_Circular_Buffer_Struct.h"
  #include "MEGA_Circular_Buffer_Functions.h"
  #include "MEGA_Data_Structs.h"

#define BUFFER_SIZE 8

extern unsigned short timerTick;

extern unsigned short batteryState;

extern unsigned short functionSelect;
extern unsigned short measurementSelect;
extern bool alarmAcknowledge;

extern unsigned int tempRawBuff[BUFFER_SIZE];
extern unsigned int sysPressRawBuff[BUFFER_SIZE];
extern unsigned int diasPressRawBuff[BUFFER_SIZE];
extern unsigned int pulseRateRawBuff[BUFFER_SIZE];

extern unsigned int tempCorrBuff[BUFFER_SIZE];
extern unsigned int sysPressCorrBuff[BUFFER_SIZE];
extern unsigned int diasPressCorrBuff[BUFFER_SIZE];
extern unsigned int pulseRateCorrBuff[BUFFER_SIZE];

extern bool tempOutOfRange;
extern bool bpOutOfRange;
extern bool pulseOutOfRange;

extern bool bpHigh;
extern bool tempHigh;
extern bool pulseLow;
extern bool batteryLow;

extern unsigned short batteryLowCounter;
extern unsigned short tempOutOfRangeCounter;
extern unsigned short bpOutOutOfRangeCounter;
extern unsigned short pulseOutOfRangeCounter;

extern CircularBuffer* tempRawCirBuff;
extern CircularBuffer* sysPressRawCirBuff;
extern CircularBuffer* diasPressRawCirBuff;
extern CircularBuffer* pulseRateRawCirBuff;

extern CircularBuffer* tempCorrCirBuff;
extern CircularBuffer* sysPressCorrCirBuff;
extern CircularBuffer* diasPressCorrCirBuff;
extern CircularBuffer* pulseRateCorrCirBuff;

extern Status batteryStatusData;
extern MeasureData measData; //changed variable name need to change everywhere else
extern ComputeData compData; //changed variable name need to change everywhere else
extern WarningAlarmData warningAlarmData;
extern DisplayData dispData; //changed variable name need to change everywhere else

extern TCB* updateBatteryStatusTCB;
extern TCB* measureDataTCB;
extern TCB* computeDataTCB;
extern TCB* checkWarningAlarmTCB;
extern TCB* displayDataTCB;

extern LinkedListNode* updateBatteryStatusLL;
extern LinkedListNode* measureDataLL;
extern LinkedListNode* computeDataLL;
extern LinkedListNode* checkWarningAlarmLL;
extern LinkedListNode* displayDataLL;

extern LinkedListNode* head;
extern LinkedListNode* curr;

extern bool insertComputeTask;
extern bool deleteComputeTask;

#endif
