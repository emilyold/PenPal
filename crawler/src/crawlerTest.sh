# check what happens if more than 3 arguments are given
moreThan3=`./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./testdir 0 extra`
echo $moreThan3

#check what happens if less than 3 arguments are given
lessThan3=`./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/`
echo $lessThan3

# test what happens if an invalid url is given
invalidURL=`./crawler www.badurl.com ./testdir 2`
echo $invalidURL

# test what happens if an invalid directory is given
invalidDirectory=`./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./baddir 2`
echo $invalidDirectory

# test what happens if an out of range depth is given
outOfRange=`./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./testdir 10`
echo $outOfRange

# test what happens if an invalid depth is given
invalidDepth=`./crawler http:/old-www.cs.dartmouth.edu/~cs50/tse/ ./testdir 2hello`
echo $invalidDepth