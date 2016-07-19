#!/usr/bin/python

import sys, math
import CMS_lumi 
from ROOT import *
gROOT.SetStyle("Plain");
gStyle.SetOptStat(0);
gROOT.SetBatch(kTRUE);	

postfix = "PF_DYMG"

etabins= {0:"Jet |#eta| < 5", 1:"Jet |#eta| < 2.5", 2:"2.5 < Jet |#eta| < 2.75",3:"2.75 < Jet |#eta| < 3", 4:"3 < Jet |#eta| < 5"}
ptbins= {0:"20 < Jet p_{T} < 100", 1:"20 < Jet p_{T} < 30", 2:"30 < Jet p_{T} < 50",3:"50 < Jet p_{T} < 100"}

etabinspdf= {0:"eta_5", 1:"eta_2p5", 2:"2p5_eta_2p75",3:"2p75_eta_3", 4:"3_eta_5"}
ptbinspdf= {0:"20_pt_100", 1:"20_pt_30", 2:"30_pt_50",3:"50_pt_100"}

dummys = []
CMS_lumi.writeExtraText = 0
CMS_lumi.lumi_13TeV = "2.3 fb^{-1}"
CMS_lumi.lumi_sqrtS = "13 TeV"
iPos=0                                                                                                        
Tl = TLatex()                                                                                                          
Tl.SetTextSizePixels(15)                                                                                            
Tl.SetTextFont(42)                                                                                                  
Tl.SetTextAlign(13);                                                                                                
ypos = 0.82                                                                                                         
xpos = 0.45   
def getLegend():
	leg = TLegend(0.6036789,0.2766226,0.7625418,0.5538752)
	leg.SetBorderSize(1)
	leg.SetTextFont(62)
	leg.SetTextSize(0.04)
	leg.SetLineColor(0)
	leg.SetLineStyle(1)
	leg.SetLineWidth(1) 
	leg.SetFillColor(0)
	leg.SetFillStyle(1001)          
	return leg

def makeEffPlots(eff, var ):
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
	dummy = hdata.GetName()
	hdata.SetTitle('')
	hdata.GetYaxis().SetLabelSize(0.04)
	hdata.GetYaxis().SetTitle("Efficiency")

	hdata.GetYaxis().SetTitleSize(0.045)
	hdata.GetYaxis().SetRangeUser(0,1.2)
	hDY.Draw("same")

	hdata.SetLineColor(kBlack)
	hdata.SetMarkerColor(kBlack)
	hdata.SetMarkerStyle(kFullCircle)

	hDY.SetFillColor(kBlue)
	hDY.SetFillStyle(3003)
	hDY.Draw("sameE4")
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
	Tl.DrawLatexNDC(xpos,ypos, 'Loose working point') 
	Tl.DrawLatexNDC(xpos,ypos-0.06, dummy) 
	CMS_lumi.CMS_lumi(p1,4, 2) 
	h_ratio = hdata.Clone()
	h_ratio.Divide(hDY)	
	p2.cd()
	h_ratio.Draw()
	h_ratio.GetXaxis().SetTickLength(0.07)
	h_ratio.GetYaxis().SetRangeUser(0.78,1.22)
	h_ratio.GetYaxis().SetTitle("data/MC")
	h_ratio.GetXaxis().SetTitle("Jet"+var)
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
	fout = TFile.Open('PUEff_'+postfix+'.root', 'recreate')   

	fdata = TFile.Open('../submit/CMG_data_DoubleMu2015_all_his.root', 'read')   
	fDY = TFile.Open('../submit/CMG_MC_DYMG_all_his.root', 'read')
	fDYmatch = TFile.Open('../submit/CMG_MC_DYMG_all_his.root', 'read')
	files = [fdata, fDY,fDYmatch]
	filenames = ['data', 'DY','DYmatch']

	Tl = TLatex()
	Tl.SetTextSizePixels(15)
	Tl.SetTextFont(42)
	Tl.SetTextAlign(13);
	ypos = 0.82
	xpos = 0.45                   
#	wps = ['loose','medium','tight']
	wps = ['loose']
#	variables = ['pT','Eta']
	variables = {'pT':('','_cent','_forw'),'Eta':('','','')}
	SR = 'dPhiGeq2p5'
	CR = 'dPhiLeq1p5'

	for i in range(0,3):
		wp = 'loose'
		for var in variables:
			efficiencies = {}
			for f,fn in zip(files,filenames):
				eta = variables[var][i]
				postfix = ''
				if 'match' in fn:
					postfix = '_QG'
				h_SR_all = f.Get('Jet' + var + '_' + SR + '_All'+postfix+eta)
				h_CR_all = f.Get('Jet' + var + '_' + CR + '_All'+eta)
				h_SR_pass = f.Get('Jet' + var + '_' + SR + '_Pass_' + wp+postfix+eta)
				h_CR_pass = f.Get('Jet' + var + '_' + CR + '_Pass_' + wp+eta)
				h_all = h_SR_all.Clone()
				h_pass = h_SR_pass.Clone()
				#scale background under the assumptio that dphi is flat for pu jets
				k = (math.pi-2.5)/1.5
				h_CR_all.Scale(k)
				h_CR_pass.Scale(k)
				#subtract background from side band
				if 'match' not in fn:
					h_all.Add(h_CR_all,-1)
					h_pass.Add(h_CR_pass,-1)

				h_pass.SetName('hpass_'+var+'_'+wp+fn+eta)
				h_all.SetName('hall_'+var+'_'+wp+fn+eta)
				h_efficiency = h_pass.Clone()
				h_efficiency.Divide(h_pass,h_all,1.0,1.0,"B")
				h_efficiency.SetName('heff_'+var+'_'+wp+fn+eta)
				c = TCanvas('Dist_'+var+'_'+wp+fn+eta,'Dist_'+var+'_'+wp+fn+eta, 800, 600)
				h_all.Draw()
				h_pass.Draw("same")
				c1 = TCanvas('eff_'+var+'_'+wp+fn+eta,'eff_'+var+'_'+wp+fn+eta, 800, 600)
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
			ceff = makeEffPlots(efficiencies, var)
			SetOwnership(ceff, 0)
			ceff.Write()
			

	

