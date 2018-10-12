#ifndef MEGA_TCB_STRUCTS_INCLUDE
#define MEGA_TCB_STRUCTS_INCLUDE

typedef struct TCB {
  void (*taskPtr)(void*);
  void* dataPtr;
} TCB;

#endif
