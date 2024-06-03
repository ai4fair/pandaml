#!/bin/bash

<<<<<<< HEAD
#SBATCH --partition=debug
#SBATCH --array=0-2
#SBATCH --job-name=mum5mup5_simulation
#SBATCH --output=%x_%a.out
#SBATCH --error=%x_%a.err
#SBATCH --time=00:10:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=n_inde01@uni-muenster.de
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --get-user-env

./runSimChain_virgo.sh 10 %x_%a DBoxGEN 1.642 %a
=======
sbatch -p=debug -a=0-2 -J=mum5mup5_simulation -o=%x_%a.out -e=%x_%a.err -t=00:10:00 --mail-type=ALL --mail-user=n_inde01@uni-muenster.de -N=1 -c=1 --get-user-env ./runSimChain_virgo.sh 10 %x_%a DBoxGEN 1.642 %a
>>>>>>> a9f6d30fa0b5a768950d165196accbf61e959d7d
