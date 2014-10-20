#include "../interface/NtupleTools3.h"

//#include "mt2w_bisect.h"
//#include "mt2w_bisect.cpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
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
	const int CutNumb = 30;

	const char * CutList[CutNumb] = {"noCut",
					 "== 1 Mu", "6Jet","HT>500","ST>250",
                                         "Nb>=1","Df<1","Df>1",
                                         "Nb=1, 250<ST<350,Df<1","Nb =1, 250<ST<350,Df>1",
                                         "Nb=1, 350<ST<450, Df<1","Nb =1, 350<ST<450,Df>1",
                                         "Nb=1, ST>450, Df<1","Nb =1, ST>450,Df>1",
                                         "Nb=2 ,250<ST<350, Df<1","Nb=2, 250<ST< 350,Df>1",
                                         "Nb=2 ,350<ST<450,Df<1","Nb=2,350<ST<450,Df>1",
                                         "Nb=2 ,ST>450, Df<1","Nb=2 ,ST>450,Df>1",
                                         "Nb>2 250<ST<350 ,Df<1","Nb>2,250<ST<350,Df>1",
                                         "Nb>2 350<ST<450,Df<1","Nb>2,350<ST<450, Df>1",
                                         "Nb>2 ST>450,Df<1","Nb>2 ST>450,Df>1",

	                                };
	//top
	double CFCounter[CutNumb];
	int   iCFCounter[CutNumb];

	for (int i=0;i < CutNumb; i++){
	  CFCounter[i] = 0;
	 iCFCounter[i] = 0;
	}
     
        string outnameStr = (string)outname;
        string TTbarModes[2] = {"MC_TTbar_DiLep","MC_TTbar_SinLep"};


         double xbinNJ[5] ={3,5,6,7,20};
         double xbinNJn[3] ={3,6,12};
         double xbinHT[4] ={500,750,1000,3000};
         double xbinST[4] ={250,350,450,2000};
	TH1F* CutFlow= new TH1F("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);
	TH1F *hHT[CutNumb];
	TH1F *hST[CutNumb];
	TH1F *hWpT[CutNumb];
	TH1F *h0JetpT[CutNumb];
	TH1F *h1JetpT[CutNumb];
	TH1F *h2JetpT[CutNumb];
	TH1F *h3JetpT[CutNumb];
	TH1F *hnJet[CutNumb];
	TH1F *hnBJet[CutNumb];
	TH1F *hnMu[CutNumb];
	TH1F *hnEl[CutNumb];
	TH1F *hMupt[CutNumb];
	TH1F *hngenLep[CutNumb];
	TH1F *hngenTau[CutNumb];
	TH1F *hngenLepFromTau[CutNumb];
	TH1F *hElpt[CutNumb];
	TH1F *hnLep[CutNumb];
	TH1F *hLeppt[CutNumb];
	TH1F *hLepeta[CutNumb];
	TH1F *hCentral[CutNumb];
	TH1F *hMET[CutNumb];
	TH1F *hdPhi[CutNumb];
	TH1F *hdPhiWLep[CutNumb];
	TH1F *hdPhiWgenLepC[CutNumb];
	TH1F *hdPhiWgenLepS[CutNumb];
	TH1F *hmT[CutNumb];
	TH1F *hmT2W[CutNumb];
	TH1F *h0bJetpT[CutNumb];
	TH1F *h1bJetpT[CutNumb];
	TH1F *h2bJetpT[CutNumb];
	TH1F *h3bJetpT[CutNumb];
	TH1F *hRS[CutNumb];
	TH1F *hRC[CutNumb];
	TH1F *hHTRS[CutNumb];
	TH1F *hHTRC[CutNumb];
	TH1F *hSTRS[CutNumb];
	TH1F *hSTRC[CutNumb];
	TH1F *hNJRS[CutNumb];
	TH1F *hNJRC[CutNumb];
	TH2F *hDfST[CutNumb];
	TH2F *hDfHT[CutNumb];
	TH2F *hHTST[CutNumb];
	TH2F *hNJST[CutNumb];
	TH2F *hMTMET[CutNumb];
	TH2F *hDfDfGen[CutNumb];

        for(int cj = 0; cj < CutNumb; cj++)
	  {
	    CutFlow->GetXaxis()->SetBinLabel(cj+1,CutList[cj]);
	    TString cutName=CutList[cj];
	    TString nCut; nCut.Form("%d",cj); 
	    hHT[cj] = new TH1F ("HT_"+nCut,"HT "+cutName,400,0.0,4000.0);
	//    hHT[cj]->Sumw2();
	    hST[cj] = new TH1F ("ST_"+nCut,"ST "+cutName,400,0.0,4000.0);
	  //  hST[cj]->Sumw2();
	    h0JetpT[cj] = new TH1F ("0JetpT_"+nCut,"0JetpT "+cutName,200,0.0,2000.0);
	    //h0JetpT[cj]->Sumw2();
            h1JetpT[cj] = new TH1F ("1JetpT_"+nCut,"1JetpT "+cutName,200,0.0,2000.0);
	    //h1JetpT[cj]->Sumw2();
            h2JetpT[cj] = new TH1F ("2JetpT_"+nCut,"2JetpT "+cutName,200,0.0,2000.0);
	    //h2JetpT[cj]->Sumw2();
            h3JetpT[cj] = new TH1F ("3JetpT_"+nCut,"3JetpT "+cutName,200,0.0,2000.0);
	    //h3JetpT[cj]->Sumw2();

	    h0bJetpT[cj] = new TH1F ("0bJetpT_"+nCut,"0bJetpT "+cutName,200,0.0,2000.0);
	    //h0bJetpT[cj]->Sumw2();
            h1bJetpT[cj] = new TH1F ("1bJetpT_"+nCut,"1bJetpT "+cutName,200,0.0,2000.0);
	    //h1bJetpT[cj]->Sumw2();
            h2bJetpT[cj] = new TH1F ("2bJetpT_"+nCut,"2bJetpT "+cutName,200,0.0,2000.0);
	    //h2bJetpT[cj]->Sumw2();
            h3bJetpT[cj] = new TH1F ("3bJetpT_"+nCut,"3bJetpT "+cutName,200,0.0,2000.0);
	    //h3bJetpT[cj]->Sumw2();
	    
	    hnJet[cj] = new TH1F ("nJet_"+nCut,"nJet "+cutName,20,0,20);
	    //hnJet[cj]->Sumw2();
	    hnBJet[cj] = new TH1F ("nBJet_"+nCut,"nBJet "+cutName,20,0,20);
	    //hnBJet[cj]->Sumw2();
	    hnMu[cj] = new TH1F ("nMu_"+nCut,"nMu "+cutName,10,0,10);
	    //hnMu[cj]->Sumw2();
	    hnEl[cj] = new TH1F ("nEl_"+nCut,"nEl "+cutName,10,0,10);
	    //hnEl[cj]->Sumw2();
	    hnLep[cj] = new TH1F ("nLep_"+nCut,"nLep "+cutName,10,0,10);
	    //hnLep[cj]->Sumw2();

	    hngenLep[cj] = new TH1F ("ngenLep_"+nCut,"nGenLep "+cutName,10,0,10);
	    //hngenLep[cj]->Sumw2();

	    hngenTau[cj] = new TH1F ("ngenTau_"+nCut,"nGenTau "+cutName,10,0,10);
	    //hngenTau[cj]->Sumw2();
	    hngenLepFromTau[cj] = new TH1F ("ngenLepFromTau_"+nCut,"nGenLepFromTau "+cutName,10,0,10);
	    //hngenLepFromTau[cj]->Sumw2();
            
	    hRS[cj] = new TH1F ("RS_"+nCut,"RS "+cutName,20,0,20);
            hRS[cj]->Sumw2();
	    hRC[cj] = new TH1F ("RC_"+nCut,"RC "+cutName,20,0,20);
	    hRC[cj]->Sumw2();
            

	    hNJRS[cj] = new TH1F ("NJRS_"+nCut,"NJRS "+cutName,4,xbinNJ);
            hNJRS[cj]->Sumw2();
	    hNJRC[cj] = new TH1F ("NJRC_"+nCut,"NJRC "+cutName,4,xbinNJ);
            hNJRC[cj]->Sumw2();

	    hHTRS[cj] = new TH1F ("HTRS_"+nCut,"HTRS "+cutName,3,xbinHT);
            hHTRS[cj]->Sumw2();
	    hHTRC[cj] = new TH1F ("HTRC_"+nCut,"HTRC "+cutName,3,xbinHT);
            hHTRC[cj]->Sumw2();

	    hSTRS[cj] = new TH1F ("STRS_"+nCut,"STRS "+cutName,3,xbinST);
            hSTRS[cj]->Sumw2();
	    hSTRC[cj] = new TH1F ("STRC_"+nCut,"STRC "+cutName,3,xbinST);
            hSTRC[cj]->Sumw2();

	    hWpT[cj] = new TH1F ("WpT_"+nCut,"WpT "+cutName,200,0.0,2000.0);
	    //hWpT[cj]->Sumw2();

	    hLeppt[cj] = new TH1F ("LeppT_"+nCut,"Lep pT "+cutName,100,0,1000);
	    //hLeppt[cj]->Sumw2();
	    hElpt[cj] = new TH1F ("ElpT_"+nCut,"El pT "+cutName,100,0,1000);
	    //hElpt[cj]->Sumw2();
	    hMupt[cj] = new TH1F ("MupT_"+nCut,"Mu pT "+cutName,100,0,1000);
	    //hMupt[cj]->Sumw2();

	    hLepeta[cj] = new TH1F ("Lepeta_"+nCut,"Lep eta "+cutName,80,-4,4);
	    //hLepeta[cj]->Sumw2();

	    hCentral[cj] = new TH1F ("Central_"+nCut," Central "+cutName,100,0,1);
	    hCentral[cj]->Sumw2();
	    hMET[cj] = new TH1F("MET_"+nCut,"MET "+cutName,200.0,0.0,4000.0);
	    //hMET[cj]->Sumw2();
	    hdPhi[cj] = new TH1F("dPhi_"+nCut,"dPhi "+cutName,80,0,3.2);
	    //hdPhi[cj]->Sumw2();
	    hdPhiWLep[cj] = new TH1F("dPhiWLep_"+nCut,"dPhiWLep "+cutName,64,0.0,3.2);
	    //hdPhiWLep[cj]->Sumw2();
	    hdPhiWgenLepC[cj] = new TH1F("dPhiWgenLepC_"+nCut,"dPhiWGenLepC "+cutName,64,0.0,3.2);
	    //hdPhiWgenLepC[cj]->Sumw2();
	    hdPhiWgenLepS[cj] = new TH1F("dPhiWgenLepS_"+nCut,"dPhiWGenLepS "+cutName,64,0.0,3.2);
	    //hdPhiWgenLepS[cj]->Sumw2();
	    hmT[cj] = new TH1F("mT_"+nCut,"mT "+cutName,100,0,1000);
	    //hmT[cj]->Sumw2();
	    hmT2W[cj] = new TH1F("mT2W_"+nCut,"mT2W "+cutName,70,0,700);
	    //hmT2W[cj]->Sumw2();
	    hDfST[cj] = new TH2F ("DfST_"+nCut,"DfST "+cutName,200,0.0,2000.0,64,0.0,3.2);
	    hDfST[cj]->Sumw2();
	    hDfDfGen[cj] = new TH2F ("DfDfGen_"+nCut,"DfDfGen "+cutName,64,0.0,3.2,64,0.0,3.2);
	    hDfDfGen[cj]->Sumw2();
	    hNJST[cj] = new TH2F ("NJST_"+nCut,"NJST "+cutName,20,0,20,200,0.0,2000.0);
	    hNJST[cj]->Sumw2();
	    hDfHT[cj] = new TH2F ("DfST_"+nCut,"DfST "+cutName,300,0.0,3000.0,64,0.0,3.2);
	    hDfHT[cj]->Sumw2();
        
	    hMTMET[cj] = new TH2F ("MTMET_"+nCut,"MTMET "+cutName,200,0.0,2000.0,200,0.0,2000);
	    hMTMET[cj]->Sumw2();
	    hHTST[cj] = new TH2F ("HTST_"+nCut,"HTST "+cutName,300,0.0,3000.0,200,0.0,2000);
	    hHTST[cj]->Sumw2();

	  }

	
	//define some cuts
	bool Debug = false;
	Float_t goodElPt = 20.0;
	Float_t goodMuPt = 20.0;
	Float_t goodLepPt = 20.0;
	Float_t vetoLepPt = 10.0;
	Float_t goodJetPt = 40.0;
	Float_t goodEta = 2.4;
	int arrayN = 50;
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
        Int_t LepGood_tightId[arrayN];
	Int_t LepGood_pdgId[arrayN];
        Int_t ngenTau;
        Int_t ngenLep;
        Int_t ngenLepFromTau;
        
        Float_t genTau_pt[2]; //[ngenLep]
        Float_t genTau_eta[2]; //[ngenLep]
        Float_t genTau_phi[2]; //[ngenLep]
        Int_t genTau_pdgId[2]; //[ngenLep]
        Float_t genLep_pt[2]; //[ngenLep]
        Float_t genLep_eta[2]; //[ngenLep]
        Float_t genLep_phi[2]; //[ngenLep]
        Int_t genLep_pdgId[2]; //[ngenLep]
        Float_t genLep_charge[2]; //[ngenLep]
        Float_t genLepFromTau_pt[2]; //[ngenLepFromTau]
        Float_t genLepFromTau_eta[2]; //[ngenLepFromTau]
        Float_t genLepFromTau_phi[2]; //[ngenLepFromTau]
        Int_t genLepFromTau_pdgId[2]; //[ngenLepFromTau]
        Float_t genLepFromTau_charge[2]; //[ngenLepFromTau]
         
	vector<TLorentzVector> goodJet;
	vector<TLorentzVector> goodbJet;
	vector<TLorentzVector> goodLep;
	vector<TLorentzVector> goodEl;
	vector<TLorentzVector> goodMu;
	//vector<TLorentzVector> WBos;
	vector<TLorentzVector> vetoLep;
//	TLorentzVector dummy;
	/////////////   event loop   //////////////////////              
	for(int entry=0; entry <  Nevents/*min(100000,Nevents)*/; entry+=1){
	  goodJet.clear();
	  goodbJet.clear();
	  goodLep.clear(); vetoLep.clear();
	  goodEl.clear(); goodMu.clear(); //WBos.clear();

	  //tree->GetEntry(entry); // <<<<<<<<<<< dk
	  Float_t fw = tree->GetEntryW(entry);	  
	  progressT();
 
            float EvWeight = 1;
                EvWeight *= fw ;

//1;//fw; //lumi calcualtion done in runAnalyzer.py (fb and pb)
	  /////////////////////////NEW for CMG//////////////////////
	  Float_t HT40=0;
	  Float_t ST=0;
	  int nJetGood = 0;
	  int nbJetGood = 0;
	  int nJet = tree->Get(nJet,"nJet");
	  tree->Get(Jet_pt[0],"Jet_pt");
	  tree->Get(Jet_eta[0],"Jet_eta");
	  tree->Get(Jet_phi[0],"Jet_phi");
	  tree->Get(Jet_mass[0],"Jet_mass");
	  tree->Get(Jet_btagCSV[0],"Jet_btagCSV");

	  int nLep = tree->Get(nLep,"nLepGood");
	  int nLepGood = 0;
	  int nElVeto = 0;
	  int nMuGood = 0;
	  int nMuVeto = 0;
	  int nElGood = 0;
	  tree->Get(ngenLep,"ngenLep"); //n prompt Lep
	  tree->Get(ngenTau,"ngenTau");// gen Tau
	  tree->Get(ngenLepFromTau,"ngenLepFromTau");// Lep from Tau decay
         // For Gen Level distributions
	  tree->Get(LepGood_pt[0],"LepGood_pt");
	  tree->Get(LepGood_eta[0],"LepGood_eta");
	  tree->Get(LepGood_phi[0],"LepGood_phi");
	  tree->Get(LepGood_mass[0],"LepGood_mass");
	  tree->Get(LepGood_pdgId[0],"LepGood_pdgId");
	  tree->Get(LepGood_relIso03[0],"LepGood_relIso03");
	  tree->Get(LepGood_tightId[0],"LepGood_tightId");
	  tree->Get(genLep_pt[0],"genLep_pt");
	  tree->Get(genLep_eta[0],"genLep_eta");
	  tree->Get(genLep_phi[0],"genLep_phi");
	  tree->Get(genLep_pdgId[0],"genLep_pdgId");
	  tree->Get(genLep_charge[0],"genLep_charge");
	  tree->Get(genLepFromTau_pt[0],"genLepFromTau_pt");
	  tree->Get(genLepFromTau_eta[0],"genLepFromTau_eta");
	  tree->Get(genLepFromTau_phi[0],"genLepFromTau_phi");
	  tree->Get(genLepFromTau_charge[0],"genLepFromTau_charge");
	  tree->Get(genLepFromTau_pdgId[0],"genLepFromTau_pdgId");
	  tree->Get(genTau_pt[0],"genTau_pt");
	  tree->Get(genTau_eta[0],"genTau_eta");
	  tree->Get(genTau_phi[0],"genTau_phi");
	  tree->Get(genTau_pdgId[0],"genTau_pdgId");
           

	  Float_t MET = tree->Get(MET,"met_pt");
        Float_t met_eta = tree->Get(met_eta,"met_eta");
        Float_t met_phi = tree->Get(met_phi,"met_phi");
	  Float_t MET_genpt = tree->Get(MET_genpt,"met_genPt");
        Float_t met_geneta = tree->Get(met_geneta,"met_genEta");
        Float_t met_genphi = tree->Get(met_genphi,"met_genPhi");
	    
            
           bool TwoLep = false;//Top decay modes
          if((ngenLep+ngenLepFromTau ==2) ||(ngenLep ==1 && ngenLepFromTau ==0 && ngenTau ==1) ||(ngenLep ==0 && ngenLepFromTau ==1 && ngenTau ==2)) TwoLep = true;
          //if((ngenLep+ngenLepFromTau ==2)) TwoLep = true;
           if (outnameStr.compare(TTbarModes[0]) ==0 ){
          if(!(TwoLep)) //DiLep = true;
          continue;}
                 if(outnameStr.compare(TTbarModes[1]) == 0){
                        if(TwoLep) continue;
                }
               //cout<<TwoLep<<endl;
	  for(int ilep=0;ilep<nLep;ilep++)
	    {
	      TLorentzVector dummy; dummy.SetPtEtaPhiM(LepGood_pt[ilep],LepGood_eta[ilep],LepGood_phi[ilep],LepGood_mass[ilep]);
	      if(dummy.Pt() > vetoLepPt && abs(LepGood_pdgId[ilep]) == 11){
		nElVeto++;}
		if( dummy.Pt() > goodLepPt &&  LepGood_tightId[ilep] ){ //TODO: need to adjust isolation
		  goodLep.push_back(dummy);
		  if(abs(LepGood_pdgId[ilep]) == 11 && LepGood_relIso03[ilep] < 0.14){
		    goodEl.push_back(dummy);
		    nElGood++;
		  }}
		  if(abs(LepGood_pdgId[ilep]) == 13){if(dummy.Pt() > vetoLepPt)  {
                    if( dummy.Pt() > goodLepPt &&  LepGood_tightId[ilep] ){
                    if  (LepGood_tightId[ilep] && LepGood_relIso03[ilep] < 0.12){
		    goodMu.push_back(dummy);
		    nMuGood++;
		  }
                 }else nMuVeto++;
                }
		}
		
	      }
	  for(int ijet=0;ijet<nJet;ijet++)
	    {
	      TLorentzVector dummy; dummy.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);
	      //put pt, eta, cuts and other stuff
	      //jet are already cleaned from all loose leptons that are in LepGood
	      if(dummy.Pt() > goodJetPt && fabs(dummy.Eta()) < goodEta){
		goodJet.push_back(dummy);
		nJetGood++;
		HT40 +=   Jet_pt[ijet]; //something buggy for HT, distribution looks strang?
                 if(Jet_btagCSV[ijet] > 0.679)
                  {
                    goodbJet.push_back(dummy);
		     nbJetGood++;

                  }
	      }
	    }
	  if( nMuGood > 0) ST = MET+goodMu[0].Pt();
	  //hnJet[0]->Fill(nJetGood,EvWeight);
	  //hnLep[0]->Fill(nLepGood,EvWeight);
	  //hnMu[0]->Fill(nMuGood,EvWeight);
	  hnEl[0]->Fill(nElGood,EvWeight);
	  if (nJetGood > 0) h0JetpT[0]->Fill(goodJet[0].Pt(),EvWeight);
	  if (nJetGood > 1) h1JetpT[0]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJetGood > 2) h2JetpT[0]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJetGood > 3) h3JetpT[0]->Fill(goodJet[3].Pt(),EvWeight);
	  if (nLepGood > 0) hLeppt[0]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[0]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[0]->Fill(goodEl[0].Pt(),EvWeight);

	  hMET[0]->Fill(MET,EvWeight);
	  hHT[0]->Fill(HT40,EvWeight);
	  hST[0]->Fill(ST,EvWeight);
	  hngenLep[0]->Fill(ngenLep,EvWeight);
	  hngenTau[0]->Fill(ngenTau,EvWeight);
	  hngenLepFromTau[0]->Fill(ngenLepFromTau,EvWeight);
         // if(ngenLepFromTau ==1)cout<<"nTau:"<<nGenTau<<endl;
          //if(ngenLepFromTau ==2)cout<<"nTau:"<<nGenTau<<endl;
	  CFCounter[0]+= EvWeight;                                                                  
	  iCFCounter[0]++;      
	  //////////////////Require exactly one good lepton and one jet (maybe change later)
	  //, need to implement loose veto//////////
	  if (nMuGood != 1) continue;
           if(nMuVeto !=0 || nElVeto !=0)continue;
	  if (nJetGood < 2) continue;
                //float DelPhiMetMu = TMath::ACos(TMath::Cos(met_phi - goodMu[0].Phi()));
                float DelPhiMetMu = fabs(met_phi - goodMu[0].Phi());
                if (DelPhiMetMu > acos(-1.0)) DelPhiMetMu -= 2*acos(-1.0);
                //if (DelPhiMetMu < -TMath::Pi()) DelPhiMetMu += 2*TMath::Pi();

