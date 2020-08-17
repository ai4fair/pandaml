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
	//Generate Data (CSVs)
	void GenerateCSV();
	void GenerateData();
	
	//SttParameters
	PndGeoSttPar *fSttParameters;
	
	//TClonesArrays
	TClonesArray *fEventHeader;
	TClonesArray *fTubeArray;
	TClonesArray *fMCTrackArray;	// from sim.root
	TClonesArray *fSttPointArray;   // from sim.root
	TClonesArray *fSttHitArray;     // from digi.root
	
	//Branch IDs
	int mcTrackBranchID;
	int sttPointBranchID;
	int sttHitBranchID;
	
	//Global Counters
	unsigned int fHitId;
	unsigned int fEventId;
	
	//CSV Files Path
	TString fCsvFilesPath;
	
	//Files
	std::ofstream fHits;
	std::ofstream fTruths;
	std::ofstream fParticles;
	std::ofstream fTubes;
	
	//Files (Will Discard)
	std::ofstream InFile1;
	std::ofstream InFile2;
	std::ofstream InFile3;
	
	ClassDef(PndMLTracking,1)
};

#endif /* PNDTRACKERS_PNDMLTRACKER_PNDMLTRACKING_H_ */
