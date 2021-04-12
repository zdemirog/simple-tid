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
/// \file analysis/AnaEx01/include/HistoManager.hh
/// \brief Definition of the HistoManager class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef B1HistoManager_h
#define B1HistoManager_h 1

#include <iostream>
#include <map>
#include <string>
#include "globals.hh"
#include "g4root.hh"
//#include "g4csv.hh"
//#include "g4xml.hh"
#include "TAxis.h"
#include "TH1.h"
class HistoManager
{
  public:
    HistoManager();
   ~HistoManager();

    void Book(G4String RootFileName);
    void Save();

     void FillHisto(G4int id, G4double e, G4double weight = 1.0);
     void Normalize(G4int id, G4double fac);


    void FillNtuple(std::map<G4int,std::map<G4int,G4double>> Ntuples);

    void FillNtupleEv(G4int evNr);

    // void PrintStatistic();

    std::map<G4int,std::map<G4int,G4double>> Branches;

    std::map<G4int,std::map<G4int,std::vector<G4double>>> hVolumes;

    
  private:
    G4bool fFactoryOn;

};

#endif
