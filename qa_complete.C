// Macro for running Panda reconstruction tasks
// to run the macro:
// root  qa_complete.C  or in root session root>.x  qa_complete.C
int qa_complete(Int_t nEvents=10, TString prefix="") {
    
    std::cout << "FLAGS: " << nEvents << "," << prefix << std::endl;
    std::cout << std::endl;

    //----- User Settings
    TString parAsciiFile = "all.par";
    //TString prefix     = "llbar_fwp";        // "llbar_fwp", "evtcomplete";
    TString input        = "";                 // "dpm", "llbar_fwp.DEC";
    TString friend1      = "sim";
    TString friend2      = "digi";
    TString friend3      = "reco";       // IdealTrackFinder (Full or Barrell)
    TString friend4      = "mltrkx";           // TrackFinder to Compare
    TString output       = "qa";               // Output filename
    

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
    

    //----- TrackingQATask
    // PndTrackingQA::PndTrackingQA(trackBranchName, idealTrackName, pndTrackData)
    PndTrackingQATask* qa = new PndTrackingQATask("SttTrackCand", "BarrelTrackCand", false);
    qa->SetFunctorName("OnlySttHitFunctor");
    qa->SetVerbose(0);
    fRun->AddTask(qa);


    //----- Init & Run
    PndEmcMapper::Init(1);
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
}

