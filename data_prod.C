bool CheckFile(TString fn);
int data_prod(Int_t nEvents=10, TString prefix="", TString outputdir="", TString flag="", Int_t Job_Id=0) {
    
    std::cout << "\nFLAGS: " << nEvents << "," << prefix << "," << outputdir << "," << Job_Id << "," << flag << std::endl;

    // Cluster ROOTs
    int from = 110, to = 110;    
    TString parFile  = TString::Format("%s_%d_par.root", prefix.Data(), from);
    TString simFile  = TString::Format("%s_%d_sim.root", prefix.Data(), from);
    TString digiFile = TString::Format("%s_%d_digi.root", prefix.Data(), from);
    TString recoFile = TString::Format("%s_%d_reco.root", prefix.Data(), from);
    TString outFile  = "out.root";  // dummy file for FairRunAna


    // Initialization
    FairLogger::GetLogger()->SetLogToFile(kFALSE);
    FairRunAna *fRun = new FairRunAna();

    // Add Input Files to FairFileSource

    // (1) Add First File to FairFileSource
    FairFileSource *fSrc = new FairFileSource(simFile);

    // (2) Add Rest Files to FairFileSource as AddFriend
    for (int i=from+1; i<=to; ++i) {
        TString simFile  = TString::Format("%s_%d_sim.root", prefix.Data(), i);
        if (CheckFile(simFile)) fSrc->AddFile(simFile);  // Mind 'AddFile()', here
    }

    fSrc->AddFriend(digiFile);
    for (int i=from+1; i<=to; ++i) {
        TString digiFile = TString::Format("%s_%d_digi.root", prefix.Data(), i);
        if (CheckFile(digiFile)) fSrc->AddFriend(digiFile);  // Mind 'AddFriend()', here
    }

    fSrc->AddFriend(recoFile);
    for (int i=from+1; i<=to; ++i) {
        TString recoFile = TString::Format("%s_%d_reco.root", prefix.Data(), i);
        if (CheckFile(recoFile)) fSrc->AddFriend(recoFile);  // Mind 'AddFriend()', here
    }

    fRun->SetSource(fSrc);

    // Add Output File to FairRootFileSink
    FairRootFileSink *fSink = new FairRootFileSink(outFile);
    fRun->SetSink(fSink);

    // FairRuntimeDb
    FairRuntimeDb *rtdb = fRun->GetRuntimeDb();
    FairParRootFileIo *parInput1 = new FairParRootFileIo();
    parInput1->open(parFile.Data());

    // FairParAsciiFileIo
    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();

    TString allDigiFile = gSystem->Getenv("VMCWORKDIR"); 
    allDigiFile += "/macro/params/all.par";

    parIo1->open(allDigiFile.Data(), "in");
    rtdb->setFirstInput(parInput1);
    rtdb->setSecondInput(parIo1);

    // HERE OUR TASK GOES!
    Int_t start_counter = nEvents*Job_Id;
    PndMLTracking *genDB = new PndMLTracking(110000, outputdir, flag);
    fRun->AddTask(genDB);

    // FairRunAna Init
    PndEmcMapper::Init(1);
    fRun->Init();
    fRun->Run(0,nEvents);
    return 0;
}

bool CheckFile(TString fn) {
    
    bool fileok=true;
    TFile fff(fn); 
    if (fff.IsZombie()) fileok=false;

    TTree *t=(TTree*)fff.Get("pndsim");
    if (t==0x0) fileok=false;

    if (!fileok) std::cout << "Skipping Broken File: '"<< fn << "'" << std::endl;
    return fileok;
}
