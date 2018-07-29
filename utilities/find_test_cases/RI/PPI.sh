#!/bin/sh

target=$1
filename=$(basename "$target")

directory=$(dirname "$target")

for query in 'q0.25' 'q0.5' 'q-1'; do
  find "$directory"/"$query" -name "$filename"'*' | sort -V
done | for line in $(cat); do
  echo $line' '$target
done;
