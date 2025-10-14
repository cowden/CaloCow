
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

#include "CXHDF5.hh"

namespace secal {

ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    CXHDF5 * data = new CXHDF5();
    SetUserAction(new RunAction(data));
}

void ActionInitialization::Build() const
{
    PrimaryGeneratorAction* primary = new PrimaryGeneratorAction;
    CXHDF5 * data = new CXHDF5();
    SetUserAction(primary);
    SetUserAction(new RunAction(data));
    SetUserAction(new EventAction(primary, data));
}
}
