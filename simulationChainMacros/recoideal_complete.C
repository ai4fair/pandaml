// c++ includes
#include <iostream>

// ROOT includes
#include <TROOT.h>

// PandaRoot includes
#include <PndMasterRunAna.h>
#include <PndEmcMapper.h>

int recoideal_complete(Int_t nEvents=10, TString prefix="") {
    
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

    // Possible input files for the track reconstruction
    TString friend1      = "sim";
    TString friend2      = "digi";
    TString friend3      = "";
    TString friend4      = "";

    // Output file name after the prefix
    TString output       = "reco";
    
    //--------------------------------------------------------------//
	//             Create the Reconstruction Run Manager            //
	//--------------------------------------------------------------//

    PndMasterRunAna *fRun = new PndMasterRunAna();

    fRun->SetInput(input);					// Input (decay) file used for the simulation
    fRun->AddFriend(friend1);				// Input file 1 (sim root file)
    fRun->AddFriend(friend2);				// Input file 2 (digi root file)
    fRun->AddFriend(friend3);				// Input file 3
    fRun->AddFriend(friend4);				// Input file 4
    fRun->SetOutput(output);				// Output file (reco root file)
    fRun->SetParamAsciiFile(parAsciiFile);	// Input file with detector parameters
    
	// -----  Initialization   ------------------------------------------------
	LOG(INFO) << "Setting up the run...";
    fRun->Setup(prefix);
    
    // -----   Add tasks   ----------------------------------------------------
	LOG(INFO) << "Adding ideal reconstruction tasks...";
    fRun->AddRecoIdealTasks();
    
	// -----   Intialise and run   --------------------------------------------
	LOG(INFO) << "Initialising and running the track reconstruction...";
    PndEmcMapper::Init(1);
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
}
