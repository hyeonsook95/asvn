CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lm
SOURCES=clientemp.c handles.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ftp

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o ftp
