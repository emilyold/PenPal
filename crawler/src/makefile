CC = gcc
CFLAGS = -Wall -pedantic -std=c11
LFLAGS = -lcurl
# my project details
EXEC = crawler
OBJS = crawler.o web.o hashtable.o list.o 
SRCS = crawler.c web.c hashtable.c list.c common.h utils.h list.h hashtable.h web.h


$(EXEC): $(OBJS) 
	$(CC) $(CFLAGS) $(LFLAGS) -o $(EXEC) $(OBJS) 

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(LFLAGS) -c $(SRCS)

debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -lcurl -o $(EXEC) $(OBJS)

clean:
	rm -f *.o 
