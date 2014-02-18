#!/bin/bash

# set to true if you want to test that category
testTcbrsZ=true  # tests -tcbrs flag options and compressed output
testCrazy=true # tests some crazy flag options that no one would ever do

# won't work on 64 bit machines
testWithStaleyCompress=true   # tests with Staley's Compress.o

lastTest=9     
kout="k.out"   
one=0; two=0; three=0; four=0; five=0; six=0; seven=0; eight=0; nine=0

# comment out the numbers you want to exclude from testing
one=1
two=2    # large tcbr output (exclude this when running on vogon)
three=3
four=4
five=5
six=6    # large tcbr output (exclude this when running on vogon)
seven=7
eight=8  # large tcbr output (exclude this when running on vogon)
nine=9

if $testTcbrsZ; then
   for i in `seq 1 $lastTest`; do
      if [ $i -eq $one -o $i -eq $two -o $i -eq $three -o $i -eq $four -o $i -eq $five -o $i -eq $six -o $i -eq $seven -o $i -eq $eight -o $i -eq $nine ]; then
         echo -e "\nTEST$i -tcbrs flags and testing compressed output\n"
         ./a.out -tcbrs test$i.in > test$i$kout
         mv test$i.in.Z test$i.in.K
         ./Compress -tcbrs test$i.in > test$i.out
         diff test$i$kout test$i.out
         diff test$i.in.K test$i.in.Z

         ./a.out -s test$i.in > test$i$kout
         ./Compress -s test$i.in > test$i.out
      fi
   done
fi

echo -e "\n\nChecking if tests are the same as original..."

echo -e "Checking Compress..."
if [ -e Compress ]; then
   diff Compress originalCompressTests/Compress
fi

echo -e "Checking test#.in.Z..."
for i in `seq 1 4`; do
   if [ -e test$i.in.Z ]; then
      diff test$i.in.Z originalCompressTests/test$i.in.Z
   fi
done

echo -e "Checking test#.in..."
for i in `seq 1 4`; do
   if [ -e test$i.in ]; then
      diff test$i.in originalCompressTests/test$i.in
   fi
done

echo -e "Checking test#.out..."
for i in `seq 1 4`; do
   if [ -e test$i.out ]; then
      diff test$i.out originalCompressTests/test$i.out
   fi
done

echo -e "Done!"

if $testCrazy; then
   i=1; j=2
   echo -e "\nTEST$i and TEST$j duplicates, two follow up dashes, multiple filenames"
   echo -e "Output file in test$i$kout test$i.out\n"
   ./a.out -srr -bss test$i.in test$j.in > test$i$kout
   mv test$i.in.Z test$i.in.K
   mv test$j.in.Z test$j.in.K
   ./Compress -srr -bss test$i.in test$j.in > test$i.out
   diff test$i$kout test$i.out
   diff test$i.in.K test$i.in.Z
   diff test$j.in.K test$j.in.Z
fi

if $testCrazy; then
   i=3; j=4
   echo -e "\nTEST$i and TEST$j flag in between filenames"
   echo -e "Output file in test$i$kout test$i.out\n"
   ./a.out -s test$i.in -b test$j.in> test$i$kout
   mv test$i.in.Z test$i.in.K
   mv test$j.in.Z test$j.in.K
   ./Compress -s test$i.in -b test$j.in > test$i.out
   diff test$i$kout test$i.out
   diff test$i.in.K test$i.in.Z
   diff test$j.in.K test$j.in.Z
fi

if $testCrazy; then
   i=5
   echo -e "\nTEST$i crazy flag options\n"
   ./a.out -srrr -rssd -bb test$i.in > test$i$kout
   mv test$i.in.Z test$i.in.K
   ./Compress -srrr -rssd -bb test$i.in > test$i.out
   diff test$i$kout test$i.out
   diff test$i.in.K test$i.in.Z
fi

if $testWithStaleyCompress; then
   ./makeWithStaleyCompress.sh
   for i in `seq 1 $lastTest`; do
      if [ $i -eq $one -o $i -eq $two -o $i -eq $three -o $i -eq $four -o $i -eq $five -o $i -eq $six -o $i -eq $seven -o $i -eq $eight -o $i -eq $nine ]; then
         echo -e "\nTEST$i testing with Staley's Compress.o\n"
         ./a.out -tcbrs test$i.in > test$i$kout
         mv test$i.in.Z test$i.in.K
         ./Compress -tcbrs test$i.in > test$i.out
         diff test$i$kout test$i.out
         diff test$i.in.K test$i.in.Z
      fi
   done
fi
