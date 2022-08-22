#!/bin/sh

# Lustre
LUSTRE_HOME="/lustre/panda/"$USER
nyx=$LUSTRE_HOME"/pandaml"
tmpdir="/tmp/"$USER
_target=$nyx"/data"


# PandaRoot
. $LUSTRE_HOME"/DEBIAN/v13.0.0-install/bin/config.sh" -p

echo "\n";


# Default Inputs
nevt=1000
prefix=xibar_xi1820
gen=Xibar_Xi1820.dec             # SBoxGEN, DBoxGEN or .DEC
pBeam=4.6                        # llbar: 1.642, xibarxi1820: 4.6 GeV/c
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


# Make Sure $_target Exists
if [ ! -d $_target ]; then
    mkdir -p $_target;
    echo "\nThe data dir. at '$_target' created."
else
    echo "\nThe data dir. at '$_target' exists."
fi


# Make Sure $tempdir Exists
if [ ! -d $tmpdir ]; then
    mkdir $tmpdir;
    echo "The temporary dir. at '$tmpdir' created."
else
    echo "The temporary dir. at '$tmpdir' exists."
fi


# Output Prefix
outprefix=$_target"/"$prefix


# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------

echo "\nLustre Home  : $LUSTRE_HOME"
echo "Working Dir. : $nyx"
echo "Temp Dir.    : $tmpdir"
echo "Target Dir.  : $_target"

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
echo "\nScript has Started..."

echo "Started Simulation..."
root -l -b -q $nyx"/"sim_complete.C\($nevt,\"$outprefix\",\"$gen\",$pBeam,$seed\) > $outprefix"_sim.log" 2>&1

echo "Started Digitization..."
root -l -b -q $nyx"/"digi_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_digi.log" 2>&1

echo "Started Ideal Reconstruction..."
root -l -b -q $nyx"/"recoideal_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_reco.log" 2>&1

echo "Started CSV Generator..."
# root -l -b -q $nyx"/"data_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_data.log" 2>&1

echo "Script has Finished...\n"
echo ""
