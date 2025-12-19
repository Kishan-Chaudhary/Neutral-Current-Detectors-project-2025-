#ifndef PhysicsList_h
#define PhysicsList_h

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"

#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "GammaNuclearPhysics.hh"
#include "ElectromagneticPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "GammaNuclearPhysicsLEND.hh"

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();
};

#endif
