#!/bin/bash

# set to true if you want to test that category
testTcbr=true  # tests -tcbr flag options
testS=true     # tests -s flag option
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

echo -e "Checking if tests are the same as original..."

echo -e "Checking Compress..."
if [ -e Compress ]; then
   diff Compress originalCompressTests/Compress
fi

echo -e "Checking test#.in.Z..."
if [ -e test1.in.Z ]; then
   diff test1.in.Z originalCompressTests/test1.in.Z
fi

if [ -e test2.in.Z ]; then
   diff test2.in.Z originalCompressTests/test2.in.Z
fi

if [ -e test3.in.Z ]; then
   diff test3.in.Z originalCompressTests/test3.in.Z
fi

if [ -e test4.in.Z ]; then
   diff test4.in.Z originalCompressTests/test4.in.Z
fi

echo -e "Checking test#.in..."
if [ -e test1.in ]; then
   diff test1.in originalCompressTests/test1.in
fi

if [ -e test2.in ]; then
   diff test2.in originalCompressTests/test2.in
fi

if [ -e test3.in ]; then
   diff test3.in originalCompressTests/test3.in
fi

if [ -e test4.in ]; then
   diff test4.in originalCompressTests/test4.in
fi

echo -e "Checking test#.out..."
if $testS -a [ -e test1.out ]; then
   diff test1.out originalCompressTests/test1.out
fi

if $testS -a [ -e test2.out ]; then
   diff test2.out originalCompressTests/test2.out
fi

if $testS -a [ -e test3.out ]; then
   diff test3.out originalCompressTests/test3.out
fi

if $testS -a [ -e test4.out ]; then
   diff test4.out originalCompressTests/test4.out
fi

echo -e "Done!"

