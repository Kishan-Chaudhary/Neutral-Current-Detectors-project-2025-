#include "MySteppingAction.hh"

// --- Geant4 Headers ---
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Neutron.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4VPhysicalVolume.hh"

// --- User Headers ---
#include "Run.hh"
#include "MyEventAction.hh"

// Constructor
MySteppingAction::MySteppingAction(MyRunAction* run) 
    : runAction(run) 
{}

// =========================================================================
// UserSteppingAction: Called at every step of the simulation
// =========================================================================
void MySteppingAction::UserSteppingAction(const G4Step* step) 
{
    G4Track* track = step->GetTrack();

    // 1. Filter: We only care about Primary Neutrons (ParentID == 0)
    //    If you want to count secondary neutrons (from interactions), remove "parentID == 0".
    if (track->GetDefinition() == G4Neutron::NeutronDefinition() && track->GetParentID() == 0) 
    {
        // 2. Get Volume Information
        // We need the physical volume of the "PreStep" (where it came from)
        // and "PostStep" (where it is going).
        G4VPhysicalVolume* preVol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        G4VPhysicalVolume* postVol = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

        // Safety check: ensure the particle didn't leave the world (postVol would be null)
        if (!preVol || !postVol) return;

        G4String preName = preVol->GetName();
        G4String postName = postVol->GetName();

        // 3. Boundary Crossing Check
        // Logic: Did the neutron move from a "NeutronScorer" volume into a "NickelTube"?
        // We check all 3 detector pairs.
        
        bool leftScorer = (preName == "NeutronScorer1" || 
                           preName == "NeutronScorer2" || 
                           preName == "NeutronScorer3");

        bool enteredTube = (postName == "NickelTube1" || 
                            postName == "NickelTube2" || 
                            postName == "NickelTube3");

        if (leftScorer && enteredTube) 
        {
            // 4. Double Counting Protection
            // A neutron might scatter at the boundary and cross it multiple times.
            // We ask MyEventAction if we have already counted this neutron for this specific event.
            
            auto eventAction = static_cast<MyEventAction*>(
                G4EventManager::GetEventManager()->GetUserEventAction()
            );

            // If we haven't counted a neutron for this event yet...
            if (eventAction && !eventAction->IsNeutronCounted()) 
            {
                // Access RunAction (removing constness to modify the counter)
                auto currentRunAction = const_cast<MyRunAction*>(
                    static_cast<const MyRunAction*>(
                        G4RunManager::GetRunManager()->GetUserRunAction()
                    )
                );

                if (currentRunAction) {
                    currentRunAction->AddNeutronEntered(); // Increment global counter
                    eventAction->MarkNeutronCounted();     // Set flag so we don't count this again
                }
            }
        }
    }
}