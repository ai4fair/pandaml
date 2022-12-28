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
    , fSttSkewHitBranchID(-1)       // SttSkewHitBranchID
    , fSttSkewHitArray(nullptr)     // SttSkewHitArray
    //, fBarrelTrackBranchID(-1)      // formerly SttMvdGemTrack
    //, fBarrelTrackArray(nullptr)    // formerly SttMvdGemTrack
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
    
    //Access STTCombinedSkewedHits Branches and their Ids
    fSttSkewHitArray = (TClonesArray*) ioman->GetObject("STTCombinedSkewedHits");
    fSttSkewHitBranchID = ioman->GetBranchId("STTCombinedSkewedHits");
    
    
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
    TFile *myFile = TFile::Open(filename);
    TTreeReader myReader("TrackML", myFile);

    TTreeReaderValue<int> hit_id(myReader, "hit_id");           // or cids
    TTreeReaderValue<long long> track_id(myReader, "track_id");
    
    // Set to Unique Track Ids
    std::set<long long> unique_track_ids;
    
    // Multimap for TrackCand
	std::unordered_multimap<int, long long> umap_track_cands;
	
	
    
    // Loop over all entries of the TTree or TChain.
    while (myReader.Next()) {
        
        // Just access the data as if they were iterators (note the '*' in front of them):
        // cout  << "hit_id: " << *hit_id << ", track_id: " << *track_id << endl;
        
        // How about making a C++ Map so one can fetch a track_id with all of its hits
        
        // Insert TrackCand
        umap_track_cands.insert({*track_id, *hit_id});
        unique_track_ids.insert(*track_id);
        
    }
    
    
    // Test Multimap and Set
    std::cout << "\nSet Elements:" << std::endl;
	for (auto const& iter : unique_track_ids) {  // c++11 
    	std::cout << ' ' << iter; 
    }
    
    // Find list of hits for a track?
    // Lets filter values based on certain key
	std::cout << "\nFilter Values for a Key:" << std::endl;
    auto range = umap_track_cands.equal_range(0);
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->first << ' ' << it->second << '\n';
    }
    
    
    
    
    
    
    
    
    std::cout << " Finishing Event: " << (fEventId) << " with Prefix: " << prefix << std::endl;
    
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

