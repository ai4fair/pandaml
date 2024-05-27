bool CheckFile(TString fn);


// Globals
PndGeoHandling* fGeoH;
std::vector<std::vector<int>> fLayers; ///< contains layer information of hits
std::map<TString, int> fLayerMap;      ///< identifier string, assigned layer id
int fLastLayerId=0;                      ///< last layer Id assigned

int GetLayer(TString identifier);
int GetLayerGem(FairHit *hit);
int GetLayerMvd(FairHit *hit);
void InitLayerMap();
void InitLayerMapMvd();
void InitLayerMapGem();

void mvdHits(Int_t nEvents = 1, TString prefix = "data/llbar") {

    TString parAsciiFile = "all.par";
    TString input        = "";   // name of input, ignored if `prefix != NULL`
    TString friend1      = "sim";
    TString friend2      = "digi";
    TString friend3      = "reco";
    TString friend4      = "";
    TString output       = "out";                // name of output: `prefix_output.root`

    // -----   Initial Settings   --------------------------------------------
    PndMasterRunAna *fRun = new PndMasterRunAna();
    fRun->SetInput(input);
    fRun->SetOutput(output);
    fRun->AddFriend(friend1);
    fRun->AddFriend(friend2);
    fRun->AddFriend(friend3);
    fRun->AddFriend(friend4);
    fRun->SetParamAsciiFile(parAsciiFile);
    fRun->UseFairLinks(kTRUE);
    fRun->Setup(prefix);                         // Setup Everything (w/ set functions above)
    fRun->Init();                                // Initialize Run Manager


    // MY Custom Code (Everything Outside of a FairTask)
    FairRootManager* ioman = FairRootManager::Instance();
    
    Int_t ev_entries = nEvents;
    TFile *out = TFile::Open(prefix+"_mvdHits.root", "RECREATE");
    
    TClonesArray* fMCTrackArray = (TClonesArray*) ioman->GetObject("MCTrack");
    //TClonesArray* fBarrelTrackArray = (TClonesArray*) ioman->GetObject("BarrelTrack"); // formerly SttMvdGemTrack
    
    TClonesArray* fMvdPointArray = (TClonesArray*) ioman->GetObject("MVDPoint");
    int mvdPointBranchID = ioman->GetBranchId("MVDPoint");
    
    TClonesArray* fMvdHitsPixelArray = (TClonesArray*) ioman->GetObject("MVDHitsPixel");
    TClonesArray* fMvdHitsStripArray = (TClonesArray*) ioman->GetObject("MVDHitsStrip");
    
    TClonesArray* fGemPointArray = (TClonesArray*) ioman->GetObject("GEMPoint");
    TClonesArray* fGemHitArray = (TClonesArray*) ioman->GetObject("GEMHit");
    
    TClonesArray* fSttPointArray = (TClonesArray*) ioman->GetObject("STTPoint");
    TClonesArray* fSttHitArray = (TClonesArray*) ioman->GetObject("STTHit");
    
    PndMCTrack *mcTrack;
    FairMultiLinkedData linksMVDPixel, linksMVDStrip, linksGEM, linksSTT;
    
    // Getting Layer_Id of MVD, GEM
    fGeoH = PndGeoHandling::Instance();
    //std::vector<std::vector<int>> fLayers; ///< contains layer information of hits
    //std::map<TString, int> fLayerMap;      ///< identifier string, assigned layer id
    //int fLastLayerId=0;                      ///< last layer Id assigned
    
    // Init Layer Map
    InitLayerMap();
    
    // Event Loop
    for (Int_t event=0; event < ev_entries; ++event) {
	    ioman->ReadEvent(event); std::cout << "Processing the Event: "<< event << std::endl;
	    
	    // Loop over MVDHitsPixel
	    std::cout << "Size of fMvdHitsPixelArray: " << fMvdHitsPixelArray->GetEntries() << std::endl;        
        int hit_counter = 0;
        for (Int_t idx = 0; idx < fMvdHitsPixelArray->GetEntries(); idx++) {
            
            hit_counter++;
            
            
            
            // Write to xxx-hits.csv (DONE !!!)
            // ---------------------------------------------------------------------------
            PndSdsHit* sdsHit = (PndSdsHit*)fMvdHitsPixelArray->At(idx);
                        
            /*
            cout << "SDS Hits" << endl;
            cout  << hit_counter            << ","      // hit_id
                  << sdsHit->GetX()         << ","      // x-position
                  << sdsHit->GetY()         << ","      // y-position
                  << sdsHit->GetZ()         << ","      // z-position
                  << sdsHit->GetDetectorID()<< ","      // volume_id
                  << GetLayerMvd(sdsHit) << ","         // layer_id
                  << sdsHit->GetSensorID()              // sensor_id/module_id
                  << std::endl;
            */
            
            
            
            
            // Write to xxx-truth.csv
            // ---------------------------------------------------------------------------
            
            // Get sttHitsLinks from STTHit
            FairMultiLinkedData_Interface *sdsHitsLinks = (FairMultiLinkedData_Interface*)fMvdHitsPixelArray->At(idx);

            // Get sttPointLinks & Data (TCloneArray) from sttHitsLinks
            FairMultiLinkedData sdsPointLinks = sdsHitsLinks->GetLinksWithType(mvdPointBranchID);
            FairMCPoint *sdsPoint = (FairMCPoint*)ioman->GetCloneOfLinkData(sdsPointLinks.GetLink(0));
            
            // Get Particle Id
            // Get MCTrack ID associated with each hit_idx (fSttHitArray)
            std::vector<FairLink> mcTrackLinks_sorted = sdsHitsLinks->GetSortedMCTracks();
            TString particle_id = "";
            
            for (unsigned int trackIndex = 0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
                
                // Append "particle_id" to xxx-truth.csv
                particle_id = std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);
            }
            
            /*
            cout << "SDS Truth" << endl;
            cout  << hit_counter       << ","   // hit_id  
                    << sdsPoint->GetX()  << ","   // tx = true x
                    << sdsPoint->GetY()  << ","   // ty = true y
                    << sdsPoint->GetZ()  << ","   // tz = true z
                    << sdsPoint->GetPx() << ","   // tpx = true px
                    << sdsPoint->GetPy() << ","   // tpy = true py
                    << sdsPoint->GetPz() << ","   // tpz = true pz
                    << (1.0)             << ",";  // FIXME: Weight Placeholder, but sum of weights of hits in track == 1)
                    << particle_id                // Particle_id from above   
                    << std::endl;    
            */
            
            
            

            
            // Write to xxx-mvd_cells.csv
            // ---------------------------------------------------------------------------
            
            cout << "Writing MVD Cells" << endl;
            cout    << hit_counter             << ","   // hit_id
                    << sdsHit->GetCharge()     << ","   // deposited charge 
                    << sdsHit->GetDetectorID() << ","   // volume_id
                    << GetLayerMvd(sdsHit)     << ","   // layer_id
                    << sdsHit->GetSensorID()   << ","   // tube_id/module_id
                    << std::endl;
           
           
           
            // Write to xxx-particles.csv
            // ---------------------------------------------------------------------------
            for (unsigned int trackIndex=0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
            
                // Fetch mcTrack associated with SttHit
                PndMCTrack *mcTrack = (PndMCTrack*)ioman->GetCloneOfLinkData(mcTrackLinks_sorted[trackIndex]);
                
                // std::cout << "No. of MCPoints in STT: " << mcTrack->GetNPoints(DetectorID::kSTT) << std::endl;
                
                // CSV:: Writting Info to CSV File. 
                cout  << std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1)   
                                                                       << ","   // track_id > 0
                            << (mcTrack->GetStartVertex()).X()         << ","   // vx = start x
                            << (mcTrack->GetStartVertex()).Y()         << ","   // vy = start y
                            << (mcTrack->GetStartVertex()).Z()         << ","   // vz = start z
                            << (mcTrack->GetMomentum()).X()            << ","   // px = x-component of track momentum
                            << (mcTrack->GetMomentum()).Y()            << ","   // py = y-component of track momentum
                            << (mcTrack->GetMomentum()).Z()            << ","   // pz = z-component of track momentum
                            << ((mcTrack->GetPdgCode()>0)?1:-1)        << ","   // q = charge of mu-/mu+
                            // <<mcTrack->GetNPoints(DetectorId::kSTT) << ","   // FIXME: nhits in STT (Not tested yet).
                            << 1                                       << ","   // FIXME: nhits==1 (just a placeholder, will count in python)
                            << mcTrack->GetPdgCode()                   << ","   // pdgcode e.g. mu- has pdgcode=-13
                            << mcTrack->GetStartTime()                 << ","   // start_time = start time of particle track
                            << mcTrack->IsGeneratorCreated()                    // flag 'primary' particle
                            << std::endl;
                  
                delete (mcTrack);
            }       
           
	    }//fMvdHitsPixelArray
	    
	    
	    // -------------------- Loop over MVDHitsStrips
	    std::cout << "Size of fMvdHitsStripArray: " << fMvdHitsStripArray->GetEntries() << std::endl;
	    
        hit_counter = 0;
        for (Int_t idx = 0; idx < fMvdHitsStripArray->GetEntries(); idx++) {
        
            // TODO: Impelment Same as above
            cout << "MvdHitsStripArray: Will be same as above" << endl;
            
            hit_counter++;
            PndSdsHit* sdsHit = (PndSdsHit*)fMvdHitsStripArray->At(idx);
            //sdsHit->Print();
            
            /*
            cout << "SDS Hits" << endl;
            cout  << hit_counter            << ","      // hit_id
                  << sdsHit->GetX()         << ","      // x-position
                  << sdsHit->GetY()         << ","      // y-position
                  << sdsHit->GetZ()         << ","      // z-position
                  << sdsHit->GetDetectorID()<< ","      // volume_id
                  << GetLayerMvd(sdsHit) << ","         // layer_id
                  << sdsHit->GetSensorID()              // sensor_id/module_id
                  << std::endl;
            */
            
            
            
            
            // Write to xxx-truth.csv
            // ---------------------------------------------------------------------------
            
            // Get sttHitsLinks from STTHit
            FairMultiLinkedData_Interface *sdsHitsLinks = (FairMultiLinkedData_Interface*)fMvdHitsStripArray->At(idx);

            // Get sttPointLinks & Data (TCloneArray) from sttHitsLinks
            FairMultiLinkedData sdsPointLinks = sdsHitsLinks->GetLinksWithType(mvdPointBranchID);
            FairMCPoint *sdsPoint = (FairMCPoint*)ioman->GetCloneOfLinkData(sdsPointLinks.GetLink(0));
            
            // Get Particle Id
            // Get MCTrack ID associated with each hit_idx (fSttHitArray)
            std::vector<FairLink> mcTrackLinks_sorted = sdsHitsLinks->GetSortedMCTracks();
            TString particle_id = "";
            
            for (unsigned int trackIndex = 0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
                
                // Append "particle_id" to xxx-truth.csv
                particle_id = std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);
            }
            
            /*
            cout << "SDS Truth" << endl;
            cout  << hit_counter       << ","   // hit_id  
                    << sdsPoint->GetX()  << ","   // tx = true x
                    << sdsPoint->GetY()  << ","   // ty = true y
                    << sdsPoint->GetZ()  << ","   // tz = true z
                    << sdsPoint->GetPx() << ","   // tpx = true px
                    << sdsPoint->GetPy() << ","   // tpy = true py
                    << sdsPoint->GetPz() << ","   // tpz = true pz
                    << (1.0)             << ",";  // FIXME: Weight Placeholder, but sum of weights of hits in track == 1)
                    << particle_id                // Particle_id from above   
                    << std::endl;    
            */
            
            
            

            
            // Write to xxx-mvd_cells.csv
            // ---------------------------------------------------------------------------
            
            cout << "Writing MVD Cells" << endl;
            cout    << hit_counter             << ","   // hit_id
                    << sdsHit->GetCharge()     << ","   // deposited charge 
                    << sdsHit->GetDetectorID() << ","   // volume_id
                    << GetLayerMvd(sdsHit)     << ","   // layer_id
                    << sdsHit->GetSensorID()   << ","   // tube_id/module_id
                    << std::endl;
        }//fMvdHitsStripArray
        
        
    }//end-Event

}

