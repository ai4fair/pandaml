#!/bin/bash

# *** Cluster USAGE ***
# sbatch [options] -- jobsim_complete.sh <prefix> <events> <dec> <pbeam> [opt] [mode]

# *** Local USAGE ***
# ./jobsim_complete.sh <prefix> <events> <dec>
# ./jobsim_complete.sh llbar 100 llbar_fwp.DEC


if [ $# -lt 3 ]; then
  echo -e "\nMinimum Three Arguments Are Required\n"
  echo -e "USAGE: sbatch -a<min>-<max> -- jobsim_complete.sh <prefix> <nEvents> <dec>\n"
  echo -e " <min>     : Minimum job number"
  echo -e " <max>     : Maximum job number"
  echo -e " <prefix>  : Prefix of output files"
  echo -e " <nevts>   : Number of events to be simulated"
  echo -e " <dec>     : Decay File or Keywords: fwp (signal), bkg (non-resonant bkg), dpm (generic bkg)"
  echo -e " <pbeam>   : Momentum of pbar-beam (GeV/c)."
  echo -e " [opt]     : Optional options: if contains 'savesim', 'saveall' or 'ana'\n";
  echo -e "Example 1 : sbatch -a1-20 [options] jobsim_complete.sh fwp 1000 llbar_fwp.DEC"
  echo -e "Example 2 : ./jobsim_complete.sh fwp 100 llbar_fwp.DEC\n"
  exit 1
fi


# Lustre Storage
# LUSTRE_HOME=/lustre/$(id -g -n)/$USER
LUSTRE_HOME="/lustre/panda/"$USER


# Working Directory
nyx=$LUSTRE_HOME"/hpc"


# Data Storage
tmpdir="/tmp/"$USER
_target=$nyx"/data"


# Init PandaRoot
#. $LUSTRE_HOME"/pandaroot/build-dev/config.sh"
#. $LUSTRE_HOME"/pandaroot/install-dev/bin/config.sh" -p
. $LUSTRE_HOME"/CENTOS/dev-install/bin/config.sh" -p

echo -e "\n";


# Defaults
prefix=llbar                # output file naming
nevt=1000                   # number of events
dec="llbar_fwp.DEC"         # decay file OR keywords [fwp, bkg, dpm]
mom=1.642                   # pbarp with 1.642 GeV/c
mode=0                      # mode for analysis
opt="ana"                   # use opt to do specific tasks e.g. ana for analysis etc.
seed=$RANDOM                # random seed for simulation
run=$SLURM_ARRAY_TASK_ID    # Slurm Array ID
TargetMode=0                # Ask for point-like (0) or extended (4) target during simulation.


# User Input
if test "$1" != ""; then
  prefix=$1
fi

if test "$2" != ""; then
  nevt=$2
fi

if test "$3" != ""; then
  dec=$3
fi

if test "$4" != ""; then
  mom=$4
fi

if test "$5" != ""; then
  opt=$5
fi

if test "$6" != ""; then
  mode=$6
fi


# Get .DEC if Only Keywords [fwp,bkg,dpm] Are Given.
# e.g. ./jobsim_complete.sh llbar 100 fwp 1.642

if [[ $dec == "fwp" ]]; then
    _target=$nyx"/data/"$dec
    dec="llbar_fwp.DEC"
fi

if [[ $dec == "bkg" ]]; then
    _target=$nyx"/data/"$dec
    dec="llbar_bkg.DEC"
fi 

if [[ $dec == "dpm" ]]; then
    _target=$nyx"/data/"$dec
    dec="dpm"
fi


# Deduce Signal/Bkg from .DEC & Create Storage Accordingly.
# e.g. ./jobsim_complete.sh llbar 100 llbar_fwp.DEC 1.642

if [[ $dec == *"fwp"* ]]; then
    IsSignal=true
    #_target=$nyx"/data/fwp"
fi

if [[ $dec == *"bkg"* ]]; then
    IsSignal=false
    #_target=$nyx"/data/bkg"
fi

if [[ $dec == *"dpm"* ]]; then
    IsSignal=false
    #_target=$nyx"/data/dpm"
fi

# Prepend Absolute Path to .DEC File
if [[ $dec == *".dec"* ]]; then
  if [[ $dec != \/* ]] ; then
    dec=$nyx"/"$dec
  fi
fi

if [[ $dec == *".DEC"* ]]; then
  if [[ $dec != \/* ]] ; then
    dec=$nyx"/"$dec
  fi
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
    pidfile=$outprefix"_pid.root"
else
    tmpdir="/tmp/"$USER"_"$SLURM_JOB_ID
    outprefix=$tmpdir"/"$prefix"_"$run
    seed=$SLURM_ARRAY_TASK_ID
    pidfile=$outprefix"_pid.root"
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
echo "Lustre Home  : $LUSTRE_HOME"
echo "Working Dir. : $nyx"
echo "Temp Dir.    : $tmpdir"
echo "Target Dir.  : $_target"
echo ""
echo -e "--Macro--"
echo -e "Events    : $nevt"
echo -e "Prefix    : $outprefix"
echo -e "Decay     : $dec"
echo -e "pBeam     : $mom"
echo -e "Seed      : $seed"
echo -e "IsSignal  : $IsSignal"
echo -e "TargetMode: $TargetMode"
echo -e "PID File  : $pidfile"


# Terminate Script for Testing.
# exit 0;


# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

echo ""
echo "Started Simulating..."
root -l -b -q $nyx"/"prod_sim.C\($nevt,\"$outprefix\",\"$dec\",$mom,$seed,$TargetMode\) > $outprefix"_sim.log" 2>&1

echo "Started Digitization..."
root -l -b -q $nyx"/"prod_digi.C\($nevt,\"$outprefix\"\) > $outprefix"_digi.log" 2>&1 

echo "Started Ideal Reconstruction..."
root -l -b -q $nyx"/"prod_rec.C\($nevt,\"$outprefix\"\) > $outprefix"_reco.log" 2>&1

echo "Started Ideal PID..."
root -l -b -q $nyx"/"prod_pid.C\($nevt,\"$outprefix\"\) > $outprefix"_pid.log" 2>&1 

echo "Finished Simulating..."
echo ""

# ---------------------------------------------------------------
#                            Initiate Analysis
# ---------------------------------------------------------------

if [[ $opt == *"ana"* ]]; then
    
    echo "Starting Analysis..."
    #root -l -q -b $nyx"/"prod_ana_multi.C\(0,\"$pidfile\",$IsSignal,0,0,$mode\) > $outprefix"_ana.log" 2>&1
    #root -l -b -q $nyx"/"ana_ntp.C\($nevt,\"$outprefix\"\) > $outprefix"_ana_ntp.log" 2>&1
    root -l -b -q $nyx"/"prod_ana.C\($nevt,\"$outprefix\",$IsSignal\) > $outprefix"_ana.log" 2>&1

    mv $outprefix"_ana.root" $_target
    mv $outprefix"_ana.log" $_target
    echo "Finishing Analysis..."
    
fi

# ---------------------------------------------------------------
#                            Storing Files
# ---------------------------------------------------------------

NUMEV=`grep 'Generated Events' $outprefix"_sim.log"`
echo $NUMEV >> $outprefix"_pid.log"

# ls in tmpdir to appear in slurmlog
# ls -ltrh $tmpdir

echo "Moving Files from '$tmpdir' to '$_target'"

# move root files to target dir
mv $outprefix"_par.root" $_target
mv $outprefix"_sim.root" $_target
mv $outprefix"_sim.log" $_target
mv $outprefix"_digi.root" $_target
mv $outprefix"_digi.log" $_target
mv $outprefix"_reco.root" $_target
mv $outprefix"_reco.log" $_target
mv $outprefix"_pid.root" $_target
mv $outprefix"_pid.log" $_target


#*** Tidy Up ***
rm -rf $tmpdir

echo "The Script has Finished wit SLURM_JOB_ID: $SLURM_JOB_ID."
