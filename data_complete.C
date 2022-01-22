int data_complete(Int_t nEvents=10, TString prefix="evtcomplete") {
    
    // Files
    TString parFile     = prefix+"_par.root";
    TString simFile     = prefix+"_sim.root";
    TString digiFile    = prefix+"_digi.root";
    TString outFile     = "out.root";
    
    // Initialization
	FairLogger::GetLogger()->SetLogToFile(kFALSE);
	FairRunAna *fRun = new FairRunAna();
    
    // Add Input File to FairFileSource
    FairFileSource *fSrc = new FairFileSource(simFile);
	fRun->SetSource(fSrc);
	
	// Add Friend File to FairFileSource
	fSrc->AddFriend(digiFile);

    // Add Output File to FairRootFileSink
    FairRootFileSink *fSink = new FairRootFileSink(outFile);
    fRun->SetSink(fSink);
    
    
	// FairRuntimeDb
	FairRuntimeDb *rtdb = fRun->GetRuntimeDb();
	FairParRootFileIo *parInput1 = new FairParRootFileIo();
	parInput1->open(parFile.Data());
	
	
	// FairParAsciiFileIo
	FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
	
	//TString allDigiFile = TString(gSystem->Getenv("VMCWORKDIR"))+"/macro/params/all.par";	 // OR
	TString allDigiFile = gSystem->Getenv("VMCWORKDIR"); 
	allDigiFile += "/macro/params/all.par";
	
	parIo1->open(allDigiFile.Data(), "in");
	rtdb->setFirstInput(parInput1);
	rtdb->setSecondInput(parIo1);
	
	
	// HERE OUR TASK GOES!
    PndMLTracking *genDB = new PndMLTracking();
    fRun->AddTask(genDB);


    // FairRunAna::Init()
    PndEmcMapper::Init(1);
    fRun->Init();
    fRun->Run(0,nEvents);
    return 0;
}
