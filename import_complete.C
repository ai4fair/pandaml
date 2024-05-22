int import_complete(Int_t nEvents=10, TString prefix="", TString inputdir="", Int_t start_counter=0) {
    
    std::cout << "\nFLAGS: " << nEvents << "," << prefix << "," << inputdir << "," << start_counter << std::endl;
    
    // ROOT Files
    TString parFile     = prefix+"_par.root";
    TString simFile     = prefix+"_sim.root";
    TString digiFile    = prefix+"_digi.root";
    TString skewFile    = prefix+"_skew.root";          // For Skewed Hit Correction
    TString recoFile    = prefix+"_recobarrel.root";    // Reco (TS+FS), RecoBarrel (TS)
    TString outFile     = prefix+"_mltrkx.root";        // TrackML to PndTrackCand

    // Initialization
    FairLogger::GetLogger()->SetLogToFile(kFALSE);
    FairRunAna *fRun = new FairRunAna();

    // Add Sim
    FairFileSource *fSrc = new FairFileSource(simFile);
    fRun->SetSource(fSrc);

    // Add Digi
    fSrc->AddFriend(digiFile);

    // Add Skew
    //fSrc->AddFriend(skewFile);

    // Add Reco
    fSrc->AddFriend(recoFile);
        
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
