#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap; // store all the pysical frame and their status and page 


// init the start index of the frame
int fifo_start;

/* Page to evict is chosen using the fifo algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int fifo_evict() {
	// since the frame is being used one by one,
	// when it is full, we will always evict the 
	// fifo_start index which start at 0.  
    int result = fifo_start;
	// after we evict the fifo_start,
	// we will need to increase fifo_start by one.
	// this due to fact that frame is being used up one by one
	// thus, when it is fill again, the next (oldest) frame is just the next 
    // index of fifo_start. 	
    fifo_start = (fifo_start + 1) % memsize;
    return result;
}

/* This function is called on each access to a page to update any information
 * needed by the fifo algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void fifo_ref(pgtbl_entry_t *p) {
	// since the frame is being used one by one, 
	// there is not need to adjust any data in this case
	return;
}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void fifo_init() {
	fifo_start = 0;
}
