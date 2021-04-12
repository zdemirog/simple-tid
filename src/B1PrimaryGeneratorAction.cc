#include "B1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4UnitsTable.hh"
#include "G4GenericMessenger.hh"
#include "B1HistoManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction(HistoManager* histo)
  : G4VUserPrimaryGeneratorAction(),fHistoManager(histo),
    fParticleGun(0),fEnergy(10*GeV),fX0min(0.0),fX0max(0.0),fY0min(0.0),fY0max(0.0),fZ0(0.0),fdThmin(0.0),fdThmax(0.0),fdPhmin(0.0),fdPhmax(0.0)
{
  totenergy=0;
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  const int proc = 3;
  G4String particleName[4]={"gamma","e-","e+","neutron"};
  const G4String hNm[4]={"g","em","ep","n"};
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName[proc]);
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(6.*MeV);
  //fin=TFile::Open("./beamGeoV2_radAnaV5.root","READ");
  fin=TFile::Open("../../../../fromGeantExamples/B1/tgtShld_conf4_tgtShldAnaV0.root","READ");
  if(fin){
    fin->ls();
  }else{
    G4cout<<"can't open file"<<G4endl;
    exit(0);
  }

  //hE=(TH1F*)fin->Get(Form("det28/d28_energy_R7_%s_allPZ",hNm[proc].data()));
  hSBS=(TH1F*)fin->Get(Form("det5510/d5510__energy_allPZ_%s",hNm[proc].data()));
  // TH1F *hDS=(TH1F*)fin->Get(Form("det5547/d5547__energy_allPZ_%s",hNm[proc].data()));
  // hE=(TH1F*)hSBS->Clone(Form("%s_USDS",hSBS->GetName()));
  // hE->Add(hDS);
  // hE->SetTitle(Form("%s + DS",hE->GetTitle()));
  //assert(hE);
  G4cout<<"Using "<<hSBS->GetTitle()<<G4endl;
  if(!hSBS){
    G4cout<<"can't find histo"<<G4endl;
    exit(0);
  }
    
  fMessenger
    = new G4GenericMessenger(this,
			     "/B1/generator/",
			     "Primary generator control");

  // Energy
  fMessenger->DeclarePropertyWithUnit("energy", "GeV", fEnergy,
				      "Energy of primaries").SetParameterName("energy", true).SetDefaultValue("10.");

  // beam spread
  fMessenger->DeclarePropertyWithUnit("beam_dphmin", "deg", fdPhmin,"Gaussian beam spread phi_min");
  fMessenger->DeclarePropertyWithUnit("beam_dphmax", "deg", fdPhmax,"Gaussian beam spread phi_max");
  fMessenger->DeclarePropertyWithUnit("beam_dthmin","deg",fdThmin,"Gaussian beam spread theta_min");
  fMessenger->DeclarePropertyWithUnit("beam_dthmax","deg",fdThmax,"Gaussian beam spread theta_max");

  // beam positions
  fMessenger->DeclarePropertyWithUnit("beam_x0min","cm",fX0min,"beam initial min-position in x");
  fMessenger->DeclarePropertyWithUnit("beam_x0max","cm",fX0max,"beam initial max-position in x");

  fMessenger->DeclarePropertyWithUnit("beam_y0min","cm",fY0min,"beam initial min-position in y");
  fMessenger->DeclarePropertyWithUnit("beam_y0max","cm",fY0max,"beam initial max-position in y");

  fMessenger->DeclarePropertyWithUnit("beam_z0","cm", fZ0,"Beam position along the beam line");
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  fin->Close();
  delete fin;
  delete fParticleGun;
  delete fMessenger;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    
  G4double x0 =  G4RandFlat::shoot(fX0min,fX0max);
  G4double y0 =  G4RandFlat::shoot(fY0min,fY0max);
  G4double z0 = -0.5*(fZ0);
    
  G4double phi = G4RandFlat::shoot(fdPhmin,fdPhmax);
  G4double theta = G4RandFlat::shoot(fdThmin,fdThmax);

  G4double randomX = std::sin(theta)*std::cos(phi);
  G4double randomY = std::sin(theta)*std::sin(phi);
  G4double randomZ = std::cos(theta);

  G4double energy = hSBS->GetRandom()*MeV;
  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(randomX,randomY,randomZ));
  fParticleGun->GeneratePrimaryVertex(anEvent);
  
  fHistoManager->FillHisto(0, energy);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

