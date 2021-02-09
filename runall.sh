#!/usr/bin/env bash

if((0)); then
rm *.root
rm *.log
rm *.pdf
rm *.csv
rm *.dat
fi

path=data

# For CSV a directory named `data` is needed
if [[ ! -e $path ]]; then
    mkdir $path
elif [[ ! -d $path ]]; then
    echo "$path already exists but is not a directory" 1>&2
fi

root -q -b sim_complete_vis.C"(10)" > sim.log 2>&1
# root -q -b sim_complete.C"(1000)" > sim.log 2>&1
root -q -b digi_complete.C > digi.log 2>&1
#root -q -b pzfiner.C > pzfinder.log 2>&1
#root -q -b recoideal_complete.C > reco.log 2>&1
#root -q -b pid_complete.C > pid.log 2>&1
#root -q -b recoqa_complete.C > recoqa.log 2>&1
#root -q -b trackml_data.C > trackml.log 2>&1
