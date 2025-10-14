#ifndef SECAL_DETECTORCONSTRUCTION_HH
#define SECAL_DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Box;

namespace secal {

class EcalSD;

class DetectorConstruction: public G4VUserDetectorConstruction
{
    public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    public:
    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    private:
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes(); 

    bool constructed_;

    G4bool fCheckOverlaps = true;

    G4Box * worldS_;
    G4LogicalVolume * worldLV_;
    G4VPhysicalVolume * worldPV_;

    G4Box * blockS_;
    G4LogicalVolume * blockLV_;
    G4VPhysicalVolume * blockPV_;

    EcalSD * SD_;
};

}
#endif
