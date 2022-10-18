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