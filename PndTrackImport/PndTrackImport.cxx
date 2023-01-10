/*
 * PndTrackImport.cxx
 *
 *  Created on: Nov 8, 2018
 *      Author: Adeel Akram
 */

#include <FairRootManager.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <FairTask.h>
#include <FairMCPoint.h>

#include <PndSttMapCreator.h>
#include <PndSttTube.h>
#include <PndTrackCand.h>
#include <TClonesArray.h>

// ROOT::RDataFrame & ROOT::RCsvDS
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RCsvDS.hxx>

// My FairTask
#include "PndTrackImport.h"


ClassImp(PndTrackImport)


/*
* (1) Constructor Initialization by Assignment:
* Avoid this method as it is a two step process. In this method, first
* default constructor is called (step 1) and then assignment operation
* is performed to intialize the data memebers.
*
* (2) Constructor Initialization by Inline Assignment:
* In this case, an object is instantiated with default values of data 
* members. Its a one step process, assignment is done in "*.h" files.
* NOTE: Inline assignment is only available in C++11 and beyond.
*
* (3) Constructor Initialization by Initializer List:
* In this case, an object is instantiated with default values of data 
* members. Its a one step process, default values are provided using
* and initializer list to the constructor. It is considered the BEST
* way for instantiating a statefull class object.
* NOTE: Initializer list depends on order of declaration of data members.
*/


/* PndTrackImport() */
PndTrackImport::PndTrackImport() {
}

PndTrackImport::PndTrackImport(int start_counter, TString csv_path)
    : fEventId(start_counter)
    , fCsvFilesPath(csv_path)
    , fMCTrackBranchID(-1)
    , fMCTrackArray(nullptr)
    //, fBarrelTrackBranchID(-1)      // formerly SttMvdGemTrack
    //, fBarrelTrackArray(nullptr)    // formerly SttMvdGemTrack
    , fMvdPointBranchID(-1)
    , fMvdPointArray(nullptr)
    , fMvdHitsPixelBranchID(-1)
    , fMvdHitsPixelArray(nullptr)
    , fMvdHitsStripBranchID(-1)
    , fMvdHitsStripArray(nullptr)
    , fGemPointBranchID(-1)
    , fGemPointArray(nullptr)
    , fGemHitBranchID(-1)
    , fGemHitArray(nullptr)
    , fSttPointBranchID(-1)
    , fSttPointArray(nullptr)
    , fSttHitBranchID(-1)           // SttHitBranchID
    , fSttHitArray(nullptr)         // SttHitArray
    , fSttParameters(nullptr)
    , fEventHeader(nullptr)
    , fTubeArray(nullptr) 
    , fBarrelTrackArray(nullptr) 
    , fBarrelTrackCandArray(nullptr) {

    /* Constructor */
}

/* Destructor */
PndTrackImport::~PndTrackImport() {
}

/* SetParContainers() */
void PndTrackImport::SetParContainers() {

    FairRuntimeDb *rtdb = FairRunAna::Instance()->GetRuntimeDb();
    fSttParameters = (PndGeoSttPar*) rtdb->getContainer("PndGeoSttPar");

}

