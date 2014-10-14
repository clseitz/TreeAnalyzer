#include "NtupleTools3.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>

using namespace std;

/* Definition of objects:
 * leptons, muons, electrons
 * jets
 */

/*
void GetElectrons(EasyChain * tree);
void GetMuons(EasyChain * tree);
void GetLeptons(EasyChain * tree);

void GetGoodElectrons(EasyChain * tree);
void GetGoodMuons(EasyChain * tree);

void GetJets(EasyChain * tree);
*/

void GetGoodLeptons(EasyChain * tree);
void GetGoodJets(EasyChain * tree);

// global object variables
vector<TLorentzVector> goodJet;
vector<TLorentzVector> goodLep;
vector<TLorentzVector> goodEl;
vector<TLorentzVector> goodMu;
vector<TLorentzVector> vetoLep;

int nLepGood;
int nMuGood;
int nElGood;

int nJetGood;

Float_t HT40;
Float_t ST;
