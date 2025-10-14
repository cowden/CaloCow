#ifndef SECAL_ACTIONINITIALIZATION_HH
#define SECAL_ACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

namespace secal {

class ActionInitialization: public G4VUserActionInitialization
{
    public:
    ActionInitialization();
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;

};

}

#endif
