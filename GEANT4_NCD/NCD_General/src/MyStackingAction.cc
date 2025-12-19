#include "MyStackingAction.hh"

// --- Geant4 Headers ---
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh" // Includes definitions for Neutron, Triton, etc.
#include "G4Neutron.hh"
#include "G4Triton.hh"

// Constructor & Destructor
MyStackingAction::MyStackingAction() {}
MyStackingAction::~MyStackingAction() {}

// =========================================================================
// ClassifyNewTrack
// Called for every new particle generated in the simulation.
// Returns:
//    fUrgent: Track immediately.
//    fWaiting: Track later (after urgent stack is empty).
//    fKill: Delete track immediately (do not simulate).
// =========================================================================
G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack(const G4Track* track)
{
    // Get the particle definition
    const G4ParticleDefinition* particle = track->GetDefinition();

    // --- Optimization Filter ---
    // We only care about:
    // 1. Neutrons (to simulate transport and capture).
    // 2. Tritons (the signal we are counting in the detector).
    //
    // WARNING: This kills the Proton from the n+He3->p+T reaction. 
    // If you ever need to calculate Total Energy Deposition (Q-value), 
    // you must NOT kill the proton, as it carries ~573 keV of energy.
    // If you are only counting captures (tritons), this is fine and faster.

    if (particle != G4Neutron::Definition() && 
        particle != G4Triton::Definition())
    {
        return fKill; // Kill gammas, electrons, protons, alphas, etc.
    }

    // Default: Simulate the particle immediately
    return fUrgent;
}