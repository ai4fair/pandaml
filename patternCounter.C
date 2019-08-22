int patternCounter() {

  Int_t nEvents = 0;

  TString inputFolder = "";

  // Set input and output files
  TString simFile = inputFolder+"sim.root";
  TString parFile = inputFolder+"par.root";
  TString digiFile = inputFolder+"digi.root"; // provides STThit
  TString recoFile = inputFolder+"recoideal.root"; // provides SttMvdGemIdealTrackCand
  TString outFile = "patterns.root";
  TString digiParFile = "all.par";

  // Set up run
  FairRunAna *fRun = new FairRunAna();
  fRun->SetInputFile(simFile);
  fRun->AddFriend(digiFile);
  fRun->AddFriend(recoFile);
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
  PatternCounter *pc = new PatternCounter();
  fRun->AddTask(pc);



  // -----   Intialise and run   --------------------------------------------
  PndEmcMapper::Init(1);
  cout << "fRun->Init()" << endl;
  fRun->Init();

  
  fRun->Run(0,nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------

  return 0;
}