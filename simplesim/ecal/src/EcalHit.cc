
#include "EcalHit.hh"
#include "G4UnitsTable.hh"

#include <iomanip>

namespace secal {

G4ThreadLocal G4Allocator<EcalHit>* EcalHitAllocator = nullptr;

EcalHit::EcalHit()
{}

EcalHit::~EcalHit()
{}

G4bool EcalHit::operator==(const EcalHit& right) const
{
    return ( this == &right ) ? true : false;
}

void EcalHit::Print()
{
    G4cout
        << "Edep: "
        << std::setw(7) << G4BestUnit(edep_, "Energy")
        << G4endl;

}

}