/* Init() */
InitStatus PndTrackImport::Init() {

    // Get instance of the FairRootManager to access tree branches
    FairRootManager *ioman = FairRootManager::Instance();
    if(!ioman) {
        std::cout << "-E- PndTrackImport::Init: FairRootManager not instantiated!" << std::endl;
        return kFATAL;
    }
    
    // Get the EventHeader
    fEventHeader = (TClonesArray*) ioman->GetObject("EventHeader.");
    if(!fEventHeader) {
        std::cout << "-E- PndTrackImport::Init: EventHeader not loaded!" << std::endl;
        return kFATAL;
    }
    
    // Access STTMapCreater
    PndSttMapCreator *mapper = new PndSttMapCreator(fSttParameters);
    fTubeArray = mapper->FillTubeArray();
        
    // Access MCTrack Branch and its Ids
    fMCTrackArray = (TClonesArray*) ioman->GetObject("MCTrack");
    fMCTrackBranchID = ioman->GetBranchId("MCTrack");
    
    if (!fMCTrackArray) {
        LOG(error) << " " << GetName() << "::Init: No MCTrack array!";
        return kERROR;
    } 
    
    // Access BarrelTrack (formerly SttMvdGemTrack) Branch and its ID
    //fBarrelTrackArray = (TClonesArray*) ioman->GetObject("BarrelTrack");
    //fBarrelTrackBranchID = ioman->GetBranchId("BarrelTrack");
    
    //Access MVDPoint, MVDHitsPixel, MvdHitsStrip Branches and their Ids
    fMvdPointArray = (TClonesArray*) ioman->GetObject("MVDPoint");
    fMvdPointBranchID = ioman->GetBranchId("MVDPoint");
    
    fMvdHitsPixelArray = (TClonesArray*) ioman->GetObject("MVDHitsPixel");
    fMvdHitsPixelBranchID = ioman->GetBranchId("MVDHitsPixel");
    
    fMvdHitsStripArray = (TClonesArray*) ioman->GetObject("MVDHitsStrip");
    fMvdHitsStripBranchID = ioman->GetBranchId("MVDHitsStrip");
    
    //Access GEMPoint, GEMHit Branches and their Ids
    fGemPointArray = (TClonesArray*) ioman->GetObject("GEMPoint");
    fGemPointBranchID = ioman->GetBranchId("GEMPoint");
    fGemHitArray = (TClonesArray*) ioman->GetObject("GEMHit");
    fGemHitBranchID = ioman->GetBranchId("GEMHit");
    
    //Access STTPoint, STTHit Branches and their Ids
    fSttPointArray = (TClonesArray*) ioman->GetObject("STTPoint");
    fSttPointBranchID = ioman->GetBranchId("STTPoint");
    
    fSttHitArray = (TClonesArray*) ioman->GetObject("STTHit");
    fSttHitBranchID = ioman->GetBranchId("STTHit");
    
    
    // TODO:  Adding PndTrack and PndTrackCand
    // Create and register PndTrack and PndTrackCand arrays
    //fBarrelTrackArray = new TClonesArray("PndTrack", 100);
    //ioman->Register("BarrelTrack", "Barrel Track", fBarrelTrackArray, GetPersistency());

    std::cout << "-I- PndTrackImport: Initialisation successful" << std::endl;
    return kSUCCESS;

}

/* Exec() */
void PndTrackImport::Exec(Option_t* /*opt*/) {
    
    // Filename of CSV
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << fEventId;
    std::string fidx = ss.str();   
    TString prefix = fCsvFilesPath+"/"+fidx;
    TString filename = prefix+".root";
    
    std::cout << "\nProcessing Event: " << (fEventId) << " with Prefix: " << prefix << std::endl;

    // RDataFrame from CSVs
    // auto rdf = ROOT::RDF::FromCSV(filename);             // Root v6.26    
    // auto rdf = ROOT::RDF::MakeCsvDataFrame(filename);    // Root v6.22
    
    // TTree from CSVs
    // TTree *t = new TTree("t", "Track Cand");
    // t->ReadFile(filename, "hit_id/I:track_id");
    
    
    // Reading TTree from ROOT
    TFile *f = TFile::Open(filename);
    
    // Get TTree from TFile (It already has a TTree)
    TTree *t = (TTree*)f->Get("TrackML");
    
    // Or, TTreeReader to get an iterator over TTree
    TTreeReader myReader("TrackML", f);
    TTreeReaderValue<int> hit_id(myReader, "hit_id");           // or cids
    TTreeReaderValue<long long> track_id(myReader, "track_id");
    
    // Set to Unique Track Ids
    std::set<int> unique_tracks;
    
    // Multimap for TrackCand
	std::unordered_multimap<int, int> umap_track_cands;
	
	// Map for TrackCand
    std::map<int, std::vector<int> > map_track_cands;
	
	// PndTrackCand
	std::map<int, PndTrackCand*> mycands;
    
    // Fill Up
    while (myReader.Next()) {
        
        int tid = int(*track_id);
        int hid = *hit_id;
        
        // Unique Tracks
        unique_tracks.insert(tid);
        
        // UMap: TrackCands
        umap_track_cands.insert({tid, hid});
        
        // Map: TrackCands
        map_track_cands[tid].push_back(hid);
        
        // Map: PndTrackCands
        //if(mycands[tid]==nullptr) {
        //    mycands[tid] = new PndTrackCand(); 
        //}
        
        
        //PndTrackCand* myT = mycands[tid];

        //... get hit pointer from hitID & Pandaroot
        //myT->AddHit(...);
      
    }  
    
     
     
     
    cout << "TrackML TTree Size: " << t->GetEntries() << endl;
    cout << "fSttHitArray Size : " << fSttHitArray->GetEntries() << endl;
    
    // Display Unique Tracks
    std::cout << "\nUnique Track IDs:" << std::endl;
	for (auto const& iter : unique_tracks) {  // c++11 
    	std::cout << ' ' << iter; 
    }
    
        
    // Filter UMap
    /*
	std::cout << "\nFilter Values for a Key:" << std::endl;
    auto range = umap_track_cands.equal_range(0);
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->first << ' ' << it->second << '\n';
    }
    */
    
    /* RALF:
    
    std::map<int, std::vector<int> > map_track_cands;
    while(){
      map_track_cands[*trID].push_pack(*hitID):
    }
    ... loop map and create track Cands


      ----- OR


    std::map<PndTrackCand*> mycands;
    while(){

      if(mycands[*trID]==nullptr) {
        mycands[*trID] = new PndTrackCand(...); 
      }

      PndTrackCand* myT = mycands[*trID];

      ... get hit pointer from hitID & Pandaroot
      myT->AddHit(...);
    }
    
    */
    
    // Track Candidates
	std::cout << "\nTrack Cands:" << std::endl;
	for (auto const &iter : map_track_cands) {  // c++11
	 
    	std::cout << iter.first << ':' ;
    	
    	for(auto const& iter1: iter.second) 
        	std::cout << ' ' << iter1;
        	
        cout << endl;
    }
    
    
    
    std::cout << "Finishing Event: " << (fEventId) << " with Prefix: " << prefix << std::endl;
    
    // Finishing
    fEventId++;

}//end-Exec()

