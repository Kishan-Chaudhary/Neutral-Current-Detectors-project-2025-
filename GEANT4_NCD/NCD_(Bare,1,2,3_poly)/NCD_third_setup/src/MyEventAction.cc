#include "MyEventAction.hh"
#include "Run.hh"
#include "G4Event.hh"

MyEventAction::MyEventAction(MyRunAction* runAction)
    : fRunAction(runAction) {
}

void MyEventAction::BeginOfEventAction(const G4Event*) {
   
}

void MyEventAction::EndOfEventAction(const G4Event*) {
    ResetNeutronCounted();
}


void MyEventAction::ResetNeutronCounted() {
    fCountedNeutrons = false;
}

bool MyEventAction::IsNeutronCounted(){
    return fCountedNeutrons;
}

void MyEventAction::MarkNeutronCounted() {
    fCountedNeutrons = true;
}
