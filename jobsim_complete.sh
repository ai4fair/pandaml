#!/bin/bash

LUSTRE_HOME="/lustre/panda/"$USER
nyx=$LUSTRE_HOME"/pandaml"

tmpdir="/tmp/"$USER
_target=$nyx"/data"


# Init PandaRoot
# . $LUSTRE_HOME"/CENTOS/v12.0.3-install/bin/config.sh" -p
. $LUSTRE_HOME"/CENTOS/dev-install/bin/config.sh" -p

echo -e "\n";


# Default Inputs
nevt=1000
prefix=mumu
gen=DBoxGEN                 # SBoxGEN, DBoxGEN or .DEC
pBeam=1.642
opt=""
seed=$RANDOM
run=$SLURM_ARRAY_TASK_ID

# User Inputs
if test "$1" != ""; then
  prefix=$1
fi

if test "$2" != ""; then
  nevt=$2
fi

if test "$3" != ""; then
  dec=$3
fi


# Make sure `$_target` Exists
if [ ! -d $_target ]; then
    mkdir -p $_target;
    echo -e "\nThe data dir. at '$_target' created."
else
    echo -e "\nThe data dir. at '$_target' exists."
fi


# IF ARRAY_TASK Used
if test "$run" == ""; then
    tmpdir="/tmp/"$USER
    outprefix=$tmpdir"/"$prefix
    seed=4200
else
    tmpdir="/tmp/"$USER"_"$SLURM_JOB_ID
    outprefix=$tmpdir"/"$prefix"_"$run
    seed=$SLURM_ARRAY_TASK_ID
fi


# Make sure $tempdir exists
if [ ! -d $tmpdir ]; then
    mkdir $tmpdir;
    echo -e "The temporary dir. at '$tmpdir' created."
else
    echo -e "The temporary dir. at '$tmpdir' exists."
fi


# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------

echo ""
echo -e "\nLustre Home  : $LUSTRE_HOME"
echo -e "Working Dir. : $nyx"
echo -e "Temp Dir.    : $tmpdir"
echo -e "Target Dir.  : $_target"
echo ""
echo -e "--Macro--"
echo -e "Events    : $nevt"
echo -e "Prefix    : $outprefix"
echo -e "Decay     : $dec"
echo -e "pBeam     : $mom"
echo -e "Seed      : $seed"
echo ""


# Terminate Script for Testing.
exit 0;


# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

echo ""
echo "Started Simulating..."
root -l -b -q $nyx"/"sim_complete.C\($nevt,\"$outprefix\",\"$gen\"\) > $outprefix"_sim.log" 2>&1

echo "Started Digitization..."
root -l -b -q $nyx"/"digi_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_digi.log" 2>&1

echo "Started Ideal Reconstruction..."
root -l -b -q $nyx"/"recoideal_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_reco.log" 2>&1

echo "Finished Simulating..."
echo ""


if [[ $opt == *"ana"* ]]; then
    
    echo "Started CSV Generator..."
    root -l -b -q $nyx"/"data_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_data.log" 2>&1
    
    mv $outprefix"_data.root" $_target
    mv $outprefix"_data.log" $_target
    echo "Finished CSV Generator..."
    
fi


#*** Storing Files ***
echo "Moving Files from '$tmpdir' to '$_target'"

mv $outprefix"_par.root" $_target
mv $outprefix"_sim.root" $_target
mv $outprefix"_sim.log" $_target
mv $outprefix"_digi.root" $_target
mv $outprefix"_digi.log" $_target
mv $outprefix"_reco.root" $_target
mv $outprefix"_reco.log" $_target


#*** Tidy Up ***
rm -rf $tmpdir

echo "The Script has Finished wit SLURM_JOB_ID: $SLURM_JOB_ID."
