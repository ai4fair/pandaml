#!/bin/bash

sbatch --partition=debug --array=0-2 --job-name=mum5mup5_simulation --output=%x_%a.out --error=%x_%a.err --time=00:10:00 --mail-type=ALL --mail-user=n_inde01@uni-muenster.de --nodes=1 --cpus-per-task=1 --get-user-env ./runSimChain_virgo.sh 10 DBoxGEN 1.642