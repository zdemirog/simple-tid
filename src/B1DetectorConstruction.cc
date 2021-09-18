#include "B1DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  G4double world_sizeXY = 2000*cm;
  G4double world_sizeZ  = 1800*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* tgtMat = nist->FindOrBuildMaterial("G4_CONCRETE");
  G4Material* fbMat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
	      0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
  
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  G4Box* solidFB =    
    new G4Box("solidFB",                    //its name
	      0.5*11249.914*mm, 0.5*5715*mm, 0.5*1*mm); //its size
    
  G4Box* solidMid =    
    new G4Box("solidMid",                    //its name
	      0.5*11249.914*mm, 0.5*5715*mm, 0.5*1400*mm); //its size

  G4LogicalVolume* logicF =                         
    new G4LogicalVolume(solidFB,            //its solid
                        fbMat,             //its material
                        "logicF");         //its name
  G4LogicalVolume* logicB =                         
    new G4LogicalVolume(solidFB,            //its solid
                        fbMat,             //its material
                        "logicB");         //its name
  G4LogicalVolume* logicMid =
    new G4LogicalVolume(solidMid,            //its solid
                        tgtMat,             //its material
                        "logicMid");         //its name
 
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,-700.5*mm),//     G4ThreeVector(0,0,-5.5*mm),
                    logicF,                //its logical volume
                    "front",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,700.5*mm),//    G4ThreeVector(0,0,5.5*mm),
                    logicB,                //its logical volume
                    "back",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),//     G4ThreeVector(0,0,0),
                    logicMid,                //its logical volume
                    "mid",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
                
  // Set Shapes as scoring volumes
  fScoringVolume.push_back(logicF);
  fScoringVolume.push_back(logicMid);
  fScoringVolume.push_back(logicB);

  //
  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
