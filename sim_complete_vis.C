// Macro for running Panda simulation  with Geant3  or Geant4 (M. Al-Turany)
// This macro is supposed to run the full simulation of the panda detector
// to run the macro:
// root  sim_complete.C  or in root session root>.x  sim_complete.C
// to run with different options:(e.g more events, different momentum, Geant4)
// root  sim_complete.C"(100, "TGeant4",2)"

int sim_complete_vis(Int_t nEvents=10000, TString  SimEngine="TGeant4", Double_t BeamMomentum=1.642)
{
	/* ************************************************************************
	* TString inputGenerator = 
	* EvtGen -> "xxxxxxxx.dec" (parses dec-file for initial particle) OR
	* 			"xxxxxxx.dec:initial_particle"
	* DPM    -> "dpm_xxxxx"
	* FTF    -> "ftf_xxxxx"
	* BOX    -> "box:type(pdgcode,mult):p(min,max):tht(min,max):phi(min,max)"
	************************************************************************ */
	
	TString parAsciiFile     = "all.par";
	TString prefix           = "evtcomplete";		// prefix for output files
	//TString inputGenerator = "dpm";
	//TString inputGenerator = "ftf";
	//TString inputGenerator = "llbar_fwp.dec";
	//TString inputGenerator = "box:type(13,10):p(1,2):tht(22,140):phi(0,360)";
	TString inputGenerator   = "box:type(13,10):p(3,7):tht(22,140):phi(0,360)";
	
	Bool_t UseDoubleBox      = kTRUE;
		
		
	//Create the Simulation Run Manager
	PndMasterRunSim *fRun = new PndMasterRunSim();
	
	if(!UseDoubleBox)
	    fRun->SetInput(inputGenerator);                               // (UseBoxGenerator = kTRUE)
	    
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
    
    
	// Box Generator
    if (UseDoubleBox) 
    {
        std::cout << "Using BoxGenerator..." << std::endl;
        
        // 1st BoxGenerator
        FairBoxGenerator* boxGen1 = new FairBoxGenerator(13, 5);    // 13 = muon; 5 = multiplicity
        boxGen1->SetPRange(1.0, 3.0);                               // GeV/c (1.0 to 3.0)
        boxGen1->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
        boxGen1->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree]
        boxGen1->SetXYZ(0., 0., 0.);                                // mm or cm ??
        fRun->AddGenerator(boxGen1);

        // 2nd BoxGenerator
        FairBoxGenerator* boxGen2 = new FairBoxGenerator(-13, 5);   // -13 = antimuon; 5 = multiplicity
        boxGen2->SetPRange(1.0, 3.0);                               // GeV/c (1.0 to 3.0)
        boxGen2->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
        boxGen2->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree]
        boxGen2->SetXYZ(0., 0., 0.);                                // mm or cm ??
        fRun->AddGenerator(boxGen2);
     
    }//end
	
	// Add tasks
	fRun->AddSimTasks();
	
	
	// Event Display (Store Trajectories)
	fRun->SetStoreTraj(kTRUE);
	
	//Trajectory Filter with Cuts (check? gives errors.):
	/*
	FairTrajFilter* trajFilter = FairTrajFilter::Instance();
	trajFilter->SetStepSizeCut(1); 									// 1 cm ???
	trajFilter->SetVertexCut(-2000.,-2000.,4.,2000.,2000.,100.);	// 
	trajFilter->SetMomentumCutP(10e-3); 							// p_lab > 10 MeV
	trajFilter->SetEnergyCut(0.0,1.02); 							// 0 < Etot < 1.04 GeV
	trajFilter->SetStorePrimaries(kTRUE);
	trajFilter->SetStoreSecondaries(kTRUE);
	*/
	
	// Intialise and run
	fRun->Init();
	fRun->Run(nEvents); 
	fRun->Finish();
	
	return 0;
}

