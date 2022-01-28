
void eventDisplay(TString prefix="./data/evtcomplete") {

	//------------------- User Settings --------------------
	TString input        = "";
	TString parAsciiFile = "all.par";
    TString parFile      = prefix+"_par.root";
    TString simFile      = prefix+"_sim.root";
    TString digiFile     = prefix+"_digi.root";
    TString recoFile     = prefix+"_reco.root";
    TString outFile      = "display.root";
    
    
    // Initialization
	FairLogger::GetLogger()->SetLogToFile(kFALSE);
	FairRunAna *fRun = new FairRunAna();
    
    // Add I/O Files
    FairFileSource *fSrc = new FairFileSource(simFile);
	fRun->SetSource(fSrc);
	//fSrc->AddFriend(digiFile);
    //fSrc->AddFriend(recoFile);
    
    FairRootFileSink *fSink = new FairRootFileSink(outFile);
    fRun->SetSink(fSink);
    
	// FairRuntimeDb
	FairRuntimeDb *rtdb = fRun->GetRuntimeDb();
	FairParRootFileIo *parInput1 = new FairParRootFileIo();
	parInput1->open(parFile.Data());
	rtdb->setFirstInput(parInput1);
	
	// FairParAsciiFileIo
	FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
	parAsciiFile = TString(gSystem->Getenv("VMCWORKDIR"))+"/macro/params/all.par";
	parIo1->open(parAsciiFile.Data(), "in");
	rtdb->setSecondInput(parIo1);
	
	
	// OR, Using MasterRunAna
	/*
	PndMasterRunAna *fRun= new PndMasterRunAna();
	fRun->SetInput(input);
	fRun->AddFriend(simFile);
	fRun->AddFriend(digiFile);
	fRun->SetOutput(outFile);
	fRun->SetParamAsciiFile(parAsciiFile);
	fRun->Setup(prefix);
	fRun->SetUseFairLinks(kTRUE);
	*/
	
	// FairEventManager
	FairEventManager *fMan= new FairEventManager();
	FairGeane *Geane = new FairGeane(); 
	fRun->AddTask(Geane);

	// MCTracks
	FairMCTracks *Track = new FairMCTracks();
	fMan->AddTask(Track);
	
	// ------------------------------------------------------------------------
	//                               MCPoints
	// ------------------------------------------------------------------------
	
	//FairMCPointDraw *MvdPoints 	  = new FairMCPointDraw ("MVDPoint", kBlue, kFullSquare);
	//FairHitDraw *EMCPoints 		  = new FairHitDraw ("EmcHit");
	//PndEmcHitDraw *EMCPoints2		  = new PndEmcHitDraw ("EmcHit");
	//EMCPoints->SetBoxDimensions(2.,2.,2.);
	//EMCPoints2->SetBoxDimensions(2.,2.,2.);
	//FairMCPointDraw *TofSciTPoint	  = new FairMCPointDraw ("SciTPoint",kTeal,kFullSquare);
	//FairMCPointDraw *MdtPoint		  = new FairMCPointDraw ("MdtPoint",kAzure,kFullSquare);
	//FairMCPointDraw *PndDrcBarPoint = new FairMCPointDraw ("DrcBarPoint",kGreen,kFullSquare);
	//FairMCPointDraw *PndDrcPDPoint  = new FairMCPointDraw ("DrcPDPoint",kViolet,kFullSquare);
	//FairMCPointDraw *PndDskParticle = new FairMCPointDraw ("DskParticle",kYellow,kFullSquare);
	//FairMCPointDraw *PndDskFLGHit   = new FairMCPointDraw ("PndDskFLGHit",kPink,kFullSquare);
	FairMCPointDraw *PndSTTPoint 	  = new FairMCPointDraw ("STTPoint",kMagenta,kFullSquare);
	//FairMCPointDraw *PndGEMPoint 	  = new FairMCPointDraw ("GEMPoint",kRed,kFullSquare);
	//FairMCPointDraw *PndFTSPoint 	  = new FairMCPointDraw ("FTSPoint",kMagenta,kFullSquare);
	//FairMCPointDraw *PndFtofPoint   = new FairMCPointDraw ("FtofPoint",kGreen,kFullSquare);
	//PndEmcHitCaloDraw *EmcHitCalo   = new PndEmcHitCaloDraw ("EmcHit");

	//----- Add Tasks:
	//fMan->AddTask (MvdPoints);
	//fMan->AddTask (EMCPoints);
	//fMan->AddTask (EMCPoints2);
	//fMan->AddTask (TofSciTPoint);
	//fMan->AddTask (MdtPoint);
	//fMan->AddTask (PndDrcBarPoint);
	//fMan->AddTask (PndDrcPDPoint);
	//fMan->AddTask (PndDskParticle);
	//fMan->AddTask (PndDskFLGHit);
	fMan->AddTask (PndSTTPoint);
	//fMan->AddTask (PndGEMPoint);
	//fMan->AddTask (PndFTSPoint);
	//fMan->AddTask (PndFtofPoint);
	//fMan->AddTask (EmcHitCalo);

	
	// ------------------------------------------------------------------------
	//                               Hits
	// ------------------------------------------------------------------------
	
	//FairHitDraw *MvdRecoHit = new FairHitDraw("MVDHitsPixel");
	//FairHitDraw *MvdRecoStrip = new FairHitDraw("MVDHitsStrip");
	FairHitDraw *STTHits = new FairHitDraw("STTHit");
	PndSttIsochroneDraw* STTIsochrone = new PndSttIsochroneDraw("STTHit");
	//STTIsochrone->UseIsochroneTime();
	//FairHitDraw *SciTHit = new FairHitDraw("SciTHit");
	//FairHitDraw *MdtHit	= new FairHitDraw("MdtHit");
	//FairHitDraw *DrcHit = new FairHitDraw("DrcHit");
	//FairHitDraw *DrcPDHit = new FairHitDraw("DrcPDHit");
	//FairHitDraw *GEMHit = new FairHitDraw("GEMHit");
	//FairHitDraw *FTSHit = new FairHitDraw("FTSHit");
	//FairHitDraw *FtofHit = new FairHitDraw("FtofHit");
	//PndEmcClusterDraw *EmcBump = new PndEmcClusterDraw("EmcBump");
	//EmcBump->SetBoxDimensions(4.0,4.0,4.0);
	
	
	//------ Add Tasks:
	//fMan->AddTask(MvdRecoHit);
	//fMan->AddTask(MvdRecoStrip);
	fMan->AddTask(STTHits);
	fMan->AddTask(STTIsochrone);
	//fMan->AddTask(SciTHit);
	//fMan->AddTask(MdtHit);
	//fMan->AddTask(DrcHit);
	//fMan->AddTask(DrcPDHit);
	//fMan->AddTask(GEMHit);
	//fMan->AddTask(FTSHit);
	//fMan->AddTask(FtofHit);
	//fMan->AddTask(EmcBump);
	

	// ------------------------------------------------------------------------
	//                               Tracks
	// ------------------------------------------------------------------------
	
	//PndTrackDraw* SttMvdTrack = new PndTrackDraw("SttMvdTrack");
	//PndTrackDraw* SttMvdGemTrack = new PndTrackDraw("SttMvdGemTrack", kTRUE);
	//PndTrackDraw* FtsIdealTrack = new PndTrackDraw("FtsIdealTrack");
	//PndTrackDraw* SttMvdGemGenTrack = new PndTrackDraw("SttMvdGemGenTrack", kTRUE);


	// ------------------------------ Add Tasks:
	//fMan->AddTask(SttMvdTrack);
	//fMan->AddTask(SttMvdGemTrack);
	//fMan->AddTask(FtsIdealTrack);
	//fMan->AddTask(SttMvdGemGenTrack);
	
	
    fRun->Init();
	fMan->Init();
}
