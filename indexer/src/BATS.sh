#!/bin/bash

echo build start date: `date`
echo hostname: `hostname`
echo operating system: `uname -s`



DATA_PATH=../../crawler/src/testdir
INDEX_FILE=index.dat
NEW_FILE=new_index.dat

make clean
make

rm *.dat

echo Testing argument checking...
echo If two many arguments are supplied:
./indexer $DATA_PATH $INDEX_FILE extra

echo If the data path is not a valid directory:
./indexer ./baddir $INDEX_FILE

echo If the index file already exists:
./indexer $DATA_PATH indexer.c

echo If testing is being executed, and the new file already exists:
./indexer $DATA_PATH $INDEX_FILE $INDEX_FILE indexer.c

echo If testing is being executed, and the results file and the rewritten file are the same:
 ./indexer $DATA_PATH $INDEX_FILE $INDEX_FILE $INDEX_FILE

 echo "If testing is being executed, index file is not the same as the file being read for testing:"
./indexer $DATA_PATH $INDEX_FILE not_the_same.file $NEW_FILE



echo "Testing the reloading capability of indexer..."
./indexer $DATA_PATH $INDEX_FILE $INDEX_FILE $NEW_FILE

echo "Indexes have been built, read and rewritten correctly!"
sort -o $INDEX_FILE $INDEX_FILE
sort -o $NEW_FILE $NEW_FILE
result=$(diff -s $INDEX_FILE $NEW_FILE)

if [ $? -eq 0 ]; then
	echo "The test checked out!"
else
	echo "The test did not check out"
fi

echo build end date: `date`

