#include <stdlib.h>
#include "MEGA_Timer_Interrupt_Functions.h"

void incrementTimerTick() {
  *timerTick += 1;
}
