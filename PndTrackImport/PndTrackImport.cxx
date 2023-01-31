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
PndTrackImport::PndTrackImport() : PndPersistencyTask("Barrel Track Finder", 1) {
}

PndTrackImport::PndTrackImport(int start_counter, TString csv_path) 
    : PndPersistencyTask("Barrel Track Finder", 1)
    , fEventId(start_counter)
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
    , f(nullptr)
    , t(nullptr)
    , fSttTrackArray(nullptr)
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
    
    SetPersistency(kTRUE);
    
    // Read TrackML Tree
    TString filename = fCsvFilesPath+"/trackml.root";
    f = TFile::Open(filename);
    t = f->Get<TTree>("TrackML");
    max_size = dynamic_cast<TLeaf*>(t->GetBranch("nml")->GetListOfLeaves()->First())->GetMaximum();
    t->SetBranchAddress("ml_hit_id", &hit_id);
    t->SetBranchAddress("ml_track_id", &track_id);
    t->SetBranchAddress("nml", &n);
    
    // trackML->SetBranchAddress("nTrackCand", &hit_id);
    // trackML->SetBranchAddress("TrackCand", &track_id);

    // Create and register PndTrack and PndTrackCand arrays
    fSttTrackArray = new TClonesArray("PndTrack", 100);
    ioman->Register("SttTrack", "STT Track", fSttTrackArray, GetPersistency());
    
    fSttTrackCandArray = new TClonesArray("PndTrackCand", 100);
    ioman->Register("SttTrackCand", "STT TrackCand", fSttTrackCandArray, GetPersistency());
    
    std::cout << "-I- PndTrackImport: Initialisation successful" << std::endl;
    return kSUCCESS;

}

/* Exec() */
void PndTrackImport::Exec(Option_t* /*opt*/) {
    
    fSttTrackCandArray->Delete();
    
    // Filename of CSV
    /*
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << fEventId;
    std::string fidx = ss.str();   
    TString prefix = fCsvFilesPath+"/TrackML/"+fidx;
    TString filename = fCsvFilesPath+"/trackml.root";
    std::cout << "\nPrefix: " << (prefix) << std::endl;
    std::cout << "Filename: " << (filename) << std::endl;
    std::cout << "Processing Event: " << (fEventId) << std::endl;
    
    // RDF from CSVs
    // auto rdf = ROOT::RDF::FromCSV(filename);             // Root v6.26    
    // auto rdf = ROOT::RDF::MakeCsvDataFrame(filename);    // Root v6.22
    
    // TTree from CSVs
    // TTree *t = new TTree("t", "Track Cand");
    // t->ReadFile(filename, "hit_id/I:track_id");
    
    // TTree from ROOT
    // TFile *myfile = TFile::Open(filename);
    // TTree *t = myfile->Get<TTree>("TrackML");
    */
    
    // We start from fEventId=0, as Event Counter. Then we 
    // get the entries of event 'fEventId' for all branches
    t->GetEntry(fEventId);
    
    // hit_id, track_id are jagged array, with size 'n'. We need
    // to loop over to read hits and tracks for a particular event.
    
    
    
    std::map<int, std::vector<int> > map_track_cands;
    std::map<int, PndTrackCand*> mycands;
    
     
    for (int i = 0; i < n; i++) {
    
        // cout << setw(3) << hit_id[i] << ", "  << setw(3) << track_id[i] << endl;
        map_track_cands[track_id[i]].push_back(hit_id[i]);
        
        // OR, PndTrackCands Map
        if (mycands[track_id[i]]==nullptr) {
            mycands[track_id[i]] = new PndTrackCand();
            
            // Issue here is somewhere -1 is fed in that gives seg-fault
            // mycands[track_id[i]] = new ((*fSttTrackCandArray)[i]) PndTrackCand();
        }
        
        PndTrackCand* myTCand = mycands[track_id[i]];

        // // get hit pointer from hitID & Pandaroot 
        int idx = hit_id[i] - 1;                       // idx =  hit_id - 1
        
        // PndSttHit* stthit = (PndSttHit*)fSttHitArray->At(idx);
        
        // Hit id is increasing number, remain valid if a track curls back to IP.
        // It seemed to be fair choice for the rho parameter.
        myTCand->AddHit(FairRootManager::Instance()->GetBranchId("STTHit"), idx, hit_id[i]);  
    
    }
    
    // Display Map
	std::cout << "\nTotal TrackML Track Cands:" << map_track_cands.size() << std::endl;
	for (auto const &iter : map_track_cands) {  // c++11
    	std::cout << iter.first << ':' /*<< iter.second.size() << endl*/ ;
    	for(auto const& iter1: iter.second) 
        	std::cout << ' ' << iter1;
        cout << endl;
    }
    
    
    // Write PndTrackCand to fSttTrackCandArray
    std::cout << "\nTotal PndTrackCand Per Event:" << mycands.size() << std::endl;
    int index =0;
	for (auto const &iter : mycands) {  // c++11
    	
    	// std::cout << iter.first << ":" << iter.second->GetNHits() << endl;
    	
    	PndTrackCand* tcand = (PndTrackCand*)iter.second;
        //std::cout << iter.first << ":"<< tcand->GetNHits() << endl;
    	//iter.second->Print();
    	
        if (iter.first >= 0) // -1: unassigned hits.
            new ((*fSttTrackCandArray)[index]) PndTrackCand(*tcand); 
    }
    
    // Clear Objects
    map_track_cands.clear();
    mycands.clear();
    std::cout << "\nCleard Maps:" << mycands.size() << std::endl;
    cout << "\nfSttHitArray Size : " << fSttHitArray->GetEntries() << endl;
    cout << "Number of Reco Hit: " << n << endl;
    fEventId++;

}//end-Exec()


/* FinishTask() */
void PndTrackImport::FinishTask() {
    
    fSttTrackArray->Clear();
    fSttTrackCandArray->Clear();
    
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

