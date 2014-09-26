#include "NtupleTools3.h"

//#include "mt2w_bisect.h"
//#include "mt2w_bisect.cpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>

using namespace std;

void TreeAnalyzer(TString list, TString outname,bool useW=true){


	
	TObjArray* arr = list.Tokenize(" ");
	int size=arr->GetEntries();
	if(size%2!=0) {
        	cout<<"unbalance file/weight list: "<<list<<endl;
        	exit(0);
	}
	vector<TString> files;
	vector<double> weights;
	for(int i=0;i<size;i+=2){
        	files.push_back( arr->At(i)->GetName() );
        	weights.push_back( atof( arr->At(i+1)->GetName() ) );
	}


       	EasyChain* tree = new EasyChain("treeProducerSusySingleLepton");
		
	for(unsigned i=0;i<files.size();i++){
       		tree->AddSmartW(files[i],weights[i]);
		cout<<"add: "<<files[i]<<" "<<weights[i]<<endl;
	}

	int Nevents=tree->GetEntries();
	cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;

	// CutFlow variables
	const int CutNumb = 2;
	const char * CutList[CutNumb] = {"noCut"};
	//top
	double CFCounter[CutNumb];
	int   iCFCounter[CutNumb];

	for (int i=0;i < CutNumb; i++){
	  CFCounter[i] = 0;
	 iCFCounter[i] = 0;
	}
	TH1F* CutFlow= new TH1F("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);
	TH1F *hHT[CutNumb];
	TH1F *h0JetpT[CutNumb];
	TH1F *h1JetpT[CutNumb];
	TH1F *h2JetpT[CutNumb];
	TH1F *h3JetpT[CutNumb];
	TH1I *hnJet[CutNumb];
	TH1F *hnBJet[CutNumb];
	TH1F *hnMu[CutNumb];
	TH1F *hnEl[CutNumb];
	TH1F *hMupt[CutNumb];
	TH1F *hElpt[CutNumb];
	TH1F *hnLep[CutNumb];
	TH1F *hLeppt[CutNumb];
	TH1F *hLepeta[CutNumb];
	TH1F *hCentral[CutNumb];
	TH1F *hMET[CutNumb];
	TH1F *hdPhi[CutNumb];
	TH1F *hmT[CutNumb];
	TH1F *hmT2W[CutNumb];

        for(int cj = 0; cj < CutNumb; cj++)
	  {
	    CutFlow->GetXaxis()->SetBinLabel(cj+1,CutList[cj]);
	    TString cutName=CutList[cj];
	    TString nCut; nCut.Form("%d",cj); 
	    hHT[cj] = new TH1F ("HT_"+nCut,"HT "+cutName,700,0.0,7000.0);
	    hHT[cj]->Sumw2();

	    h0JetpT[cj] = new TH1F ("0JetpT_"+nCut,"0JetpT "+cutName,200,0.0,2000.0);
	    h0JetpT[cj]->Sumw2();
            h1JetpT[cj] = new TH1F ("1JetpT_"+nCut,"1JetpT "+cutName,200,0.0,2000.0);
	    h1JetpT[cj]->Sumw2();
            h2JetpT[cj] = new TH1F ("2JetpT_"+nCut,"2JetpT "+cutName,200,0.0,2000.0);
	    h2JetpT[cj]->Sumw2();
            h3JetpT[cj] = new TH1F ("3JetpT_"+nCut,"3JetpT "+cutName,200,0.0,2000.0);
	    h3JetpT[cj]->Sumw2();
	    
	    hnJet[cj] = new TH1I ("nJet_"+nCut,"nJet "+cutName,20,0,20);
	    hnJet[cj]->Sumw2();
	    hnBJet[cj] = new TH1F ("nBJet_"+nCut,"nBJet "+cutName,20,0,20);
	    hnBJet[cj]->Sumw2();
	    hnMu[cj] = new TH1F ("nMu_"+nCut,"nMu "+cutName,10,0,10);
	    hnMu[cj]->Sumw2();
	    hnEl[cj] = new TH1F ("nEl_"+nCut,"nEl "+cutName,10,0,10);
	    hnEl[cj]->Sumw2();
	    hnLep[cj] = new TH1F ("nLep_"+nCut,"nLep "+cutName,10,0,10);
	    hnLep[cj]->Sumw2();


	    hLeppt[cj] = new TH1F ("LeppT_"+nCut,"Lep pT "+cutName,100,0,1000);
	    hLeppt[cj]->Sumw2();
	    hElpt[cj] = new TH1F ("ElpT_"+nCut,"El pT "+cutName,100,0,1000);
	    hElpt[cj]->Sumw2();
	    hMupt[cj] = new TH1F ("MupT_"+nCut,"Mu pT "+cutName,100,0,1000);
	    hMupt[cj]->Sumw2();

	    hLepeta[cj] = new TH1F ("Lepeta_"+nCut,"Lep eta "+cutName,80,-4,4);
	    hLepeta[cj]->Sumw2();

	    hCentral[cj] = new TH1F ("Central_"+nCut," Central "+cutName,100,0,1);
	    hCentral[cj]->Sumw2();
	    hMET[cj] = new TH1F("MET_"+nCut,"MET "+cutName,200.0,0.0,4000.0);
	    hMET[cj]->Sumw2();
	    hdPhi[cj] = new TH1F("dPhi_"+nCut,"dPhi "+cutName,80,0,3.2);
	    hdPhi[cj]->Sumw2();
	    hmT[cj] = new TH1F("mT_"+nCut,"mT "+cutName,100,0,1000);
	    hmT[cj]->Sumw2();
	    hmT2W[cj] = new TH1F("mT2W_"+nCut,"mT2W "+cutName,70,0,700);
	    hmT2W[cj]->Sumw2();
	  }

	
	//define some cuts
	bool Debug = false;
	Float_t goodElPt = 20.0;
	Float_t goodMuPt = 20.0;
	Float_t goodLepPt = 20.0;
	Float_t vetoLepPt = 10.0;
	Float_t goodJetPt = 10.0;
	Float_t goodEta = 2.4;
	int arrayN = 500;
	Float_t Jet_pt[arrayN]; 
	Float_t Jet_eta[arrayN];
	Float_t Jet_phi[arrayN];
	Float_t Jet_mass[arrayN];
	Float_t Jet_btagCSV[arrayN];

	Float_t LepGood_pt[arrayN]; 
	Float_t LepGood_eta[arrayN];
	Float_t LepGood_phi[arrayN];
	Float_t LepGood_mass[arrayN];
	Float_t LepGood_relIso03[arrayN];
	Int_t LepGood_pdgId[arrayN];
	vector<TLorentzVector> goodJet;
	vector<TLorentzVector> goodLep;
	vector<TLorentzVector> goodEl;
	vector<TLorentzVector> goodMu;
	vector<TLorentzVector> vetoLep;
	TLorentzVector dummy;
	/////////////   event loop   //////////////////////              
	for(int entry=0; entry <  Nevents/*min(100000,Nevents)*/; entry+=1){
	  goodJet.clear();
	  goodLep.clear(); vetoLep.clear();
	  goodEl.clear(); goodMu.clear();

	  //tree->GetEntry(entry); // <<<<<<<<<<< dk
	  Float_t fw = tree->GetEntryW(entry);	  
	  progressT();
 

	  int EvWeight = 1;//fw; //lumi calcualtion done in runAnalyzer.py (fb and pb)
	  /////////////////////////NEW for CMG//////////////////////

	  int nJet = tree->Get(nJet,"nJet");
	  int nJetGood = 0;
	  tree->Get(Jet_pt[0],"Jet_pt");
	  tree->Get(Jet_eta[0],"Jet_eta");
	  tree->Get(Jet_phi[0],"Jet_phi");
	  tree->Get(Jet_mass[0],"Jet_mass");
	  tree->Get(Jet_btagCSV[0],"Jet_btagCSV");

	  int nLep = tree->Get(nLep,"nLepGood");
	  int nLepGood = 0;
	  int nMuGood = 0;
	  int nElGood = 0;
	  tree->Get(LepGood_pt[0],"LepGood_pt");
	  tree->Get(LepGood_eta[0],"LepGood_eta");
	  tree->Get(LepGood_phi[0],"LepGood_phi");
	  tree->Get(LepGood_mass[0],"LepGood_mass");
	  tree->Get(LepGood_relIso03[0],"LepGood_relIso03");
	  tree->Get(LepGood_pdgId[0],"LepGood_pdgId");


	  Float_t MET = tree->Get(MET,"met_pt");
	  
	  ///at some point export this as functions  
	  for(int ilep=0;ilep<nLep;ilep++)
	    {
	      dummy.SetPtEtaPhiM(LepGood_pt[ilep],LepGood_eta[ilep],LepGood_phi[ilep],LepGood_mass[ilep]);
	      if(dummy.Pt() > vetoLepPt && fabs(dummy.Eta()) < goodEta){
		vetoLep.push_back(dummy);
		if( dummy.Pt() > goodLepPt && LepGood_relIso03[ilep] < 0.15){ //TODO: need to adjust isolation
		  goodLep.push_back(dummy);
		  if(abs(LepGood_pdgId[ilep]) == 11){
		    goodEl.push_back(dummy);
		    nElGood++;
		  }
		  if(abs(LepGood_pdgId[ilep]) == 13){
		    goodMu.push_back(dummy);
		    nMuGood++;
		  }
		  nLepGood++;
		}

	      }
	    }
	  for(int ijet=0;ijet<nJet;ijet++)
	    {
	      dummy.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);
	      //put pt, eta, cuts and other stuff
	      //jet are already cleaned from all loose leptons that are in LepGood
	      if(dummy.Pt() > goodJetPt && fabs(dummy.Eta()) < goodEta){
		goodJet.push_back(dummy);
		nJetGood++;
	      }
	    }

	  hnJet[0]->Fill(nJetGood,EvWeight);
	  hnLep[0]->Fill(nLepGood,EvWeight);
	  if (nJet > 0) h0JetpT[0]->Fill(goodJet[0].Pt(),EvWeight);
	  if (nJet > 1) h1JetpT[0]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJet > 2) h2JetpT[0]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJet > 6) h3JetpT[0]->Fill(goodJet[6].Pt(),EvWeight);

	  if (nLepGood > 0) hLeppt[0]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[0]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[0]->Fill(goodEl[0].Pt(),EvWeight);

	  hMET[0]->Fill(MET,EvWeight);

	  CFCounter[0]+= EvWeight;                                                                  
	  iCFCounter[0]++;      

	  	  ////////////////////////////////////////////////////////////
	  /////////////////////////////////////////////////////////

	  	
}/////////////////////////////////////////END HERE
	// ^loop end^


	  


	ofstream tfile;
	TString textfilename = "CMG_cutflow_"+outname+".txt"; 
	tfile.open(textfilename);

	tfile << "########################################" << endl;
	tfile << "Cut efficiency numbers:" << endl;
	for(int ci = 0; ci < CutNumb; ci++)
	{
		tfile << "After cut " << CutList[ci] <<  "\t\t"
		      << CFCounter[ci]  << "\t events left\t"<< iCFCounter[ci] <<" cnt\t"<< endl;
		CutFlow->SetBinContent(1+ci,CFCounter[ci]);
	}
	TFile * outf;
	TString rootfilename = "CMG_"+outname+"_his.root"; 

	outf  = new TFile(rootfilename,"RECREATE");
	
	for(int cj = 0; cj < CutNumb; cj++)
	  {
	    hHT[cj]->Write();
	    h0JetpT[cj]->Write();
	    h1JetpT[cj]->Write();
	    h2JetpT[cj]->Write();
	    h3JetpT[cj]->Write(); 
	    hnJet[cj]->Write();
	    hnBJet[cj]->Write();
	    hnMu[cj]->Write(); 
	    hnEl[cj]->Write(); 
	    hMupt[cj]->Write(); 
	    hElpt[cj]->Write(); 
	    hnLep[cj]->Write(); 
	    hLeppt[cj]->Write(); 
	    hLepeta[cj]->Write(); 
	    hCentral[cj]->Write();
	    hMET[cj]->Write();
	    hdPhi[cj]->Write();
	    hmT[cj]->Write();
	    hmT2W[cj]->Write();
	  }
}

