#!/usr/bin/env python
import os, glob, sys

def chunks(l, nchunks):
    n = len(l) / nchunks
    n = max(1, n)
    print n
    return [l[i:i + n] for i in range(0, len(l), n)]

if __name__ == "__main__":


#    sample = 'DY'
#    DList = glob.glob('/pnfs/desy.de/cms/tier2/store/user/clseitz/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_zmm_25ns/160501_085349/0000/*root')

#    sample = 'DoubleMu2015'
#    DList = glob.glob('/pnfs/desy.de/cms/tier2/store/user/clseitz/DoubleMuon/crab_Run2015D_DoubleMu_PUDiscUpdate/160610_090625/000*/*root')

    sample = 'DYMG'
    DList = glob.glob('/pnfs/desy.de/cms/tier2/store/user/clseitz/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_zmmMG_PUDiscUpd_25ns/160610_082829/0000/*root')

    nFiles = len(DList)
    print "found "+ str(nFiles)+ " files"
    nFolders = 10

    nFilesPerFolder = nFiles / nFolders
    print "will create "+ str(nFolders) + " folders, with " + str(nFilesPerFolder)


    for i,chunk in enumerate(chunks(DList, nFolders)):
        cmd = 'mkdir '+sample+'_chunk'+str(i)
        os.popen(cmd)
        print cmd
        for f in chunk:
            cmd = "ln -s " + f + " "+sample+"_chunk"+str(i) + "/."
            print cmd
            os.popen(cmd)

    
