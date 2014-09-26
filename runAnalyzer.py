#!/usr/bin/python

import sys
from ROOT import gROOT

# scenarios and samples
scenarios = ['MC','data']
samples   = ['TTbar','T1tttt_1500_100','T1tttt_1200_800']

Lumi=1 #given in fb^-1
Lumi=Lumi * 1000


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

# choose the analysis and a sample
if len(sys.argv)>1:
	if sys.argv[1]=='SingleS_P':  # single lepton testing version 
		gROOT.LoadMacro('readerSingleS_P.C+')
		from ROOT import readerSingleS_P as reader
	elif sys.argv[1]=='TreeAnalyzer':  # single lepton testing version 
		gROOT.LoadMacro('TreeAnalyzer.C+')
		from ROOT import TreeAnalyzer as reader
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
weights={}
for scene in scenarios:
	inDir[scene]={}
#	weights[scene]={}


dirsHT['TTbar']  = ['/']
weights['TTbar'] = [812.8] #cross section in pb
#weights['TTbar'] = scale(Lumi,weights['TTbar'])
inDir['MC']['TTbar'] = '/afs/desy.de/user/s/safarzad/dust/13TeV/TTJet/TTJets_MSDecaysCKM_central_PU_S14_POSTLS170/treeProducerSusySingleLepton/'

# for HT binned input files use: 
#dirsHT['DiBoson']  = ['0-300/','300-700/','700-1300/','1300-2100/','2100-100000/']
#weights['DiBoson'] = [249.97710, 35.23062, 4.13743, 0.41702, 0.04770]
#weights['DiBoson'] = scale(Lumi,weights['DiBoson'])
#inDir['MC']['DiBoson'] = '/afs/desy.de/user/t/trippk/dust/AN_PAS_TP/DELPHES/nTUPLER/batch/1st_Output/PhaseII_140PU_ProdJul28/diboson/'

## TP
dirsHT['T1tttt_1500_100'] = ['/']
weights['T1tttt_1500_100'] = [0.1*Lumi]
inDir['MC']['T1tttt_1500_100'] = '/afs/desy.de/user/s/safarzad/dust/13TeV/T1tttt/TLVector/T1tttt2J_6_PU_S14_POSTLS170/treeProducerSusySingleLepton/'

dirsHT['T1tttt_1200_800'] = ['/']
weights['T1tttt_1200_800'] = [0.1*Lumi]
inDir['MC']['T1ttt_1200_800'] = '/afs/desy.de/user/s/safarzad/dust/13TeV/T1tttt/T1tttt2J_7_PU_S14_POSTLS170/treeProducerSusySingleLepton/'


from ROOT import TFile
from glob import glob
from sys import exit
def GetEntries(dirname):
	files = glob(dirname+'/*.root')
	if len(files)>1:
		print 'GetEntries: there is more than 1 root file in '+dirname
		exit(0)
	print dirname,files
	file=TFile(files[0])
	#tree = file.Get("delphTree")
	tree = file.Get("treeProducerSusySingleLepton")
	return tree.GetEntries()
	
# do it
for scene in scenarios:
	for samp in samples:
		if do[scene][samp]: 
			f=''
			print dirsHT, samp
			for i in range(len(dirsHT[samp])):
				entries = GetEntries(inDir[scene][samp]+dirsHT[samp][i])
				f=f+inDir[scene][samp]+dirsHT[samp][i]+' '+str(weights[samp][i]/entries)+' '
			print f,samp,scene
			reader(f,scene+'_'+samp)




