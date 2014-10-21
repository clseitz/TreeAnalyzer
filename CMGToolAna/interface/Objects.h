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
void GetLeptons(EasyChain * tree);

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
extern std::vector<TLorentzVector> goodLep;
extern std::vector<TLorentzVector> goodEl;
extern std::vector<TLorentzVector> goodMu;
extern std::vector<TLorentzVector> vetoLep;
extern std::vector<TLorentzVector> vetoEl;
extern std::vector<TLorentzVector> vetoMu;


int nLepGood;
int nMuGood;
int nElGood;

int nLepVeto;
int nElVeto;
int nMuVeto;

int nJetGood;
int nBJetGood;

Float_t HT40;
Float_t ST;

#endif
