#!/bin/sh

if [ $# -lt 1 ]; then
  echo -e "\nMacro File is Necessary.\n"
  echo -e "USAGE: ./root.sh <macro.C>"
  exit 1
fi

FILE=$1
CONTAINER=$HOME/fair/stable/v12.0.3.sif     # FairSoft(nov20p1), FairRoot(18.6.3), PandaRoot(v12.0.3)
CONTAINER=$HOME/fair/containers/dev220310    # FairSoft(nov20p1), FairRoot(18.6.3), PandaRoot(dev210810)

# singularity exec $CONTAINER root -l -b -q $FILE && exit
singularity run $CONTAINER -c "root -l $FILE" && exit

