##!/usr/bin/python
from operator import mul

def scale(fac,list):
    return map(mul,len(list)*[fac],list)

# scenarios and samples
scenarios = ['MC','data']
SAMPLES = ['WJets','TTbar','TTbar_DiLep','TTbar_SinLep','T1tttt_1500_100','T1tttt_1200_800','T1tttt_800_450','T1tttt_1300_100','T1qqqq_1400_15']
treename = 'treeProducerSusySingleLepton/'
base = '/nfs/dust/cms/group/susy-desy/Run2/MC/CMGtuples/Phys14_v1/'
Lumi = 1 #given in fb^-1
Lumi = Lumi * 1000

# prepare empty dictionaries
dirsHT={}
inDir={}
xsec_lumi={}
evtgen={}
for scene in scenarios:
    inDir[scene]={}

##################################
#       BACKGROUNDS
##################################
sample = 'TTbar'
dirsHT[sample]  = ['/']
xsec_lumi[sample] = [809.1*Lumi] #cross section in pb
inDir['MC'][sample] = base+'TTJets'
##################################
sample = 'TTbar_SinLep'
dirsHT[sample]  = ['/']
xsec_lumi[sample] = [809.1*Lumi] #cross section in pb
inDir['MC'][sample] = base+'TTJets/'
##################################
sample = 'TTbar_DiLep'
dirsHT[sample]  = ['/']
xsec_lumi[sample] = [809.1*Lumi] #cross section in pb
inDir['MC'][sample] = base+'TTJets/'
##################################
sample = 'WJets'
dirsHT[sample]  = ['100-200/','200-400/','400-600/','600-Inf/']
xsec_lumi[sample] = [2234.9,580.06,68.38,23.14]
xsec_lumi[sample] = scale(Lumi,xsec_lumi[sample])
inDir['MC'][sample] = base+'WJets/'

##################################
#       SIGNAL
##################################
sample = 'T1tttt_1500_100'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.0141903*Lumi]
inDir['MC'][sample] = base+'SMS_T1tttt_2J_mGl1500_mLSP100/'
##################################
sample = 'T1tttt_1200_800'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.085641*Lumi]
inDir['MC'][sample] = base+'SMS_T1tttt_2J_mGl1200_mLSP800/'
## DESY PRIVATE
sample = 'T1tttt_800_450'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [1.489*Lumi]
inDir['MC'][sample] = base+'SMS_T1tttt_2J_mGl800_mLSP450/'
##################################
sample = 'T1tttt_1300_100'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.044*Lumi]
inDir['MC'][sample] = base+'SMS_T1tttt_2J_mGl1300_mLSP100/'
##################################
sample = 'T1qqqq_1400_15'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.044*Lumi] # FIXME
inDir['MC'][sample] = base+'SMS_Gl_Gl_mGl1400_mLSP300_mChi315/'
##################################
