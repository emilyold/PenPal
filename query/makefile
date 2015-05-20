CC = gcc
CFLAGS = -Wall -pedantic -std=c11
INDEXDIR = ../indexer/src/

EXEC = query
SRCS = ./src/query.c $(INDEXDIR)hashtable.c $(INDEXDIR)file.c $(INDEXDIR)web.c $(INDEXDIR)indexer_utils.c $(INDEXDIR)hashtable.h $(INDEXDIR)file.h $(INDEXDIR)web.h $(INDEXDIR)indexer_utils.h
OBJS = query.o hashtable.o file.o web.o indexer_utils.o
CFILES=./src/query.c

UTILDIR=../util/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(INDEXDIR)hashtable.c $(INDEXDIR)file.c $(INDEXDIR)web.c
UTILH=$(UTILC:.c=.h)

$(EXEC): $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS)

clean:
	rm -f *.o
	rm -f *.h.gch
	rm -f query