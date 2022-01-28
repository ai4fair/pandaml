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
	virtual ~PndMLTracking();

protected:

	virtual InitStatus Init();
	virtual void SetParContainers();
	virtual void Exec(Option_t* opt);
	virtual void FinishTask();
	
	virtual FairMCPoint* GetFairMCPoint(TString fBranchName, FairMultiLinkedData_Interface* links, FairMultiLinkedData& array);

private:
    
    /** SttParameters **/
	PndGeoSttPar *fSttParameters;
	
	/** EventHeader **/
	TClonesArray *fEventHeader;
	
	/** STTMapCreater **/
	TClonesArray *fTubeArray;
	
	
	
	/** New Code **/
	
	//CSV Path
	TString fCsvFilesPath;
	
	//CSV Files
	std::ofstream fHits;              // Hits
	std::ofstream fTruths;            // Truths
	std::ofstream fParticles;         // Particles/Tracks
	std::ofstream fTubes;             // Tubes
	
	//CSV Data
	void GenerateMvdData();           // Tracking Data from MVD (Under Construction)
	void GenerateGemData();           // Tracking Data from GEM (Under Construction)
	void GenerateSttData();           // Tracking Data from STT
	
	
	//Detector Data
	int mcTrackBranchID;               // BranchID for MC Points
	TClonesArray *fMCTrackArray;       // Storage for MC Points
	
	int mvdHitsPixelBranchID           // BranchID for MVD Pixel Hits
	TClonesArray *fMvdHitsPixelArray;  // Storage for MVD Pixel Hits
	
	int mvdHitsStripBranchID           // BranchID for MVD Strip Hits
	TClonesArray *fMvdHitsStripArray;  // Storage for MVD Strip Hits
	
	int gemHitBranchID                 // BranchID for GEM Hits
	TClonesArray *fGemHitArray;        // Storage for GEM Hits
	
	int sttHitBranchID;                // BranchID for STT Hits
	TClonesArray *fSttHitArray;        // Storage for STT Hits
	
	//Global Counters
	unsigned int fHitId;               // Hit ID
	unsigned int fEventId;             // Event ID
	


	/** OLD Code **/

	//CSV Files
	std::ofstream InFile1;
	std::ofstream InFile2;
	std::ofstream InFile3;
	
	//CSV Data (STT)
	void GenerateCSV();
	void GenerateData();
	
	
	
	ClassDef(PndMLTracking,1)
};

#endif /* PNDTRACKERS_PNDMLTRACKER_PNDMLTRACKING_H_ */
