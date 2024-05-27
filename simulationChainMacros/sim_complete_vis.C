// Macro for running Panda simulation  with Geant3  or Geant4 (M. Al-Turany)
// This macro is supposed to run the full simulation of the panda detector and
// to save some visualization points to display track in the eventdisplay
// to run the macro:
// root  sim_complete_vis.C  or in root session root>.x  sim_complete_vis.C
// to run with different options:(e.g more events, different momentum, Geant4)
// root  sim_complete_vis.C"(100, "TGeant4",2)"

int sim_complete_vis(TString prefix = "evtcomplete", Int_t nEvents = 10, Float_t mom = 7.24) {
    //-----User Settings:-----------------------------------------------
    TString OutputFile = prefix + "_sim.root";
    TString ParOutputfile = prefix + "_par.root";
    Double_t BeamMomentum = 15.0; // beam momentum ONLY for the scaling of the dipole field. For the generator use "mom"
    TString SimEngine = "TGeant3";
    TString MediaFile = "media_pnd.geo";
    gDebug = 0;
    TString digiFile = "all.par"; // The emc run the hit producer directly
                                  // choose your event generator
    Bool_t UseEvtGen = kFALSE;
    Bool_t UseEvtGenDirect = kFALSE;
    Bool_t UseDpm = kFALSE;
    Bool_t UseFtf = kFALSE;
    Bool_t UseBoxGenerator = kTRUE; // FIX ME

    //------------------------------------------------------------------

    TStopwatch timer;
    timer.Start();

    gRandom->SetSeed();
    // Create the Simulation run manager--------------------------------
    FairRunSim* fRun = new FairRunSim();
    fRun->SetName(SimEngine.Data());
    fRun->SetOutputFile(OutputFile.Data());
    fRun->SetBeamMom(BeamMomentum);
    fRun->SetMaterials(MediaFile.Data());
    fRun->SetUseFairLinks(kTRUE);
    FairRuntimeDb* rtdb = fRun->GetRuntimeDb();

    // Set the parameters
    //-------------------------------
    TString allDigiFile = gSystem->Getenv("VMCWORKDIR");
    allDigiFile += "/macro/params/";
    allDigiFile += digiFile;

    //-------Set the parameter output --------------------
    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
    parIo1->open(allDigiFile.Data(), "in");
    rtdb->setFirstInput(parIo1);

    //---------------------Set Parameter output      ----------
    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo* output = new FairParRootFileIo(kParameterMerged);
    output->open(ParOutputfile.Data());
    rtdb->setOutput(output);

    // Create and add detectors

    //-------------------------  CAVE      -----------------

    FairModule* Cave = new PndCave("CAVE");
    Cave->SetGeometryFileName("pndcave.geo");
    fRun->AddModule(Cave);
    //-------------------------  Magnet   -----------------
    FairModule* Magnet = new PndMagnet("MAGNET");
    // Magnet->SetGeometryFileName("FullSolenoid_V842.root");
    Magnet->SetGeometryFileName("FullSuperconductingSolenoid_v831.root");
    fRun->AddModule(Magnet);
    FairModule* Dipole = new PndMagnet("MAGNET");
    Dipole->SetGeometryFileName("dipole.geo");
    fRun->AddModule(Dipole);
    //-------------------------  Pipe     -----------------
    FairModule* Pipe = new PndPipe("PIPE");
    Pipe->SetGeometryFileName("beampipe_201112.root");
    fRun->AddModule(Pipe);
    //-------------------------  STT       -----------------
    FairDetector* Stt = new PndStt("STT", kTRUE);
    Stt->SetGeometryFileName("straws_skewed_blocks_35cm_pipe.geo");
    fRun->AddModule(Stt);
    //-------------------------  MVD       -----------------
    FairDetector* Mvd = new PndMvdDetector("MVD", kTRUE);
    Mvd->SetGeometryFileName("Mvd-2.1_FullVersion.root");
    fRun->AddModule(Mvd);
    //-------------------------  GEM       -----------------
    FairDetector* Gem = new PndGemDetector("GEM", kTRUE);
    Gem->SetGeometryFileName("gem_3Stations_Tube.root");
    fRun->AddModule(Gem);
    //-------------------------  EMC       -----------------
    PndEmc* Emc = new PndEmc("EMC", kTRUE);
    Emc->SetGeometryVersion(1);
    Emc->SetStorageOfData(kFALSE);
    fRun->AddModule(Emc);
    //-------------------------  SCITIL    -----------------
    FairDetector* SciT = new PndSciT("SCIT", kTRUE);
    SciT->SetGeometryFileName("SciTil_201504.root");
    fRun->AddModule(SciT);
    //-------------------------  DRC       -----------------
    PndDrc* Drc = new PndDrc("DIRC", kTRUE);
    Drc->SetGeometryFileName("dirc_l0_p0_updated.root");
    Drc->SetRunCherenkov(kFALSE);
    fRun->AddModule(Drc);
    //-------------------------  DISC      -----------------
    PndDsk* Dsk = new PndDsk("DSK", kTRUE);
    Dsk->SetStoreCerenkovs(kFALSE);
    Dsk->SetStoreTrackPoints(kFALSE);
    fRun->AddModule(Dsk);
    //-------------------------  MDT       -----------------
    PndMdt* Muo = new PndMdt("MDT", kTRUE);
    Muo->SetBarrel("fast");
    Muo->SetEndcap("fast");
    Muo->SetMuonFilter("fast");
    Muo->SetForward("fast");
    Muo->SetMdtMagnet(kTRUE);
    Muo->SetMdtMFIron(kTRUE);
    fRun->AddModule(Muo);
    //-------------------------  FTS       -----------------
    FairDetector* Fts = new PndFts("FTS", kTRUE);
    Fts->SetGeometryFileName("fts.geo");
    fRun->AddModule(Fts);
    //-------------------------  FTOF      -----------------
    FairDetector* FTof = new PndFtof("FTOF", kTRUE);
    FTof->SetGeometryFileName("ftofwall.root");
    fRun->AddModule(FTof);
    //-------------------------  RICH       ----------------
    FairDetector* Rich = new PndRich("RICH", kFALSE);
    Rich->SetGeometryFileName("rich_v2.geo");
    fRun->AddModule(Rich);

    // Create and Set Event Generator
    //-------------------------------
    //----------------------------------------------------------------------------------------------------------------------------
    // BOX Generator mu+ mu-
    //----------------------------------------------------------------------------------------------------------------------------
    FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
    fRun->SetGenerator(primGen);

    if (UseBoxGenerator) {                                      // Box Generator
        FairBoxGenerator* boxGen = new FairBoxGenerator(13, 1); // 13 = muon; 3 = multipl.
        boxGen->SetPRange(0.1, 15);                             // GeV/c3
        boxGen->SetPhiRange(0., 360.);                          // Azimuth angle range [degree]
        boxGen->SetThetaRange(0.0, 180.);                       // Polar angle in lab system range [degree]
        boxGen->SetXYZ(0., 0., 0.);                             // mm o cm ??
        primGen->AddGenerator(boxGen);

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        FairBoxGenerator* boxGen2 = new FairBoxGenerator(-13, 1); // -13 = antimuon; 3 = multipl.
        boxGen2->SetPRange(0.1, 15);                              // GeV/c
        boxGen2->SetPhiRange(0., 360.);                           // Azimuth angle range [degree]
        boxGen2->SetThetaRange(0.0, 180.);                        // Polar angle in lab system range [degree]
        boxGen2->SetXYZ(0., 0., 0.);                              // mm o cm ??
        primGen->AddGenerator(boxGen2);
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
    //----------------------------------------------------------------------------------------------------------------------------

    if (UseDpm) {
        PndDpmDirect* Dpm = new PndDpmDirect(mom, 1);
        primGen->AddGenerator(Dpm);
    }
    if (UseFtf) { // the lines of Ftf have to be commented out if
        //          TString macfile = gSystem->Getenv("VMCWORKDIR");							//you want to run Geant4 with
        //          this macro
        //	  macfile += "/pgenerators/FtfEvtGen/PbarP.mac";									//
        //	  PndFtfDirect *Ftf = new PndFtfDirect(macfile.Data());								//
        PndFtfDirect* Ftf = new PndFtfDirect("anti_proton", "G4_H", 1, "ftfp", mom, 123456); //
        primGen->AddGenerator(Ftf);                                                          //
    }                                                                                        //
    if (UseEvtGen) {
        TString EvtInput = gSystem->Getenv("VMCWORKDIR");
        EvtInput += "/input/psi2s_jpsi2pi_1k.evt";
        FairEvtGenGenerator* evtGen = new FairEvtGenGenerator(EvtInput.Data());
        primGen->AddGenerator(evtGen);
    }
    if (UseEvtGenDirect) {
        TString EvtInput = gSystem->Getenv("VMCWORKDIR");
        EvtInput += "/macro/run/2pipi.dec";
        PndEvtGenDirect* EvtGen = new PndEvtGenDirect("pbarpSystem", EvtInput.Data(), mom);
        EvtGen->SetStoreTree(kFALSE);
        primGen->AddGenerator(EvtGen);
    }

    //---------------------Create and Set the Field(s)----------
    PndMultiField* fField = new PndMultiField("FULL");
    fRun->SetField(fField);

    // EMC Hit producer
    //-------------------------------
    PndEmcHitProducer* emcHitProd = new PndEmcHitProducer();
    fRun->AddTask(emcHitProd);

    //-------------------------- switch on the vis manager-----------
    fRun->SetStoreTraj(kTRUE);
    //-------------------------  Initialize the RUN  -----------------
    fRun->Init();
    //----------------- Set some cuts for the visualization-----------
    FairTrajFilter* trajFilter = FairTrajFilter::Instance();
    // Set cuts for storing the trajectpries
    trajFilter->SetStepSizeCut(0.04); // 1 cm
                                      //     trajFilter->SetVertexCut(-2000., -2000., 4., 2000., 2000., 100.);
                                      //     trajFilter->SetMomentumCutP(10e-3); // p_lab > 10 MeV
                                      //     trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
    trajFilter->SetStorePrimaries(kTRUE);
    trajFilter->SetStoreSecondaries(kTRUE);
    //-------------------------  Run the Simulation  -----------------
    fRun->Run(nEvents);
    //-------------------------  Save the parameters -----------------
    rtdb->saveOutput();
    //------------------------Print some info and exit----------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    printf("RealTime=%f seconds, CpuTime=%f seconds\n", rtime, ctime);

    cout << " Test passed" << endl;
    cout << " All ok " << endl;

    //  exit(0);
    return 0;
}
