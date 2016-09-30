#!/usr/bin/python

import sys, math
import CMS_lumi, tdrstyle
from ROOT import *
gROOT.SetStyle("Plain");
gStyle.SetOptStat(0);
gROOT.SetBatch(kTRUE);	


postfix = "PF_DYMG"

etabins= {0:"|#eta| < 5", 1:"|#eta| < 2.5", 2:"2.5 < |#eta| < 2.75",3:"2.75 < |#eta| < 3", 4:"3 < |#eta| < 5"}
ptbins= {0:"20 < p_{T} < 100 GeV", 1:"20 < p_{T} < 30 GeV", 2:"30 < p_{T} < 50 GeV",3:"50 < p_{T} < 100 GeV"}

etabinspdf= {0:"eta_5", 1:"eta_2p5", 2:"2p5_eta_2p75",3:"2p75_eta_3", 4:"3_eta_5"}
ptbinspdf= {0:"20_pt_100", 1:"20_pt_30", 2:"30_pt_50",3:"50_pt_100"}


varnames = {'hJetpT':'Jet p_{T} [GeV]','hJetEta':'Jet #eta'}
dummys = []
CMS_lumi.writeExtraText = 1
CMS_lumi.lumi_13TeV = "2.3 fb^{-1}"
CMS_lumi.lumi_sqrtS = "13 TeV"
iPos=0                                                                                                        
Tl = TLatex()                                                                                                          
Tl.SetTextSizePixels(15)                                                                                            
Tl.SetTextFont(42)                                                                                                  
Tl.SetTextAlign(13);                                                                                                
ypos = 0.88                                                                                                         
xpos = 0.45   
def getLegend():
	leg = TLegend(0.3875839,0.1026148,0.5469799,0.3787879)
	leg.SetBorderSize(1)
	leg.SetTextFont(62)
	leg.SetTextSize(0.04)
	leg.SetLineColor(0)
	leg.SetLineStyle(1)
	leg.SetLineWidth(1) 
	leg.SetFillColor(0)
	leg.SetFillStyle(1001)          
	return leg

def makeEffPlots(eff, plot ,k, j):
	hdata = eff["data"]
	hDY = eff["DY"]
	hDYmatch = eff["DYmatch"]
	c = TCanvas(hdata.GetName()+'_comp',hdata.GetName()+'_comp', 600, 600)
	
	p2 = TPad("pad2","pad2",0,0,1,0.31);                                                                                  
        p2.SetTopMargin(0);                                                                                                   
        p2.SetBottomMargin(0.31);                                                                                             
	p2.SetFillStyle(0);                                                                                       
	p2.Draw();                                                                                                            
        p1 = TPad("pad1","pad1",0,0.31,1,1);                                                                                 
        p1.SetBottomMargin(0.00);                                                                                            
        p1.Draw();                                                                                                          
	
        p1.cd()   
	hdata.Draw()

	hdata.SetTitle('')
	hdata.GetYaxis().SetLabelSize(0.04)
	hdata.GetYaxis().SetTitle("Efficiency")

	hdata.GetYaxis().SetTitleSize(0.045)
	hdata.GetYaxis().SetRangeUser(0.42,1.3)
	hDY.Draw("same")

	hdata.SetLineColor(kBlack)
	hdata.SetMarkerColor(kBlack)
	hdata.SetMarkerStyle(kFullCircle)

	hDY.SetFillColor(kBlue)
	hDY.SetFillStyle(3003)
	hDY.Draw("sameE4")
	hDYmatch.SetLineColor(kBlue)
	hDYmatch.SetLineColor(kBlue)
	hDYmatch.Draw("samee")

	hdata.Draw("same")

	dummys.append(hdata)
	dummys.append(hDY)

	leg = getLegend()
	leg.AddEntry(hDY,"MC","f")
	leg.AddEntry(hDYmatch,"MC gen matched","elp")
	leg.AddEntry(hdata,"Data","lep")
	dummys.append(leg)
	leg.Draw()
	if "loose" in hdata.GetName():
		Tl.DrawLatexNDC(xpos,ypos, 'Loose MVA working point') 
	elif "medium" in hdata.GetName():
		Tl.DrawLatexNDC(xpos,ypos, 'Medium MVA working point') 
	elif "tight" in hdata.GetName():
		Tl.DrawLatexNDC(xpos,ypos, 'Tight MVA working point') 
	Tl.DrawLatexNDC(xpos,ypos-0.06, etabins[k]) 
	Tl.DrawLatexNDC(xpos,ypos-0.12, ptbins[j]) 
	CMS_lumi.CMS_lumi(p1,4, 11) 
	h_ratio = hdata.Clone()
	h_ratio.Divide(hDY)	
	p2.cd()
	p2.SetGridy()
	h_ratio.Draw()
	h_ratio.GetXaxis().SetTickLength(0.07)
	h_ratio.GetYaxis().SetRangeUser(0.82,1.18)
	h_ratio.GetYaxis().SetTitle("data/MC   ")
	
	h_ratio.GetXaxis().SetTitle(varnames[plot])
	h_ratio.GetYaxis().SetLabelSize(0.09)
	h_ratio.GetXaxis().SetLabelSize(0.09)
	h_ratio.GetYaxis().SetTitleSize(0.09)
	h_ratio.GetXaxis().SetTitleSize(0.09)
	h_ratio.GetYaxis().SetTitleOffset(0.4)
	h_ratio.GetYaxis().SetNdivisions(505)
	

	
	dummys.append(h_ratio)

	return c

