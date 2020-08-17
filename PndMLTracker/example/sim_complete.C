// Macro for running Panda simulation  with Geant3  or Geant4 (M. Al-Turany)
// This macro is supposed to run the full simulation of the panda detector
// to run the macro:
// root  sim_complete.C  or in root session root>.x  sim_complete.C
// to run with different options:(e.g more events, different momentum, Geant4)
// root  sim_complete.C"(100, "TGeant4",2)"

int sim_complete(Int_t nEvents=10000, TString  SimEngine="TGeant4", Double_t BeamMomentum=4.0)
{
	//User Settings
	TString parAsciiFile= "all.par";
	TString prefix      = "evtcomplete";		// prefix string for output files

	//TString inputGenerator = 
	//EvtGen -> "xxxxxxxx.dec" (parses dec-file for initial particle) or "xxxxxxx.dec:initial_particle"
	//DPM    -> "dpm_xxxxx"
	//FTF    -> "ftf_xxxxx"
	//BOX    -> "box:type(pdgcode,mult):p(min,max):tht(min,max):phi(min,max)"
	
	//TString inputGenerator = "dpm";
	//TString inputGenerator = "ftf";
	TString inputGenerator = "box:type(13,10):p(1,2):tht(10,120):phi(0,360)";
	//TString inputGenerator = "llbar_fwp.dec";
	
	//Create the Simulation Run Manager
	PndMasterRunSim *fRun = new PndMasterRunSim();
	fRun->SetInput(inputGenerator);
	fRun->SetName(SimEngine);
	fRun->SetParamAsciiFile(parAsciiFile);
	fRun->SetNumberOfEvents(nEvents);
	fRun->SetBeamMom(BeamMomentum);
	
	//Initialization
	fRun->Setup(prefix);
	
	//Geometry
	fRun->CreateGeometry();
	
	//Event Generator
	fRun->SetGenerator();

	//Event Filter Setup
	FairFilteredPrimaryGenerator *primGen = fRun->GetFilteredPrimaryGenerator();
	primGen->SetVerbose(1);

	//Example configuration for the event filter
	/*
	FairEvtFilterOnSingleParticleCounts* chrgFilter = 
	new FairEvtFilterOnSingleParticleCounts("chrgFilter");
	chrgFilter->AndMinCharge(4, FairEvtFilter::kCharged);
	primGen->AndFilter(chrgFilter);  
	*/
	
	// Add tasks
	fRun->AddSimTasks();
	
	// Intialise and run
	fRun->Init();
	fRun->Run(nEvents); 
	fRun->Finish();
	
	return 0;
}

