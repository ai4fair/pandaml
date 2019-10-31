
void eventDisplay() {

	//------------------- User Settings --------------------
	TString SimEngine	= "TGeant4"; 
	TString SimFile		= "evtcomplete_sim.root";
	TString DigiFile	= "evtcomplete_digi.root";
	//TString RecoFile	= "evtcomplete_reco.root";
	TString ParFile    	= "evtcomplete_par.root";

	//------------------- Reconstruction Run ---------------
	FairRunAna *fRun = new FairRunAna();
	fRun->SetInputFile(SimFile.Data());
	fRun->AddFriend(DigiFile.Data()); 
	//fRun->AddFriend(RecoFile.Data());
	fRun->SetOutputFile("display.root");
	
	//------------------- FairRuntimeDB --------------------
	FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
	FairParRootFileIo* parInput1 = new FairParRootFileIo();
	parInput1->open(ParFile.Data());
	rtdb->setFirstInput(parInput1);
	
	//------------------- FairEventManager -----------------
	FairEventManager *fMan= new FairEventManager();
	FairGeane *Geane = new FairGeane(); 
	fRun->AddTask(Geane);

	//------------------- MCTracks -------------------------
	FairMCTracks *Track=new FairMCTracks("Monte-Carlo Tracks");
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

	// ------------------------------ Add Tasks:
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
	
	
	// ------------------------------ Add Tasks:
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

	fMan->Init();
}
