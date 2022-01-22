#!/bin/sh

# Script to run whole analysis chain using Singularity container.
# The script is based on runideal.sh script with the only exeption 
# of running the PandaRoot from a special Singularity container.

if((1)); then
  rm *.root
  rm *.log
  rm *.pdf
  rm core*
  rm *.dat
fi

# For CSV a directory named `data` is needed
path=data
if [[ ! -e $path ]]; then
    mkdir $path
elif [[ ! -d $path ]]; then
    echo "$path already exists but is not a directory" 1>&2

#CONTAINER=$HOME/fair/stable/v12.0.3.sif     # FairSoft(nov20p1), FairRoot(18.6.3), PandaRoot(v12.0.3)
CONTAINER=$HOME/fair/stable/dev210810.sif    # FairSoft(nov20p1), FairRoot(18.6.3), PandaRoot(dev210810)


# Input Flags
nevt=100
prefix=evtcomplete
inputGen=llbar_fwp.DEC
inputGen=box:type(13,10):p(1.0,3.0):tht(22,140):phi(0,360)
UseDoubleBox=kFALSE
mom=1.642
seed=42


if test "$1" != ""; then
  nevt=$1
fi

if test "$2" != ""; then
  prefix=$2
fi


echo ""
echo "Script has Started..."

echo "Started Simulation..."
singularity exec $CONTAINER root -l -b -q sim_complete.C\($nevt,\"$prefix\",\"$inputGen\",$UseDoubleBox\) > sim.log 2>&1

echo "Started Digitization..."
singularity exec $CONTAINER root -l -b -q digi_complete.C\($nevt,\"$prefix\"\) > digi.log 2>&1

echo "Started CSV Generator..."
singularity exec $CONTAINER root -l -b -q data_complete.C\($nevt,\"$prefix\"\) > data.log 2>&1

echo "Script has Finished..."
echo ""