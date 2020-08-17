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

// PndMLTracking()
PndMLTracking::PndMLTracking() {
	fSttParameters = NULL;
	fEventHeader = NULL;
	fTubeArray = NULL;
	fMCTrackArray = NULL;
	fSttPointArray = NULL;
	fSttHitArray = NULL;
	mcTrackBranchID = -1;
	sttPointBranchID = -1;
	sttHitBranchID = -1;
	fHitId = 0;
	fEventId = 0;
	fCsvFilesPath = "./data/";
}

// ~PndMLTracking()
PndMLTracking::~PndMLTracking() {
}

// PndMLTracking::SetParContainers()
void PndMLTracking::SetParContainers() 
{
	FairRuntimeDb *rtdb = FairRunAna::Instance()->GetRuntimeDb();
	fSttParameters = (PndGeoSttPar*) rtdb->getContainer("PndGeoSttPar");
}

// Init()
InitStatus PndMLTracking::Init() {

	// Get instance of the FairRootManager to access tree branches
	FairRootManager *ioman = FairRootManager::Instance();
	if(!ioman) 
	{
		std::cout << "-E- PndMLTracking::Init: FairRootManager not instantiated!" << std::endl;
		return kFATAL;
	}

	// Get the EventHeader
	fEventHeader = (TClonesArray*) ioman->GetObject("EventHeader.");
	if(!fEventHeader) 
	{
		std::cout << "-E- PndMLTracking::Init: EventHeader not loaded!" << std::endl;
		return kFATAL;
	}

	// Access STTHit branch and determine its branch ID
	fSttHitArray = (TClonesArray*) ioman->GetObject("STTHit");
	sttHitBranchID = ioman->GetBranchId("STTHit");
	
	// Access STTPoint branch and determine its branch ID
	fSttPointArray = (TClonesArray*) ioman->GetObject("STTPoint");
	sttPointBranchID = ioman->GetBranchId("STTPoint");
	
	// Access MCTrack branch and determine its branch ID
	fMCTrackArray = (TClonesArray*) ioman->GetObject("MCTrack");
	mcTrackBranchID = ioman->GetBranchId("MCTrack");
	
	// Access STTMapCreater
	PndSttMapCreator *mapper = new PndSttMapCreator(fSttParameters);
	fTubeArray = mapper->FillTubeArray();
	      
	std::cout << "-I- PndMLTracking: Initialisation successful" << std::endl;
	return kSUCCESS;
}

// Exec()
void PndMLTracking::Exec(Option_t* /*opt*/) {
	
	// TODO: Function Calls
	GenerateData();
	// GenerateSTTMap();
	

    // Debugging Tasks
    // TODO: Find STTHits and STTPoints Belonging to Primary Tracks (Kind of Filtering)
    
    // 1 - Get STTPoint connected to STTHit
    // 2 - Get STTPoint connected to MCTrack
    // 3 - Filter STTPoints/STTHits if IsGeneratorCreated()
    // 4 - Get TrackID/MCTrackID of STTPoint/STTHit
    
	/*
    // Start with STTHits
	int counter=0;
	std::cout << "\nEvent# " << fEventId << " SttHits (Total): " << fSttHitArray->GetEntriesFast() << std::endl;
	for (int idx = 0; idx < fSttHitArray->GetEntriesFast(); idx++) {
	
		// Get FairRootManager Instance
  		FairRootManager *ioman = FairRootManager::Instance();
  		
		// Get sttHitsLinks from STTHit
		FairMultiLinkedData_Interface *sttHitsLinks = (FairMultiLinkedData_Interface*)fSttHitArray->At(idx);
		
		// Get sttPointLinks & Data (TCloneArray) from sttHitsLinks
		FairMultiLinkedData sttPointLinks = sttHitsLinks->GetLinksWithType(sttPointBranchID);
		FairMCPoint *sttpoint = (FairMCPoint*)ioman->GetCloneOfLinkData(sttPointLinks.GetLink(0));
			        
        // Terminate if sttpoint=NULL
        if (sttpoint == 0) {continue;}
        
        // Get mcTrackLinks & Data (TCloneArray) from sttpoint (OR sttPointLinks?)
        FairMultiLinkedData mcTrackLinks = sttpoint->GetLinksWithType(mcTrackBranchID); 
        PndMCTrack *mctrack = (PndMCTrack*)ioman->GetCloneOfLinkData(mcTrackLinks.GetLink(0));
        
        // Terminate if mcTrack=NULL
        if (mctrack == 0) {continue;}

        // Terminate if not GeneratorCreated
        if (!mctrack->IsGeneratorCreated())
        {	
        	//std::cout << "Excluding STTPoint " << std::endl;
        	continue;
        }
        
        counter++;
        
        // TODO: Write STTHits and STTPoints (Primary Tracks) to a CSV
 
    }
    
    std::cout << "Event# " << fEventId << " STTHits (Primary): " << counter << std::endl;
    fEventId++;
	*/
	
	std::cout << "-I- PndMLTracking: Exec successful >> " << (fEventId - 1) << std::endl;
}

