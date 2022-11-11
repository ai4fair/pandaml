/*
 * PndMLTracking.h
 *
 *  Created on: Nov 8, 2018
 *      Author: Adeel Akram
 */

#ifndef PNDTRACKERS_PNDMLTRACKER_PNDMLTRACKING_H_
#define PNDTRACKERS_PNDMLTRACKER_PNDMLTRACKING_H_

#include <FairTask.h>
#include <FairMCPoint.h>
#include <PndGeoSttPar.h>
#include <PndMCTrack.h>
#include <PndSdsHit.h>
#include <PndGemHit.h>
#include <PndSttHit.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "PndGeoHandling.h"

using namespace std;

class PndMLTracking: public FairTask {

public:

    PndMLTracking();
    PndMLTracking(int start_counter, TString csv_path, TString assist_by_ideal);
    virtual ~PndMLTracking();

protected:

    virtual InitStatus Init();
    virtual void SetParContainers();
    virtual void Exec(Option_t* opt);
    virtual void FinishTask();

    virtual FairMCPoint* GetFairMCPoint(TString fBranchName, FairMultiLinkedData_Interface* links, FairMultiLinkedData& array);

private:

    /* TODO: Inline Initialization or List Initialization */

    // Start Counter
    unsigned int fHitId;
    unsigned int fEventId;             // Used for Naming CSV Files

    //CSV Path
    TString fCsvFilesPath;             // Path for CSV Files
    
    //Assitance by IdealTrackFinder
    TString fAssistedByIdeal;
    
    // MC Track
    int fMCTrackBranchID;              // BranchID for MC Points
    TClonesArray *fMCTrackArray;       // Storage for MC Points
    
    // MVD Point
    int fMvdPointBranchID;             // BranchID for MVD Points (MC)
    TClonesArray *fMvdPointArray;      // Storage for MVD Points (MC)
    
    // MVD Hits
    int fMvdHitsPixelBranchID;         // BranchID for MVD Pixel Hits
    TClonesArray *fMvdHitsPixelArray;  // Storage for MVD Pixel Hits

    int fMvdHitsStripBranchID;         // BranchID for MVD Strip Hits
    TClonesArray *fMvdHitsStripArray;  // Storage for MVD Strip Hits
    
    // GEM Point
    int fGemPointBranchID;
    TClonesArray* fGemPointArray;
    
    // GEM Hits
    int fGemHitBranchID;               // BranchID for GEM Hits
    TClonesArray *fGemHitArray;        // Storage for GEM Hits

    // STT MC Point
    int fSttPointBranchID;             // BranchID for STT Points (MC)
    TClonesArray *fSttPointArray;      // Storage for STT Points (MC)

    // STT Hits
    int fSttHitBranchID;               // BranchID for STT Hits
    TClonesArray *fSttHitArray;        // Storage for STT Hits

    // BarrelTrack
    int fBarrelTrackBranchID;          // BranchID for Reco. Ideal Tracks
    TClonesArray *fBarrelTrackArray;   // Storage for Reco. Ideal Tracks
    
    /* SttParameters */
    PndGeoSttPar *fSttParameters;

    /* EventHeader */
    TClonesArray *fEventHeader;

    /* STTMapCreater */
    TClonesArray *fTubeArray;
    
    /* LayerMap for MVD/GEM */
    PndGeoHandling* fGeoH;
    vector<vector<int>> fLayers;       //< contains layer information of hits
    map<TString, int> fLayerMap;       //< identifier string, assigned layer id
    int fLastLayerId;                  //< last layer Id assigned
    
    //CSV Files
    std::ofstream fHits;               // Hits
    std::ofstream fTruths;             // Truths
    std::ofstream fParticles;          // Particles
    std::ofstream fCells;              // Cells (tubes, pixels, sensors)
	    
    /** CSV Generators **/
    void GenerateMvdData();            // Tracking Data from MVD
    void GenerateGemData();            // Tracking Data from GEM
    void GenerateSttData();            // Tracking Data from STT
    void GenerateParticlesData();      // Particles for MVD, GEM and STT
    
    /** Layer Map **/
    int GetLayer(TString identifier);
    int GetLayerGem(FairHit *hit);
    int GetLayerMvd(FairHit *hit);
    void InitLayerMap();
    void InitLayerMapMvd();
    void InitLayerMapGem();

    
    /** OLD Code (Kept for Ref.) **/

    //Event Counter
    //unsigned int fHitId=0;

    //CSV Files (STT)
    //std::ofstream InFile1;
    //std::ofstream InFile2;
    //std::ofstream InFile3;

    //CSV Data (STT)
    //void GenerateCSV();
    //void GenerateData();

    ClassDef(PndMLTracking,1)
};

#endif /* PNDTRACKERS_PNDMLTRACKER_PNDMLTRACKING_H_ */
