#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

#define PAGESIZE 4096 //size of memory to allocate from OS
#define MINALLOC 8 //allocations will be 8 bytes or multiples of it

// You can declare any data structures required here
struct memm{
	//struct mem *next; //next memory block address
    bool isfree; //if block is free or not
    size_t size; //size of block
    char *start; //starting address of memory block
    char *end;

};

// function declarations
int init();
int cleanup();
char *alloc(int);
void dealloc(char *);
