bool CheckFile(TString fn);
int import_prod(Int_t nEvents=10, TString prefix="", TString inputdir="", Int_t start_counter=0) {
    
    // prefix: get sim/digi/reco root files
    // inpudir: track cands from trackml, start_counter: starting name of track cands.

    std::cout << "\nFLAGS: " << nEvents << "," << prefix << "," << inputdir << "," << start_counter << std::endl;

    // Cluster ROOTs
    int from = 110, to = 110;    
    TString parFile  = TString::Format("%s_%d_par.root", prefix.Data(), from);
    TString simFile  = TString::Format("%s_%d_sim.root", prefix.Data(), from);
    TString digiFile = TString::Format("%s_%d_digi.root", prefix.Data(), from);
    TString recoFile = TString::Format("%s_%d_reco.root", prefix.Data(), from);
    TString trkxFile = TString::Format("%s_%d_trkx.root", prefix.Data(), from); // Python TrackCands
    TString outFile  = TString::Format("%s_%d_mltrkx.root", prefix.Data(), from);  // SetOutputFile() of the FairRunAna

    cout << "simFile: " <<  simFile << endl;

    // Initialization
    FairLogger::GetLogger()->SetLogToFile(kFALSE);
    FairRunAna *fRun = new FairRunAna();

    // Add Input Files to FairFileSource

    // (1) Add First File to FairFileSource
    FairFileSource *fSrc = new FairFileSource(simFile);

    // (2) Add Rest Files to FairFileSource as AddFile() and AddFriend()
    for (int i=from+1; i<=to; ++i) {
        TString simFile  = TString::Format("%s_%d_sim.root", prefix.Data(), i);
        if (CheckFile(simFile)) {fSrc->AddFile(simFile); cout << "simFile: " <<  simFile << endl;}
    }

    fSrc->AddFriend(digiFile);
    for (int i=from+1; i<=to; ++i) {
        TString digiFile = TString::Format("%s_%d_digi.root", prefix.Data(), i);
        if (CheckFile(digiFile)) {fSrc->AddFriend(digiFile); cout << "digiFile: " <<  digiFile << endl;}
    }

    fSrc->AddFriend(recoFile);
    for (int i=from+1; i<=to; ++i) {
        TString recoFile = TString::Format("%s_%d_reco.root", prefix.Data(), i);
        if (CheckFile(recoFile)) {fSrc->AddFriend(recoFile); cout << "recoFile: " <<  recoFile << endl;}
    }
    
    // Python TrackCands
    //fSrc->AddFriend(trkxFile);
    
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
    PndTrackImport *obj = new PndTrackImport(start_counter, inputdir);
    fRun->AddTask(obj);

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
