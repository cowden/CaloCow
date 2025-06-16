#ifndef SECAL_ECALSD_HH
#define SECAL_ECALSD_HH

#include "G4VSensitiveDetector.hh"
#include "EcalHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

namespace secal {

class EcalSD: public G4VSensitiveDetector
{
    public:
    EcalSD(const G4String& name,
        const G4String& hitsCollectionName
        );
    ~EcalSD() override;

    // methods from base class
    void Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

    private:
    EcalHitsCollection* hitsCollection_ = nullptr;

    G4int gridSize_; // side of square grid (gridSize_ X gridSize_)
    G4double ha_; // half length of cell 

    unsigned getCellIndex_(const G4ThreeVector &);
};

}

#endif
