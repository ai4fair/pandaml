// Macro for running Panda reconstruction tasks
// to run the macro:
// root  reco_complete.C  or in root session root>.x  reco_complete.C
int skew_complete(Int_t nEvents=10, TString prefix="") {

    std::cout << "FLAGS: " << nEvents << "," << prefix << std::endl;
    std::cout << std::endl;
    
    //----- User Settings
    TString parAsciiFile = "all.par";
    //TString prefix     = "llbar_fwp";        // "llbar_fwp", "evtcomplete";
    TString input        = "";                 // "dpm", "llbar_fwp.DEC";
    TString friend1		 = "sim";
    TString friend2      = "digi";
    TString friend3      = "";
    TString friend4      = "";
    TString output       = "skew";
    
    
     //----- Init Settings
    PndMasterRunAna *fRun= new PndMasterRunAna();
    fRun->SetInput(input);
    fRun->AddFriend(friend1);
    fRun->AddFriend(friend2);
    fRun->AddFriend(friend3);
    fRun->AddFriend(friend4);
    fRun->SetOutput(output);
    fRun->SetParamAsciiFile(parAsciiFile);
    fRun->Setup(prefix);

    //----- Add SkewedCombined
    FairGeane *Geane = new FairGeane();
    fRun->AddTask(Geane);
    PndSttSkewedCombineTask *SkewedCombined = new PndSttSkewedCombineTask();
    SkewedCombined->SetPersistence(kTRUE);
    fRun->AddTask(SkewedCombined);
    
    //----- Init & Run
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;

}
