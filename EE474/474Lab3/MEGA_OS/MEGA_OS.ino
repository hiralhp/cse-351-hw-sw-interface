#//include <stdlib.h>
#include <Arduino.h>
//#include "TimerOne.h"

extern "C" {
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
}

void setup() {
  tftSetup();
  Timer1.initialize();
  Timer1.attachInterrupt(incrementTimerTick);
  initializeVariables();
  initializeCircularBuffers();
  initializeDataStructs();
  initializeTCBStructs();
  intializeLinkedListNodes();
  intializeMisc();

  
  Serial.begin(9600);
}

void loop() {
  
  if (timerTick % 2 == 0) {
    // Get user input from TFT
  }

  if (timerTick % 5  == 0) {
    curr = head;
    while(curr != NULL) {
      curr->task->taskPtr(curr->task->dataPtr);

      if (insertComputeTask) { // Need to modify values in functions!
        insertTask(measureDataLL, checkWarningAlarmLL, computeDataLL);
      }

      if (deleteComputeTask) { // Need to modify values in fucntions!
        deleteTask(computeDataLL);
      }
    }
  }
}

void tftSetup(){
    Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9341;
   
  }

  tft.begin(identifier);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
}


