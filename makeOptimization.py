#!/usr/bin/python

import sys
from ROOT import TH1D,THStack,TFile,TCanvas,TLegend,kRed,kOrange,kBlue,kCyan,RooStats
import math
dirName='September5_30GeV20GeVveto'
f = TFile(dirName+"/"+dirName+"_optimization.root", "recreate")

global h_stc,h_nm1,h_nm2,h_nm3,h_stcBkd,h_nm1Bkd,h_nm2Bkd,h_nm3Bkd

def get(file,what,Lfac,lineCol,lineWid,lineSty,fillCol,reb=0):
	
	h = file.Get(what)
	h.SetLineColor(lineCol)
	h.SetLineWidth(lineWid)
	h.SetLineStyle(lineSty)
	h.SetFillColor(fillCol)
	h.Scale(Lfac)
	if reb==1: h.Rebin()
	return h.Clone()

ana='SingleS_P+DelphMET'
tev='14TeV'	
# scale factor - histos are for 300/fb
Lfac=1.0
#base of filename
base=dirName+'/'+ana+'_'
#
###
## pileup scenario
#pu  ='NoPU' 
#pu  ='50PU' 
pu ='140PU'

c1=TCanvas('c1','',800,600)

#--------------------------------------------------------------------------------------------------------------------------------------------------------

########## preselection
quantity=['nJet','nBJet','0JetpT','RawMET','HT','mT','mT2W','Central','dPhi']
cutstart = [0,0,0,0,0,0,0,0,0]
#cutend = [1,1,1,1,1,1,1,1,1]
cutend = [10,10,1000,2500,2000,500,500,20,100]
cutstep = [1,1,20,50,50,20,20,1,4]
binsize=[1,1,10,20,20,10,10,0.01,0.04]

evsel=[3,5,7,6,11,11,13,15,18]
#evsel=[16,17,18,19,19,19,19,19,19]
#evsel=[3,3,3,3,3,3]
cuts=["bjets == 1/2"] 
cutsShort=["nBJeteq12"]



file_TTbar = TFile.Open(base+pu+'_TTbar_his.root')
file_BosonJets = TFile.Open(base+pu+'_BosonJets_his.root')
file_TopJets = TFile.Open(base+pu+'_TopJets_his.root')
file_DiBoson = TFile.Open(base+pu+'_DiBoson_his.root')

file_STC = TFile.Open(base+pu+'_STCfirst_his.root')
file_NM1 = TFile.Open(base+pu+'_NM1_his.root')
file_NM2 = TFile.Open(base+pu+'_NM2_his.root')
file_NM3 = TFile.Open(base+pu+'_NM3_his.root')
file_NM3_stoponly = TFile.Open(base+pu+'_NM3_his_stoponly.root')
file_STC_stoponly = TFile.Open(base+pu+'_STCfirst_his_stoponly.root')
file_NM1_stoponly = TFile.Open(base+pu+'_NM1_his_stoponly.root')
file_NM2_stoponly = TFile.Open(base+pu+'_NM2_his_stoponly.root')

i=0


for what in quantity:
	print what , "stop only", "no stop", "bkg"
	k = evsel[i]
	print what+'_'+str(k)
	h_ttbar = get(file_TTbar, what+'_'+str(k),Lfac,1,1,1,kBlue,0)
	h_bjets = get(file_BosonJets, what+'_'+str(k),Lfac,1,1,1,kOrange,0)
	h_tjets = get(file_TopJets, what+'_'+str(k),Lfac,1,1,1,kCyan,0)
	h_dibos = get(file_DiBoson, what+'_'+str(k),Lfac,1,1,1,kRed,0)
	h_stc  = get(file_STC, what+'_'+str(k),Lfac,9,2,1,0,0)
	h_nm1  = get(file_NM1, what+'_'+str(k),Lfac,6,2,1,0,0)
	h_nm2  = get(file_NM2, what+'_'+str(k),Lfac,1,2,1,0,0)
	h_nm3  = get(file_NM3, what+'_'+str(k),Lfac,8,2,1,0,0)
	
	h_stc_stoponly  = get(file_STC_stoponly, what+'_'+str(k),Lfac,9,2,2,0,0)
	h_nm1_stoponly  = get(file_NM1_stoponly, what+'_'+str(k),Lfac,6,2,2,0,0)
	h_nm2_stoponly  = get(file_NM2_stoponly, what+'_'+str(k),Lfac,1,2,2,0,0)
	h_nm3_stoponly  = get(file_NM3_stoponly, what+'_'+str(k),Lfac,8,2,2,0,0)

	print	cutstart[i],cutend[i],cutstep[i]
	for cut in range(cutstart[i],cutend[i],cutstep[i]):
		if what == 'Central':
			cut = cut/20.0
		if what == 'dPhi':
			cut = cut/25.0
		n_bkg=0
		binstart = int(cut / binsize[i] + 1)
		binend=5000 #binstart+1
		n_ttbar = h_ttbar.Integral(binstart,binend)
		n_bjets = h_bjets.Integral(binstart,binend)
		n_tjets = h_tjets.Integral(binstart,binend)
		n_dibos = h_dibos.Integral(binstart,binend)

		n_bkg = n_ttbar+n_bjets+n_tjets+n_dibos


		n_stoponly = h_stc_stoponly.Integral(binstart,binend)
		n_susybkg = h_stc.Integral(binstart,binend)-n_stoponly

		
		stoponly_sig_over_sqrtbkg = 0
		susybkg_sig_over_sqrtbkg = 0
		if (n_bkg != 0):
			stoponly_sig_over_sqrtbkg = n_stoponly/(math.sqrt(n_bkg+n_susybkg)+1)
			susybkg_sig_over_sqrtbkg = n_susybkg/(math.sqrt(n_bkg)+1)
		ratio =0
		if (n_stoponly+n_susybkg != 0):
			ratio  = n_stoponly/(n_stoponly+n_susybkg)
		significance = 0
		if (n_bkg != 0):
			significance = RooStats.NumberCountingUtils.BinomialObsZ(n_susybkg+n_stoponly+n_bkg,n_bkg,0.15)
			#(n_susybkg+n_stoponly)/(math.sqrt(n_bkg + 0.25*0.25*n_bkg*n_bkg))
			
		print binstart	
		print "%.2f %.2f %.2f %.2f  %.2f %.2f %.2f %.2f" % (cut, n_stoponly, n_susybkg, ratio*100, stoponly_sig_over_sqrtbkg, susybkg_sig_over_sqrtbkg, significance, n_bkg)	

		#print stoponly_sig_over_sqrtbkg, susybkg_sig_over_sqrtbkg, significance
	i=i+1
  	#--------------------------------------------------------------------------------------------------------------------------------------------------------
