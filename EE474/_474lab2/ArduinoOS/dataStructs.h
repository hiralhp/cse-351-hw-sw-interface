#ifndef DATASTRUCTS_INCLUDE
#define DATASTRUCTS_INCLUDE
#include "boolean.h"
// Define data struct for measure data task
struct measureDataStructure {
    unsigned int* tempRaw[8];
    unsigned int* bpRaw[16];
    unsigned int* pulseRateRaw[8];
    unsigned short* measurementSelection; 
};
typedef struct measureDataStructure measureDataStruct;

// Define data struct for compute data task
struct computeDataStructure {
    unsigned int*  tempRaw[8];
    unsigned int*  bpRaw[16];
    unsigned int*  pulseRateRaw[8];
    unsigned char* tempCorrected[8];
    unsigned char* bpCorrected[16];
    unsigned char* pulseRateCorrected[8];
    unsigned short* measurementSelection; 
};
typedef struct computeDataStructure computeDataStruct;

// Define data struct for warning/alarm task
struct warningAlarmDataStruct {
    unsigned char* tempCorrected[8];
    unsigned char* bpCorrected[16];
    unsigned char* pulseRateCorrected[8];
    unsigned short*  batteryState;
    Bool* bpOutOfRange;
    Bool* tempOutOfRange;
    Bool* pulseOutOfRange;
    Bool* bpHigh;
    Bool* tempHigh;
    Bool* pulseLow;
};
typedef struct warningAlarmDataStruct warningAlarmStruct;



// Define data struct for display data task
struct displayDataStructure {
  char buffer[50];
//  Serial.print("Battery value in struct: ");
//   Serial.println(batteryState);
  
//    sprintf(buffer, "Battery Adr in initialization = %d", ((*ptr).batteryState));
  //  Serial.println(buffer);
    unsigned char* tempCorrected[8];
    unsigned char* bpCorrected[16];
    unsigned char* pulseRateCorrected[8];
    unsigned short*  batteryState;
    Bool* bpOutOfRange;
    Bool* tempOutOfRange;
    Bool* pulseOutOfRange;
    Bool* bpHigh;
    Bool* tempHigh;
    Bool* pulseLow;
};
typedef struct displayDataStructure displayDataStruct;

// Define data struct for update battery status task
struct updateBatteryStatusDataStructure {
unsigned short* batteryState;
};
typedef struct updateBatteryStatusDataStructure updateStatusStruct;

struct keypadDataStructure {
unsigned short* measurementSelection;
unsigned short* alarmAcknowledge;
unsigned short* functionSelect;

};
typedef struct keypadDataStructure keypadDataStruct;

struct communicationsDataStructure {
unsigned short* measurementSelection;
    unsigned char* tempCorrected[8];
    unsigned char* bpCorrected[16];
    unsigned char* pulseRateCorrected[8];


};
typedef struct communicationsDataStructure communicationsDataStruct;

#endif



