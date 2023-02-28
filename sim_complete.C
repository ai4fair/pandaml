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
        
    //TString prefix     = "";		          // "" (auto from inputGen) or "llbar_fwp" or "evtcomplete";
    //TString inputGen   = "llbar_fwp.dec";   // EvtGen: llbar_fwp.dec, bkg_xixibar.dec, etc.
    //TString inputGen   = "ftf";             // BkgGen: dpm, ftf, pythia8 set as fRun->SetInput(inputGen)
    //TString inputGen   = "box:type(13,10):p(1.0,3.0):tht(22,140):phi(0,360)";
    
    TString decayMode1 = "UserDecayConfig1.C";// only for Xibar_Xi1820.dec
    TString decayMode2 = "UserDecayConfig2.C";// only for llbar_fwp_1_642GeV.dec
    
    //----- Init Settings
    PndMasterRunSim *fRun = new PndMasterRunSim();
    
    
    //--------------------------------------------------------------//
    //                     Select an Event Generator                //
    
    // Signal Studies
    
    // (EvtGen Generator
    if (inputGen.Contains("dec")) {
        std::cout << "-I- Using EvtGen Generator..." << std::endl;
        fRun->SetInput(inputGen);
    }
        
    // EvtGen Generator (Explicit Init.)
    if (inputGen.Contains("EvtGenFWP")) {
    
        std::cout << "-I- Using EvtGen (Explicitly) Generator..." << std::endl;
        
        inputGen = "llbar_fwp.dec";
        PndEvtGenDirect* evtGenDirect = new PndEvtGenDirect("pbarpSystem", inputGen.Data(), pBeam);
        evtGenDirect->SetStoreTree(kFALSE);
        fRun->AddGenerator(evtGenDirect);
    }
    
    if (inputGen.Contains("EvtGenBKG")) {
    
        std::cout << "-I- Using EvtGen (Explicitly) Generator..." << std::endl;
        
        inputGen = "llbar_bkg.dec";
        PndEvtGenDirect* evtGenDirect = new PndEvtGenDirect("pbarpSystem", inputGen.Data(), pBeam);
        evtGenDirect->SetStoreTree(kFALSE);
        fRun->AddGenerator(evtGenDirect);
    }
    
    // Single Box Generator
    if (inputGen.Contains("SBoxGEN")) {
    
        std::cout << "-I- Using Single BoxGenerator..." << std::endl;
        
        FairBoxGenerator* boxGen = new FairBoxGenerator(13, 5);    // 13 = muon; 5 = multiplicity
        boxGen->SetPRange(1.0, 3.0);                               // GeV/c (1.0 to 3.0)
        boxGen->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
        boxGen->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree], STT
        boxGen->SetXYZ(0., 0., 0.);                                // mm or cm ??  IP we can shift it in R and Z directions
        fRun->AddGenerator(boxGen);
    }
    
    // Double Box Generator
    if (inputGen.Contains("DBoxGEN")) {
    
        std::cout << "-I- Using Double BoxGenerator..." << std::endl;
        
        // 1st BoxGenerator
        FairBoxGenerator* boxGen1 = new FairBoxGenerator(3122, 3);  // 13=muon; 3122=Lambda; 5=multiplicity
        boxGen1->SetPRange(0.1, 1.5);                               // GeV/c (1.0 to 3.0), 100 MeV to 1.5 GeV
        boxGen1->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
        boxGen1->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree], STT
        //boxGen1->SetThetaRange(3., 150.);                         // Polar angle in lab system range [degree], CTS
        boxGen1->SetXYZ(0., 0., 0.);                                // mm or cm ??  IP we can shift it in R and Z directions
        fRun->AddGenerator(boxGen1);

        // 2nd BoxGenerator
        FairBoxGenerator* boxGen2 = new FairBoxGenerator(-3122, 3); // -13=antimuon; -3122=anti-Lambda; 5=multiplicity
        boxGen2->SetPRange(0.1, 1.5);                               // GeV/c (1.0 to 3.0), 100 MeV to 1.5 GeV
        boxGen2->SetPhiRange(0., 360.);                             // Azimuth angle range [degree]
        boxGen2->SetThetaRange(22., 140.);                          // Polar angle in lab system range [degree], STT
        //boxGen2->SetThetaRange(3., 150.);                         // Polar angle in lab system range [degree], CTS
        boxGen2->SetXYZ(0., 0., 0.);                                // mm or cm ?? IP we can shift it in R and Z directions
        fRun->AddGenerator(boxGen2);
     
    }
    
    // Background Studies
    // It is a coctail of everything from pbarp interaction
    
    if (inputGen.Contains("dpm")) {
        std::cout << "-I- Using DPM Generator..." << std::endl;
        fRun->SetInput(inputGen);
    }
    
    if (inputGen.Contains("ftf")) {
        std::cout << "-I- Using FTF Generator..." << std::endl;
        fRun->SetInput(inputGen);
    }
    
    if (inputGen.Contains("pythia8")) {
        std::cout << "-I- Using Pythia8 Generator..." << std::endl;
        fRun->SetInput(inputGen);
    }
    
    //                                                              //
    //--------------------------------------------------------------//
    
    
    //----- Init Settings
    fRun->SetName("TGeant4");
    fRun->SetParamAsciiFile(parAsciiFile);
    fRun->SetNumberOfEvents(nEvents);
    fRun->SetBeamMom(pBeam);
    fRun->SetStoreTraj(kTRUE);
    //fRun->SetUserDecay(decayMode);                                // Force FairRoot for a certain decay chains
    
	//----- Init
    fRun->Setup(prefix);

    //----- Geometry
    fRun->CreateGeometry();

    //----- Generator
    fRun->SetGenerator();
   
    //----- AddSimTasks
    fRun->AddSimTasks();
    
    //----- Init & Run
    fRun->Init();
    fRun->Run(nEvents); 
    fRun->Finish();
    return 0;
}
