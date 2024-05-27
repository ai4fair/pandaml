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

#include <ROOT/RDataFrame.hxx>
#include <ROOT/RCsvDS.hxx>

#include "PndTrackImport.h"


ClassImp(PndTrackImport)

PndTrackImport::PndTrackImport() : PndPersistencyTask("Barrel Track Finder", 1) {
}

PndTrackImport::PndTrackImport(int start_counter, TString csv_path) 
    : PndPersistencyTask("Barrel Track Finder", 1)
    , fEventId(start_counter)
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
    , fSttParameters(nullptr)
    , fEventHeader(nullptr)
    , fTubeArray(nullptr)
    , f(nullptr)
    , t(nullptr)
    //, fSttTrackArray(nullptr)
    , fSttTrackCandArray(nullptr)
    {

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
    
    //Access STTPoint, STTHit Branches and their Ids
    fSttPointArray = (TClonesArray*) ioman->GetObject("STTPoint");
    fSttPointBranchID = ioman->GetBranchId("STTPoint");
    
    fSttHitArray = (TClonesArray*) ioman->GetObject("STTHit");
    fSttHitBranchID = ioman->GetBranchId("STTHit");
    
    // Set Persistency
    SetPersistency(kTRUE);
    
    // Read TrackML Tree
    TString filename = fCsvFilesPath+"/trackml.root";
    f = TFile::Open(filename);
    t = f->Get<TTree>("TrackML");
    max_size = dynamic_cast<TLeaf*>(t->GetBranch("nml")->GetListOfLeaves()->First())->GetMaximum();
    t->SetBranchAddress("ml_hit_id", &hit_id);
    t->SetBranchAddress("ml_track_id", &track_id);
    t->SetBranchAddress("nml", &n);
       
    fSttTrackCandArray = new TClonesArray("PndTrackCand", 100);
    ioman->Register("SttTrackCand", "STT TrackCand", fSttTrackCandArray, GetPersistency());
    
    std::cout << "-I- PndTrackImport: Initialisation successful" << std::endl;
    return kSUCCESS;

}

/* Exec() */
void PndTrackImport::Exec(Option_t* /*opt*/) {
    
    fSttTrackCandArray->Delete();
    
    // Event Number
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << fEventId;
    std::string fidx = ss.str();
    std::cout << "\nProcessing Event: " << (fEventId) << std::endl;
    
    t->GetEntry(fEventId);
    std::map<int, std::vector<int> > map_track_cands;
    std::map<int, PndTrackCand*> map_tcands;
    
    // Create PndTrackCand Map
    for (int i = 0; i < n; i++) {
    
        // cout << setw(3) << hit_id[i] << ", "  << setw(3) << track_id[i] << endl;
        map_track_cands[track_id[i]].push_back(hit_id[i]);
        
        // OR, PndTrackCands Map
        if (map_tcands[track_id[i]]==nullptr) {
            map_tcands[track_id[i]] = new PndTrackCand();
            
            // Issue here is somewhere -1 is fed in that gives seg-fault
            // map_tcands[track_id[i]] = new ((*fSttTrackCandArray)[i]) PndTrackCand();
        }
        
        PndTrackCand* myTCand = map_tcands[track_id[i]];

        // get hit pointer from hitID & Pandaroot 
        int idx = hit_id[i] - 1;
        Double_t rho = (double)hit_id[i];       
        
        //FIXME: issue with adding FairLink to PndTrackCand
        PndSttHit* stthit = (PndSttHit*)fSttHitArray->At(idx);
        

        std::cout << "FairLink to TrackCand (2):" << FairLink(-1, -1, fSttHitBranchID, idx) << std::endl;
        std::cout << "FairLink to TrackCand (2):" << FairLink(-1, FairRootManager::Instance()->GetEntryNr(), fSttHitBranchID, idx) << std::endl;
        
        FairLink link(-1, FairRootManager::Instance()->GetEntryNr(), fSttHitBranchID, idx);
        myTCand->SetInsertHistory(kTRUE);
        myTCand->AddHit(link, rho); 
        
    }//end
    
    
    // Display Map
	std::cout << "\nTotal TrackML Track Cands: " << map_track_cands.size() << std::endl;
	for (auto const &iter : map_track_cands) {
    	std::cout << iter.first << ':';
    	for(auto const& iter1: iter.second) 
        	std::cout << ' ' << iter1;
        cout << endl;
    }
    
    
    // Write PndTrackCand to fSttTrackCandArray
    std::cout << "Total PndTrackCand Per Event: " << map_tcands.size() << std::endl;
    int index =0;
	for (auto const &iter : map_tcands) {
   	
    	PndTrackCand* tcand = (PndTrackCand*)iter.second;
        if (iter.first >= 0) // -1: unassigned hits.
            new ((*fSttTrackCandArray)[index]) PndTrackCand(*tcand);
    
    }//end
    
    
    // Clear Objects
    map_track_cands.clear();
    map_tcands.clear();

    cout << "fSttHitArray Size : " << fSttHitArray->GetEntries() << endl;
    cout << "Number of Reco Hit: " << n << endl;
    fEventId++;

}//end-Exec()


/* FinishTask() */
void PndTrackImport::FinishTask() {
    
    // Finishing FairTask   
    std::cout << "\n-I- Total Number of Events Processed:" << fEventId << std::endl;
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

