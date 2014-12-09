This code is used to run ntuples produced by CMG tools.
It is not necessary to know the exact content of the ntuple beforehand, branches
can be read on the fly. The tree name is hard coded and would need to be adjusted.

Explanation:

src/ClassObjects.C: This file contains all the object definitions (leptons, jets, met, genparticles etc).
		    The function GetKinVariables() contains event variables such as HT, ST, etc.
		    All varibales and functions are defined in interface/ClassObjects.C

src/TreeAnalyzer_example.C: Contains the example code to run over ntuples with the event loop.
			    A few basic plots are filled and examples are given for a more complex histogram structure.
			    The cutflow can also be written out here into a textfile.

Makefile: The variable CODE needs the name of the analyzer in the 
	  src directory that should be compiled (i.e.: TreeAnalzyer_example).

How to get code to run:
* make
* cd python
* ./runAnalyzer.py TreeAnalyzer_example MC_TTbar


-------------------------------------------------------------------------------
* runAnalyzer.py: python run script that defines all samples, cross sections, number of events, lumi
* TreeAnalyzer.C: actuall C++ code to run over the ntuple and make plots
* NtupleTools2.h: code to read the nutple on the fly
* go.sh, setupNAH.sh: scripts to submit jobs at DESY run like
* ./go.sh TreeAnalyzer

ToDo List:
 * adjust "0" cut to represent the ntuple cuts for consistency check
 * implement jetID (applied already in CMG), muon, electron id
 * fix setupNAF.sh script to use work properly on SL6 machines
 * allow input of multiple files
 * implement analysis, histograms, etc.

Done:
 * event weight is fixed, automatic read in fixed