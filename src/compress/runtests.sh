#!/bin/bash

testctbr1=true
testctbr2=true
testctbr3=true
testctbr4=true
testctbr5=true
testctbr6=true

testpack1=true
testpack2=true
testpack3=true
testpack4=true
testpack5=true
testpack6=true

testspace=false

if $testctbr1; then 
   ./a.out -ctbr test1.in > test1k.out
   ./Compress -ctbr test1.in > test1.out
   echo -e "TEST1\n"
   diff -bw test1k.out test1.out
fi

if $testctbr2; then 
   ./a.out -ctbr test2.in > test2k.out
   ./Compress -ctbr test2.in > test2.out
   echo -e "TEST2\n"
   diff -bw test2k.out test2.out
fi

if $testctbr3; then 
   ./a.out -ctbr test3.in > test3k.out
   ./Compress -ctbr test3.in > test3.out
   echo -e "TEST3\n"
   diff -bw test3k.out test3.out
fi

if $testctbr4; then 
   ./a.out -ctbr test4.in > test4k.out
   ./Compress -ctbr test4.in > test4.out
   echo -e "TEST4\n"
   diff -bw test4k.out test4.out
fi

if $testctbr5; then 
   ./a.out -ctbr test5.in > test5k.out
   ./Compress -ctbr test5.in > test5.out
   echo -e "TEST5\n"
   diff -bw test5k.out test5.out
fi

if $testctbr6; then 
   ./a.out -ctbr test6.in > test6k.out
   ./Compress -ctbr test6.in > test6.out
   echo -e "TEST6\n"
   diff -bw test6k.out test6.out
fi

if $testpack1; then 
   ./a.out test1.in
   ./Compress test1.in
   echo -e "TEST1\n"
   diff -bw test1.in.kZ test1.in.Z
fi

if $testpack2; then 
   ./a.out test2.in
   ./Compress test2.in
   echo -e "TEST2\n"
   diff -bw test2.in.kZ test2.in.Z
fi

if $testpack3; then 
   ./a.out test3.in
   ./Compress test3.in
   echo -e "TEST3\n"
   diff -bw test3.in.kZ test3.in.Z
fi

if $testpack4; then 
   ./a.out test4.in
   ./Compress test4.in
   echo -e "TEST4\n"
   diff -bw test4.in.kZ test4.in.Z
fi

if $testpack5; then 
   ./a.out test5.in
   ./Compress test5.in
   echo -e "TEST5\n"
   diff -bw test5.in.kZ test5.in.Z
fi

if $testpack6; then 
   ./a.out test6.in
   ./Compress test6.in
   echo -e "TEST6\n"
   diff -bw test6.in.kZ test6.in.Z
fi

if $testspace; then 
   ./a.out -s test5.in > test5k.out
   ./Compress -s test5.in > test5.out
   echo -e "TEST5\n"
   diff -bw test5k.out test5.out
fi
