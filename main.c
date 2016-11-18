/*
Basics of program needed to complete assignment 7
*/

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TLB_SIZE 16
#define PAGES 256
#define PAGE_MASK 255

#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255

#define MEMORY_SIZE PAGES * PAGE_SIZE

// Max number of characters per line of virtual address file to be read (addresses.txt)
#define BUFFER_SIZE 10

typedef struct {
  uint8_t pageFrame[256];
}MemoryPage;


int main(int argc, const char *argv[])
{


  return 0;
}
