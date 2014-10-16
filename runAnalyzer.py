#!/usr/bin/python

import sys, os
from ROOT import gROOT

# scenarios and samples
scenarios = ['MC','data']
samples   = ['TTbar','T1tttt_1500_100','T1tttt_1200_800']
treename = 'treeProducerSusySingleLepton'
Lumi = 1 #given in fb^-1
Lumi = Lumi * 1000


def help():
        print 'First argument analysis:'
        print './runreader.py   SingleS TYPE_SAMPLE'
        print ' TYPE = MC, data'
        print ' SAMPLE = ',
        for sample in samples:
                print sample,
        sys.exit(0)

from operator import mul
def scale(fac,list):
        return map(mul,len(list)*[fac],list)

def GetNevents(loc):
        EvtFile = open(loc+"/ttHLepSkimmer/events.txt", "r")
        theInts = []
        for val in EvtFile.read().split():
                if val.isdigit():
                        theInts.append(val)
        EvtFile.close()
        return float(theInts[0])

# choose the analysis and a sample
if len(sys.argv)>1:
        if sys.argv[1]=='SingleS_P':  # single lepton testing version
                gROOT.LoadMacro('readerSingleS_P.C+')
                from ROOT import readerSingleS_P as reader
        elif sys.argv[1]=='TreeAnalyzer':  # single lepton testing version
                gROOT.LoadMacro('TreeAnalyzer.C+')
                from ROOT import TreeAnalyzer as reader
        elif sys.argv[1]=='TreeAnalyzer_ext':  # single lepton testing version
                gROOT.LoadMacro('Objects.C+')
                gROOT.LoadMacro('TreeAnalyzer_ext.C+')
                from ROOT import TreeAnalyzer_ext as reader
        else:
                help()
else:
        help()


#prepare empty dictionaries
do={}
for scene in scenarios:
        do[scene]={}
        for samp in samples:
                do[scene][samp] = 0
flag=False
for e in sys.argv:
        for scene in scenarios:
                for samp in samples:
                        if e==scene+'_'+samp:
                                do[scene][samp] = 1
                                flag=True
if not flag: help()


# prepare empty dictionaries
dirsHT={}
inDir={}
xsec_lumi={}
evtgen={}
for scene in scenarios:
        inDir[scene]={}
#       weights[scene]={}

sample = 'TTbar'
dirsHT[sample]  = ['/']
xsec_lumi[sample] = [812.8*Lumi] #cross section in pb
#xsec['TTbar'] = scale(Lumi,weights['TTbar'])
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/TTJet/TTJets_MSDecaysCKM_central_PU_S14_POSTLS170/'
evtgen[sample]  =  GetNevents(inDir['MC'][sample])

# for HT binned input files use:
#dirsHT['DiBoson']  = ['0-300/','300-700/','700-1300/','1300-2100/','2100-100000/']
#xsec_lumi['DiBoson'] = [249.97710, 35.23062, 4.13743, 0.41702, 0.04770]
#xsec_lumi['DiBoson'] = scale(Lumi,weights['DiBoson'])
#inDir['MC']['DiBoson'] = '/afs/desy.de/user/t/trippk/dust/AN_PAS_TP/DELPHES/nTUPLER/batch/1st_Output/PhaseII_140PU_ProdJul28/diboson/'

## TP
sample = 'T1tttt_1500_100'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.0141903*Lumi]
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/T1tttt/T1tttt2J_6_PU_S14_POSTLS170/'
evtgen[sample] = GetNevents(inDir['MC'][sample])

sample = 'T1tttt_1200_800'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.085641*Lumi]
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/T1tttt/T1tttt2J_7_PU_S14_POSTLS170/'
evtgen[sample] = GetNevents(inDir['MC'][sample])

from ROOT import TFile
from glob import glob
from sys import exit

def GetTreeName(file):
        keylist = file.GetListOfKeys()
        treeKey = keylist.At(0)
        treeName = treeKey.GetName()

        if 'tree' in treeName:
                return treeName
        else:
                print 'Tree not found in ', file.GetName()
                exit(0)

def GetEntries(dirname):
        files = glob(dirname+'/*.root')

        if len(files)<1:
                print 'GetEntries: there is NO root file in '+dirname
                exit(0)

        elif len(files)==1:
                print 'Dirname', dirname, files
                file=TFile(files[0])
                treeName =  GetTreeName(file)
               #tree = file.Get("delphTree")
               #tree = file.Get("treeProducerSusySingleLepton")
                tree = file.Get(treeName)

                print 'Going to analyze ', treeName
                return tree.GetEntries()

        elif len(files)>1:
                print 'GetEntries: there is more than 1 root file in '+dirname
                exit(0)


# do it
for scene in scenarios:
        for samp in samples:
                if do[scene][samp]:
                        f=''
                        print dirsHT, samp
                        for i in range(len(dirsHT[samp])):
                                entries = evtgen[samp]
                                print "cross section x lumi",xsec_lumi[samp], "Events generated", entries
                                f=f+inDir[scene][samp]+treename+dirsHT[samp][i]+' '+str(xsec_lumi[samp][i]/entries)+' '
                        print f,samp,scene

                        reader(f,scene+'_'+samp)