if __name__ == "__main__":  
	hists = []	
	fout = TFile.Open('PUFakeRate_new'+postfix+'.root', 'recreate')   

	fdata = TFile.Open('../submit/CMG_data_DoubleMu2015_all_his.root', 'read')   
	fDY = TFile.Open('../submit/CMG_MC_DYMG_all_his.root', 'read')
	fDYmatch = TFile.Open('../submit/CMG_MC_DYMG_all_his.root', 'read')
	files = [fdata, fDY,fDYmatch]
	filenames = ['data', 'DY','DYmatch']

	Tl = TLatex()
	Tl.SetTextSizePixels(15)
	Tl.SetTextFont(42)
	Tl.SetTextAlign(13);

	wps = ['loose','medium','tight']
#	wps = ['loose']
#	variables = ['pT','Eta']
	variables = {'pT':('','_cent','_forw'),'Eta':('','','')}
	SR = 'dPhiGeq2p5'
	CR = 'dPhiLeq1p5'

	plots = ['hJetpT','hJetEta']
	etaranges = {'hJetpT':5,'hJetEta':1}
	ptranges = {'hJetpT':1,'hJetEta':4}
	for wp in wps:
		for plot in plots:
			for k in range (0,etaranges[plot],1):
				for j in range (0,ptranges[plot],1):
					efficiencies = {}
					for f,fn in zip(files,filenames):
					#					wp = 'medium'
						print k,j
						binAllFlav = '_0'+'_'+str(j)+'_'+str(k)
						print 'Efficiency/'+plot + '_' + SR + '_All'+binAllFlav
						h_SR_all = f.Get('Efficiency/'+plot + '_' + CR + '_All'+binAllFlav).Clone()

						h_SR_pass = f.Get('Efficiency/'+plot + '_' + CR + '_Pass_' + wp +binAllFlav).Clone()

						h_SR_all_match =  TH1D()
						h_SR_pass_match =  TH1D()
						if 'match' in fn:
							h3 = fDY.Get('Efficiency/'+plot + '_' + CR + '_All'+ '_3'+'_'+str(j)+'_'+str(k)).Clone()
							h_SR_all_match = h3.Clone()
							
							h3 = fDY.Get('Efficiency/'+plot + '_' + CR +  '_Pass_' + wp + '_3'+'_'+str(j)+'_'+str(k)).Clone()
							h_SR_pass_match = h3.Clone()
														
						h_all = h_SR_all.Clone()
						h_pass = h_SR_pass.Clone()
					
						if 'match' in fn:
							h_all = h_SR_all_match.Clone()
							h_pass = h_SR_pass_match.Clone()
							
						h_all.Rebin(2)
						h_pass.Rebin(2)
						binName = '_0'+'_'+etabinspdf[k]+'_'+ptbinspdf[j]
						h_pass.SetName('hpass_'+plot+'_'+wp+fn+binName)
						h_all.SetName('hall_'+plot+'_'+wp+fn+binName)

						h_efficiency = h_pass.Clone()
						
						h_efficiency.Divide(h_pass,h_all,1.0,1.0,"B")
						h_efficiency.SetName('heff_'+plot+'_'+wp+fn+binName)
						c = TCanvas('Dist_'+plot+'_'+wp+fn+binName,'Dist_'+plot+'_'+wp+fn+binName, 800, 600)
						h_all.Draw()
						h_pass.Draw("same")
						c1 = TCanvas('fake_'+plot+'_'+wp+fn+binName,'fake_'+plot+'_'+wp+fn+binName, 800, 600)
						h_efficiency.Draw("")
						hists.append(h_efficiency)
						hists.append(h_pass)
						hists.append(h_all)
						fout.cd()
						c.Write()
						h_efficiency.Write()
						h_pass.Write()
						h_all.Write()

						c1.Write()
						efficiencies[fn] = h_efficiency 
						
					fout.cd()
					ceff = makeEffPlots(efficiencies, plot, k, j)
					SetOwnership(ceff, 0)
					ceff.Write()
					ceff.SaveAs("FakePlots/"+ceff.GetName()+".pdf")

			

	

