#!/bin/bash

echo "build start date: `date`"
echo "hostname: `hostname`"
echo "operating system: `uname -s`"



DATA_PATH=../../crawler/src/testdir
INDEX_FILE=index.dat
NEW_FILE=new_index.dat

echo -e "\ncleaning up..."
make clean

echo -e "\nbuilding the indexer..."
make

echo -e "\nTesting argument checking..."
echo -e "\nIf two many arguments are supplied:"
./indexer $DATA_PATH $INDEX_FILE extra

echo -e "\nIf the data path is not a valid directory:"
./indexer ./baddir $INDEX_FILE

echo -e "\nIf the index file already exists:"
./indexer $DATA_PATH indexer.c

echo -e "\nIf testing is being executed, and the new file already exists:"
./indexer $DATA_PATH $INDEX_FILE $INDEX_FILE indexer.c

echo -e "\nIf testing is being executed, and the results file and the rewritten file are the same:"
./indexer $DATA_PATH $INDEX_FILE $INDEX_FILE $INDEX_FILE

echo -e "\nRunning indexer with 3 arguments..."
./indexer $DATA_PATH $INDEX_FILE

echo -e "\nThe sorted output file from indexer is:"
sort $INDEX_FILE

echo -e "\nCleaning up before reloading test..."
make clean

echo -e "\nRebuilding indexer to run the reloading test..."
make

echo -e "\nTesting the reloading capability of indexer..."
./indexer $DATA_PATH $INDEX_FILE $INDEX_FILE $NEW_FILE



echo -e "\nIndexes have been built, read and rewritten correctly!"
sort -o $INDEX_FILE $INDEX_FILE
sort -o $NEW_FILE $NEW_FILE
RESULT=$(diff -s $INDEX_FILE $NEW_FILE)

if [ $? -eq 0 ]; then
	echo "The test checked out!"
else
	echo "The test did not check out. Here are the differences in the files:"
	echo $RESULT

fi

echo build end date: `date`

