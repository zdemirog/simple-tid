#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1HistoManager.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction(HistoManager* histo):
  G4UserRunAction(),fHistoManager(histo),
  fEdepF(0.),
  fEdepF2(0.),
  fEdepB(0.),
  fEdepB2(0.),
  fEdepM(0.),
  fEdepM2(0.)
{ 
  // add new units for dose
  // 
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray); 

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdepF);
  accumulableManager->RegisterAccumulable(fEdepF2); 
  accumulableManager->RegisterAccumulable(fEdepB);
  accumulableManager->RegisterAccumulable(fEdepB2); 
  accumulableManager->RegisterAccumulable(fEdepM);
  accumulableManager->RegisterAccumulable(fEdepM2);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  // Open an output file
  G4String fileName = "B1";
  fHistoManager->Book(fileName);
  EnergyDeposit  = EnergyDeposit2  = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
     
  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  for(int i=0;i<3;i++){
    //G4cout<<G4endl<<G4endl<<"\t\t\t"<<i<<G4endl<<G4endl<<G4endl;
    G4double edep(0),edep2(0);
    if(i==0){
      edep  = fEdepF.GetValue();
      edep2 = fEdepF2.GetValue();
    }else if(i==2){
      edep  = fEdepB.GetValue();
      edep2 = fEdepB2.GetValue();
    }else if(i==1){
      edep  = fEdepM.GetValue();
      edep2 = fEdepM2.GetValue();
      EnergyDeposit = fEdepM.GetValue();
      EnergyDeposit2 = fEdepM2.GetValue();

    }

    G4double rms = edep2 - edep*edep/nofEvents;
    if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;  
    
    const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4double mass = detectorConstruction->GetScoringVolume()[i]->GetMass();
    G4double dose = edep/mass;
    G4double rmsDose = rms/mass;
    G4double length2 = 132*cm;
    G4double density2 = detectorConstruction->GetScoringVolume()[1]->GetMaterial()->GetDensity();
    //total energy loss
    EnergyDeposit /= nofEvents; EnergyDeposit2 /= nofEvents;
    G4double rmsEdep = EnergyDeposit2 - EnergyDeposit*EnergyDeposit;
    if(rmsEdep >0.)rmsEdep=std::sqrt(rmsEdep/nofEvents);
    else rmsEdep =0;

    // total energy loss
    G4double meandEdx2  = EnergyDeposit/length2;
    G4double stopPower2 = meandEdx2/density2;

      
      
    // Run conditions
    //  note: There is no primary generator action object for "master"
    //        run manager for multi-threaded mode.
    const B1PrimaryGeneratorAction* generatorAction
      = static_cast<const B1PrimaryGeneratorAction*>
      (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    G4String runCondition;
    if (generatorAction)
      {
	const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
	runCondition += particleGun->GetParticleDefinition()->GetParticleName();
	runCondition += " of ";
	G4double particleEnergy = particleGun->GetParticleEnergy();
	runCondition += G4BestUnit(particleEnergy,"Energy");
      }
    
    // Print
    //  
    // if (IsMaster()) {
    //   G4cout
    // 	<< G4endl
    // 	<< "--------------------End of Global Run-----------------------";
    // }
    // else {
    //   G4cout
    // 	<< G4endl
    // 	<< "--------------------End of Local Run------------------------";
    // }
    
    // G4cout
    //  << G4endl
    //  << " The run consists of " << nofEvents << " "<< runCondition
    //  << G4endl
    //  << " Cumulated dose per run, in scoring volume : " 
    //  << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
    //  << G4endl
    //  << "------------------------------------------------------------"
    //  << G4endl
    //  << G4endl;
    G4cout << "nofEvents: " <<nofEvents << ",runCondition: " << runCondition << ", dose/gray: " << dose/gray << " ,rmsDose/gray: " << rmsDose/gray <<G4endl;
    
    if (i==1){  G4int prec = G4cout.precision(3);

      G4cout << "\n Total energy deposit in " <<detectorConstruction->GetScoringVolume()[i]->GetName() <<" per event = "
             << G4BestUnit(EnergyDeposit,"Energy")
             << ", Material Name:" << detectorConstruction->GetScoringVolume()[i]->GetMaterial()->GetName() <<G4endl;
             
      G4cout << "\n -----> Mean dE/dx2 = " << meandEdx2/(MeV/cm) << " MeV/cm"
             << "\t(" << stopPower2/(MeV*cm2/g) << " MeV*cm2/g)"
             << G4endl;
             
      G4cout.precision(prec);}
      
  }
  fHistoManager->Save();

}
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::AddEdep(std::vector<G4double> edep)
{
  //G4cout<<fEdepF.GetValue()<<" F "<<edep[0]<<" > ";
  fEdepF  += edep[0];
  //G4cout<<fEdepF.GetValue()<<G4endl;
  fEdepF2 += edep[0]*edep[0];
  //G4cout<<fEdepB.GetValue()<<" B "<<edep[2]<<" > ";
  fEdepB  += edep[2];
  //G4cout<<fEdepB.GetValue()<<G4endl;
  fEdepB2 += edep[2]*edep[2];
  //G4cout<<fEdepM.GetValue()<<" M "<<edep[1]<<" > ";
  fEdepM  += edep[1];
  //G4cout<<fEdepM.GetValue()<<G4endl;
  fEdepM2 += edep[1]*edep[1];
  //std::cin.ignore();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

