// Macro for running Panda digitization tasks
// to run the macro:
// root  reco_complete.C  or in root session root>.x  reco_complete.C
int recoideal_barrel(Int_t nEvents=10, TString prefix="") {
    
    std::cout << "FLAGS: " << nEvents << "," << prefix << std::endl;
    std::cout << std::endl;
    
    //----- User Settings    
    TString parAsciiFile = "all.par";
    //TString prefix     = "llbar_fwp";        // "llbar_fwp", "evtcomplete";
    TString input        = "";                 // "dpm", "llbar_fwp.DEC";
    TString friend1      = "sim";
    TString friend2      = "digi";
    TString friend3      = "";
    TString friend4      = "";
    TString output       = "recobarrel";       // 'recobarrel' to identify it from 'reco'
    
    //----- Init Settings
    PndMasterRunAna *fRun = new PndMasterRunAna();
    fRun->SetInput(input);
    fRun->AddFriend(friend1);
    fRun->AddFriend(friend2);
    fRun->AddFriend(friend3);
    fRun->AddFriend(friend4);
    fRun->SetOutput(output);
    fRun->SetParamAsciiFile(parAsciiFile);
    fRun->Setup(prefix);
    
    //----- AddRecoIdealTasks (Only Barrel Part)
    PndIdealTrackFinder *trkx = new PndIdealTrackFinder();
    fRun->AddTask(trkx);
    trkx->SetTrackSelector("NoFtsTrackFunctor");
    trkx->AddBranchName("STTHit");
    trkx->AddBranchName("MVDHitsPixel");
    trkx->AddBranchName("MVDHitsStrip");
    trkx->AddBranchName("GEMHit");
    // trkx->SetRelativeMomentumSmearing(0.05);
    // trkx->SetVertexSmearing(0.05, 0.05, 0.05);
    trkx->SetTrackingEfficiency(1.);
    trkx->SetOutputBranchName("BarrelIdealTrack");  // Whatever name we choose.
    trkx->SetPersistence(kTRUE);

    //----- Init & Run
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
}
