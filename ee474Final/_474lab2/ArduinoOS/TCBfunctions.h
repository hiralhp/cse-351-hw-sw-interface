#ifndef TCBFUNCTIONS_INCLUDE
#define TCBFUNCTIONS_INCLUDE

#include <stdio.h>
#include <stdlib.h>

#include "simulation.h"

void measureData(void* data);

void computeData(void* data);

void warningAlarm(void* data);

void displayData(void* data);

void updateBatteryStatus(void* data);

#endif


