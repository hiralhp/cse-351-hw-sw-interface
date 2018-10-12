#include <stdio.h>
#include <stdlib.h>

/*#include "initialization.h"
 #include "simulation.h"
 #include "boolean.h"
 #include "TCBStruct.h"
 #include "TCBfunctions.h"
 #include "dataStructs.h"
 #include "dataTypes.h"*/

void measureData(void* data);

void computeData(void* data);

void warningAlarm(void* data);

void displayData(void* data);

void updateBatteryStatus(void* data);

//void remoteWarnFunc(keypadDataStruct* ptr);

void keypad(void* ptr);

//Bool clearOrBack();
void warningButton();

void communicationCall(void* data);

void remoteComm(void* bptr);

