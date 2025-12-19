#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4NuclideTable.hh"

#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsXS.hh"

#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4HadronPhysicsShielding.hh"

#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonQMDPhysics.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4RadioactiveDecay.hh"
#include "G4RadioactiveDecayPhysics.hh"

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

PhysicsList::PhysicsList()
{
 //Some physics lists require a verbose level
  //We don't require the associated verbose information
  //So we set the verbose level to 0
  G4int verb = 1;
  SetVerboseLevel(verb);
  //Some also require strings but we'll set that as we go

  // Hadron Elastic scattering
  RegisterPhysics( new HadronElasticPhysicsHP(verb));
  //RegisterPhysics(new G4HadronElasticPhysicsXS(verb));
  
  // Hadron Inelastic Physics
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP());
  //RegisterPhysics(new G4HadronInelasticQBBC(verb));

  // Ion Physics
  //RegisterPhysics( new G4IonPhysics());
  RegisterPhysics(new G4IonPhysicsXS(verb));

  // stopping Particles
  RegisterPhysics( new G4StoppingPhysics());

  // Gamma-Nuclear Physics
  //RegisterPhysics( new GammaNuclearPhysics("Gamma"));

  // EM physics
  RegisterPhysics(new ElectromagneticPhysics());

  // //Photodisintegration for Dueterium
  // RegisterPhysics(new GammaNuclearPhysicsLEND("gamma"));

  // Decay
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  //G4RadioactiveDecayPhysics* decayProcess = new G4RadioactiveDecayPhysics();
  //decayProcess->SetThreshold(1 * MeV); // Set threshold for alpha detection
  //RegisterProcess(decayProcess);

  //EM extra list (Kishan)
 // RegisterPhysics(new G4EmExtraPhysics());

}

PhysicsList::~PhysicsList()
{ }

//Need to revise SetCuts
