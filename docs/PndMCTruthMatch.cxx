/*
 * PndIdealTrackFinders.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: stockman
 */

#include "PndMCTruthMatch.h"

#include "FairRootManager.h"
#include "FairMultiLinkedData.h"
#include "FairMultiLinkedData_Interface.h"

#include "PndMCTrack.h"
#include "PndFtsHit.h"
#include "FairMCPoint.h"

ClassImp(PndMCTruthMatch);

PndMCTruthMatch::PndMCTruthMatch() :
  fInBranchName("")
{
	// TODO Auto-generated constructor stub
}


PndMCTruthMatch::PndMCTruthMatch(TString inBranchName) :
  fInBranchName(inBranchName)
{
}

PndMCTruthMatch::~PndMCTruthMatch() {
	// TODO Auto-generated destructor stub
}

// -----   Public method Init   --------------------------------------------
InitStatus PndMCTruthMatch::Init()
{

	FairRootManager* ioman = FairRootManager::Instance();
	if (!ioman) {
		std::cout << "-E- PndMCTruthMatch::Init: "
				<< "RootManager not instantiated!" << std::endl;
		return kFATAL;
	}

	if (fInBranchName == ""){
	    // Use hits of all tracking subsystems if nothing is given
		std::cout << "-E- PndMCTruthMatch::Init no InBranchName given" << std::endl;
	}

	fInBranch = (TClonesArray*)ioman->GetObject(fInBranchName);

  	fMCTrack = (TClonesArray*)ioman->GetObject("MCTrack");

    event_id = 0;
    hit_id   = 0;
  	return kSUCCESS;
  	
  	// output 'CSV' file
}

void PndMCTruthMatch::Exec(Option_t*)
{
    
	InFile1.open ("box_detector_"+std::to_string(event_id)+".csv");
	InFile2.open ("box_real_"+std::to_string(event_id)+".csv");
	
    
	//std::cout << "------------ EventNr " << FairRootManager::Instance()->GetEntryNr() << " ------------" << std::endl;
	//std::cout << FairRootManager::Instance()->GetBranchId(fInBranchName) << " : " << fInBranchName << " Entries: " << fInBranch->GetEntriesFast() << std::endl;

    InFile1<< "hit_id" <<","<< "x" <<","<< "y" <<","<< "z" <<","<< "layer_id" <<","<< "chamber_id"<<","<<"tube_id"<<","<<"skewed"<<","<<"isochrone"<<","<<"halflength"<<","<<"depcharge"<<","<< "particle_id"<<std::endl;
    InFile2<< "hit_id" <<","<< "tx" <<","<< "ty" <<","<< "tz" <<","<< "px" <<","<< "py"<<","<<"pz"<<std::endl;

	for (int i = 0; i < fInBranch->GetEntriesFast(); i++)
	{
		hit_id++;
		///
		
		FairMultiLinkedData_Interface* myData = (FairMultiLinkedData_Interface*)fInBranch->At(i);
		
		FairMultiLinkedData array;
		FairMCPoint *ftspoint = GetFairMCPoint("FTSPoint", myData, array);
        if (ftspoint == 0) {continue;}
        
		InFile2<<  hit_id  <<","<<ftspoint->GetX()<<","<<ftspoint->GetY()<<","
		      <<ftspoint->GetZ()<<","<<ftspoint->GetPx()<<","
		      <<ftspoint->GetPy()<<","<<ftspoint->GetPz();		
		
		PndFtsHit* ftshit     = (PndFtsHit*)fInBranch->At(i);
		
		InFile1<<  hit_id  <<","<<ftshit->GetX()<<","<<ftshit->GetY()<<","
		      <<ftshit->GetZ()<<","<<ftshit->GetLayerID()<<","
		      <<ftshit->GetChamberID()<<","<<ftshit->GetTubeID()<<","
		      <<ftshit->GetSkewed()<<","
		      <<ftshit->GetIsochrone()<<","
		      <<ftshit->GetHalfLenght()<<","
		      <<ftshit->GetDepCharge()<<",";
		     
		//std::cout<<ftshit->GetHalfLenght()<<std::endl;      	      
		///	        
        
        //std::cout<<"Hit X position "<<ftshit->GetX()<<", the corresponding point "<<point->GetX()<<std::endl;
        //std::cout << "Point: " << point->GetX() << ", Hit: " << ftshit->GetX() << std::endl;
		
		//std::cout << i << " : " << *myData << std::endl;
		///
		//std::cout << "# links " << myData->GetNLinks() << std::endl;
		///
		std::vector<FairLink> myLinks = myData->GetSortedMCTracks();
		ii = 0;
		for (unsigned int trackIndex = 0; trackIndex < myLinks.size(); trackIndex++)
		{
			ii++;
			PndMCTrack* myTrack = (PndMCTrack*)FairRootManager::Instance()->GetCloneOfLinkData(myLinks[trackIndex]);
			//std::cout << " Link Size : " << myLinks.size() << std::endl;
			std::cout << myLinks[trackIndex] << " : " << *myTrack;
			//std::cout << myLinks[trackIndex] << " : " << myLinks[trackIndex].GetIndex();
			
			InFile1<<std::to_string(event_id)+std::to_string(myLinks[trackIndex].GetIndex());
			delete (myTrack);
		}
	InFile1<<std::endl;
	InFile2<<std::endl;
	
	//std::cout << std::endl;
	
	}
    InFile1.close();
    InFile2.close();
	//std::cout<<"event_id "<<event_id<<std::endl;
    event_id++;
    //if(ii>1 || ii<1)
    //{std::cout<<"tracks "<<ii<<std::endl;}
    //else{std::cout<<"none "<<std::endl;}
}
void PndMCTruthMatch::Finish()
{
	InFile1.close();
}


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
FairMCPoint* PndMCTruthMatch::GetFairMCPoint(TString fBranchName, FairMultiLinkedData_Interface* links, FairMultiLinkedData& array)
{
	// get the mc point(s) from each reco hit ......
	FairMultiLinkedData mcpoints = links->GetLinksWithType(FairRootManager::Instance()->GetBranchId(fBranchName));
	array = mcpoints;

	if (array.GetNLinks() == 0){
		return 0;
	}
	return (FairMCPoint *) FairRootManager::Instance()->GetCloneOfLinkData(array.GetLink(0));
}
