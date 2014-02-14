#!/bin/bash

testTcbr=true
testS=false
testZ=true

if $testTcbr; then
   for i in `seq 1 6`; do
      if [ $i -ne 2 -a $i -ne 6 ]; then
         ./a.out -tcbr test$i.in > testk.out
         ./Compress -tcbr test$i.in > test$i.out
         echo -e "TEST$i\n"
         diff -bw testk.out test$i.out
      fi
   done
fi

if $testS; then
   for i in `seq 1 6`; do
      if [ $i -ne 2 -a $i -ne 6 ]; then
         ./a.out -s test$i.in > testk.out
         ./Compress -s test$i.in > test$i.out
         echo -e "TEST$i\n"
         diff -bw testk.out test$i.out
      fi
   done
fi

if $testZ; then
   for i in `seq 1 6`; do
      if [ $i -ne 2 -a $i -ne 6 ]; then
         ./a.out test$i.in 
         ./Compress test$i.in
         echo -e "TEST$i\n"
         diff -bw test$i.in.K test$i.in.Z
      fi
   done
fi

