##!/usr/bin/python
from operator import mul

def scale(fac,list):
    return map(mul,len(list)*[fac],list)

# scenarios and samples
scenarios = ['MC','data']
SAMPLES = ['WJets','TTbar','TTbar_DiLep','TTbar_SinLep','T1tttt_1500_100','T1tttt_1200_800','T1tttt_800_450','T1tttt_1300_100','T1qqqq_1400_15']
treename = 'treeProducerSusySingleLepton/'
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
inDir['MC'][sample] = '/nfs/dust/cms/group/susy-desy/ISOTrck/TTJets/TTJets_MSDecaysCKM_central_PU_S14_POSTLS170/'
##################################
sample = 'TTbar_SinLep'
dirsHT[sample]  = ['/']
xsec_lumi[sample] = [809.1*Lumi] #cross section in pb
inDir['MC'][sample] = '/nfs/dust/cms/group/susy-desy/ISOTrck/TTJets/TTJets_MSDecaysCKM_central_PU_S14_POSTLS170/'
##################################
sample = 'TTbar_DiLep'
dirsHT[sample]  = ['/']
xsec_lumi[sample] = [809.1*Lumi] #cross section in pb
inDir['MC'][sample] = '/nfs/dust/cms/group/susy-desy/ISOTrck/TTJets/TTJets_MSDecaysCKM_central_PU_S14_POSTLS170/'
##################################
sample = 'WJets'
dirsHT[sample]  = ['100-200/','200-400/','400-600/','600-Inf/']
xsec_lumi[sample] = [2234.9,580.06,68.38,23.14]
xsec_lumi[sample] = scale(Lumi,xsec_lumi[sample])
inDir['MC'][sample] = '/nfs/dust/cms/group/susy-desy/ISOTrck/WJets/'

##################################
#       SIGNAL
##################################
sample = 'T1tttt_1500_100'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.0141903*Lumi]
inDir['MC'][sample] = '/nfs/dust/cms/group/susy-desy/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl1500_mLSP100_PU_S14_POSTLS170/'
##################################
sample = 'T1tttt_1200_800'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.085641*Lumi]
inDir['MC'][sample] = '/nfs/dust/cms/group/susy-desy/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl1200_mLSP800_PU_S14_POSTLS170/'
## DESY PRIVATE
sample = 'T1tttt_800_450'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [1.489*Lumi]
inDir['MC'][sample] = '/nfs/dust/cms/group/susy-desy/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl800_mLSP450_PU_S14_POSTLS170/'
##################################
sample = 'T1tttt_1300_100'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.044*Lumi]
inDir['MC'][sample] = '/nfs/dust/cms/group/susy-desy/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl1300_mLSP100_PU_S14_POSTLS170/'
##################################
sample = 'T1qqqq_1400_15'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.044*Lumi] # FIXME
inDir['MC'][sample] = '/nfs/dust/cms/group/susy-desy/Run2/MC/CMGtuples/Signal/SMS_Gl_Gl_mGl1400_mLSP300_mChi315_PU_S14/'
##################################
