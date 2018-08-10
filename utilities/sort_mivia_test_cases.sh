#!/bin/sh

sed 's/_m\([0-9]\)/_a\1/' "$1" | sort -V > "$1.tmp"
sed 's/_a\([0-9]\)/_m\1/' "$1.tmp" > "$1"
rm "$1.tmp"
