#!/usr/bin/python

import sys, os
import CMS_lumi , tdrstyle
from ROOT import *
gROOT.SetStyle("Plain");
gStyle.SetOptStat(0);
gROOT.SetBatch(kTRUE);	
#tdrstyle.setTDRStyle()
postfix = "PF_DYMG"

etabins= {0:"Jet |#eta| < 5", 1:"Jet |#eta| < 2.5", 2:"2.5 < Jet |#eta| < 2.75",3:"2.75 < Jet |#eta| < 3", 4:"3 < Jet |#eta| < 5"}
ptbins= {0:"20 < Jet p_{T} < 100", 1:"20 < Jet p_{T} < 30", 2:"30 < Jet p_{T} < 50",3:"50 < Jet p_{T} < 100"}

etabinspdf= {0:"eta_5", 1:"eta_2p5", 2:"2p5_eta_2p75",3:"2p75_eta_3", 4:"3_eta_5"}
ptbinspdf= {0:"20_pt_100", 1:"20_pt_30", 2:"30_pt_50",3:"50_pt_100"}

def getLegend():
	leg = TLegend(0.7282851,0.6096491,0.8875278,0.8874269)
	leg.SetBorderSize(1)
	leg.SetTextFont(62)
	leg.SetTextSize(0.04)
	leg.SetLineColor(0)
	leg.SetLineStyle(1)
	leg.SetLineWidth(1) 
	leg.SetFillColor(0)
	leg.SetFillStyle(1001)          

	return leg
if __name__ == "__main__":  
	hists = []	
	fout = TFile.Open('JetMetPlots_Var_'+postfix+'.root', 'recreate')   

	fdata = TFile.Open('../submit/CMG_data_DoubleMu2015_all_his.root', 'read')   
	fDY = TFile.Open('../submit/CMG_MC_DYMG_all_his.root', 'read')   

	if not os.path.exists("VarPlots"): os.system("mkdir -p VarPlots")
	
	CMS_lumi.writeExtraText = 1
	CMS_lumi.lumi_13TeV = "2.3 fb^{-1}"
	CMS_lumi.lumi_sqrtS = "13 TeV"
	iPos=0                                                                                                        
	Tl = TLatex()
	Tl.SetTextSizePixels(15)
	Tl.SetTextFont(42)
	Tl.SetTextAlign(13);
	ypos = 0.82
	xpos = 0.45                   
	plots = ['mLL_after','npv','nJet20','nJet20clean']
	for i in range (0,2):
		for plot in plots:
			hDY = 	fDY.Get(plot)
			hdata = fdata.Get(plot)
			hDY.SetLineColor(kBlack)
#			hDY.SetFillColor(kBlack)
#			hDY.SetFillStyle(3003)
			sf = 1
			post = ""
			if i ==1:
				sf = hdata.Integral()/hDY.Integral() 
				post = "norm"
			c = TCanvas(plot+post,plot+post, 800, 600)
			hDY.Scale(sf)
			hDY.Draw("hist")
			hDY.SetTitle("")
			hDY.SetLineWidth(2)
			hDY.GetXaxis().SetTitle(plot)
			hdata.SetMarkerStyle(kFullCircle)
			hdata.Draw("same")
			hists.append(hdata)
			hists.append(hDY)
			leg = getLegend()
			leg.AddEntry(hDY,'All','lf')
			leg.AddEntry(hdata,'Data','pe')
			leg.Draw()
			Tl.DrawLatexNDC(xpos,ypos, ptbins[0]) 
			Tl.DrawLatexNDC(xpos,ypos-0.06, etabins[0])
			Tl.DrawLatexNDC(xpos,ypos-0.12, "MC_{SF} = %.2f" % sf)
			fout.cd()
			CMS_lumi.CMS_lumi(c,4, 11)   
			c.Write()

	
	plots = ['hnpv','hPUdisc','hmLL','hDRweighted',	'hnTot','hnCh','haxisMajor','haxisMinor','hfRing0','hfRing1',
		 'hfRing2','hfRing3','hptD','hbetaClassic','hpull','hjetR','hjetRchg','hjtpt','hjteta']

	plotsDict = {'hnpv':'N_{vertex}','hPUdisc':'Pileup Jet MVA','hmLL':'Dilepton mass','hDRweighted':'hDRweighted',
		 'hnTot':'hnTot','hnCh':'hnCh',
		 'haxisMajor':'haxisMajor','haxisMinor':'haxisMinor','hfRing0':'hfRing0','hfRing1':'hfRing1',
		 'hfRing2':'hfRing2','hfRing3':'hfRing3','hptD':'hptD','hbetaClassic':'#beta',
		 'hpull':'Pull','hjetR':'hjetR','hjetRchg':'hjetRchg','hjtpt':'Jet p_{T}','hjteta':'Jet #eta'}

   	for plot in plots:

		c = TCanvas(plot+"_all",plot+"_all", 1200, 900)
		c.Divide(3,4,0.002,0.002)		
		icanvas=1
		for k in range (0,5):
			for j in range (0,4):
				print icanvas
				c2 = TCanvas(plot+str(k)+str(j),plot+str(k)+str(j), 800, 600)  

				
