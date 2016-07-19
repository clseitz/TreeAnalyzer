##!/usr/bin/python
from operator import mul

def scale(fac,list):
    return map(mul,len(list)*[fac],list)

# scenarios and samples
scenarios = ['MC','data']
treename = ''
base = ''
Lumi = 2.300 #given in fb^-1
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
sample = 'DY' 
dirsHT[sample]  = ['DY_chunk0/','DY_chunk1/','DY_chunk2/','DY_chunk3/','DY_chunk4/']
nLepFlav = 3.0
xsec_lumi[sample] = [5902.0, 5902.0, 5902.0, 5902.0, 5902.0] #from AN2015_250_v8
xsec_lumi[sample] = scale(Lumi,xsec_lumi[sample])
evtgen[sample] = 19259724 #sum of weights #orignal number of events 28751199
inDir['MC'][sample] = base+'DY/'

sample = 'DoubleMu2015'
dirsHT[sample]  = ['DoubleMu2015_chunk0/','DoubleMu2015_chunk1/','DoubleMu2015_chunk2/','DoubleMu2015_chunk3/']
xsec_lumi[sample] = [1,1,1,1]
evtgen[sample] = 1
inDir['data'][sample] = base+'DoubleMu2015/'


for i in range(0,10):    
    sample = 'DY_chunk' + str(i)
    dirsHT[sample]  = ['/']
    xsec_lumi[sample] = [5902.0]
    xsec_lumi[sample] = scale(Lumi,xsec_lumi[sample])
    evtgen[sample] = 19259724 #sum of weights #orignal number of events 28751199
    inDir['MC'][sample] = base+'DY/DY_chunk'+ str(i)

for i in range(0,11):    
    sample = 'DYMG_chunk' + str(i)
    dirsHT[sample]  = ['/']
    xsec_lumi[sample] = [5833.0]
    xsec_lumi[sample] = scale(Lumi,xsec_lumi[sample])
    evtgen[sample] = 9004328 #actually too high few jobs missing
    inDir['MC'][sample] = base+'DYMG/DYMG_chunk'+ str(i)


for i in range(0,11):    
    sample = 'DoubleMu2015_chunk' + str(i)
    dirsHT[sample]  = ['/']
    xsec_lumi[sample] = [1]
    evtgen[sample] = 1
    inDir['data'][sample] = base+'DoubleMu2015/DoubleMu2015_chunk'+ str(i)


SAMPLES = sorted(inDir['MC'].keys() + inDir['data'].keys()) 
print SAMPLES
