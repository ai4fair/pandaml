#!/bin/sh

if [ $# -lt 1 ]; then
  echo -e "\nMacro File is Necessary.\n"
  echo -e "USAGE: ./root.sh <macro.C>"
  exit 1
fi

FILE=$1
CONTAINER=~/gsi/containers/v13.0.0.sif
# singularity exec $CONTAINER root -l -b -q $FILE && exit
singularity run $CONTAINER -c "root -l -b -q $FILE" && exit

