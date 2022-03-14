// Macro for running Panda reconstruction tasks
// to run the macro:
// root  recoideal_complete.C  or in root session root>.x  recoideal_complete.C
int recoideal_complete(Int_t nEvents=10, TString prefix="") {
    
    std::cout << "FLAGS: " << nEvents << "," << prefix << "," << options << std::endl;
    std::cout << std::endl;
    
    //----- User Settings    
    TString parAsciiFile = "all.par";
    //TString prefix     = "llbar_fwp";        // "llbar_fwp", "evtcomplete";
    TString input        = "";                 // "dpm", "llbar_fwp.DEC";
    TString friend1      = "sim";
    TString friend2      = "digi";
    TString friend3      = "";
    TString friend4      = "";
    TString output       = "reco";
    
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
    
    //----- AddRecoIdealTasks
    fRun->AddRecoIdealTasks();
    
    //----- Intialise & Run
    PndEmcMapper::Init(1);
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
}
