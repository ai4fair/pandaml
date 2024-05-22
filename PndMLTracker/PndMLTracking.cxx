/*
 * PndMLTracking.cxx
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
#include "PndMLTracking.h"

ClassImp(PndMLTracking)

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

/* PndMLTracking() */
PndMLTracking::PndMLTracking()
    : fHitId(0)
    , fEventId(0)
    , fCsvFilesPath("./data")
    , fAssistedByIdeal("NoIdealTracker")
    , fMCTrackBranchID(-1)
    , fMCTrackArray(nullptr)
    , fBarrelTrackBranchID(-1)      //formerly SttMvdGemTrack
    , fBarrelTrackArray(nullptr)    // formerly SttMvdGemTrack
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
    , fSttParameters(nullptr)
    , fEventHeader(nullptr)
    , fTubeArray(nullptr) 
    , fGeoH(nullptr)
    , fLayers()
    , fLayerMap()
    , fLastLayerId(0) {

    /* Constructor (1) */
}

/* PndMLTracking(int) */
PndMLTracking::PndMLTracking(int start_counter, TString csv_path, TString assist_by_ideal)
    : fHitId(0)
    , fEventId(start_counter)
    , fCsvFilesPath(csv_path)
    , fAssistedByIdeal(assist_by_ideal)
    , fMCTrackBranchID(-1)
    , fMCTrackArray(nullptr)
    , fBarrelTrackBranchID(-1)      // formerly SttMvdGemTrack
    , fBarrelTrackArray(nullptr)    // formerly SttMvdGemTrack
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
    , fSttParameters(nullptr)
    , fEventHeader(nullptr)
    , fTubeArray(nullptr) 
    , fGeoH(nullptr)
    , fLayers()
    , fLayerMap()
    , fLastLayerId(0) {

    /* Constructor (2) */
}

/* Destructor */
PndMLTracking::~PndMLTracking() {
}

/* SetParContainers() */
void PndMLTracking::SetParContainers() {

    FairRuntimeDb *rtdb = FairRunAna::Instance()->GetRuntimeDb();
    fSttParameters = (PndGeoSttPar*) rtdb->getContainer("PndGeoSttPar");

}

/* Init() */
InitStatus PndMLTracking::Init() {

    // Get instance of the FairRootManager to access tree branches
    FairRootManager *ioman = FairRootManager::Instance();
    if(!ioman) {
        std::cout << "-E- PndMLTracking::Init: FairRootManager not instantiated!" << std::endl;
        return kFATAL;
    }
    
    // Get the EventHeader
    fEventHeader = (TClonesArray*) ioman->GetObject("EventHeader.");
    if(!fEventHeader) {
        std::cout << "-E- PndMLTracking::Init: EventHeader not loaded!" << std::endl;
        return kFATAL;
    }
    
    // Access STTMapCreater
    PndSttMapCreator *mapper = new PndSttMapCreator(fSttParameters);
    fTubeArray = mapper->FillTubeArray();
    
    // MVD/GEM Layer Mapping
    fGeoH = PndGeoHandling::Instance();
    InitLayerMap();
    
    // Access MCTrack branch and its ID
    fMCTrackArray = (TClonesArray*) ioman->GetObject("MCTrack");
    fMCTrackBranchID = ioman->GetBranchId("MCTrack");
    
    if (!fMCTrackArray) {
        LOG(error) << " " << GetName() << "::Init: No MCTrack array!";
        return kERROR;
    } 
    
    // Access BarrelTrack (formerly SttMvdGemTrack) branch and its ID
    fBarrelTrackArray = (TClonesArray*) ioman->GetObject("BarrelTrack");
    fBarrelTrackBranchID = ioman->GetBranchId("BarrelTrack");
    
    /*
    if (!fBarrelTrackArray) {
        LOG(error) << " " << GetName() << "::Init: No BarrelTrack array!";
        return kERROR;
    } 
    */
    
    //Access MVDPoint, MVDHitsPixel, MvdHitsStrip branches and their Ids
    fMvdPointArray = (TClonesArray*) ioman->GetObject("MVDPoint");
    fMvdPointBranchID = ioman->GetBranchId("MVDPoint");
    
    fMvdHitsPixelArray = (TClonesArray*) ioman->GetObject("MVDHitsPixel");
    fMvdHitsPixelBranchID = ioman->GetBranchId("MVDHitsPixel");
    
    fMvdHitsStripArray = (TClonesArray*) ioman->GetObject("MVDHitsStrip");
    fMvdHitsStripBranchID = ioman->GetBranchId("MVDHitsStrip");
    
    //Access GEMPoint, GEMHit branches and their Ids
    fGemPointArray = (TClonesArray*) ioman->GetObject("GEMPoint");
    fGemPointBranchID = ioman->GetBranchId("GEMPoint");
    fGemHitArray = (TClonesArray*) ioman->GetObject("GEMHit");
    fGemHitBranchID = ioman->GetBranchId("GEMHit");
    
    //Access STTPoint, STTHit branches and their Ids
    fSttPointArray = (TClonesArray*) ioman->GetObject("STTPoint");
    fSttPointBranchID = ioman->GetBranchId("STTPoint");
    
    fSttHitArray = (TClonesArray*) ioman->GetObject("STTHit");
    fSttHitBranchID = ioman->GetBranchId("STTHit");
    
    fSttSkewHitArray = (TClonesArray*) ioman->GetObject("STTCombinedSkewedHits");
    fSttSkewHitBranchID = ioman->GetBranchId("STTCombinedSkewedHits");

    std::cout << "-I- PndMLTracking: Initialisation successful" << std::endl;
    return kSUCCESS;

}

