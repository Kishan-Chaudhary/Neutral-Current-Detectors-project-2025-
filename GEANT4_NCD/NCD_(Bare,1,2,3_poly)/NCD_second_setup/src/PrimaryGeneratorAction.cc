
#include "globals.hh"

#include "PrimaryGeneratorAction.hh"
#include "Randomize.hh"  
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4RunManager.hh"
#include "Run.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	// Use the GPS to generate primary particles,
	// Particle type, energy position, direction are specified in 
	// macro files
	fGun = new G4GeneralParticleSource();
    /*   //fParticleGun = new G4ParticleGun(1);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "neutron";
    G4ParticleDefinition* particle = particleTable->FindParticle("neutron");

    // Set the particle definition
    fGun->SetParticleDefinition(particle);

    

    //Defining particle position to be random and isometric along outside of world sphere
    G4double dcosTheta = 2 * G4UniformRand() - 1.;
    G4double dsinTheta = std::sqrt(1. - dcosTheta * dcosTheta);
    G4double dphi = twopi * G4UniformRand();
    G4double dx = 1.189 * cm * dsinTheta * std::cos(dphi),
        dy = 1.189 * cm * dsinTheta * std::sin(dphi),
        dz = 1.189 * cm * dcosTheta;
    G4ThreeVector position(dx, dy, dz);

    G4double pi = 3.14159265359;
    //Defining particle momentum direction to be shot at a random angle inwards within the world sphere
    G4double vTheta = std::acos(dcosTheta) + pi / 2 + G4UniformRand() * pi;
    G4double vcosTheta = std::cos(vTheta);
    G4double vsinTheta = std::sin(vTheta);
    G4double vphi = dphi - pi / 2 + G4UniformRand() * pi;
    G4double vx = vsinTheta * std::cos(vphi),
        vy = vsinTheta * std::sin(vphi),
        vz = vcosTheta;
    G4ThreeVector momentum(vx, vy, vz);



    //Defining particle momentum to be random and isometric (Point Source)
    //G4double cosTheta = 2*G4UniformRand() - 1.;
    //G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    //G4double phi = twopi*G4UniformRand(); 
    //G4double vx = sinTheta*std::cos(phi),
    //         vy = sinTheta*std::sin(phi),
    //         vz = cosTheta;
    //G4ThreeVector momentum(vx,vy,vz);

    // Apply the random position to the particle source
    fGun->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
    fGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(position);
    // Set the energy distribution
   // You can add more energy bins or use a different spectrum type
    fGun->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Arb");
    fGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(momentum);
    //fParticleGun->SetParticleMomentum(G4UniformRand()*10*MeV);
    */

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fGun;
	//delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	//fParticleGun->GeneratePrimaryVertex(anEvent);
    
    fGun->GeneratePrimaryVertex(anEvent);
    // auto runAction = const_cast<MyRunAction*>(
    //     static_cast<const MyRunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
    //     runAction->SetGunEnergy(0.01);
    //     G4cout<<"GPS energy sent to run action:"<<fGun->GetCurrentSource()->GetEneDist()->GetMonoEnergy();


}


