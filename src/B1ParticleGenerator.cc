#include "B1ParticleGenerator.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4GenericMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
B1ParticleGenerator::B1ParticleGenerator()
  : fMessenger(nullptr), fParticle(0)
{
  DefineCommands();

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
B1ParticleGenerator::~B1ParticleGenerator()
{
  delete fMessenger;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1ParticleGenerator::DefineCommands()
{
  fMessenger = new G4GenericMessenger(this, "/B1/particle_generator/", "Name the particle for the file name");
  fMessenger->DeclareProperty("proc", fParticle, "Index of the particle in the order");


}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void B1ParticleGenerator::GetValue(int p_name)const
{
  p_name = fParticle;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
