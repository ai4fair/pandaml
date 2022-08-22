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
run=0

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


# Make sure `$_target` Exists
if [ ! -d $_target ]; then
    mkdir -p $_target;
    echo "\nThe data dir. at '$_target' created."
else
    echo "\nThe data dir. at '$_target' exists."
fi


# Make sure $tempdir exists
if [ ! -d $tmpdir ]; then
    mkdir -p $tmpdir;
    echo "The temporary dir. at '$tmpdir' created."
else
    echo "The temporary dir. at '$tmpdir' exists."
fi

# Output Prefix
outprefix=$tmpdir"/"$prefix

#*** Print Flags ***
echo "\nLustre Home  : $LUSTRE_HOME"
echo "Working Dir. : $nyx"
echo "Temp Dir.    : $tmpdir"
echo "Target Dir.  : $_target"
echo "\nEvents    : $nevt"
echo "Prefix    : $outprefix"
echo "Decay     : $gen"
echo "pBeam     : $pBeam"
echo "Seed      : $seed"
echo "Run       : $run"


# Terminate Script for Testing.
# exit 0;


#*** Initiate Simulaton ***
echo "\nStarted Simulating..."
root -l -b -q $nyx"/"sim_complete.C\($nevt,\"$outprefix\",\"$gen\",$pBeam,$seed\) > $outprefix"_sim.log" 2>&1

echo "Started Digitization..."
root -l -b -q $nyx"/"digi_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_digi.log" 2>&1

echo "Started Ideal Reconstruction..."
root -l -b -q $nyx"/"recoideal_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_reco.log" 2>&1

echo "Started CSV Generator..."
root -l -b -q $nyx"/"data_complete.C\($nevt,\"$outprefix\",\"$tmpdir\",$run\) > $outprefix"_data.log" 2>&1

echo "Finished Simulating...\n"


#*** Storing Files ***
echo "Moving Files from '$tmpdir' to '$_target'"

cp $outprefix"_par.root" $_target
cp $outprefix"_sim.root" $_target
cp $outprefix"_sim.log" $_target
cp $outprefix"_digi.root" $_target
cp $outprefix"_digi.log" $_target
cp $outprefix"_reco.root" $_target
cp $outprefix"_reco.log" $_target
cp $outprefix"_data.root" $_target
cp $outprefix"_data.log" $_target

cp $tmpdir"/"*.csv $_target


#*** Tidy Up ***
rm -rf $tmpdir

echo "The Script has Finished wit SLURM_JOB_ID: $run."
