// Build a TTree to Replace CSV Data Files. Idea is to save the
// fParticles, fTruth, fHits and fCells as Branches in the TTree
// Then use UpRoot to read and manupilate data in Python especially
// during the Processing Stage in the Deep Learning Pipeline.



#include "Particles.h"

bool CheckFile(TString fn);


class Particle2 {

public:
    Double32_t fPx, fPy, fPz, fE;
    Double32_t fStartX, fStartY, fStartZ, fStartT;
    
    Int_t fParticleId;
    Int_t fQ;
    Int_t fNHits;
    Int_t fPdgCode;
    Bool_t fIsGenCreated;
};



void ttree(Int_t nEvents = 1, TString prefix = "data/mumu") {

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
    
    TClonesArray* fMvdPointArray = (TClonesArray*) ioman->GetObject("MVDPoint");
    TClonesArray* fMvdHitsPixelArray = (TClonesArray*) ioman->GetObject("MvdHitsPixel");
    TClonesArray* fMvdHitsStripArray = (TClonesArray*) ioman->GetObject("MvdHitsStrip");
    
    TClonesArray* fGemPointArray = (TClonesArray*) ioman->GetObject("GEMPoint");
    TClonesArray* fGemHitArray = (TClonesArray*) ioman->GetObject("GEMHit");
    
    TClonesArray* fSttPointArray = (TClonesArray*) ioman->GetObject("STTPoint");
    TClonesArray* fSttHitArray = (TClonesArray*) ioman->GetObject("STTHit");

    FairMultiLinkedData linksMC;
    FairMultiLinkedData linksMVDPixel, linksMVDStrip, linksGEM, linksSTT;
    
    // MCPoints in MVD, GEM and STT
    Int_t TrackCounter = 0;
    Int_t MVDPoints = 0, GEMPoints =0, STTPoints = 0;
    Int_t fMVDPoints = 0, fGEMPoints = 0, fSTTPoints = 0, fTotalPoints = 0;
    
    
    // TTree Setup
    TFile *f = new TFile("AFile.root","RECREATE");
    TTree *tree = new TTree("mumu","A Root Tree for Storing Muon Tracks");
    
    
    // Event Loop
    for (Int_t event=0; event < ev_entries; ++event) {
        ioman->ReadEvent(event); std::cout << "\nProcessing the Event: "<< event << std::endl;

        
        // BarrelTrackArray Loop
        std::cout << "BarrelTracks per Event: " << fBarrelTrackArray->GetEntries() << std::endl;
        
        for (Int_t idx = 0; idx < fBarrelTrackArray->GetEntries(); idx++) {
            
            // Fetch a PndTrack from the fBarrelTrackArray
            PndTrack *barrelTrack = (PndTrack *)fBarrelTrackArray->At(idx);
            
            // Connect BarrelTrack to its MCTrack
            linksMC = barrelTrack->GetLinksWithType(ioman->GetBranchId("MCTrack"));
                       
            // Here, linksMC.GetNLinks()==1 always. So "if-for-if" always runs, then why we have these statements.
            if (linksMC.GetNLinks()>0) {
                for (Int_t i=0; i<linksMC.GetNLinks(); i++) {
                    if (linksMC.GetLink(i).GetIndex()==barrelTrack->GetTrackCand().getMcTrackId()) {

                        // Here I am getting MCTrack Data (Array) which is associated with BarrelTrack. 
                        // So, in other words, I am getting MCTruth of the a particular BarrelTrack.
                        
                        PndMCTrack *mcTrack = (PndMCTrack *)ioman->GetCloneOfLinkData(linksMC.GetLink(i));
                        linksSTT = barrelTrack->GetLinksWithType(ioman->GetBranchId("STTHit"));
                        linksGEM = barrelTrack->GetLinksWithType(ioman->GetBranchId("GEMHit"));
                        linksMVDPixel = barrelTrack->GetLinksWithType(ioman->GetBranchId("MvdHitsPixel"));
                        linksMVDStrip = barrelTrack->GetLinksWithType(ioman->GetBranchId("MvdHitsStrip"));
                        
                        Int_t Nhits = (linksMVDPixel.GetNLinks()+linksMVDStrip.GetNLinks()+linksGEM.GetNLinks()+linksSTT.GetNLinks());
                        
                        // If the number of STT hits greater than 0, write MC track to file!!
                        
                        
                        // Init Particle Object
                        Particles* part = new Particles();
                        
                        // Print Particles
                        // part->Print();
                        
                        
                        // Fill Particle Object
                        part->SetStartVertex(mcTrack->GetStartVertex());
                        part->SetMomentum(mcTrack->GetMomentum());
                        part->SetParticleId(linksMC.GetLink(i).GetIndex());
                        part->SetCharge((mcTrack->GetPdgCode()>0)?1:-1);
                        part->SetNHits(Nhits);                            // Can we use linksMC?
                        part->SetPdgCode(mcTrack->GetPdgCode());
                        part->SetStartTime(mcTrack->GetStartTime());
                        part->SetIsGenCreated(mcTrack->IsGeneratorCreated());                        
                        
                        // Add the Object to fParticle branch.
                        //tree->Branch("fParticle","Particle",&part,32000,0);
                        //tree->Fill();
 
                        
                        
                        cout << "Printing to CSV" << endl;
                        
                        // Write to CSV
                        std::cout   << (linksMC.GetLink(i).GetIndex()    == part->GetParticleId() )      << ","   // track_id > 0
                                    << (mcTrack->GetStartVertex().X()    == part->GetStartVertex().X() ) << ","   // vx = start x [cm, ns]
                                    << (mcTrack->GetStartVertex().Y()    == part->GetStartVertex().Y() ) << ","   // vy = start y [cm, ns]
                                    << (mcTrack->GetStartVertex().Z()    == part->GetStartVertex().Z() ) << ","   // vz = start z [cm, ns]
                                    << (mcTrack->GetMomentum().X()       == part->GetMomentum().X() )    << ","   // px = x-component of track momentum
                                    << (mcTrack->GetMomentum().Y()       == part->GetMomentum().Y() )    << ","   // py = y-component of track momentum
                                    << (mcTrack->GetMomentum().Z()       == part->GetMomentum().Z() )    << ","   // pz = z-component of track momentum
                                    << (((mcTrack->GetPdgCode()>0)?1:-1) == part->GetCharge() )          << ","   // q = charge of mu-/mu+
                                    << (Nhits)                           == part->GetNHits())            << ","   // nhits
                                    << (mcTrack->GetPdgCode()            == part->GetPdgCode() )         << ","   // pdgcode e.g. mu- has pdgcode=-13
                                    << (mcTrack->GetStartTime()          == part->GetStartTime())        << ","   // start_time = starting time of particle track
                                    << (mcTrack->IsGeneratorCreated()    == part->GetIsGenCreated())           // If a particle is primary or not
                                    << std::endl;                        
                        del part;
                    }//end-if(GetLink(i))
                }//end-for(GetNLinks)
            }//end-if(GetNLinks)
            
        }//end-for (barrelTrack)
        
        tree->Print();
        tree->Write();
        f->Close();
        
        // ----------------- Start BarrelTrackArray Loop
	    
	    
	    
    }//Event
}//Macro

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















