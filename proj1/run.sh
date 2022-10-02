#!/bin/bash

for file in test/*gee.txt
do
  echo "Running gee.py for $file"
  python3 gee.py $file
done