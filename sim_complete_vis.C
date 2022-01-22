// Macro for running Panda simulation  with Geant3  or Geant4 (M. Al-Turany)
// This macro is supposed to run the full simulation of the panda detector
// to run the macro:
// root  sim_complete.C  or in root session root>.x  sim_complete.C
// to run with different options:(e.g more events, different momentum, Geant4)
// root  sim_complete.C"(100, "TGeant4", 2)"

int sim_complete_vis(Int_t nEvents=100, TString prefix="evtcomplete", TString inputGen="llbar_fwp.DEC", Bool_t UseDoubleBox=kFALSE, Double_t pBeam=1.642, Int_t seed=42) {

    std::cout << "FLAGS: " << nEvents << "," << prefix << "," << inputGen << "," << pBeam << std::endl;
    std::cout << "SEED : " << seed << std::endl;
    
    // Set Seed for Random Generator
    // gRandom->SetSeed();
    
    // OR from a specific seed.
    gRandom->SetSeed(seed);
    
    
    /* ************************************************************************
    * TString inputGen =
    * EvtGen -> "xxxxxxxx.dec" (parses dec-file for initial particle)
    *           OR "xxxxxxx.dec:initial_particle"
    *
    * DPM    -> "dpm_xxxxx"
    * FTF    -> "ftf_xxxxx"
    * PYTHIA -> "pythia_xxxxx" (future replacement of DPM/FTF for Bkg Simulation)
    *
    * BOX    -> "box:type(pdgcode,mult):p(min,max):tht(min,max):phi(min,max)"
    * PIPI   -> "pipi:cosTheta(min,max)"
    * LEP    -> "leplep:pid(value):gegm(value):cosTheta(min,max)"
    ************************************************************************ */
    
    TString parAsciiFile = "all.par";
    //TString prefix     = "";		          // "" (auto from inputGen) or "llbar_fwp" or "evtcomplete";
    //TString inputGen   = "llbar_fwp.DEC";   // EvtGen: llbar_fwp.DEC, bkg_xixibar.DEC, etc.
    //TString inputGen   = "dpm";             // BkgGen: dpm, ftf, pythia8 (will be default)
    //TString inputGen   = "box:type(13,10):p(1.0,3.0):tht(22,140):phi(0,360)";
    
    //----- Create the Simulation Run Manager
    PndMasterRunSim *fRun = new PndMasterRunSim();
    
    if(!UseDoubleBox)
	    fRun->SetInput(inputGen);                               // (UseBoxGenerator = !UseDoubleBox)
	    
    fRun->SetName("TGeant4");
    fRun->SetParamAsciiFile(parAsciiFile);
    fRun->SetNumberOfEvents(nEvents);
    fRun->SetBeamMom(pBeam);
	
	//----- Initialization
    fRun->Setup(prefix);

    //----- Geometry
    fRun->CreateGeometry();

    //----- Event Generator
    fRun->SetGenerator();                                       // (UseBoxGenerator = !UseDoubleBox)
	
	
	//--------------------------------------------------------------
    //                     Box Generator mu+ mu-
    //--------------------------------------------------------------
    
	// Double Box Generator
    if (UseDoubleBox) 
    {
        std::cout << "Using Double BoxGenerator..." << std::endl;
        
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
     
    }
    else
        std::cout << "Using Box or EvtGen Generator..." << std::endl;

	//----- Add Sim Tasks
    fRun->AddSimTasks();
    
    //----- Event Display (Store Trajectories)
	fRun->SetStoreTraj(kTRUE);
	
	//----- Trajectory Filter with Cuts (check? gives errors.):
	/*
	FairTrajFilter* trajFilter = FairTrajFilter::Instance();
	trajFilter->SetStepSizeCut(1); 									// 1 cm ???
	trajFilter->SetVertexCut(-2000.,-2000.,4.,2000.,2000.,100.);	// 
	trajFilter->SetMomentumCutP(10e-3); 							// p_lab > 10 MeV
	trajFilter->SetEnergyCut(0.0,1.02); 							// 0 < Etot < 1.04 GeV
	trajFilter->SetStorePrimaries(kTRUE);
	trajFilter->SetStoreSecondaries(kTRUE);
	*/
    
    
    //----- Intialise & Run
    fRun->Init();
    fRun->Run(nEvents); 
    fRun->Finish();
    return 0;
}
