#ifndef SECAL_PRIMAYGENERATORACTION_HH
#define SECAL_PRIMAYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "CLHEP/Random/Random.h"
#include "G4ParticleGun.hh"

class G4Event;

namespace secal {

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event* event) override;

    void SetRandomFlag(G4bool value);

    G4ParticleGun* GetParticleGun() const { return particleGun_; }

    private:
    G4ParticleGun* particleGun_ = nullptr;

    CLHEP::HepRandom ran1;
    G4double Ebeam;
};

}

#endif 
