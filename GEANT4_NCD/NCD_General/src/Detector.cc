#include "Detector.hh"

// --- Geant4 Core Headers ---
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4TouchableHistory.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

// --- Particle Definitions ---
#include "G4ParticleDefinition.hh"
#include "G4Alpha.hh" 
#include "G4Neutron.hh"
#include "G4Triton.hh" 

// --- User Headers ---
#include "Run.hh" // Required to access MyRunAction methods

// =========================================================================
// Constructor & Destructor
// =========================================================================
SensitiveDetector::SensitiveDetector(G4String name) 
: G4VSensitiveDetector(name), 
  fCurrentEventID(-1), 
  fCurrentTrackID(-1), 
  TritonCount(0), 
  PreviousEdepEventID(0), 
  CurrentEdepEventID(1), 
  TotalEdepEvent(0)
{}

SensitiveDetector::~SensitiveDetector()
{}

// =========================================================================
// ProcessHits: Called for every single step a particle takes in this volume
// =========================================================================
G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
    // 1. Basic Step Information
    G4Track* track = aStep->GetTrack();
    G4String particleName = track->GetDefinition()->GetParticleName();
    
    // Update internal state trackers (optional, mostly for debugging)
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    fCurrentEventID = evt;
    fCurrentTrackID = track->GetTrackID();

    // 2. Access the Run Action
    // We need to cast to MyRunAction to call custom methods like AddTriton().
    // The const_cast is necessary because GetUserRunAction() returns a const pointer.
    auto runAction = const_cast<MyRunAction*>(
        static_cast<const MyRunAction*>(
            G4RunManager::GetRunManager()->GetUserRunAction()
        )
    );
    
    // Safety check: Ensure runAction exists
    if (!runAction) return false; 

    // 3. Triton Detection Logic
    // We filter for:
    //  a) The particle is a "triton"
    //  b) IsFirstStepInVolume() -> This ensures we count the triton only ONCE 
    //     when it is created or enters the detector, rather than counting every 
    //     small step it takes while moving through the gas.
    
    if (particleName == "triton" && aStep->IsFirstStepInVolume()) {
        
        // Register the count in the thread-local RunAction
        runAction->AddTriton();

        /* // --- Debugging Info (Uncomment if needed) ---
        G4double ekin = aStep->GetPreStepPoint()->GetKineticEnergy();
        G4ThreeVector pos = track->GetPosition();
        G4cout << " >> SD: Triton Detected!" 
               << " Event: " << evt 
               << " Energy: " << ekin / keV << " keV" 
               << G4endl; 
        */
    }

    return true;
}