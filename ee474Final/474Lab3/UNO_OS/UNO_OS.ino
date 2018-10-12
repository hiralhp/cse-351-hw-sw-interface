#include <Arduino.h>

#define baudRate 9600
#define interruptPin 10   // Arbitrary pin. Fix later.
#define delayTimeSec 0.1

unsigned int pulseCount = 0;

void setup() {
  Serial.begin(baudRate);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), incrementPulseCount, RISING);
}

void loop() {
    waitForMEGASendByte();

    // Replace with actual simulation names from last lab.
    unsigned int rawTemp = getRawTemp();
    unsigned int rawSysPres = getRawSysPres();
    unsigned int rawDiasPres = getRawDiasPres();
    unsigned int rawPulseRate = getRawPulseRate();

    Serial.write(rawTemp);
    Serial.write(rawSysPres);
    Serial.write(rawDiasPres);
    Serial.write(rawPulseRate);
}

void incrementPulseCount() {
  pulseCount += 1;
}

void waitForMEGASendByte() {
  while(Serial.available() < 1) {
    if (Serial.available() == 1) {
      break;
    }
    continue;
  }

  Serial.read();
}

int getRawPulseRate() {
  pulseCount = 0;

  delay(1000 * delayTimeSec);

  unsigned int rawPulseRate = (numPulses / delayTimeMS) * 60;

  return rawPulseRate;
}
