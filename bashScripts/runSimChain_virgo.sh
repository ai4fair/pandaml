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
# recoideal_complete.C 		Ideal reconstruction of tracks using the MC truth
# data_complete.C		Transfer of hit and track information into CSV files for the ML pipeline

# ---------------------------------------------------------------
#                       Create Directories
# ---------------------------------------------------------------

# Make a temporary directory for the files during the simulation steps
tmpRootDir=/tmp/$SLURM_JOB_NAME/$SLURM_ARRAY_TASK_ID/root
tmpCvsDir=/tmp/$SLURM_JOB_NAME/$SLURM_ARRAY_TASK_ID/cvs
mkdir -p $tmpRootDir
mkdir -p $tmpCvsDir

# Prefix for the in- and output root files
prefix=$tmpRootDir/${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}

# ---------------------------------------------------------------
#                              Print Flags
# ---------------------------------------------------------------

echo ""
echo "Temp Directory (root)	: $tmpRootDir"
echo "Temp Directory (cvs)	: $tmpCvsDir"
echo "Output Directory		: $OUTPUT_DIR"
echo "Number of Events		: $NUM_GEN_EVENTS"
echo "Prefix     		: $prefix"
echo "MC Generator 		: $GENERATOR_NAME"
echo "Beam Momentum       	: $BEAM_MOMENTUM"
echo "Seed			: $SLURM_ARRAY_TASK_ID"
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
root -l -b -q ../simulationChainMacros/data_complete.C\($NUM_GEN_EVENTS,\"$prefix\",\"$tmpCvsDir\",\"$CVS_GEN_FLAG\"\)

echo "Finished All Simulation Tasks"
echo ""

# ---------------------------------------------------------------
#                Move Files to the Target Directory
# ---------------------------------------------------------------

# Move the root and csv files out of the temporary directory
taskOutputDir=$OUTPUT_DIR/$SLURM_JOB_NAME/$SLURM_ARRAY_TASK_ID
mkdir -p $taskOutputDir
echo "Moving files from $tmpRootDir and $tmpCvsDir to $taskOutputDir"
mv $tmpRootDir $taskOutputDir
mv $tmpCvsDir $taskOutputDir
echo "Done"
echo " "

# Move the log files out of the bash script directory
logDir=$OUTPUT_DIR/$SLURM_JOB_NAME/$SLURM_ARRAY_TASK_ID/log
mkdir -p $logDir
echo "Moving log files from $pwd to $logDir"
mv ${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}.out $logDir
mv ${SLURM_JOB_NAME}_${SLURM_ARRAY_TASK_ID}.err $logDir
echo "Done"
echo " "

if [ -e "gphysi.dat" ]; then
	echo "Moving the gphysi.dat file containing the GEANT4 parameters to $OUTPUT_DIR/$SLURM_JOB_NAME"
	mv gphysi.dat $OUTPUT_DIR/$SLURM_JOB_NAME
fi

# Tidy Up
echo "Tidying Up..."
rm -rf $tmpDir
echo "Done"
echo " "
echo "Everything is done!"
