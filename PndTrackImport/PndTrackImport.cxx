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
    , fBarrelTrackBranchID(-1)      //formerly SttMvdGemTrack
    , fBarrelTrackArray(nullptr)    // formerly SttMvdGemTrack
    , fSttParameters(nullptr)
    , fEventHeader(nullptr)
    , fTubeArray(nullptr) {

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
    fBarrelTrackArray = (TClonesArray*) ioman->GetObject("BarrelTrack");
    fBarrelTrackBranchID = ioman->GetBranchId("BarrelTrack");
    
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

    std::cout << "-I- PndTrackImport: Initialisation successful" << std::endl;
    return kSUCCESS;

}

/* Exec() */
void PndTrackImport::Exec(Option_t* /*opt*/) {
    
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << fEventId;
    std::string fidx = ss.str();   
    TString prefix = fCsvFilesPath+"/"+fidx;
    TString filename = prefix+".csv";
    
    std::cout << "\nProcessing Event: " << (fEventId) << " with Prefix: " << prefix << std::endl;
    

    // RDataFrame from CSVs
    //auto tdf = ROOT::RDF::FromCSV(filename);             // Root v6.26    
    //auto rdf = ROOT::RDF::MakeCsvDataFrame(filename);    // Root v6.22
    
    // TTree from CSVs
    TTree *t = new TTree("t", "Track Cand");
    t->ReadFile(filename, "hit_id/I:track_id"); // we should get tree with branches hit_id, track_id
    t->Print();
    //t->Scan();
   
    // TODO: Loop over TTree Branches to Get Track Cands e.g. as track_id: {list of hids/cids}
    
    
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

