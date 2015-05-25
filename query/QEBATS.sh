#!/bin/bash

echo "build start date: `date`"
echo "hostname: `hostname`"
echo "operating system: `uname -s`"


cd ../crawler/src

echo -e "Building crawler...\n"
make clean
make
mkdir testdir

echo -e "\nCrawling http://old-www.cs.dartmouth.edu/~cs50/tse/ at depth 3"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./testdir 3

echo -e "\nCleaning up the crawler directory...\n"
make clean

cd ../../indexer/src

echo -e "\nBuilding indexer...\n"
make

echo -e "Using Zhao Tian's indexer files for depth 3"

echo -e "Cleaning up indexer directory...\n"

cd ../../query

echo -e "Building the query engine...\n"
make

cd test
echo -e "Building the query engine test script...\n"
make

echo -e "\nUnit testing the query engine...\n"
query_test

if [ $? == 0 ]; then
	make clean
	cd ..
	echo -e "\nStarting the query engine using Zhao Tian's level 3 index.dat file\n"
	query ~tianzhao/tse/indexer/lvl3/index.dat ../crawler/src/testdir
	echo -e "\nCleaning up query directory...\n"
	make clean
	cd ../crawler/src
	rm -r testdir
else
	make clean
	cd ../crawler/src
	rm -r testdir
	echo -e "\nQuery testing failed.\n"
fi






