CC=gcc
CFLAGS= -I./include -L. -Wall
LIBS= -lsqlite3 -lcomdlg32
SOURCES= $(wildcard src/*.c)

all: extractor

extractor: $(SOURCES)
	$(CC) $(SOURCES) -o extractor $(CFLAGS) $(LIBS)
	
clean: 
	rm extractor