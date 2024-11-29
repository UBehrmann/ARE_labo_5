# ARE Laboratoire 5

# Simulation

Dans le dossier ../hps_acquis_string/hard/sim/

> vsim -do ../script/run_avalon_sim.tcl

# Code pour lancer le preloader

Dans le dossier ../hps_acquis_string/hard/script/

```bash

python3 pgm_fpga.py -s=../eda/output_files/DE1_SoC_top.sof
python3 upld_hps.py

```