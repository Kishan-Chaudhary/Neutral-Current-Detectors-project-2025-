#include "Run.hh"

// --- Geant4 Headers ---
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"

// --- Standard Headers ---
#include <fstream>
#include <iostream>

// =========================================================================
// Constructor & Destructor
// =========================================================================
MyRunAction::MyRunAction()
    : G4UserRunAction(), 
      Triton_counts(0), 
      Neutron_entered(0), 
      fGunEnergy(0.0) // Initialize energy
{
    // Register accumulables to the manager for thread-safety.
    // This allows worker threads to maintain local counters that are merged later.
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->RegisterAccumulable(Triton_counts);
    accumulableManager->RegisterAccumulable(Neutron_entered);
}

MyRunAction::~MyRunAction()
{}

// =========================================================================
// BeginOfRunAction: Called at the start of every run
// =========================================================================
void MyRunAction::BeginOfRunAction(const G4Run*)
{
    // Reset all accumulables to zero at the start of a new run.
    G4AccumulableManager::Instance()->Reset();
}

// =========================================================================
// EndOfRunAction: Called at the end of every run
// =========================================================================
void MyRunAction::EndOfRunAction(const G4Run* run)
{
    // Merge the counters from all worker threads into the Master thread.
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    // Only the Master thread outputs the final results to the file.
    // Worker threads should not write to the file to avoid race conditions.
    if (IsMaster())
    {
        G4int runID = run->GetRunID();
        G4int totalEvents = run->GetNumberOfEventToBeProcessed();
        G4int finalTritonCount = Triton_counts.GetValue();
        G4int finalNeutronCount = Neutron_entered.GetValue();

        // --- Console Output (Debug) ---
        G4cout << " >> Run " << runID << " Completed." << G4endl;
        G4cout << "    Gun Energy: " << fGunEnergy / MeV << " MeV" << G4endl;
        G4cout << "    Events Processed: " << totalEvents << G4endl;
        G4cout << "    Neutrons Entered (Scorer->Tube): " << finalNeutronCount << G4endl;
        G4cout << "    Tritons Detected: " << finalTritonCount << G4endl;

        // --- File Output (CSV) ---
        // Writing to "bare_response.csv". Use std::ios::app to append new runs.
        std::ofstream file("Response.csv", std::ios::app);
        
        if (file.is_open()) {
            // CSV Format: RunID, TotalEvents, GunEnergy, NeutronEntered, TritonCounts
            file << runID << "," 
                 << totalEvents << ","
                 << fGunEnergy << ","  // Ensure units are consistent (default is internal Geant4 MeV)
                 << finalNeutronCount << ","
                 << finalTritonCount << "\n";
            
            file.close();
        } else {
            G4cerr << "Error: Could not open bare_response.csv for writing!" << G4endl;
        }
    }
}

// =========================================================================
// Helper Methods (Thread-Safe Counters)
// =========================================================================

void MyRunAction::AddTriton()
{
    // This operator++ is overloaded by G4Accumulable to be thread-safe
    Triton_counts++; 
}

void MyRunAction::AddNeutronEntered()
{
    Neutron_entered++;
}

void MyRunAction::SetGunEnergy(G4double E)
{
    fGunEnergy = E;
}

void MyRunAction::SetFileName(G4String filename)
{
    fileName = filename;
}

G4String MyRunAction::GetFileName()
{
    return fileName;
}