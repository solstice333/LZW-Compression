#!/bin/bash

./a.out -c test1.in > test1k.out
#./a.out -c test2.in > test2k.out
./a.out -c test3.in > test3k.out
./a.out -c test4.in > test4k.out
./a.out -c test5.in > test5k.out

./Compress -c test1.in > test1.out
#./Compress -c test2.in > test2.out
./Compress -c test3.in > test3.out
./Compress -c test4.in > test4.out
./Compress -c test5.in > test5.out

echo -e "TEST1\n"
diff -bw test1k.out test1.out
#echo -e "TEST2\n"
#diff -bw test2k.out test2.out
echo -e "TEST3\n"
diff -bw test3k.out test3.out
echo -e "TEST4\n"
diff -bw test4k.out test4.out
echo -e "TEST5\n"
diff -bw test5k.out test5.out
