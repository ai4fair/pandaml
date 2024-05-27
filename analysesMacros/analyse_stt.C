#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

using std::cout;
using std::endl;

void analyse_stt() {
    
    TString path = "./data/";
	TString simFile = path+"llbar_sim.root";
	TString parFile = path+"llbar_par.root";
	TString digiFile =path+"llbar_digi.root";
	TString recoFile = path+"llbar_reco.root";
	TString outFile = "out.root"; //of the FairRunAna
	TFile *out = TFile::Open("analyse_stt_4STT_hits.root", "RECREATE"); //output for the hsitograms

	// ******************* Define variables ***********************************************
	int Ev_start = 0;
	int Ev_end = 0;     // take 0 for all events

	int hits_STT=0; // total number of STT hits
	int hits_STT_per_track=0; // number of STT hits per track

	int reco=0; // set to one if a track is reconstructible
	int reco_events=0; // number of reconstructable events
	int reco_tracks=0; // number of reconstructable tracks

	int num_protons=0; // number of reconstructed proton tracks
	int num_anti_protons=0; // number of reconstructed anti proton tracks
	int num_pi_plus=0; // number of reconstructed pi plus tracks
	int num_pi_minus=0; // number of reconstructed pi minus tracks

	int proton_track=0; // if a proton track is found in the track array this is set to one
	int pi_minus_track=0; // if a pi minus is found in the track array this is set to one
	int antiProton_track=0; // if an anti-proton track is found in the track array this is set to one
	int pi_plus_track=0; // if a pi plus is found in the track array this is set to one

	int antiProton=0; // if an  anti proton track is reconstructed this is set to one for the event
	int pi_plus=0; // if a pi plus track is reconstructed this is set to one for the event
	int proton=0; // if a proton track is reconstructed this is set to one for the event
	int pi_minus=0; // if a pi minus track is reconstructed this is set to one for the event

	int reco_tracks_per_event=0; // number of reconstructible tracks per event
	int reco_anti_lambda=0; // number of reconstructible anti-lambda
	int reco_lambda=0; // number of reconstructinle lambda
	int reco_lambda_anti_lambda=0; // number of reconstructible lambda and anti-lambda in the same event

	double rMomAll;
	double zMomAll;
	double rMomGenCreatedAll;
	double zMomGenCreatedAll;
	double rMomLambda;
	double zMomLambda;
	double rMomAntiLambda;
	double zMomAntiLambda;
	double rMomGenCrProton;
	double zMomGenCrProton;
	double rMomGenCrAntiProton;
	double zMomGenCrAntiProton;
	double rMomGenCrPiMinus;
	double zMomGenCrPiMinus;
	double rMomGenCrPiPlus;
	double zMomGenCrPiPlus;

	double rMomPiMinus;
	double zMomPiMinus;
	double rMomProton;
	double zMomProton;
	double rMomAntiProton;
	double zMomAntiProton;
	double rMomPiPlus;
	double zMomPiPlus;

	TStopwatch timer;
	timer.Start();

	// *** FairRunAna
    FairLogger::GetLogger()->SetLogToFile(kFALSE);
    FairRunAna *fRun = new FairRunAna();
    fRun->SetUseFairLinks(kTRUE);
    
    FairFileSource *fSrc = new FairFileSource(recoFile);
    fSrc->AddFriend(simFile);
    fSrc->AddFriend(digiFile);
    fRun->SetSource(fSrc);
    FairRootFileSink *fSink = new FairRootFileSink(outFile);
    fRun->SetSink(fSink);
    
	// FairRuntimeDb
	FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
	FairParRootFileIo* parInput1 = new FairParRootFileIo();
	parInput1->open(parFile.Data());
	
	// FairParAsciiFileIo
	FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
	TString allDigiFile = gSystem->Getenv("VMCWORKDIR"); 
	allDigiFile += "/macro/params/all.par";
	parIo1->open(allDigiFile.Data(), "in");
	rtdb->setFirstInput(parInput1);
	rtdb->setSecondInput(parIo1);

	// FairRunAna::Init()
	fRun->Init();
    
	// FairRootManager::Instance()
	FairRootManager* ioman = FairRootManager::Instance();

    // --------------------------------------------------------

	TClonesArray* mcTrackArray = (TClonesArray*) ioman->GetObject("MCTrack"); // if not "initialized" here it may produces error at the first access
	TClonesArray* SttMvdGemTrackArray = (TClonesArray*)ioman->GetObject("BarrelTrack");
	TClonesArray* TrackCandArray = (TClonesArray*)ioman->GetObject("BarrelTrackCand");
	TClonesArray* sttHitArray = (TClonesArray*) ioman->GetObject("STTHit");
	TClonesArray* sciTHitArray = (TClonesArray*) ioman->GetObject("SciTHit");
	TClonesArray* mvdHitsPixelArray = (TClonesArray*) ioman->GetObject("MVDHitsPixel");
	TClonesArray* mvdHitsStripArray = (TClonesArray*) ioman->GetObject("MVDHitsStrip");

	PndMCTrack *mcTrack;
	PndTrack *sttMvdGemTrack;
	PndTrackCand trackCand;
	PndSciTHit *sciTHit;
	PndSttHit *sttHit;
	PndSdsDigiPixel *mvdHitsPixel;
	PndSdsDigiStrip *mvdHitsStrip;

	FairMultiLinkedData links;
	FairMultiLinkedData link;
	FairMultiLinkedData linksMCTrack;
	FairMultiLinkedData linksMvdPixel;
	FairMultiLinkedData linksMvdStrip;
	FairMultiLinkedData linksSTT;
	FairMultiLinkedData linksSciT;

	///////////////////////////////////////////////  STT /////////////////////////////////////////////////
	TH1F *hsttHitsProton = new TH1F("hsttHitsProton","Protons, 1.642 GeV",110,0,110);
	hsttHitsProton->GetXaxis()->SetTitle("Number of STT hits");
	hsttHitsProton->GetYaxis()->SetTitle("Number of tracks");

	TH1F *hsttHitsPiMinus = new TH1F("hsttHitsPiMinus","Pi minus, 1.642 GeV",110,0,110);
	hsttHitsPiMinus->GetXaxis()->SetTitle("Number of STT hits");
	hsttHitsPiMinus->GetYaxis()->SetTitle("Number of tracks");

	TH1F *hsttHitsAntiProton = new TH1F("hsttHitsAntiProton","Anti-Protons, 1.642 GeV",110,0,110);
	hsttHitsAntiProton->GetXaxis()->SetTitle("Number of STT hits");
	hsttHitsAntiProton->GetYaxis()->SetTitle("Number of tracks");

	TH1F *hsttHitsPiPlus = new TH1F("hsttHitsPiPlus","Pi plus, 1.642 GeV",110,0,110);
	hsttHitsPiPlus->GetXaxis()->SetTitle("Number of STT hits");
	hsttHitsPiPlus->GetYaxis()->SetTitle("Number of tracks");

	//////////////////////////////////// Momentum distributions ////////////////////////////////////////////

	TH2F *hmomentumDistrPiMinus= new TH2F("hmomentumDistrPiMinus","Pi minus, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomentumDistrPiMinus->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomentumDistrPiMinus->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH1F *htrackPtPiMinus= new TH1F("htrackPtPiMinus","Pi minus, 1.642 GeV",170,-0.10,1.7);
	htrackPtPiMinus->GetXaxis()->SetTitle("Transverse Momentum [GeV]");
	htrackPtPiMinus->GetYaxis()->SetTitle("Number of Tracks");


	TH2F *hmomentumDistrProton= new TH2F("hmomentumDistrProton","Proton, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomentumDistrProton->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomentumDistrProton->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH1F *htrackPtProton= new TH1F("htrackPtProton","Proton, 1.642 GeV",170,0,1.7);
	htrackPtProton->GetXaxis()->SetTitle("Transverse Momentum [GeV]");
	htrackPtProton->GetYaxis()->SetTitle("Number of Tracks");

	TH2F *hmomentumDistrPiPlus= new TH2F("hmomentumDistrPiPlus","Pi plus, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomentumDistrPiPlus->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomentumDistrPiPlus->GetYaxis()->SetTitle("R-momentum");

	TH1F *htrackPtPiPlus= new TH1F("htrackPtPiPlus","Pi plus, 1.642 GeV",170,0,1.7);
	htrackPtPiPlus->GetXaxis()->SetTitle("Transverse Momentum [GeV]");
	htrackPtPiPlus->GetYaxis()->SetTitle("Number of Tracks");

	TH2F *hmomentumDistrAntiProton= new TH2F("hmomentumDistrAntiProton","Anti proton, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomentumDistrAntiProton->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomentumDistrAntiProton->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH1F *htrackPtAntiProton= new TH1F("htrackPtAntiProton","Anti proton, 1.642 GeV",170,0,1.7);
	htrackPtAntiProton->GetXaxis()->SetTitle("Transverse Momentum [GeV]");
	htrackPtAntiProton->GetYaxis()->SetTitle("Number of Tracks");

	TH2F *hmomentumDistrAll= new TH2F("hmomentumDistrAll","All particles, 1.642 GeV",1400,-7,7,350,0,3.5);
	hmomentumDistrAll->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomentumDistrAll->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH2F *hmomentumDistrGenCreatedAll= new TH2F("hmomentumDistrGenCreatedAll","All generator created particles, 1.642 GeV",150,-0.1,1.4,150,0,0.45);
	hmomentumDistrGenCreatedAll->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomentumDistrGenCreatedAll->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH2F *hmomentumDistrLambda= new TH2F("hmomentumDistrLambda","Lambda, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomentumDistrLambda->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomentumDistrLambda->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH2F *hmomentumDistrAntiLambda= new TH2F("hmomentumDistrAntiLambda","Anti lambda, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomentumDistrAntiLambda->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomentumDistrAntiLambda->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH2F *hmomDistrGenCreatedProtons= new TH2F("hmomDistrGenCreatedProtons","Proton, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomDistrGenCreatedProtons->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomDistrGenCreatedProtons->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH2F *hmomDistrGenCreatedAntiProtons= new TH2F("hmomDistrGenCreatedAntiProtons","Anti Proton, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomDistrGenCreatedAntiProtons->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomDistrGenCreatedAntiProtons->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH2F *hmomDistrGenCreatedPiPlus= new TH2F("hmomDistrGenCreatedPiPlus","Pi plus, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomDistrGenCreatedPiPlus->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomDistrGenCreatedPiPlus->GetYaxis()->SetTitle("R-momentum [GeV]");

	TH2F *hmomDistrGenCreatedPiMinus= new TH2F("hmomDistrGenCreatedPiMinus","Pi minus, 1.642 GeV",470,-0.1,4.6,460,0,4.6);
	hmomDistrGenCreatedPiMinus->GetXaxis()->SetTitle("z-momentum [GeV]");
	hmomDistrGenCreatedPiMinus->GetYaxis()->SetTitle("R-momentum [GeV]");

	if (Ev_end == 0)
		Ev_end = Int_t((ioman->GetInChain())->GetEntries());
	cout << " EndEvent = " << Ev_end << endl;

	for (int i_Event = Ev_start; i_Event < Ev_end; i_Event++) { // ------- Loop over digitized events
		ioman->ReadEvent(i_Event);
		cout << "  " << endl;
		cout << "event: " << i_Event << endl;

		//******************* inside an event ********************

		// Loop over MCtrack array in order to plot the momentum distribution for different quantities
		int num_tracks=mcTrackArray ->GetEntries();
		for (Int_t i_Array = 0; i_Array < mcTrackArray ->GetEntries(); i_Array++) { //loop over MCtrack array
			mcTrack = (PndMCTrack *) mcTrackArray ->At(i_Array);

			rMomAll=mcTrack->GetPt();
			zMomAll=mcTrack->GetMomentum().Pz();
			hmomentumDistrAll->Fill(zMomAll,rMomAll);

			if (mcTrack->IsGeneratorCreated()){

				rMomGenCreatedAll=mcTrack->GetPt();
				zMomGenCreatedAll=mcTrack->GetMomentum().Pz();
				hmomentumDistrGenCreatedAll->Fill(zMomGenCreatedAll,rMomGenCreatedAll);

				if (mcTrack->GetPdgCode()==3122){
					rMomLambda=mcTrack->GetPt();
					zMomLambda=mcTrack->GetMomentum().Pz();
					hmomentumDistrLambda->Fill(zMomLambda,rMomLambda);
				}

				if(mcTrack->GetPdgCode()==-3122){
					rMomAntiLambda=mcTrack->GetPt();
					zMomAntiLambda=mcTrack->GetMomentum().Pz();
					hmomentumDistrAntiLambda->Fill(zMomAntiLambda,rMomAntiLambda);
				}

				if (mcTrack->GetPdgCode()==2212){
					rMomGenCrProton=mcTrack->GetPt();
					zMomGenCrProton=mcTrack->GetMomentum().Pz();
					hmomDistrGenCreatedProtons->Fill(zMomGenCrProton,rMomGenCrProton);
				}

				if(mcTrack->GetPdgCode()==-2212){
					rMomGenCrAntiProton=mcTrack->GetPt();
					zMomGenCrAntiProton=mcTrack->GetMomentum().Pz();
					hmomDistrGenCreatedAntiProtons->Fill(zMomGenCrAntiProton,rMomGenCrAntiProton);
				}

				if (mcTrack->GetPdgCode()==-211){
					rMomGenCrPiMinus=mcTrack->GetPt();
					zMomGenCrPiMinus=mcTrack->GetMomentum().Pz();
					hmomDistrGenCreatedPiMinus->Fill(zMomGenCrPiMinus,rMomGenCrPiMinus);
				}

				if(mcTrack->GetPdgCode()==211){
					rMomGenCrPiPlus=mcTrack->GetPt();
					zMomGenCrPiPlus=mcTrack->GetMomentum().Pz();
					hmomDistrGenCreatedPiPlus->Fill(zMomGenCrPiPlus,rMomGenCrPiPlus);
				}
			}

		} // end MCtrack array

		// ***************** set to zero for each event ************************************************************
		reco_tracks_per_event=0;
		proton_track=0;
		pi_minus_track=0;
		antiProton_track=0;
		pi_plus_track=0;
		// *********************************************************************************************************

		num_tracks=SttMvdGemTrackArray ->GetEntries();
		for (Int_t i_Array = 0; i_Array < SttMvdGemTrackArray ->GetEntries(); i_Array++) { //loop over trackarray

			// ************** set to zero for each track ***********************************************************
			proton=0;
			pi_minus=0;
			antiProton=0;
			pi_plus=0;

			hits_STT_per_track=0;
			reco=0;
			// ****************************************************************************************************

			sttMvdGemTrack = (PndTrack *) SttMvdGemTrackArray ->At(i_Array);

			mcTrack = NULL;
			links = sttMvdGemTrack->GetLinksWithType(ioman->GetBranchId("MCTrack")); // create the links between the track and the MCTrack
			if (links.GetNLinks()>0){
				for (Int_t i=0; i<links.GetNLinks(); i++){

					if(links.GetLink(i).GetIndex()==sttMvdGemTrack->GetTrackCand().getMcTrackId()){

						mcTrack= (PndMCTrack *) ioman->GetCloneOfLinkData(links.GetLink(i));
						if (mcTrack->IsGeneratorLast()) {

							// Check the number of STT hits
							linksSTT = sttMvdGemTrack->GetLinksWithType(ioman->GetBranchId("STTHit"));
							if (linksSTT.GetNLinks()>0){
								for (Int_t n=0; n<linksSTT.GetNLinks(); n++){ // loop over the STT hits
									sttHit= (PndSttHit *) ioman->GetCloneOfLinkData(linksSTT.GetLink(n)); // get the object the link is pointing to
									linksMCTrack= sttHit->GetLinksWithType(ioman->GetBranchId("MCTrack"));
									if (linksMCTrack.GetNLinks()>0){ // check if there are links to a MC track for the hit
										mcTrack= (PndMCTrack *) ioman->GetCloneOfLinkData(linksMCTrack.GetLink(0));
										if (mcTrack->IsGeneratorCreated()) {
											hits_STT++;
											hits_STT_per_track++;
										}
									}
								}
							} // STT end

							int j=4; // minimum number of hits in the stt for a track to be reconstructable

							if(hits_STT_per_track>=j){

								reco=1;
								reco_tracks++;

								if(sttMvdGemTrack->GetPidHypo()==-211){
									num_pi_minus++;
									pi_minus=1;
									hsttHitsPiMinus->Fill(hsttHitsPiMinus->GetBin(hits_STT_per_track));
									reco_tracks_per_event++;

									rMomPiMinus=mcTrack->GetPt();
									zMomPiMinus=mcTrack->GetMomentum().Pz();

									hmomentumDistrPiMinus->Fill(zMomPiMinus,rMomPiMinus);
									htrackPtPiMinus->Fill(rMomPiMinus);
								}

								if (sttMvdGemTrack->GetPidHypo()==2212){
									num_protons++;
									proton=1;
									hsttHitsProton->Fill(hsttHitsProton->GetBin(hits_STT_per_track));
									reco_tracks_per_event++;

									rMomProton=mcTrack->GetPt();
									zMomProton=mcTrack->GetMomentum().Pz();

									hmomentumDistrProton->Fill(zMomProton,rMomProton);
									htrackPtProton->Fill(rMomProton);
								}

								if (sttMvdGemTrack->GetPidHypo()==-2212){
									num_anti_protons++;
									antiProton=1;
									hsttHitsAntiProton->Fill(hsttHitsAntiProton->GetBin(hits_STT_per_track));
									reco_tracks_per_event++;

									rMomAntiProton=mcTrack->GetPt();
									zMomAntiProton=mcTrack->GetMomentum().Pz();

									hmomentumDistrAntiProton->Fill(zMomAntiProton,rMomAntiProton);
									htrackPtAntiProton->Fill(rMomAntiProton);
								}

								if(sttMvdGemTrack->GetPidHypo()==211){
									num_pi_plus++;
									pi_plus=1;
									hsttHitsPiPlus->Fill(hsttHitsPiPlus->GetBin(hits_STT_per_track));
									reco_tracks_per_event++;

									rMomPiPlus=mcTrack->GetPt();
									zMomPiPlus=mcTrack->GetMomentum().Pz();

									hmomentumDistrPiPlus->Fill(zMomPiPlus,rMomPiPlus);
									htrackPtPiPlus->Fill(rMomPiPlus);
								}

							} // hits stt per track end

						} // if loop over MC tracks linked to one reco track end generator created end

					} // if index of link==index of rec track
				} // loop over MC tracks linked to one reco track end

			} // if Nlinks to mc track from reco track>0 end

			if (proton==1){
				proton_track=1;
			}
			if (pi_minus==1){
				pi_minus_track=1;
			}
			if (antiProton==1){
				antiProton_track=1;
			}
			if (pi_plus==1){
				pi_plus_track=1;
			}

		} // end track array

		if (antiProton_track==1 && pi_plus_track==1){ // check if anti-lambda was reconstructed
			reco_anti_lambda++;
			if (proton_track==1 && pi_minus_track==1){ // check if both lambda and anti-lambda was reconstructed in the same event
				reco_lambda_anti_lambda++;
			}
		}

		if (proton_track==1 && pi_minus_track==1){ // check if lambda was reconstructed
			reco_lambda++;
			reco_events++;

		}

	} // end event

	out->cd();

	hsttHitsProton->Write();
	hsttHitsPiMinus->Write();
	hsttHitsAntiProton->Write();
	hsttHitsPiPlus->Write();

	hmomentumDistrPiMinus->Write();
	htrackPtPiMinus->Write();

	hmomentumDistrProton->Write();
	htrackPtProton->Write();

	hmomentumDistrPiPlus->Write();
	htrackPtPiPlus->Write();

	hmomentumDistrAntiProton->Write();
	htrackPtAntiProton->Write();

	hmomentumDistrAll->Write();
	hmomentumDistrGenCreatedAll->Write();

	hmomentumDistrLambda->Write();
	hmomentumDistrAntiLambda->Write();

	hmomDistrGenCreatedProtons->Write();
	hmomDistrGenCreatedAntiProtons->Write();
	hmomDistrGenCreatedPiMinus->Write();
	hmomDistrGenCreatedPiPlus->Write();

	// *********** write out you results *****************************************
	cout << "Number of reconstructed tracks: " << reco_tracks << endl;
	cout << "Number of reconstructed events (at least lambdas): " << reco_events << endl;
	cout << " " << endl;
	cout << "From lambda: " << endl;
	cout << "Number of protons: " << num_protons << endl;
	cout << "Number of pi minus: " << num_pi_minus << endl;
	cout << "  " << endl;
	cout << "From anti-lambda: " << endl;
	cout << "Number of anti protons: " << num_anti_protons << endl;
	cout << "Number of pi plus: " << num_pi_plus << endl;
	cout << "  " << endl;
	cout << "Number of reconstructible lambdas: " << reco_lambda << endl;
	cout << "Number of reconstructible anti-lambdas: " << reco_anti_lambda << endl;
	cout << "Number of reconstructible lambdas and anti-lambdas in the same event: " << reco_lambda_anti_lambda << endl;
	out->Save();

	timer.Stop();
	Double_t rtime = timer.RealTime();
	Double_t ctime = timer.CpuTime();
	cout << endl << endl;

	// *********************************************************
	cout << "Macro finished succesfully." << endl;
	cout << "Output file is " << outFile << endl;
	cout << "Parameter file is " << parFile << endl;
	cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
	cout << endl;
	// -----------
}

