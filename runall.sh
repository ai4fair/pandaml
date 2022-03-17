#!/bin/sh

# Lustre Home
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
gen=DBoxGEN             # SBoxGEN, DBoxGEN or .DEC
pBeam=1.642
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
    echo -e "\nThe data dir. at '$_target' created."
else
    echo -e "\nThe data dir. at '$_target' exists."
fi


# Make Sure $tempdir Exists
if [ ! -d $tmpdir ]; then
    mkdir $tmpdir;
    echo -e "The temporary dir. at '$tmpdir' created."
else
    echo -e "The temporary dir. at '$tmpdir' exists."
fi


# outprefix
outprefix=$_target"/"$prefix


# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------

echo ""
echo -e "\nLustre Home  : $LUSTRE_HOME"
echo -e "Working Dir. : $nyx"
echo -e "Temp Dir.    : $tmpdir"
echo -e "Target Dir.  : $_target"
echo ""
echo -e "Events    : $nevt"
echo -e "Prefix    : $outprefix"
echo -e "Decay     : $gen"
echo -e "pBeam     : $pBeam"
echo -e "Seed      : $seed"
echo ""


# Terminate Script for Testing.
# exit 0;


# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

echo ""
echo -e "\nScript has Started..."

echo "Started Simulation..."
root -l -b -q $nyx"/"sim_complete.C\($nevt,\"$outprefix\",\"$gen\"\) > $outprefix"_sim.log" 2>&1

echo "Started Digitization..."
root -l -b -q $nyx"/"digi_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_digi.log" 2>&1

echo "Started Ideal Reconstruction..."
root -l -b -q $nyx"/"recoideal_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_reco.log" 2>&1

echo "Started CSV Generator..."
root -l -b -q $nyx"/"data_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_data.log" 2>&1

echo "Script has Finished..."
echo ""
