#include "MyStackingAction.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

MyStackingAction::MyStackingAction() {}
MyStackingAction::~MyStackingAction() {}

G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack(const G4Track* track)
{
    // Get the particle definition
    G4ParticleDefinition* particle = track->GetDefinition();

    //Kill particles except neutron and tritons
    if (particle != G4Neutron::Definition() &&
    particle != G4Triton::Definition())
    {
    return fKill;
    }

    // Keep all neutrons
    return fUrgent;
}
