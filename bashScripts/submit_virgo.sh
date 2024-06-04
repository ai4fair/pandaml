#!/bin/bash

sbatch --partition=main --array=0-1 --job-name=test --output=%x_%a.out --error=%x_%a.err --time=00:05:00 --mail-type=ALL --mail-user=n_inde01@uni-muenster.de --nodes=1 --cpus-per-task=1 --get-user-env ./runSimChain_virgo.sh 1 DBoxGEN 1.642
