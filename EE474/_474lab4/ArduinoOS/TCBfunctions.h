#ifndef TCBFUNCTIONS_INCLUDE
#define TCBFUNCTIONS_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include "boolean.h"
#include "simulation.h"

void measureData(void* data);

void computeData(void* data);

void warningAlarm(void* data);

void displayData(void* data);

void updateBatteryStatus(void* data);

void keypad(void* ptr);

void setupButton();

void warningButton();

void checkStart(void* data);

Bool clearOrBack();

Bool checkBegin();

Bool checkChoices();

void choicesSetupButtons();

void changeColor(void* ptr);

#endif



