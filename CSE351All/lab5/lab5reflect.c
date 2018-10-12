#include <stdlib.h>
#include <stdio.h>

// Assume called with one argument that is a positive number
int main(int argc, char**argv) {
  int *p, *q;
  p = (int*)malloc(sizeof(int));
  *p = 42;
  free(p);
  q = (int*)malloc(atoi(argv[1])*sizeof(int));
  *q = 43;
  *p = 44;
  printf("%d\n",*q);
  free(q);
}

