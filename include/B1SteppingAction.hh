#ifndef B1SteppingAction_h
#define B1SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <vector>
class B1EventAction;
class B1RunAction;
class HistoManager;
class G4LogicalVolume;

/// Stepping action class
/// 

class B1SteppingAction : public G4UserSteppingAction
{
public:
  B1SteppingAction(B1EventAction* eventAction, HistoManager* histo);
  virtual ~B1SteppingAction();

  // method from the base class
  virtual void UserSteppingAction(const G4Step*);

private:
  B1EventAction*  fEventAction;
  std::vector<G4LogicalVolume*> fScoringVolume;
  G4int evNr;
  G4double edepStep ;
  std::map<G4int,std::map<G4int,G4double>> Ntuples;
  HistoManager*  fHistoManager;
  G4String  ParticleName;
  G4int ParticlePDGcode;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
