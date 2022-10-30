#!/bin/bash

cd test/text/
for filename in *.txt; do
  file="${filename%%.*}"
  echo "Running $file"
  cd ../..
  python3 gee.py test/text/$file.txt > test/out/$file.out
  rm test/out/$file.out
  cd test/text/
done