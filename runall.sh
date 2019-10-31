#!/usr/bin/env bash

if((1)); then
rm *.root
rm *.log
rm *.pdf
rm *.csv
rm *.dat
fi

root -q -b sim_complete_vis.C"(1000)" > sim.log 2>&1
#root -q -b sim_complete.C"(1000)" > sim.log 2>&1
root -q -b digi_complete.C > digi.log 2>&1
#root -q -b recoideal_complete.C > reco.log 2>&1
#root -q -b pid_complete.C > pid.log 2>&1
#root -q -b recoqa_complete.C > recoqa.log 2>&1
root -q -b generatePatternDB.C > genDB.log 2>&1
