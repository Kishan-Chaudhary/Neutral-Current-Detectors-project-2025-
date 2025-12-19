#include "G4UserSteppingAction.hh"
#include "globals.hh"

class MyRunAction; // forward declare

class MySteppingAction : public G4UserSteppingAction {
public:
    MySteppingAction(MyRunAction* runAction);
    virtual ~MySteppingAction() {}
    virtual void UserSteppingAction(const G4Step* step);

private:
    MyRunAction* runAction;
};

