int tracking(Int_t nEvents = 10000) {

	//-----User Settings:------------------------------------------------------
    TString  parAsciiFile   = "all.par";
    TString  prefix         = "dpm";
    TString  input          = "dpm";
    TString  output         = "rec";
    TString  friend1        = "sim";
    TString  friend2        = "digi_timebased";
    TString  friend3        = "";
    TString  friend4        = "";
    TString  fOptions       = "gf2"; // "gf2" for genfit 2

    // -----   Initial Settings   --------------------------------------------
    PndMasterRunAna *fRun= new PndMasterRunAna();
    fRun->SetInput(input);
    fRun->SetOutput(output);
    fRun->AddFriend(friend1);
    fRun->AddFriend(friend2);
    fRun->AddFriend(friend3);
    fRun->AddFriend(friend4);
    fRun->SetParamAsciiFile(parAsciiFile);
    fRun->Setup(prefix);

    FairGeane *Geane = new FairGeane();
    fRun->AddTask(Geane);
    
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
    
    /*
    // CellTrackFinder
    PndSttCellTrackFinderTask *cellTrackFinder = new PndSttCellTrackFinderTask();
    cellTrackFinder->AddHitBranch("STTHit");
    //cellTrackFinder->SetAnalyseSteps(true);
    //cellTrackFinder->SetClusterTime(200);
    cellTrackFinder->SetRunTimeBased(kFALSE);
    cellTrackFinder->SetRunWithSortedHits(kFALSE);
    cellTrackFinder->SetCalcWithCorrectedIsochrones(kFALSE);
    fRun->AddTask(cellTrackFinder);
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
    PndMvdTrackerTask *mvdTracker = new PndMvdTrackerTask();
    mvdTracker->SetInputTrackBranchName("FinalTrack");
    mvdTracker->SetInputTrackCandBranchName("FinalTrackCand");
    mvdTracker->SetInputRiemannTrackBranchName("FinalRiemannTrack");
    mvdTracker->IncludeDetector(true,false,false);
    mvdTracker->SetUseRiemannTrack(true);
    mvdTracker->SetMvdHitDist(3.0);
    mvdTracker->SetRunIn3D(true);
    mvdTracker->SetNoAreaExclusion(true);
    mvdTracker->SetUseHemisphere(true);
    mvdTracker->SetWeightsMvdHit(10.0);
    mvdTracker->SetUseHelix(false);
    mvdTracker->DrawTracks(true);
    fRun->AddTask(mvdTracker);
    */
    
    
    /*
    PndTrackingQATask* trackingQA = new PndTrackingQATask("FinalTrack", "IdealTrack");
    trackingQA->SetFunctorName("StandardTrackFunctor");     // Default StandardTrackFunctor
    //trackingQA->AddHitsBranchName("STTHit");              // Default MVDHitsPixel, MVDHitsStrip, STTHit, GEMHit, FTSHit 
    trackingQA->SetRunTimeBased(kFALSE);                    // Default kFALSE
    trackingQA->SetVerbose(0);                              // Deafult 1 (derived from FairTask)
    fRun->AddTask(trackingQA); 
    */


    // -----   Intialise and run   --------------------------------------------
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
}
