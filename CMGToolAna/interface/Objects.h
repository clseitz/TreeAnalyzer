#ifndef OBJECTS_H
#define OBJECTS_H

#include "NtupleTools3.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>

// shouldn't be using 'using' in header files
//using namespace std;

/* Definition of objects:
 * leptons, muons, electrons
 * jets
 */


void GetJets(EasyChain * tree);
void GetMET(EasyChain * tree);
void GetGenMET(EasyChain * tree);

void GetLeptons(EasyChain * tree);
void GetGenLeptons(EasyChain * tree);
void GetGenLeptonsFromTau(EasyChain * tree);
void GetGenTaus(EasyChain * tree);

// to be written ?
/*
  void GetElectrons(EasyChain * tree);
  void GetMuons(EasyChain * tree);
  void GetGoodLeptons(EasyChain * tree);
  void GetGoodElectrons(EasyChain * tree);
  void GetGoodMuons(EasyChain * tree);
*/

// global object variables
extern std::vector<TLorentzVector> goodJet;
extern std::vector<TLorentzVector> goodBJet;

extern TLorentzVector MET;
extern TLorentzVector genMET;
extern TLorentzVector METnoPU;

extern std::vector<TLorentzVector> goodLep;
extern std::vector<TLorentzVector> goodEl;
extern std::vector<TLorentzVector> goodMu;

extern std::vector<TLorentzVector> vetoLep;
extern std::vector<TLorentzVector> vetoEl;
extern std::vector<TLorentzVector> vetoMu;

extern std::vector<TLorentzVector> genLep;
extern std::vector<TLorentzVector> genEl;
extern std::vector<TLorentzVector> genMu;
extern std::vector<TLorentzVector> genTau;

extern std::vector<TLorentzVector> genLepFromTau;
extern std::vector<TLorentzVector> genElFromTau;
extern std::vector<TLorentzVector> genMuFromTau;

// objects number can be aslo detemined as object.size()

Int_t nLepGood;
Int_t nMuGood;
Int_t nElGood;

Int_t nLepVeto;
Int_t nElVeto;
Int_t nMuVeto;

Int_t nJetGood;
Int_t nBJetGood;

Int_t nGenTau;
Int_t nGenLep;
Int_t nGenLepFromTau;

// global variables

Float_t HT40;
Float_t ST;

#endif
