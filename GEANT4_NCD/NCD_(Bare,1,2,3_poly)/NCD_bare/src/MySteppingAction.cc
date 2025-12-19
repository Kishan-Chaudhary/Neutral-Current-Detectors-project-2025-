#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "MySteppingAction.hh"
#include "Run.hh"
#include "G4Step.hh"
#include "G4Neutron.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "MyEventAction.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

MySteppingAction::MySteppingAction(MyRunAction* run) : runAction(run) {}

void MySteppingAction::UserSteppingAction(const G4Step* step) {
    auto track = step->GetTrack();
    auto parentID = track->GetParentID();


	// Check if the particle is a neutron and it's a primary particle
    if (track->GetDefinition() == G4Neutron::NeutronDefinition() && parentID == 0) {
        auto preVol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        auto postVol = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

        if (postVol &&(preVol->GetName() == "NeutronScorer1" || preVol->GetName() == "NeutronScorer2" || preVol->GetName() == "NeutronScorer3"))
         {
            if (postVol->GetName() == "NickelTube1" || postVol->GetName() == "NickelTube2" || postVol->GetName() == "NickelTube3") {
                //Non-const cast
                auto eventAction = static_cast<MyEventAction*>(
                    G4EventManager::GetEventManager()->GetUserEventAction());

                // Count only the first entrance for this track
                if (!eventAction->IsNeutronCounted()) {
                    auto runAction = const_cast<MyRunAction*>(
                        static_cast<const MyRunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));


                    runAction->AddNeutronEntered();
                    eventAction->MarkNeutronCounted();
                }
            }
        }
    }
}

