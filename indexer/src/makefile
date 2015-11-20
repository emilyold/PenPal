CC = gcc
CFLAGS = -Wall -pedantic -std=c11


EXEC = indexer
SRCS = indexer.c 
CFILES=./indexer.c

UTILDIR=../../util/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=../../util/hashtable.c ../../util/file.c ../../util/web.c ../../util/indexer_utils.c
UTILH=$(UTILC:.c=.h)

$(EXEC): $(SRCS) $(UTILH) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(CFILES) -L$(UTILDIR) $(UTILFLAG)

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make;

clean:
	rm -f *.o
	rm -f *.h.gch
	rm -f *.dat
	rm -f indexer