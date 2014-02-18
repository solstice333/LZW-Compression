#LZWCmp#

##To run##
To verify tests src/compress,
1. `$ make all`
2. `$ ./runtests.sh`

To compress stuff,
1. `./a.out [-options] [file1, file2,...]

Options include,
1. -t - dump contents of bst 
2. -c - print code to stdout right before it's sent to sink 
3. -b - announce increase in bits
4. -r - announce recycle
5. -s - announce space
