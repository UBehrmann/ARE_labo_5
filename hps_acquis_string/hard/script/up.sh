#!/bin/bash

# Program the FPGA
python3 pgm_fpga.py -s=../eda/output_files/DE1_SoC_top.sof

# Upload to HPS
python3 upld_hps.py
