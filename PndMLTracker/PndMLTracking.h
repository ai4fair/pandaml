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
#include <PndSttHit.h>
#include <fstream>
#include <sstream>
#include <iomanip>

class PndMLTracking: public FairTask {

public:

    PndMLTracking();
    PndMLTracking(int start_counter, TString csv_path);
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
    unsigned int fEventId;             // Used for Naming CSV Files

    //CSV Path
    TString fCsvFilesPath;             // Path for CSV Files

    // MC Track
    int mcTrackBranchID;               // BranchID for MC Points
    TClonesArray *fMCTrackArray;       // Storage for MC Points

    // MVD Hits
    int mvdHitsPixelBranchID;          // BranchID for MVD Pixel Hits
    TClonesArray *fMvdHitsPixelArray;  // Storage for MVD Pixel Hits

    int mvdHitsStripBranchID;          // BranchID for MVD Strip Hits
    TClonesArray *fMvdHitsStripArray;  // Storage for MVD Strip Hits

    // GEM Hits
    int gemHitBranchID;                // BranchID for GEM Hits
    TClonesArray *fGemHitArray;        // Storage for GEM Hits

    // STT MC Point
    int sttPointBranchID;              // BranchID for STT Points (MC)
    TClonesArray *fSttPointArray;      // Storage for STT Points (MC)

    // STT Hits
    int sttHitBranchID;                // BranchID for STT Hits
    TClonesArray *fSttHitArray;        // Storage for STT Hits

    // BarrelTrack
    int barrelTrackBranchID;           // BranchID for Reco. Ideal Tracks
    TClonesArray *fBarrelTrackArray;   // Storage for Reco. Ideal Tracks
    
    /* SttParameters */
    PndGeoSttPar *fSttParameters;

    /* EventHeader */
    TClonesArray *fEventHeader;

    /* STTMapCreater */
    TClonesArray *fTubeArray;
    
    //CSV Files
    std::ofstream fHits;               // Hits
    std::ofstream fTruths;             // Truths
    std::ofstream fParticles;          // Particles/Tracks
    std::ofstream fTubes;              // Tubes
	    
    /** CSV Generators **/
    void GenerateMvdData();            // Tracking Data from MVD (Under Construction)
    void GenerateGemData();            // Tracking Data from GEM (Under Construction)
    void GenerateSttData();            // Tracking Data from STT


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
