#ifndef _XML_STACK_H
#define _XML_STACK_H 1

#include "xml-parser.h"

typedef struct {
	void *data;
	struct Node *next;
} Node;


void xml_stack_free(Node *node);
void xml_stack_push(void *data);
void *xml_stack_pop();

#endif /* _XML_STACK_H */
