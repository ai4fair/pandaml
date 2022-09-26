#!/bin/sh

# Running Script with User Inputs
# ./run <nevt> <prefix> <gen> <pBeam>
# e.g.
# ./run 10 ftf ftf 7.1

# PandaRoot
. "/home/"$USER"/gsi/pandaroot/install-v13.0.0/bin/config.sh" -p


# Default Inputs
nevt=10
prefix=fwp
gen=llbar_fwp.dec         # .dec, SBoxGEN, DBoxGEN OR dpm, ftf, pythia8
pBeam=1.642               # llbar: 1.642, xibarxi1820: 4.6 GeV/c
seed=42


# User Inputs
if test "$1" != ""; then
  nevt=$1
fi

if test "$2" != ""; then
  prefix=$2
fi

if test "$3" != ""; then
  gen=$3
fi

if test "$4" != ""; then
  pBeam=$4
fi


# Make Sure $_target Exists
_target=data
if [ ! -d $_target ]; then
    mkdir -p $_target;
    echo "\nThe data dir. at '$_target' created."
else
    echo "\nThe data dir. at '$_target' exists."
fi


# Output Prefix
outprefix=$_target"/"$prefix

# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------

echo "\nEvents    : $nevt"
echo "Prefix    : $outprefix"
echo "Decay     : $gen"
echo "pBeam     : $pBeam"
echo "Seed      : $seed"

# Terminate Script for Testing.
# exit 0;

# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

echo ""
echo "Started Simulation..."
root -l -b -q sim_complete.C\($nevt,\"$outprefix\",\"$gen\",$pBeam,$seed\) > $outprefix"_sim.log" 2>&1

echo "Started Digitization..."
root -l -b -q digi_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_digi.log" 2>&1

echo "Started Ideal Reconstruction..."
root -l -b -q recoideal_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_reco.log" 2>&1

echo "Started CSV Generator..."
root -l -b -q data_complete.C\($nevt,\"$outprefix\",\"$_target\"\) > $outprefix"_data.log" 2>&1

echo "Finished Simulation..."
echo ""
