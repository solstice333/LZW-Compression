#!/bin/bash

./a.out < test1.in > test1k.out
#./a.out < test2.in > test2k.out

echo -e "TEST1:\n"

diff -bw test1k.out test1.out
#diff -bw test2k.out test2.out
