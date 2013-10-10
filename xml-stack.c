/**
 * This stack simply stores the values and pops them
 * out during the start calls.
 */
#include <stdio.h>
#include "xml-stack.h"
#include <string.h>

Node *xml_stack_top;

/**
 * Allocates a node for the stack.
 *
 * Arguments: void *data - pointer to some other data
 * returns:   NULL if allocation error, or the Node
 */
static Node *stack_allocate(void *data) {
	Node *node;

	node = (Node *)malloc(sizeof(Node));

	if (node == NULL) {
		printf("ERROR: Unable to allocate memory for the stack!\n");
		return(NULL);
	}

	node->data = data;
	return (node);
}

/**
 * Calls node allocation and puts it in the stack.  
 *
 * Arguments: void *data - pointer to the data 
 */
void xml_stack_push(void *data) {
	Node *node;
	node = (Node *)stack_allocate(data);
	(Node *)node->next = xml_stack_top;
	xml_stack_top = node;
}

/**
 * Free node from the tree
 * 
 * Arguments: Node *node - node to free
 */
void xml_stack_free(Node *node) {
	free(node->data);
	free(node);
}

/**
 * Pop the node off the stack
 *
 * What we should do is just return a void * to the data.
 * and then free the node.
 * 
 * Returns: XMLNode
 */
/* void *xml_stack_pop() */
void *xml_stack_pop() {
	
	Node *node;
	void *data = NULL;

	if (xml_stack_top == NULL) {
		return(NULL);
	}

	node = xml_stack_top;
	data = node->data;
	xml_stack_top  = (Node *)node->next;
	
	return(data);
	
}
