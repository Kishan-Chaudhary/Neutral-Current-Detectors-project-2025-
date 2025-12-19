#ifndef MYEVENTACTION_HH
#define MYEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <set>

class MyRunAction;   // forward declaration

class MyEventAction : public G4UserEventAction {
public:
    MyEventAction(MyRunAction* runAction);
    virtual ~MyEventAction() = default;

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void ResetNeutronCounted();
    bool IsNeutronCounted();
    void MarkNeutronCounted();

private:
    MyRunAction* fRunAction;
    G4bool fCountedNeutrons; // track IDs per event
};

#endif