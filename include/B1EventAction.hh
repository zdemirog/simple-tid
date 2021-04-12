#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class B1RunAction;
class HistoManager;

/// Event action class
///

class B1EventAction : public G4UserEventAction
{
public:
  B1EventAction(B1RunAction* runAction, HistoManager* histo);
  virtual ~B1EventAction();

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

  void AddEdep(G4int vol, G4double edep) { fEdep[vol] += edep; }
  void AddNIEL(G4int vol, G4double edep) { fNIEL[vol] += edep; }

  G4int GetCurrentEvNr(){return currentEv;}

private:
  B1RunAction* fRunAction;
  HistoManager*  fHistoManager;
  std::vector<G4double>     fEdep;
  std::vector<G4double>     fNIEL;
  G4int currentEv;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
