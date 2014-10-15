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

void GetGoodLeptons(EasyChain * tree);
void GetGoodJets(EasyChain * tree);

// to be written
/*
void GetElectrons(EasyChain * tree);
void GetMuons(EasyChain * tree);
void GetLeptons(EasyChain * tree);

void GetGoodElectrons(EasyChain * tree);
void GetGoodMuons(EasyChain * tree);

void GetJets(EasyChain * tree);
*/

// global object variables
extern std::vector<TLorentzVector> goodJet;
extern std::vector<TLorentzVector> goodLep;
extern std::vector<TLorentzVector> goodEl;
extern std::vector<TLorentzVector> goodMu;
extern std::vector<TLorentzVector> vetoLep;

int nLepGood;
int nMuGood;
int nElGood;

int nJetGood;

Float_t HT40;
Float_t ST;

#endif
