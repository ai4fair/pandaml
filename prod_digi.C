// Macro for running Panda digitization tasks
// to run the macro:
// root  digi_complete.C  or in root session root>.x  digi_complete.C
int prod_digi(Int_t nEvents=10, TString prefix="ll") {
    
    std::cout << "FLAGS: " << nEvents << "," << prefix << std::endl;
    
    //----- User Settings
    TString parAsciiFile = "all.par";
    //TString prefix     = "llbar_fwp";        // "llbar_fwp", "evtcomplete";
    TString input        = "";                 // "dpm", "llbar_fwp.DEC";
    TString friend1		 = "sim";
    TString friend2      = "";
    TString friend3      = "";
    TString friend4      = "";
    TString output       = "digi";
    TString fOption      = "";
    
    //----- Init Settings
    PndMasterRunAna *fRun= new PndMasterRunAna();
    fRun->SetInput(input);
    fRun->SetOutput(output);
    fRun->AddFriend(friend1);
    fRun->AddFriend(friend2);
    fRun->AddFriend(friend3);
    fRun->AddFriend(friend4);
    fRun->SetParamAsciiFile(parAsciiFile);
    fRun->Setup(prefix);
    
    //----- Add Options
    if (fOption != "")
        fRun->SetOptions(fOption);
    
    //----- AddDigiTasks
    fRun->AddDigiTasks();
    
    //----- Intialise & Run
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
}
