#!/bin/bash

testTcbr=true
testS=false
testZ=true
exclude26=false

kout=k.out

two=0
six=0
if $exclude26; then
   two=2
   six=6
fi

if $testTcbr; then
   for i in `seq 1 6`; do
      if [ $i -ne $two -a $i -ne $six ]; then
         ./a.out -tcbr test$i.in > test$i$kout
         ./Compress -tcbr test$i.in > test$i.out
         echo -e "TEST$i\n"
         diff -bw test$i$kout test$i.out
      fi
   done
fi

if $testS; then
   for i in `seq 1 6`; do
      if [ $i -ne $two -a $i -ne $six ]; then
         ./a.out -s test$i.in > test$i$kout
         ./Compress -s test$i.in > test$i.out
         echo -e "TEST$i\n"
         diff -bw test$i$kout test$i.out
      fi
   done
fi

if $testZ; then
   for i in `seq 1 6`; do
      if [ $i -ne $two -a $i -ne $six ]; then
         ./a.out test$i.in 
         ./Compress test$i.in
         echo -e "TEST$i\n"
         diff -bw test$i.in.K test$i.in.Z
      fi
   done
fi

