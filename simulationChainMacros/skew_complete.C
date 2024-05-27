// Macro for running Panda reconstruction tasks
// to run the macro:
// root  reco_complete.C  or in root session root>.x  reco_complete.C

// c++ includes
#include <iostream>

// ROOT includes
#include <TROOT.h>

// FairRoot includes
#include <FairGeane.h>

// PandaRoot includes
#include <PndMasterRunAna.h>
#include <PndSttSkewedCombineTask.h>

int skew_complete(Int_t nEvents=10, TString prefix="") 
{
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
	TString friend2      = "digi";
	TString friend3      = "";
	TString friend4      = "";
	
	// Output file name after the prefix
	TString output       = "skew";

    //--------------------------------------------------------------//
	//                  Create the Run Manager                      //
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
    LOG(INFO) << "Adding GEANE tasks...";
    // The GEANE allows particle trajectory transport (the track parameters and errors) through the given geometry and magnetic fields
    FairGeane *Geane = new FairGeane();
    fRun->AddTask(Geane);
    
    LOG(INFO) << "Adding skewed straws correction tasks...";
    PndSttSkewedCombineTask *SkewedCombined = new PndSttSkewedCombineTask();
    SkewedCombined->SetPersistence(kTRUE);
    fRun->AddTask(SkewedCombined);
    
    // -----   Intialise and run   --------------------------------------------
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;

}
