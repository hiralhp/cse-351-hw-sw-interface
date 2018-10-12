#ifndef MEGA_LINKED_LIST_NODE_FUNCTIONS_INCLUDE
#define MEGA_LINKED_LIST_NODE_FUNCTIONS_INCLUDE
#include "MEGA_Linked_List_Node_Struct.h"
#include "MEGA_TCB_Struct.h"

LinkedListNode* createNode(TCB* task);

void insertTask(LinkedListNode* previousTask, LinkedListNode* nextTask, LinkedListNode* task);

void deleteTask(LinkedListNode* task);

#endif
