#!/bin/bash

cd test/text/
for filename in *.txt; do
  file="${filename%%.*}"
  echo "Running $file"
  cd ../..
  python3 gee.py test/text/$file.gee.txt > test/out/$file.out
  diff -b test/out/$file.out test/out/$file-out.txt
  rm test/out/$file.out
  cd test/text/
done