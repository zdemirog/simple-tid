#include "B1EventAction.hh"
#include "B1RunAction.hh"
#include "B1HistoManager.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction, HistoManager* histo)
  : G4UserEventAction(),
    fRunAction(runAction),fHistoManager(histo)
{
  for(int i=0;i<3;i++) fEdep.push_back(0);
  currentEv=0;

} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  for(int i=0;i<3;i++) fEdep[i]=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  currentEv++;
  G4int  evNr = event->GetEventID();
 
  fHistoManager->FillNtupleEv(evNr);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
