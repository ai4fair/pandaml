// c++ includes
#include <iostream>

// ROOT includes
#include <TROOT.h>
#include <TRandom.h>

// FairRoot includes
#include <FairBoxGenerator.h>
#include <FairLogger.h>

// PandaRoot includes
// #include <PndMasterRunSim.h>
#include <PndEvtGenDirect.h>
#include <PndEmcGeoPar.h>

int sim_complete(Int_t nEvents=10, TString prefix="", TString inputGen="", Double_t pBeam=1.642, Int_t seed=42) 
{
	// Print the input parameters
	std::cout << std::endl;
	LOG(info) << "========== Input Parameters ==========";
	LOG(info) << "nEvents: " 	<< nEvents;
	LOG(info) << "prefix: " 	<< prefix.Data();
	LOG(info) << "inputGen: " 	<< inputGen.Data();
	LOG(info) << "pBeam: " 		<< pBeam;
	LOG(info) << "seed: " 		<< seed;
	std::cout << std::endl;
	
	//-------------------------------------------------------------------------//
	//                               User Settings                             //
	//-------------------------------------------------------------------------//

	TString parAsciiFile = "all.par"; // File that contains all detector parameters
		
	// TString inputGenerator =
	// EvtGen -> "xxxxxxxx.dec" (parses dec-file for initial particle) or "xxxxxxx.dec:initial_particle"
	// DPM    -> "dpm_xxxxx"
	// FTF    -> "ftf_xxxxx"
	// BOX    -> "box:type(pdgcode,mult):p(min,max):tht(min,max):phi(min,max)"
	// PIPI   -> "pipi:cosTheta(min,max)"
	// LEP    -> "leplep:pid(value):gegm(value):cosTheta(min,max)"

	// Examples:
	// TString inputGenerator = "psi2s_Jpsi2pi_Jpsi_mumu.dec";
	// TString inputGenerator = "ftf";
	// TString inputGenerator = "ftf";
	// TString inputGenerator = "box:type(211,1):p(1,1):tht(10,120):phi(0,360)";
	
	// TString decayMode1 = "UserDecayConfig1.C";// only for Xibar_Xi1820.dec
	// TString decayMode2 = "UserDecayConfig2.C";// only for llbar_fwp_1_642GeV.dec
	
	//--------------------------------------------------------------//
	//             Create the Simulation Run Manager                //
	//--------------------------------------------------------------//
	
	PndMasterRunSim *fRun = new PndMasterRunSim();

	// Select which generator to use
	
	// EvtGen Generator
	if (inputGen.Contains("dec")) 
	{
		LOG(info) << "Using the EvtGen generator...";
		std::ifstream decayFile(inputGen.Data());
		if (!decayFile) 
		{
			LOG(ERROR) << "File " << inputGen.Data() << " not found!";
			return 1;
		}
		LOG(info) << "Input generator: " << inputGen.Data();
		fRun->SetInput(inputGen);
	}
		
	if (inputGen.Contains("EvtGenFWP")) 
	{    
		LOG(info) << "Using EvtGen generator with llbar_fwp.dec...";
		
		inputGen = "llbar_fwp.dec";
		PndEvtGenDirect* evtGenDirect = new PndEvtGenDirect("pbarpSystem", inputGen.Data(), pBeam);
		evtGenDirect->SetStoreTree(kFALSE);
		fRun->AddGenerator(evtGenDirect);
	}
	
	if (inputGen.Contains("EvtGenBKG")) 
	{
		LOG(info) << "Using EvtGen generator with llbar_bkg.dec ...";
		
		inputGen = "llbar_bkg.dec";
		PndEvtGenDirect* evtGenDirect = new PndEvtGenDirect("pbarpSystem", inputGen.Data(), pBeam);
		evtGenDirect->SetStoreTree(kFALSE);
		fRun->AddGenerator(evtGenDirect);
	}
	
	// Single Box Generator
	if (inputGen.Contains("SBoxGEN")) 
	{
		LOG(info) << " Using a single BoxGenerator...";
		
		FairBoxGenerator* boxGen = new FairBoxGenerator(13, 5);    // 13 = muon; 5 = multiplicity
		boxGen->SetName("boxGen5mum");							   // Name of the generator for the log file
		boxGen->SetPRange(1.0, 3.0);                               // GeV/c (1.0 to 3.0)
		boxGen->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
		boxGen->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree], STT
		boxGen->SetXYZ(0., 0., 0.);                                // Set vertex position in [cm]
		fRun->AddGenerator(boxGen);
	}
	
	// Double Box Generator
	if (inputGen.Contains("DBoxGEN")) 
	{
		LOG(info) << "Using Double BoxGenerator...";
		
		// 1st BoxGenerator
		FairBoxGenerator* boxGen1 = new FairBoxGenerator(13, 5);    // 13=muon; 3122=Lambda; multiplicity
		boxGen1->SetName("boxGen for 5 mu-");				// Name of the generator for the log file
		boxGen1->SetPRange(0.1, 1.5);                               // Momentum Range: 100 MeV to 1.5 GeV
		boxGen1->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
		boxGen1->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree], STT
		//boxGen1->SetThetaRange(3., 150.);                         // Polar angle in lab system range [degree], CTS
		boxGen1->SetXYZ(0., 0., 0.);                                // Set vertex position in [cm]
		fRun->AddGenerator(boxGen1);

		// 2nd BoxGenerator
		FairBoxGenerator* boxGen2 = new FairBoxGenerator(-13, 5);	// -13=antimuon; -3122=anti-Lambda; multiplicity
		boxGen2->SetName("boxGen for 5 mu+");						// Name of the generator for the log file
		boxGen2->SetPRange(0.1, 1.5);                          		// Momentum range: 100 MeV to 1.5 GeV
		boxGen2->SetPhiRange(0., 360.);                          	// Azimuth angle range [degree]
		boxGen2->SetThetaRange(22., 140.);                       	// Polar angle in lab system range [degree], STT
		//boxGen2->SetThetaRange(3., 150.);                      	// Polar angle in lab system range [degree], CTS
		boxGen2->SetXYZ(0., 0., 0.);                             	// Set vertex position in [cm]
		fRun->AddGenerator(boxGen2);
	 
	}
	
	// Background Studies
	// It is a cocktail of everything from pbarp interaction
	
	if (inputGen.Contains("dpm")) 
	{
		LOG(info) << "Using the DPM generator...";
		fRun->SetInput(inputGen);
	}
	
	if (inputGen.Contains("ftf")) 
	{
		LOG(info) << "Using the FTF generator...";
		fRun->SetInput(inputGen);
	}
	
	if (inputGen.Contains("pythia8")) 
	{
		LOG(info) << "Using the Pythia8 generator...";
		fRun->SetInput(inputGen);
	}    
	
	// other settings for the generator and propagator
	fRun->SetName("TGeant4");               // Engine for the detector simulation
	fRun->SetParamAsciiFile(parAsciiFile);  // Name of the Ascii file that contains all the detector parameters (It automatically searches in the $VMCWORKDIR)
	fRun->SetNumberOfEvents(nEvents);       // Number of events to be generated
	fRun->SetBeamMom(pBeam);                // Beam momentum of the anti-protons in GeV/c
	fRun->SetStoreTraj(kTRUE);              // Store the trajectories of the particles
	//fRun->SetUserDecay(decayMode);        // Force FairRoot for a certain decay chains (whatever that means)
	
	// -----  Initialization   ------------------------------------------------
	LOG(info) << "Setting up the run...";
	fRun->Setup(prefix);

	// -----   Geometry   -----------------------------------------------------
	LOG(info) << "Creating the geometry...";
	fRun->CreateGeometry();

	// -----   Event generator   ----------------------------------------------
  	fRun->SetGenerator();

	// -----   Add tasks   ----------------------------------------------------
	LOG(info) << "Adding simulation tasks...";
	fRun->AddSimTasks();
	
	// -----   Intialise and run   --------------------------------------------
	LOG(info) << "Initialising and running the simulation...";

	fRun->Init(); // There still seem to be some issues with initializing some EMC functions when the container is loaded
	fRun->Run(nEvents); 
	fRun->Finish();

	return 0;
}
