int eventDisplay(TString prefix = "./qa/mumu_110")
{
  //-----User Settings:-----------------------------------------------

  Bool_t enablePointDraw = kTRUE;
  Bool_t enableHitDraw = kTRUE;
  Bool_t enableTrackDraw = kTRUE;

  TString parAsciiFile = "all.par";

  TString output = "dummy";
  TString friend1 = "sim";
  TString friend2 = "digi";
  TString friend3 = "reco";
  TString friend4 = "mltrkx";

  // -----   Initial Settings   --------------------------------------------
  PndMasterRunAna *fRun = new PndMasterRunAna();
  fRun->SetOutput(output);
  fRun->AddFriend(friend1);
  fRun->AddFriend(friend2);
  fRun->AddFriend(friend3);
  fRun->AddFriend(friend4);
  fRun->SetParamAsciiFile(parAsciiFile);
  fRun->Setup(prefix);

  fRun->UseFairLinks(kTRUE);

  FairEventManager *fMan = new FairEventManager();

  PndSttRootGeometryInitTask *task = new PndSttRootGeometryInitTask();
  fRun->AddTask(task);

  //----------------------Traks and points -------------------------------------
  if (enablePointDraw) {
    FairGeoTracksDraw *Track = new FairGeoTracksDraw("Monte-Carlo Tracks");
    FairMCPointDraw *MvdPoints = new FairMCPointDraw("MVDPoint", kBlue, kFullSquare);
    FairMCPointDraw *PndSTTPoint = new FairMCPointDraw("STTPoint", kMagenta, kFullSquare);
    FairMCPointDraw *PndGEMPoint = new FairMCPointDraw("GEMPoint", kRed, kFullSquare);
    FairMCPointDraw *TofSciTPoint = new FairMCPointDraw("SciTPoint", kTeal, kFullSquare);
    FairMCPointDraw *PndDrcBarPoint = new FairMCPointDraw("DrcBarPoint", kGreen, kFullSquare);
    FairMCPointDraw *PndDrcPDPoint = new FairMCPointDraw("DrcPDPoint", kViolet, kFullSquare);
    FairMCPointDraw *PndDskParticle = new FairMCPointDraw("DskParticle", kYellow, kFullSquare);
    FairMCPointDraw *PndDskFLGHit = new FairMCPointDraw("PndDskFLGHit", kPink, kFullSquare);
    FairMCPointDraw *MdtPoint = new FairMCPointDraw("MdtPoint", kAzure, kFullSquare);
    PndEmcHitDraw *EMCPoints = new PndEmcHitDraw("EmcHit");						 //< displays individual EMC hits with their energy
    EMCPoints->SetBoxDimensions(2., 2., 2.);
    //PndEmcHitClusterDraw *EMCHitCluster = new PndEmcHitClusterDraw("EmcHit");  //< groups EMC hits into their clusters
    PndEmcHitCaloDraw *EmcHitCalo = new PndEmcHitCaloDraw("EmcHit");			 //< displays EMC hits as towers

    FairMCPointDraw *PndFTSPoint = new FairMCPointDraw("FTSPoint", kMagenta, kFullSquare);
    FairMCPointDraw *PndFtofPoint = new FairMCPointDraw("FtofPoint", kGreen, kFullSquare);

    fMan->AddTask(Track);
    fMan->AddTask(MvdPoints);
    fMan->AddTask(PndSTTPoint);
    fMan->AddTask(PndGEMPoint);
    fMan->AddTask(TofSciTPoint);
    fMan->AddTask(PndDrcBarPoint);
    fMan->AddTask(PndDrcPDPoint);
    fMan->AddTask(PndDskParticle);
    fMan->AddTask(PndDskFLGHit);
    fMan->AddTask(MdtPoint);
    fMan->AddTask(EMCPoints);
    //fMan->AddTask(EMCHitCluster);
    fMan->AddTask(EmcHitCalo);
    fMan->AddTask(PndFTSPoint);
    fMan->AddTask(PndFtofPoint);
  }

  //--------------- Hits ----------------------

  if (enableHitDraw) {
    FairHitDraw *MvdRecoHit = new FairHitDraw("MVDHitsPixel");
    FairHitDraw *MvdRecoStrip = new FairHitDraw("MVDHitsStrip");
    FairHitDraw *STTHits = new FairHitDraw("STTHit");				        //< draws the mid point of the fired STT tube
    PndSttIsochroneDraw *STTIsochrone = new PndSttIsochroneDraw("STTHit");  //< draws a tube with radius equal to the isochrone radius and fired tube parameters
    FairHitDraw *GEMHit = new FairHitDraw("GEMHit");
    FairHitDraw *SciTHit = new FairHitDraw("SciTHit");
    FairHitDraw *DrcHit = new FairHitDraw("DrcHit");
    FairHitDraw *DrcPDHit = new FairHitDraw("DrcPDHit");
    FairHitDraw *MdtHit = new FairHitDraw("MdtHit");

    PndEmcClusterDraw *EmcBump = new PndEmcClusterDraw("EmcBump");
    EmcBump->SetBoxDimensions(4.0, 4.0, 4.0);

    FairHitDraw *FTSHit = new FairHitDraw("FTSHit");
    FairHitDraw *FtofHit = new FairHitDraw("FtofHit");

    fMan->AddTask(MvdRecoHit);
    fMan->AddTask(MvdRecoStrip);
    fMan->AddTask(STTHits);
    fMan->AddTask(STTIsochrone);
    fMan->AddTask(GEMHit);
    fMan->AddTask(SciTHit);
    fMan->AddTask(DrcHit);
    fMan->AddTask(DrcPDHit);
    fMan->AddTask(MdtHit);
    fMan->AddTask(EmcBump);
    fMan->AddTask(FTSHit);
    fMan->AddTask(FtofHit);
  }

  if (enableTrackDraw) {
    PndTrackDraw *SttTrackDraw = new PndTrackDraw("PndTrackCand", kTRUE);
    
    
    PndTrackDraw *BarrelTrackDraw = new PndTrackDraw("BarrelTrack", kTRUE);
    PndTrackDraw *SttMvdTrack = new PndTrackDraw("BarrelGenTrack", kTRUE);
    PndTrackDraw *SttMvdGemTrack = new PndTrackDraw("SttMvdGemTrack", kTRUE);
    PndTrackDraw *SttMvdGemGenTrack = new PndTrackDraw("SttMvdGemGenTrack", kTRUE);
    PndTrackDraw *FtsIdealTrack = new PndTrackDraw("FtsIdealTrack");
    PndTrackDraw *FtsIdealGenTrack = new PndTrackDraw("FtsIdealGenTrack");
    
    fMan->AddTask(SttTrackDraw);
    fMan->AddTask(BarrelTrackDraw);
    fMan->AddTask(SttMvdTrack);
    fMan->AddTask(SttMvdGemTrack);
    fMan->AddTask(SttMvdGemGenTrack);
    fMan->AddTask(FtsIdealTrack);
    fMan->AddTask(FtsIdealGenTrack);
  }

  fMan->Init();

  TEveViewer* viewer = (TEveViewer*)(gEve->GetViewers()->FindChild("Viewer 1"));
  viewer->GetGLViewer()->CurrentCamera().SetCenterVec(0., 0., 0.);
  viewer->GetGLViewer()->CurrentCamera().SetExternalCenter(kTRUE);
  viewer->GetGLViewer()->SetDrawCameraCenter(kTRUE);

  return 0;
}
