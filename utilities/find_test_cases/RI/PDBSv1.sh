#!/bin/sh

target=$1
filename=$(basename "$target")

directory=$(dirname "$target")

queries_directory="$directory"/"$filename"'_queries'

find "$queries_directory" -type f | sort -V | for line in $(cat); do
  echo $line' '$target
done;
