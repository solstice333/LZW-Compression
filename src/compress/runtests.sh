#!/bin/bash

# set to true if you want to test that category
testTcbr=true
testS=false
testZ=true
kout="k.out"

one=0; two=0; three=0; four=0; five=0; six=0

# comment out the numbers you want to include in testing
#one=1
#two=2
#three=3
#four=4
#five=5
#six=6

if $testTcbr; then
   for i in `seq 1 6`; do
      if [ $i -ne $one -a $i -ne $two -a $i -ne $three -a $i -ne $four -a $i -ne $five -a $i -ne $six ]; then
         ./a.out -tcbr test$i.in > test$i$kout
         ./Compress -tcbr test$i.in > test$i.out
         echo -e "TEST$i\n"
         diff test$i$kout test$i.out
      fi
   done
fi

if $testS; then
   for i in `seq 1 6`; do
      if [ $i -ne $one -a $i -ne $two -a $i -ne $three -a $i -ne $four -a $i -ne $five -a $i -ne $six ]; then
         ./a.out -s test$i.in > test$i$kout
         ./Compress -s test$i.in > test$i.out
         echo -e "TEST$i\n"
         diff test$i$kout test$i.out
      fi
   done
fi

if $testZ; then
   for i in `seq 1 6`; do
      if [ $i -ne $one -a $i -ne $two -a $i -ne $three -a $i -ne $four -a $i -ne $five -a $i -ne $six ]; then
         ./a.out test$i.in 
         ./Compress test$i.in
         echo -e "TEST$i\n"
         diff test$i.in.K test$i.in.Z
      fi
   done
fi

