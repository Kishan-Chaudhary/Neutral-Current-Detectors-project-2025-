#ifndef Action_h
#define Action_h

#include "G4VUserActionInitialization.hh"
#include "Run.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void Build() const;
    virtual void BuildForMaster() const;
};

#endif
