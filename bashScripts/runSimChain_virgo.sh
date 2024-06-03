#!/bin/sh

# ---------------------------------------------------------------
#                       Description
# ---------------------------------------------------------------
# This script sequentially runs multiple parts of the PandaRoot simulation chain.
#
# The following steps/macros are currently included:
# sim_complete.C		Event generation & propagation in the dectactor simulation
# digi_complete.C		Creation of realistic detector responses to the particle propagations
# skew_complete.C		Correction of the longitudinal parameters of the hits in the skewed straw tubes (currently not used)
# recoideal_complete.C 	Ideal reconstruction of tracks using the MC truth
# data_complete.C		Transfer of hit and track information into CSV files for the ML pipeline
#
# Input parameters for the script:
# $1: Number of events to simulate
# $2: Prefix/Name for the output files
# $3: MC generator to use (SBoxGEN, DBoxGEN [default], ftf, pythia8, <file>.dec)
# $4: Beam momentum in GeV (default: 1.642 GeV)
# $5: Random seed for the simulation (default: 42)

# ---------------------------------------------------------------
#                       User & Default Inputs
# ---------------------------------------------------------------

# Number of events
nevt=1 # default
if test "$1" != ""; then
  nevt=$1
fi

# Prefix for output files
# prefix="test" # default
# if test "$2" != ""; then
#   prefix=$2
# fi

prefix=${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}

# MC generator: SBoxGEN, DBoxGEN, ftf, pythia8, <file>.dec (EvtGEN)
gen=DBoxGEN # default
if test "$2" != ""; then
  gen=$2
fi

# Beam momentum => llbar: 1.642 GeV, xibarxi1820: 4.6 GeV, J/Psi: 6.231552 GeV
pBeam=1.642
if test "$3" != ""; then
  pBeam=$3
fi

# Random seed
# seed=42
# if test "$5" != ""; then
#   seed=$5
# fi
seed=$SLURM_ARRAY_TASK_ID

flag="WithoutIdeal"     # With/Without IdealTrackFinder to Fill fParticles CSV

# ---------------------------------------------------------------
#                       Create Directories
# ---------------------------------------------------------------

# target directory for the output files
_target=/lustre/panda/ninderw/simFiles/$prefix

# create the directory structure
mkdir -p $_target/root/
mkdir -p $_target/log/
mkdir -p $_target/cvs/

# make temp directory for the intermediate files
tmpdir=/tmp/pandaSim/$prefix
mkdir -p $tmpdir

# Output path
outprefix=$tmpdir/$prefix

# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------
echo ""
echo "Temp Dir.   : $tmpdir"
echo "Target Dir. : $_target"
echo "Events      : $nevt"
echo "Prefix      : $outprefix"
echo "Decay       : $gen"
echo "pBeam       : $pBeam"
echo "Seed        : $seed"

# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

echo ""
# Generates events and propagates them to a detector simulation
echo "Started Simulation..."
root -l -b -q ../simulationChainMacros/sim_complete.C\($nevt,\"$outprefix\",\"$gen\",$pBeam,$seed\)

# Takes the generated particle tracks and simulates the detector response
echo "Started Digitization..."
root -l -b -q ../simulationChainMacros/digi_complete.C\($nevt,\"$outprefix\"\)

# Applies an correction to the longitudinal parameters of the hits in the skewed straw tubes
echo "Started Skewed Correction..."
root -l -b -q ../simulationChainMacros/skew_complete.C\($nevt,\"$outprefix\"\)

# Reconstructs tracks from the hits in the straw tubes using MC truth (ideal track reconstruction)
echo "Started Ideal Reconstruction..."
root -l -b -q ../simulationChainMacros/recoideal_complete.C\($nevt,\"$outprefix\"\)

# Transfers the hit and track information into CVS files that are readable for the ML pipeline
echo "Started CSV Generator..."
root -l -b -q ../simulationChainMacros/data_complete.C\($nevt,\"$outprefix\",\"$tmpdir\",\"$flag\"\)

echo "Finished All Tasks"

echo ""

# ---------------------------------------------------------------
#                Move Files to the Target Directory
# ---------------------------------------------------------------

echo "Moving Files from '$tmpdir' to '$_target'"

mv ${outprefix}*.root $_target/root
mv ${tmpdir}"/"*.csv $_target/cvs
mv ${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}.out $_target/log/
mv ${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}.err $_target/log/

#*** Tidy Up ***
rm -rf $tmpdir

