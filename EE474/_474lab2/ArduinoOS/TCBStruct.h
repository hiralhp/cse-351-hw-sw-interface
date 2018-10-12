#ifndef TCB_INCLUDE
#define TCB_INCLUDE

// Define task control block structure
struct taskControlBlockStruct {
    void (*taskFuncPtr)(void*);
    void* taskDataPtr;
};
typedef struct taskControlBlockStruct TCB;

#endif
