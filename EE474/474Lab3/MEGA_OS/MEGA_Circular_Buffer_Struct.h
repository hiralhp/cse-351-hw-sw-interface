#ifndef MEGA_CIRCULAR_BUFFER_STRUCT_INCLUDE
#define MEGA_CIRCULAR_BUFFER_STRUCT_INCLUDE

typedef struct CircularBuffer {
  unsigned int* circularBuffer;
  unsigned int start;
  unsigned int end;
  unsigned int length;
  bool isEmpty;
  bool isFull;
  void (*write)(int*, int*, int*, int, bool*, bool*, int);
  int (*read)(int*, int*, int*, int, bool*, bool*, int);
} CircularBuffer;

#endif

/*

*/