/* Exec() */
void PndMLTracking::Exec(Option_t* /*opt*/) {
    
    // Debugging Tasks    
    // 1 - Get STTPoint connected to STTHit
    // 2 - Get STTPoint connected to MCTrack
    // 3 - Filter STTPoints/STTHits if IsGeneratorCreated()
    // 4 - Get TrackID/MCTrackID of STTPoint/STTHit
    
    // Set Event Index
    std::stringstream ss;
    ss << std::setw(10) << std::setfill('0') << fEventId;
    std::string fidx = ss.str();
    
    /* ***********************************************************************
    *                          Open CSV Files
    *  ********************************************************************* */
    
    TString csv_path = fCsvFilesPath+"/event"+fidx;
    
    fHits.open(csv_path+"-hits.csv");
    fTruths.open(csv_path+"-truth.csv");
    fParticles.open(csv_path+"-particles.csv");
    fCells.open(csv_path+"-cells.csv");
    
    std::cout << "\n-I- Processing Event: " << fEventId << std::endl;
    
    /* ***********************************************************************
    *                          Add CSV Header
    *  ********************************************************************* */

    // TODO: Add TClonesArray Indices, as fCIdx, in the CSVs along with fHitId.
    // fCIdx is the exact location of a hit one can use to build PndTrackCand.
        
    /* ------------------------------------------------------------------------
    *                          (1) Event Hits
    *  --------------------------------------------------------------------- */

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    *
    * The hits file contains the following values for each hit/entry:
    *
    *  - hit_id : numerical identifier of the hit inside the event.
    *  - x, y, z : measured x, y, z position (in mm) of the hit in global coordinates.
    *  - volume_id : numerical identifier of the detector group.
    *  - layer_id : numerical identifier of the detector layer inside the group.
    *  - module_id : numerical identifier of the detector module inside the layer.
    *
    * The volume/layer/module id could in principle be deduced from x, y, z. They are 
    * given here to simplify detector-specific data handling.
    *
    * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    fHits   << "hit_id" << ","
            << "x" << "," 
            << "y" << "," 
            << "z" << ","
            << "volume_id" << ","       // e.g. STT (sub-detectors)
            << "layer_id"  << ","       // e.g. layer_id in STT
            << "module_id" << ","       // e.g. module_id==tube_id
            << "tclone_id"              // e.g. TCloneArray Index
            //<< "fair_link"            // e.g. FairLinks= sttHitsLinks
            << std::endl;
    
    /* ------------------------------------------------------------------------
    *                          (2) Event Truths
    *  --------------------------------------------------------------------- */
    
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    *
    * The truth file contains the mapping between hits and generating particles 
    * and the true particle state at each measured hit. Each entry maps one hit 
    * to one particle.
    *
    * - hit_id: numerical identifier of the hit as defined in the hits file.
    * - particle_id: numerical identifier of the generating particle as defined 
    *                in the particles file. A value of 0 means that the hit did 
    *                not originate from a reconstructible 
    *                particle, but e.g. from detector noise.
    *
    * - tx, ty, tz: true intersection point in global coordinates (in mm)
    *               between the particle trajectory and the sensitive surface.
    *
    * - tpx,tpy,tpz: true particle momentum (in GeV/c) in the global coordinate 
    *                system at the intersection point. The corresponding vector 
    *               is tangent to the particle trajectory at intersection point.
    *
    * - weight: per-hit weight used for the scoring metric; total sum of weights 
    *           within one event equals to one.
    * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    
    fTruths << "hit_id" << ","
            << "tx"  << ","
            << "ty"  << ","
            << "tz"  << ","
            << "tpx" << ","
            << "tpy" << ","
            << "tpz" << ","
            << "weight" << ","
            << "particle_id"
            << std::endl;
    
    /* ------------------------------------------------------------------------
    *                          (3) Event Tubes/Cells/Sensors
    *  --------------------------------------------------------------------- */
    
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    *
    * - hit_id: numerical identifier of the hit as defined in the hits file.
    * - isochrone:
    * - depcharge:
    * - energyloss:
    * - volume_id, layer_id, module_id
    * - skewed, sector_id
    * 
    * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    // Future Add: GetHalfLength(), GetWireDirection(), GetRadIn(), GetRadOut(),
    //             GetNeighborings(), GetDistance(), IsParallel()    
    fCells  << "hit_id"    << ","       // HitId
            << "depcharge" << ","       // Deposited charge
            << "energyloss"<< ","       // Energy loss
            << "volume_id" << ","       // Detector Identifier
            << "layer_id"  << ","       // Layer inside a detector
            << "module_id" << ","       // Module Id: MVD Sensor/GEM Sensor/Straw Tube
            << "sector_id" << ","       // sector of tube (only for STT)
            << "isochrone" << ","       // isochrone radius (only for STT)
            << "skewed"                 // if tube is skewed (only for STT)
            << std::endl;
            
    /* ------------------------------------------------------------------------
    *                          (4) Event Particles
    *  --------------------------------------------------------------------- */
    
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    *
    * The particles files contains the following values for each particle/entry:
    * 
    * - particle_id: numerical identifier of the particle inside the event.
    * - particle_type: numerical identifier of the particle type (PDG CODE)
    * - vx, vy, vz: initial position or vertex (in mm) in global coordinates.
    * - px, py, pz: initial momentum (in GeV/c) along each global axis.
    * - q: particle charge (as multiple of the absolute electron charge).
    * - nhits: number of hits generated by this particle.
    * 
    * All entries contain the generated information or ground truth.
    * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    
    fParticles  << "particle_id" <<","  // identifier of a track (track_id)
                << "vx" << ","
                << "vy" << ","
                << "vz" << ","
                << "px" << ","
                << "py" << ","
                << "pz" << ","
                << "q"  << ","
                << "nhits" << ","
                << "pdgcode" << ","
                << "start_time" << ","
                << "primary"
                << std::endl;
    
    /* ************************************************************************
    *                       Add CSV Data according to Header
    *  ********************************************************************* */
    
    //GenerateMvdPixelData();
    //GenerateMvdStripData();
    //GenerateGemData();
    
    GenerateSttData();
    //GenerateSttSkewData();
    
    GenerateParticlesData();
    
    //Close CSVs
    fHits.close();
    fTruths.close();
    fParticles.close();
    fCells.close();
    
    std::cout << "-I- Finishing Event: " << (fEventId) << " with Hits: " << fHitId << std::endl;
    
    //Reset Counters
    fHitId = 0;
    fEventId++;

}//end-Exec()


