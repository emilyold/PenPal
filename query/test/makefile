CC = gcc
CFLAGS = -Wall -pedantic -std=c11

EXEC = query_test
SRCS = query_test.c ../src/operations.c ../src/operations.h
CFILES = query_test.c ../src/operations.c

UTILDIR=../../util/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)hashtable.c $(UTILDIR)file.c $(UTILDIR)web.c $(UTILDIR)indexer_utils.c
UTILH=$(UTILC:.c=.h)

$(EXEC): $(SRCS) $(UTILH) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(CFILES) -L$(UTILDIR) $(UTILFLAG)
	
$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make;

debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(CFILES) -L$(UTILDIR) $(UTILFLAG)

clean:
	cd $(UTILDIR); make clean;
	rm -f query_test
