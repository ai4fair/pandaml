#!/bin/bash

#SBATCH --job-name=test
#SBATCH --output=%x_%a.out
#SBATCH --error=%x_%a.err
#SBATCH --mail-type=ALL
#SBATCH --mail-user=n_inde01@uni-muenster.de

#SBATCH --get-user-env
#SBATCH --partition=debug
#SBATCH --time=00:10:00
#SBATCH --array=0-1
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1

export NUM_GEN_EVENTS=1
export GENERATOR_NAME=DBoxGEN
export BEAM_MOMENTUM=1.642

./runSimChain_virgo.sh