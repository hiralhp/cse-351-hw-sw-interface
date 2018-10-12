#ifndef TCB_INCLUDE
#define TCB_INCLUDE

// Define task control block structure
struct taskControlBlockStruct {
    void (*taskPtr)(void*);
    void* dataPtr;
};
typedef struct taskControlBlockStruct TCB;

#endif

