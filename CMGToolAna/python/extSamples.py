#!/usr/bin/python

import sys
import os
from glob import glob
from sys import exit

from ROOT import gROOT
from ROOT import TFile

from EOSsamples import *
#from DESYsamples import *
#import samples
#SAMPLES = ['WJets','TTbar','TTbar_DiLep','TTbar_SinLep','T1tttt_1500_100','T1tttt_1200_800','T1tttt_800_450','T1tttt_1300_100']

def help():
    print 'First argument analysis:'
    print './runreader.py Analyzer TYPE_SAMPLE'
    print ' TYPE = MC, data'
    print ' SAMPLE = ',

#    for sample in SAMPLES:
#        sample,
    print SAMPLES

    sys.exit(0)

def GetNevents(loc):
    EvtFile = open(loc+"ttHLepSkimmer/events.txt", "r")
    theInts = []
    for val in EvtFile.read().split():
        if val.isdigit():
            theInts.append(val)
        EvtFile.close()
    return float(theInts[0])

def GetTreeName(file):
    keylist = file.GetListOfKeys()
    treeKey = keylist.At(0)
    treeName = treeKey.GetName()

    if 'tree' in treeName:
        return treeName
    else:
        print 'Tree not found in ', file.GetName()
        exit(0)

# choose the analysis and a sample
srcdir = '../src/'

#gROOT.LoadMacro(srcdir+'Objects.C+')

if len(sys.argv)>1:
    if sys.argv[1]=='testSamples':
        print 'Testing samples file'
    elif sys.argv[1]=='TreeAnalyzer':  # single lepton testing version
        print 'Using Single'
        gROOT.LoadMacro(srcdir+'ClassObjects.C+')
        gROOT.LoadMacro(srcdir+'TreeAnalyzer_SingleLep.C+')
        from ROOT import TreeAnalyzer as reader
    elif sys.argv[1]=='TreeOutput':  # test tree output
        gROOT.LoadMacro(srcdir+'ClassObjects.C+')
        gROOT.LoadMacro(srcdir+'TreeOutput.C+')
        from ROOT import TreeWriter as reader
    else:
        help()
else:
    help()

foundFlag=False
for e in sys.argv:
    for scene in scenarios:
        for samp in SAMPLES:
            if e==scene+'_'+samp:
                evtgen[samp] = [GetNevents(inDir[scene][samp])]
                do[scene][samp] = 1
                foundFlag=True

if not foundFlag: help()

# do it
for scene in scenarios:

    for samp in SAMPLES:
        if do[scene][samp]:
            f=''
            print dirsHT, samp
            for i in range(len(dirsHT[samp])):
                entries = evtgen[samp][i]
                print "cross section x lumi",xsec_lumi[samp], "Events generated", entries
                f=f+inDir[scene][samp]+dirsHT[samp][i]+treename+' '+str(xsec_lumi[samp][i]/entries)+' '
                print "file name to be processed", f
                print f,samp,scene
                reader(f,scene+'_'+samp)