/* GenerateSttData() */
void PndMLTracking::GenerateSttData() {
    
    std::cout << "-I- Runing GenerateSttData() with SttHitArray Size: " 
              << fSttHitArray->GetEntries() << std::endl;
    
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
        //if (!mctrack->IsGeneratorCreated())   // BoxGen: muons, doesn't work for Lambdas
        //if (!mctrack->IsGeneratorLast())      // set for llbar_fwp.dec
        //    continue;
        
        // Get STTHit
        PndSttHit* stthit = (PndSttHit*)fSttHitArray->At(idx);
        PndSttTube *tube = (PndSttTube*) fTubeArray->At(stthit->GetTubeID());
        
        // Remove Skewed Hits
        // if (tube->IsSkew()) {continue;}
        
        // Hit Counter (Always Start from 1 to N)
        fHitId++;
        
        // See if HitId, SttHitArray Id
        // std::cout << "SttHitArray Id: " << idx << " fHitId: " << fHitId << std::endl;
        
        // Write to xxx-hits.csv
        // ---------------------------------------------------------------------------        
        fHits << fHitId                    << ","   // hit_id
              << stthit->GetX()            << ","   // x-position
              << stthit->GetY()            << ","   // y-position
              << stthit->GetZ()            << ","   // z-position
              << stthit->GetDetectorID()   << ","   // volume_id
              << tube->GetLayerID()        << ","   // layer_id
              << stthit->GetTubeID()       << ","   // tube_id/module_id
              << idx                                // TCloneArray Index
              //<< sttHitsLinks                       // or sttHitsLinks
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
                << (1.0)                   << ","   // Weight placeholder (sum of weights of hits in track == 1)
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
        * ---------------------------------------------------------------------- */
        
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


/* GenerateParticlesData() */
void PndMLTracking::GenerateParticlesData() {
    
    // Write to xxx-particles.csv (Using IdealTrackFinder)
    // ---------------------------------------------------------------------------
    
    if (fAssistedByIdeal.Contains("WithIdeal")) {
        
        std::cout << "-I- Running GenerateParticlesData()" << std::endl;
        //std::cout << "-I- Running IdealTrackFinder for fParticles" << std::endl;
        
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
                        if (mcTrack->IsGeneratorCreated()) {     // box generator: muons
                        //if (mcTrack->IsGeneratorLast()) {      // llbar_fwp.dec

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
                                        << mcTrack->IsGeneratorDecayed()            // If a particle is primary or not
                                        << std::endl;
                                        
                           }//end-IsGeneratorCreated()
                            
                        }//end-if(GetLink(i))
                        
                    }//end-for(GetNLinks)
                }//end-if(GetNLinks)
        }//end-for (barrelTrack)        
    }//particles by IdealTrackFinder
    
    else
        std::cout << "-I- Skipping IdealTrackFinder for Particles" << std::endl;

}//GenerateParticlesData

















