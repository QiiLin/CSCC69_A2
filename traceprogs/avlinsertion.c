#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Define an AVL tree
struct node {
	int key;
	int height;
	struct node *left;
	struct node *right;
}

// Get the height of the node
int height(struct node *N) {
	if (N == NULL) {
		return 0;
	} else {
		return N->height;
	}
}

// Comparsion function
int compare(int first, int second) {
	if (first > second) {
		return first;
	} else {
		return second;
	}
}

// Allocate a new node
struct node* newNode(int key) {
	struct node* node = (struct node*)malloc(sizeof(struct Node));
	node -> key = key;
	node -> left = NULL;
	node -> right = NULL;
	node -> height = 1;
	return node;
}

// Right rotation
struct node *rightRotate(struct node *t) {
	struct node *lc = t -> left;
	struct node *rc = lc -> right;
	lc->right = t;
	t->left = rc;
	// update the height
	t -> height = compare(height(t->left), height(t->right))+1;
	lc -> height = compare(height(lc->left), height(lc->right))+1;
	// return new root
	return lc;
}

// Left rotation
struct node *leftRotate(struct node *x) {
	struct node *y = x -> right;
	struct node *T2 = y -> left;
	y->left = x; 
    x->right = T2; 
    //  Update heights 
    x->height = max(height(x->left), height(x->right))+1; 
    y->height = max(height(y->left), height(y->right))+1; 
    // Return new root 
    return y; 
}

// Get Balance factor of node N 
int getBalance(struct node *N) 
{ 
    if (N == NULL) 
        return 0; 
    return height(N->left) - height(N->right); 
} 

int main(int argc, char ** argv) {
	
	return 0;
}
