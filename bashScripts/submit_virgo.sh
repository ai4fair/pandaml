#!/bin/bash

#SBATCH --job-name=test
#SBATCH --output=%x_%a.out
#SBATCH --error=%x_%a.err
#SBATCH --mail-type=ALL
#SBATCH --mail-user=n_inde01@uni-muenster.de

#SBATCH --get-user-env
#SBATCH --partition=main
#SBATCH --time=00:10:00
#SBATCH --array=0-1
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1

# Output directory for the simulation, cvs and log files
export OUTPUT_DIR=/lustre/panda/ninderw/simFiles

export CVS_GEN_FLAG="WithoutIdeal" 	# With/Without IdealTrackFinder to Fill fParticles CSV
export NUM_GEN_EVENTS=1				# Number of events to be generated
export GENERATOR_NAME=DBoxGEN		# Specify the MC generator that should be used to generate the events
export BEAM_MOMENTUM=1.642			# Beam momentum in GeV/c

./runSimChain_virgo.sh
