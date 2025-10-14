#ifndef SECAL_EVENTACTION_HH
#define SECAL_EVENTACTION_HH

#include "G4UserEventAction.hh"

#include "EcalHit.hh"
#include "globals.hh"

#include <vector>

namespace secal {

class PrimaryGeneratorAction;
class CXHDF5;

class EventAction: public G4UserEventAction
{
    public:
    EventAction(PrimaryGeneratorAction *, CXHDF5 *);
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    private:
    EcalHitsCollection* GetHitsCollection(G4int hcID,
        const G4Event* event) const;

    G4int hcid_ = -1;
    PrimaryGeneratorAction * primary_;
    CXHDF5* data_out_;
    std::vector<float>  raw_data_;
};

}

#endif
