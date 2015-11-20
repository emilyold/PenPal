CC=gcc
CFLAGS= -Wall -pedantic -std=c11

LIB=libtseutil.a
OBJS=hashtable.o web.o file.o indexer_utils.o
SRCS=hashtable.c web.c file.c indexer_utils.c hashtable.h web.h file.h indexer_utils.h


$(LIB): $(OBJS)
	ar -cq libtseutil.a *.o

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f *.h.gch
	rm -f *.o