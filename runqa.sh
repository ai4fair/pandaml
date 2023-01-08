#!/bin/sh

# Running Script with User Inputs
# ./run <nevt> <prefix> <gen> <pBeam>
# e.g.
# ./run 10 ftf ftf 7.1

tmpdir="/tmp/"$USER
_target="data_sets/ctd2022/qa/event_test"
inputdir="data_sets/ctd2022/qa/root_cand"

# PandaRoot
. "/home/"$USER"/gsi/pandaroot/install-v13.0.0/bin/config.sh" -p


# Default Inputs
nevt=3
prefix=mumu
start=5000                 # Starting of CSV/ROOT Track Cands (TrackML)
flag="WithoutIdeal" # Options: WithIdeal, WithoutIdeal


# User Inputs
if test "$1" != ""; then
  nevt=$1
fi

if test "$2" != ""; then
  prefix=$2
fi

# Make Sure $_target Exists
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
echo "\nStorage Dir. : $tmpdir"
echo "Sim Dir.  : $_target"
echo "Trkx Dir.  : $inputdir"
echo "Events       : $nevt"
echo "Prefix       : $outprefix"

# Terminate Script for Testing.
# exit 0;

# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

# echo "\n...Started Data Generation..."
# root -l -b -q data_prod.C\($nevt,\"$outprefix\",\"$_target\",\"$flag\"\) > $outprefix"_data.log" 2>&1


echo "\n...Started Track Import..."
root -l -b -q import_prod.C\($nevt,\"$outprefix\",\"$inputdir\",$start\) # > $outprefix"_import.log" 2>&1

echo ""
