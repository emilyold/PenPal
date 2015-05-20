CC=gcc
CFLAGS= -Wall -pedantic -std=c11

LIB=libtseutil.a
OBJS=hashtable.o web.o file.o
SRCS=../indexer/src/hashtable.c ../indexer/src/web.c ../indexer/src/file.c ../indexer/src/hashtable.h ../indexer/src/web.h ../indexer/src/file.h


$(LIB): $(OBJS)
	ar -cq libtseutil.a 

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)