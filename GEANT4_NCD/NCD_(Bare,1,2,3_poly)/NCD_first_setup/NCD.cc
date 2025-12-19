#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4ParticleDefinition.hh"
#include "DetectorConstruction.hh"

#include "PhysicsList.hh"
#include "HadronElasticPhysicsHP.hh"
#include "GammaNuclearPhysics.hh"
#include "ElectromagneticPhysics.hh"

#include "Action.hh"

int main(int argc, char** argv) {

	//choose the Random engine
	G4Random::setTheEngine(new CLHEP::RanecuEngine);

	// Set random seed (fixed or dynamic)
	G4long seed = time(nullptr);
	CLHEP::HepRandom::setTheSeed(seed);
	G4cout << "Random seed = " << seed << G4endl;

	// Log the seed
	std::ofstream seedOut("random_seed.log", std::ios::app);
	seedOut << "Random seed: " << seed << std::endl;
	seedOut.close();
	
	// Construct the default run manager
   /* G4RunManager* runManager = new G4RunManager(); */
	// Use multithreaded run manager
	G4MTRunManager* runManager = new G4MTRunManager();
	runManager->SetNumberOfThreads(1);
	runManager->SetUserInitialization(new DetectorConstruction());
	runManager->SetUserInitialization(new PhysicsList());
	runManager->SetUserInitialization(new ActionInitialization());
	runManager->Initialize();

	//Visualization initialize
	// G4VisManager* visManager = new G4VisExecutive;
	// visManager->Initialize();


	// Get the pointer to the User Interface manager
G4UImanager* UI = G4UImanager::GetUIpointer();

// Only create a UI if no arguments are passed
G4UIExecutive* ui = nullptr;
if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
}

// --- Visualization setup ---
// (Optional, only if you use visualization)
// G4VisManager* visManager = new G4VisExecutive;
// visManager->Initialize();

// --- Run initialization ---
runManager->Initialize();

// --- Batch or interactive mode ---
if (ui) {
    // Interactive mode
    UI->ApplyCommand("/process/verbose 2");
    ui->SessionStart();
    delete ui;
} else {
    // Batch mode (used in SLURM)
    G4String macroFile = argv[1];
    G4String command = "/control/execute " + macroFile;
    UI->ApplyCommand(command);
}
	/*UI->ApplyCommand("/run/numberOfThreads 10");*/


	//Use verbosity levels if troubleshooting is required 
	//UI->ApplyCommand("/run/verbose 2");          // Set run manager verbosity
	//UI->ApplyCommand("/event/verbose 2");        // Set event verbosity
	//UI->ApplyCommand("/tracking/verbose 2");     // Set tracking verbosity
	//UI->ApplyCommand("/step/verbose 2");         // Set step verbosity
	UI->ApplyCommand("/process/verbose 2");      // Set process verbosity

	//Particle Filter: Use it to see particular particle tracks in visualization 
	//UI->ApplyCommand("/vis/filtering/trajectories/create/particleFilter");
	//UI->ApplyCommand("/vis/filtering/trajectories/particleFilter-0/add alpha");

	
	//Sets the max amount of events that are visualized
	// UI->ApplyCommand("/vis/scene/endOfEventAction accumulate 100");



	//Turn the Fission Fragements on,but set verbose inside UI itself
	UI->ApplyCommand("/process/had/particle_hp/produce_fission_fragment true");
	/*UI->ApplyCommand("/control/execute AmBe_TestSource.mac");*/
	// UI->ApplyCommand("/control/execute macro/MultipleRuns.mac");
	//Wendt Fission Model
	//UI->ApplyCommand("/process/had/particle_hp/use_Wendt_fission_model true");



	//Visualization termination
	// delete visManager;

	//Job termination 
	//
	delete runManager;

	return 0;
}
