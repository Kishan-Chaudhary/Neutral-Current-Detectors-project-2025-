#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
class G4GeneralParticleSource;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	explicit PrimaryGeneratorAction();
	~PrimaryGeneratorAction();

public:
	void GeneratePrimaries(G4Event* anEvent) override;
	G4double GetGunEnergy() const { return fGun->GetParticleEnergy(); }

private:
	G4GeneralParticleSource* fGun;
	//G4ParticleGun* fParticleGun;
};
#endif
