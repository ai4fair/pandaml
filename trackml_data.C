int trackml_data (Int_t nEvents = 0) {

    /* A FairTask (PndMLTracking) to generate data from MCTracks */
    
    TString prefix = "evtcomplete";

    // Set input and output files
    TString simFile     = prefix+"_sim.root";
    TString parFile     = prefix+"_par.root";
    TString digiFile    = prefix+"_digi.root"; 				// provides STThit
    //TString recoFile  = prefix+"_recoideal.root"; 		// provides SttMvdGemIdealTrackCand
    TString outFile     = "patterns.root";
    TString digiParFile = "all.par";

    // Set up run
    FairRunAna *fRun = new FairRunAna();
    fRun->SetInputFile(simFile);
    fRun->AddFriend(digiFile);
    //fRun->AddFriend(recoFile);
    fRun->SetOutputFile(outFile);
    fRun->SetUseFairLinks(kTRUE);
    fRun->SetGenerateRunInfo(kFALSE);

    // -----  Parameter database   --------------------------------------------
    TString emcDigiFile = gSystem->Getenv("VMCWORKDIR");
    emcDigiFile += "/macro/params/";
    emcDigiFile += digiParFile;

    FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
    FairParRootFileIo* parInput1 = new FairParRootFileIo();
    parInput1->open(parFile.Data());

    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
    parIo1->open(emcDigiFile.Data(),"in");

    rtdb->setFirstInput(parInput1);
    rtdb->setSecondInput(parIo1);

    // starts the pattern counter, no specific options at the moment
    PndMLTracking *genDB = new PndMLTracking();
    //PndPatternDBGenerator *genDB = new PndPatternDBGenerator();
    fRun->AddTask(genDB);



    // -----   Intialise and run   --------------------------------------------
    PndEmcMapper::Init(1);
    cout << "fRun->Init()" << endl;
    fRun->Init();
    fRun->Run(0,nEvents);
    return 0;
}
