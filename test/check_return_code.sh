#!/bin/sh
../pcp-machine $1
return=$?
if [ $return -ne $2 ]; then
  echo "Wanted return code $2 but got $return"
  return 1
else
  return 0
fi