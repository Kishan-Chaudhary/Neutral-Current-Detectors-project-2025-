#ifndef Run_h
#define Run_h

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4AnalysisMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4Accumulable.hh"
#include "G4AccumulableManager.hh"
#include <cmath>

class MyRunAction : public G4UserRunAction
{
public:
  MyRunAction();
  ~MyRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  G4AnalysisManager* man;
private:
    G4Accumulable<G4int> Triton_counts = 0.0;
	G4Accumulable<G4int> Neutron_entered = 0.0;
	G4double fGunEnergy = 0;
	G4String fileName = "output";

public:
    void AddTriton();
    G4double GetTritonCounts();
	void ResetTritonCounts();
	void ResetNeutronEntered();
    void AddNeutronEntered();
	G4double GetNeutronEntered();
	void SetGunEnergy(G4double E);
	void SetFileName(G4String filename);
	G4String GetFileName();
};


#endif
