#include "G4AnalysisManager.hh"
#include "Run.hh"
#include "G4UImanager.hh"
#include "G4Run.hh"
#include <ctime>
#include "PrimaryGeneratorAction.hh"

MyRunAction::MyRunAction()
{
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->RegisterAccumulable(Triton_counts);
	accumulableManager->RegisterAccumulable(Neutron_entered);

	// auto analysisManager = G4AnalysisManager::Instance();
	
	//  // Optional: choose file type (root, csv, xml)
    // analysisManager->SetDefaultFileType("csv");

    // // Create an ntuple for your quantities
    // analysisManager->CreateNtuple("Results", "Triton and Neutron counts");
    // analysisManager->CreateNtupleDColumn("RunID");
	// analysisManager->CreateNtupleDColumn("TotalEvents");
    // //analysisManager->CreateNtupleDColumn("GunEnergy");
    // //analysisManager->CreateNtupleDColumn("NeutronEntered");
    // analysisManager->CreateNtupleDColumn("TritonCounts");
    // analysisManager->FinishNtuple();

}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
	G4AccumulableManager::Instance()->Reset();
	// auto analysisManager = G4AnalysisManager::Instance();

	// // Open an output file
	// G4String filename = "B4.csv";
	// SetFileName(filename);
	// analysisManager->OpenFile(GetFileName());	

}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Merge();
	// G4String fileName;
	// const G4UIcommand* cmd = G4UImanager::GetUIpointer()->FindCommand("/control/alias");
	// fileName = G4UImanager::GetUIpointer()->GetCurrentValue("/control/alias outFile");

	if (IsMaster())
	{
		// auto analysisManager = G4AnalysisManager::Instance();
		//  // Fill one row per run (or per event elsewhere)
        // analysisManager->FillNtupleDColumn(0, run->GetRunID());
        // analysisManager->FillNtupleDColumn(1, run->GetNumberOfEventToBeProcessed());
        // analysisManager->FillNtupleDColumn(2, Triton_counts.GetValue());
        // //analysisManager->FillNtupleDColumn(3, Triton_counts.GetValue());
        // analysisManager->AddNtupleRow();

        // // Write and close
        // analysisManager->Write();
        // analysisManager->CloseFile();
        //G4cout << "Run " << run->GetRunID() << " written to file." << G4endl;
		// std::ofstream file(GetFileName(), std::ios::app);
		std::ofstream file("2inch_response.csv", std::ios::app);
		G4cout<<"FGunEnergy"<<fGunEnergy<<"\n";
		file << run->GetRunID() << ","/*<< fGunEnergy  << ","<< Neutron_entered.GetValue()*/ << run->GetNumberOfEventToBeProcessed() <<
		","<< Triton_counts.GetValue() << ","<< "\n";
		file.close();
		G4cout << "[DEBUG] Triton counter at end of run " << run->GetRunID() << ":" << Triton_counts.GetValue() << G4endl;
		G4cout << "[DEBUG] Neutron counter at end of run " << run->GetRunID() << ":" << Neutron_entered.GetValue() << G4endl;
		/*ResetTritonCounts(); No need when using G4Accumulable
		ResetNeutronEntered();*/
	}
}

void MyRunAction::AddTriton()
{
	Triton_counts++;
	/*G4cout << "[DEBUG] Triton counter incremented in runaction: " << Triton_counts.GetValue() << G4endl;*/
}

G4String MyRunAction::GetFileName()
{
	return fileName;
}
void MyRunAction::SetFileName(G4String filename)
{
	fileName = filename;
}
void MyRunAction::AddNeutronEntered()
{
	Neutron_entered++;
	/*G4cout << "[DEBUG] Neutron counter incremented in runaction: " << Neutron_entered() << G4endl;*/
}
void MyRunAction::SetGunEnergy(G4double E)
{
	fGunEnergy=E;
}