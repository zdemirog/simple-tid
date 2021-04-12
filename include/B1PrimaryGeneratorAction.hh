#ifndef B1PrimaryGeneratorAction_h
#define B1PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "B1Analysis.hh"
#include <TFile.h>
#include "TH1F.h"
#include "TFile.h"

class G4ParticleGun;
class G4Event;
class G4Box;
class G4GenericMessenger;
class HistoManager;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class B1PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  B1PrimaryGeneratorAction(HistoManager* histo);
  //virtual void DefineCommands();
  virtual ~B1PrimaryGeneratorAction();

  // method from the base class
  virtual void GeneratePrimaries(G4Event*);         
  
  // method to access particle gun
  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
private:
  HistoManager*  fHistoManager;
  G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
  TFile *fin;
  TH1F *hSBS;
  G4GenericMessenger* fMessenger;
  G4double totenergy ;
  G4double fEnergy;
  G4double fX0min,fX0max, fY0min,fY0max, fZ0;
  G4double fdThmin, fdThmax;
  G4double fdPhmin, fdPhmax;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
