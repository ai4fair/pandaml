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

# Make a temporary directory for the files during the simulation steps
tmpDir=/tmp/$SLURM_JOB_NAME/$SLURM_ARRAY_TASK_ID
mkdir -p $tmpDir/root
mkdir -p $tmpDir/cvs

# Prefix for the in- and output root files
prefix=$tmpDir/root/${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}

# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------

echo ""
echo "Temp Directory (root)	: $tmpDir/root"
echo "Temp Directory (cvs)	: $tmpDir/cvs"
echo "Output Directory		: $outDir"
echo "Number of Events		: $NUM_GEN_EVENTS"
echo "Prefix     			: $prefix"
echo "MC Generator 			: $GENERATOR_NAME"
echo "Beam Momentum       	: $BEAM_MOMENTUM"
echo "Seed					: $SLURM_ARRAY_TASK_ID"
echo ""

# ---------------------------------------------------------------
#                            Initiate Simulaton
# ---------------------------------------------------------------

# Generates events and propagates them to a detector simulation
echo "Started Simulation..."
root -l -b -q ../simulationChainMacros/sim_complete.C\($NUM_GEN_EVENTS,\"$prefix\",\"$GENERATOR_NAME\",$BEAM_MOMENTUM,$SLURM_ARRAY_TASK_ID\)

# Takes the generated particle tracks and simulates the detector response
echo "Started Digitization..."
root -l -b -q ../simulationChainMacros/digi_complete.C\($NUM_GEN_EVENTS,\"$prefix\"\)

# Applies an correction to the longitudinal parameters of the hits in the skewed straw tubes
echo "Started Skewed Correction..."
root -l -b -q ../simulationChainMacros/skew_complete.C\($NUM_GEN_EVENTS,\"$prefix\"\)

# Reconstructs tracks from the hits in the straw tubes using MC truth (ideal track reconstruction)
echo "Started Ideal Reconstruction..."
root -l -b -q ../simulationChainMacros/recoideal_complete.C\($NUM_GEN_EVENTS,\"$prefix\"\)

# Transfers the hit and track information into CVS files that are readable for the ML pipeline
echo "Started CSV Generator..."
root -l -b -q ../simulationChainMacros/data_complete.C\($NUM_GEN_EVENTS,\"$prefix\",\"$tmpDir/cvs\",\"$CVS_GEN_FLAG\"\)

echo "Finished All Simulation Tasks"
echo ""

# ---------------------------------------------------------------
#                Move Files to the Target Directory
# ---------------------------------------------------------------

echo "Moving Files from '$tmpDir' to '$outDir'"

mv ${prefix}*.root $outDir/root
mv $tmpDir/cvs/* $outDir/cvs
mv ${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}.out $outDir/log/
mv ${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}.err $outDir/log/

#*** Tidy Up ***
rm -rf $tmpDir

echo "Everything is done!"