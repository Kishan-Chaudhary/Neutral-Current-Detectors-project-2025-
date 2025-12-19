#include "EventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

EventAction::EventAction() : G4UserEventAction(), fEventEdep(0.0) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
    //fEventEdep = 0.0;  // reset at end of event
}

void EventAction::EndOfEventAction(const G4Event*) {
    /*G4AnalysisManager* man = G4AnalysisManager::Instance();

    man->FillNtupleDColumn(12, 0, fEventEdep);
    man->AddNtupleRow(12);*/
}

void EventAction::AddEdep(G4double edep) {
    /*fEventEdep += edep;*/
 
}

G4double EventAction::GetEdep() const {
    return fEventEdep;
}