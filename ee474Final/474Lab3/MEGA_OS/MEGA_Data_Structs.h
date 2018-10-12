#ifndef MEGA_DATA_STRUCTS_INCLUDE
#define MEGA_DATA_STRUCTS_INCLUDE

typedef struct Status {
  unsigned short* batteryState;
} Status;

typedef struct MeasureData {
  CircularBuffer* tempRawCirBuff;
  CircularBuffer* sysPressRawCirBuff;
  CircularBuffer* diasPressRawCirBuff;
  CircularBuffer* pulseRateRawCirBuff;
} MeasureData;

typedef struct ComputeData {
  CircularBuffer* tempRawCirBuff;
  CircularBuffer* sysPressRawCirBuff;
  CircularBuffer* diasPressRawCirBuff;
  CircularBuffer* pulseRateRawCirBuff;
  CircularBuffer* tempCorrCirBuff;
  CircularBuffer* sysPressCorrCirBuff;
  CircularBuffer* diasPressCorrCirBuff;
  CircularBuffer* pulseRateCorrCirBuff;
} ComputeData;

typedef struct WarningAlarmData {
  unsigned short* batteryState;
  CircularBuffer* tempCorrCirBuff;
  CircularBuffer* sysPressCorrCirBuff;
  CircularBuffer* diasPressCorrCirBuff;
  CircularBuffer* pulseRateCorrCirBuff;
  bool* tempOutOfRange;
  bool* bpOutOfRange;
  bool* pulseOutOfRange;
  bool* batteryLow;
  bool* tempHigh;
  bool* bpHigh;
  bool* pulseLow;
  bool* alarmAcknowledge;
  unsigned short* batteryLowCounter;
  unsigned short* tempOutOfRangeCounter;
  unsigned short* bpOutOutOfRangeCounter;
  unsigned short* pulseOutOfRangeCounter;
} WarningAlarmData;

typedef struct DisplayData {
  unsigned short* functionSelect;
  unsigned short* measurementSelect;
  bool* alarmAcknowledge;
  unsigned short* batteryState;
  CircularBuffer* tempCorrCirBuff;
  CircularBuffer* sysPressCorrCirBuff;
  CircularBuffer* diasPressCorrCirBuff;
  CircularBuffer* pulseRateCorrCirBuff;
  bool* bpOutOfRange;
  bool* tempOutOfRange;
  bool* pulseOutOfRange;
  bool* batteryLow;
  bool* bpHigh;
  bool* tempHigh;
  bool* pulseLow;
} DisplayData;

#endif
