//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file analysis/AnaEx01/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "B1HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFactoryOn(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book(G4String fileName)
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
      
  // Create directories
  analysisManager->SetHistoDirectoryName("histos");
  analysisManager->SetNtupleDirectoryName("ntuples");
    
  // Open an output file
  G4bool fileOpen = analysisManager->OpenFile(fileName);
  if (! fileOpen) {
    G4cerr << "\n---> HistoManager::Book(): cannot open "
           << analysisManager->GetFileName() << G4endl;
    return;
  }
  
  // Create histograms.
  // Histogram ids are generated automatically starting from 0.
  // The start value can be changed by:
  // analysisManager->SetFirstHistoId(1);
  
  analysisManager->CreateH1("Energy","Energy (MeV)", 121, -8, 4.1);
  G4int bins = analysisManager->GetH1Nbins(0);
  G4double from = analysisManager->GetH1Xmin(0);
  G4double to = analysisManager->GetH1Xmax(0);
  G4double width = (to - from) / bins;
  std::vector<G4double> new_bins;
  for (int i = 0; i <= bins; i++) {new_bins.push_back(pow(10, from + i * width));}
  analysisManager->SetH1(0, new_bins);

  // Create ntuples.
  // Ntuples ids are generated automatically starting from 0.
  // The start value can be changed by:
  // analysisManager->SetFirstMtupleId(1);
  // Create ntuple (id = 0)
  analysisManager->CreateNtuple("SBSbunkerExitvF", "SBS Bunker Exits Front");
  analysisManager->CreateNtupleDColumn("ExitvF_evNr",hVolumes[0][0]);
  analysisManager->CreateNtupleDColumn("ExitvF_PDGencoding",hVolumes[0][1]);
  analysisManager->CreateNtupleDColumn("ExitvF_TrackID",hVolumes[0][2]);
  analysisManager->CreateNtupleDColumn("ExitvF_ParentTrackID",hVolumes[0][3]);
  analysisManager->CreateNtupleDColumn("ExitvF_x",hVolumes[0][4]);
  analysisManager->CreateNtupleDColumn("ExitvF_y",hVolumes[0][5]);
  analysisManager->CreateNtupleDColumn("ExitvF_z",hVolumes[0][6]);
  analysisManager->CreateNtupleDColumn("ExitvF_px",hVolumes[0][7]);
  analysisManager->CreateNtupleDColumn("ExitvF_py",hVolumes[0][8]);
  analysisManager->CreateNtupleDColumn("ExitvF_pz",hVolumes[0][9]);
  analysisManager->CreateNtupleDColumn("ExitvF_theta",hVolumes[0][10]);
  analysisManager->CreateNtupleDColumn("ExitvF_phi",hVolumes[0][11]);
  analysisManager->CreateNtupleDColumn("ExitvF_totEnergy",hVolumes[0][12]);
  analysisManager->CreateNtupleDColumn("ExitvF_kEnergy",hVolumes[0][13]);
  analysisManager->CreateNtupleDColumn("ExitvF_length",hVolumes[0][14]);
  analysisManager->CreateNtupleDColumn("ExitvF_prevolumes",hVolumes[0][15]);
  analysisManager->CreateNtupleDColumn("ExitvF_postvolumes",hVolumes[0][16]);
  analysisManager->FinishNtuple();
    
  analysisManager->CreateNtuple("SBSbunkerEntervB", "SBS Bunker Enter Back");
  analysisManager->CreateNtupleDColumn("EntervB_evNr",hVolumes[1][0]);
  analysisManager->CreateNtupleDColumn("EntervB_PDGencoding",hVolumes[1][1]);
  analysisManager->CreateNtupleDColumn("EntervB_TrackID",hVolumes[1][2]);
  analysisManager->CreateNtupleDColumn("EntervB_ParentTrackID",hVolumes[1][3]);
  analysisManager->CreateNtupleDColumn("EntervB_x",hVolumes[1][4]);
  analysisManager->CreateNtupleDColumn("EntervB_y",hVolumes[1][5]);
  analysisManager->CreateNtupleDColumn("EntervB_z",hVolumes[1][6]);
  analysisManager->CreateNtupleDColumn("EntervB_px",hVolumes[1][7]);
  analysisManager->CreateNtupleDColumn("EntervB_py",hVolumes[1][8]);
  analysisManager->CreateNtupleDColumn("EntervB_pz",hVolumes[1][9]);
  analysisManager->CreateNtupleDColumn("EntervB_theta",hVolumes[1][10]);
  analysisManager->CreateNtupleDColumn("EntervB_phi",hVolumes[1][11]);
  analysisManager->CreateNtupleDColumn("EntervB_totEnergy",hVolumes[1][12]);
  analysisManager->CreateNtupleDColumn("EntervB_kEnergy",hVolumes[1][13]);
  analysisManager->CreateNtupleDColumn("EntervB_length",hVolumes[1][14]);
  analysisManager->CreateNtupleDColumn("EntervB_prevolumes",hVolumes[1][15]);
  analysisManager->CreateNtupleDColumn("EntervB_postvolumes",hVolumes[1][16]);
  analysisManager->FinishNtuple();
        
  fFactoryOn = true;

  G4cout << "\n----> Output file is open in "
         << analysisManager->GetFileName() << "."
         << analysisManager->GetFileType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
  if (! fFactoryOn) return;
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
   
  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
      
  delete G4AnalysisManager::Instance();
  fFactoryOn = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(ih, xbin, weight);
    

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Normalize(G4int ih, G4double fac)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4H1* h1 = analysisManager->GetH1(ih);
  if (h1) h1->scale(fac);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple(std::map<G4int,std::map<G4int,G4double>> Ntuples)
{
    
  Branches = Ntuples;

  for (const auto& p : Branches ) {
    for (const auto& q : Branches[p.first] ){
              
      if(q.first == 0){ // dont fill every time event number
      }
      else {
	hVolumes[p.first][q.first].push_back(Branches[p.first][q.first]);
      }
    }
    Branches[p.first].clear();
    Ntuples[p.first].clear();

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtupleEv(G4int evNr){

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  for (const auto& p : hVolumes ) {

    hVolumes[p.first][0].push_back(evNr);

    analysisManager->AddNtupleRow(p.first);

    for (const auto& q : hVolumes[p.first] ){
      hVolumes[p.first][q.first].clear();
    }

  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
