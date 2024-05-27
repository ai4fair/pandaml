bool CheckFile(TString fn);
void mcTrackInfo(Int_t nEvents = 1, TString prefix = "data/mumu") {

    TString parAsciiFile = "all.par";
    TString input        = "Xibar_Xi1820.dec";   // name of input, ignored if `prefix != NULL`
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
    TFile *out = TFile::Open(prefix+"_mcTrackInfo.root", "RECREATE");
    
    TClonesArray* fMCTrackArray = (TClonesArray*) ioman->GetObject("MCTrack");
    TClonesArray* fBarrelTrackArray = (TClonesArray*) ioman->GetObject("BarrelTrack"); // formerly SttMvdGemTrack
    
    TClonesArray* fMvdHitsPixelArray = (TClonesArray*) ioman->GetObject("MvdHitsPixel");
    TClonesArray* fMvdHitsStripArray = (TClonesArray*) ioman->GetObject("MvdHitsStrip");
    TClonesArray* fGemHitArray = (TClonesArray*) ioman->GetObject("GEMHit");
    TClonesArray* fSttPointArray = (TClonesArray*) ioman->GetObject("STTPoint");
    TClonesArray* fSttHitArray = (TClonesArray*) ioman->GetObject("STTHit");
    
    FairMultiLinkedData links;
    FairMultiLinkedData linksMVDPixel, linksMVDStrip, linksGEM, linksSTT;
    PndMCTrack *mcTrack;
    
    std::cout << std::endl;
    std::cout << "Sample Name  : " << prefix << std::endl;
    std::cout << "No. of Events: " << nEvents << std::endl;
    std::cout << std::endl;
    
    // MCPoints in MVD, GEM and STT
    Int_t TrackCounter = 0;
    Int_t MVDPoints = 0, GEMPoints =0, STTPoints = 0;
    Int_t fMVDPoints = 0, fGEMPoints = 0, fSTTPoints = 0, fTotalPoints = 0;
    
    // Event Loop
    for (Int_t event=0; event < ev_entries; ++event) {
	    ioman->ReadEvent(event); std::cout << "Processing the Event: "<< event << std::endl;
	    
        // ----------------- Start MCTrackArray Loop
        std::cout << "MCTracks per Event: " << fMCTrackArray->GetEntries() << std::endl;
        for (Int_t mc = 0; mc < fMCTrackArray->GetEntries(); mc++) {
        
			PndMCTrack *mcTrack = (PndMCTrack*)fMCTrackArray->At(mc);
			
			if (mcTrack->IsGeneratorCreated()) { 
			    
			    TrackCounter++;
                MVDPoints += mcTrack->GetNPoints(DetectorId::kMVD);
                GEMPoints += mcTrack->GetNPoints(DetectorId::kGEM);
                STTPoints += mcTrack->GetNPoints(DetectorId::kSTT);
            }
            
		}//endfor-mc
        // ----------------- End MCTrackArray Loop
                
        //std::cout << "- EvtGen: No. of MCTracks: " << TrackCounter << std::endl;
        //std::cout << "- EvtGen: Total MVDPoints: " << MVDPoints << std::endl;
        //std::cout << "- EvtGen: Total GEMPoints: " << GEMPoints << std::endl;
        //std::cout << "- EvtGen: Total STTPoints: " << STTPoints << std::endl;
        
        
        
        
        
        
        
        
        // ----------------- Start BarrelTrackArray Loop
        std::cout << "BarrelTracks per Event: " << fBarrelTrackArray->GetEntries() << std::endl;
        
        for (Int_t idx = 0; idx < fBarrelTrackArray->GetEntries(); idx++) {
            
            // Fetch a PndTrack from the fBarrelTrackArray
            PndTrack *barrelTrack = (PndTrack *)fBarrelTrackArray->At(idx);
            
            // Connect BarrelTrack to its MCTrack
            links = barrelTrack->GetLinksWithType(ioman->GetBranchId("MCTrack"));
            
            // Connect BarrelTrack to its MCTrack
            fMVDPoints = barrelTrack->GetLinksWithType(ioman->GetBranchId("MVDPoint")).GetNLinks(); // mind the branch name
            fGEMPoints = barrelTrack->GetLinksWithType(ioman->GetBranchId("GEMPoint")).GetNLinks(); // mind the branch name
            fSTTPoints = barrelTrack->GetLinksWithType(ioman->GetBranchId("STTPoint")).GetNLinks(); // mind the branch name
            fTotalPoints = fMVDPoints + fSTTPoints + fGEMPoints;
            
            std::cout << "\nlinkMC: " << links.GetNLinks() << std::endl;
            std::cout << "\nMVDPoints: " << fMVDPoints << std::endl;
            std::cout << "GEMPoints: " << fGEMPoints << std::endl;
            std::cout << "STTPoints: " << fSTTPoints << std::endl;
            //std::cout << "TotalPoints: " << fTotalPoints << std::endl;
            
            fMVDPoints = 0, fGEMPoints = 0, fSTTPoints = 0, fTotalPoints = 0;
            
            // Here, links.GetNLinks()==1 always. So if-for-if always runs, then why we have these statements.
            if (links.GetNLinks()>0) {
                for (Int_t i=0; i<links.GetNLinks(); i++) {
                    if (links.GetLink(i).GetIndex()==barrelTrack->GetTrackCand().getMcTrackId()) {

                        // Here I am getting MCTrack Data (Array) which is associated with BarrelTrack. 
                        // So, in other words, I am getting MCTruth of the a particular BarrelTrack.
                        
                        PndMCTrack *mcTrack = (PndMCTrack *)ioman->GetCloneOfLinkData(links.GetLink(i));
                        
                        fMVDPoints = mcTrack->GetNPoints(DetectorId::kMVD);
                        fGEMPoints = mcTrack->GetNPoints(DetectorId::kGEM);
                        fSTTPoints = mcTrack->GetNPoints(DetectorId::kSTT);
                        fTotalPoints = fMVDPoints + fSTTPoints + fGEMPoints;
                        
                        
                        //std::cout << "\nMVDPoints: " << fMVDPoints << std::endl;
                        //std::cout << "GEMPoints: " << fGEMPoints << std::endl;
                        //std::cout << "STTPoints: " << fSTTPoints << std::endl;
                        //std::cout << "TotalPoints: " << fTotalPoints << std::endl;
                        
                        
                        linksMVDPixel = barrelTrack->GetLinksWithType(ioman->GetBranchId("MVDPoints"));
                        linksMVDStrip = barrelTrack->GetLinksWithType(ioman->GetBranchId("MvdHitsStrip"));
                        linksGEM = barrelTrack->GetLinksWithType(ioman->GetBranchId("GEMHit"));
                        linksSTT = barrelTrack->GetLinksWithType(ioman->GetBranchId("STTHit"));
                        
                        //std::cout << "\nMVDHits: " << linksMVDPixel.GetNLinks() << std::endl;
                        //std::cout << "GEMHits: " << linksGEM.GetNLinks() << std::endl;
                        //std::cout << "STTHits: " << linksSTT.GetNLinks() << std::endl;
                        //std::cout << "TotalHits: " << fTotalPoints << std::endl;
                        
                        
                        //std::cout << "--- mcTrack Index: " << idx << mcTrack->GetPdgCode() << std::endl;
                        //std::cout << "--- mcTrack Code : " << idx << mcTrack->GetPdgCode() << std::endl;
                        
                        //std::cout << "--- IsGeneratorCreated : " << mcTrack->IsGeneratorCreated() << std::endl;
                        //std::cout << "--- IsGeneratorDecayed : " << mcTrack->IsGeneratorDecayed() << std::endl;
                        //std::cout << "--- IsGeneratorLast    : " << mcTrack->IsGeneratorLast() << std::endl;
                        //std::cout << std::endl;
                        
                        
                        
                        /*
                        // Get Only Primary Tracks (Skip Secondaries e.g. e+/e-)
                        if (mcTrack->IsGeneratorCreated()) {  
                            
                            std::cout << "IsGeneratorCreated Ideal Track : " << idx << " with PdgCode: " << mcTrack->GetPdgCode() << std::endl;
                            
                        
                            // Check the number of STT hits
                            linksSTT = barrelTrack->GetLinksWithType(ioman->GetBranchId("STTHit"));
                            
                            // If the number of STT hits greater than 0, write MC track to file!!

                            // CSV:: Writting Info to CSV File.
                            std::cout   << (std::to_string(links.GetLink(i).GetIndex() + 1)) << "," // track_id > 0
                                        << (mcTrack->GetStartVertex()).X() << ","   // vx = start x [cm, ns]
                                        << (mcTrack->GetStartVertex()).Y() << ","   // vy = start y [cm, ns]
                                        << (mcTrack->GetStartVertex()).Z() << ","   // vz = start z [cm, ns]
                                        << (mcTrack->GetMomentum()).X()    << ","   // px = x-component of track momentum
                                        << (mcTrack->GetMomentum()).Y()    << ","   // py = y-component of track momentum
                                        << (mcTrack->GetMomentum()).Z()    << ","   // pz = z-component of track momentum
                                        << ((mcTrack->GetPdgCode()>0)?1:-1)<< ","   // q = charge of mu-/mu+
                                        << (linksSTT.GetNLinks())          << ","   // nhits
                                        << mcTrack->GetPdgCode()           << ","   // pdgcode e.g. mu- has pdgcode=-13
                                        << mcTrack->GetStartTime()                  // start_time = starting time of particle track
                                        << std::endl;
                                        
                        }//end-IsGeneratorCreated()
                        
                        */
                        
                        
                        
                        
                    }//end-if(GetLink(i))
                }//end-for(GetNLinks)
            }//end-if(GetNLinks)
        }//end-for (barrelTrack)
        
        
        // ----------------- Start BarrelTrackArray Loop
	    
	    
	    
    }//Event Loop

    // Finish Up (Final Diagnostics)
    // std::cout << "\n...Explicitly Trigger Finish()...\n" << std::endl;
    //fRun->Finish();

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















