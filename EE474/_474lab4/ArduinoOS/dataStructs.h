#ifndef DATASTRUCTS_INCLUDE
#define DATASTRUCTS_INCLUDE
#include "boolean.h"
// Define data struct for measure data task
struct measureDataStructure {
    unsigned int* tempRaw;
    unsigned int* sbpRaw;
    unsigned int* dbpRaw;
    unsigned int* pulseRateRaw;
    unsigned int* rrRaw;
    unsigned int* ekgRaw;
    unsigned short* measurementSelection; 
};
typedef struct measureDataStructure measureDataStruct;

// Define data struct for compute data task
struct computeDataStructure {
    unsigned int*  tempRaw;
    unsigned int*  sbpRaw;
    unsigned int*  dbpRaw;
    unsigned int*  rrRaw;
    unsigned int* ekgRaw;
    unsigned int*  pulseRateRaw;
    unsigned char* tempCorrected;
    unsigned char* sbpCorrected;
    unsigned char* dbpCorrected;
    unsigned char*  rrCorrected;
    unsigned char* pulseRateCorrected;
    unsigned char* ekgCorrected;
    unsigned short* measurementSelection; 
};
typedef struct computeDataStructure computeDataStruct;

// Define data struct for warning/alarm task
struct warningAlarmDataStruct {
    unsigned char* tempCorrected;
    unsigned char* sbpCorrected;
    unsigned char* dbpCorrected;
    unsigned char* rrCorrected;
    unsigned char* pulseRateCorrected;
    unsigned char* ekgCorrected;
    unsigned short*  batteryState;
    Bool* bpOutOfRange;
    Bool* tempOutOfRange;
    Bool* pulseOutOfRange;
    Bool* bpHigh;
    Bool* tempHigh;
    Bool* pulseLow;
    Bool* rrOutOfRange;
    Bool* ekgOutOfRange;
};
typedef struct warningAlarmDataStruct warningAlarmStruct;



// Define data struct for display data task
struct displayDataStructure {
  char buffer[50];
//  Serial.print("Battery value in struct: ");
//   Serial.println(batteryState);
  
//    sprintf(buffer, "Battery Adr in initialization = %d", ((*ptr).batteryState));
  //  Serial.println(buffer);
    unsigned char* tempCorrected;
    unsigned char* sbpCorrected;
    unsigned char* dbpCorrected;
    unsigned char*  rrCorrected;
    unsigned char* pulseRateCorrected;
    unsigned char* ekgCorrected;
    unsigned short*  batteryState;
    Bool* bpOutOfRange;
    Bool* tempOutOfRange;
    Bool* pulseOutOfRange;
    Bool* bpHigh;
    Bool* tempHigh;
    Bool* pulseLow;
    Bool* rrOutOfRange;
    Bool* ekgOutOfRange;
};
typedef struct displayDataStructure displayDataStruct;

// Define data struct for update battery status task
struct updateBatteryStatusDataStructure {
unsigned short* batteryState;
};
typedef struct updateBatteryStatusDataStructure updateStatusStruct;


struct keypadDataStructure {
    unsigned char* tempCorrected;
    unsigned char* sbpCorrected;
    unsigned char* dbpCorrected;
    unsigned char*  rrCorrected;
    unsigned char* pulseRateCorrected;
    unsigned char* ekgCorrected;
    unsigned short*  batteryState;
    Bool* bpOutOfRange;
    Bool* tempOutOfRange;
    Bool* pulseOutOfRange;
    Bool* bpHigh;
    Bool* tempHigh;
    Bool* pulseLow;
    Bool* rrOutOfRange;
    Bool* ekgOutOfRange;

};
typedef struct keypadDataStructure keypadDataStruct;

struct communicationsDataStructure {
unsigned short* measurementSelection;
unsigned int*  tempRaw;
    unsigned int*  sbpRaw;
    unsigned int*  dbpRaw;
    unsigned int*  rrRaw;
    unsigned int* ekgRaw;
    unsigned char* tempCorrected;
    unsigned char* sbpCorrected;
    unsigned char* dbpCorrected;
    unsigned char* rrCorrected;
    unsigned char* pulseRateCorrected;
    unsigned char* ekgCorrected;


};
typedef struct communicationsDataStructure communicationsDataStruct;

#endif




