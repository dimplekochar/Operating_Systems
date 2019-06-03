#include "alloc.h"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std; 
/* Code to allocate page of 4KB size with mmap() call and
* initialization of other necessary data structures.
* return 0 on success and 1 on failure (e.g if mmap() fails)
*/
char *pagem;
int i=1, maxi=PAGESIZE/MINALLOC;
memm entire;
memm blocks[512]; //max number of blocks

int init()
{
	// Write your code below
	pagem = (char*)mmap (0, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (pagem == MAP_FAILED) 
        return 1;
    entire.isfree=1;
    entire.size=PAGESIZE;
    entire.start=pagem;
    entire.end=pagem+PAGESIZE;
    //cout<<pagem<<endl;
    //cout<<entire.isfree<<" "<<entire.size<<" "<<entire.start<<" "<<entire.end;
    for (int j=0; j<maxi; j++)
	blocks[j].isfree=0;
    blocks[0]=entire;

    return 0;
  
}

/* optional cleanup with munmap() call
* return 0 on success and 1 on failure (if munmap() fails)
*/
int cleanup()
{

	// Write your code below
	int unmap_result = munmap(pagem, PAGESIZE);
  	if (unmap_result == -1) 
    	return 1;
	return 0;
  

  
}

/* Function to allocate memory of given size
* argument: bufSize - size of the buffer
* return value: on success - returns pointer to starting address of allocated memory
*               on failure (not able to allocate) - returns NULL
*/
char *alloc(int bufSize)
{
	// write your code below
	if (bufSize%8!=0)
		return 0;

	for (int k=0; k<maxi; k++){
		if (blocks[k].isfree==1 && blocks[k].size>bufSize){
			blocks[k].isfree=0;
			blocks[i].isfree=1;
   			blocks[i].size=blocks[k].size-bufSize;
    		blocks[k].size=bufSize;
    		blocks[i].end= blocks[k].end;
    		blocks[k].end=blocks[k].start+bufSize;
    		blocks[i].start=blocks[k].end+1;
    		i++;
    		return blocks[k].start;
		}
		else if (blocks[k].isfree==1 && blocks[k].size==bufSize){
			blocks[k].isfree=0;
    		return blocks[k].start;
		}
	}
	return 0;
}


/* Function to free the memory
* argument: takes the starting address of an allocated buffer
*/
void dealloc(char *memAddr)
{
	// write your code below
	for (int k=0; k<maxi; k++){
		if (blocks[k].start==memAddr){
			blocks[k].isfree=1;
		}
	}
}
