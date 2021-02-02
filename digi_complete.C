// Macro for running Panda digitization tasks
// to run the macro:
// root  digi_complete.C  or in root session root>.x  digi_complete.C
int digi_complete(Int_t nEvents = 0) {

    //----- User Settings
    TString  parAsciiFile  = "all.par";
    TString  prefix        = "evtcomplete";
    //TString  input       = "llbar_fwp.dec";
    TString  input         = "box:type(13,10):p(.3,.7):tht(22,140):phi(0,360)";
    TString  friend1       = "sim";
    TString  friend2       = "";
    TString  friend3       = "";
    TString  friend4       = "";
    TString  output        = "digi";
    
    
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


    //----- Add Tasks
    fRun->AddDigiTasks();


    //----- Intialise & Run
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
}
