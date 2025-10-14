#ifndef SECAL_ECALHIT_HH
#define SECAL_ECALHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

#include "PrimaryGeneratorAction.hh"

namespace secal {

class EcalHit : public G4VHit
{
    public:
    EcalHit();
    EcalHit(const EcalHit&) = default;
    ~EcalHit() override;

    EcalHit& operator=(const EcalHit&) = default;
    G4bool operator==(const EcalHit&) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    void Draw() override{}
    void Print() override;

    void Add(G4double d3);

    G4double GetEdep() const;

    private:
    G4double edep_ = 0.;
};

using EcalHitsCollection = G4THitsCollection<EcalHit>;
extern G4ThreadLocal G4Allocator<EcalHit>* EcalHitAllocator;

inline void* EcalHit::operator new(size_t)
{
    if (  !EcalHitAllocator ) {
        EcalHitAllocator = new G4Allocator<EcalHit>;
    }
    void *hit;
    hit = (void*)EcalHitAllocator->MallocSingle();
    return hit;
}

inline void EcalHit::operator delete(void *hit)
{
    if ( !EcalHitAllocator ) {
        EcalHitAllocator = new G4Allocator<EcalHit>;
    }
    EcalHitAllocator->FreeSingle((EcalHit*) hit);
}

inline void EcalHit::Add(G4double de) {
    edep_ += de;
}

inline G4double EcalHit::GetEdep() const {
    return edep_;
}

}

#endif 
