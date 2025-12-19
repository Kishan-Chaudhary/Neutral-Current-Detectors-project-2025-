#include "G4ClassificationOfNewTrack.hh"
#include "globals.hh"
#include "G4UserStackingAction.hh"
#include "G4ThreeVector.hh"


#ifndef MyStackingAction_H
#define MyStackingAction_H 1



class MyStackingAction : public G4UserStackingAction
{
public:
    MyStackingAction();
    ~MyStackingAction() override;

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track) override;
};

#endif