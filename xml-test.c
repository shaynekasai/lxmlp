#include <stdio.h>
#include <stdlib.h>
#include "xml-parser.h"

void start(char *element) {

	XMLNode *attrib;
	
	printf("<%s>\n", element);
	
	while ((attrib = xml_get_value()) != NULL) {
		printf("attrib name: %s = %s\n", attrib->name, attrib->value);
	}
	
	 
}

void end(char *element) {
  printf("</%s>\n", element);
}

/**
 * Do this when processing process instruction
 */
void process() {

}

int main(int argc, char *argv[]) {
	FILE *fp;
	char buffer[XML_BUFFER_SIZE];

        xml_set_handlers(start, end);

	fp = fopen(argv[1], "r");
	
	if (!fp) {
		printf("ERROR: Can't open file for reading\n");
		exit(-1);
	}

	while(fgets(buffer, XML_BUFFER_SIZE, fp) != NULL) {
		xml_parse_chunk(buffer);
	}

	
	
	fclose(fp);
	return(0);
}
