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
	unsigned int frame;
};

struct pgtbl_entry_node* start;
struct pgtbl_entry_node* end;





/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict() {
    struct pgtbl_entry_node* temp = end;
	// update end 
	end = temp-> prev;
	end->next = NULL;
	// store target entry 
	int res = temp-> frame;
	// free temp 
	free(temp);
	// return target frame
	return res;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {
	struct pgtbl_entry_node* temp;
	struct pgtbl_entry_node* curr = start;
	int found = 0;
	// try to assign temp to hold the result
	if (start == NULL) {
		// No thing in the list and create a new item
		temp = (struct pgtbl_entry_node *) malloc(sizeof(struct pgtbl_entry_node));
		if (temp == NULL) {
			fprintf(stderr, "Memory allocation failed");
			exit(1);
		}
		temp -> frame = p->frame >> PAGE_SHIFT;
		temp -> next = NULL;
		temp -> prev = NULL;
		
		start = temp;
		end = temp;
		return;
	} else {
		while (curr != NULL) {
			if (curr->frame == (p->frame >> PAGE_SHIFT)) {
				found = 1;
				break;
			}
			curr = curr -> next;
		}
		if (found) {
			if (start == curr) {
				return;
			}
			if (curr == end) {
				// remove from 
				end = curr -> prev;
				end-> next = NULL;
				curr-> prev = NULL;
				
				curr-> next = start;
				start-> prev = curr; 
				start = curr;
				return;
			}
			// it is found in the middle 
			temp = curr;
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			curr->next = NULL;
			curr->prev = NULL;
			
			temp-> next = start;
			start-> prev = temp; 
			start = temp;
			return;
		} else {
			temp = (struct pgtbl_entry_node *) malloc(sizeof(struct pgtbl_entry_node));
			if (temp == NULL) {
				fprintf(stderr, "Memory allocation failed");
				exit(1);
			}
			temp -> frame = p->frame >> PAGE_SHIFT;
		    temp -> next = NULL;
		    temp -> prev = NULL;
			// add to the head;
			temp-> next = start;
			start-> prev = temp; 
			start = temp;
			return;
		}
	}
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
	start = NULL;
	end = NULL;
}
