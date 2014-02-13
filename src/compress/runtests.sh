#!/bin/bash

teststdout1=false
teststdout2=false
teststdout3=false
teststdout4=false
teststdout5=false
teststdout6=false

testpack1=false
testpack2=false
testpack3=false
testpack4=false
testpack5=false
testpack6=false

testspace=true

if $teststdout1; then 
   ./a.out -ct test1.in > test1k.out
   ./Compress -ct test1.in > test1.out
   echo -e "TEST1\n"
   diff -bw test1k.out test1.out
fi

if $teststdout2; then 
   ./a.out -ct test2.in > test2k.out
   ./Compress -ct test2.in > test2.out
   echo -e "TEST2\n"
   diff -bw test2k.out test2.out
fi

if $teststdout3; then 
   ./a.out -ct test3.in > test3k.out
   ./Compress -ct test3.in > test3.out
   echo -e "TEST3\n"
   diff -bw test3k.out test3.out
fi

if $teststdout4; then 
   ./a.out -ct test4.in > test4k.out
   ./Compress -ct test4.in > test4.out
   echo -e "TEST4\n"
   diff -bw test4k.out test4.out
fi

if $teststdout5; then 
   ./a.out -ct test5.in > test5k.out
   ./Compress -ct test5.in > test5.out
   echo -e "TEST5\n"
   diff -bw test5k.out test5.out
fi

if $teststdout6; then 
   ./a.out -ct test6.in > test6k.out
   ./Compress -ct test6.in > test6.out
   echo -e "TEST6\n"
   diff -bw test6k.out test6.out
fi

if $testpack1; then 
   ./a.out test1.in 
   ./Compress test1.in
   echo -e "TEST1\n"
   diff -bw test1.in.k.Z test1.in.Z
fi

if $testpack2; then 
   ./a.out test2.in 
   ./Compress test2.in
   echo -e "TEST2\n"
   diff -bw test2.in.k.Z test2.in.Z
fi

if $testpack3; then 
   ./a.out test3.in 
   ./Compress test3.in
   echo -e "TEST3\n"
   diff -bw test3.in.k.Z test3.in.Z
fi

if $testpack4; then 
   ./a.out test4.in 
   ./Compress test4.in
   echo -e "TEST4\n"
   diff -bw test4.in.k.Z test4.in.Z
fi

if $testpack5; then 
   ./a.out test5.in 
   ./Compress test5.in
   echo -e "TEST5\n"
   diff -bw test5.in.k.Z test5.in.Z
fi

if $testpack6; then 
   ./a.out test6.in 
   ./Compress test6.in
   echo -e "TEST6\n"
   diff -bw test6.in.k.Z test6.in.Z
fi

if $testspace; then 
   ./a.out -s test5.in > test5k.out
   ./Compress -s test5.in > test5.out
   echo -e "TEST5\n"
   diff -bw test5k.out test5.out
fi
