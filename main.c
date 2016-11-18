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
  int8_t pageFrame[256];
}MemoryPage;

typedef struct {
  uint16_t* addresses;
  uint32_t size;
}MemoryAddresses;

struct tlbentry {
    uint8_t logical;
    uint8_t physical;
};

// TLB is kept track of as a circular array, with the oldest element being overwritten once
//the TLB is full.
struct tlbentry tlb[TLB_SIZE];

//number of inserts into TLB that have been completed. Use as tlbindex % TLB_SIZE for
//the index of the next TLB line to use.
int32_t tlbindex = 0;

// pagetable[logical_page] is the physical page number for logical page. Value is -1
// if that logical page isn't yet in the table.
int32_t pagetable[PAGES];

//simulation of RAM
int8_t main_memory[MEMORY_SIZE];

// Pointer to memory mapped secondary storage
int8_t *backing;

size_t count_lines(char* file_name){
  FILE* file_pointer;
  char* line = NULL;
  size_t len = 0;
  size_t line_count = 0;
  ssize_t read;

  file_pointer = fopen(file_name, "r");
  if(file_pointer == NULL){
    exit(-1);
  }

  while((read = getline(&line, &len, file_pointer)) != -1){
    line_count++;
  }

  fclose(file_pointer);
  free(line);

  return line_count;
}

MemoryAddresses* get_memory_addresses(){
  FILE* file_pointer;
  char* line = NULL;
  size_t len = 0;
  size_t count = count_lines("addresses.txt");
  ssize_t read;

  MemoryAddresses* output = calloc(1, sizeof(MemoryAddresses));
  output->size = count;
  output->addresses = calloc(count, sizeof(uint16_t));


  file_pointer = fopen("addresses.txt", "r");
  if(file_pointer == NULL){
    exit(-1);
  }

  count = 0;
  while((read = getline(&line, &len, file_pointer)) != -1){
    output->addresses[count] = (uint16_t) atoi(line);
    count++;
  }

  fclose(file_pointer);
  free(line);

  return output;
}


int main(int argc, const char *argv[])
{
  MemoryAddresses* addresses = get_memory_addresses();
  for(size_t i = 0; i < addresses->size; i++){
    printf("%d\n", addresses->addresses[i]);
  }

  return 0;
}
