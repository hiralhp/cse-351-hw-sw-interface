#include <stdlib.h>
#include "MEGA_Circular_Buffer_Functions.h"

// Writes to buffer. Writes to next available buffer index
void write(int* circularBuffer, int* start, int* end, int length, bool* isEmpty, bool* isFull, int data) {
  *start = (*start + 1) % length;

  // Check if buffer is full
  if ( (*start == *end) && !*isFull ) {
    *isFull = true;
  }

  // Only add to buffer if buffer is not full
  if (!*isFull) {
    circularBuffer[*start] = &data;

    // isEmpty field becomes false if "first" element of "empty" buffer was added
    if (*isEmpty) {
      *isEmpty = false;
    }

  }
}

// Reads from buffer. Reads from next available buffer index
int read(int* circularBuffer, int* start, int* end, int length, bool* isEmpty, bool* isFull, int data) {
  // Only read if buffer is not "empty"
  if (!*isEmpty) {
    // Get read index
    int readIndex = *end;

    // Update end index
    *end = (*end + 1) % length;

    // isEmpty field becomes true if read and write index same and not empty
    if ( (*end == *start) && !*isEmpty ) {
      *isEmpty = true;
    }

    // isFull field beocmes false if buffer was full
    if (*isFull) {
      *isFull = false;
    }
    return circularBuffer[readIndex];
  }
}
