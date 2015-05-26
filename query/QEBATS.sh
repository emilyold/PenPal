#!/bin/bash

# Automated testing script for the Tiny Search Engine

echo "build start date: `date`" > QEBATS.log
echo "hostname: `hostname`" >> QEBATS.log
echo "operating system: `uname -s`" >> QEBATS.log

cd ../crawler/src 

echo -e "Building crawler...\n" >> ../../query/QEBATS.log
make clean >> ../../query/QEBATS.log
make >> ../../query/QEBATS.log
mkdir testdir >> ../../query/QEBATS.log

echo -e "\nCrawling http://old-www.cs.dartmouth.edu/~cs50/tse/ at depth 3" >> ../../query/QEBATS.log
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./testdir 3 >> ../../query/QEBATS.log

echo -e "\nCleaning up the crawler directory...\n" >> ../../query/QEBATS.log
make clean >> ../../query/QEBATS.log
 
cd ../../indexer/src 

echo -e "\nBuilding indexer...\n" >> ../../query/QEBATS.log
make >> ../../QEBATS.log

echo -e "Using Zhao Tian's indexer files for depth 3" >> ../../query/QEBATS.log

echo -e "Cleaning up indexer directory...\n" >> ../../query/QEBATS.log

cd ../../query 

echo -e "Building the query engine...\n" >> QEBATS.log
make >> QEBATS.log

cd test 
echo -e "Building the query engine test script...\n" >> ../QEBATS.log
make >> QEBATS.log

echo -e "\nUnit testing the query engine...\n" >> ../QEBATS.log
query_test >> ../QEBATS.log

echo -e "\nCleaning up the testing...\n" >> ../QEBATS.log
make clean >> ../QEBATS.log

if [ $? == 0 ]; then
	
	cd ..
	echo -e "\nStarting the query engine using Zhao Tian's level 3 index.dat file\n" >> QEBATS.log
	
	printf "dartmouth college\ndartmouth AND college\ndartmouth OR college\nsdlkgjslk\ndartmouth AND college OR computer AND science\n" | query ~tianzhao/tse/indexer/lvl3/index.dat ../crawler/src/testdir >> QEBATS.log
	
	echo -e "\n\nCleaning up query directory...\n" >> QEBATS.log
	make clean >> QEBATS.log
	cd ../crawler/src
	rm -r testdir

fi
