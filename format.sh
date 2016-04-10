#!/bin/sh

BASE=`pwd`
#for dir in src src/bif src/miniz src/platf include include/bif include/struct test; do
for F in `find . -name \*.h -print -o -name \*.cpp -print -name \*.c -print -name \*.inl -print`; do
    echo "===> $F"
    clang-format -i $F
done