#				print plot + '_0'+'_'+str(k)+'_'+str(j)
				
				h1 = fDY.Get('Variables/'+plot + '_1'+'_'+str(j)+'_'+str(k)).Clone()
				h2 = fDY.Get('Variables/'+plot + '_2'+'_'+str(j)+'_'+str(k)).Clone()
				h3 = fDY.Get('Variables/'+plot + '_3'+'_'+str(j)+'_'+str(k)).Clone()
				h4 = fDY.Get('Variables/'+plot + '_4'+'_'+str(j)+'_'+str(k)).Clone()

 
				hdata = fdata.Get('Variables/'+plot + '_0'+'_'+str(j)+'_'+str(k)).Clone()
				htot = fDY.Get('Variables/'+plot + '_0'+'_'+str(j)+'_'+str(k)).Clone() 

				htotsum = fDY.Get('Variables/'+plot + '_1'+'_'+str(j)+'_'+str(k)).Clone()
				htotsum.Add(h2)
				htotsum.Add(h3)
				htotsum.Add(h4)
				hists.append(htotsum)
				hists.append(h1)
				hists.append(h2)
				hists.append(h3)
				hists.append(h4)
				hists.append(hdata)

				sf = hdata.Integral()/htotsum.Integral()

				print htotsum.Integral(), hdata.Integral(), sf, hdata.Integral()/htotsum.Integral()
				h1.SetLineColor(kRed)
				h2.SetLineColor(kBlue)
				h3.SetLineColor(kGreen)
				h4.SetLineColor(kMagenta)
				htotsum.SetLineColor(kBlack)
				h1.SetLineWidth(2)
				h2.SetLineWidth(2)
				h3.SetLineWidth(2)
				h4.SetLineWidth(2)
				htotsum.SetLineWidth(2)

				h1.SetTitle("Quarks")
				h2.SetTitle("Gluons")
				h3.SetTitle("PU")
				h4.SetTitle("Rest")


#				htotsum.SetLineStyle(2)
				htotsum.SetLineWidth(2)

				
				h1.Scale(sf)
				h2.Scale(sf)
				h3.Scale(sf)
				h4.Scale(sf)
				htot.Scale(sf)
				htotsum.Scale(sf)

#				c.cd(icanvas)				
				c2.cd()

				htotsum.Draw("histe")
				h1.Draw("histsame")
				h2.Draw("histsame")
				h3.Draw("histsame")
				h4.Draw("histsame")
				hdata.SetMarkerStyle(kFullCircle)

				hdata.Draw("samee")

				ymax = float(max(hdata.GetMaximum(), htotsum.GetMaximum()))*1.8
				htotsum.GetYaxis().SetRangeUser(0.01, ymax)
				htotsum.GetXaxis().SetTitleSize(0.055)
				htotsum.GetXaxis().SetTitleOffset(0.8)
				htotsum.GetXaxis().SetTitle(plotsDict[plot])
				leg = getLegend()
				leg.AddEntry(h1)
				leg.AddEntry(h2)
				leg.AddEntry(h3)
				leg.AddEntry(h4)
				leg.AddEntry(htotsum,'All','lf')
				leg.AddEntry(hdata,'Data','pe')
				leg.Draw()

				Tl.DrawLatexNDC(xpos,ypos, ptbins[j]) 
				Tl.DrawLatexNDC(xpos,ypos-0.06, etabins[k])
				Tl.DrawLatexNDC(xpos,ypos-0.12, "MC_{SF} = %.2f" % sf)
				fout.cd()
				CMS_lumi.CMS_lumi(c2,4, 11)   
				if 'beta' in plot:
					c2.SetLogy()
					htotsum.GetYaxis().SetRangeUser(1, 4*ymax)
				c2.SetName("plotsForPAS/"+plot+"_"+etabinspdf[k]+"_"+ptbinspdf[j])
				#if (j == 2 and (k ==1 or k ==4)):
				c2.Write()
				c2.SaveAs("VarPlots/"+plot+"_"+etabinspdf[k]+"_"+ptbinspdf[j]+".pdf")
				if j > 0 and k > 0:
					c.cd(icanvas)
					c.SetLogy()
					h1.SetLineWidth(1)
					h2.SetLineWidth(1)
					h3.SetLineWidth(1)
					h4.SetLineWidth(1)
					htotsum.SetLineWidth(1)
					htotsum.Draw("histe")
					h1.Draw("histsame")
					h2.Draw("histsame")
					h3.Draw("histsame")
					h4.Draw("histsame")
					hdata.SetMarkerSize(0.6)
					hdata.Draw("samee")
					leg2 = getLegend()
					leg2.AddEntry(h1)
					leg2.AddEntry(h2)
					leg2.AddEntry(h3)
					leg2.AddEntry(h4)
					leg2.AddEntry(htotsum,'All','lf')
					leg2.AddEntry(hdata,'Data','pe')
					hists.append(leg2)
					leg2.Draw()
					Tl.DrawLatexNDC(xpos+0.07,ypos, ptbins[j]) 
					Tl.DrawLatexNDC(xpos+0.07,ypos-0.06, etabins[k])
					Tl.DrawLatexNDC(xpos+0.07,ypos-0.12, "MC_{SF} = %.2f" % sf)
					CMS_lumi.CMS_lumi(c.cd(icanvas),4, 11) 
					icanvas = icanvas + 1

		fout.cd()
#		for obj in c.cd(1).GetListOfPrimitives():
#			print obj
		c.Write()
		c.SaveAs("VarPlots/"+plot+".pdf")
		
		#print hists
