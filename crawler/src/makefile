CC = gcc
CFLAGS = -Wall -pedantic -std=c11
LFLAGS = -lcurl
# my project details
EXEC = crawler
OBJS = crawler2.o web.o hashtable.o list.o 
SRCS = crawler2.c web.c hashtable.c list.c common.h utils.h list.h hashtable.h web.h


$(EXEC): $(OBJS) $(UTILLIB)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(EXEC) $(OBJS) $(UTILFLAG)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(LFLAGS) -c $(SRCS)

debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) $(UTILFLAG)

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make

clean:
	rm -f *~
	rm -f *#
	rm -f ./myApp
	rm -f *.o
	cd $(UTILDIR); make clean 