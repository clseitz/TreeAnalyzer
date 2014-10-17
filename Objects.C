#include "Objects.h"

using namespace std;

// global Object vectors
vector<TLorentzVector> goodJet;
vector<TLorentzVector> goodBJet;
vector<TLorentzVector> goodLep;
vector<TLorentzVector> goodEl;
vector<TLorentzVector> goodMu;
vector<TLorentzVector> vetoLep;
vector<TLorentzVector> vetoEl;
vector<TLorentzVector> vetoMu;

// Object cuts
Float_t goodEta = 2.4;

//leptons
Float_t goodElPt = 20.0;
Float_t goodMuPt = 20.0;
Float_t goodLepPt = 20.0;
Float_t vetoLepPt = 10.0;

Float_t goodEl_relIso03 = 0.14;
Float_t goodMu_relIso03 = 0.12;
Float_t goodLep_relIso03 = 0.15;

Bool_t goodMu_tightID = true;

//jets
Float_t goodJetPt = 40.0;
Float_t goodJetBtagCSV = 0.679;

// variables for tree
const int arrayN = 50;
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
Int_t   LepGood_pdgId[arrayN];
Bool_t  LepGood_tightID[arrayN];


void GetLeptons(EasyChain * tree){

    // clearing objects
    goodLep.clear();
    goodEl.clear();
    goodMu.clear();

    vetoLep.clear();
    vetoEl.clear();
    vetoMu.clear();

    nLepGood = 0;
    nMuGood = 0;
    nElGood = 0;

    nLepVeto = 0;
    nElVeto = 0;
    nMuVeto = 0;

    // filling objects from tree
    int nLep = tree->Get(nLep,"nLepGood");
    tree->Get(LepGood_pt[0],"LepGood_pt");
    tree->Get(LepGood_eta[0],"LepGood_eta");
    tree->Get(LepGood_phi[0],"LepGood_phi");
    tree->Get(LepGood_mass[0],"LepGood_mass");
    tree->Get(LepGood_relIso03[0],"LepGood_relIso03");
    tree->Get(LepGood_pdgId[0],"LepGood_pdgId");
    tree->Get(LepGood_tightID[0],"LepGood_tightId");

    for(int ilep = 0; ilep < nLep; ilep++){

        TLorentzVector dummyLep;
        dummyLep.SetPtEtaPhiM(LepGood_pt[ilep],LepGood_eta[ilep],LepGood_phi[ilep],LepGood_mass[ilep]);

/*
  bool isGoodMu = false;
  bool isGoodEl = false;
  bool isGoodLep = false;
  bool isVetoLep = false;
*/
        bool isVetoMu = false;
        bool isVetoEl = false;

        // common cuts for all leptons (good and veto leps pass)
        if(dummyLep.Pt() < vetoLepPt || fabs(dummyLep.Eta()) > goodEta)
            continue;

        // Muon cuts
        if(abs(LepGood_pdgId[ilep]) == 13)
            if( dummyLep.Pt() > goodMuPt)
                if(LepGood_tightID[ilep]){
                    if(LepGood_relIso03[ilep] < goodMu_relIso03){
//			isGoodMu = true;

                        goodMu.push_back(dummyLep);
                        nMuGood++;

                        continue;
                    }
                    else{
                        isVetoMu = true;
			nMuVeto++;
		    }
		}

        // Electron cuts
        if(abs(LepGood_pdgId[ilep]) == 11)
            if( dummyLep.Pt() > goodElPt){
//              if(LepGood_tightID[ilep])
                if(LepGood_relIso03[ilep] < goodEl_relIso03){
//                    isGoodEl = true;

                    goodEl.push_back(dummyLep);
                    nElGood++;

                    continue;
                }
                else{
                    isVetoEl = true;
		    nElVeto++;
		}
	    }

        // Only non-good El or Mu will pass => veto leptons
	if(isVetoEl || isVetoMu){
	    vetoLep.push_back(dummyLep);
	    nLepVeto++;
	}
    }

    cout << "Get leptons summary: total number of Leptons = \t" << nLep << endl;
    cout << "Number of good Muons = \t" << nMuGood << " and veto Mu = \t" << nMuVeto << endl;
    cout << "Number of good Electrons = \t" << nElGood  << " and veto El = \t" << nElVeto << endl;
    cout << "Number of veto leptons = \t" << nLepVeto << endl;

}

void GetGoodLeptons(EasyChain * tree){

    goodLep.clear(); vetoLep.clear();
    goodEl.clear(); goodMu.clear();

    nLepGood = 0;
    nMuGood = 0;
    nElGood = 0;

    int nLep = tree->Get(nLep,"nLepGood");
    tree->Get(LepGood_pt[0],"LepGood_pt");
    tree->Get(LepGood_eta[0],"LepGood_eta");
    tree->Get(LepGood_phi[0],"LepGood_phi");
    tree->Get(LepGood_mass[0],"LepGood_mass");
    tree->Get(LepGood_relIso03[0],"LepGood_relIso03");
    tree->Get(LepGood_pdgId[0],"LepGood_pdgId");

    for(int ilep = 0; ilep < nLep; ilep++){

        TLorentzVector dummyLep;
        dummyLep.SetPtEtaPhiM(LepGood_pt[ilep],LepGood_eta[ilep],LepGood_phi[ilep],LepGood_mass[ilep]);

        if(dummyLep.Pt() > vetoLepPt && fabs(dummyLep.Eta()) < goodEta){
            vetoLep.push_back(dummyLep);
            if( dummyLep.Pt() > goodLepPt && LepGood_relIso03[ilep] < 0.15){ //TODO: need to adjust isolation
                goodLep.push_back(dummyLep);
                if(abs(LepGood_pdgId[ilep]) == 11){
                    goodEl.push_back(dummyLep);
                    nElGood++;
                }
                if(abs(LepGood_pdgId[ilep]) == 13){
                    goodMu.push_back(dummyLep);
                    nMuGood++;
                }
                nLepGood++;
            }
        }
    }
}

void GetJets(EasyChain * tree){
    goodJet.clear();
    goodBJet.clear();

//    ST=0;

    HT40=0;
    nJetGood = 0;
    nBJetGood = 0;

    int nJet = tree->Get(nJet,"nJet");
    tree->Get(Jet_pt[0],"Jet_pt");
    tree->Get(Jet_eta[0],"Jet_eta");
    tree->Get(Jet_phi[0],"Jet_phi");
    tree->Get(Jet_mass[0],"Jet_mass");
    tree->Get(Jet_btagCSV[0],"Jet_btagCSV");

    for(int ijet = 0; ijet < nJet; ijet++)
    {
        TLorentzVector dummyJet;
        dummyJet.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);
        //put pt, eta, cuts and other stuff
        //jet are already cleaned from all loose leptons that are in LepGood
        if(dummyJet.Pt() > goodJetPt && fabs(dummyJet.Eta()) < goodEta){
            goodJet.push_back(dummyJet);
            nJetGood++;
            HT40 = HT40 + dummyJet.Pt();

	    // filling B jets
	    if(Jet_btagCSV[ijet] > goodJetBtagCSV){
		goodBJet.push_back(dummyJet);
		nBJetGood++;
	    }
        }
    }

    cout << "Get jets summary: total number of jets = \t" << nJet << endl;
    cout << "Number of good jetss = \t" << nJetGood  << " and b jets = \t" << nBJetGood << endl;

}