/* GenerateSttSkewedData() */
void PndMLTracking::GenerateSttSkewData() {
    
    std::cout << "-I- PndMLTracking: Runing GenerateSttSkewData()" << std::endl;
    
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
              << stthit->GetTubeID()       << ","   // tube_id/module_id
              << idx                       << ","   // TCloneArray Index
              << sttHitsLinks                       // or sttHitsLinks
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
                << (1.0)                   << ","   // Weight placeholder (sum of weights of hits in track == 1)
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
        * ---------------------------------------------------------------------- */
        
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


/* GenerateMvdPixelData() */
void PndMLTracking::GenerateMvdPixelData() { 
    
    std::cout << "-I- PndMLTracking: Runing GenerateMvdPixelData()" << std::endl;
    
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
              << sdsHit->GetSensorID()     << ","   // sensor_id/module_id
              << idx                       << ","   // TCloneArray Index
              << mvdHitsLinks                       // or mvdHitsLinks
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
                << (1.0)                   << ","   // Weight placeholder (sum of weights of hits in track == 1)
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


void PndMLTracking::GenerateMvdStripData() { 
    
    std::cout << "-I- PndMLTracking: Runing GenerateMvdStripData()" << std::endl;   
    
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
              << sdsHit->GetSensorID()     << ","   // sensor_id/module_id
              << idx                       << ","   // TCloneArray Index
              << mvdHitsLinks                       // or mvdHitsLinks
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
                << (1.0)                   << ","   // Weight placeholder (sum of weights of hits in track == 1)
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


/* GenerateGemData() */
void PndMLTracking::GenerateGemData() { 
    
    std::cout << "-I- PndMLTracking: Runing GenerateGemData()" << std::endl;
    
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
        //if (!mctrack->IsGeneratorCreated())
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
              << gemHit->GetSensorNr()     << ","   // sensor_id/module_id
              << idx                       << ","   // TCloneArray Index
              << gemHitsLinks                       // or mvdHitsLinks
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
                << (1.0)                   << ","   // Weight placeholder (sum of weights of hits in track == 1)
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


/* FinishTask() */
void PndMLTracking::FinishTask() {
    
    // Close all files
    fHits.close();
    fTruths.close();
    fParticles.close();
    fCells.close();
        
    std::cout << "\n-I- Task Generating CSVs has Finished." << std::endl;
}


/* GetFairMCPoint() */
FairMCPoint* PndMLTracking::GetFairMCPoint(TString fBranchName, FairMultiLinkedData_Interface* links, FairMultiLinkedData& array) {
    
    // get the mc point(s) from each reco hit
    FairMultiLinkedData mcpoints = links->GetLinksWithType(FairRootManager::Instance()->GetBranchId(fBranchName));
    array = mcpoints;
    if (array.GetNLinks() == 0){
        return 0;
    }
    return (FairMCPoint*) FairRootManager::Instance()->GetCloneOfLinkData(array.GetLink(0));
}

// Thanks to J. Regina for following functions.
int PndMLTracking::GetLayer(TString identifier)
{
  std::map<TString, int>::iterator layerIter;
  for (layerIter = fLayerMap.begin(); layerIter != fLayerMap.end(); layerIter++) {
    if (identifier.Contains(layerIter->first)) {
      return layerIter->second;
    }
  }
  return 0;
}

int PndMLTracking::GetLayerGem(FairHit *hit)
{
  PndGemHit *gemHit = (PndGemHit *)hit;

  TString prefix("Gem_");
  prefix += (gemHit->GetStationNr());
  prefix += ("_");
  prefix += (gemHit->GetSensorNr());

  return GetLayer(prefix);
}

int PndMLTracking::GetLayerMvd(FairHit *hit)
{
  PndSdsHit *tempHit = (PndSdsHit *)(hit);
  TString geoPath = fGeoH->GetPath(tempHit->GetSensorID());

  return GetLayer(geoPath);
}


void PndMLTracking::InitLayerMap()
{
  InitLayerMapMvd();
  InitLayerMapGem();
}

void PndMLTracking::InitLayerMapMvd()
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

void PndMLTracking::InitLayerMapGem()
{
  fLayerMap["Gem_1_1"] = fLastLayerId + 1;
  fLayerMap["Gem_1_2"] = fLastLayerId + 2;

  fLayerMap["Gem_2_1"] = fLastLayerId + 3;
  fLayerMap["Gem_2_2"] = fLastLayerId + 4;

  fLayerMap["Gem_3_1"] = fLastLayerId + 5;
  fLayerMap["Gem_3_2"] = fLastLayerId + 6;

  fLastLayerId += 6;
}





















/* OLD Code (Kept for Reference) */
/*
// GenerateData()
void PndMLTracking::GenerateData() {
    
    // Event Files
    std::stringstream ss;
    ss << std::setw(10) << std::setfill('0') << fEventId;
    std::string fidx = ss.str();
    //std::cout << "Processing Event: " << "event"+fidx << std::endl;

    // Open Files (CSV)
    fHits.open(fCsvFilesPath+"event"+fidx+"-hits.csv");
    fTruths.open(fCsvFilesPath+"event"+fidx+"-truth.csv");
    fParticles.open(fCsvFilesPath+"event"+fidx+"-particles.csv");
    fTubes.open(fCsvFilesPath+"event"+fidx+"-tubes.csv");

    // ////////////////////////////////////////////////////////////////////
    //                            Add Header                             //
    // ////////////////////////////////////////////////////////////////////
    // Hit Positions
    fHits << "hit_id" <<","
          << "x" << "," 
          << "y" << "," 
          << "z" << ","
          << "tube_id" << "," 
          << "skewed" << "," 
          << "layer_id" << "," 
          << "sector_id" << "," 
          << "volume_id" << "," 
          << "isochrone" << "," 
          << "depcharge" << "," 
          << "energyloss" << "," 
          << "particle_id" 
          <<std::endl;

    // MC Truth
    fTruths << "hit_id" <<","
            << "tx" << ","
            << "ty" << ","
            << "tz" << ","
            << "tpx"<< ","
            << "tpy"<< ","
            << "tpz"<< ","
            // << "weight" << ","
            << "particle_id"
            << std::endl;

    // Particle initial position/vertex(mm), momentum(GeV/c) and charge (q)
    fParticles << "particle_id" <<","
               << "vx" << ","
               << "vy" << ","
               << "vz" << ","
               << "px" << ","
               << "py" << ","
               << "pz" << ","
               << "q"  << ","
               // << "nhits" << ","
               << "pdg_code" << ","
               << "start_time"
               << std::endl;
               
    // Tube Parameters
    // Future Add: GetHalfLength(), GetWireDirection(), GetRadIn(), GetRadOut(),
    //             GetNeighborings(), GetDistance(), IsParallel()
    // Remove Tube Parameters  (redundent w.r.t fHits)
    fTubes << "hit_id"    << ","
           << "tube_id"   << "," 
           << "skewed"    << "," 
           << "layer_id"  << "," 
           << "sector_id" << "," 
           << "volume_id" << "," 
           << "isochrone" << "," 
           << "depcharge" << "," 
           << "energyloss"
           <<std::endl;

    // ////////////////////////////////////////////////////////////////////
    //                            Event Loop
    // ////////////////////////////////////////////////////////////////////
    int hit_id = 0;
    for (int hit_idx = 0; hit_idx < fSttHitArray->GetEntriesFast(); hit_idx++) {

        // Get FairRootManager Instance
        FairRootManager *ioman = FairRootManager::Instance();

        // Get sttHitsLinks from STTHit
        FairMultiLinkedData_Interface *sttHitsLinks = (FairMultiLinkedData_Interface*)fSttHitArray->At(hit_idx);

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
        if (!mctrack->IsGeneratorCreated())
            continue;
        
        // Hit Counter (very important in case number of tracks vary per event)
        hit_id++;
        
        // --------------------------------------------------------------------
        //                            MCPoints/fTruths
        // --------------------------------------------------------------------
        fTruths << hit_id << ","            // hit_id=0 is for interaction vertex
                << sttpoint->GetX()    << ","
                << sttpoint->GetY() << ","
                  << sttpoint->GetZ() << ","
                  << sttpoint->GetPx()<< ","
                  << sttpoint->GetPy()<< ","
                  << sttpoint->GetPz()<< ",";

        // --------------------------------------------------------------------
        //                            STTHists/fHits
        // --------------------------------------------------------------------
        // First Get access to SttHit & PndSttTube
        PndSttHit* stthit = (PndSttHit*)fSttHitArray->At(hit_idx);
        PndSttTube *tube = (PndSttTube*) fTubeArray->At(stthit->GetTubeID());
        fHits << hit_id                 <<","
              << stthit->GetX()         << "," 
              << stthit->GetY()         << "," 
              << stthit->GetZ()         << ","
              << stthit->GetTubeID()    << "," 
              << tube->IsSkew()         << "," 
              << tube->GetLayerID()     << ","
              << tube->GetSectorID()    << "," 
              << stthit->GetDetectorID()<< "," 
              << stthit->GetIsochrone() << "," 
              << stthit->GetDepCharge() << "," 
              << stthit->GetEnergyLoss()<< ",";

        // --------------------------------------------------------------------
        //                            PndTube/fTubes
        // --------------------------------------------------------------------
        // Future Add: GetHalfLength(), GetWireDirection(), GetRadIn(), GetRadOut(),
        //             GetNeighborings(), GetDistance(), IsParallel()
        // Remove Tube Parameters  (redundent w.r.t fHits)
        fTubes<< hit_id                 << ","
              << stthit->GetTubeID()    << ","
              << tube->IsSkew()         << ","
              << tube->GetLayerID()     << ","
              << tube->GetSectorID()    << ","
              << stthit->GetDetectorID()<< "," 
              << stthit->GetIsochrone() << ","
              << stthit->GetDepCharge() << "," 
              << stthit->GetEnergyLoss();

        // Particle/Track initial position/vertex(mm), momentum(GeV/c) and charge (q)
        
        std::vector<FairLink> mcTrackLinks_sorted = sttHitsLinks->GetSortedMCTracks();
        for (unsigned int trackIndex = 0; trackIndex < mcTrackLinks_sorted.size(); trackIndex++) {
            
            // 
            PndMCTrack *myTrack = (PndMCTrack*)ioman->GetCloneOfLinkData(mcTrackLinks_sorted[trackIndex]);
            
            // Get Track Index of MCTrack >> fHits            
            fHits     << std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);      // particle_id into hits.csv
            fTruths << std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);        // particle_id into truths.csv
            
            // ----------------------------------------------------------------
            //                            fParticles
            // ----------------------------------------------------------------              
            fParticles  << std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1) << ","
                        << (myTrack->GetStartVertex()).X() << ","
                        << (myTrack->GetStartVertex()).Y() << ","
                        << (myTrack->GetStartVertex()).Z() << ","
                        << (myTrack->GetMomentum()).X() << ","
                        << (myTrack->GetMomentum()).Y() << ","
                        << (myTrack->GetMomentum()).Z() << ","
                        << (-1) << ","                                   // muon
                        << myTrack->GetPdgCode() << ","
                        << myTrack->GetStartTime();
                                  
            delete (myTrack);
        }
        
        // Add Line Breaks
        fHits << std::endl;
        fTruths << std::endl;
        fParticles << std::endl;
        fTubes << std::endl;
        
    }//end-for
    
    // Close Files
    fHits.close();
    fTruths.close();
    fParticles.close();
    fTubes.close();

    fEventId++;
}

// GenerateCSV()
void PndMLTracking::GenerateCSV() {
    
    // Call inside Exec (Waleed)
    InFile1.open (fCsvFilesPath+"box_detector_"+std::to_string(fEventId)+".csv");
    InFile2.open (fCsvFilesPath+"box_real_"+std::to_string(fEventId)+".csv");
    InFile3.open (fCsvFilesPath+"box_mctruth_"+std::to_string(fEventId)+".csv");

    InFile1 << "hit_id" <<","<< "x" <<","<< "y" <<","<< "z"<<","
            << "tube_id" <<","<< "skewed"<<","<< "layer_id"<<","<< "sector_id"<<","
            << "isochrone" <<","<< "depcharge"<<","<< "particle_id"<<std::endl;
           
    InFile2 << "hit_id" <<","<< "tx" <<","<< "ty" <<","<< "tz" <<","<< "px" <<","<< "py"<<","<<"pz"<<std::endl;
    InFile3 << "hit_id" <<","<< "pdgcode" <<","<< "vertexX" <<","<< "vertexY" <<","<< "vertexZ" <<","<< "vertexT"<<std::endl;

    for (int i = 0; i < fSttHitArray->GetEntriesFast(); i++)
    {
        fHitId++;
        
        // Get access to MCPoint >> STTPoint using FairMultiLinkedData_Interface
        FairMultiLinkedData_Interface* myData = (FairMultiLinkedData_Interface*)fSttHitArray->At(i);
        FairMultiLinkedData array;
        FairMCPoint *sttpoint = GetFairMCPoint("STTPoint", myData, array);
        if (sttpoint == 0) {continue;}
        
        InFile2 << fHitId  <<","
                << sttpoint->GetX() <<","
                << sttpoint->GetY() <<","
                  << sttpoint->GetZ() <<","
                  << sttpoint->GetPx() <<","
                  << sttpoint->GetPy() <<","
                  << sttpoint->GetPz();
        
        // Get access to SttHit & PndSttTube
        PndSttHit* stthit = (PndSttHit*)fSttHitArray->At(i);
        PndSttTube *tube = (PndSttTube*) fTubeArray->At(stthit->GetTubeID());
        
        InFile1 << fHitId <<","
                << stthit->GetX() <<","
                << stthit->GetY() <<","
                << stthit->GetZ() <<","
                << stthit->GetTubeID() <<","
                << tube->IsSkew() << ","
                << tube->GetLayerID() << ","
                << tube->GetSectorID() << ","
                << stthit->GetIsochrone() <<","
                << stthit->GetDepCharge() <<",";
        
        std::vector<FairLink> myLinks = myData->GetSortedMCTracks();
        for (unsigned int trackIndex = 0; trackIndex < myLinks.size(); trackIndex++)
        {
            PndMCTrack* myTrack = (PndMCTrack*)FairRootManager::Instance()->GetCloneOfLinkData(myLinks[trackIndex]);
            
            // Get Track Index of MCTrack >> InFile1                
            InFile1 << std::to_string(fEventId)+std::to_string(myLinks[trackIndex].GetIndex());
            
            InFile3 << fHitId <<","
                    <<myTrack->GetPdgCode()<<","
                    <<(myTrack->GetMomentum()).X()<<"," //(myTrack->GetStartVertex()).X()
                    <<(myTrack->GetMomentum()).Y()<<"," //(myTrack->GetStartVertex()).Y()
                    <<(myTrack->GetMomentum()).Z()<<"," //(myTrack->GetStartVertex()).Z()
                    <<myTrack->GetStartTime();
                                         
            delete (myTrack);
        }
        
        InFile1<<std::endl;
        InFile2<<std::endl;
        InFile3<<std::endl;
        
    }//end-for
    
    InFile1.close();
    InFile2.close();
    InFile3.close();
    fEventId++;
}//end-GenerateCSV()
*/