/* FinishTask() */
void PndTrackImport::FinishTask() {
    
    // Finishing FairTask        
    std::cout << "\n-I- PndTrackImport Task Has Finished." << std::endl;
}


/* GetFairMCPoint() */
FairMCPoint* PndTrackImport::GetFairMCPoint(TString fBranchName, FairMultiLinkedData_Interface* links, FairMultiLinkedData& array) {
    
    // get the mc point(s) from each reco hit
    FairMultiLinkedData mcpoints = links->GetLinksWithType(FairRootManager::Instance()->GetBranchId(fBranchName));
    array = mcpoints;
    if (array.GetNLinks() == 0){
        return 0;
    }
    return (FairMCPoint*) FairRootManager::Instance()->GetCloneOfLinkData(array.GetLink(0));
}


void PndTrackImport::SttTrackCand() { 

    std::cout << "-I- PndTrackImport: Runing SttTrackCand()" << std::endl;
    
    // SttHitArray
    if (fSttHitArray->GetEntries()==0)
         std::cout << "Warning! SttHitArray is Empty." << std::endl;
         
    for (int idx=0; idx < fSttHitArray->GetEntries(); idx++) {

        // Get FairRootManager Instance
        FairRootManager *ioman = FairRootManager::Instance();

        // Get sttHitsLinks from STTHit
        FairMultiLinkedData_Interface *sttHitsLinks = (FairMultiLinkedData_Interface*)fSttHitArray->At(idx);

        // Get sttPointLinks & Data (TCloneArray) from sttHitsLinks
        FairMultiLinkedData sttPointLinks = sttHitsLinks->GetLinksWithType(fSttPointBranchID);
        FairMCPoint *sttpoint = (FairMCPoint*)ioman->GetCloneOfLinkData(sttPointLinks.GetLink(0));
        
        // Terminate if sttpoint=NULL
        if (sttpoint == 0) {continue;}
        
        // Get mcTrackLinks & Data (TCloneArray) from sttpoint (OR sttPointLinks?)
        FairMultiLinkedData mcTrackLinks = sttpoint->GetLinksWithType(fMCTrackBranchID); 
        PndMCTrack *mctrack = (PndMCTrack*)ioman->GetCloneOfLinkData(mcTrackLinks.GetLink(0));
        
        // Terminate if mcTrack=NULL
        if (mctrack == 0) {continue;}
        
        // Terminate if not Primary
        // if (!mctrack->IsGeneratorCreated())
        //    continue;
        
    
    
    
        
    }//fSttHitArray
    
}//SttTrackCand

