//#include <stdlib>
#include "MEGA_Linked_List_Node_Functions.h"
#include "MEGA_Linked_List_Node_Struct.h"
#include "TCBStruct.h"
#include <Arduino.h> //for null

// Creates and returns reference to linked list node
LinkedListNode* createNode(TCB* task) {
  LinkedListNode* node = (LinkedListNode*) malloc (sizeof(LinkedListNode));

  if (node) {
    node->task = task;
    node->previousTask = NULL;
    node->nextTask = NULL;
  }

  return node;
}

// Inserts given node between two given nodes
void insertTask(LinkedListNode* previousTask, LinkedListNode* task, LinkedListNode* nextTask) {
  if (previousTask != NULL) {
    previousTask->nextTask = task;  
  }
  
  if (nextTask != NULL) {
    nextTask->previousTask = task;
  }
  
  task->previousTask = previousTask;
  task->nextTask = nextTask;
}

// Deletes given node
void deleteTask(LinkedListNode* task) {
  LinkedListNode* previousTask = task->previousTask;
  LinkedListNode* nextTask = task->nextTask;

  if (previousTask != NULL) {
    previousTask->nextTask = nextTask;
  }

  if (nextTask != NULL) {
    nextTask->previousTask = previousTask; 
  }
  
  task->previousTask = NULL;
  task->nextTask = NULL;
}


