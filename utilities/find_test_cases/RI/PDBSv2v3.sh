#!/bin/sh

for d in ./datasets/RI/PDBSv2v3/*; do
  find "$d" -maxdepth 1 -type f | for f in $(cat); do
    name=$(basename "$f")
    find "$d"/queries -name 'query_*_'"$name" | for q in $(cat); do
      echo "$q"' '"$f"
    done
  done
done | sort -V
