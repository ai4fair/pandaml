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

# ---------------------------------------------------------------
#                       Create Directories
# ---------------------------------------------------------------

prefix=${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}

# target directory for the output files
outDir=/lustre/panda/ninderw/simFiles/$prefix

# create the directory structure
mkdir -p $outDir/root/
mkdir -p $outDir/log/
mkdir -p $outDir/cvs/

# make temp directory for the intermediate files
tmpDir=/tmp/pandaSim/$prefix
mkdir -p $tmpDir

# Output path
outprefix=$tmpDir/$prefix

# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------

echo ""
echo "Temp Dir.  			: $tmpDir"
echo "Target Dir.			: $outDir"
echo "Events     			: $NUM_GEN_EVENTS"
echo "Prefix     			: $outprefix"
echo "Decay      			: $GENERATOR_NAME"
echo "BEAM_MOMENTUM       	: $BEAM_MOMENTUM"
echo "SLURM_ARRAY_TASK_ID   : $SLURM_ARRAY_TASK_ID"
echo ""

# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

# Generates events and propagates them to a detector simulation
echo "Started Simulation..."
root -l -b -q ../simulationChainMacros/sim_complete.C\($NUM_GEN_EVENTS,\"$outprefix\",\"$GENERATOR_NAME\",$BEAM_MOMENTUM,$SLURM_ARRAY_TASK_ID\)

# Takes the generated particle tracks and simulates the detector response
echo "Started Digitization..."
root -l -b -q ../simulationChainMacros/digi_complete.C\($NUM_GEN_EVENTS,\"$outprefix\"\)

# Applies an correction to the longitudinal parameters of the hits in the skewed straw tubes
echo "Started Skewed Correction..."
root -l -b -q ../simulationChainMacros/skew_complete.C\($NUM_GEN_EVENTS,\"$outprefix\"\)

# Reconstructs tracks from the hits in the straw tubes using MC truth (ideal track reconstruction)
echo "Started Ideal Reconstruction..."
root -l -b -q ../simulationChainMacros/recoideal_complete.C\($NUM_GEN_EVENTS,\"$outprefix\"\)

# Transfers the hit and track information into CVS files that are readable for the ML pipeline
echo "Started CSV Generator..."
flag="WithoutIdeal" # With/Without IdealTrackFinder to Fill fParticles CSV
root -l -b -q ../simulationChainMacros/data_complete.C\($NUM_GEN_EVENTS,\"$outprefix\",\"$tmpDir\",\"$flag\"\)

echo "Finished All Simulation Tasks"

echo ""

# ---------------------------------------------------------------
#                Move Files to the Target Directory
# ---------------------------------------------------------------

echo "Moving Files from '$tmpDir' to '$outDir'"

mv ${outprefix}*.root $outDir/root
mv ${tmpDir}"/"*.csv $outDir/cvs
mv ${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}.out $outDir/log/
mv ${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}.err $outDir/log/

#*** Tidy Up ***
rm -rf $tmpDir

echo "Everything is done!"