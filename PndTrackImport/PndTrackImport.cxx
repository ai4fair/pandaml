/*
 * PndTrackImport.cxx
 *
 *  Created on: Nov 8, 2018
 *      Author: Adeel Akram
 */

#include <FairRootManager.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <PndSttMapCreator.h>
#include <PndSttTube.h>
#include <PndTrackCand.h>
#include <TClonesArray.h>

#include "FairTask.h"
#include "FairMCPoint.h"
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
    ss << std::setw(10) << std::setfill('0') << fEventId;
    std::string fidx = ss.str();   
    TString csv_path = fCsvFilesPath+fidx;
    
    
    fInfile.open(csv_path+"-hits.csv");
    std::cout << "\nProcessing Event: " << csv_path << std::endl;
    
    
    
    
   
    
    std::cout << "Finishing Event: " << (fEventId) << " with Prefix: " << "event"+fidx << std::endl;
    

}//end-Exec()

/* FinishTask() */
void PndTrackImport::FinishTask() {
    
    // Close all files
        
    std::cout << "-I- Task Generating CSVs has Finished." << std::endl;
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
        
        // Hit Counter (very important in case number of tracks vary per event)
        fHitId++;
    
    
    
        
    }//fSttHitArray
    
}//SttTrackCand














/*
// GenerateMvdPixelData()
void PndTrackImport::GenerateMvdPixelData() { 
    
    std::cout << "-I- PndTrackImport: Runing GenerateMvdPixelData()" << std::endl;
    
    // MvdHitsPixel
    if (fMvdHitsPixelArray->GetEntries()==0)
         std::cout << "Warning! MvdHitsPixel is Empty." << std::endl;
         
    for (int idx=0; idx < fMvdHitsPixelArray->GetEntries(); idx++) {
        
        // Get FairRootManager Instance
        FairRootManager *ioman = FairRootManager::Instance();
        
        // Get mvdHitsLinks from MVDHitsPixel
        FairMultiLinkedData_Interface *mvdHitsLinks = (FairMultiLinkedData_Interface*)fMvdHitsPixelArray->At(idx);

        // Get mvdPointLinks & Data (TCloneArray) from mvdHitsLinks
        FairMultiLinkedData mvdPointLinks = mvdHitsLinks->GetLinksWithType(fMvdPointBranchID);
        FairMCPoint *sdsPoint = (FairMCPoint*)ioman->GetCloneOfLinkData(mvdPointLinks.GetLink(0));
        
        // Terminate if sdsPoint=NULL
        if (sdsPoint == 0) {continue;}
        
        // Get mcTrackLinks & Data (TCloneArray) from sdsPoint (OR mvdPointLinks?)
        FairMultiLinkedData mcTrackLinks = sdsPoint->GetLinksWithType(fMCTrackBranchID); 
        PndMCTrack *mctrack = (PndMCTrack*)ioman->GetCloneOfLinkData(mcTrackLinks.GetLink(0));
        
        // Terminate if mcTrack=NULL
        if (mctrack == 0) {continue;}
        
        // Terminate if not Primary
        // if (!mctrack->IsGeneratorCreated())
        //    continue;
        
        // Hit Counter (very important in case number of tracks vary per event)
        fHitId++;
        
                
        // Write to xxx-hits.csv
        // ---------------------------------------------------------------------------
        PndSdsHit* sdsHit = (PndSdsHit*)fMvdHitsPixelArray->At(idx);
        
        fHits << fHitId                    << ","   // hit_id
              << sdsHit->GetX()            << ","   // x-position
              << sdsHit->GetY()            << ","   // y-position
              << sdsHit->GetZ()            << ","   // z-position
              << sdsHit->GetDetectorID()   << ","   // volume_id (2 for Pixel)
              << GetLayerMvd(sdsHit)       << ","   // layer_id
              << sdsHit->GetSensorID()              // sensor_id/module_id
              << std::endl;


        // Write to xxx-truth.csv
        // ---------------------------------------------------------------------------
        
        // Get Particle Id
        // Get MCTrack ID associated with each idx (fMvdHitsPixelArray)
        std::vector<FairLink> mcTrackLinks_sorted = mvdHitsLinks->GetSortedMCTracks();
        TString particle_id = "";
        
        for (unsigned int trackIndex = 0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
            
            // Append "particle_id" to xxx-truth.csv
            particle_id = std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);
        }
        
        fTruths << fHitId                  << ","   // hit_id  
                << sdsPoint->GetX()        << ","   // tx = true x
                << sdsPoint->GetY()        << ","   // ty = true y
                << sdsPoint->GetZ()        << ","   // tz = true z
                << sdsPoint->GetPx()       << ","   // tpx = true px
                << sdsPoint->GetPy()       << ","   // tpy = true py
                << sdsPoint->GetPz()       << ","   // tpz = true pz
                << (1.0)                   << ","   // FIXME: Weight Placeholder, but sum of weights of hits in track == 1)
                << particle_id                      // Particle_id from above   
                << std::endl;    
       
       
        // Write to xxx-cells.csv
        // ---------------------------------------------------------------------------           
        fCells  << fHitId                  << ","   // hit_id
                << sdsHit->GetCharge()     << ","   // deposited charge 
                << sdsHit->GetEloss()      << ","   // energy loss (silicon)
                << sdsHit->GetDetectorID() << ","   // volume_id (2 for Pixel)
                << GetLayerMvd(sdsHit)     << ","   // layer_id
                << sdsHit->GetSensorID()   << ","   // module_id
                << ("-nan")                << ","   // sector_id
                << ("-nan")                << ","   // skewed
                << ("-nan")                         // isochrone
                << std::endl;
                
    }//fMvdHitsPixelArray  
    
}//GenerateMvdPixelData


// GenerateMvdStripData()
void PndTrackImport::GenerateMvdStripData() { 
    
    std::cout << "-I- PndTrackImport: Runing GenerateMvdStripData()" << std::endl;   
    
    // MvdHitsStripArray
    if (fMvdHitsStripArray->GetEntries()==0)
         std::cout << "Warning! MvdHitsStripArray is Empty." << std::endl;
         
    for (int idx=0; idx < fMvdHitsStripArray->GetEntries(); idx++) {

        // Get FairRootManager Instance
        FairRootManager *ioman = FairRootManager::Instance();

        // Get mvdHitsLinks from MVDHitsStrip
        FairMultiLinkedData_Interface *mvdHitsLinks = (FairMultiLinkedData_Interface*)fMvdHitsStripArray->At(idx);

        // Get mvdPointLinks & Data (TCloneArray) from mvdHitsLinks
        FairMultiLinkedData mvdPointLinks = mvdHitsLinks->GetLinksWithType(fMvdPointBranchID);
        FairMCPoint *sdsPoint = (FairMCPoint*)ioman->GetCloneOfLinkData(mvdPointLinks.GetLink(0));
        
        // Terminate if sdsPoint=NULL
        if (sdsPoint == 0) {continue;}
        
        // Get mcTrackLinks & Data (TCloneArray) from sdsPoint (OR mvdPointLinks?)
        FairMultiLinkedData mcTrackLinks = sdsPoint->GetLinksWithType(fMCTrackBranchID); 
        PndMCTrack *mctrack = (PndMCTrack*)ioman->GetCloneOfLinkData(mcTrackLinks.GetLink(0));
        
        // Terminate if mcTrack=NULL
        if (mctrack == 0) {continue;}
        
        // Terminate if not Primary
        // if (!mctrack->IsGeneratorCreated())
        //    continue;
        
        // Hit Counter (very important in case number of tracks vary per event)
        fHitId++;

        
        // Write to xxx-hits.csv
        // ---------------------------------------------------------------------------
        PndSdsHit* sdsHit = (PndSdsHit*)fMvdHitsStripArray->At(idx);
        
        fHits << fHitId                    << ","   // hit_id
              << sdsHit->GetX()            << ","   // x-position
              << sdsHit->GetY()            << ","   // y-position
              << sdsHit->GetZ()            << ","   // z-position
              //<< sdsHit->GetDetectorID() << ","   // volume_id (27 for strip)
              << (3)                       << ","   // volume_id (27 --> 3)
              << GetLayerMvd(sdsHit)       << ","   // layer_id
              << sdsHit->GetSensorID()              // sensor_id/module_id
              << std::endl;


        // Write to xxx-truth.csv
        // ---------------------------------------------------------------------------
        
        // Get Particle Id
        // Get MCTrack ID associated with each idx (fMvdHitsStripArray)
        std::vector<FairLink> mcTrackLinks_sorted = mvdHitsLinks->GetSortedMCTracks();
        TString particle_id = "";
        
        for (unsigned int trackIndex = 0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
            
            // Append "particle_id" to xxx-truth.csv
            particle_id = std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);
        }
        
        fTruths << fHitId                  << ","   // hit_id  
                << sdsPoint->GetX()        << ","   // tx = true x
                << sdsPoint->GetY()        << ","   // ty = true y
                << sdsPoint->GetZ()        << ","   // tz = true z
                << sdsPoint->GetPx()       << ","   // tpx = true px
                << sdsPoint->GetPy()       << ","   // tpy = true py
                << sdsPoint->GetPz()       << ","   // tpz = true pz
                << (1.0)                   << ","   // FIXME: Weight Placeholder, but sum of weights of hits in track == 1)
                << particle_id                      // Particle_id from above   
                << std::endl;    
       
       
        // Write to xxx-cells.csv
        // ---------------------------------------------------------------------------           
        fCells  << fHitId                  << ","   // hit_id
                << sdsHit->GetCharge()     << ","   // deposited charge 
                << sdsHit->GetEloss()      << ","   // energy loss (silicon)
                //<<sdsHit->GetDetectorID()<< ","   // volume_id (27 for strip)
                << (3)                     << ","   // volume_id (27 --> 3)
                << GetLayerMvd(sdsHit)     << ","   // layer_id
                << sdsHit->GetSensorID()   << ","   // module_id
                << ("-nan")                << ","   // sector_id
                << ("-nan")                << ","   // skewed
                << ("-nan")                         // isochrone
                << std::endl;
                
    }//fMvdHitsStripArray
    
    
}//GenerateMvdStripData


// GenerateGemData()
void PndTrackImport::GenerateGemData() { 
    
    std::cout << "-I- PndTrackImport: Runing GenerateGemData()" << std::endl;
    
    // GemHitArray
    if (fGemHitArray->GetEntries()==0)
         std::cout << "Warning! GemHitArray is Empty." << std::endl;
         
    for (Int_t idx = 0; idx < fGemHitArray->GetEntries(); idx++) {
        
        
        // Get FairRootManager Instance
        FairRootManager *ioman = FairRootManager::Instance();

        // Get gemHitsLinks from GEMHit
        FairMultiLinkedData_Interface *gemHitsLinks = (FairMultiLinkedData_Interface*)fGemHitArray->At(idx);

        // Get gemPointLinks & Data (TCloneArray) from gemHitsLinks
        FairMultiLinkedData gemPointLinks = gemHitsLinks->GetLinksWithType(fGemPointBranchID);
        FairMCPoint *gemPoint = (FairMCPoint*)ioman->GetCloneOfLinkData(gemPointLinks.GetLink(0));
        
        // Terminate if gemPoint=NULL
        if (gemPoint == 0) {continue;}
        
        // Get mcTrackLinks & Data (TCloneArray) from gemPoint (OR gemPointLinks?)
        FairMultiLinkedData mcTrackLinks = gemPoint->GetLinksWithType(fMCTrackBranchID); 
        PndMCTrack *mctrack = (PndMCTrack*)ioman->GetCloneOfLinkData(mcTrackLinks.GetLink(0));
        
        // Terminate if mcTrack=NULL
        if (mctrack == 0) {continue;}
        
        // Terminate if not Primary
        // if (!mctrack->IsGeneratorCreated())
        //    continue;
        
        
        // Hit Counter (very important in case number of tracks vary per event)
        fHitId++;
        
        // Write to xxx-hits.csv
        // ---------------------------------------------------------------------------        
        PndGemHit* gemHit = (PndGemHit*)fGemHitArray->At(idx);
        
        fHits << fHitId                    << ","   // hit_id
              << gemHit->GetX()            << ","   // x-position
              << gemHit->GetY()            << ","   // y-position
              << gemHit->GetZ()            << ","   // z-position
              //<<gemHit->GetDetectorID()  << ","   // volume_id (strange numbers)
              << (6)                       << ","   // volume_id (let's say its 6)
              << GetLayerGem(gemHit)       << ","   // layer_id
              << gemHit->GetSensorNr()              // sensor_id/module_id
              << std::endl;



        // Write to xxx-truth.csv
        // ---------------------------------------------------------------------------
        
        // Get Particle Id
        std::vector<FairLink> mcTrackLinks_sorted = gemHitsLinks->GetSortedMCTracks();
        TString particle_id = "";
        
        for (unsigned int trackIndex = 0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
            
            // Append "particle_id" to xxx-truth.csv
            particle_id = std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);
        }
        
        fTruths << fHitId                  << ","   // hit_id  
                << gemPoint->GetX()        << ","   // tx = true x
                << gemPoint->GetY()        << ","   // ty = true y
                << gemPoint->GetZ()        << ","   // tz = true z
                << gemPoint->GetPx()       << ","   // tpx = true px
                << gemPoint->GetPy()       << ","   // tpy = true py
                << gemPoint->GetPz()       << ","   // tpz = true pz
                << (1.0)                   << ","   // FIXME: Weight Placeholder, but sum of weights of hits in track == 1)
                << particle_id                      // Particle_id from above   
                << std::endl;    
       
       
        // Write to xxx-cells.csv
        // ---------------------------------------------------------------------------        
        fCells  << fHitId                  << ","   // hit_id
                << gemHit->GetCharge()     << ","   // deposited charge 
                << gemHit->GetEloss()      << ","   // energy loss (silicon)
                //<<gemHit->GetDetectorID()<< ","   // volume_id (strange numbers)
                << (6)                     << ","   // volume_id (let's say its 6)
                << GetLayerGem(gemHit)     << ","   // layer_id
                << gemHit->GetSensorNr()   << ","   // module_id
                << ("-nan")                << ","   // sector_id
                << ("-nan")                << ","   // skewed
                << ("-nan")                         // isochrone
                << std::endl;
    }//GemHitArray

}//GenerateGemData


// GenerateSttData()
void PndTrackImport::GenerateSttData() {
    
    std::cout << "-I- PndTrackImport: Runing GenerateSttData()" << std::endl;
    
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
        
        // Hit Counter (very important in case number of tracks vary per event)
        fHitId++;
        
        // Write to xxx-hits.csv
        // ---------------------------------------------------------------------------
        PndSttHit* stthit = (PndSttHit*)fSttHitArray->At(idx);
        PndSttTube *tube = (PndSttTube*) fTubeArray->At(stthit->GetTubeID());
        
        // TODO: Skewed Hits Correction Separately.
        // Skip Skewed Tubes, Will handle them in GenerateSttSkewData()
        if (tube->IsSkew()) {continue;}
        
        fHits << fHitId                    << ","   // hit_id
              << stthit->GetX()            << ","   // x-position
              << stthit->GetY()            << ","   // y-position
              << stthit->GetZ()            << ","   // z-position
              << stthit->GetDetectorID()   << ","   // volume_id
              << tube->GetLayerID()        << ","   // layer_id
              << stthit->GetTubeID()                // tube_id/module_id
              << std::endl;
        
        
        // Write to xxx-truth.csv
        // ---------------------------------------------------------------------------
        
        // Get Particle Id
        TString particle_id = "";
        std::vector<FairLink> mcTrackLinks_sorted = sttHitsLinks->GetSortedMCTracks();
         
        for (unsigned int trackIndex = 0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
            
            // Append "particle_id" to xxx-truth.csv
            particle_id = std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);
        }
        
         fTruths << fHitId                 << ","   // hit_id  
                << sttpoint->GetX()        << ","   // tx = true x
                << sttpoint->GetY()        << ","   // ty = true y
                << sttpoint->GetZ()        << ","   // tz = true z
                << sttpoint->GetPx()       << ","   // tpx = true px
                << sttpoint->GetPy()       << ","   // tpy = true py
                << sttpoint->GetPz()       << ","   // tpz = true pz
                << (1.0)                   << ","   // FIXME: weight (sum of weights of hits in track == 1)
                << particle_id                      // Particle_id from above   
                << std::endl;    


        // Write to xxx-cells.csv
        // ---------------------------------------------------------------------------
        fCells  << fHitId                  << ","   // hit_id
                << stthit->GetDepCharge()  << ","   // deposited charge 
                << stthit->GetEnergyLoss() << ","   // energy loss (silicon)
                << stthit->GetDetectorID() << ","   // volume_id
                << tube->GetLayerID()      << ","   // layer_id
                << stthit->GetTubeID()     << ","   // module_id
                << tube->GetSectorID()     << ","   // sector_id
                << stthit->GetIsochrone()  << ","   // isochrone
                << tube->IsSkew()                   // skewed
                << std::endl;



        // Write to xxx-particles.csv
        // ---------------------------------------------------------------------------
        
        /** -----------------------------------------------------------------------
        * FIXME: The issue is fParticles should contain unique particle_ids (tracks)
        * e.g. if there are 10 particles/event then it should contain only 10 records.
        * But right now fParticles contain a particle records which are equal to the
        * size of fSttHitArray. Which mean there will be repition of data in the file.
        * One solution might be to write the fParticles outside of fSttHitArray. So I
        * shifted the this piece of code outside of fSttHitArray loop. Note, however, 
        * that if MVD and GEM are also included then fParticles should be generated in 
        * Exec() rather than GenerateMvdData()/GenerateGemData() or GenerateSttData()
        * -------------------------------------------------------------------------/
        
        // NOTE: In particle.csv dataframe, count duplicate particle_ids and add this 
        // number as nhits. After that one can drop_duplicates from particle dataframe.
        

        //std::cout << "Number of MCTracks (fParticles): " << mcTrackLinks_sorted.size() << std::endl;
        //std::vector<FairLink> mcTrackLinks_sorted = sttHitsLinks->GetSortedMCTracks();
        
        if (fAssistedByIdeal.Contains("WithoutIdeal")) {
            for (unsigned int trackIndex=0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
            
                // Fetch mcTrack associated with SttHit
                PndMCTrack *mcTrack = (PndMCTrack*)ioman->GetCloneOfLinkData(mcTrackLinks_sorted[trackIndex]);
                
                // std::cout << "No. of MCPoints in STT: " << mcTrack->GetNPoints(DetectorID::kSTT) << std::endl;
                
                // CSV:: Writting Info to CSV File. 
                fParticles  << std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1)   
                                                                       << ","   // track_id > 0
                            << (mcTrack->GetStartVertex()).X()         << ","   // vx = start x
                            << (mcTrack->GetStartVertex()).Y()         << ","   // vy = start y
                            << (mcTrack->GetStartVertex()).Z()         << ","   // vz = start z
                            << (mcTrack->GetMomentum()).X()            << ","   // px = x-component of track momentum
                            << (mcTrack->GetMomentum()).Y()            << ","   // py = y-component of track momentum
                            << (mcTrack->GetMomentum()).Z()            << ","   // pz = z-component of track momentum
                            << ((mcTrack->GetPdgCode()>0)?1:-1)        << ","   // q = charge of mu-/mu+
                            // <<mcTrack->GetNPoints(DetectorId::kSTT) << ","   // FIXME: nhits in STT (Not tested yet).
                            << 1                                       << ","   // FIXME: nhits==1 (just a placeholder)
                            << mcTrack->GetPdgCode()                   << ","   // pdgcode e.g. mu- has pdgcode=-13
                            << mcTrack->GetStartTime()                 << ","   // start_time = start time of particle track
                            << mcTrack->IsGeneratorCreated()                    // flag 'primary' particle
                            << std::endl;
                  
                delete (mcTrack);
                
            }//McTracks
        }//fAssistedByIdeal
    }//SttHitArray
    
}//end-GenerateSttData()


// GenerateSttSkewedData()
void PndTrackImport::GenerateSttSkewData() {
    
    std::cout << "-I- PndTrackImport: Runing GenerateSttSkewData()" << std::endl;
    
    // SttSkewHitArray
    if (fSttSkewHitArray->GetEntries()==0)
         std::cout << "Warning! SttSkewHitArray is Empty." << std::endl;
         
    for (int idx=0; idx < fSttSkewHitArray->GetEntries(); idx++) {

        // Get FairRootManager Instance
        FairRootManager *ioman = FairRootManager::Instance();

        // Get sttHitsLinks from STTHit
        FairMultiLinkedData_Interface *sttHitsLinks = (FairMultiLinkedData_Interface*)fSttSkewHitArray->At(idx);

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
        
        // Hit Counter (very important in case number of tracks vary per event)
        fHitId++;
        
        // Write to xxx-hits.csv
        // ---------------------------------------------------------------------------
        PndSttHit* stthit = (PndSttHit*)fSttSkewHitArray->At(idx);
        PndSttTube *tube = (PndSttTube*) fTubeArray->At(stthit->GetTubeID());
        
        fHits << fHitId                    << ","   // hit_id
              << stthit->GetX()            << ","   // x-position
              << stthit->GetY()            << ","   // y-position
              << stthit->GetZ()            << ","   // z-position
              //<< stthit->GetDetectorID() << ","   // volume_id (-1 for stt skewed layers)
              << (9)                       << ","   // volume_id (9 for stt)
              << tube->GetLayerID()        << ","   // layer_id
              << stthit->GetTubeID()                // tube_id/module_id
              << std::endl;
        
        
        // Write to xxx-truth.csv
        // ---------------------------------------------------------------------------
        
        // Get Particle Id
        TString particle_id = "";
        std::vector<FairLink> mcTrackLinks_sorted = sttHitsLinks->GetSortedMCTracks();
         
        for (unsigned int trackIndex = 0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
            
            // Append "particle_id" to xxx-truth.csv
            particle_id = std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);
        }
        
        fTruths << fHitId                  << ","   // hit_id  
                << sttpoint->GetX()        << ","   // tx = true x
                << sttpoint->GetY()        << ","   // ty = true y
                << sttpoint->GetZ()        << ","   // tz = true z
                << sttpoint->GetPx()       << ","   // tpx = true px
                << sttpoint->GetPy()       << ","   // tpy = true py
                << sttpoint->GetPz()       << ","   // tpz = true pz
                << (1.0)                   << ","   // FIXME: weight (sum of weights of hits in track == 1)
                << particle_id                      // Particle_id from above   
                << std::endl;    


        // Write to xxx-cells.csv
        // ---------------------------------------------------------------------------
        fCells  << fHitId                  << ","   // hit_id
                << stthit->GetDepCharge()  << ","   // deposited charge 
                << stthit->GetEnergyLoss() << ","   // energy loss (silicon)
                //<< stthit->GetDetectorID()<< ","  // volume_id (-1 for stt skewed layers)
                << (9)                     << ","   // volume_id (9 for stt)
                << tube->GetLayerID()      << ","   // layer_id
                << stthit->GetTubeID()     << ","   // module_id
                << tube->GetSectorID()     << ","   // sector_id
                << stthit->GetIsochrone()  << ","   // isochrone
                << tube->IsSkew()                   // skewed
                << std::endl;



        // Write to xxx-particles.csv
        // ---------------------------------------------------------------------------
        
        /** -----------------------------------------------------------------------
        * FIXME: The issue is fParticles should contain unique particle_ids (tracks)
        * e.g. if there are 10 particles/event then it should contain only 10 records.
        * But right now fParticles contain a particle records which are equal to the
        * size of fSttSkewHitArray. Which mean there will be repition of data in the file.
        * One solution might be to write the fParticles outside of fSttSkewHitArray. So I
        * shifted the this piece of code outside of fSttSkewHitArray loop. Note, however, 
        * that if MVD and GEM are also included then fParticles should be generated in 
        * Exec() rather than GenerateMvdData()/GenerateGemData() or GenerateSttData()
        * ------------------------------------------------------------------------/
        
        // NOTE: In particle.csv dataframe, count duplicate particle_ids and add this 
        // number as nhits. After that one can drop_duplicates from particle dataframe.
        

        //std::cout << "Number of MCTracks (fParticles): " << mcTrackLinks_sorted.size() << std::endl;
        //std::vector<FairLink> mcTrackLinks_sorted = sttHitsLinks->GetSortedMCTracks();
        
        if (fAssistedByIdeal.Contains("WithoutIdeal")) {
            for (unsigned int trackIndex=0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
            
                // Fetch mcTrack associated with SttHit
                PndMCTrack *mcTrack = (PndMCTrack*)ioman->GetCloneOfLinkData(mcTrackLinks_sorted[trackIndex]);
                
                // std::cout << "No. of MCPoints in STT: " << mcTrack->GetNPoints(DetectorID::kSTT) << std::endl;
                
                // CSV:: Writting Info to CSV File. 
                fParticles  << std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1)   
                                                                       << ","   // track_id > 0
                            << (mcTrack->GetStartVertex()).X()         << ","   // vx = start x
                            << (mcTrack->GetStartVertex()).Y()         << ","   // vy = start y
                            << (mcTrack->GetStartVertex()).Z()         << ","   // vz = start z
                            << (mcTrack->GetMomentum()).X()            << ","   // px = x-component of track momentum
                            << (mcTrack->GetMomentum()).Y()            << ","   // py = y-component of track momentum
                            << (mcTrack->GetMomentum()).Z()            << ","   // pz = z-component of track momentum
                            << ((mcTrack->GetPdgCode()>0)?1:-1)        << ","   // q = charge of mu-/mu+
                            // <<mcTrack->GetNPoints(DetectorId::kSTT) << ","   // FIXME: nhits in STT (Not tested yet).
                            << 1                                       << ","   // FIXME: nhits==1 (just a placeholder)
                            << mcTrack->GetPdgCode()                   << ","   // pdgcode e.g. mu- has pdgcode=-13
                            << mcTrack->GetStartTime()                 << ","   // start_time = start time of particle track
                            << mcTrack->IsGeneratorCreated()                    // flag 'primary' particle
                            << std::endl;
                  
                delete (mcTrack);
                
            }//McTracks
        }//fAssistedByIdeal
    }//SttHitArray
    
}//end-GenerateSttSkewedData()


// GenerateParticlesData()
void PndTrackImport::GenerateParticlesData() {
    
    // Write to xxx-particles.csv (Using IdealTrackFinder)
    // ---------------------------------------------------------------------------
    
    if (fAssistedByIdeal.Contains("WithIdeal")) {
        
        std::cout << "-I- PndTrackImport: Running GenerateParticlesData()" << std::endl;
        std::cout << "-I- PndTrackImport: Running IdealTrackFinder for Particles" << std::endl;
        
        FairMultiLinkedData linksMC, linksMVDPixel,linksMVDStrip,linksGEM,linksSTT;
        
        // Get FairRootManager Instance
        FairRootManager *ioman = FairRootManager::Instance();
        
        // Loop over ideal tracks i.e. BarrelTrackArray
        for (Int_t idx = 0; idx < fBarrelTrackArray->GetEntries(); idx++) { //loop over trackarray
                        
            // Fetch a PndTrack from the fBarrelTrackArray
            PndTrack *barrelTrack = (PndTrack *)fBarrelTrackArray->At(idx);
            
            // Create the links between the BarrelTrack and the MCTrack
            linksMC = barrelTrack->GetLinksWithType(ioman->GetBranchId("MCTrack")); 
            
            // Here, linksMC.GetNLinks()==1 always.
            if (linksMC.GetNLinks()>0) {
                for (Int_t i=0; i<linksMC.GetNLinks(); i++) {
                    if (linksMC.GetLink(i).GetIndex()==barrelTrack->GetTrackCand().getMcTrackId()) {
                        PndMCTrack *mcTrack = (PndMCTrack *)ioman->GetCloneOfLinkData(linksMC.GetLink(i));
                        
                        // Get Only Primary Tracks
                        //if (mcTrack->IsGeneratorCreated()) {

                            // Links of Primary Tracks
                            linksMVDPixel = barrelTrack->GetLinksWithType(ioman->GetBranchId("MVDHitsPixel"));
                            linksMVDStrip = barrelTrack->GetLinksWithType(ioman->GetBranchId("MVDHitsPixel"));
                            linksGEM = barrelTrack->GetLinksWithType(ioman->GetBranchId("GEMHit"));
                            linksSTT = barrelTrack->GetLinksWithType(ioman->GetBranchId("STTHit"));
                            
                            Int_t Nhits = (linksMVDPixel.GetNLinks()+linksMVDStrip.GetNLinks()+linksGEM.GetNLinks()+linksSTT.GetNLinks());
                            // If the number of STT hits greater than 0, write MC track to file!! if linksSTT.GetNLinks() > 0

                            // CSV:: Writting Info to CSV File.
                            fParticles  << (std::to_string(linksMC.GetLink(i).GetIndex() + 1)) << "," // track_id > 0
                                        << (mcTrack->GetStartVertex()).X() << ","   // vx = start x [cm, ns]
                                        << (mcTrack->GetStartVertex()).Y() << ","   // vy = start y [cm, ns]
                                        << (mcTrack->GetStartVertex()).Z() << ","   // vz = start z [cm, ns]
                                        << (mcTrack->GetMomentum()).X()    << ","   // px = x-component of track momentum
                                        << (mcTrack->GetMomentum()).Y()    << ","   // py = y-component of track momentum
                                        << (mcTrack->GetMomentum()).Z()    << ","   // pz = z-component of track momentum
                                        << ((mcTrack->GetPdgCode()>0)?1:-1)<< ","   // q = charge of mu-/mu+
                                        << Nhits                           << ","   // nhits in MVD+GEM+STT
                                        << mcTrack->GetPdgCode()           << ","   // pdgcode e.g. mu- has pdgcode=-13
                                        << mcTrack->GetStartTime()         << ","   // start_time = start time of particle track
                                        << mcTrack->IsGeneratorCreated()            // If a particle is primary or not
                                        << std::endl;
                                        
                           // }//end-IsGeneratorCreated()
                            
                        }//end-if(GetLink(i))
                        
                    }//end-for(GetNLinks)
                }//end-if(GetNLinks)
        }//end-for (barrelTrack)        
    }//particles by IdealTrackFinder
    
    else
        std::cout << "-I- PndTrackImport: Skipping IdealTrackFinder for Particles" << std::endl;

}//GenerateParticlesData


// Thanks to J. Regina for following functions.
int PndTrackImport::GetLayer(TString identifier)
{
  std::map<TString, int>::iterator layerIter;
  for (layerIter = fLayerMap.begin(); layerIter != fLayerMap.end(); layerIter++) {
    if (identifier.Contains(layerIter->first)) {
      return layerIter->second;
    }
  }
  return 0;
}

int PndTrackImport::GetLayerGem(FairHit *hit)
{
  PndGemHit *gemHit = (PndGemHit *)hit;

  TString prefix("Gem_");
  prefix += (gemHit->GetStationNr());
  prefix += ("_");
  prefix += (gemHit->GetSensorNr());

  return GetLayer(prefix);
}

int PndTrackImport::GetLayerMvd(FairHit *hit)
{
  PndSdsHit *tempHit = (PndSdsHit *)(hit);
  TString geoPath = fGeoH->GetPath(tempHit->GetSensorID());

  return GetLayer(geoPath);
}


void PndTrackImport::InitLayerMap()
{
  InitLayerMapMvd();
  InitLayerMapGem();
}

void PndTrackImport::InitLayerMapMvd()
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

void PndTrackImport::InitLayerMapGem()
{
  fLayerMap["Gem_1_1"] = fLastLayerId + 1;
  fLayerMap["Gem_1_2"] = fLastLayerId + 2;

  fLayerMap["Gem_2_1"] = fLastLayerId + 3;
  fLayerMap["Gem_2_2"] = fLastLayerId + 4;

  fLayerMap["Gem_3_1"] = fLastLayerId + 5;
  fLayerMap["Gem_3_2"] = fLastLayerId + 6;

  fLastLayerId += 6;
}

*/
