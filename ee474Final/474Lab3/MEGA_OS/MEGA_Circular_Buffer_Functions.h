#ifndef MEGA_CIRCULAR_BUFFER_FUNCTIONS_INCLUDE
#define MEGA_CIRCULAR_BUFFER_FUNCTIONS_INCLUDE

void write(int* circularBuffer, int* start, int* end, int length, bool* isEmpty, bool* isFull, int data);

int read(int* circularBuffer, int* start, int* end, int length, bool* isEmpty, bool* isFull, int data);

#endif