bool CheckFile(TString fn) {
    /* Check if a file is broken.*/
    bool fileok=true;
    TFile fff(fn); 
    if (fff.IsZombie()) fileok=false;

    TTree *t=(TTree*)fff.Get("pndsim");
    if (t==0x0) fileok=false;

    if (!fileok) std::cout << "Skipping Broken File: '"<< fn << "'" << std::endl;
    return fileok;
}



int GetLayer(TString identifier)
{
  std::map<TString, int>::iterator layerIter;
  for (layerIter = fLayerMap.begin(); layerIter != fLayerMap.end(); layerIter++) {
    if (identifier.Contains(layerIter->first)) {
      return layerIter->second;
    }
  }
  return 0;
}

int GetLayerGem(FairHit *hit)
{
  PndGemHit *gemHit = (PndGemHit *)hit;

  TString prefix("Gem_");
  prefix += (gemHit->GetStationNr());
  prefix += ("_");
  prefix += (gemHit->GetSensorNr());

  return GetLayer(prefix);
}

int GetLayerMvd(FairHit *hit)
{
  PndSdsHit *tempHit = (PndSdsHit *)(hit);
  TString geoPath = fGeoH->GetPath(tempHit->GetSensorID());

  return GetLayer(geoPath);
}


void InitLayerMap()
{
  InitLayerMapMvd();
  InitLayerMapGem();
}

