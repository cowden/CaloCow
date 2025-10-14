#include "EcalSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

namespace secal {

EcalSD::EcalSD(const G4String& name,
    const G4String& hitsCollectionName)
: G4VSensitiveDetector(name),
gridSize_(5), ha_(11*cm)
{
    collectionName.insert(hitsCollectionName); 
}

EcalSD::~EcalSD(){ }

void EcalSD::Initialize(G4HCofThisEvent * hce)
{
    hitsCollection_ = new EcalHitsCollection(SensitiveDetectorName, collectionName[0]);

    auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, hitsCollection_ );

    for ( G4int i=0; i<26; i++ )
        hitsCollection_->insert(new EcalHit());
}

G4bool EcalSD::ProcessHits(G4Step* step, G4TouchableHistory *)
{
    auto edep = step->GetTotalEnergyDeposit();

    G4double stepLength = 0.;
    if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. )
        stepLength = step->GetStepLength();

    if ( edep == 0. && stepLength == 0. ) return false;

    // get x-y position
    const G4StepPoint * p1 = step->GetPreStepPoint();
    const G4StepPoint * p2 = step->GetPostStepPoint();
    
    const G4ThreeVector& x1 = p1->GetPosition();
    const G4ThreeVector& x2 = p2->GetPosition();

    // calculate cell index
    unsigned index1 = getCellIndex_(x1);
    if ( index1 == UINT_MAX )
        return false;

    // add edep
    auto hit = (*hitsCollection_)[index1];
    if ( !hit ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hit " << index1;
        G4Exception("EcalSD::ProcessHits()",
            "MyCode0004", FatalException, msg);
    }
    hit->Add(edep);

    return true;
}

void EcalSD::EndOfEvent(G4HCofThisEvent*)
{
    if ( verboseLevel > 1 ) {
        auto nofHits = hitsCollection_->entries();
        G4cout << G4endl
            << "------>Hits Collection: in this event they are " << nofHits
            << " hits in the block: " << G4endl;
        for ( std::size_t i=0; i<nofHits; ++i ) (*hitsCollection_)[i]->Print();
    }
}

unsigned EcalSD::getCellIndex_(const G4ThreeVector & x)
{
    unsigned i=0U;
    unsigned j=0U;

    if ( x.x() > gridSize_*ha_ || x.x() < -gridSize_*ha_ )
        return UINT_MAX;

    if ( x.y() > gridSize_*ha_ || x.y() < -gridSize_*ha_ )
        return UINT_MAX;

    while ( x.x() < gridSize_*ha_ - 2.*i*ha_ )
        i++;

    while ( x.y() < gridSize_*ha_ - 2.*j*ha_ )
        j++;

    i--;
    j--;

    return j*gridSize_+i;
}

}
