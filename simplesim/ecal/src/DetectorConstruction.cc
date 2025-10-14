
#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "EcalSD.hh"

namespace secal {

DetectorConstruction::DetectorConstruction()
:constructed_(false)
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume * DetectorConstruction::Construct()
{
    DefineMaterials();
    return DefineVolumes();
}

void DetectorConstruction::DefineMaterials()
{
    auto nistManager = G4NistManager::Instance();
    nistManager->FindOrBuildMaterial("G4_PbWO4");

    nistManager->FindOrBuildMaterial("G4_Galactic");

    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
    constructed_ = true;

    auto defaultMaterial = G4Material::GetMaterial("G4_Galactic");
    auto pwoMaterial = G4Material::GetMaterial("G4_PbWO4");

    // world volume
    worldS_ = new G4Box("World",
        22*5/2.*cm + 3*cm,
        22*5/2.*cm + 3*cm,
        23/2.*cm + 3*cm);
    worldLV_ = new G4LogicalVolume(
        worldS_,
        defaultMaterial,
        "World");
    worldPV_ = new G4PVPlacement(
        0,
        G4ThreeVector(),
        worldLV_,
        "World",
        0, 
        false,
        0,
        fCheckOverlaps);

    // ecal volume
    blockS_ = new G4Box("Block",
        22*5/2.*cm,
        22*5/2.*cm,
        23/2*cm);
    blockLV_ = new G4LogicalVolume(
        blockS_,
        pwoMaterial,
        "Block");
    blockPV_ = new G4PVPlacement(
        0,
        G4ThreeVector(),
        blockLV_,
        "Block",
        worldLV_,
        false,
        0,
        fCheckOverlaps);

    return worldPV_;
}

void DetectorConstruction::ConstructSDandField()
{
    SD_ = new EcalSD("EcalSD", "EcalHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(SD_);
    SetSensitiveDetector("Block", SD_);
}

}
