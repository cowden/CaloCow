
#include "EventAction.hh"
#include "EcalSD.hh"
#include "EcalHit.hh"
#include "CXHDF5.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include <sstream>

namespace secal {

EventAction::EventAction(PrimaryGeneratorAction * prim, CXHDF5* data)
:primary_(prim), data_out_(data)
{}

EventAction::~EventAction()
{}

EcalHitsCollection*
EventAction::GetHitsCollection(G4int hcID, const G4Event *event) const
{
    auto hc = static_cast<EcalHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

    if ( !hc ){
        G4ExceptionDescription msg;
        msg << "Cannot access hits collection ID " << hcID;
        G4Exception("EventAction::GetHitsCollection()",
          "MyCode0003", FatalException, msg);
    }

    return hc;
}

void EventAction::BeginOfEventAction(const G4Event* event )
{
    raw_data_.resize(data_out_->size());
    const unsigned size = raw_data_.size();
    for ( unsigned i=0; i != size; i++ )
        raw_data_[i] = 0.;
}

void EventAction::EndOfEventAction(const G4Event* event )
{

    const unsigned eid  = event->GetEventID();

    if ( hcid_ == -1 )
        hcid_ = G4SDManager::GetSDMpointer()->GetCollectionID("EcalHitsCollection");

    std::stringstream msg;
    // get primary information
    auto prim_pos = primary_->GetParticleGun()->GetParticlePosition();
    auto prim_e = primary_->GetParticleGun()->GetParticleEnergy();
    msg << prim_e << " (" << prim_pos.x() << ", " << prim_pos.y() <<") ";
    raw_data_[0] = prim_e;
    raw_data_[1] = prim_pos.x();
    raw_data_[2] = prim_pos.y();

    // read hits
    auto hc = GetHitsCollection(hcid_, event);
    std::size_t nhits = hc->GetSize();
    for ( unsigned i=0; i != nhits; i++ ) {
        msg << i << ": " << (*hc)[i]->GetEdep() << " ";
        raw_data_[i+3] = (*hc)[i]->GetEdep();
    }
    G4cout << msg.str() << G4endl;

    data_out_->write_event(eid, &raw_data_[0]);
}

}
