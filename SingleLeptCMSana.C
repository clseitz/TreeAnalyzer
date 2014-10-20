#include "NtupleTools3.h"
#include "Objects.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>

using namespace std;

void TreeAnalyzer_ext(TString list, TString outname,bool useW=true){



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

	for(unsigned int i=0; i < files.size(); i++){
       		tree->AddSmartW(files[i], weights[i]);
		cout << "add: " << files[i] << " " << weights[i] << endl;
	}

	int Nevents=tree->GetEntries();
	cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;

	// CutFlow variables
	const int CutNumb = 7;
	const char * CutList[CutNumb] = {"noCut",
					 "== 1 lepton, >= 2 jets",
					 "veto lose leptons",
					 ">= 6 jets",
					 "HT > 500",
					 ">= 1 b jet",
					 "S_T > 250"
					};
	//top
	double CFCounter[CutNumb];
	int   iCFCounter[CutNumb];
	int CutCount = 0; // count the cut number

	for (int i=0;i < CutNumb; i++){
	  CFCounter[i] = 0;
	 iCFCounter[i] = 0;
	}

	TH1F* CutFlow= new TH1F("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);
	TH1F *hHT[CutNumb];
	TH1F *hST[CutNumb];
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
	    hHT[cj] = new TH1F ("HT_"+nCut,"HT "+cutName,200,0.0,4000.0);
	    hHT[cj]->Sumw2();
	    hST[cj] = new TH1F ("ST_"+nCut,"ST "+cutName,200,0.0,4000.0);
	    hST[cj]->Sumw2();
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

	/////////////   event loop   //////////////////////
	for(int entry=0; entry <  Nevents/*min(1000000,Nevents)*/; entry+=1){
//	for(int entry=0; entry <  1000/*min(1000000,Nevents)*/; entry+=1){

	  //tree->GetEntry(entry); // <<<<<<<<<<< dk
	  Float_t fw = tree->GetEntryW(entry);
	  progressT();

	  Float_t EvWeight = fw; //lumi calcualtion done in runAnalyzer.py (fb and pb)

	  /////////////////////////NEW for CMG//////////////////////
	  ST=0;

	  GetLeptons(tree);
	  GetJets(tree);

	  // Reset Cut counter
	  CutCount = 0;
	  // fill histos

	  Float_t MET = tree->Get(MET,"met_pt");
	  if( nLepGood > 0) ST = MET+goodLep[0].Pt();

	  hnJet[CutCount]->Fill(nJetGood,EvWeight);
	  hnLep[CutCount]->Fill(nLepGood,EvWeight);
	  hnMu[CutCount]->Fill(nMuGood,EvWeight);
	  hnEl[CutCount]->Fill(nElGood,EvWeight);
	  if (nJetGood > 0) h0JetpT[CutCount]->Fill(goodJet[0].Pt(),EvWeight);
	  if (nJetGood > 1) h1JetpT[CutCount]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJetGood > 2) h2JetpT[CutCount]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJetGood > 3) h3JetpT[CutCount]->Fill(goodJet[3].Pt(),EvWeight);
	  if (nLepGood > 0) hLeppt[CutCount]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[CutCount]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[CutCount]->Fill(goodEl[0].Pt(),EvWeight);

	  hMET[CutCount]->Fill(MET,EvWeight);
	  hHT[CutCount]->Fill(HT40,EvWeight);
	  hST[CutCount]->Fill(ST,EvWeight);

	  CFCounter[CutCount]+= EvWeight;
	  iCFCounter[CutCount]++;
	  CutCount++;

	  // 1. Cut
	  // Require exactly one good lepton and two jets
	  //
	  if (nMuGood != 1) continue;
	  if (nJetGood < 2) continue; //already a njet > 2 cut applied in the ntuple for 25 GeV jets

	  // fill histos
	  hnJet[CutCount]->Fill(nJetGood,EvWeight);
	  hnLep[CutCount]->Fill(nLepGood,EvWeight);
	  hnMu[CutCount]->Fill(nMuGood,EvWeight);
	  hnEl[CutCount]->Fill(nElGood,EvWeight);
	  if (nJetGood > 0) h0JetpT[CutCount]->Fill(goodJet[0].Pt(),EvWeight);
	  if (nJetGood > 1) h1JetpT[CutCount]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJetGood > 2) h2JetpT[CutCount]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJetGood > 3) h3JetpT[CutCount]->Fill(goodJet[3].Pt(),EvWeight);
	  if (nLepGood > 0) hLeppt[CutCount]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[CutCount]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[CutCount]->Fill(goodEl[0].Pt(),EvWeight);

	  hMET[CutCount]->Fill(MET,EvWeight);
	  hHT[CutCount]->Fill(HT40,EvWeight);
	  hST[CutCount]->Fill(ST,EvWeight);

	  CFCounter[CutCount]+= EvWeight;
	  iCFCounter[CutCount]++;
	  CutCount++;

	  // 2. Cut
	  // Veto lose leptons
	  //
	  if (nLepVeto > 0) continue;

	  // fill histos
	  hnJet[CutCount]->Fill(nJetGood,EvWeight);
	  hnLep[CutCount]->Fill(nLepGood,EvWeight);
	  hnMu[CutCount]->Fill(nMuGood,EvWeight);
	  hnEl[CutCount]->Fill(nElGood,EvWeight);
	  if (nJetGood > 0) h0JetpT[CutCount]->Fill(goodJet[0].Pt(),EvWeight);
	  if (nJetGood > 1) h1JetpT[CutCount]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJetGood > 2) h2JetpT[CutCount]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJetGood > 3) h3JetpT[CutCount]->Fill(goodJet[3].Pt(),EvWeight);
	  if (nLepGood > 0) hLeppt[CutCount]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[CutCount]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[CutCount]->Fill(goodEl[0].Pt(),EvWeight);

	  hMET[CutCount]->Fill(MET,EvWeight);
	  hHT[CutCount]->Fill(HT40,EvWeight);
	  hST[CutCount]->Fill(ST,EvWeight);

	  CFCounter[CutCount]+= EvWeight;
	  iCFCounter[CutCount]++;
	  CutCount++;


	  // 3. Cut
	  // Require at least 6 good jets
	  //
	  if (nJetGood < 6) continue;

	  // fill histos
	  hnJet[CutCount]->Fill(nJetGood,EvWeight);
	  hnLep[CutCount]->Fill(nLepGood,EvWeight);
	  hnMu[CutCount]->Fill(nMuGood,EvWeight);
	  hnEl[CutCount]->Fill(nElGood,EvWeight);
	  if (nJetGood > 0) h0JetpT[CutCount]->Fill(goodJet[0].Pt(),EvWeight);
	  if (nJetGood > 1) h1JetpT[CutCount]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJetGood > 2) h2JetpT[CutCount]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJetGood > 3) h3JetpT[CutCount]->Fill(goodJet[3].Pt(),EvWeight);
	  if (nLepGood > 0) hLeppt[CutCount]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[CutCount]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[CutCount]->Fill(goodEl[0].Pt(),EvWeight);

	  hMET[CutCount]->Fill(MET,EvWeight);
	  hHT[CutCount]->Fill(HT40,EvWeight);
	  hST[CutCount]->Fill(ST,EvWeight);

	  CFCounter[CutCount]+= EvWeight;
	  iCFCounter[CutCount]++;
	  CutCount++;

	  // 4. Cut
	  // Require HT >= 500
	  //
	  if (HT40 < 500) continue;

	  // fill histos
	  hnJet[CutCount]->Fill(nJetGood,EvWeight);
	  hnLep[CutCount]->Fill(nLepGood,EvWeight);
	  hnMu[CutCount]->Fill(nMuGood,EvWeight);
	  hnEl[CutCount]->Fill(nElGood,EvWeight);
	  if (nJetGood > 0) h0JetpT[CutCount]->Fill(goodJet[0].Pt(),EvWeight);
	  if (nJetGood > 1) h1JetpT[CutCount]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJetGood > 2) h2JetpT[CutCount]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJetGood > 3) h3JetpT[CutCount]->Fill(goodJet[3].Pt(),EvWeight);
	  if (nLepGood > 0) hLeppt[CutCount]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[CutCount]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[CutCount]->Fill(goodEl[0].Pt(),EvWeight);

	  hMET[CutCount]->Fill(MET,EvWeight);
	  hHT[CutCount]->Fill(HT40,EvWeight);
	  hST[CutCount]->Fill(ST,EvWeight);

	  CFCounter[CutCount]+= EvWeight;
	  iCFCounter[CutCount]++;
	  CutCount++;

	  // 5. Cut
	  // Require at least 1 b tagged jet
	  //
	  if (nBJetGood < 1) continue;

	  // fill histos
	  hnJet[CutCount]->Fill(nJetGood,EvWeight);
	  hnLep[CutCount]->Fill(nLepGood,EvWeight);
	  hnMu[CutCount]->Fill(nMuGood,EvWeight);
	  hnEl[CutCount]->Fill(nElGood,EvWeight);
	  if (nJetGood > 0) h0JetpT[CutCount]->Fill(goodJet[0].Pt(),EvWeight);
	  if (nJetGood > 1) h1JetpT[CutCount]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJetGood > 2) h2JetpT[CutCount]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJetGood > 3) h3JetpT[CutCount]->Fill(goodJet[3].Pt(),EvWeight);
	  if (nLepGood > 0) hLeppt[CutCount]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[CutCount]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[CutCount]->Fill(goodEl[0].Pt(),EvWeight);

	  hMET[CutCount]->Fill(MET,EvWeight);
	  hHT[CutCount]->Fill(HT40,EvWeight);
	  hST[CutCount]->Fill(ST,EvWeight);

	  CFCounter[CutCount]+= EvWeight;
	  iCFCounter[CutCount]++;
	  CutCount++;

	  // 6. Cut
	  // Require S_T > 250
	  //
	  if (ST < 250) continue;

	  // fill histos
	  hnJet[CutCount]->Fill(nJetGood,EvWeight);
	  hnLep[CutCount]->Fill(nLepGood,EvWeight);
	  hnMu[CutCount]->Fill(nMuGood,EvWeight);
	  hnEl[CutCount]->Fill(nElGood,EvWeight);
	  if (nJetGood > 0) h0JetpT[CutCount]->Fill(goodJet[0].Pt(),EvWeight);
	  if (nJetGood > 1) h1JetpT[CutCount]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJetGood > 2) h2JetpT[CutCount]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJetGood > 3) h3JetpT[CutCount]->Fill(goodJet[3].Pt(),EvWeight);
	  if (nLepGood > 0) hLeppt[CutCount]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[CutCount]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[CutCount]->Fill(goodEl[0].Pt(),EvWeight);

	  hMET[CutCount]->Fill(MET,EvWeight);
	  hHT[CutCount]->Fill(HT40,EvWeight);
	  hST[CutCount]->Fill(ST,EvWeight);

	  CFCounter[CutCount]+= EvWeight;
	  iCFCounter[CutCount]++;
	  CutCount++;


	/////////////////////////////////////////////////////////
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
	    hST[cj]->Write();
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

	tfile.close();
	outf->Close();
}
