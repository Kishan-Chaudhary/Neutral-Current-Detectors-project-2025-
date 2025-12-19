#include <iostream>
#include <fstream>  // Required for file output
#include <ctime>    // Required for time()

// --- Geant4 Core ---
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

// --- User Defined Classes ---
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "Action.hh"

// --- Physics Modules (Optional if included in PhysicsList.hh, but kept for reference) ---
#include "HadronElasticPhysicsHP.hh"
#include "GammaNuclearPhysics.hh"
#include "ElectromagneticPhysics.hh"

int main(int argc, char** argv) {

    // =========================================================================
    // 1. RANDOM NUMBER ENGINE SETUP
    // =========================================================================
    
    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Set random seed (dynamic based on system time)
    // Use a fixed seed (e.g., 1234) for reproducible debugging results.
    G4long seed = time(nullptr);
    CLHEP::HepRandom::setTheSeed(seed);
    
    // Log the random seed to console and file for reproducibility
    G4cout << "Random seed = " << seed << G4endl;
    std::ofstream seedOut("random_seed.log", std::ios::app);
    if (seedOut.is_open()) {
        seedOut << "Random seed: " << seed << std::endl;
        seedOut.close();
    }

    // =========================================================================
    // 2. RUN MANAGER INITIALIZATION
    // =========================================================================

    // Construct the Multithreaded Run Manager
    // (Replace with G4RunManager for single-threaded sequential mode if needed)
    G4MTRunManager* runManager = new G4MTRunManager();
    
    // Set the number of threads (Default: 1)
    // Increase this number for faster execution on multi-core systems
    runManager->SetNumberOfThreads(1);

    // --- User Initialization Classes ---
    // 1. Geometry
    runManager->SetUserInitialization(new DetectorConstruction());
    // 2. Physics List
    runManager->SetUserInitialization(new PhysicsList());
    // 3. User Actions (Primary Generator, Stepping, Tracking, etc.)
    runManager->SetUserInitialization(new ActionInitialization());

    // Initialize the kernel
    // (This builds the physics tables and geometry)
    runManager->Initialize();

    // =========================================================================
    // 3. UI AND VISUALIZATION MANAGER
    // =========================================================================

    // Get the pointer to the User Interface manager
    G4UImanager* UI = G4UImanager::GetUIpointer();

    // Setup Visualization Manager
    // (This enables graphics like OpenGL, VRML, etc.)
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // Detect Interactive vs. Batch Mode
    G4UIExecutive* ui = nullptr;
    
    // If no command line arguments are provided, start in interactive mode (GUI)
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // =========================================================================
    // 4. SIMULATION EXECUTION
    // =========================================================================

    // --- Configure Physics Verbosity ---
    // Useful for verifying that specific physics models (like NeutronHP) are active
    UI->ApplyCommand("/process/verbose 2");

    // --- Configure Neutron Fission Settings ---
    // Enable explicit fission fragment production for detailed analysis
    UI->ApplyCommand("/process/had/particle_hp/produce_fission_fragment true");

    // (Optional) Enable Wendt Fission Model
    // UI->ApplyCommand("/process/had/particle_hp/use_Wendt_fission_model true");

    // --- Execute Run ---
    if (ui) {
        // Interactive Mode: Open the GUI session
        // Users can type commands like "/run/beamOn 100" manually
        ui->SessionStart();
        delete ui;
    } 
    else {
        // Batch Mode: Execute the macro file provided as an argument
        // Usage: ./your_executable run.mac
        G4String macroFile = argv[1];
        G4String command = "/control/execute " + macroFile;
        UI->ApplyCommand(command);
    }

    // =========================================================================
    // 5. JOB TERMINATION
    // =========================================================================

    // Clean up memory
    delete visManager;
    delete runManager;

    return 0;
}