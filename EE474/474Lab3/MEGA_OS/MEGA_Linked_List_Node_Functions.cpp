//#include <stdlib>
#include "MEGA_Linked_List_Node_Functions.h"
#include "MEGA_Linked_List_Node_Struct.h"
#include "MEGA_TCB_Struct.h"
#include <Arduino.h> //for null

// Creates and returns reference to linked list node
LinkedListNode* createNode(TCB* task) {
  LinkedListNode* node = (LinkedListNode*) malloc (sizeof(LinkedListNode));

  if (node) {
    node->previousTask = NULL;
    node->nextTask = NULL;
  }
}

// Inserts given node between two given nodes
void insertTask(LinkedListNode* previousTask, LinkedListNode* task, LinkedListNode* nextTask) {
  previousTask->nextTask = task;
  task->previousTask = previousTask;
  task->nextTask = nextTask;
  nextTask->previousTask = task;
}

// Deletes given node
void deleteTask(LinkedListNode* task) {
  LinkedListNode* previousTask = task->previousTask;
  LinkedListNode* nextTask = task->nextTask;

  previousTask->nextTask = nextTask;
  nextTask->previousTask = previousTask;
}
