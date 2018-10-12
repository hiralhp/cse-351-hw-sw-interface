#include <Arduino.h>
#include "boolean.h"
#include <math.h>
#include <float.h>
#include "optfft.h"
#define baudRate 9600
#define delayTimeSec 0.1
#include <avr/pgmspace.h>



/*******************************************************************************/
const byte ledPin = 13;

unsigned int temperatureRaw = 50;
unsigned int systolicPressRaw = 80;
unsigned int diastolicPressRaw = 80;

Bool valsToSend[8] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

// Button/switch variables
const int buttonPin = 9;
const int diasPin = 11;
const int sysPin = 12;
int buttonState = 0;
int pinState11 = 0;
int pinState12 = 0;

int currBloodPressure = 80;

unsigned long lastCalcPulseTime = millis() / 1000;
unsigned long currCalcPulseTime = millis() / 1000;

// Pulse Variables
const byte pulsePin = 3;
volatile byte state = LOW;
unsigned int oldPulse = 50;
unsigned int pulseRateRaw = 50;
unsigned int percentChangePulse[8];
unsigned int percentChangeCountPulse = 0;
volatile unsigned int pulseCount = 0;

// Respiration Variables

unsigned int oldResp = 50;
unsigned int respRateRaw = 50;
unsigned int percentChangeResp[8];
unsigned int percentChangeCountResp = 0;

// Temperature Variables
unsigned int temperaturePin = A0;
int temperatureValue = 0;
unsigned int convertedTemp = 0;



// time stuff
unsigned long oldTimePulse = millis();
unsigned long newTimePulse = millis();

// EKG Raw Data Buffer
int ekgReadPin = 5; 
signed int ekgRawDataBuffer[256];


int fft = 0;
//unsigned int ekgMax = 0;
//unsigned char ekgMaxIndex = 0;

void pulseCountIncrement(){  
  oldPulse = pulseRateRaw;
  newTimePulse = millis();
  pulseCount++;
//*25/11-250
  if(newTimePulse - oldTimePulse != 0){
    pulseRateRaw = (newTimePulse - oldTimePulse);
  }

  unsigned int temp15Pulse = (abs(pulseRateRaw - oldPulse) / abs(oldPulse)) * 100;
  if (temp15Pulse > 15) {
    percentChangePulse[percentChangeCountPulse] = pulseRateRaw;
    percentChangeCountPulse++;
    if (percentChangeCountPulse == 8) {
      percentChangeCountPulse = 0;
    }
  }

  if (pulseRateRaw > 200) {
    pulseRateRaw = 200;
  } else if (pulseRateRaw < 10) {
    pulseRateRaw = 10;
  }

  pulseCount = 0;
  oldTimePulse = newTimePulse;

  calcRespiration();
}

void setup() {

  Serial.begin(9600);
  //pinMode(interruptPin, INPUT_PULLUP);
  
  ////Serial.println("Reached!");

  // Button/switch setup
  pinMode(buttonPin, INPUT);
  pinMode(diasPin, INPUT);
  pinMode(sysPin, INPUT);
  pinMode(ledPin, OUTPUT);
//  pinMode(pulsePin, INPUT_PULLUP);
  

  // Attaching Interupts
  attachInterrupt(digitalPinToInterrupt(2), pulseCountIncrement, RISING);
  //attachInterrupt(digitalPinToInterrupt(2), pulseCountIncrement, RISING);

}


 

//returns the pulse count from the pulse rate transducer
//call needs to divide pulse count by 1 second and multiply by 60 to get BPM
/*void calcPulse(struct MeasureData* measureData) {
  unsigned long start = millis();
  while ((millis() - start) < 5000)
  {
    newVal = digitalRead(inPin);
    digitalWrite(outPin, newVal);
    if(newVal != oldVal) { 
      oldVal = newVal; 
      if(newVal == 1) {
        pulseCount++; 
        
      }
    }
  }
}*/


