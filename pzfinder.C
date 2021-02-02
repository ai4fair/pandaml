int pzfinder (Int_t nEvents = 10000) {

    /* A FairTask (PndSttSkewStrawPzFinderTask) to find pz */
    
    
    //----- User Settings
    TString  parAsciiFile   = "all.par";
    TString  prefix         = "evtcomplete";
    TString  input          = "llbar_fwp.dec";
    //TString  input        = "bkg_xixibar.DEC";
    //TString  input        = "dpm";
    TString  friend1        = "sim";
    TString  friend2        = "digi";
    TString  friend3        = "";
    TString  friend4        = "";
    TString  output         = "rec_pzfinder";
   
    TString  fOptions       = "gf2";                           // "gf2" for genfit 2

    //----- Initial Settings
    PndMasterRunAna *fRun= new PndMasterRunAna();
    fRun->SetInput(input);
    fRun->SetOutput(output);
    fRun->AddFriend(friend1);
    fRun->AddFriend(friend2);
    fRun->AddFriend(friend3);
    fRun->AddFriend(friend4);
    fRun->SetParamAsciiFile(parAsciiFile);
    fRun->Setup(prefix);

    
    /*
    PndIdealTrackFinder* idealtracking = new PndIdealTrackFinder();
    idealtracking->SetTrackSelector("StandardTrackFunctor");
    idealtracking->SetRelativeMomentumSmearing(0.00001);
    idealtracking->SetVertexSmearing(0.00001, 0.00001, 0.00001);
    //idealtracking->AddBranchName("STTHit");
    idealtracking->SetOutputBranchName("IdealTrack");
    idealtracking->SetRunTimeBased(kFALSE);
    idealtracking->SetPersistence(kTRUE);
    fRun->AddTask(idealtracking);
    */
    

    PndSttSkewStrawPzFinderTask *pzfinder = new PndSttSkewStrawPzFinderTask();
    //pzfinder->setTrackBranch("SttMvdGemTrack");
    //pzfinder->setTrackCandBranch("SttMvdGemTrackCand");
    pzfinder->setTrackBranch("CombiTrack");                     // Output branch from previous tracking
    pzfinder->setTrackCandBranch("CombiTrackCand");             // Output branch from previous tracking
    pzfinder->setOutputPrefix("Final");
    pzfinder->WithRiemann(true);
    pzfinder->setMethod(1);                                     // Recursive annealing fit
    pzfinder->StoreData(true);
    fRun->AddTask(pzfinder); 
    
    /*
    PndTrackingQATask* trackingQA = new PndTrackingQATask("FinalTrack", "IdealTrack");
    
    
    trackingQA->SetFunctorName("StandardTrackFunctor");         // Default StandardTrackFunctor
    //trackingQA->AddHitsBranchName("STTHit");                  // Default MVDHitsPixel, MVDHitsStrip, STTHit, GEMHit, FTSHit 
    trackingQA->SetRunTimeBased(kFALSE);                        // Default kFALSE
    trackingQA->SetVerbose(0);                                  // Deafult 1 (derived from FairTask)
    fRun->AddTask(trackingQA); 
    */


    //----- Intialise & Run
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
    }
