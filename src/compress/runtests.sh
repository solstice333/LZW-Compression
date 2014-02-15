#!/bin/bash

# set to true if you want to test that category
testTcbr=true  # tests -tcbr flag options
testS=false    # tests -s flag option
testZ=true     # tests .Z compressed files

lastTest=8     
kout="k.out"   
one=0; two=0; three=0; four=0; five=0; six=0; seven=0; eight=0

# comment out the numbers you want to exclude from testing
one=1
two=2
three=3
four=4
five=5
six=6
seven=7
eight=8

if $testTcbr; then
   for i in `seq 1 $lastTest`; do
      if [ $i -eq $one -o $i -eq $two -o $i -eq $three -o $i -eq $four -o $i -eq $five -o $i -eq $six -o $i -eq $seven -o $i -eq $eight ]; then
         ./a.out -tcbr test$i.in > test$i$kout
         ./Compress -tcbr test$i.in > test$i.out
         echo -e "TEST$i -tcbr flags\n"
         diff test$i$kout test$i.out
      fi
   done
fi

if $testS; then
   for i in `seq 1 $lastTest`; do
      if [ $i -eq $one -o $i -eq $two -o $i -eq $three -o $i -eq $four -o $i -eq $five -o $i -eq $six -o $i -eq $seven -o $i -eq $eight ]; then
         ./a.out -s test$i.in > test$i$kout
         ./Compress -s test$i.in > test$i.out
         echo -e "TEST$i -s flags\n"
         diff test$i$kout test$i.out
      fi
   done
fi

if $testZ; then
   for i in `seq 1 $lastTest`; do
      if [ $i -eq $one -o $i -eq $two -o $i -eq $three -o $i -eq $four -o $i -eq $five -o $i -eq $six -o $i -eq $seven -o $i -eq $eight ]; then
         ./a.out test$i.in 
         ./Compress test$i.in
         echo -e "TEST$i .Z compressed files\n"
         diff test$i.in.K test$i.in.Z
      fi
   done
fi

