#!/bin/sh
OUTPUT="`../pcp-machine $1`"
WANTED="$2"

if [ "$OUTPUT" != "$WANTED" ]; then
  echo "Wanted '$WANTED' but got '$OUTPUT'"
  return 1
else
  return 0
fi