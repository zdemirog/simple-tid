#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1RunAction.hh"
#include "B1HistoManager.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Gamma.hh"
#include "G4Neutron.hh"
#include <sstream>
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "B1Analysis.hh"
#include "G4Track.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4PhysicalConstants.hh"
#include "G4ProcessManager.hh"
#include "G4EmCalculator.hh"
#include "G4UnitsTable.hh"
#include "G4NIELCalculator.hh"
#include "G4ICRU49NuclearStoppingModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction, HistoManager* histo)
  : G4UserSteppingAction(),
    fEventAction(eventAction),
    fHistoManager(histo)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  /// Get Event Number
  evNr = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
  if (fScoringVolume.size()==0) { 
    const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }

  //track informations
  G4Track* track = step->GetTrack();
  G4StepPoint* prePoint   = step->GetPreStepPoint();
  G4StepPoint* postPoint = step->GetPostStepPoint();
  G4VPhysicalVolume* prePV= prePoint->GetPhysicalVolume();
  G4VPhysicalVolume* postPV= postPoint->GetPhysicalVolume();

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();

  //G4Material* material = volume->GetMaterial();
  G4int found = -1;
  for(unsigned long i=0;i<fScoringVolume.size();i++)
    if(volume == fScoringVolume[i])
      found = i;

  // check if we are in scoring volume
  if (found==-1) return;
  // collect energy deposited in this step
  edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(found,edepStep);
 
  if(track->GetNextVolume() != 0){
    
     
    if (prePV->GetName()=="front" && postPV->GetName() != "World" && postPoint->GetStepStatus() == fGeomBoundary) {
      // leaving the volume front
      G4double preVol=0;
      G4double postVol=0;
          
      if(prePV->GetName() == "World") {preVol = 1.;}
      else if(prePV->GetName() == "front") {preVol = 2.;}
      else if(prePV->GetName() == "mid") {preVol = 3.;}
      else if(prePV->GetName() == "back") {preVol = 4.;}
      else {preVol = 5.;}

      if(postPV->GetName() == "World") {postVol = 1.;}
      else if(postPV->GetName() == "front") {postVol = 2.;}
      else if(postPV->GetName() == "mid")   {postVol = 3.;}
      else if(postPV->GetName() == "back")  {postVol = 4.;}
      else {postVol = 5.;}
          
      Ntuples[0][0]  = evNr;
      Ntuples[0][1]  = track->GetDefinition()->GetPDGEncoding();
      Ntuples[0][2]  = track->GetTrackID();
      Ntuples[0][3]  = track->GetParentID();
      Ntuples[0][4]  = postPoint->GetPosition().getX();
      Ntuples[0][5]  = postPoint->GetPosition().getY();
      Ntuples[0][6]  = postPoint->GetPosition().getZ();
      Ntuples[0][7]  = postPoint->GetMomentum().getX();
      Ntuples[0][8]  = postPoint->GetMomentum().getY();
      Ntuples[0][9]  = postPoint->GetMomentum().getZ();
      Ntuples[0][10] = postPoint->GetMomentumDirection().theta();
      Ntuples[0][11] = postPoint->GetMomentumDirection().phi();
      Ntuples[0][12] = postPoint->GetTotalEnergy();
      Ntuples[0][13] = postPoint->GetKineticEnergy();
      Ntuples[0][14] = step->GetStepLength();
      Ntuples[0][15] = preVol;
      Ntuples[0][16] = postVol;

      fHistoManager->FillNtuple(Ntuples);
      Ntuples.clear();
    
    }

      
    if (postPV->GetName() == "back" && prePoint->GetStepStatus() == fGeomBoundary) {
      // entered the volume back, or within it
      G4double preVol=0;
      G4double postVol=0;
      if(prePV->GetName() == "World") {preVol = 1.;}
      else if(prePV->GetName() == "front") {preVol = 2.;}
      else if(prePV->GetName() == "mid") {preVol = 3.;}
      else if(prePV->GetName() == "back") {preVol = 4.;}
      else {preVol = 5.;}

      if(postPV->GetName() == "World") {postVol = 1.;}
      else if(postPV->GetName() == "front") {postVol = 2.;}
      else if(postPV->GetName() == "mid")   {postVol = 3.;}
      else if(postPV->GetName() == "back")  {postVol = 4.;}
      else {postVol = 5.;}

      Ntuples[1][0]  = evNr;
      Ntuples[1][1]  = track->GetDefinition()->GetPDGEncoding();
      Ntuples[1][2]  = track->GetTrackID();
      Ntuples[1][3]  = track->GetParentID();
      Ntuples[1][4]  = prePoint->GetPosition().getX();
      Ntuples[1][5]  = prePoint->GetPosition().getY();
      Ntuples[1][6]  = prePoint->GetPosition().getZ();
      Ntuples[1][7]  = prePoint->GetMomentum().getX();
      Ntuples[1][8]  = prePoint->GetMomentum().getY();
      Ntuples[1][9]  = prePoint->GetMomentum().getZ();
      Ntuples[1][10] = prePoint->GetMomentumDirection().theta();
      Ntuples[1][11] = prePoint->GetMomentumDirection().phi();
      Ntuples[1][12] = prePoint->GetTotalEnergy();
      Ntuples[1][13] = prePoint->GetKineticEnergy();
      Ntuples[1][14] = step->GetStepLength();
      Ntuples[1][15] = preVol;
      Ntuples[1][16] = postVol;
          
      fHistoManager->FillNtuple(Ntuples);
      Ntuples.clear();

    }
      
  }

  return;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

