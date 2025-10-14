#ifndef SECAL_RUNACTION_HH
#define SECAL_RUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

namespace secal {

class CXHDF5;

class RunAction: public G4UserRunAction
{
    public:
    RunAction(CXHDF5 *);
    ~RunAction();

    virtual G4Run* GenerateRun();

    virtual void BeginOfRunAction(const G4Run *);
    virtual void EndOfRunAction(const G4Run *);

    private:
    CXHDF5 * data_out_;
};

}

#endif
