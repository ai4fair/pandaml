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
    PndTrackImport(int start_counter, TString csv_path);
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
    
    // BarrelTrack
    //int fBarrelTrackBranchID;          // BranchID for Reco. Ideal Tracks
    //TClonesArray *fBarrelTrackArray;   // Storage for Reco. Ideal Tracks
    
    /* SttParameters */
    PndGeoSttPar *fSttParameters;

    /* EventHeader */
    TClonesArray *fEventHeader;

    /* STTMapCreater */
    TClonesArray *fTubeArray;
    
    // TODO: PndTrack and PndTrackCand
    /** Output array of global tracks **/
    TClonesArray *fBarrelTrackArray; ///< Output array of PndTrackCands
    TClonesArray *fBarrelTrackCandArray;
    
    
    
    std::ofstream fInfile;               // Reading a CSV
    void SttTrackCand();
    
    ClassDef(PndTrackImport,1)
};

#endif /* PNDTRACKERS_PNDMLTRACKER_PNDTRACKIMPORT_H_ */
