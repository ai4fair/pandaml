#!/usr/bin/env bash
root -q -b sim_complete_vis.C"(1000)" > sim.log 2>&1
root -q -b digi_complete.C > digi.log 2>&1
#root -q -b recoideal_complete.C > recoideal.log 2>&1
#root -q -b generatePatternDB.C > genDB.log 2>&1
