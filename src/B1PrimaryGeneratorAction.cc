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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  const int proc = 2;
  G4String particleName[4]={"gamma","e-","e+","neutron"};
  const G4String hNm[4]={"g","em","ep","n"};
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName[proc]);
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(6.*MeV);
  fin=TFile::Open("./beamGeoV2_radAnaV5.root","READ");
  if(fin){
    fin->ls();
  }else{
    G4cout<<"can't open file"<<G4endl;
    exit(0);
  }

  hE=(TH1F*)fin->Get(Form("det28/d28_energy_R7_%s_allPZ",hNm[proc].data()));
  assert(hE);
  G4cout<<"Using "<<hE->GetTitle()<<G4endl;
  if(!hE){
    G4cout<<"can't find histo"<<G4endl;
    exit(0);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  fin->Close();
  delete fin;
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4double x0 = 2*cm*(G4UniformRand()-0.5);
  G4double y0 = 2*cm*(G4UniformRand()-0.5);
  G4double z0 = -7*cm;
  G4double energy = hE->GetRandom()*MeV;
  //G4double energy = 6*MeV;
  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

