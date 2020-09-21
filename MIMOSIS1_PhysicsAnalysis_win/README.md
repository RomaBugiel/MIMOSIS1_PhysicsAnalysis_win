# How to run 

## General desciption 
__MIMOSIS1_PhysicsAnalysis__

The software is dedicated for:
- reading the .root tree with MIMOSIS1 raw data in FIRED format (prepared by MIMOSIS1_DataConverter software)
- saving this data in univeral data format that can be passed to set of class performing the physical analysis of MIMOSIS1 data
- savig the output .root tree with results of physics analysis 

For that moment (10.09.2020) the software is working with __Integrated_Frame__ data structure that correspond to __FIRED__ data format from MIMOSIS1 DAQ. In the future it should be also done for RAW and DEC data format (when available). 

## GitHub
Repository can be cloned from:

__https://github.com/RomaBugiel/MIMOSIS1_PhysicsAnalysis_win.git__

## Software
* C++14, ROOT 

### Needed packages 
The software was tested on:

* ROOT 6.22/00 
* Windows: 10
* g++: 9.3.0 

## Compilation and run

### Software


#### Configuration file


#### Output
> In general the path for saving the root trees is provided via config_file.cfg.
> If not changed, one can find the outputs in: ./outputData

### Documentation


## Scirpits


