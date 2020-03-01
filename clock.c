#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

int clock_index;


/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int clock_evict() {
	// find the clock_index of evict canadiate 
	while(coremap[clock_index].pte->frame & PG_REF) {
		coremap[clock_index].pte->frame = coremap[clock_index].pte->frame &~ PG_REF;
		clock_index ++;
		if (clock_index >= memsize) {
			clock_index = clock_index % memsize;
		}
 	}
	int res = clock_index;
	// pass handler to next clock_index
	clock_index ++;
	if (clock_index >= memsize) {
		clock_index = clock_index % memsize;
	}
	return res;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {
	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init() {
	clock_index = 0;
}
