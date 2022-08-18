// Macro for running Panda simulation  with Geant3  or Geant4 (M. Al-Turany)
// This macro is supposed to run the full simulation of the panda detector
// to run the macro:
// root  sim_complete.C  or in root session root>.x  sim_complete.C
// to run with different options:(e.g more events, different momentum, Geant4)
// root  sim_complete.C"(100, "TGeant4", 2)"

int sim_complete(Int_t nEvents=10, TString prefix="", TString inputGen="", Double_t pBeam=1.642, Int_t seed=42) {

    std::cout << "\nFLAGS: " << nEvents << "," << prefix << "," << inputGen << "," << pBeam << std::endl;
    std::cout << "SEED : " << seed << std::endl << std::endl;
    
    // Set Seed for Random Generator
    if(seed==0)
        gRandom->SetSeed();
    else
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
    
    //----- User Settings
    TString parAsciiFile = "all.par";
    TString decayMode = "UserDecayConfig.C";
    
    //TString prefix     = "";		          // "" (auto from inputGen) or "llbar_fwp" or "evtcomplete";
    //TString inputGen   = "llbar_fwp.DEC";   // EvtGen: llbar_fwp.DEC, bkg_xixibar.DEC, etc.
    //TString inputGen   = "dpm";             // BkgGen: dpm, ftf, pythia8 (will be default)
    //TString inputGen   = "box:type(13,10):p(1.0,3.0):tht(22,140):phi(0,360)";
    
    
    //----- Init Settings
    PndMasterRunSim *fRun = new PndMasterRunSim();
    
    
    //--------------------------------------------------------------//
    //                     Select an Event Generator                //
    
    // (1) EvtGen Generator (Implicit Init.)
    if (inputGen.Contains("dec")) {
        std::cout << "-I- Using EvtGen Generator..." << std::endl;
        fRun->SetInput(inputGen);
    }
    
    // (2) EvtGen Generator (Explicit Init.)
    if (inputGen.Contains("EvtGenFWP")) {
    
        std::cout << "-I- Using EvtGen (FWP) Generator..." << std::endl;
        
        inputGen = "llbar_fwp.DEC";
        PndEvtGenDirect* evtGenDirect = new PndEvtGenDirect("pbarpSystem", inputGen.Data(), pBeam);
        evtGenDirect->SetStoreTree(kFALSE);
        fRun->AddGenerator(evtGenDirect);
    }
    
    // (3) EvtGen Generator (Explicit Init.)
    if (inputGen.Contains("EvtGenBKG")) {
                
        std::cout << "-I- Using EvtGen (BKG) Generator..." << std::endl;
        
        inputGen = "llbar_bkg.DEC";
        PndEvtGenDirect* evtGenDirect = new PndEvtGenDirect("pbarpSystem", inputGen.Data(), pBeam);
        evtGenDirect->SetStoreTree(kFALSE);
        fRun->AddGenerator(evtGenDirect);
    }
    
    // (4) Single Box Generator
    if (inputGen.Contains("SBoxGEN"))
    {
        std::cout << "-I- Using Single BoxGenerator..." << std::endl;
        
        FairBoxGenerator* boxGen = new FairBoxGenerator(13, 5);    // 13 = muon; 5 = multiplicity
        boxGen->SetPRange(1.0, 3.0);                               // GeV/c (1.0 to 3.0)
        boxGen->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
        boxGen->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree]
        boxGen->SetXYZ(0., 0., 0.);                                // mm or cm ??
        fRun->AddGenerator(boxGen);
    }
    
    // (5) Double Box Generator
    if (inputGen.Contains("DBoxGEN"))
    {
        std::cout << "-I- Using Double BoxGenerator..." << std::endl;
        
        // 1st BoxGenerator
        FairBoxGenerator* boxGen1 = new FairBoxGenerator(13, 5);    // 13 = muon; 5 = multiplicity
        boxGen1->SetPRange(0.1, 1.5);                               // GeV/c (1.0 to 3.0), 100 MeV to 1.5 GeV
        boxGen1->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
        boxGen1->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree]
        boxGen1->SetXYZ(0., 0., 0.);                                // mm or cm ??
        fRun->AddGenerator(boxGen1);

        // 2nd BoxGenerator
        FairBoxGenerator* boxGen2 = new FairBoxGenerator(-13, 5);   // -13 = antimuon; 5 = multiplicity
        boxGen2->SetPRange(0.1, 1.5);                               // GeV/c (1.0 to 3.0), 100 MeV to 1.5 GeV
        boxGen2->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
        boxGen2->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree]
        boxGen2->SetXYZ(0., 0., 0.);                                // mm or cm ??
        fRun->AddGenerator(boxGen2);
     
    }
    
    //                                                              //
    //--------------------------------------------------------------//
    
    
    //----- Init Settings
    fRun->SetName("TGeant4");
    fRun->SetParamAsciiFile(parAsciiFile);
    fRun->SetNumberOfEvents(nEvents);
    fRun->SetBeamMom(pBeam);
    fRun->SetStoreTraj(kTRUE);
    fRun->SetOptions(options);
    fRun->SetUserDecay(decayMode);
    
	//----- Init
    fRun->Setup(prefix);

    //----- Geometry
    fRun->CreateGeometry();

    //----- Event Generator
    fRun->SetGenerator();
   
    //----- AddSimTasks
    fRun->AddSimTasks();
    
    //----- Init & Run
    fRun->Init();
    fRun->Run(nEvents); 
    fRun->Finish();
    return 0;
}
