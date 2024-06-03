#!/bin/bash

sbatch -p=debug -a=0-2 -J=mum5mup5_simulation -o=%x_%a.out -e=%x_%a.err -t=00:10:00 --mail-type=ALL --mail-user=n_inde01@uni-muenster.de -N=1 -c=1 --get-user-env ./runSimChain_virgo.sh 10 %x_%a DBoxGEN 1.642 %a