#include <stdio.h>
#include <stdlib.h>

#include "initialization.h"
#include "simulation.h"

unsigned int simulateTemperatureRawCount = 0;
Bool simulateTemperatureRawIncrement = TRUE;

unsigned int simulateSystolicPressRawCount = 0;
unsigned int simulateSystolicPressRawCountInitalValue = 120;
Bool simulateSystolicPressRawComplete = FALSE;

unsigned int simulateDiastolicPressRawCount = 0;
unsigned int simulateDiastolicPressRawCountInitialValue = 80;
Bool simulateDiastolicPressRawComplete = TRUE;

unsigned int simulatePulseRateRawCount = 0;
Bool simulatePulseRateIncrement = FALSE;
/*
void simulateTemperatureRaw() {
    simulateTemperatureRawCount++;
    if ( temperatureRaw < 50 && simulateTemperatureRawIncrement == TRUE) {
        if (simulateTemperatureRawCount % 2 == 0) {
            temperatureRaw += 2;
        } else {
            temperatureRaw -= 1;
        }
        if (temperatureRaw > 50) {
            simulateTemperatureRawIncrement = FALSE;
        }
    } else {
        if (simulateTemperatureRawCount % 2 == 0) {
            temperatureRaw += 1;
        } else {
            temperatureRaw -= 2;
        }
        if (temperatureRaw > 50) {
            simulateTemperatureRawIncrement = TRUE;
        }
    }
}

void simulateSystolicPressRaw() {
    simulateSystolicPressRawCount++;
    if (simulateSystolicPressRawComplete == FALSE &&
        simulateDiastolicPressRawComplete == TRUE ) {
        if (simulateSystolicPressRawCount % 2 == 0) {
            systolicPressRaw += 3;
        } else {
            systolicPressRaw -= 1;
        }
        if (systolicPressRaw > 100) {
            simulateSystolicPressRawComplete = TRUE;
            systolicPressRaw = simulateSystolicPressRawCountInitalValue;
        }
    }
}

void simulateDiastolicRateRaw() {
    simulateDiastolicPressRawCount++;
    if (simulateDiastolicPressRawComplete == FALSE &&
        simulateSystolicPressRawComplete == TRUE ) {
        if (simulateDiastolicPressRawCount % 2 == 0) {
            diastolicPressRaw -= 2;
        } else {
            diastolicPressRaw += 1;
        }
        if (diastolicPressRaw < 40) {
            simulateDiastolicPressRawComplete = TRUE;
            diastolicPressRaw = simulateSystolicPressRawCountInitalValue;
        }
    }
}












void simulatePulseRateRaw() {
    simulatePulseRateRawCount++;
    if (pulseRateRaw < 40 && simulatePulseRateIncrement == TRUE) {
        if (simulatePulseRateRawCount % 2 == 0) {
            pulseRateRaw -= 1;
        } else {
            pulseRateRaw += 3;
        }
    } else {
        if (simulatePulseRateRawCount % 2 == 0) {
            pulseRateRaw += 1;
        } else {
            pulseRateRaw -= 3;
        }   
    }
}
*/
