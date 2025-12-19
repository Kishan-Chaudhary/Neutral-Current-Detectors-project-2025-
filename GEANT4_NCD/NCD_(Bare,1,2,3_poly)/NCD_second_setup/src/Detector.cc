#include "Detector.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "Run.hh"
#include "G4Event.hh"
#include "G4Alpha.hh" 
#include "G4TouchableHistory.hh"
#include "G4CsvAnalysisManager.hh"
#include "G4Neutron.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name), fCurrentEventID(-1), fCurrentTrackID(-1), TritonCount(0), PreviousEdepEventID(0),CurrentEdepEventID(1),TotalEdepEvent(0)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = aStep->GetTrack()->GetTrackID();
    G4int parentID = aStep->GetTrack()->GetParentID();
    G4double edep = aStep->GetTotalEnergyDeposit();
    CurrentEdepEventID = evt;
     //Checking if the current step is part of a new event or a new track
    //if (evt != fCurrentEventID || trackID != fCurrentTrackID) {
        fCurrentEventID = evt;
        fCurrentTrackID = trackID;

        G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();

        G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

        // Access the kinetic energy at the pre-step
        G4double ekin1 = preStepPoint->GetKineticEnergy();
        G4double CutOff = 10 * keV;

        // Print or store particle information for debugging
        G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
       
            G4ThreeVector position = aStep->GetTrack()->GetPosition();
           /* G4cout << G4endl << "Event in Sensitive Detector(BDS): " << evt << ", TrackID: " << trackID << ", ParentID: "<<parentID << ", Particle: " << particleName
                << ", Kinetic Energy: " << ekin1 << " MeV"
                << ", Position: " << position / cm << " cm" << ", EDEP: "<<edep <<" MeV "<< G4endl;*/
       
        


		//Make simulation faster by killing other particles steps
        G4AnalysisManager* man = G4AnalysisManager::Instance();
       
        auto runAction = const_cast<MyRunAction*>(
            static_cast<const MyRunAction*>(
                G4RunManager::GetRunManager()->GetUserRunAction()
                ));
            if (aStep->IsFirstStepInVolume() &&
                aStep->GetTrack()->GetDefinition()->GetParticleName() == "triton") {
                /*G4cout << G4endl << "Event in Sensitive Detector(BDS): " << evt << ", TrackID: " << trackID << ", ParentID: " << parentID << ", Particle: " << particleName
                    << ", Kinetic Energy: " << ekin1 << " MeV"
                    << ", Position: " << position / cm << " cm" << ", EDEP: " << edep << " MeV " << G4endl;*/
                runAction->AddTriton();
            }
             

    return true;
}
