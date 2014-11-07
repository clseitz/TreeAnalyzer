#!/usr/bin/python

import sys
import os
from glob import glob
from sys import exit
from operator import mul

from ROOT import gROOT
from ROOT import TFile

import samples

def help():
    print 'First argument analysis:'
    print './runreader.py   SingleS TYPE_SAMPLE'
    print ' TYPE = MC, data'
    print ' SAMPLE = ',
    for sample in samples:
        print sample,
        sys.exit(0)

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

#gROOT.LoadMacro('Objects.C+')
srcdir = '../src/'

#gROOT.LoadMacro(srcdir+'Objects.C+')

if len(sys.argv)>1:
	if sys.argv[1]=='testSamples':
		print 'Testing samples file'
	else:
		help()
else:
    help()

# do it
for scene in scenarios:

    for samp in samples:
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