// FinishTask()
void PndMLTracking::FinishTask() {

	// Close all files
	fHits.close();
	fTruths.close();
	fParticles.close();
	fTubes.close();

	std::cout << "Task Generating CSVs has finished." << std::endl;
}

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
		FairMultiLinkedData sttPointLinks = sttHitsLinks->GetLinksWithType(sttPointBranchID);
		FairMCPoint *sttpoint = (FairMCPoint*)ioman->GetCloneOfLinkData(sttPointLinks.GetLink(0));
			        
        // Terminate if sttpoint=NULL
        if (sttpoint == 0) {continue;}
        
        // Get mcTrackLinks & Data (TCloneArray) from sttpoint (OR sttPointLinks?)
        FairMultiLinkedData mcTrackLinks = sttpoint->GetLinksWithType(mcTrackBranchID); 
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
				<< sttpoint->GetX()	<< ","
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
		fHits << hit_id 				<<","
			  << stthit->GetX() 		<< "," 
			  << stthit->GetY() 		<< "," 
			  << stthit->GetZ() 		<< ","
			  << stthit->GetTubeID()	<< "," 
			  << tube->IsSkew() 		<< "," 
			  << tube->GetLayerID() 	<< ","
			  << tube->GetSectorID()	<< "," 
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
		fTubes<< hit_id 				<< ","
			  << stthit->GetTubeID()	<< ","
			  << tube->IsSkew() 		<< ","
			  << tube->GetLayerID() 	<< ","
			  << tube->GetSectorID()	<< ","
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
			fHits 	<< std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);      // particle_id into hits.csv
			fTruths << std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1);    	// particle_id into truths.csv
					
			// ----------------------------------------------------------------
			//                            fParticles
			// ----------------------------------------------------------------   		   
			fParticles 	<< std::to_string(mcTrackLinks_sorted[trackIndex].GetIndex() + 1) << ","
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

// GetFairMCPoint()
FairMCPoint* PndMLTracking::GetFairMCPoint(TString fBranchName, FairMultiLinkedData_Interface* links, FairMultiLinkedData& array)
{
	// get the mc point(s) from each reco hit
	FairMultiLinkedData mcpoints = links->GetLinksWithType(FairRootManager::Instance()->GetBranchId(fBranchName));
	array = mcpoints;
	if (array.GetNLinks() == 0){
		return 0;
	}
	return (FairMCPoint*) FairRootManager::Instance()->GetCloneOfLinkData(array.GetLink(0));
}

// GenerateCSV()
void PndMLTracking::GenerateCSV() {
	
	// Call inside Exec (Waleed)
	InFile1.open (fCsvFilesPath+"box_detector_"+std::to_string(fEventId)+".csv");
	InFile2.open (fCsvFilesPath+"box_real_"+std::to_string(fEventId)+".csv");
	InFile3.open (fCsvFilesPath+"box_mctruth_"+std::to_string(fEventId)+".csv");

    InFile1	<< "hit_id" <<","<< "x" <<","<< "y" <<","<< "z"<<","
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
