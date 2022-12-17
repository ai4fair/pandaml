/*
 * PndTrackImport.h
 *
 *  Created on: Nov 8, 2018
 *      Author: Adeel Akram
 */

#ifndef PNDTRACKERS_PNDMLTRACKER_PNDTRACKIMPORT_H_
#define PNDTRACKERS_PNDMLTRACKER_PNDTRACKIMPORT_H_

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

class PndTrackImport: public FairTask {

public:

    PndTrackImport();
    virtual ~PndTrackImport();

protected:

    virtual InitStatus Init();
    virtual void SetParContainers();
    virtual void Exec(Option_t* opt);
    virtual void FinishTask();

    virtual FairMCPoint* GetFairMCPoint(TString fBranchName, FairMultiLinkedData_Interface* links, FairMultiLinkedData& array);

private:

    /* TODO: Inline Initialization or List Initialization */

    // Event Id
    unsigned int fEventId;             // Used for Naming CSV Files

    //CSV Path
    TString fCsvFilesPath;             // Path for CSV Files
    
    // MCTrack
    int fMCTrackBranchID;              // BranchID for MCPoints
    TClonesArray *fMCTrackArray;       // Storage for MCPoints
    
    // MVDPoint
    int fMvdPointBranchID;             // BranchID for MVDPoints
    TClonesArray *fMvdPointArray;      // Storage for MVDPoints
    
    // MVDHits
    int fMvdHitsPixelBranchID;         // BranchID for MVDPixel Hits
    TClonesArray *fMvdHitsPixelArray;  // Storage for MVDPixel Hits
    int fMvdHitsStripBranchID;         // BranchID for MVDStrip Hits
    TClonesArray *fMvdHitsStripArray;  // Storage for MVDStrip Hits
    
    // GEMPoint
    int fGemPointBranchID;             // BranchID for GEMPoints
    TClonesArray* fGemPointArray;      // Storage for GEMPoints
    
    // GEMHits
    int fGemHitBranchID;               // BranchID for GEMHits
    TClonesArray *fGemHitArray;        // Storage for GEMHits

    // STTPoint
    int fSttPointBranchID;             // BranchID for STTPoints
    TClonesArray *fSttPointArray;      // Storage for STTPoints

    // STTHits
    int fSttHitBranchID;               // BranchID for STTHits
    TClonesArray *fSttHitArray;        // Storage for STTHits
    
    // STTCombinedSkewedHits
    int fSttSkewHitBranchID;           // BranchID for STTCombinedSkewedHits
    TClonesArray *fSttSkewHitArray;    // Storage for STTCombinedSkewedHits
    
    /* SttParameters */
    PndGeoSttPar *fSttParameters;

    /* EventHeader */
    TClonesArray *fEventHeader;

    /* STTMapCreater */
    TClonesArray *fTubeArray;
    
    std::ofstream fInfile;               // Reading a CSV
    void SttTrackCand();
    
    
    
    /*
    // LayerMap for MVD/GEM
    PndGeoHandling* fGeoH;
    vector<vector<int>> fLayers;       //< contains layer information of hits
    map<TString, int> fLayerMap;       //< identifier string, assigned layer id
    int fLastLayerId;                  //< last layer Id assigned
    
    //CSV Files
    std::ofstream fHits;               // Hits
    std::ofstream fTruths;             // Truths
    std::ofstream fParticles;          // Particles
    std::ofstream fCells;              // Cells (tubes, pixels, sensors)
	    
    // CSV Generators
    void GenerateMvdPixelData();       // Tracking Data from MVDPixel
    void GenerateMvdStripData();       // Tracking Data from MVDStrip
    void GenerateGemData();            // Tracking Data from GEM
    void GenerateSttData();            // Tracking Data from STT (Non-skewed Hits)
    void GenerateSttSkewData();        // Tracking Data from STT (Skewed Hits Correction)
    void GenerateParticlesData();      // Particles for MVD, GEM and STT
    
    // Layer Map
    int GetLayer(TString identifier);
    int GetLayerGem(FairHit *hit);
    int GetLayerMvd(FairHit *hit);
    void InitLayerMap();
    void InitLayerMapMvd();
    void InitLayerMapGem();
    */

    ClassDef(PndTrackImport,1)
};

#endif /* PNDTRACKERS_PNDMLTRACKER_PNDTRACKIMPORT_H_ */
