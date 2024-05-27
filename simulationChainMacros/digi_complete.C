// c++ includes
#include <iostream>

// ROOT includes
#include <TROOT.h>

// PandaRoot includes
#include <PndMasterRunAna.h>

int digi_complete(Int_t nEvents=10, TString prefix="") {

	// Print the input parameters
	std::cout << std::endl;
	LOG(INFO) << "========== Input Parameters ==========";
	LOG(INFO) << "nEvents: " 	<< nEvents;
	LOG(INFO) << "prefix: " 	<< prefix.Data();
	std::cout << std::endl;
	
	//-------------------------------------------------------------------------//
	//                               User Settings                             //
	//-------------------------------------------------------------------------//

	TString parAsciiFile = "all.par";   // File that contains all detector parameters
	TString input        = "";          // (Decay) file with which the simulation was performed
	
	// Possible input files for the digitization
	TString friend1		 = "sim";       
	TString friend2      = "";
	TString friend3      = "";
	TString friend4      = "";
	
	// Output file name after the prefix
	TString output       = "digi";

	//--------------------------------------------------------------//
	//             Create the Digitization Run Manager              //
	//--------------------------------------------------------------//

	PndMasterRunAna *fRun= new PndMasterRunAna();
	
	fRun->SetInput(input);					// Input (decay) file used for the simulation
	fRun->AddFriend(friend1);				// Input file 1 (sim root file)
	fRun->AddFriend(friend2);				// Input file 2
	fRun->AddFriend(friend3);				// Input file 3
	fRun->AddFriend(friend4);				// Input file 4
	fRun->SetOutput(output);				// Output file (digi root file)
	fRun->SetParamAsciiFile(parAsciiFile);	// Input file with detector parameters
	
	// -----  Initialization   ------------------------------------------------
	LOG(INFO) << "Setting up the run...";
	fRun->Setup(prefix);

	// -----   Add tasks   ----------------------------------------------------
	LOG(INFO) << "Adding digitization tasks...";
	fRun->AddDigiTasks();

	// -----   Intialise and run   --------------------------------------------
	LOG(INFO) << "Initialising and running the digitization...";
	fRun->Init();
	fRun->Run(0, nEvents);
	fRun->Finish();
	return 0;
}
