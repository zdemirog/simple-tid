#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include <vector>

class G4Run;
class HistoManager;
/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class B1RunAction : public G4UserRunAction
{
public:
  B1RunAction(HistoManager* histo);
  virtual ~B1RunAction();

  // virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);

  void AddEdep (std::vector<G4double> edep); 
  void AddNIEL(std::vector<G4double> edep);

  std::vector<G4int>& GetEvNr() {return fEvNr; }
  std::vector<G4String>& GetPname() {return fPname; }
  std::vector<G4int>& GetPDG() {return fPDG; }
  std::vector<G4double>& GetEdep() {return fedep; }
  std::vector<G4double>& GetEtrans() {return fEtrans; }
  std::vector<G4double>& GetlgepsE() {return flgepsE; }

  std::vector<G4double>& GetPreX() {return fPreX; }
  std::vector<G4double>& GetPreY() {return fPreY; }
  std::vector<G4double>& GetPreZ() {return fPreZ; }
  std::vector<G4double>& GetPrePx() {return fPrePx; }
  std::vector<G4double>& GetPrePy() {return fPrePy; }
  std::vector<G4double>& GetPrePz() {return fPrePz; }
  std::vector<G4double>& GetPreKE() {return fPreKE; }
  std::vector<G4String>& GetPreVolumeName() {return fPreVolumeName; }

  std::vector<G4double>& GetPostX() {return fPostX; }
  std::vector<G4double>& GetPostY() {return fPostY; }
  std::vector<G4double>& GetPostZ() {return fPostZ; }
  std::vector<G4double>& GetPostPx() {return fPostPx; }
  std::vector<G4double>& GetPostPy() {return fPostPy; }
  std::vector<G4double>& GetPostPz() {return fPostPz; }
  std::vector<G4double>& GetPostKE() {return fPostKE; }
  std::vector<G4String>& GetPostVolumeName() {return fPostVolumeName; }

    
private:
    HistoManager* fHistoManager;

  G4Accumulable<G4double> fEdepF;
  G4Accumulable<G4double> fEdepF2;
  G4Accumulable<G4double> fEdepB;
  G4Accumulable<G4double> fEdepB2;
  G4Accumulable<G4double> fEdepM;
  G4Accumulable<G4double> fEdepM2;
    
  std::vector<G4int> fEvNr;
  std::vector<G4String> fPname;
  std::vector<G4int> fPDG;
  std::vector<G4double> fedep;
  std::vector<G4double> fEtrans;
  std::vector<G4double> flgepsE;
  std::vector<G4double> fNIEL;

  std::vector<G4double> fPreX;
  std::vector<G4double> fPreY;
  std::vector<G4double> fPreZ;
  std::vector<G4double> fPrePx;
  std::vector<G4double> fPrePy;
  std::vector<G4double> fPrePz;
  std::vector<G4double> fPreKE;
  std::vector<G4String> fPreVolumeName;

  std::vector<G4double> fPostX;
  std::vector<G4double> fPostY;
  std::vector<G4double> fPostZ;
  std::vector<G4double> fPostPx;
  std::vector<G4double> fPostPy;
  std::vector<G4double> fPostPz;
  std::vector<G4double> fPostKE;
  std::vector<G4String> fPostVolumeName;

  G4double EnergyDeposit,  EnergyDeposit2;
  G4double NonIonEnergyDeposit,  NonIonEnergyDeposit2;
    
};

#endif

