#!/bin/bash

#SBATCH --partition=debug
#SBATCH --array=0-2
#SBATCH --job-name=mum5mup5_simulation_%a
#SBATCH --output=%x_%a.out
#SBATCH --error=%x_%a.err
#SBATCH --time=00:10:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=n_inde01@uni-muenster.de
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --get-user-env
#SBATCH --chdir=/lustre/panda/ninderw/simFiles

runSimChain_virgo.sh 1000 %x_%a DBoxGEN 1.642 %a