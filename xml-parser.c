#include <stdio.h>
#include <string.h>

#include "xml-parser.h"
#include "xml-stack.h"

XML_Start_Handler xml_start_handler;
XML_End_Handler   xml_end_handler;

XMLState XMLCurrentState = XML_INST;

extern xml_stack_top; /* xml-stack.h */


/**
 * malloc's a new node that stores the value
 *
 * arguments: char *xml_tag_value - the element name 
 * returns: XMLNode
 */
XMLNode *xml_create_node(char *xml_element_name, char *xml_tag_value) {
	XMLNode *node = NULL;
	node = (XMLNode *)malloc(sizeof(XMLNode));
	
	if (node == NULL) {
		printf("ERROR: Could not allocate memory for value!\n");
		return(NULL);
	} else {
		strcpy(node->name,  xml_element_name);
		strcpy(node->value, xml_tag_value);
		return(node);
	}
}

/**
 * destroy the XMLNode
 * 
 * arguments: XMLNode *node - the node to free
 */
void xml_free_node(XMLNode *node) {
	free(node);
}


/**
 * This basically gets the XML entity name
 *
 * arguments: char *xml_tag_buffer
 */
static char *xml_get_element(char *xml_tag_buffer) {
	char xml_element_buffer[XML_BUFFER_SIZE];
	char *p = NULL;
	char *q = NULL;
	char *r = NULL;
	
	p = &xml_tag_buffer[0];
	q = strchr(xml_tag_buffer, ' ');

	if (xml_tag_buffer[0] == '/') {
		xml_tag_buffer++;
	}
	
	/* find the beginning */
	if (q) {
		strncpy(xml_element_buffer, p, q - p);
		xml_element_buffer[q - p] = (char)NULL;
		r = xml_element_buffer;
		return (r);
	} else {
		return (xml_tag_buffer);
	}
	
}

/**
 * This is the main engine that parses each chunk from
 * whatever.  This function also calls the DTD parser,
 * version parser, and values parser.
 *
 * arguments: char *chunk - the string to parse
 */

void xml_parse_chunk(char *chunk) {
	XMLNode *node;
	XMLNode *stack_node_value;
	Node *stack;

	char xml_tag_buffer[XML_BUFFER_SIZE];
	char xml_value_buffer[XML_BUFFER_SIZE];

	int i = 0;
	int len = strlen(chunk);
	
	char *p = NULL; 
	char *q = NULL;
	char *r = NULL;
	
	/* xml_parse_dtd */
	if (XMLCurrentState == XML_INST) {
		chunk = xml_parse_inst(chunk);
		if (XMLCurrentState == XML_INST) {
			return;
		}
	}

	
	/* xml_parse_version */
	
	for (i = 0; i < len; i++) {
		if (chunk[i] == '<') {
			q = strchr(&chunk[i], '>');
			p = &chunk[i + 1];

			if (p) {
				strncpy(xml_tag_buffer, p, q - p);
				xml_tag_buffer[q - p] = (char)NULL;

				if (xml_tag_buffer[0] == '/') {
					#if DEBUG
						printf("XMLP END: %s\n", xml_get_element(xml_tag_buffer));
					#endif
					if (xml_end_handler) {
						xml_end_handler(xml_get_element(xml_tag_buffer));
					}
				} else {
					#if DEBUG
						printf("XMLP START: %s\n", xml_get_element(xml_tag_buffer));					
					#endif
				
					r = strchr(q + 1, '<');
					if (r) {
						strncpy(xml_value_buffer, q + 1, r - q -1);
						xml_value_buffer[r - q] = (char)NULL;
						
						node = xml_create_node(xml_get_element(xml_tag_buffer), xml_value_buffer);
						xml_stack_push(node);
					} else {
						stack_node_value = NULL;
					}

					/* return element and stack */
					if (xml_start_handler) {
						xml_start_handler(xml_get_element(xml_tag_buffer));
					}	
					
					xml_parse_attribs(xml_tag_buffer);
					
				}

			}	
					
		}
	}
	
	
}

/**
 * Pop off the value from the stack.
 *
 * returns: pointer to the node 
 */
XMLNode *xml_get_value() {
	XMLNode *node = NULL;
	XMLNode *temp = NULL;
	char *p = NULL;
	
	/* fixme, need to free() */
	node = (XMLNode *)xml_stack_pop();

	if (node) {
		temp = xml_create_node(node->name, node->value);
		free(node);
		if (temp) {
			return(temp);
		} else {
			return(NULL);
		}
	} else {
		return (NULL);
	}
}

/**
 * Setup the start and end function pointers.  These two functions
 * are executed every time a start or end element is processed.
 */
void xml_set_handlers(XML_Start_Handler start, XML_End_Handler end) {
	xml_start_handler = start;
	xml_end_handler   = end;
}

/**
 * This parses a string chunk for values until it reaches end of
 * chunk
 *
 * value = "foo" word="bar"
 */
void xml_parse_attribs(char *chunk) {
	XMLNode *node;
	char element[XML_BUFFER_SIZE];
	char value[XML_BUFFER_SIZE];
		
	char *p = NULL;
	char *q = NULL;
	char *s = NULL;
	int i = 0;
	int len = strlen(chunk);

	p = strchr(chunk, ' ');
	
	if (p) {
		for (i = 0; i < len; i++) {
			if (chunk[i] == ' ') {
				p = &chunk[i];
			}
			
			if (chunk[i] == '=') {
				q = &chunk[i];
				
				if (p) {
					strncpy(element, p, q - p);
					element[q - p] = (char)NULL;
					
					p = strchr(q, '"');
					
					if (p) {
						/* until we find " and " - 1 != \ */	
						q = strchr(p + 1, '"');
						if (q) {
							strncpy(value, p + 1, q - p);
							value[q - p - 1] = (char)NULL;
							if (element) {
								node = xml_create_node(element, value);
								xml_stack_push(node);
							}
						} else {
							return;
						}
					} else {
						return;
					}

					p = NULL;
				} else {
					return;
				}
			}
		}
	}
	
}

/**
 * This function is supposed to process process instructions.
 * Right now i'm just going to process the instruction and
 * throw it out.  -fixme-
 * 
 * arguments: char *chunk - the chunk to parse
 * returns: pointer to the end of process instruction
 */
char *xml_parse_inst(char *chunk) {
	char *p = NULL;
	char *q = NULL;
	char *r = NULL;
	
	p = strchr(chunk, '<');
	
	if (p) {
		if (chunk[p - chunk + 1] == '?') {
			XMLCurrentState = XML_INST;
			r = strchr(chunk, '\0');
		}
	} else {
		XMLCurrentState = XML_NONE;
	}
	
	q = strchr(chunk, '>');

	if (q) {
		if (chunk[q - chunk - 1] == '?') {
			XMLCurrentState = XML_NONE;
			r = strchr(chunk, '\0');
		}
	} else {
		/* just shove this into a string or whatever */
	}
	
	if (!r) {
		XMLCurrentState = XML_NONE;
		r = chunk;
	}
	
	return (r);
	
}
