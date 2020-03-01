#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"

// include for parameters of trace file 
#include "sim.h"


extern int debug;

extern struct frame *coremap;


// store data from trace file 
struct trace_node {
	addr_t addrs;
	struct trace_node* next;
};


struct trace_node* start;
struct trace_node* curr;
 
int* dist_holder; 

int numberOfLine = 0;

/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int opt_evict() {
	int i;
	int max_distance  = 0;
	int frame_index = 0;
	for (i = 0; i < memsize; i++) {
		if (dist_holder[i] > max_distance) {
			max_distance = dist_holder[i];
			frame_index = i;
		}
	}
	return frame_index;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {
	// read the file and create list of it 
	struct trace_node* item = curr;
	curr = curr-> next;
	free(item);
	int frame_index = p->frame >> PAGE_SHIFT;
	struct trace_node* temp = curr;
	int distance = 0;
	while(temp != NULL) {
		if (temp -> addrs == coremap[frame_index].addrs) {
			break;
		}
		temp = temp-> next;
		distance ++; 
	}
	
	dist_holder[frame_index] = (temp == NULL) ? numberOfLine : distance;
	return;
}

/* Initializes any data structures needed for this
 * replacement algorithm.
 */
void opt_init() {
	dist_holder = (int *) malloc(sizeof(int) * memsize);
	int i = 0;
	// init distance holder to all zero
	for ( i = 0; i < memsize; i++ ){
		dist_holder[i] = 0;
	}
	
	// init start and curr to NULL
	start = NULL;
	curr = NULL;
	
	// start reading from trace file 
	FILE *trace_file = fopen(tracefile, "r");
	if (trace_file == NULL) {
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	// copy from replay_trace in sim.c and modified for my own usage
	char buf[MAXLINE];
	addr_t vaddr = 0;
	char type;

	while(fgets(buf, MAXLINE, trace_file) != NULL) {
		if(buf[0] != '=') {
			sscanf(buf, "%c %lx", &type, &vaddr);
			// create node for each record in trace_file
			struct trace_node* temp = (struct trace_node*) malloc(sizeof(struct trace_node));
			if (temp == NULL) {
				fprintf(stderr, "Memory allocation failed");
				exit(1);
			}
			temp->addrs = vaddr >> PAGE_SHIFT;
			temp->next = NULL;
			
			// is it init case 
			if (start == NULL) {
				start = temp;
				curr = temp;
			} else {
				// it is not the init case, append it to the end
				curr->next = temp;
				curr = temp;
			}
			numberOfLine ++;
		}
	}
	// after we finish getting the data from trace file
	// we reset the curr to the start fo the list 
	curr = start;
}

