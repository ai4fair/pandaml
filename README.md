
## Data Generation

### Run Simulation

```bash
# syntax
./runall <total events> <output_prefix> <generator_type>
```

To run simulation one needs to run either `runall.sh` (local) or `runsif.sh` (singularity) scripts. So for example,

```bash
# run on local machine
./runall 100 box DBGEN
```

Similary if one have singularity container for PandaRoot, then just run `runsif.sh`.

```bash
# run on local machine
./runsif 100 box DBGEN
```



