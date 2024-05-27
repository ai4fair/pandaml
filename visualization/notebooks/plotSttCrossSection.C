using namespace std;

#include <TROOT.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TGeoManager.h>
#include <TStyle.h>

#include <FairGeoLoader.h>
#include <FairGeoInterface.h>
#include <FairGeoMedia.h>
#include <FairGeoBuilder.h>

#include <iostream>

void plotSttCrossSection()
{   
    /*
    TGeoManager *geoMan = new TGeoManager("sttGeoManager","STT Geometry");
    TGeoMaterial *mat = new TGeoMaterial("Vacuum",0,0,0);
    TGeoMedium   *med = new TGeoMedium("Vacuum",1,mat);
    TGeoVolume *top=gGeoManager->MakeBox("Top",med,10.,10.,10.);
    geoMan->SetTopVolume(top);
    geoMan->CloseGeometry();
    top->SetLineColor(kMagenta);
    geoMan->SetTopVisible(); // the TOP is invisible
    geoMan->SetVisLevel(4);
    top->Draw();
    */

    TGeoManager::Import("A_sttgeometry.root","sttGeo");

    /*
    TCanvas *c1 = new TCanvas("c1","c1",800,800);
    gGeoManager->SetVisLevel(4);
    gGeoManager->GetTopVolume()->Draw();
    c1->SaveAs("test.png");
    exit(0);
    */
}