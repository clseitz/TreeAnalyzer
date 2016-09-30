#include "../interface/NtupleTools3.h"
#include "../interface/Objects.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <iostream>

#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
using namespace std;
// instance of the Objects class with functionality defined in ClassObjects.C

bool debug = false;
const int CutNumb = 26; // number of Cuts
const char * CutList[CutNumb] = {"noCut",
				 "== 1 Mu", "6Jet","HT>500","ST>250",
				 "Nb>=1"
};
// define global hists for plots after each cut
TH1F* CutFlow= new TH1F("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);



int main (int argc, char* argv[]){
    if (argc < 4) {
	cout<<"usage ./TreeAnalyzer_example [INPUTFOLDER] [XSEC * LUMI] [SAMPLENAME]"<<endl;
	return 1;
    }
    cout<<argc<<" "<<argv[1]<<" "<<argv[2]<<endl;
    TString list = argv[1];
    for (int i = 2; i<argc-1;i++){
	list.Append(" ");
	list.Append(argv[i]);
    }
    TString outname = argv[argc-1];
    bool isData = outname.Contains("data");
    cout<<outname<<" "<<isData<<endl;

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

    TFile * outt;
    TString treefilename = "CMG_"+outname+"_tree.root";
    outt = new TFile(treefilename,"RECREATE");
    TTree * tout = new TTree("tree","tree with jets");
    Long64_t maxTreeSize = 1000000000*2; //max size set to 2 GBytes
    tout->SetMaxTreeSize(maxTreeSize);
    //////////Book some histograms////
    TH1F h_MCweight("MCweight","MCweight", 2 , 0 , 2 ) ;
    h_MCweight.Sumw2();
    TH1F h_mLL("mLL","mLL", 100 , 50 , 150 ) ;
    h_mLL.Sumw2();
    TH1F h_1stJetpT("1stJetpT","1stJetpT", 100 , 0 , 200 ) ;
    h_1stJetpT.Sumw2();

    TH1F h_1stJeteta("1stJeteta","1stJeteta", 50 , -5 , 5 ) ;
    h_1stJeteta.Sumw2();

    TH1F h_mLL_after("mLL_after","mLL_after", 100 , 50 , 150 ) ;
    h_mLL_after.Sumw2();
    TH1F h_npv("npv","npv", 40 , 0 , 40 ) ;
    h_npv.Sumw2();
    TH1F h_nJet20("nJet20","nJet20", 40 , 0 , 40 ) ;
    h_nJet20.Sumw2();
    TH1F h_nJet20clean("nJet20clean","nJet20clean", 40 , 0 , 40 ) ;
    h_nJet20clean.Sumw2();

    //plots for efficiency

    TH1F h_JetpT_dPhiGeq2p5_Pass_loose("JetpT_dPhiGeq2p5_Pass_loose","JetpT_dPhiGeq2p5_Pass_loose", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiGeq2p5_Pass_loose_QG("JetpT_dPhiGeq2p5_Pass_loose_QG","JetpT_dPhiGeq2p5_Pass_loose_QG", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiLeq1p5_Pass_loose("JetpT_dPhiLeq1p5_Pass_loose","JetpT_dPhiLeq1p5_Pass_loose", 20 , 20 , 100 ) ;

    TH1F h_JetpT_dPhiGeq2p5_Pass_loose_cent("JetpT_dPhiGeq2p5_Pass_loose_cent","JetpT_dPhiGeq2p5_Pass_loose_cent", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiGeq2p5_Pass_loose_QG_cent("JetpT_dPhiGeq2p5_Pass_loose_QG_cent","JetpT_dPhiGeq2p5_Pass_loose_QG_cent", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiLeq1p5_Pass_loose_cent("JetpT_dPhiLeq1p5_Pass_loose_cent","JetpT_dPhiLeq1p5_Pass_loose_cent", 20 , 20 , 100 ) ;

    TH1F h_JetpT_dPhiGeq2p5_Pass_loose_forw("JetpT_dPhiGeq2p5_Pass_loose_forw","JetpT_dPhiGeq2p5_Pass_loose_forw", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiGeq2p5_Pass_loose_QG_forw("JetpT_dPhiGeq2p5_Pass_loose_QG_forw","JetpT_dPhiGeq2p5_Pass_loose_QG_forw", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiLeq1p5_Pass_loose_forw("JetpT_dPhiLeq1p5_Pass_loose_forw","JetpT_dPhiLeq1p5_Pass_loose_forw", 20 , 20 , 100 ) ;



    TH1F h_JetpT_dPhiGeq2p5_Pass_medium("JetpT_dPhiGeq2p5_Pass_medium","JetpT_dPhiGeq2p5_Pass_medium", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiGeq2p5_Pass_medium_QG("JetpT_dPhiGeq2p5_Pass_medium_QG","JetpT_dPhiGeq2p5_Pass_medium_QG", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiLeq1p5_Pass_medium("JetpT_dPhiLeq1p5_Pass_medium","JetpT_dPhiLeq1p5_Pass_medium", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiGeq2p5_Pass_tight("JetpT_dPhiGeq2p5_Pass_tight","JetpT_dPhiGeq2p5_Pass_tight", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiGeq2p5_Pass_tight_QG("JetpT_dPhiGeq2p5_Pass_tight_QG","JetpT_dPhiGeq2p5_Pass_tight_QG", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiLeq1p5_Pass_tight("JetpT_dPhiLeq1p5_Pass_tight","JetpT_dPhiLeq1p5_Pass_tight", 20 , 20 , 100 ) ;

    TH1F h_JetpT_dPhiGeq2p5_All("JetpT_dPhiGeq2p5_All","JetpT_dPhiGeq2p5_All", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiGeq2p5_All_QG("JetpT_dPhiGeq2p5_All_QG","JetpT_dPhiGeq2p5_All_QG", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiLeq1p5_All("JetpT_dPhiLeq1p5_All","JetpT_dPhiLeq1p5_All", 20 , 20 , 100 ) ;

    TH1F h_JetpT_dPhiGeq2p5_All_cent("JetpT_dPhiGeq2p5_All_cent","JetpT_dPhiGeq2p5_All_cent", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiGeq2p5_All_QG_cent("JetpT_dPhiGeq2p5_All_QG_cent","JetpT_dPhiGeq2p5_All_QG_cent", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiLeq1p5_All_cent("JetpT_dPhiLeq1p5_All_cent","JetpT_dPhiLeq1p5_All_cent", 20 , 20 , 100 ) ;

    TH1F h_JetpT_dPhiGeq2p5_All_forw("JetpT_dPhiGeq2p5_All_forw","JetpT_dPhiGeq2p5_All_forw", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiGeq2p5_All_QG_forw("JetpT_dPhiGeq2p5_All_QG_forw","JetpT_dPhiGeq2p5_All_QG_forw", 20 , 20 , 100 ) ;
    TH1F h_JetpT_dPhiLeq1p5_All_forw("JetpT_dPhiLeq1p5_All_forw","JetpT_dPhiLeq1p5_All_forw", 20 , 20 , 100 ) ;

    TH1F h_JetEta_dPhiGeq2p5_Pass_loose("JetEta_dPhiGeq2p5_Pass_loose","JetEta_dPhiGeq2p5_Pass_loose", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiGeq2p5_Pass_loose_QG("JetEta_dPhiGeq2p5_Pass_loose_QG","JetEta_dPhiGeq2p5_Pass_loose_QG", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiLeq1p5_Pass_loose("JetEta_dPhiLeq1p5_Pass_loose","JetEta_dPhiLeq1p5_Pass_loose", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiGeq2p5_Pass_medium("JetEta_dPhiGeq2p5_Pass_medium","JetEta_dPhiGeq2p5_Pass_medium", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiGeq2p5_Pass_medium_QG("JetEta_dPhiGeq2p5_Pass_medium_QG","JetEta_dPhiGeq2p5_Pass_medium_QG", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiLeq1p5_Pass_medium("JetEta_dPhiLeq1p5_Pass_medium","JetEta_dPhiLeq1p5_Pass_medium", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiGeq2p5_Pass_tight("JetEta_dPhiGeq2p5_Pass_tight","JetEta_dPhiGeq2p5_Pass_tight", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiGeq2p5_Pass_tight_QG("JetEta_dPhiGeq2p5_Pass_tight_QG","JetEta_dPhiGeq2p5_Pass_tight_QG", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiLeq1p5_Pass_tight("JetEta_dPhiLeq1p5_Pass_tight","JetEta_dPhiLeq1p5_Pass_tight", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiGeq2p5_All("JetEta_dPhiGeq2p5_All","JetEta_dPhiGeq2p5_All", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiGeq2p5_All_QG("JetEta_dPhiGeq2p5_All_QG","JetEta_dPhiGeq2p5_All_QG", 25 , -5 , 5 ) ;
    TH1F h_JetEta_dPhiLeq1p5_All("JetEta_dPhiLeq1p5_All","JetEta_dPhiLeq1p5_All", 25 , -5 , 5 ) ;

    h_JetpT_dPhiGeq2p5_Pass_loose.Sumw2();
    h_JetpT_dPhiGeq2p5_Pass_loose_QG.Sumw2();
    h_JetpT_dPhiLeq1p5_Pass_loose.Sumw2();

    h_JetpT_dPhiGeq2p5_Pass_loose_cent.Sumw2();
    h_JetpT_dPhiGeq2p5_Pass_loose_QG_cent.Sumw2();
    h_JetpT_dPhiLeq1p5_Pass_loose_cent.Sumw2();

    h_JetpT_dPhiGeq2p5_Pass_loose_forw.Sumw2();
    h_JetpT_dPhiGeq2p5_Pass_loose_QG_forw.Sumw2();
    h_JetpT_dPhiLeq1p5_Pass_loose_forw.Sumw2();

    h_JetpT_dPhiGeq2p5_Pass_medium.Sumw2();
    h_JetpT_dPhiGeq2p5_Pass_medium_QG.Sumw2();
    h_JetpT_dPhiLeq1p5_Pass_medium.Sumw2();
    h_JetpT_dPhiGeq2p5_Pass_tight.Sumw2();
    h_JetpT_dPhiGeq2p5_Pass_tight_QG.Sumw2();
    h_JetpT_dPhiLeq1p5_Pass_tight.Sumw2();

    h_JetpT_dPhiGeq2p5_All.Sumw2();
    h_JetpT_dPhiGeq2p5_All_QG.Sumw2();
    h_JetpT_dPhiLeq1p5_All.Sumw2();


    h_JetpT_dPhiGeq2p5_All_cent.Sumw2();
    h_JetpT_dPhiGeq2p5_All_QG_cent.Sumw2();
    h_JetpT_dPhiLeq1p5_All_cent.Sumw2();

    h_JetpT_dPhiGeq2p5_All_forw.Sumw2();
    h_JetpT_dPhiGeq2p5_All_QG_forw.Sumw2();
    h_JetpT_dPhiLeq1p5_All_forw.Sumw2();



    h_JetEta_dPhiGeq2p5_Pass_loose.Sumw2();
    h_JetEta_dPhiGeq2p5_Pass_loose_QG.Sumw2();
    h_JetEta_dPhiLeq1p5_Pass_loose.Sumw2();
    h_JetEta_dPhiGeq2p5_Pass_medium.Sumw2();
    h_JetEta_dPhiGeq2p5_Pass_medium_QG.Sumw2();
    h_JetEta_dPhiLeq1p5_Pass_medium.Sumw2();
    h_JetEta_dPhiGeq2p5_Pass_tight.Sumw2();
    h_JetEta_dPhiGeq2p5_Pass_tight_QG.Sumw2();
    h_JetEta_dPhiLeq1p5_Pass_tight.Sumw2();
    h_JetEta_dPhiGeq2p5_All.Sumw2();
    h_JetEta_dPhiGeq2p5_All_QG.Sumw2();
    h_JetEta_dPhiLeq1p5_All.Sumw2();
     


    TH1F h_m1_iso("h_m1_iso","h_m1_iso", 100 , 0 , 4 ) ;
    TH1F h_m2_iso("h_m2_iso","h_m2_iso", 100 , 0 , 4 ) ;


    ////////Add histograms from Jin/////
    int nflav = 5;
    int npt = 4;
    int neta = 5;

    TH1D * hJetpT_dPhiGeq2p5_Pass_loose[nflav][npt][neta];
    TH1D * hJetpT_dPhiLeq1p5_Pass_loose[nflav][npt][neta];
    TH1D * hJetpT_dPhiGeq2p5_All[nflav][npt][neta];
    TH1D * hJetpT_dPhiLeq1p5_All[nflav][npt][neta];

    TH1D * hJetEta_dPhiGeq2p5_Pass_loose[nflav][npt][neta];
    TH1D * hJetEta_dPhiLeq1p5_Pass_loose[nflav][npt][neta];
    TH1D * hJetEta_dPhiGeq2p5_All[nflav][npt][neta];
    TH1D * hJetEta_dPhiLeq1p5_All[nflav][npt][neta];

    TH1D * hJetpT_dPhiGeq2p5_Pass_medium[nflav][npt][neta];
    TH1D * hJetpT_dPhiLeq1p5_Pass_medium[nflav][npt][neta];
    TH1D * hJetEta_dPhiGeq2p5_Pass_medium[nflav][npt][neta];
    TH1D * hJetEta_dPhiLeq1p5_Pass_medium[nflav][npt][neta];


    TH1D * hJetpT_dPhiGeq2p5_Pass_tight[nflav][npt][neta];
    TH1D * hJetpT_dPhiLeq1p5_Pass_tight[nflav][npt][neta];
    TH1D * hJetEta_dPhiGeq2p5_Pass_tight[nflav][npt][neta];
    TH1D * hJetEta_dPhiLeq1p5_Pass_tight[nflav][npt][neta];

    TH1D * hDRweighted[nflav][npt][neta];
    TH1D * hmLL[nflav][npt][neta];
    TH1D * hnpv[nflav][npt][neta];
    TH1D * hnTot[nflav][npt][neta];
    TH1D * hnCh[nflav][npt][neta];
    TH1D * haxisMajor[nflav][npt][neta];
    TH1D * haxisMinor[nflav][npt][neta];
    TH1D * hfRing0[nflav][npt][neta];
    TH1D * hfRing1[nflav][npt][neta];
    TH1D * hfRing2[nflav][npt][neta];
    TH1D * hfRing3[nflav][npt][neta];
    TH1D * hptD[nflav][npt][neta];
    TH1D * hbeta[nflav][npt][neta];
    TH1D * hpull[nflav][npt][neta];

    TH1D * hPUdisc[nflav][npt][neta];
    TH1D * hjetR[nflav][npt][neta];
    TH1D * hjetRchg[nflav][npt][neta];
    TH1D * hjtpt[nflav][npt][neta];
    TH1D * hjteta[nflav][npt][neta];
    TH2D * hPtRatio_vs_dphiZJ[nflav];

    for(int i=0;i<nflav;i++){
      hPtRatio_vs_dphiZJ[i] = new TH2D(Form("hPtRatio_vs_dphiZJ_%d",i),Form("hPtRatio_vs_dphiZJ_%d",i), 62 , -3.2 , 3.2 , 100, 0, 10) ;
      hPtRatio_vs_dphiZJ[i]->SetTitle(";dPhi(Z,J);p^{T}_{J}/p^{T}_{Z}");
      hPtRatio_vs_dphiZJ[i]->Sumw2();
      for(int j=0;j<npt;j++){
	for(int k=0;k<neta;k++){
	  hmLL[i][j][k]=new TH1D(Form("hmLL_%d_%d_%d",i,j,k),Form("hmLL_%d_%d_%d",i,j,k),100,50,150);
	  hmLL[i][j][k]->SetTitle(";mLL;");
	  hmLL[i][j][k]->Sumw2();
	  hDRweighted[i][j][k]=new TH1D(Form("hDRweighted_%d_%d_%d",i,j,k),Form("hDRweighted_%d_%d_%d",i,j,k),100,0.,0.1);
	  hDRweighted[i][j][k]->SetTitle(";dR2Mean;");
	  hDRweighted[i][j][k]->Sumw2();
	  hnpv[i][j][k]=new TH1D(Form("hnpv_%d_%d_%d",i,j,k),Form("hnpv_%d_%d_%d",i,j,k),100,0.,100.);
	  hnpv[i][j][k]->SetTitle(";npv;");
	  hnpv[i][j][k]->Sumw2();
	  hnTot[i][j][k]=new TH1D(Form("hnTot_%d_%d_%d",i,j,k),Form("hnTot_%d_%d_%d",i,j,k),60,0.,60.);
	  hnTot[i][j][k]->SetTitle(";nParticles;");
	  hnTot[i][j][k]->Sumw2();
	  hnCh[i][j][k]=new TH1D(Form("hnCh_%d_%d_%d",i,j,k),Form("hnCh_%d_%d_%d",i,j,k),40,0.,40.);
	  hnCh[i][j][k]->SetTitle(";nCharged;");
	  hnCh[i][j][k]->Sumw2();
	  haxisMajor[i][j][k]=new TH1D(Form("haxisMajor_%d_%d_%d",i,j,k),Form("haxisMajor_%d_%d_%d",i,j,k),60,0.,0.3);
	  haxisMajor[i][j][k]->SetTitle(";majW;");
	  haxisMajor[i][j][k]->Sumw2();
	  haxisMinor[i][j][k]=new TH1D(Form("haxisMinor_%d_%d_%d",i,j,k),Form("haxisMinor_%d_%d_%d",i,j,k),40,0.,0.2);
	  haxisMinor[i][j][k]->SetTitle(";minW;");
	  haxisMinor[i][j][k]->Sumw2();
	  hfRing0[i][j][k]=new TH1D(Form("hfRing0_%d_%d_%d",i,j,k),Form("hfRing0_%d_%d_%d",i,j,k),100,0.,1.);
	  hfRing0[i][j][k]->SetTitle(";frac01;");
	  hfRing0[i][j][k]->Sumw2();
	  hfRing1[i][j][k]=new TH1D(Form("hfRing1_%d_%d_%d",i,j,k),Form("hfRing1_%d_%d_%d",i,j,k),100,0.,1.);
	  hfRing1[i][j][k]->SetTitle(";frac02;");
	  hfRing1[i][j][k]->Sumw2();
	  hfRing2[i][j][k]=new TH1D(Form("hfRing2_%d_%d_%d",i,j,k),Form("hfRing2_%d_%d_%d",i,j,k),100,0.,1.);
	  hfRing2[i][j][k]->SetTitle(";frac03;");
	  hfRing2[i][j][k]->Sumw2();
	  hfRing3[i][j][k]=new TH1D(Form("hfRing3_%d_%d_%d",i,j,k),Form("hfRing3_%d_%d_%d",i,j,k),100,0.,1.);
	  hfRing3[i][j][k]->SetTitle(";frac04;");
	  hfRing3[i][j][k]->Sumw2();
	  hptD[i][j][k]=new TH1D(Form("hptD_%d_%d_%d",i,j,k),Form("hptD_%d_%d_%d",i,j,k),100,0.,1.);
	  hptD[i][j][k]->SetTitle(";ptD;");
	  hptD[i][j][k]->Sumw2();
	  hbeta[i][j][k]=new TH1D(Form("hbetaClassic_%d_%d_%d",i,j,k),Form("hbetaClassic_%d_%d_%d",i,j,k),100,0.,1.);
	  hbeta[i][j][k]->SetTitle(";betaClassic;");
	  hbeta[i][j][k]->Sumw2();
	  hpull[i][j][k]=new TH1D(Form("hpull_%d_%d_%d",i,j,k),Form("hpull_%d_%d_%d",i,j,k),25,0.,0.025);
	  hpull[i][j][k]->SetTitle(";pull;");
	  hpull[i][j][k]->Sumw2();

	  hPUdisc[i][j][k]=new TH1D(Form("hPUdisc_%d_%d_%d",i,j,k),Form("hPUdisc_%d_%d_%d",i,j,k),50,-1,1);
	  hPUdisc[i][j][k]->SetTitle(";PUdisc;");
	  hPUdisc[i][j][k]->Sumw2();

	  hjetR[i][j][k]=new TH1D(Form("hjetR_%d_%d_%d",i,j,k),Form("hjetR_%d_%d_%d",i,j,k),100,0.,1.);
	  hjetR[i][j][k]->SetTitle(";jetR;");
	  hjetR[i][j][k]->Sumw2();
	  hjetRchg[i][j][k]=new TH1D(Form("hjetRchg_%d_%d_%d",i,j,k),Form("hjetRchg_%d_%d_%d",i,j,k),100,0.,1.);
	  hjetRchg[i][j][k]->SetTitle(";jetRchg;");
	  hjetRchg[i][j][k]->Sumw2();
	  hjtpt[i][j][k]=new TH1D(Form("hjtpt_%d_%d_%d",i,j,k),Form("hjtpt_%d_%d_%d",i,j,k),100,0.,100.);
	  hjtpt[i][j][k]->SetTitle(";jet p_{T};");
	  hjtpt[i][j][k]->Sumw2();
	  hjteta[i][j][k]=new TH1D(Form("hjteta_%d_%d_%d",i,j,k),Form("hjteta_%d_%d_%d",i,j,k),100,-5.,5.);
	  hjteta[i][j][k]->SetTitle(";jet #eta;");
	  hjteta[i][j][k]->Sumw2();


	  ///////////Plots for efficieceny//////////

	  hJetpT_dPhiGeq2p5_All[i][j][k]=new TH1D(Form("hJetpT_dPhiGeq2p5_All_%d_%d_%d",i,j,k),Form("hJetpT_dPhiGeq2p5_All_%d_%d_%d",i,j,k),20,20.,100);
	  hJetpT_dPhiGeq2p5_All[i][j][k]->SetTitle(";jet p_{T};");
	  hJetpT_dPhiGeq2p5_All[i][j][k]->Sumw2();

	  hJetpT_dPhiLeq1p5_All[i][j][k]=new TH1D(Form("hJetpT_dPhiLeq1p5_All_%d_%d_%d",i,j,k),Form("hJetpT_dPhiLeq1p5_All_%d_%d_%d",i,j,k),20,20.,100);
	  hJetpT_dPhiLeq1p5_All[i][j][k]->SetTitle(";jet p_{T};");
	  hJetpT_dPhiLeq1p5_All[i][j][k]->Sumw2();


	  hJetpT_dPhiGeq2p5_Pass_loose[i][j][k]=new TH1D(Form("hJetpT_dPhiGeq2p5_Pass_loose_%d_%d_%d",i,j,k),Form("hJetpT_dPhiGeq2p5_Pass_loose_%d_%d_%d",i,j,k),20,20.,100);
	  hJetpT_dPhiGeq2p5_Pass_loose[i][j][k]->SetTitle(";jet p_{T};");
	  hJetpT_dPhiGeq2p5_Pass_loose[i][j][k]->Sumw2();

	  hJetpT_dPhiLeq1p5_Pass_loose[i][j][k]=new TH1D(Form("hJetpT_dPhiLeq1p5_Pass_loose_%d_%d_%d",i,j,k),Form("hJetpT_dPhiLeq1p5_Pass_loose_%d_%d_%d",i,j,k),20,20.,100);
	  hJetpT_dPhiLeq1p5_Pass_loose[i][j][k]->SetTitle(";jet p_{T};");
	  hJetpT_dPhiLeq1p5_Pass_loose[i][j][k]->Sumw2();


	  hJetpT_dPhiGeq2p5_Pass_medium[i][j][k]=new TH1D(Form("hJetpT_dPhiGeq2p5_Pass_medium_%d_%d_%d",i,j,k),Form("hJetpT_dPhiGeq2p5_Pass_medium_%d_%d_%d",i,j,k),20,20.,100);
	  hJetpT_dPhiGeq2p5_Pass_medium[i][j][k]->SetTitle(";jet p_{T};");
	  hJetpT_dPhiGeq2p5_Pass_medium[i][j][k]->Sumw2();

	  hJetpT_dPhiLeq1p5_Pass_medium[i][j][k]=new TH1D(Form("hJetpT_dPhiLeq1p5_Pass_medium_%d_%d_%d",i,j,k),Form("hJetpT_dPhiLeq1p5_Pass_medium_%d_%d_%d",i,j,k),20,20.,100);
	  hJetpT_dPhiLeq1p5_Pass_medium[i][j][k]->SetTitle(";jet p_{T};");
	  hJetpT_dPhiLeq1p5_Pass_medium[i][j][k]->Sumw2();


	  hJetpT_dPhiGeq2p5_Pass_tight[i][j][k]=new TH1D(Form("hJetpT_dPhiGeq2p5_Pass_tight_%d_%d_%d",i,j,k),Form("hJetpT_dPhiGeq2p5_Pass_tight_%d_%d_%d",i,j,k),20,20.,100);
	  hJetpT_dPhiGeq2p5_Pass_tight[i][j][k]->SetTitle(";jet p_{T};");
	  hJetpT_dPhiGeq2p5_Pass_tight[i][j][k]->Sumw2();

	  hJetpT_dPhiLeq1p5_Pass_tight[i][j][k]=new TH1D(Form("hJetpT_dPhiLeq1p5_Pass_tight_%d_%d_%d",i,j,k),Form("hJetpT_dPhiLeq1p5_Pass_tight_%d_%d_%d",i,j,k),20,20.,100);
	  hJetpT_dPhiLeq1p5_Pass_tight[i][j][k]->SetTitle(";jet p_{T};");
	  hJetpT_dPhiLeq1p5_Pass_tight[i][j][k]->Sumw2();
	  //////////ETA/////

	  hJetEta_dPhiGeq2p5_All[i][j][k]=new TH1D(Form("hJetEta_dPhiGeq2p5_All_%d_%d_%d",i,j,k),Form("hJetEta_dPhiGeq2p5_All_%d_%d_%d",i,j,k),25,-5,5);
	  hJetEta_dPhiGeq2p5_All[i][j][k]->SetTitle(";jet p_{T};");
	  hJetEta_dPhiGeq2p5_All[i][j][k]->Sumw2();

	  hJetEta_dPhiLeq1p5_All[i][j][k]=new TH1D(Form("hJetEta_dPhiLeq1p5_All_%d_%d_%d",i,j,k),Form("hJetEta_dPhiLeq1p5_All_%d_%d_%d",i,j,k),25,-5,5);
	  hJetEta_dPhiLeq1p5_All[i][j][k]->SetTitle(";jet p_{T};");
	  hJetEta_dPhiLeq1p5_All[i][j][k]->Sumw2();


	  hJetEta_dPhiGeq2p5_Pass_loose[i][j][k]=new TH1D(Form("hJetEta_dPhiGeq2p5_Pass_loose_%d_%d_%d",i,j,k),Form("hJetEta_dPhiGeq2p5_Pass_loose_%d_%d_%d",i,j,k),25,-5,5);
	  hJetEta_dPhiGeq2p5_Pass_loose[i][j][k]->SetTitle(";jet p_{T};");
	  hJetEta_dPhiGeq2p5_Pass_loose[i][j][k]->Sumw2();

	  hJetEta_dPhiLeq1p5_Pass_loose[i][j][k]=new TH1D(Form("hJetEta_dPhiLeq1p5_Pass_loose_%d_%d_%d",i,j,k),Form("hJetEta_dPhiLeq1p5_Pass_loose_%d_%d_%d",i,j,k),25,-5,5);
	  hJetEta_dPhiLeq1p5_Pass_loose[i][j][k]->SetTitle(";jet p_{T};");
	  hJetEta_dPhiLeq1p5_Pass_loose[i][j][k]->Sumw2();


	  hJetEta_dPhiGeq2p5_Pass_medium[i][j][k]=new TH1D(Form("hJetEta_dPhiGeq2p5_Pass_medium_%d_%d_%d",i,j,k),Form("hJetEta_dPhiGeq2p5_Pass_medium_%d_%d_%d",i,j,k),25,-5,5);
	  hJetEta_dPhiGeq2p5_Pass_medium[i][j][k]->SetTitle(";jet p_{T};");
	  hJetEta_dPhiGeq2p5_Pass_medium[i][j][k]->Sumw2();

	  hJetEta_dPhiLeq1p5_Pass_medium[i][j][k]=new TH1D(Form("hJetEta_dPhiLeq1p5_Pass_medium_%d_%d_%d",i,j,k),Form("hJetEta_dPhiLeq1p5_Pass_medium_%d_%d_%d",i,j,k),25,-5,5);
	  hJetEta_dPhiLeq1p5_Pass_medium[i][j][k]->SetTitle(";jet p_{T};");
	  hJetEta_dPhiLeq1p5_Pass_medium[i][j][k]->Sumw2();



	  hJetEta_dPhiGeq2p5_Pass_tight[i][j][k]=new TH1D(Form("hJetEta_dPhiGeq2p5_Pass_tight_%d_%d_%d",i,j,k),Form("hJetEta_dPhiGeq2p5_Pass_tight_%d_%d_%d",i,j,k),25,-5,5);
	  hJetEta_dPhiGeq2p5_Pass_tight[i][j][k]->SetTitle(";jet p_{T};");
	  hJetEta_dPhiGeq2p5_Pass_tight[i][j][k]->Sumw2();

	  hJetEta_dPhiLeq1p5_Pass_tight[i][j][k]=new TH1D(Form("hJetEta_dPhiLeq1p5_Pass_tight_%d_%d_%d",i,j,k),Form("hJetEta_dPhiLeq1p5_Pass_tight_%d_%d_%d",i,j,k),25,-5,5);
	  hJetEta_dPhiLeq1p5_Pass_tight[i][j][k]->SetTitle(";jet p_{T};");
	  hJetEta_dPhiLeq1p5_Pass_tight[i][j][k]->Sumw2();


	}
      }
    }


    //    TChain* chainJet = new TChain("AK4PFCHS/t");
    //TChain* chainEvt = new TChain("event/t");
    EasyChain* chainJet = new EasyChain("AK4PFCHS/t");
    EasyChain* chainEvt = new EasyChain("event/t");
    EasyChain* chainMu = new EasyChain("muons/t");
    EasyChain* chainHlt = new EasyChain("hlt/t");
    Double_t startweight = 1;
    for(unsigned i=0;i<files.size();i++){
      chainJet->AddSmartW(files[i],weights[i]);//AddFile(files[i]);
      chainEvt->AddSmartW(files[i],weights[i]);//AddFile(files[i]);
      chainMu->AddSmartW(files[i],weights[i]);//AddFile(files[i]);
      chainHlt->AddSmartW(files[i],weights[i]);//AddFile(files[i]);
      cout<<"add: "<<files[i]<<" "<<weights[i]<<endl;
      startweight = weights[i];
    }

    //lumi calcualtion done in runAnalyzer.py (fb and pb)                                                      

    TTreeReader evtReader(chainEvt);
    TTreeReader jetReader(chainJet);
    TTreeReader muReader(chainMu);
    TTreeReader hltReader(chainHlt);

    cout<<"applying weight "<<startweight<<endl;

    
    int Nevents=evtReader.GetEntries(1);
    cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;

    TTreeReaderValue<ULong64_t> evt(evtReader, "evt");
    TTreeReaderValue<float> MCweight(evtReader, "weight");
    TTreeReaderValue<ULong64_t> npv(evtReader, "npv");

    TTreeReaderArray<float> betaStar(jetReader, "betaStar");
    TTreeReaderArray<float> jet_pt(jetReader, "p4.fCoordinates.fPt");
    TTreeReaderArray<float> jet_eta(jetReader, "p4.fCoordinates.fEta");
    TTreeReaderArray<float> jet_phi(jetReader, "p4.fCoordinates.fPhi");
    TTreeReaderArray<float> jet_e(jetReader, "p4.fCoordinates.fE");
    TTreeReaderArray<float> jet_beta(jetReader, "betaClassic");
    TTreeReaderArray<float> jet_Rchg(jetReader, "jetRchg");
    TTreeReaderArray<float> jet_DRweighted(jetReader, "DRweighted");
    TTreeReaderArray<float> jet_fRing0(jetReader, "fRing0");    
    TTreeReaderArray<float> jet_fRing1(jetReader, "fRing1");    
    TTreeReaderArray<float> jet_fRing2(jetReader, "fRing2");    
    TTreeReaderArray<float> jet_fRing3(jetReader, "fRing3");    
    TTreeReaderArray<float> jet_fRing4(jetReader, "fRing4");    
    //TTreeReaderArray<float> jet_jetRneutral(jetReader, "jetRneutral");    
    //TTreeReaderArray<float> jet_jetRchg(jetReader, "jetRchg");    
    TTreeReaderArray<float> jet_jetR(jetReader, "jetR");    
    TTreeReaderArray<float> jet_axisMajor(jetReader, "axisMajor");    
    TTreeReaderArray<float> jet_axisMinor(jetReader, "axisMinor");    
    TTreeReaderArray<unsigned short> jet_nCh(jetReader, "nCh");    
    TTreeReaderArray<int> jet_nTot(jetReader, "nTot");    
    TTreeReaderArray<float> jet_ptD(jetReader, "ptD");    
    TTreeReaderArray<float> jet_pull(jetReader, "pull");    
    TTreeReaderArray<float> jet_PUdisc(jetReader, "PUJetId_fullDiscriminant");    
    TTreeReaderArray<int> jet_PUfullID(jetReader, "PUJetId_fullId");    

    cout<<"2"<<endl;
    //variables for PUjet ID


    TTreeReaderArray<float> genjet_pt(jetReader, "gen_p4.fCoordinates.fPt");
    TTreeReaderArray<float> genjet_eta(jetReader, "gen_p4.fCoordinates.fEta");
    TTreeReaderArray<float> genjet_phi(jetReader, "gen_p4.fCoordinates.fPhi");
    TTreeReaderArray<float> genjet_e(jetReader, "gen_p4.fCoordinates.fE");
    TTreeReaderArray<int> partonFlavor(jetReader, "partonFlavor");
    TTreeReaderArray<float> dRMatch(jetReader, "dRMatch");
    cout<<"3"<<endl;
    ///////////////////////MUONS////////////////////
    TTreeReaderArray<float> mu_pt(muReader, "p4.fCoordinates.fPt");
    TTreeReaderArray<float> mu_eta(muReader, "p4.fCoordinates.fEta");
    TTreeReaderArray<float> mu_phi(muReader, "p4.fCoordinates.fPhi");
    TTreeReaderArray<float> mu_e(muReader, "p4.fCoordinates.fE");
    TTreeReaderArray<int> mu_charge(muReader, "charge");
    TTreeReaderArray<float> mu_iso(muReader, "relativeIsoR03_deltaBeta");
    TTreeReaderValue<vector<bool>> mu_isTight(muReader, "isTight");
    cout<<"4"<<endl;
    TTreeReaderArray<string> hlt_paths(hltReader, "paths");
    TTreeReaderArray<vector<int>> hlt_objects_paths(hltReader, "objects_paths");
    TTreeReaderValue<vector<vector<bool>>> hlt_objects_paths_islast(hltReader, "objects_paths_islast");
    

    /////Define small output tree/////////

 
    float mLL, chargeLL; 
   std::vector<float>* jpt = new std::vector<float>;
    std::vector<float>* jeta= new std::vector<float>;
    std::vector<float>* jphi= new std::vector<float>;
    std::vector<float>* je= new std::vector<float>;
    std::vector<int>* jpartonflavor = new std::vector<int>;
    std::vector<float>* refjpt = new std::vector<float>;
    std::vector<float>* refjeta = new std::vector<float>;
    std::vector<float>* refjphi = new std::vector<float>;
    std::vector<float>* refje = new std::vector<float>;
    std::vector<float>* jbeta = new std::vector<float>;
    std::vector<float>* jRchg = new std::vector<float>;
    std::vector<float>* jDRweighted = new std::vector<float>;
    std::vector<float>* jfRing0 = new std::vector<float>;
    std::vector<float>* jfRing1 = new std::vector<float>;
    std::vector<float>* jfRing2 = new std::vector<float>;
    std::vector<float>* jfRing3 = new std::vector<float>;
    std::vector<float>* jfRing4 = new std::vector<float>;
    std::vector<float>* jjetR = new std::vector<float>;
    std::vector<float>* jaxisMajor = new std::vector<float>;
    std::vector<float>* jaxisMinor = new std::vector<float>;
    std::vector<unsigned short>* jnCh  = new std::vector<unsigned short>;
    std::vector<int>* jnTot = new std::vector<int>;
    std::vector<float>* jptD  = new std::vector<float>;
    std::vector<float>* jpull  = new std::vector<float>;

   //    cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;
    string outnameStr = (string)outname;
    const std::string trig1("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ");
    const std::string trig2("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ");

    cout << "Starting event loop" << endl;
    Long64_t entry = 0;
    //    for(int entry=0; entry < Nevents/*min(1000000,Nevents)*/; entry+=1){

    while (jetReader.Next() /*&& entry < 100000*/) {
      
      evtReader.Next();  
      muReader.Next();  
      hltReader.Next();  
      //      if (entry > 20) break;
            if (entry % 1000 == 0)
      cout << "================= Processing entry: " << entry << '\r' << flush;
      
      
      
      entry++;

      std::vector<Jet> jets20;
      TLorentzVector Zvector;
      jets20.clear();
      float NormMCweight = (*MCweight)/fabs(*MCweight);
      float myweight=startweight * NormMCweight;
      ///Fill histogram for MC weights///
      
      h_MCweight.Fill(1, NormMCweight);
      ///////Trigger objects//////////////////
      std::vector< int > TrigObjIdx ; 
      
      bool trigOK=false;
      for( unsigned int i = 0 ; i < hlt_paths.GetSize(); i ++){
	std::string * trig = & hlt_paths[i];
	
	int trigFound=0;
	if( trig->find( trig1 ) != std::string::npos) trigFound++;
	if( trig->find( trig2 ) != std::string::npos) trigFound++;
	
	if (trigFound == 0) continue;
	trigOK = true;
	
	//	cout<<"i = "<<i<<" hlt: "<<(*trig)<<endl;
	for( int iObj = 0 ; iObj < hlt_objects_paths.GetSize() ; iObj ++ ){
	  // Search obje that matches to the trigger.
	  std::vector<int> p = hlt_objects_paths[iObj] ;
	  std::vector<bool> b = hlt_objects_paths_islast->at(iObj) ;
	  for( unsigned int j = 0 ; j < p.size() ; j++ ){
	    if( b.at(j) && ( p.at(j) == i )) {
	      TrigObjIdx . push_back( iObj );
	    }
	  }
	}
      }
      
      if (!trigOK) continue;
      //////////////////////////////////////
      
      
      //////Muon selection on Z events///////////
      vector<Lepton> muons;
      int nMu = mu_pt.GetSize();
      for (int i=0; i<nMu; i++){
	//cout<<mu_isTight->at(i)<<" "<<mu_pt[i]<<" "<<fabs(mu_eta[i])<<" "<<mu_iso[i]<<endl;
	if(mu_isTight->at(i)  && mu_pt[i] > 20.0 && fabs(mu_eta[i]) < 2.4 && mu_iso[i] < 0.1){ //fix eta and pt cuts
	  Lepton dummy;
	  dummy.SetPtEtaPhiE(mu_pt[i], mu_eta[i], mu_phi[i], mu_e[i]);
	  dummy.charge = mu_charge[i];
	  dummy.relIso03 = mu_iso[i];
	  muons.push_back(dummy);
	}
      }
      
      //Select exactly two tight leptons
      if (muons.size() != 2 ) continue;
      
      mLL = (muons[0] + muons[1]).M();
      chargeLL = muons[0].charge * muons[1].charge;
      Zvector = (muons[0] + muons[1]);
      if (debug) cout<<" mass "<< mLL<< " " << chargeLL << endl;
      //Remove same sign pairs
      if (chargeLL > 0 ) continue;
      h_mLL.Fill(mLL, myweight);
      h_m1_iso.Fill( muons[0].relIso03, myweight);
      h_m2_iso.Fill( muons[1].relIso03, myweight);
      //Remove events outside Z window
      if ( ! (mLL > 70 && mLL < 110)) continue;
      
      //////////////////////////////////////////////////
      bool pre_sel;
      bool flavor_def[4];
      bool pt_bin[3];
      bool eta_bin[4];
      
      //////remove jets that overlap with one of the muons////
      int nJet = jet_pt.GetSize();

      int nJet20 = 0;
      int nJet20clean = 0;
      int nGenJet = genjet_pt.GetSize();
      if (debug) cout<<"nJet "<<nJet<<" nGenJet "<<nGenJet<<endl;
      for (int nJ=0; nJ<nJet; nJ++){
	Jet vJet, vGenJet, v0;
	vJet.SetPtEtaPhiE(jet_pt[nJ],jet_eta[nJ],jet_phi[nJ],jet_e[nJ]);
	
	if (vJet.Pt() < 20.0 or vJet.Pt() > 100 or fabs(vJet.Eta()) > 5.0)
	  continue;
	//coutn jets
	nJet20++;

	vGenJet.SetPtEtaPhiE(0,0,0,0);
	int i_partonflavor = 999;
	float  refdrjt = 999;
	float  refdrjtwrong = 999;
	if (!isData){
	  vGenJet.SetPtEtaPhiE(genjet_pt[nJ],genjet_eta[nJ],genjet_phi[nJ],genjet_e[nJ]);
	  refdrjtwrong = vJet.DeltaR(vGenJet);
	  for (int nGJ=0; nGJ<nGenJet; nGJ++){  
	    vGenJet.SetPtEtaPhiE(genjet_pt[nGJ],genjet_eta[nGJ],genjet_phi[nGJ],genjet_e[nGJ]);
	    if (vJet.DeltaR(vGenJet) < refdrjt){
		refdrjt = vJet.DeltaR(vGenJet);
	      }

	  }
	  i_partonflavor = partonFlavor[nJ];
	}
	v0.SetPtEtaPhiE(0,0,0,0);
	//remove jets that are close to a muon
	if(vJet!=v0 && vJet.DeltaR(muons[0]) < 0.4  )
	  continue;
	if(vJet!=v0 && vJet.DeltaR(muons[1]) < 0.4  )
	  continue;
	
	//       	cout<<"refdrjtwrong "<<refdrjtwrong<<" dRMatch[nJ] "<<dRMatch[nJ]<<" refdrjt "<<refdrjt<<" i_partonflavor "<<i_partonflavor<<endl;
	flavor_def[0] = true; //all
	flavor_def[1]= isData ? isData : ( dRMatch[nJ] < 0.2 && abs(i_partonflavor) > 0 && abs(i_partonflavor)<6 ) ; // quarks
	flavor_def[2]= isData ? isData : ( dRMatch[nJ] < 0.2 && i_partonflavor == 21); // glouns
	flavor_def[3]= isData ? isData : ( dRMatch[nJ] > 0.3 && i_partonflavor == 0); // pileup
	flavor_def[4] = isData ? isData : !(flavor_def[1] || flavor_def[2] || flavor_def[3]); //rest
	vJet.flavor=0;
	if (flavor_def[0]) vJet.flavor = 0; //will actually always be overwritten
	if (flavor_def[1]) vJet.flavor = 1;
	if (flavor_def[2]) vJet.flavor = 2;
	if (flavor_def[3]) vJet.flavor = 3;
	if (flavor_def[4]) vJet.flavor = 4;

	vJet.PU_loose = bool(jet_PUfullID[nJ] & (1 << 2));
	vJet.PU_medium = bool(jet_PUfullID[nJ] & (1 << 1));
	vJet.PU_tight = bool(jet_PUfullID[nJ] & (1 << 0));
	//vJet.PUdisc = jet_PUdisc[nJ];
	//cout<<"1 "<< flavor_def[1]<<" 2 "<<flavor_def[2]<<" 3 "<<flavor_def[3]<<endl;
	pt_bin[0]=(vJet.Pt() > 20);
	pt_bin[1]=(vJet.Pt() > 20 && vJet.Pt() < 30);
	pt_bin[2]=(vJet.Pt() > 30 && vJet.Pt() < 50);
	pt_bin[3]=(vJet.Pt() > 50 && vJet.Pt() < 100);
	
	eta_bin[0]=(fabs(vJet.Eta()) < 5);
	eta_bin[1]=(fabs(vJet.Eta()) < 2.5);
	eta_bin[2]=(fabs(vJet.Eta()) > 2.5 && fabs(vJet.Eta()) < 2.75);
	eta_bin[3]=(fabs(vJet.Eta()) > 2.75 && fabs(vJet.Eta()) < 3.);
	eta_bin[4]=(fabs(vJet.Eta()) > 3. && fabs(vJet.Eta()) < 5.);
	
	nJet20clean++;
	jets20.push_back(vJet);
	
	for(int i=0;i<nflav;i++){
	  if(!flavor_def[i]) continue;
	  for(int j=0;j<npt;j++){
	    if(!pt_bin[j]) continue;
	    for(int k=0;k<neta;k++){
	      if(!eta_bin[k]) continue;
	      hmLL[i][j][k]->Fill(mLL,myweight);
	      hnpv[i][j][k]->Fill(*npv,myweight);
	      hDRweighted[i][j][k]->Fill(jet_DRweighted[nJ],myweight);
	      hnTot[i][j][k]->Fill(jet_nTot[nJ],myweight);
	      hnCh[i][j][k]->Fill(jet_nCh[nJ],myweight);
	      haxisMajor[i][j][k]->Fill(jet_axisMajor[nJ],myweight);
	      haxisMinor[i][j][k]->Fill(jet_axisMinor[nJ],myweight);
	      hfRing0[i][j][k]->Fill(jet_fRing0[nJ],myweight);
	      hfRing1[i][j][k]->Fill(jet_fRing1[nJ],myweight);
	      hfRing2[i][j][k]->Fill(jet_fRing2[nJ],myweight);
	      hfRing3[i][j][k]->Fill(jet_fRing3[nJ],myweight);
	      hptD[i][j][k]->Fill(jet_ptD[nJ],myweight);
	      hbeta[i][j][k]->Fill(jet_beta[nJ],myweight);
	      hpull[i][j][k]->Fill(jet_pull[nJ],myweight);
	      hPUdisc[i][j][k]->Fill(jet_PUdisc[nJ],myweight);
	      hjetR[i][j][k]->Fill(jet_jetR[nJ],myweight);
	      hjetRchg[i][j][k]->Fill(jet_Rchg[nJ],myweight);
	      hjtpt[i][j][k] ->Fill(vJet.Pt(),myweight);
	      hjteta[i][j][k]->Fill(vJet.Eta(),myweight);

	      float pt = vJet.Pt();
	      float ptratio = pt/Zvector.Pt();
	      float dphi = Zvector.DeltaPhi(vJet);
	      float eta = vJet.Eta();
	      float flav = vJet.flavor;
	      bool loose =  vJet.PU_loose;
	      bool medium =  vJet.PU_medium;
	      bool tight =  vJet.PU_tight;
	      bool matched = (flav == 1 or flav ==2 or flav ==4);

	      //	      cout<<"----nominal njet loop bins "<<i<<" "<< j << " "<<k<<endl;
	      //cout<<"pt: "<< pt <<" eta: "<<eta<<" "<<loose << " "<<medium
	      //  << " "<<tight<<" flav: "<<flav<<endl;
	      if (fabs(dphi) > 2.5 and 0.5 < ptratio and ptratio < 1.5){
		hJetpT_dPhiGeq2p5_All[i][j][k]->Fill(pt, myweight);
		hJetEta_dPhiGeq2p5_All[i][j][k]->Fill(eta, myweight);
		if (loose){
		  hJetpT_dPhiGeq2p5_Pass_loose[i][j][k]->Fill(pt, myweight);	    
		  hJetEta_dPhiGeq2p5_Pass_loose[i][j][k]->Fill(eta, myweight);	    
		}

		if (medium){
		  hJetpT_dPhiGeq2p5_Pass_medium[i][j][k]->Fill(pt, myweight);	    
		  hJetEta_dPhiGeq2p5_Pass_medium[i][j][k]->Fill(eta, myweight);	    
		}

		if (tight){
		  hJetpT_dPhiGeq2p5_Pass_tight[i][j][k]->Fill(pt, myweight);	    
		  hJetEta_dPhiGeq2p5_Pass_tight[i][j][k]->Fill(eta, myweight);	    
		}
	      }
	      if (fabs(dphi) < 1.5){// and 0.5 < ptratio and ptratio < 1.5){
		hJetpT_dPhiLeq1p5_All[i][j][k]->Fill(pt, myweight);
		hJetEta_dPhiLeq1p5_All[i][j][k]->Fill(eta, myweight);
		if (loose){
		  hJetpT_dPhiLeq1p5_Pass_loose[i][j][k]->Fill(pt, myweight);	    
		  hJetEta_dPhiLeq1p5_Pass_loose[i][j][k]->Fill(eta, myweight);	    
		}

		if (medium){
		  hJetpT_dPhiLeq1p5_Pass_medium[i][j][k]->Fill(pt, myweight);	    
		  hJetEta_dPhiLeq1p5_Pass_medium[i][j][k]->Fill(eta, myweight);	    
		}

		if (tight){
		  hJetpT_dPhiLeq1p5_Pass_tight[i][j][k]->Fill(pt, myweight);	    
		  hJetEta_dPhiLeq1p5_Pass_tight[i][j][k]->Fill(eta, myweight);	    
		}
	      }
	      

	      
	    }//pt bin
	  }// eta bin
	}// flavor
      }// njet
      h_nJet20.Fill(nJet20, myweight);
      h_nJet20clean.Fill(nJet20clean, myweight);
      
      if (nJet20clean==0) continue;
      for (int jet =0; jet < jets20.size(); jet++){
	float pt = jets20.at(jet).Pt();
	float ptratio = jets20.at(jet).Pt()/Zvector.Pt();
	float dphi = Zvector.DeltaPhi(jets20.at(jet));
	float eta = jets20.at(jet).Eta();
	float flav = jets20.at(jet).flavor;
	bool loose = jets20.at(jet).PU_loose;
	bool medium = jets20.at(jet).PU_medium;
	bool tight = jets20.at(jet).PU_tight;
	bool matched = (flav == 1 or flav ==2 or flav ==4);
	//cout<<"----njet20 loop"<<endl;
	//cout<<"pt: "<< pt <<" eta: "<<eta<<" "<<loose << " "<<medium
	// << " "<<tight<<" flav: "<<flav<<endl;
	hPtRatio_vs_dphiZJ[0]->Fill(dphi, ptratio);
	hPtRatio_vs_dphiZJ[jets20.at(jet).flavor]->Fill(dphi, ptratio);
	if (fabs(dphi) > 2.5 and 0.5 < ptratio and ptratio < 1.5){
	  h_JetpT_dPhiGeq2p5_All.Fill(pt, myweight);
	  h_JetEta_dPhiGeq2p5_All.Fill(eta, myweight);
	  if (fabs(eta) < 2.5){
	    h_JetpT_dPhiGeq2p5_All_cent.Fill(pt, myweight);
	  }
	  if (fabs(eta) > 3.0 and fabs(eta) < 5.0){
	    h_JetpT_dPhiGeq2p5_All_forw.Fill(pt, myweight);
	  }
	  if (matched){
	    h_JetpT_dPhiGeq2p5_All_QG.Fill(pt, myweight);
	    h_JetEta_dPhiGeq2p5_All_QG.Fill(eta, myweight);
	    if (fabs(eta) < 2.5){
	      h_JetpT_dPhiGeq2p5_All_QG_cent.Fill(pt, myweight);
	    }
	    if (fabs(eta) > 3.0 and fabs(eta) < 5.0){
	      h_JetpT_dPhiGeq2p5_All_QG_forw.Fill(pt, myweight);
	    }
	  }
	  if (loose){
	    h_JetpT_dPhiGeq2p5_Pass_loose.Fill(pt, myweight);	    
	    h_JetEta_dPhiGeq2p5_Pass_loose.Fill(eta, myweight);
	    if (fabs(eta) < 2.5){
	      h_JetpT_dPhiGeq2p5_Pass_loose_cent.Fill(pt, myweight);
	    }
	    if (fabs(eta) > 3.0 and fabs(eta) < 5.0){
	      h_JetpT_dPhiGeq2p5_Pass_loose_forw.Fill(pt, myweight);
	    }
	    if (matched){
	      h_JetpT_dPhiGeq2p5_Pass_loose_QG.Fill(pt, myweight);
	      h_JetEta_dPhiGeq2p5_Pass_loose_QG.Fill(eta, myweight);
	      if (fabs(eta) < 2.5){
		h_JetpT_dPhiGeq2p5_Pass_loose_QG_cent.Fill(pt, myweight);
	      }
	      if (fabs(eta) > 3.0 and fabs(eta) < 5.0){
		h_JetpT_dPhiGeq2p5_Pass_loose_QG_forw.Fill(pt, myweight);
	      }
	    }
	  }
	  if (medium){
	    h_JetpT_dPhiGeq2p5_Pass_medium.Fill(pt, myweight);	    
	    h_JetEta_dPhiGeq2p5_Pass_medium.Fill(eta, myweight);
	    if (flav == 1 or flav == 2 or flav == 4){
	      h_JetpT_dPhiGeq2p5_Pass_medium_QG.Fill(pt, myweight);
	      h_JetEta_dPhiGeq2p5_Pass_medium_QG.Fill(eta, myweight);
	    }
	  }

	  if (tight){
	    h_JetpT_dPhiGeq2p5_Pass_tight.Fill(pt, myweight);	    
	    h_JetEta_dPhiGeq2p5_Pass_tight.Fill(eta, myweight);
	  
	    if (matched){
	      h_JetpT_dPhiGeq2p5_Pass_tight_QG.Fill(pt, myweight);
	      h_JetEta_dPhiGeq2p5_Pass_tight_QG.Fill(eta, myweight);
	    }
	  }
	}
	
	if (fabs(dphi) < 1.5 and 0.5 < ptratio and ptratio < 1.5){
	  h_JetpT_dPhiLeq1p5_All.Fill(pt, myweight);
	  h_JetEta_dPhiLeq1p5_All.Fill(eta, myweight);
	  if (fabs(eta) < 2.5){
	    h_JetpT_dPhiLeq1p5_All_cent.Fill(pt, myweight);
	  }
	  if (fabs(eta) > 3.0 and fabs(eta) < 5.0){
	    h_JetpT_dPhiLeq1p5_All_forw.Fill(pt, myweight);
	  }
	  if (loose){
	    h_JetpT_dPhiLeq1p5_Pass_loose.Fill(pt, myweight);	    
	    h_JetEta_dPhiLeq1p5_Pass_loose.Fill(eta, myweight);
	    if (fabs(eta) < 2.5){
	      h_JetpT_dPhiLeq1p5_Pass_loose_cent.Fill(pt, myweight);
	    }
	    if (fabs(eta) > 3.0 and fabs(eta) < 5.0){
	      h_JetpT_dPhiLeq1p5_Pass_loose_forw.Fill(pt, myweight);
	    }
	  }

	  if (medium){
	    h_JetpT_dPhiLeq1p5_Pass_medium.Fill(pt, myweight);	    
	    h_JetEta_dPhiLeq1p5_Pass_medium.Fill(eta, myweight);
	  }

	  if (tight){
	    h_JetpT_dPhiLeq1p5_Pass_tight.Fill(pt, myweight);	    
	    h_JetEta_dPhiLeq1p5_Pass_tight.Fill(eta, myweight);
	  }
	}
	
	}
      h_mLL_after.Fill(mLL,myweight);
      h_npv.Fill(*npv,myweight);
      
      //cout<<"zpt "<<Zvector.Pt()<<" jetpt "<<jets20.at(0).Pt()<< "flavor "<<jets20.at(0).flavor<<" dphi "<<Zvector.DeltaPhi(jets20.at(0))<<endl;
      //      hPtRatio_vs_dphiZJ[0]->Fill(Zvector.DeltaPhi(jets20.at(0)), jets20.at(0).Pt()/Zvector.Pt());
      //hPtRatio_vs_dphiZJ[jets20.at(0).flavor]->Fill(Zvector.DeltaPhi(jets20.at(0)), jets20.at(0).Pt()/Zvector.Pt());
    }//event loop

    

    cout << endl << "Finished event loop" << endl;

    //write out cutflow
    ofstream tfile;
    TString textfilename = "CMG_cutflow_"+outname+".txt";
    tfile.open(textfilename);
    tfile << "########################################" << endl;
    tfile << "Cut efficiency numbers:" << endl;

    //write out histograms
    TFile * outf;
    TString rootfilename = "CMG_"+outname+"_his.root";
    outf = new TFile(rootfilename,"RECREATE");
    char FOLDER[100];
    h_MCweight.Write();
    h_mLL.Write();
    h_mLL_after.Write();
    h_npv.Write();
    h_nJet20.Write();
    h_nJet20clean.Write();
    h_m1_iso.Write();
    h_m2_iso.Write();
    h_1stJetpT.Write();
    h_1stJeteta.Write();

    h_JetpT_dPhiGeq2p5_All_cent.Write();
    h_JetpT_dPhiGeq2p5_All_QG_cent.Write();
    h_JetpT_dPhiLeq1p5_All_cent.Write();

    h_JetpT_dPhiGeq2p5_All_forw.Write();
    h_JetpT_dPhiGeq2p5_All_QG_forw.Write();
    h_JetpT_dPhiLeq1p5_All_forw.Write();

    h_JetpT_dPhiGeq2p5_Pass_loose_cent.Write();
    h_JetpT_dPhiGeq2p5_Pass_loose_QG_cent.Write();
    h_JetpT_dPhiLeq1p5_Pass_loose_cent.Write();

    h_JetpT_dPhiGeq2p5_Pass_loose_forw.Write();
    h_JetpT_dPhiGeq2p5_Pass_loose_QG_forw.Write();
    h_JetpT_dPhiLeq1p5_Pass_loose_forw.Write();

    h_JetpT_dPhiGeq2p5_Pass_loose.Write();
    h_JetpT_dPhiGeq2p5_Pass_loose_QG.Write();
    h_JetpT_dPhiLeq1p5_Pass_loose.Write();
    h_JetpT_dPhiGeq2p5_Pass_medium.Write();
    h_JetpT_dPhiGeq2p5_Pass_medium_QG.Write();
    h_JetpT_dPhiLeq1p5_Pass_medium.Write();
    h_JetpT_dPhiGeq2p5_Pass_tight.Write();
    h_JetpT_dPhiGeq2p5_Pass_tight_QG.Write();
    h_JetpT_dPhiLeq1p5_Pass_tight.Write();
    h_JetpT_dPhiGeq2p5_All.Write();
    h_JetpT_dPhiGeq2p5_All_QG.Write();
    h_JetpT_dPhiLeq1p5_All.Write();


    h_JetEta_dPhiGeq2p5_Pass_loose.Write();
    h_JetEta_dPhiGeq2p5_Pass_loose_QG.Write();
    h_JetEta_dPhiLeq1p5_Pass_loose.Write();
    h_JetEta_dPhiGeq2p5_Pass_medium.Write();
    h_JetEta_dPhiGeq2p5_Pass_medium_QG.Write();
    h_JetEta_dPhiLeq1p5_Pass_medium.Write();
    h_JetEta_dPhiGeq2p5_Pass_tight.Write();
    h_JetEta_dPhiGeq2p5_Pass_tight_QG.Write();
    h_JetEta_dPhiLeq1p5_Pass_tight.Write();
    h_JetEta_dPhiGeq2p5_All.Write();
    h_JetEta_dPhiGeq2p5_All_QG.Write();
    h_JetEta_dPhiLeq1p5_All.Write();

    outf->mkdir("Variables");
    outf->mkdir("Efficiency");
    for(int i=0;i<nflav;i++){
      hPtRatio_vs_dphiZJ[i]->Write();
      for(int j=0;j<npt;j++){
	for(int k=0;k<neta;k++){
	  outf->cd("Variables");
	  hmLL[i][j][k]->Write();
	  hDRweighted[i][j][k]->Write();
	  hnpv[i][j][k]->Write();
	  hnTot[i][j][k]->Write();
	  hnCh[i][j][k]->Write();
	  haxisMajor[i][j][k]->Write();
	  haxisMinor[i][j][k]->Write();
	  hfRing0[i][j][k]->Write();
	  hfRing1[i][j][k]->Write();
	  hfRing2[i][j][k]->Write();
	  hfRing3[i][j][k]->Write();
	  hptD[i][j][k]->Write();
	  hbeta[i][j][k]->Write();
	  hpull[i][j][k]->Write();
	  hPUdisc[i][j][k]->Write();
	  hjetR[i][j][k]->Write();
	  hjetRchg[i][j][k]->Write();
	  hjtpt[i][j][k]->Write();
	  hjteta[i][j][k]->Write();

 	  outf->cd("Efficiency");
	  hJetpT_dPhiGeq2p5_All[i][j][k]->Write();
	  hJetpT_dPhiLeq1p5_All[i][j][k]->Write();
	  hJetpT_dPhiGeq2p5_Pass_loose[i][j][k]->Write();
	  hJetpT_dPhiLeq1p5_Pass_loose[i][j][k]->Write();

	  hJetEta_dPhiGeq2p5_All[i][j][k]->Write();
	  hJetEta_dPhiLeq1p5_All[i][j][k]->Write();
	  hJetEta_dPhiGeq2p5_Pass_loose[i][j][k]->Write();
	  hJetEta_dPhiLeq1p5_Pass_loose[i][j][k]->Write();

	  hJetpT_dPhiGeq2p5_Pass_medium[i][j][k]->Write();
	  hJetpT_dPhiLeq1p5_Pass_medium[i][j][k]->Write();
	  hJetEta_dPhiGeq2p5_Pass_medium[i][j][k]->Write();
	  hJetEta_dPhiLeq1p5_Pass_medium[i][j][k]->Write();

	  hJetpT_dPhiGeq2p5_Pass_tight[i][j][k]->Write();
	  hJetpT_dPhiLeq1p5_Pass_tight[i][j][k]->Write();
	  hJetEta_dPhiGeq2p5_Pass_tight[i][j][k]->Write();
	  hJetEta_dPhiLeq1p5_Pass_tight[i][j][k]->Write();

	}
      }
    }

    //first the main histograms
    cout<<"start with normal histograms"<<endl;
    for(int cj = 0; cj < CutNumb; cj++)
    {
	outf->cd();
    }
    cout<<"done with nested histograms"<<endl;




    outt = tout->GetCurrentFile(); //to get the pointer to the current file
    outt->Write();
    outt->Close();


    cout<<"done writing small tree"<<endl;
}