void InitLayerMapMvd()
{
  fLayerMap["PixeloBlo1"] = fLastLayerId + 1;
  fLayerMap["PixeloSdko(Silicon)_1"] = fLastLayerId + 2; // naming after MVD2.2
  fLayerMap["PixeloSdkoco(Silicon)_1"] = fLastLayerId + 2;
  fLayerMap["PixeloSdko(Silicon)_2"] = fLastLayerId + 3; // naming after MVD2.2
  fLayerMap["PixeloSdkoco(Silicon)_2"] = fLastLayerId + 3;
  fLayerMap["PixeloSdko(Silicon)_3"] = fLastLayerId + 4; // naming after MVD2.2
  fLayerMap["PixeloSdkoco(Silicon)_3"] = fLastLayerId + 4;
  fLayerMap["PixeloSdko(Silicon)_4"] = fLastLayerId + 5; // naming after MVD2.2
  fLayerMap["PixeloSdkoco(Silicon)_4"] = fLastLayerId + 5;
  fLayerMap["PixeloBlo2"] = fLastLayerId + 6;
  fLayerMap["PixeloLdkoio(Silicon)_1"] = fLastLayerId + 7; // naming after MVD2.2
  fLayerMap["PixeloLdkoco(Silicon)_1"] = fLastLayerId + 7;
  fLayerMap["PixeloLdkoiio(Silicon)_1"] = fLastLayerId + 8; // naming after MVD2.2
  fLayerMap["PixeloLdkoco(Silicon)_2"] = fLastLayerId + 8;
  fLayerMap["PixeloLdkoiiio(Silicon)_1"] = fLastLayerId + 9; // naming after MVD2.2
  fLayerMap["PixeloLdkoco(Silicon)_3"] = fLastLayerId + 9;
  fLayerMap["PixeloLdkoiiio(Silicon)_2"] = fLastLayerId + 10; // naming after MVD2.2
  fLayerMap["PixeloLdkoco(Silicon)_4"] = fLastLayerId + 10;
  fLayerMap["PixeloLdkoiio(Silicon)_2"] = fLastLayerId + 11; // naming after MVD2.2
  fLayerMap["PixeloLdkoco(Silicon)_5"] = fLastLayerId + 11;
  fLayerMap["PixeloLdkoio(Silicon)_2"] = fLastLayerId + 12; // naming after MVD2.2
  fLayerMap["PixeloLdkoco(Silicon)_6"] = fLastLayerId + 12;
  fLayerMap["PixeloLdkoiiio(Silicon)_3"] = fLastLayerId + 13; // naming after MVD2.2
  fLayerMap["PixeloLdkoco(Silicon)_7"] = fLastLayerId + 13;
  fLayerMap["PixeloLdkoiiio(Silicon)_4"] = fLastLayerId + 14; // naming after MVD2.2
  fLayerMap["PixeloLdkoco(Silicon)_8"] = fLastLayerId + 14;

  fLayerMap["StripoBl3o(Silicon)"] = fLastLayerId + 15;
  fLayerMap["Fwdo(Silicon)_1"] = fLastLayerId + 16;
  fLayerMap["StripoBl4o(Silicon)"] = fLastLayerId + 17;
  // fLayerMap["Fwdo(Silicon)_2")){Layer=10;flag=false;}

  fLayerMap["StripoLdkoTrapSoRingAoSilicon_1"] = fLastLayerId + 18;
  fLayerMap["StripoLdkoTrapSoRingAoSilicon_2"] = fLastLayerId + 19;
  fLayerMap["StripoLdkoTrapSoRingBoSilicon_1"] = fLastLayerId + 18;
  fLayerMap["StripoLdkoTrapSoRingBoSilicon_2"] = fLastLayerId + 19;
  fLayerMap["StripoLdko5-6oTrapSo(Silicon)_1"] = fLastLayerId + 20;

  fLayerMap["LambdaDisk_1"] = fLastLayerId + 21;
  fLayerMap["LambdaDisk_2"] = fLastLayerId + 22;
  fLastLayerId += 22;
}

void InitLayerMapGem()
{
  fLayerMap["Gem_1_1"] = fLastLayerId + 1;
  fLayerMap["Gem_1_2"] = fLastLayerId + 2;

  fLayerMap["Gem_2_1"] = fLastLayerId + 3;
  fLayerMap["Gem_2_2"] = fLastLayerId + 4;

  fLayerMap["Gem_3_1"] = fLastLayerId + 5;
  fLayerMap["Gem_3_2"] = fLastLayerId + 6;

  fLastLayerId += 6;
}








