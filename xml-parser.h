#ifndef _XML_PARSER_H
#define _XML_PARSER_H 1

#include "xml-parser.h"
#include "xml-stack.h"

#define XML_BUFFER_SIZE 2048

typedef enum {
	XML_NONE,
	XML_DTD,
	XML_INST
} XMLState;

typedef struct {
	char name[XML_BUFFER_SIZE];
	char value[XML_BUFFER_SIZE];
} XMLNode;

typedef void (*XML_Start_Handler) (char *xml_element);
typedef void (*XML_End_Handler)   (char *xml_element);

XMLNode *xml_create_node(char *xml_element_name, char *xml_tag_value);
void xml_free_node(XMLNode *node);
void xml_parse_chunk(char *chunk);
void xml_set_handlers(XML_Start_Handler start, XML_End_Handler end);
void xml_parse_attribs(char *chunk);
char *xml_parse_inst(char *chunk);
XMLNode *xml_get_value();


#endif /* _XML_PARSER_H */

