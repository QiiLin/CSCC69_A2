#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap; // store all the pysical frame and their status and page 


// double linked struct 
struct pgtbl_entry_node {
	struct pgtbl_entry_node* prev;
	struct pgtbl_entry_node* next;
	pgtbl_entry_t* entry;
};  

struct pgtbl_entry_node* start;
struct pgtbl_entry_node* end;



// idea: store a queue of valid page in queue and 
// store the start pointer, end pointer
// Note the pagetable are fixed size numberof pagetable * size of pagetable entry
// Thus we can keep track all of it in this fix array





/* Page to evict is chosen using the fifo algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int fifo_evict() {
	struct pgtbl_entry_node* temp = end;
	// if there is not active node here 
	if (end == NULL) {
		return -1;
	}
	// update end 
	end = temp->prev;
	// store target entry 
	pgtbl_entry_t* temp_entry = temp->entry;
	// free temp 
	free(temp);
	// return target frame
	return (temp_entry->frame) >> PAGE_SHIFT;
}

/* This function is called on each access to a page to update any information
 * needed by the fifo algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void fifo_ref(pgtbl_entry_t *p) {
	int found = 0;
	struct pgtbl_entry_node* curr = start;
	while(curr != NULL) {
		if (curr-> entry == p) {
			found = 1;
			break;
		}
		curr = curr->next;
	}
	if (!found && (p->frame)& PG_VALID) {
		struct pgtbl_entry_node* temp = (struct pgtbl_entry_node *) malloc(sizeof(struct pgtbl_entry_node)); 
		temp-> entry = p;
		if (start == NULL) {
			start = temp;
			end = temp;
		} else {
			start->prev = temp;
			temp->next = start;
			start = temp;
		}
	}
	return;
}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void fifo_init() {
}