//                float Wpt = TMath::Sqrt( TMath::Power(MET,2) + TMath::Power(goodMu[0].Pt(),2) + 2*goodMu[0].Pt()*MET*TMath::Cos(DelPhiMetMu));
	       TLorentzVector METV;  METV.SetPtEtaPhiM(MET,0.0,met_phi,0.0);
	       TLorentzVector WBos;  WBos = METV+goodMu[0];
                float  DelPhiWlep = (WBos.Phi() - goodMu[0].Phi());
                     if (DelPhiWlep > TMath::Pi()) DelPhiWlep -= 2*TMath::Pi();
                     if (DelPhiWlep <= -TMath::Pi()) DelPhiWlep += 2*TMath::Pi();
                float Wpt = WBos.Pt();
                float Wphi = WBos.Phi();
                float MT =sqrt(pow((goodMu[0].Et()+METV.Et()),2)-pow((goodMu[0].Px()+METV.Px()),2)-pow((goodMu[0].Py()+METV.Py()),2));
	  hnJet[1]->Fill(nJetGood,EvWeight);
	  hnBJet[1]->Fill(nbJetGood,EvWeight);
	  hnLep[1]->Fill(nLepGood,EvWeight);
	  hnMu[1]->Fill(nMuGood,EvWeight);
	  hnEl[1]->Fill(nElGood,EvWeight);
	   h0JetpT[1]->Fill(goodJet[0].Pt(),EvWeight);
	   h1JetpT[1]->Fill(goodJet[1].Pt(),EvWeight);
	  if (nJetGood > 2) h2JetpT[1]->Fill(goodJet[2].Pt(),EvWeight);
	  if (nJetGood > 3) h3JetpT[1]->Fill(goodJet[3].Pt(),EvWeight);
	  if (nLepGood > 0) hLeppt[1]->Fill(goodLep[0].Pt(),EvWeight);
	  if (nMuGood > 0) hMupt[1]->Fill(goodMu[0].Pt(),EvWeight);
	  if (nElGood > 0) hElpt[1]->Fill(goodEl[0].Pt(),EvWeight);
	  if (nbJetGood > 0) h0bJetpT[0]->Fill(goodbJet[0].Pt(),EvWeight);
	  if (nbJetGood > 1) h1bJetpT[0]->Fill(goodbJet[1].Pt(),EvWeight);
	  if (nbJetGood > 2) h2bJetpT[0]->Fill(goodbJet[2].Pt(),EvWeight);
	  if (nbJetGood > 3) h3bJetpT[0]->Fill(goodbJet[3].Pt(),EvWeight);

	  hMET[1]->Fill(MET,EvWeight);
	  hnBJet[1]->Fill(nbJetGood,EvWeight);
	  hHT[1]->Fill(HT40,EvWeight);
	  hST[1]->Fill(ST,EvWeight);
	  hWpT[1]->Fill(Wpt,EvWeight);
	  hdPhiWLep[1]->Fill(fabs(DelPhiWlep),EvWeight);
	  hdPhi[1]->Fill(DelPhiMetMu,EvWeight);

	  CFCounter[1]+= EvWeight;                                                                  
	  iCFCounter[1]++;

       if (nJetGood < 6) continue;
       //if (!(nJetGood >3 && nJetGood <6)) continue;
       //if (nJetGood < 3) continue; //control sample or new search bin?
      
	  hnBJet[2]->Fill(nbJetGood,EvWeight);
	  hnJet[2]->Fill(nJetGood,EvWeight);
	  hHT[2]->Fill(HT40,EvWeight);
	  hST[2]->Fill(ST,EvWeight);
	  hdPhiWLep[2]->Fill(fabs(DelPhiWlep),EvWeight);

           hDfST[2]->Fill(ST,fabs(DelPhiWlep));
           hDfHT[2]->Fill(HT40,fabs(DelPhiWlep));
           hHTST[2]->Fill(HT40,ST);
           hNJST[2]->Fill(nJetGood,ST);
           hMTMET[2]->Fill(MT,MET);
	  hWpT[2]->Fill(Wpt,EvWeight);
          CFCounter[2]+= EvWeight;
          iCFCounter[2]++;

       if (HT40 < 500) continue;
              
           if(MET > 250 && MT > 120) hDfST[3]->Fill(ST,fabs(DelPhiWlep));
           if(ST > 250 && fabs(DelPhiWlep) >= 1) hMTMET[3]->Fill(MT,MET);
           
	  hnBJet[3]->Fill(nbJetGood,EvWeight);
	  hnJet[3]->Fill(nJetGood,EvWeight);
	  hHT[3]->Fill(HT40,EvWeight);
	  hST[3]->Fill(ST,EvWeight);
	  hdPhiWLep[3]->Fill(fabs(DelPhiWlep),EvWeight);
	  hdPhi[3]->Fill(DelPhiMetMu,EvWeight);

	  hWpT[3]->Fill(Wpt,EvWeight);
          CFCounter[3]+= EvWeight;
          iCFCounter[3]++;

       if (ST < 250 ) continue;
	  hnBJet[4]->Fill(nbJetGood,EvWeight);
	  hnJet[4]->Fill(nJetGood,EvWeight);
	  hHT[4]->Fill(HT40,EvWeight);
	  hST[4]->Fill(ST,EvWeight);
	  hdPhiWLep[4]->Fill(fabs(DelPhiWlep),EvWeight);
	  hWpT[4]->Fill(Wpt,EvWeight);

          CFCounter[4]+= EvWeight;
          iCFCounter[4]++;

      
       if (nbJetGood < 1) continue;
	  hnBJet[5]->Fill(nbJetGood,EvWeight);
	  hnJet[5]->Fill(nJetGood,EvWeight);
	  hngenLep[5]->Fill(ngenLep,EvWeight);
	  hngenLepFromTau[5]->Fill(ngenLepFromTau,EvWeight);
	  hHT[5]->Fill(HT40,EvWeight);
	  hST[5]->Fill(ST,EvWeight);  
	  hWpT[5]->Fill(Wpt,EvWeight);
	  hdPhiWLep[5]->Fill(fabs((DelPhiWlep)),EvWeight);
	  hdPhi[5]->Fill(DelPhiMetMu,EvWeight);
          CFCounter[5]+= EvWeight;
          iCFCounter[5]++;
           if(fabs(DelPhiWlep) < 1){
          CFCounter[6]+= EvWeight;
          iCFCounter[6]++;}
           if(fabs(DelPhiWlep) >= 1){
          CFCounter[7]+= EvWeight;
          iCFCounter[7]++;}

   ////// HT, NJ, ST ,Nb dependency of RCS
          if(fabs(DelPhiWlep) < 1){
	  hRC[5]->Fill(nbJetGood,EvWeight);
	  hNJRC[5]->Fill(nJetGood,EvWeight);
	  hHTRC[5]->Fill(HT40,EvWeight);
	  hSTRC[5]->Fill(ST,EvWeight);
         
          }
          if(fabs(DelPhiWlep) >= 1) {
	  hRS[5]->Fill(nbJetGood,EvWeight);
	  hNJRS[5]->Fill(nJetGood,EvWeight);
	  hHTRS[5]->Fill(HT40,EvWeight);
	  hSTRS[5]->Fill(ST,EvWeight);
          }
       if (ST > 250 && ST < 350 ) {
          if(fabs(DelPhiWlep) < 1){
	  hRC[6]->Fill(nbJetGood,EvWeight);
	  hNJRC[6]->Fill(nJetGood,EvWeight);
	  hHTRC[6]->Fill(HT40,EvWeight);
	  hSTRC[6]->Fill(ST,EvWeight);
           }
          if(fabs(DelPhiWlep) >= 1){
	  hRS[6]->Fill(nbJetGood,EvWeight);
	  hNJRS[6]->Fill(nJetGood,EvWeight);
	  hHTRS[6]->Fill(HT40,EvWeight);
	  hSTRS[6]->Fill(ST,EvWeight);
           }
          }
       if (ST > 350 && ST < 450 ) {
          if(fabs(DelPhiWlep) < 1){
	  hRC[7]->Fill(nbJetGood,EvWeight);
	  hNJRC[7]->Fill(nJetGood,EvWeight);
	  hHTRC[7]->Fill(HT40,EvWeight);
	  hSTRC[7]->Fill(ST,EvWeight);
          }
          if(fabs(DelPhiWlep) >= 1){
	  hRS[7]->Fill(nbJetGood,EvWeight);
	  hNJRS[7]->Fill(nJetGood,EvWeight);
	  hHTRS[7]->Fill(HT40,EvWeight);
	  hSTRS[7]->Fill(ST,EvWeight);
          }
          }
       if (ST > 450  ) {
          if(fabs(DelPhiWlep) < 1){
	  hRC[8]->Fill(nbJetGood,EvWeight);
	  hNJRC[8]->Fill(nJetGood,EvWeight);
	  hHTRC[8]->Fill(HT40,EvWeight);
	  hSTRC[8]->Fill(ST,EvWeight);
          }
          if(fabs(DelPhiWlep) >= 1){
	  hRS[8]->Fill(nbJetGood,EvWeight);
	  hNJRS[8]->Fill(nJetGood,EvWeight);
	  hHTRS[8]->Fill(HT40,EvWeight);
	  hSTRS[8]->Fill(ST,EvWeight);
          }
          }


   ////// End HT, NJ, ST ,Nb dependency of RCS

       if (nbJetGood == 1) {
       if (ST > 250 && ST <= 350 ) {
	  hnBJet[6]->Fill(nbJetGood,EvWeight);
	  hHT[6]->Fill(HT40,EvWeight);
	  hST[6]->Fill(ST,EvWeight);
	  hdPhiWLep[6]->Fill(fabs(DelPhiWlep),EvWeight);
	  hWpT[6]->Fill(Wpt,EvWeight);
          if(fabs(DelPhiWlep) < 1.0)
	  hRC[8]->Fill(nJetGood,EvWeight);
          if(fabs(DelPhiWlep) >= 1.0)
	  hRS[8]->Fill(nJetGood,EvWeight);
           if(fabs(DelPhiWlep) < 1){
          CFCounter[8]+= EvWeight;
          iCFCounter[8]++;}
           if(fabs(DelPhiWlep) >= 1){
          CFCounter[9]+= EvWeight;
          iCFCounter[9]++;}
            }

       if (ST > 350 && ST <= 450 ) {
	  hnBJet[7]->Fill(nbJetGood,EvWeight);
	  hnJet[7]->Fill(nJetGood,EvWeight);
	  hHT[7]->Fill(HT40,EvWeight);
	  hST[7]->Fill(ST,EvWeight);
	  hdPhiWLep[7]->Fill(fabs(DelPhiWlep),EvWeight);

	  hWpT[7]->Fill(Wpt,EvWeight);
          if(fabs(DelPhiWlep) < 1.0)
	  hRC[9]->Fill(nJetGood,EvWeight);
          if(fabs(DelPhiWlep) >= 1.0)
	  hRS[9]->Fill(nJetGood,EvWeight);
           if(fabs(DelPhiWlep) < 1){
          CFCounter[10]+= EvWeight;
          iCFCounter[10]++;}
           if(fabs(DelPhiWlep) >= 1){
          CFCounter[11]+= EvWeight;
          iCFCounter[11]++;}
         }

       if (ST >= 450 ) {
	  hnBJet[8]->Fill(nbJetGood,EvWeight);
	  hnJet[8]->Fill(nJetGood,EvWeight);
	  hHT[8]->Fill(HT40,EvWeight);
	  hST[8]->Fill(ST,EvWeight);
	  hdPhiWLep[8]->Fill(fabs(DelPhiWlep),EvWeight);

	  hWpT[8]->Fill(Wpt,EvWeight);
          if(fabs(DelPhiWlep) < 0.5)
	  hRC[10]->Fill(nJetGood,EvWeight);
          if(fabs(DelPhiWlep) >= 0.5)
	  hRS[10]->Fill(nJetGood,EvWeight);

           if(fabs(DelPhiWlep) < 0.5){
          CFCounter[12]+= EvWeight;
          iCFCounter[12]++;}
           if(fabs(DelPhiWlep) >= 0.5){
          CFCounter[13]+= EvWeight;
          iCFCounter[13]++;}
           } //ST > 450 GeV
}// Nb=1

       if (nbJetGood == 2) {

       if (ST > 250 && ST <= 350 ) {
          if(fabs(DelPhiWlep) < 1.0)
	  hRC[11]->Fill(nJetGood,EvWeight);
          if(fabs(DelPhiWlep) >= 1.0)
	  hRS[11]->Fill(nJetGood,EvWeight);
         if(fabs(DelPhiWlep) < 1.0){
          CFCounter[14]+= EvWeight;
          iCFCounter[14]++;}
           if(fabs(DelPhiWlep) >= 1.0){
          CFCounter[15]+= EvWeight;
          iCFCounter[15]++;}
        }
        if (ST > 350 && ST <= 450 ) {
          if(fabs(DelPhiWlep) < 1.0)
	  hRC[12]->Fill(nJetGood,EvWeight);
          if(fabs(DelPhiWlep) >= 1.0)
	  hRS[12]->Fill(nJetGood,EvWeight);
         if(fabs(DelPhiWlep) < 1.0){
          CFCounter[16]+= EvWeight;
          iCFCounter[16]++;}
           if(fabs(DelPhiWlep) >= 1.0){
          CFCounter[17]+= EvWeight;
          iCFCounter[17]++;}
        }
       if (ST > 450  ) {
          if(fabs(DelPhiWlep) < 1.0)
	  hRC[13]->Fill(nJetGood,EvWeight);
          if(fabs(DelPhiWlep) >= 1.0)
	  hRS[13]->Fill(nJetGood,EvWeight);
         if(fabs(DelPhiWlep) < 0.5){
          CFCounter[18]+= EvWeight;
          iCFCounter[18]++;}
           if(fabs(DelPhiWlep) >= 0.5){
          CFCounter[19]+= EvWeight;
          iCFCounter[19]++;}
        }

    }// Nb ==2

       if (nbJetGood > 2) {

       if (ST > 250 && ST <= 350 ) {
          if(fabs(DelPhiWlep) < 1.0)
	  hRC[14]->Fill(nbJetGood,EvWeight);
          if(fabs(DelPhiWlep) >= 1.0)
	  hRS[14]->Fill(nbJetGood,EvWeight);
         if(fabs(DelPhiWlep) < 1.0){
          CFCounter[20]+= EvWeight;
          iCFCounter[20]++;}
           if(fabs(DelPhiWlep) >= 1.0){
          CFCounter[21]+= EvWeight;
          iCFCounter[21]++;}
        }
        if (ST > 350 && ST <= 450 ) {
          if(fabs(DelPhiWlep) < 1.0)
	  hRC[15]->Fill(nbJetGood,EvWeight);
          if(fabs(DelPhiWlep) >= 1.0)
	  hRS[15]->Fill(nbJetGood,EvWeight);
         if(fabs(DelPhiWlep) < 1.0){
          CFCounter[22]+= EvWeight;
          iCFCounter[22]++;}
           if(fabs(DelPhiWlep) >= 1.0){
          CFCounter[23]+= EvWeight;
          iCFCounter[23]++;}
        }
       if (ST > 450  ) {
          if(fabs(DelPhiWlep) < 1.0)
	  hRC[16]->Fill(nbJetGood,EvWeight);
          if(fabs(DelPhiWlep) >= 1.0)
	  hRS[16]->Fill(nbJetGood,EvWeight);
         if(fabs(DelPhiWlep) < 0.5){
          CFCounter[24]+= EvWeight;
          iCFCounter[24]++;}
           if(fabs(DelPhiWlep) >= 0.5){
          CFCounter[25]+= EvWeight;
          iCFCounter[25]++;}
        }

    }// Nb >2


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
	
	    h0JetpT[1]->Write();
	    h1JetpT[1]->Write();
	    h2JetpT[1]->Write();
	    h3JetpT[1]->Write(); 
	    h0bJetpT[1]->Write();
	    h1bJetpT[1]->Write();
	    h2bJetpT[1]->Write();
	    h3bJetpT[1]->Write(); 
	for(int cj = 0; cj < CutNumb; cj++)
	  {

	    hRS[cj]->Write();
	    hRC[cj]->Write();
	    hHTRS[cj]->Write();
	    hHTRC[cj]->Write();
	    hSTRS[cj]->Write();
	    hSTRC[cj]->Write();
	    hNJRS[cj]->Write();
	    hNJRC[cj]->Write();
	    hHT[cj]->Write();
            hST[cj]->Write();
	    hWpT[cj]->Write();
	    hnJet[cj]->Write();
	    hnBJet[cj]->Write();
	    hnMu[cj]->Write(); 
	    hnEl[cj]->Write(); 
	    hMupt[cj]->Write(); 
	    hElpt[cj]->Write(); 
	    hnLep[cj]->Write(); 
	    //hngenLep[cj]->Write(); 
	    //hngenTau[cj]->Write(); 
	    //hngenLepFromTau[cj]->Write(); 

	    hLeppt[cj]->Write(); 
	    hLepeta[cj]->Write(); 
	    hMET[cj]->Write();
	    hdPhiWLep[cj]->Write();
	    hdPhi[cj]->Write();
            hDfST[cj]->Write();
            hDfHT[cj]->Write();
            hHTST[cj]->Write();
            hNJST[cj]->Write();
            hMTMET[cj]->Write();
        
	  }
}

