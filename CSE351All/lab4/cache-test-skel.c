/*
Hiral Patel
CSE 351 - Spring 2018
Lab 4 - Mystery Caches

Mystery Cache Geometries:
mystery0:
    block size = 64 bytes
    cache size = 262144 bytes
    associativity = 1
mystery1:
    block size = 8 bytes
    cache size = 16384 bytes
    associativity = 4
mystery2:
    block size = 16 bytes
    cache size = 65536 bytes
    associativity = 16
mystery3:
    block size = 2 bytes
    cache size = 131072 bytes
    associativity = 2
*/

#include <stdlib.h>
#include <stdio.h>

#include "mystery-cache.h"

/*
 * NOTE: When using access_cache() you do not need to provide a "real" memory
 * addresses. You can use any convenient integer value as a memory address,
 * you should not be able to cause a segmentation fault by providing a memory
 * address out of your programs address space as the argument to access_cache.
 */

/*
   Returns the size (in B) of each block in the cache.
*/
int get_block_size(void) {
  int blockSize = 0;
  int i = 0;
    access_cache(0);
    while(access_cache(i)){
        blockSize++;
	i++;
    }
    return blockSize;
}

/*
   Returns the size (in B) of the cache.
*/
int get_cache_size(int block_size) {
    int currSize = 0;
    flush_cache();
    access_cache(0);
    while(access_cache(0)){
        flush_cache();
        currSize += block_size;
        for (int i=0; i<currSize; i+=block_size )
            access_cache(i);
        
    }
    currSize -= block_size;
    return currSize;
}

/*
   Returns the associativity of the cache.
*/
int get_cache_assoc(int cache_size) {
  int cacheAssociativity = 0;
  int cacheCheck = 1;
  flush_cache();
  access_cache(0);
  while (access_cache(0)) {
    int i = cache_size;
    cacheAssociativity = 0;
    while (i <= cacheCheck) {
      i += cache_size;
      cacheAssociativity++;
      access_cache(i);
    }
    cacheCheck += cache_size;
  }

  return cacheAssociativity;
}

int main(void) {
  int size;
  int assoc;
  int block_size;

  cache_init(0,0);

  block_size=get_block_size();
  size=get_cache_size(block_size);
  assoc=get_cache_assoc(size);

  printf("Cache block size: %d bytes\n", block_size);
  printf("Cache size: %d bytes\n", size);
  printf("Cache associativity: %d\n", assoc);

  return EXIT_SUCCESS;
}
