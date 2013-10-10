CC = gcc 

INCLUDES = 
LIBS = 
FLAGS = -lm

OBJS = xml-parser.o xml-test.o xml-stack.o

.c.o:
	$(CC) $(INCLUDES) $(LIBS) -c $<


xml-test: $(OBJS)
	$(CC) -o xml-test $(OBJS) $(INCLUDES) $(LIBS) $(FLAGS)

xml-stack.o: xml-stack.c

xml-parser.o: xml-parser.c

xml-test.o: xml-test.c

clean:
	rm -f *~ *.exe *.o xml-test

