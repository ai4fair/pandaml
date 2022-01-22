#!/bin/sh

# Script to run whole analysis chain using Singularity container.
# The script is based on runideal.sh script with the only exeption 
# of running the PandaRoot from a special Singularity container.

if((0)); then
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
fi

#CONTAINER=$HOME/fair/stable/v12.0.3.sif     # FairSoft(nov20p1), FairRoot(18.6.3), PandaRoot(v12.0.3)
CONTAINER=$HOME/fair/stable/dev210810.sif    # FairSoft(nov20p1), FairRoot(18.6.3), PandaRoot(dev210810)


# Input Flags
nevt=100
prefix=evtcomplete
dec=llbar_fwp.dec
mom=1.642
seed=42


if test "$1" != ""; then
  nevt=$1
fi

if test "$2" != ""; then
  prefix=$2
fi

if test "$3" != ""; then
  dec=$3
fi


outprefix=$path"/"$prefix

# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------

echo -e "Events    : $nevt"
echo -e "Prefix    : $outprefix"
echo -e "Decay     : $dec"
echo -e "pBeam     : $mom"
echo -e "Seed      : $seed"

# Terminate Script for Testing.
#exit 0;

# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

echo ""
echo "Script has Started..."

echo "Started Simulation..."
#singularity exec $CONTAINER root -l -b -q sim_complete_vis.C\($nevt,\"$outprefix\",\"$dec\"\) > $outprefix"_sim.log" 2>&1

echo "Started Digitization..."
#singularity exec $CONTAINER root -l -b -q digi_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_digi.log" 2>&1

echo "Started CSV Generator..."
singularity exec $CONTAINER root -l -b -q data_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_data.log" 2>&1

echo "Script has Finished..."
echo ""
