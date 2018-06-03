#!/bin/sh

find "$1" -regex '.+\.A[0-9][0-9]' | sort | awk '{g = $1; gsub("A","B",g); print $1 " " g}'
