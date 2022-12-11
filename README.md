## _Mount Lustre_

```bash
# mount lustre
mnt=/tmp/lustre
mkdir $mnt
sshfs lxpool-lustre:/lustre/panda/aakram/ $mnt

# unmount lustre
fusermount -u $mnt
```

## _Run Simulation_
### _1. Virgo Cluster_

```bash
# connect to virgo
ssh lxpool-virgo-debian10
```



```bash
# one job (single node)
./jobsim_complete.sh 100 llbar llbar_fwp.dec
```

```bash
# for job array
./submit.jobs
```

### _2. Local Machine_

To run simulation one needs to run either `runall.sh` (local) or `runsif.sh` (singularity) scripts _e.g._

```bash
# run on local machine
./runall 100 box DBGEN
```

Similary if one have singularity container for PandaRoot, then just run `runsif.sh`.

```bash
# run on container
./runsif 100 box DBGEN
```


All bash scripts should run following simulation chain:

```bash
root -l -b -q sim_complete.C\($nevt,\"$outprefix\",\"$gen\",$pBeam,$seed\) > $outprefix"_sim.log" 2>&1
root -l -b -q digi_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_digi.log" 2>&1
root -l -b -q skew_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_skew.log" 2>&1
root -l -b -q recoideal_complete.C\($nevt,\"$outprefix\"\) > $outprefix"_reco.log" 2>&1
root -l -b -q data_complete.C\($nevt,\"$outprefix\",\"$_target\",\"$flag\"\) > $outprefix"_data.log" 2>&1
```