void loop() {
  digitalWrite(ledPin, state);
  signed int ekgImagDataBuffer[256];
  

   if (Serial.available()){ //start bit
   int startBit = Serial.read(); //read start bit
   while(Serial.available()){ //if size = 1 then it is the end bit
    int call = Serial.read();
      if(call == 0){
        valsToSend[0] = TRUE;
      }
      else if(call == 1){
        valsToSend[1] = TRUE;
       
      }
     else  if(call == 2){
        valsToSend[2] = TRUE;

      }
      else if(call == 3){
        valsToSend[3] = TRUE;

      }else if(call == 6){
        valsToSend[6] = TRUE;

      } else if(call == 7){
        valsToSend[7] = TRUE;

      } else {
        int endBit = Serial.read(); //end bit
      }
   }
  // int endBit = Serial.read(); //end bit
   
   Serial.write(200);

        
        if(valsToSend[0] == TRUE){
        
          Serial.write(temperatureRaw);
        } if(valsToSend[1] == TRUE){
          //Serial.write();
          
          
          Serial.write(pulseRateRaw);
       
 
        } if(valsToSend[2] == TRUE){
          //Serial.write();

          Serial.write(pulseRateRaw);

        } if(valsToSend[3] == TRUE){
          //Serial.write(3);

          Serial.write(currBloodPressure);

        
         } if(valsToSend[6] == TRUE){
          //Serial.write(3);

          Serial.write(respRateRaw);

       
         } if(valsToSend[6] == TRUE){
          //Serial.write(3);

          Serial.write(fft);

        }

   
  Serial.write(105);
   }
  valsToSend[0] = FALSE;
  valsToSend[1] = FALSE;
  valsToSend[2] = FALSE;
  valsToSend[3] = FALSE;
  valsToSend[6] = FALSE;
  valsToSend[7] = FALSE;
////uncomment this for communication
//if (Serial.available()) {
//      Serial.read();
//      Serial.write(temperatureRaw);
//      Serial.write(pulseRateRaw);
//      Serial.write(currBloodPressure);
//      Serial.write(fft);
//      //Serial.write(currBloodPressure);
//      //Serial.write(respRateRaw);
//      Serial.write(244); 
//    }
//  if(millis() % 5 == 0){
//   detectPressure();
//  }
  detectTemperature();
  detectPressure();   
  //calculatePulse(); 
   // count++;

  // Collect raw EKG data
  /*for (int i = 0; i < 256; i++) {
   // if (i <= 256) {
      ekgRawDataBuffer[i] = analogRead(ekgReadPin); // Takes 100 microseconds

    delayMicroseconds(33);                        // Delay by 33 microseconds such that measurement taken every 133 microseconds, which approximately corresponds to sample rate of 7.5 kHz (Nyquist rate).
//delay(200);
    //Serial.print(i);
    //Serial.print(": ");
    //Serial.println(ekgRawDataBuffer[i]);
    
  }*/
  
for (int i = 0; i < 256; i++) {
  ekgRawDataBuffer[i] = analogRead(ekgReadPin)*21/205-31; // Min: 0, Max: Approx. 615
  //delayMicroseconds(80);
   //Serial.print(i);
   //Serial.print(": ");
   //Serial.println(ekgRawDataBuffer[i]);
}

for (int i = 0; i < 256; i++) {
  ekgImagDataBuffer[i] = 0;
}
  // Find m_index from raw EKG data
//  m_indexFind:
//  for (int i = 0; i < 128; i++) {
//    if (ekgRawDataBuffer[i] >= ekgMax) {
//      ekgMax = ekgRawDataBuffer[i];
//      ekgMaxIndex = i;
//    }
//  }
//fft = ekgMaxIndex;
 fft = optfft(ekgRawDataBuffer, ekgImagDataBuffer);
 // Serial.print("fft index: ");
 // Serial.println(fft);
  
//  // Debugging print statements
//  delay(10000);
//  
//  for (int i = 0; i < 256; i++) {
//    ////Serial.println("Index: %d", i);
//    ////Serial.println("Value: %d", ekgRawDataBuffer[i]);
//
//    //Serial.print("Index: ");
//    //Serial.println(i);
//    //Serial.print("Value: ");
//    //Serial.println(ekgRawDataBuffer[i]);
//  }
//
//  delay(10000);
//
//  //System.out.println(ekgMax);
//  //Serial.print("EKG Max: ");
//  //Serial.println(ekgMax);
 // //Serial.print("EKG Max Index: ");
  ////Serial.println(ekgMaxIndex);
}

void detectPressure(){
  if(millis() % 1000 == 0){
     buttonState = digitalRead(buttonPin);
    pinState11 = digitalRead(diasPin);
    pinState12 = digitalRead(sysPin);
    
    if (buttonState == HIGH && pinState12 == HIGH) {
      currBloodPressure = currBloodPressure * 1.1;
      ////Serial.println(currBloodPressure);
    } else if (buttonState == HIGH && pinState11 == HIGH) {
      currBloodPressure = currBloodPressure * 0.9;
      ////Serial.println(currBloodPressure);
    }
    if(currBloodPressure <50){
       currBloodPressure = 50;
    } else if (currBloodPressure > 150){
      currBloodPressure = 150;
    }
  }
}

void calcRespiration() {
  oldResp = respRateRaw;
  respRateRaw = pulseRateRaw * 4.0 / 6.0;
  unsigned int temp15Resp = (abs(respRateRaw - oldResp) / abs(oldResp)) * 100;
  if (temp15Resp > 15) {
    percentChangeResp[percentChangeCountResp] = respRateRaw;
    percentChangeCountResp++;
    if (percentChangeCountResp == 8) {
      percentChangeCountResp = 0;
    }
  }

  if (respRateRaw > 50) {
    respRateRaw = 50;
  } else if (respRateRaw < 10) {
    respRateRaw = 10;
  }
  
}

void detectTemperature() {
  // convertedTemp
  temperatureValue = analogRead(temperaturePin);
  temperatureValue = temperatureValue  * 5.0 / 1023.0;
  temperatureRaw = temperatureValue * 12;
  
}

