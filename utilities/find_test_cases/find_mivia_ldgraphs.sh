#!/bin/sh

find "$1" -regex '.+.sub.grf' | sort | awk '{g = $1; gsub(".sub","",g); gsub("query/","target/",g); print $1 " " g}'
