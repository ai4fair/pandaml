// Macro for running Panda digitization tasks
// to run the macro:
// root  digi_complete.C  or in root session root>.x  digi_complete.C
int pidideal_complete(Int_t nEvents = 0) {

    //----- User Settings
    TString  parAsciiFile  = "all.par";
    TString  prefix        = "evtcomplete";
    //TString  input       = "llbar_fwp.dec";
    TString  input         = "box:type(13,10):p(.3,.7):tht(22,140):phi(0,360)";
    TString  friend1       = "sim";
    TString  friend2       = "digi";
    TString  friend3       = "reco";
    TString  friend4       = "";
    TString  output        = "pid";

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
    fRun->AddPidTasks();

    //----- Intialise & Run
    PndEmcMapper::Init(1);
    fRun->Init();
    fRun->Run(0, nEvents);
    fRun->Finish();
    return 0;
}
