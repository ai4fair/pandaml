#!/bin/sh

# Running Script with User Inputs
# ./run <nevt> <prefix> <gen> <pBeam>
# e.g.
# ./run 10 ftf ftf 7.1

tmpdir="/tmp/"$USER
_target="/shared/adeel/data_sets/ctd2022/run_100k/qa"

# PandaRoot
. "/home/"$USER"/gsi/pandaroot/install-v13.0.0/bin/config.sh" -p


# Default Inputs
nevt=5000
prefix=mumu_110
start=0  # Starting of CSV/ROOT Track Cands (TrackML)
flag="WithIdeal"  # Options: WithIdeal, WithoutIdeal


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
echo "Storage Dir. : $tmpdir"
echo "Sim Dir.     : $_target"
echo "Trkx Dir.    : $_target"
echo "Events       : $nevt"
echo "Prefix       : $outprefix\n"

# Terminate Script for Testing.
# exit 0;

# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

# echo "\nStarted Data Generation..."
# root -l -b -q data_prod.C\($nevt,\"$outprefix\",\"$_target\",\"$flag\"\) > $outprefix"_data.log" 2>&1

echo "Creating TracMLCand to PndTrackCands"
root -l -b -q import_complete.C\($nevt,\"$outprefix\",\"$_target\",$start\) # > $outprefix"_import.log" 2>&1

echo "Running Tracking QA"
root -l -b -q qa_complete.C\($nevt,\"$outprefix\"\) # > $outprefix"_qa.log" 2>&1













