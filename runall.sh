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

# PandaRoot (Virgo)
# . "/lustre/panda/CENTOS/dev-install/bin/config.sh" -p

# Input Flags
nevt=100
prefix=evtcomplete

# gen=BGEN                    # Single Box Gen
gen=DBGEN                     # Double Box Gen
# gen=llbar_bkg.DEC           # EvtGen (DEC)
pBeam=1.642
seed=42


if test "$1" != ""; then
  nevt=$1
fi

if test "$2" != ""; then
  prefix=$2
fi

if test "$3" != ""; then
  gen=$3
fi

# outprefix
outprefix=$path"/"$prefix

# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------

echo -e "Events    : $nevt"
echo -e "Prefix    : $outprefix"
echo -e "Decay     : $gen"
echo -e "pBeam     : $pBeam"
echo -e "Seed      : $seed"

# Terminate Script for Testing.
# exit 0;

# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

echo ""
echo "Script has Started..."

echo "Started Simulation..."
root -l -b -q sim_complete.C\($nevt,\"$outprefix\",\"$gen\"\) > $outprefix"_sim.log" 2>&1

echo "Started Digitization..."
root -l -b -q digi_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_digi.log" 2>&1

echo "Started Ideal Reconstruction..."
root -l -b -q recoideal_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_reco.log" 2>&1

echo "Started CSV Generator..."
root -l -b -q data_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_data.log" 2>&1

echo "Script has Finished..."
echo ""
