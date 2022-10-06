

## Virgo Cluster

```bash
# connect to virgo
ssh lxpool-virgo-debian10
```

```bash
# mount lustre
mnt=/tmp/lustre
mkdir $mnt
sshfs lxpool-lustre:/lustre/panda/aakram/ $mnt

# unmount lustre
fusermount -u $mnt
```

```bash
# run simulation
./jobsim_complete.sh <total_events> <output_prefix> <generator_type>

# For example
./jobsim_complete.sh 100 llbar llbar_fwp.dec
```

```bash
# for job array
./submit.jobs
```

## Desktop PC

```bash
# run simulation
./runall  <total_events> <output_prefix> <generator_type>
```

To run simulation one needs to run either `runall.sh` (local) or `runsif.sh` (singularity) scripts _e.g._

```bash
# run on local machine
./runall 100 box DBGEN
```

Similary if one have singularity container for PandaRoot, then just run `runsif.sh`.

```bash
# run on local machine
./runsif 100 box DBGEN
```



