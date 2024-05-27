// Macro for running Panda digitization tasks
// to run the macro:
// root  recoideal_barrel.C  or in root session root>.x  recoideal_barrel.C
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
    PndIdealTrackFinder *recoideal = new PndIdealTrackFinder();
    fRun->AddTask(recoideal);
    recoideal->SetTrackSelector("NoFtsTrackFunctor");
    recoideal->AddBranchName("STTHit");
    recoideal->AddBranchName("MVDHitsPixel");
    recoideal->AddBranchName("MVDHitsStrip");
    recoideal->AddBranchName("GEMHit");
    // recoideal->SetRelativeMomentumSmearing(0.05);
    // recoideal->SetVertexSmearing(0.05, 0.05, 0.05);
    recoideal->SetTrackingEfficiency(1.);
    recoideal->SetOutputBranchName("BarrelIdealTrack");  // Whatever name we choose.
    recoideal->SetPersistence(kTRUE);

    //----- Init & Run
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
}
