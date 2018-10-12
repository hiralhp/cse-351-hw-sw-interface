#ifndef MEGA_LINKED_LIST_NODE_INCLUDE
#define MEGA_LINKED_LIST_NODE_INCLUDE
#include "TCBStruct.h"

typedef struct LinkedListNode {
  TCB* task;
  LinkedListNode* previousTask;
  LinkedListNode* nextTask;
} LinkedListNode;

#endif

