This code is used to run ntuples produced by CMG tools.
It is not necessary to know the exact content of the ntuple beforehand, branches
can be read on the fly. The tree name is hard coded and would need to be adjusted.

How to run:
./runAnalyzer.py TreeAnalyzer MC_TTbar

runAnalyzer.py: python run script that defines all samples, cross sections, number of events, lumi
TreeAnalyzer.C: actuall C++ code to run over the ntuple and make plots
NtupleTools2.h: code to read the nutple on the fly
go.sh, setupNAH.sh: scripts to submit jobs at DESY run like
./go.sh TreeAnalyzer

ToDo List:
 * adjust weights properly, need to input number of events manually since we are applying a skim
 * implement jetID (applied already in CMG), muon, electron id
 * fix setupNAF.sh script to use work properly on SL6 machines
 * implement analysis, histograms, etc.
