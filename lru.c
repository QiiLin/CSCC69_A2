#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;


// double linked struct 
struct pgtbl_entry_node {
	struct pgtbl_entry_node* prev;
	struct pgtbl_entry_node* next;
	pgtbl_entry_t* entry;
};  

struct pgtbl_entry_node* start;
struct pgtbl_entry_node* end;





/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict() {
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
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {
	int found = 0;
	struct pgtbl_entry_node* curr = start;
	while(curr != NULL) {
		if (curr-> entry == p) {
			found = 1;
			break;
		}
		curr = curr->next;
	}
	if (found) {
		// remove it from list, need to handle next handle start and end 
		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
		if (curr == start) {
			start = curr->next;
		}
		if (curr == end) {
			end = curr->prev;
		}
	}
	if ((p->frame)& PG_VALID) {
		struct pgtbl_entry_node* temp;
		if (found) {
			temp = curr;
		} else {
			temp = (struct pgtbl_entry_node *) malloc(sizeof(struct pgtbl_entry_node)); 
		}
		temp-> entry = p;
		if (start == NULL) {
			start = temp;
			end = temp;
		} else {
			start->prev = temp;
			temp->next = start;
			start = temp;
		}
	} else if (found) {
		free(curr);
	}
	return;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
}
