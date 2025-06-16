
#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace secal {

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    G4int nParticles = 1;
    particleGun_ = new G4ParticleGun(nParticles);

    auto particleDefinition
      = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    particleGun_->SetParticleDefinition(particleDefinition);
    particleGun_->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    particleGun_->SetParticleEnergy(50.*MeV);

    Ebeam = -1;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete particleGun_;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    G4double wha = 0.;
    auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

    G4Box * worldBox = nullptr;
    if ( worldLV )
        worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());

    if ( worldBox )
        wha = worldBox->GetZHalfLength();
    else {
        G4ExceptionDescription msg;
        msg << "World volume of box shape not found." << G4endl;
        msg << "The gun will be placed in the center.";
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
            "MyCode0002", JustWarning, msg);
    }

    particleGun_->SetParticlePosition(G4ThreeVector((ran1.flat()*4.-2.)*cm, (ran1.flat()*4.-2.)*cm, -wha));

    if ( Ebeam < 0. ) Ebeam = particleGun_->GetParticleEnergy();
    auto t = ran1.flat();  // the first draw is needed for some reason
    G4double event_energy = (500.+ran1.flat()*(Ebeam-500.))*MeV;
    particleGun_->SetParticleEnergy(event_energy);

    particleGun_->GeneratePrimaryVertex(event);

}

}
