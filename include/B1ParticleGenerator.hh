#ifndef B1ParticleGenerator_h
#define B1ParticleGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "B1Analysis.hh"
#include <TFile.h>
#include "TH1F.h"

class G4GenericMessenger;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class B1ParticleGenerator
{
public:
  B1ParticleGenerator();
  virtual ~B1ParticleGenerator();

  // method from the base class
    virtual void GetValue(int val) const;
    void SetValue(int value) { fParticle = value ; }
    G4int GetParValue() const { return fParticle; }
  
private:
    void DefineCommands();
    G4GenericMessenger *fMessenger;
      int fParticle;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
