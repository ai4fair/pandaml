#include <TROOT.h>
#include <TCanvas.h>
#include <TFile.h>

#include <TGeoManager.h>
#include <TGeoArb8.h>
#include <TGeoPgon.h>
#include <TGeoEltu.h>
#include <TGeoCompositeShape.h>
#include <TGeoSphere.h>
#include <TGeoVolume.h>
#include <TGeoMatrix.h>
#include <TGeoNode.h>

#include <string.h>
#include <ctime>
#include <iostream>

#include <FairGeoInterface.h>
#include <FairGeoLoader.h>
#include <FairGeoMedia.h>
#include <FairGeoBuilder.h>

using namespace std;

#define PI 3.14159265
TGeoVolume *constructGasManifold()
{
  Double_t tubeRadius = 0.507;
  Double_t tubeLength = 147.5;
  Double_t frameThickness = 2;
  TGeoVolume *gasManifold;
  gasManifold = new TGeoVolumeAssembly("GasManifold");

  Int_t Angleda12[] = {0, -36, -48, -96, -108, -144};

  // GasManifold Pipe
  Int_t gmPipelavel[30];
  for (Int_t i = 0; i < 6; i++) {
    for (Int_t j = 0; j < 5; j++) {
      gmPipelavel[i * 5 + j] = 0;
    }
  }
  TGeoBBox *empBox;
  empBox = new TGeoBBox("empBox", 0, 0, .3 / sqrt(2));
  Double_t sPoint[50][3];

  for (Int_t i = 0; i < 6; i++) {
    for (Int_t j = 0; j < 5; j++) {
      sPoint[i * 5 + j][0] = 42.3 * cos((75 - 1.5 * j + Angleda12[i]) * PI / 180);
      sPoint[i * 5 + j][1] = 42.3 * sin((75 - 1.5 * j + Angleda12[i]) * PI / 180);
      if (j != 3) {
        sPoint[i * 5 + j][2] = (tubeLength + 2.4 - 0.8 * j - 2.1) / 2.0;
      } else {
        sPoint[i * 5 + j][2] = (tubeLength + 2.4 - 3.7 - 1) / 2.0;
      }
    }
  }

  Double_t specArf[50][10][3];

  /* for(Int_t i=0 ; i< 6; i++){
   for(Int_t j=0 ; j< 5; j++){
   Int_t dLevel = gmPipelavel[i*5+j];
   for(Int_t k=0; k<dLevel; k++){

   }
   }
   }*/
  Int_t lv;
  gmPipelavel[0] = 4;
  specArf[0][0][0] = tubeLength + 2.4 + 2.1;
  specArf[0][0][1] = 0;
  specArf[0][0][2] = 0;
  specArf[0][1][0] = 5.5;
  specArf[0][1][1] = 90;
  specArf[0][1][2] = 0;
  specArf[0][2][0] = 5.1;
  specArf[0][2][1] = 150;
  specArf[0][2][2] = 0;
  specArf[0][3][0] = 31.1;
  specArf[0][3][1] = 240;
  specArf[0][3][2] = 0;
  gmPipelavel[1] = 4;
  specArf[1][0][0] = tubeLength + 2.4 + 2.9;
  specArf[1][0][1] = 0;
  specArf[1][0][2] = 0;
  specArf[1][1][0] = 6.5;
  specArf[1][1][1] = 90;
  specArf[1][1][2] = 0;
  specArf[1][2][0] = 3.3;
  specArf[1][2][1] = 150;
  specArf[1][2][2] = 0;
  specArf[1][3][0] = 32.3;
  specArf[1][3][1] = 240;
  specArf[1][3][2] = 0;
  gmPipelavel[2] = 5;
  specArf[2][0][0] = tubeLength + 2.4 + 3.7;
  specArf[2][0][1] = 0;
  specArf[2][0][2] = 0;
  specArf[2][1][0] = 10.0;
  specArf[2][1][1] = 90;
  specArf[2][1][2] = 0;
  specArf[2][2][0] = 14.0;
  specArf[2][2][1] = 180;
  specArf[2][2][2] = 0;
  specArf[2][3][0] = 27.0;
  specArf[2][3][1] = 240;
  specArf[2][3][2] = 7.0;
  specArf[2][4][0] = 21.0;
  specArf[2][4][1] = 240;
  specArf[2][4][2] = 0;
  gmPipelavel[3] = 5;
  specArf[3][0][0] = tubeLength + 2.4 + 4.7;
  specArf[3][0][1] = 0;
  specArf[3][0][2] = 0;
  specArf[3][1][0] = 11.0;
  specArf[3][1][1] = 90;
  specArf[3][1][2] = 0;
  specArf[3][2][0] = 19.0;
  specArf[3][2][1] = 180;
  specArf[3][2][2] = 0;
  specArf[3][3][0] = 29.0;
  specArf[3][3][1] = 240;
  specArf[3][3][2] = 6.0;
  specArf[3][4][0] = 16.5;
  specArf[3][4][1] = 240;
  specArf[3][4][2] = 0;
  /////////////////////////////////////
  gmPipelavel[6] = 3;
  sPoint[6][2] = (tubeLength + 2.4 - 5.0) / 2.0;
  specArf[6][0][0] = tubeLength + 2.4 + 5.0;
  specArf[6][0][1] = 0;
  specArf[6][0][2] = 0;
  specArf[6][1][0] = 5.0;
  specArf[6][1][1] = 150;
  specArf[6][1][2] = 0;
  specArf[6][2][0] = 31.3;
  specArf[6][2][1] = 60;
  specArf[6][2][2] = 0;

  gmPipelavel[7] = 5;
  sPoint[7][2] = (tubeLength + 2.4 - 5.2) / 2.0;
  specArf[7][0][0] = tubeLength + 2.4 + 5.2;
  specArf[7][0][1] = 0;
  specArf[7][0][2] = 0;
  specArf[7][1][0] = 6.5;
  specArf[7][1][1] = 150;
  specArf[7][1][2] = 0;
  specArf[7][2][0] = 10.5;
  specArf[7][2][1] = 120;
  specArf[7][2][2] = 0;
  specArf[7][3][0] = 26.8;
  specArf[7][3][1] = 60;
  specArf[7][3][2] = 3.6;
  specArf[7][4][0] = 21.0;
  specArf[7][4][1] = 60;
  specArf[7][4][2] = 0;

  gmPipelavel[8] = 3;
  sPoint[8][2] = (tubeLength + 2.4 - 3.2) / 2.0;
  specArf[8][0][0] = tubeLength + 2.4 + 3.2;
  specArf[8][0][1] = 0;
  specArf[8][0][2] = 0;
  specArf[8][1][0] = 2.3;
  specArf[8][1][1] = 150;
  specArf[8][1][2] = 0;
  specArf[8][2][0] = 32.3;
  specArf[8][2][1] = 60;
  specArf[8][2][2] = 0;

  gmPipelavel[9] = 5;
  sPoint[9][2] = (tubeLength + 2.4 - 4.2) / 2.0;
  specArf[9][0][0] = tubeLength + 2.4 + 4.2;
  specArf[9][0][1] = 0;
  specArf[9][0][2] = 0;
  specArf[9][1][0] = 2.3;
  specArf[9][1][1] = 150;
  specArf[9][1][2] = 0;
  specArf[9][2][0] = 16.5;
  specArf[9][2][1] = 120;
  specArf[9][2][2] = 0;
  specArf[9][3][0] = 29.0;
  specArf[9][3][1] = 60;
  specArf[9][3][2] = 3.5;
  specArf[9][4][0] = 17.2;
  specArf[9][4][1] = 60;
  specArf[9][4][2] = 0;
  //*************************************************
  gmPipelavel[11] = 3;
  sPoint[11][2] = (tubeLength + 2.4 - 4.0) / 2.0;
  specArf[11][0][0] = tubeLength + 2.4 + 4.0;
  specArf[11][0][1] = 0;
  specArf[11][0][2] = 0;
  specArf[11][1][0] = 4.4;
  specArf[11][1][1] = 90;
  specArf[11][1][2] = 0;
  specArf[11][2][0] = 36.0;
  specArf[11][2][1] = 180;
  specArf[11][2][2] = 0;

  gmPipelavel[12] = 3;
  sPoint[12][2] = (tubeLength + 2.4 - 4.0) / 2.0;
  specArf[12][0][0] = tubeLength + 2.4 + 4.0;
  specArf[12][0][1] = 0;
  specArf[12][0][2] = 0;
  specArf[12][1][0] = 3.2;
  specArf[12][1][1] = 90;
  specArf[12][1][2] = 0;
  specArf[12][2][0] = 35.2;
  specArf[12][2][1] = 180;
  specArf[12][2][2] = 0;

  gmPipelavel[13] = 5;
  sPoint[13][2] = (tubeLength + 2.4 - 5.5) / 2.0;
  specArf[13][0][0] = tubeLength + 2.4 + 5.5;
  specArf[13][0][1] = 0;
  specArf[13][0][2] = 0;
  specArf[13][1][0] = 10.0;
  specArf[13][1][1] = 90;
  specArf[13][1][2] = 0;
  specArf[13][2][0] = 31.0;
  specArf[13][2][1] = 180;
  specArf[13][2][2] = 8.0;
  specArf[13][3][0] = 10.0;
  specArf[13][3][1] = 120;
  specArf[13][3][2] = 0;
  specArf[13][4][0] = 20.0;
  specArf[13][4][1] = 180;
  specArf[13][4][2] = 0;

  gmPipelavel[14] = 4;
  sPoint[14][2] = (tubeLength + 2.4 - 4.2) / 2.0;
  specArf[14][0][0] = tubeLength + 2.4 + 4.2;
  specArf[14][0][1] = 0;
  specArf[14][0][2] = 0;
  specArf[14][1][0] = 14.2;
  specArf[14][1][1] = 90;
  specArf[14][1][2] = 0;
  specArf[14][2][0] = 29.0;
  specArf[14][2][1] = 180;
  specArf[14][2][2] = 10.0;
  specArf[14][3][0] = 28.0;
  specArf[14][3][1] = 180;
  specArf[14][3][2] = 0;
  ////////////////////////////////////////////////////
  lv = 18;
  gmPipelavel[lv] = 3;
  sPoint[lv][2] = (tubeLength + 2.4 - 4.0) / 2.0;
  specArf[lv][0][0] = tubeLength + 2.4 + 4.0;
  specArf[lv][0][1] = 0;
  specArf[lv][0][2] = 0;
  specArf[lv][1][0] = 3.7;
  specArf[lv][1][1] = 180 - 90;
  specArf[lv][1][2] = 0;
  specArf[lv][2][0] = 36.0;
  specArf[lv][2][1] = 180 - 180;
  specArf[lv][2][2] = 0;

  lv = 17;
  gmPipelavel[lv] = 3;
  sPoint[12][2] = (tubeLength + 2.4 - 4.0) / 2.0;
  specArf[lv][0][0] = tubeLength + 2.4 + 4.0;
  specArf[lv][0][1] = 0;
  specArf[lv][0][2] = 0;
  specArf[lv][1][0] = 2.2;
  specArf[lv][1][1] = 180 - 90;
  specArf[lv][1][2] = 0;
  specArf[lv][2][0] = 35.2;
  specArf[lv][2][1] = 180 - 180;
  specArf[lv][2][2] = 0;
  lv = 16;
  gmPipelavel[lv] = 5;
  sPoint[lv][2] = (tubeLength + 2.4 - 5.5) / 2.0;
  specArf[lv][0][0] = tubeLength + 2.4 + 5.5;
  specArf[lv][0][1] = 0;
  specArf[lv][0][2] = 0;
  specArf[lv][1][0] = 10.0;
  specArf[lv][1][1] = 180 - 90;
  specArf[lv][1][2] = 0;
  specArf[lv][2][0] = 31.0;
  specArf[lv][2][1] = 180 - 180;
  specArf[lv][2][2] = 7.0;
  specArf[lv][3][0] = 11.4;
  specArf[lv][3][1] = 180 - 120;
  specArf[lv][3][2] = 0;
  specArf[lv][4][0] = 20.0;
  specArf[lv][4][1] = 180 - 180;
  specArf[lv][4][2] = 0;
  lv = 15;
  gmPipelavel[lv] = 4;
  sPoint[lv][2] = (tubeLength + 2.4 - 4.2) / 2.0;
  specArf[lv][0][0] = tubeLength + 2.4 + 4.2;
  specArf[lv][0][1] = 0;
  specArf[lv][0][2] = 0;
  specArf[lv][1][0] = 15.2;
  specArf[lv][1][1] = 180 - 90;
  specArf[lv][1][2] = 0;
  specArf[lv][2][0] = 29.0;
  specArf[lv][2][1] = 180 - 180;
  specArf[lv][2][2] = 11.4;
  specArf[lv][3][0] = 28.0;
  specArf[lv][3][1] = 180 - 180;
  specArf[lv][3][2] = 0;

  for (Int_t i = 29; i > 19; i--) {
    gmPipelavel[i] = gmPipelavel[29 - i];
    for (Int_t j = 0; j < gmPipelavel[i]; j++) {
      specArf[i][j][0] = specArf[29 - i][j][0];
      specArf[i][j][2] = specArf[29 - i][j][2];
      if (i != 0) {
        specArf[i][j][1] = 180 - specArf[29 - i][j][1];
      } else {
        specArf[i][j][1] = specArf[29 - i][j][1];
      }
    }
  }

  std::vector<TGeoTube *> tubef;
  std::vector<TGeoTube *> gasf;
  std::vector<TGeoCombiTrans *> trf;

  std::vector<TGeoVolume *> gmPipeArr;
  std::vector<TGeoVolume *> gasArr;
  std::vector<TGeoCompositeShape *> tubeshapecomp;
  std::vector<TGeoCompositeShape *> gasshapecomp;
  Int_t index = 0;
  Int_t index2 = 0;
  for (Int_t i = 0; i < 6; i++) {
    for (Int_t j = 0; j < 5; j++) {
      if (gmPipelavel[i * 5 + j] != 0) {
        Int_t dLevel = gmPipelavel[i * 5 + j];
        std::string name;
        std::string tubetmpin = "empBox ";
        std::string gastmpin = "empBox ";
        std::string gasdel = " - (";

        Double_t sX = sPoint[i * 5 + j][0];
        Double_t sY = sPoint[i * 5 + j][1];
        Double_t sZ = sPoint[i * 5 + j][2];
        Int_t sIdPoint = 0;
        Int_t angle = 0;
        name = "gmPipeShape" + std::to_string(i * 5 + j);
        for (Int_t k = 0; k < dLevel; k++) {
          Double_t x, y, z;
          if (k == 0) {
            angle = 0;
            x = sX;
            y = sY;
            z = sZ;
          } else if (k > 0) {
            angle = 90;
            x = sX - (specArf[i * 5 + j][k][0] / 2) * sin(specArf[i * 5 + j][k][1] * PI / 180);
            y = sY + (specArf[i * 5 + j][k][0] / 2) * cos(specArf[i * 5 + j][k][1] * PI / 180);
            z = sZ;
            if (specArf[i * 5 + j][k][2] != 0) {
              x = x + (specArf[i * 5 + j][sIdPoint][0] - specArf[i * 5 + j][k][2]) * sin(specArf[i * 5 + j][sIdPoint][1] * PI / 180);
              y = y - (specArf[i * 5 + j][sIdPoint][0] - specArf[i * 5 + j][k][2]) * cos(specArf[i * 5 + j][sIdPoint][1] * PI / 180);
            }
          }

          std::string tubefname = "tubef" + std::to_string(i) + std::to_string(j) + std::to_string(k);
          std::string gasfname = "gasf" + std::to_string(i) + std::to_string(j) + std::to_string(k);
          std::string trfname = "trf" + std::to_string(i) + std::to_string(j) + std::to_string(k);
          tubef.push_back(new TGeoTube(tubefname.c_str(), 0.15, .2, (specArf[i * 5 + j][k][0]) / 2.0));
          gasf.push_back(new TGeoTube(gasfname.c_str(), 0, .1499, (specArf[i * 5 + j][k][0]) / 2.0));

          trf.push_back(new TGeoCombiTrans(trfname.c_str(), x, y, z, new TGeoRotation("r", specArf[i * 5 + j][k][1], angle, 0)));
          trf[index2]->RegisterYourself();
          index2++;
          tubetmpin += "+" + tubefname + ":" + trfname;
          gastmpin += "+" + gasfname + ":" + trfname;
          if (k == 0)
            gasdel += tubefname + ":" + trfname;
          else
            gasdel += "+" + tubefname + ":" + trfname;

          if (specArf[i * 5 + j][k][2] == 0) {
            if (k != 0) {
              sX = sX - (specArf[i * 5 + j][k][0]) * sin(specArf[i * 5 + j][k][1] * PI / 180);
              sY = sY + (specArf[i * 5 + j][k][0]) * cos(specArf[i * 5 + j][k][1] * PI / 180);
            } else {
              sZ = (sZ - (tubeLength + 2.4) / 2.0) * 2;
            }
            sIdPoint = k;
          }
        }
        gasdel += ")";
        gastmpin += gasdel;
        tubeshapecomp.push_back(new TGeoCompositeShape(name.c_str(), tubetmpin.c_str()));
        gasshapecomp.push_back(new TGeoCompositeShape(name.c_str(), gastmpin.c_str()));

        gmPipeArr.push_back(new TGeoVolume("gmPipe", tubeshapecomp[index], gGeoManager->GetMedium("Aluminum")));
        TGeoVolume *gas;
        gas = new TGeoVolume("ArCO2", gasshapecomp[index], gGeoManager->GetMedium("ArCO2"));
        gas->SetLineColor(20);
        gasArr.push_back(gas);

        index++;
      }
    }
  }

  index = 0;
  for (Int_t i = 0; i < 6; i++) {
    for (Int_t j = 0; j < 5; j++) {
      if (gmPipelavel[i * 5 + j] != 0) {
        TGeoVolume *gmPipe;
        gmPipe = new TGeoVolumeAssembly("gasManifold");
        gmPipe->AddNode(gmPipeArr[index], 0);
        gmPipe->AddNode(gasArr[index], 0);
        gasManifold->AddNode(gmPipe, index);
        index++;
      }
    }
  }

  return gasManifold;
}

TGeoVolume *constructMechanicalFrame()
{
  Double_t tubeRadius = 0.507;
  Double_t tubeLength = 147.5;
  Double_t frameThickness = 2.0;
  TGeoVolume *mechanicalFrame, *frameStrip;
  mechanicalFrame = new TGeoVolumeAssembly("mechanicalFrame");
  frameStrip = new TGeoVolumeAssembly("frameStrip");

  TGeoVolume *tubeFrame;
  TGeoTube *tubeFrameShape;
  tubeFrameShape = new TGeoTube("tubeFrameShape", 0.95, 1.1, tubeLength / 2);
  tubeFrame = new TGeoVolume("tubeFrame", tubeFrameShape, gGeoManager->GetMedium("ABS"));
  tubeFrame->SetLineColor(37);

  TGeoVolume *supportedFrame;
  TGeoTube *bigRingShape;
  bigRingShape = new TGeoTube("bigRingShape", 39.6, 44.3, frameThickness / 2);
  TGeoTube *smallRingShape;
  smallRingShape = new TGeoTube("smallRingShape", 15.0, 16.1, frameThickness / 2);

  TGeoBBox *BoxShape;
  BoxShape = new TGeoBBox("BoxShape", 3.5 / 2, 13.0, frameThickness / 2);
  TGeoCombiTrans *tbs1 = new TGeoCombiTrans("tbs1", 3.5 / 2 + 1.0, 19.0 + 9.0, 0, new TGeoRotation("dd", 0, 0, 0));
  tbs1->RegisterYourself();
  TGeoCombiTrans *tbs2 = new TGeoCombiTrans("tbs2", 29.0 * sin(60 * PI / 180), 29.0 * cos(60 * PI / 180), 0, new TGeoRotation("dd", 0, 0, -60));
  tbs2->RegisterYourself();
  TGeoCombiTrans *tbs3 = new TGeoCombiTrans("tbs3", 29.0 * sin(60 * PI / 180), -29.0 * cos(60 * PI / 180), 0, new TGeoRotation("dd", 0, 0, 60));
  tbs3->RegisterYourself();
  TGeoCombiTrans *tbs4 = new TGeoCombiTrans("tbs4", 3.5 / 2 + 1.0, -19.0 - 9.0, 0, new TGeoRotation("dd", 0, 0, 0));
  tbs4->RegisterYourself();

  TGeoBBox *delBoxShape;

  delBoxShape = new TGeoBBox("delBoxShape", 25.0, 50.0, frameThickness);
  TGeoTranslation *tdbs = new TGeoTranslation("tdbs", -25.0 + 2.0, 0, 0);
  tdbs->RegisterYourself();

  TGeoTube *delBigRingShape;
  delBigRingShape = new TGeoTube("delBigRingShape", 41.6, 45.0, frameThickness / 2);
  TGeoTranslation *tdBRs = new TGeoTranslation("tdbrs", 0, 0, 1.2);
  tdBRs->RegisterYourself();

  // Big Tube Hole
  TGeoTube *tubeHoleShape; // The shape for the big tubeHole on mechanicalframe
  tubeHoleShape = new TGeoTube("tubeHoleShape", 0, .7, frameThickness * 4);

  Int_t Angles8[] = {78, 30, -30, -78};
  std::vector<TGeoTranslation> fts8;
  for (Int_t i = 0; i < 4; i++) {
    fts8.push_back(TGeoTranslation(43.0 * cos(Angles8[i] * PI / 180), 43.0 * sin(Angles8[i] * PI / 180), 0));
  }
  std::vector<TGeoHMatrix *> tbh; // Translation for big tubeHole
  for (Int_t i = 0; i < 4; i++) {
    TGeoHMatrix *transfb = new TGeoHMatrix(fts8[i]);
    tbh.push_back(transfb);
    std::string name = "tbh" + std::to_string(i);
    tbh[i]->SetName(name.c_str());
    tbh[i]->RegisterYourself();
  }

  // Small Tube Hole
  TGeoTube *tubeHoleShape2; // The shape for the small tubeHole on mechanicalframe
  tubeHoleShape2 = new TGeoTube("tubeHoleShape2", 0, .32, 2.0);

  Int_t Angleda12[] = {0, -36, -48, -96, -108, -144};
  std::vector<TGeoTranslation *> tTHS;
  for (Int_t i = 0; i < 6; i++) {
    for (Int_t j = 0; j < 5; j++) {
      tTHS.push_back(new TGeoTranslation(42.3 * cos((75 - 1.5 * j + Angleda12[i]) * PI / 180), 42.3 * sin((75 - 1.5 * j + Angleda12[i]) * PI / 180), 0));
      std::string name = "tThs" + std::to_string(i * 5 + j);
      tTHS[i * 5 + j]->SetName(name.c_str());
      tTHS[i * 5 + j]->RegisterYourself();
    }
  }

  std::string tempStr = "(smallRingShape + bigRingShape + BoxShape:tbs1+ BoxShape:tbs2+ BoxShape:tbs3+BoxShape:tbs4)-delBigRingShape:tdbrs-delBoxShape:tdbs";
  for (Int_t i = 0; i < 4; i++) {
    tempStr = tempStr + "-tubeHoleShape:tbh" + std::to_string(i);
  }
  for (Int_t i = 0; i < 6; i++) {
    for (Int_t j = 0; j < 5; j++) {
      tempStr = tempStr + " - tubeHoleShape2:tThs" + std::to_string(i * 5 + j);
    }
  }

  TGeoCompositeShape *supportedFrameShape = new TGeoCompositeShape("supportedFrameShape", tempStr.c_str());

  supportedFrame = new TGeoVolume("supportedFrame", supportedFrameShape, gGeoManager->GetMedium("Aluminum"));
  supportedFrame->SetLineColor(29);

  TGeoVolume *gasPibeCoverM = gGeoManager->MakeTubs("gasPibeCoverM", gGeoManager->GetMedium("ABS"), 42.0, 43.0, tubeLength / 2, 67.6, 76.4);
  TGeoVolume *gasPibeCoverD = gGeoManager->MakeTubs("gasPibeCoverD", gGeoManager->GetMedium("ABS"), 41.8, 42.8, tubeLength / 2 + 10.0, 67.87, 76.13);
  TGeoShape *gPCM = gasPibeCoverM->GetShape();
  gPCM->SetName("gPCM");
  TGeoShape *gPCD = gasPibeCoverD->GetShape();
  gPCD->SetName("gPCD");
  TGeoCompositeShape *gasPibeCoverP1Shape = new TGeoCompositeShape("gasPibeCoverP1Shape", "gPCM-gPCD");
  TGeoVolume *gasPibeCoverP1 = new TGeoVolume("gasPibeCoverP1", gasPibeCoverP1Shape, gGeoManager->GetMedium("ABS"));
  gasPibeCoverP1->SetLineColor(45);

  TGeoVolume *gasPibeHolderM = gGeoManager->MakeTubs("gasPibeHolderM", gGeoManager->GetMedium("ABS"), 41.8, 42.8, 1.0, 68.15, 75.85);
  TGeoShape *gPHM = gasPibeHolderM->GetShape();
  gPHM->SetName("gPHM");

  TGeoCompositeShape *gasPibeHolderShape =
    new TGeoCompositeShape("gasPibeHolderShape", "gPHM - tubeHoleShape2:tThs0- tubeHoleShape2:tThs1- tubeHoleShape2:tThs2- tubeHoleShape2:tThs3- tubeHoleShape2:tThs4");
  TGeoVolume *gasPibeHolder = new TGeoVolume("gasPibeHolder", gasPibeHolderShape, gGeoManager->GetMedium("ABS"));
  gasPibeHolder->SetLineColor(29);

  TGeoVolume *gasPibeCoverMb = gGeoManager->MakeTubs("gasPibeCoverMb", gGeoManager->GetMedium("ABS"), 41.6, 42.8, tubeLength / 2, 19.87 + 48, 40.13 + 48);
  TGeoVolume *gasPibeCoverDb = gGeoManager->MakeTubs("gasPibeCoverDb", gGeoManager->GetMedium("ABS"), 41.8, 42.8 + 1, tubeLength / 2 + 100, 32.13 + 48, 39.87 + 48);
  TGeoVolume *gasPibeCoverDb2 = gGeoManager->MakeTubs("gasPibeCoverDb2", gGeoManager->GetMedium("ABS"), 41.8, 42.8, tubeLength / 2 + 100, 28.13 + 48, 31.87 + 48);
  TGeoShape *gPCMb = gasPibeCoverMb->GetShape();
  gPCMb->SetName("gPCMb");
  TGeoShape *gPCDb = gasPibeCoverDb->GetShape();
  gPCDb->SetName("gPCDb");
  TGeoShape *gPCDb2 = gasPibeCoverDb2->GetShape();
  gPCDb2->SetName("gPCDb2");
  TGeoRotation *rd = new TGeoRotation("rd", -12, 0, 0);
  rd->RegisterYourself();

  TGeoVolume *gasPibeCoverMb2 = gGeoManager->MakeTubs("gasPibeCoverMb2", gGeoManager->GetMedium("ABS"), 41.6, 42.8, tubeLength / 2, 19.87 + 48, 32 + 48);
  TGeoShape *gPCMb2 = gasPibeCoverMb2->GetShape();
  gPCMb2->SetName("gPCMb2");
  TGeoVolume *gasPibeCoverDb3 = gGeoManager->MakeTubs("gasPibeCoverDb3", gGeoManager->GetMedium("ABS"), 41.8, 42.8 + .1, tubeLength / 2 + 10.0, 28.13 + 48, 35 + 48);
  TGeoShape *gPCDb3 = gasPibeCoverDb3->GetShape();
  gPCDb3->SetName("gPCDb3");

  TGeoCompositeShape *gasPibeCoverP1Shapeb2 = new TGeoCompositeShape("gasPibeCoverP1Shapeb", "gPCMb2-gPCDb:rd -gPCDb3");
  TGeoVolume *gasPibeCoverP1b2 = new TGeoVolume("gasPibeCoverP1b2", gasPibeCoverP1Shapeb2, gGeoManager->GetMedium("ABS"));
  gasPibeCoverP1b2->SetLineColor(49);
  TGeoCompositeShape *gasPibeCoverP1Shapeb = new TGeoCompositeShape("gasPibeCoverP1Shapeb", "gPCMb-gPCDb:rd-gPCDb -gPCDb2");
  TGeoVolume *gasPibeCoverP1b = new TGeoVolume("gasPibeCoverP1b", gasPibeCoverP1Shapeb, gGeoManager->GetMedium("ABS"));
  gasPibeCoverP1b->SetLineColor(49);

  //  Translation for carbonfiber tube
  TGeoTranslation *tx1 = new TGeoTranslation("tx1", 43.0 * cos(78 * PI / 180), 43.0 * sin(78 * PI / 180), (tubeLength) / 2 + 1.2);
  tx1->RegisterYourself();
  TGeoTranslation *tx2 = new TGeoTranslation("tx2", 43.0 * cos(30 * PI / 180), 43.0 * sin(30 * PI / 180), (tubeLength) / 2 + 1.2);
  tx2->RegisterYourself();

  TGeoTranslation *tx3 = new TGeoTranslation("tx3", 43.0 * cos(-30 * PI / 180), 43.0 * sin(-30 * PI / 180), (tubeLength) / 2 + 1.2);
  tx3->RegisterYourself();
  TGeoTranslation *tx4 = new TGeoTranslation("tx4", 43.0 * cos(-78 * PI / 180), 43.0 * sin(-78 * PI / 180), (tubeLength) / 2 + 1.2);
  tx4->RegisterYourself();

  mechanicalFrame->AddNode(gasPibeCoverP1b2, 0, new TGeoTranslation(0, 0, (tubeLength) / 2 + 1.2));
  mechanicalFrame->AddNode(gasPibeCoverP1b, 0, new TGeoCombiTrans("tx4f", 0, 0, (tubeLength) / 2 + 1.2, new TGeoRotation("r", -48, 0, 0)));
  mechanicalFrame->AddNode(gasPibeCoverP1b, 1, new TGeoCombiTrans("tx4f", 0, 0, (tubeLength) / 2 + 1.2, new TGeoRotation("r", -108, 0, 0)));
  mechanicalFrame->AddNode(gasPibeCoverP1b2, 1, new TGeoCombiTrans("tx4f", 0, 0, (tubeLength) / 2 + 1.2, new TGeoRotation("r", 0, 180, 0)));

  Double_t ArLgasPibeHolder[] = {4.51353, 15.7344, 17.5, 17.5, 17.5, 17.5, 17.5, 17.5, 17.5};
  Double_t GasPibeHolderPosZ = 0;
  for (Int_t i = 0; i < 9; i++) {
    GasPibeHolderPosZ = GasPibeHolderPosZ + ArLgasPibeHolder[i];
    frameStrip->AddNode(gasPibeHolder, 65, new TGeoTranslation(0, 0, GasPibeHolderPosZ + 1.2));
  }
  frameStrip->AddNode(gasPibeCoverP1, 61, new TGeoTranslation(0, 0, (tubeLength) / 2 + 1.2));

  mechanicalFrame->AddNode(frameStrip, 0);
  mechanicalFrame->AddNode(frameStrip, 1, new TGeoCombiTrans("tx4f", 0, 0, 0, new TGeoRotation("r", -36, 0, 0)));
  mechanicalFrame->AddNode(frameStrip, 2, new TGeoCombiTrans("txg", 0, 0, 0, new TGeoRotation("r", -48, 0, 0)));
  mechanicalFrame->AddNode(frameStrip, 3, new TGeoCombiTrans("tx4f", 0, 0, 0, new TGeoRotation("r", -96, 0, 0)));
  mechanicalFrame->AddNode(frameStrip, 4, new TGeoCombiTrans("txg", 0, 0, 0, new TGeoRotation("r", -108, 0, 0)));
  mechanicalFrame->AddNode(frameStrip, 5, new TGeoCombiTrans("txg", 0, 0, 0, new TGeoRotation("r", -144, 0, 0)));

  mechanicalFrame->AddNode(tubeFrame, 15, tx1);
  mechanicalFrame->AddNode(tubeFrame, 15, tx2);
  mechanicalFrame->AddNode(tubeFrame, 15, tx3);
  mechanicalFrame->AddNode(tubeFrame, 15, tx4);

  TGeoBBox *delBoxShape2;
  delBoxShape2 = new TGeoBBox("delBoxShape2", 25.0, 50.0, 100.0);

  TGeoVolume *sttCover;
  TGeoTube *sttCoverShape;
  sttCoverShape = new TGeoTube("sttCoverShape", 42.395 + tubeRadius + .08 + 1.3, 42.395 + tubeRadius + .13 + 1.3, tubeLength / 2 + 6.0);
  TGeoTube *sttCoverShape2;
  sttCoverShape2 = new TGeoTube("sttCoverShape2", 15.0, 15.05, tubeLength / 2);

  TGeoCompositeShape *sttCoverShapeCom = new TGeoCompositeShape("sttShapeCom", "(sttCoverShape+sttCoverShape2)-delBoxShape2:tdbs");
  sttCover = new TGeoVolume("sttCover", sttCoverShapeCom, gGeoManager->GetMedium("ABS"));
  sttCover->SetLineColor(23);

  mechanicalFrame->AddNode(supportedFrame, 17, new TGeoTranslation(0, 0, frameThickness / 2));
  mechanicalFrame->AddNode(supportedFrame, 18, new TGeoCombiTrans(0, 0, (tubeLength) + 1.4, new TGeoRotation("r", 180, 180, 180)));

  return mechanicalFrame;
}

int createstt()
{
  Double_t tubeRadius = 0.507;

  Double_t tubeLengthArr[] = {140.0, 140.0 / cos(2.9 * PI / 180.0), 118.5, 98.5, 78.5, 58.5, 38.5, 18.5};
  Double_t crimpPinLength = 1.0;
  TCanvas *c = new TCanvas("c", "c", 0, 0, 900, 900);
  TString vmcWorkdir = getenv("VMCWORKDIR");

  // TString fGeoFile= vmcWorkdir + Form("/../../../Desktop/STTGEOMETRY/FAIRGEO/Asttgeo.root");
  TString fGeoFile = vmcWorkdir + Form("/geometry/A_sttgeometry.root");
  // if(iter!=0) fGeoFile= geomPath + Form("/jaja2.root");
  TFile *fi = new TFile(fGeoFile, "RECREATE");
  cout << "Output file = " << fGeoFile << endl;


  FairGeoLoader *geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface *geoFace = geoLoad->getGeoInterface();

  geoFace->setMediaFile(vmcWorkdir + "/geometry/media_pnd.geo");
  geoFace->readMedia();

  FairGeoMedia *Media = geoFace->getMedia();
  FairGeoBuilder *geobuild = geoLoad->getGeoBuilder();

  FairGeoMedium *SttAir = Media->getMedium("air");
  FairGeoMedium *SttAluminum = Media->getMedium("Aluminum");
  FairGeoMedium *SttPVC = Media->getMedium("PVC");
  FairGeoMedium *SttMylar = Media->getMedium("mylar");
  FairGeoMedium *SttArCO2 = Media->getMedium("ArCO2");
  FairGeoMedium *SttArCO22b = Media->getMedium("STTmix9010_2bar");
  FairGeoMedium *SttCopper = Media->getMedium("copper");
  FairGeoMedium *SttReW = Media->getMedium("ReW");
  FairGeoMedium *SttCuBe2 = Media->getMedium("CuBe2");
  FairGeoMedium *SttABS = Media->getMedium("ABS");

  Int_t nmed = geobuild->createMedium(SttAir);

  nmed = geobuild->createMedium(SttAluminum);

  nmed = geobuild->createMedium(SttPVC);

  nmed = geobuild->createMedium(SttMylar);
  nmed = geobuild->createMedium(SttArCO2);
  nmed = geobuild->createMedium(SttArCO22b);
  nmed = geobuild->createMedium(SttCopper);
  nmed = geobuild->createMedium(SttReW);
  nmed = geobuild->createMedium(SttCuBe2);
  nmed = geobuild->createMedium(SttABS);

  TGeoVolume *tube, *top, *halfSide;
  TGeoVolume *sttVol;

  // Create top volume
  // TGeoManager *SttGeom = new TGeoManager("stt","Second stt geometry model");
  TGeoManager *gGeoMan = (TGeoManager *)gROOT->FindObject("FAIRGeom");
  gGeoManager->SetNsegments(80);

  top = new TGeoVolumeAssembly("top");
  sttVol = new TGeoVolumeAssembly("stt02assembly0");
  halfSide = new TGeoVolumeAssembly("sttHalfSide");
  gGeoManager->SetTopVolume(top);

  //  ****************************** Create Tube ******************************
  std::vector<TGeoVolume *> tubeArr;
  for (Int_t i = 0; i < 8; i++) {

    Double_t tubeLength = tubeLengthArr[i];
    Double_t wireLength = tubeLength + 2.0;
    Double_t fac = 0.000000001;

    TGeoVolume *tubeTemp;
    TGeoVolume *cap, *EndCap, *CrimpPin, *Wire, *MylarLayer, *AlLayer1, *AlLayer2, *RetainingRing, *SpringSmallRing, *SpringBigRing, *NobleGas;
    TGeoPcon *WireCoveredL2, *WireCoveredL3;
    std::string tubeType = "tubeType" + std::to_string(i);
    std::string tubeName = "tubestt02-" + std::to_string(i);
    tubeTemp = new TGeoVolumeAssembly(tubeName.c_str());
    cap = new TGeoVolumeAssembly("Cap");

    WireCoveredL2 = new TGeoPgon("WireCoveredLayer2", 0, 360, 80, 16);
    WireCoveredL2->DefineSection(0, 0, (.1 + (.040 + .025) / 2) / 2 + 0.03, .125);
    WireCoveredL2->DefineSection(1, 0.025, (.1 + (.040 + .025) / 2) / 2 + 0.05, .15);
    WireCoveredL2->DefineSection(2, 0.03, (.1 + (.040 + .025) / 2) / 2, .15);
    WireCoveredL2->DefineSection(3, 0.045, (.1 + (.040 + .025) / 2) / 2, .15);
    WireCoveredL2->DefineSection(4, 0.045, (.1 + (.040 + .025) / 2) / 2, .115);
    WireCoveredL2->DefineSection(5, 0.15, (.1 + (.040 + .025) / 2) / 2, .115);
    WireCoveredL2->DefineSection(6, 0.15, (.1 + (.040 + .025) / 2) / 2, .15);
    WireCoveredL2->DefineSection(7, 0.5, (.1 + (.040 + .025) / 2) / 2, .15);
    WireCoveredL2->DefineSection(8, 0.5, (.1 + (.040 + .025) / 2) / 2, .45);
    WireCoveredL2->DefineSection(9, 0.55, (.1 + (.040 + .025) / 2) / 2, .45);
    WireCoveredL2->DefineSection(10, 0.55, (.1 + (.040 + .025) / 2) / 2, .42);
    WireCoveredL2->DefineSection(11, 0.58, (.1 + (.040 + .025) / 2) / 2, .42);
    WireCoveredL2->DefineSection(12, 0.58, (.1 + (.040 + .025) / 2) / 2, .1);
    WireCoveredL2->DefineSection(13, 0.7, (.1 + (.040 + .025) / 2) / 2, .1);
    WireCoveredL2->DefineSection(14, 0.7, .03, .1);
    WireCoveredL2->DefineSection(15, 0.8, .03, .1);

    WireCoveredL3 = new TGeoPgon("WireCoveredLayer3", 0, 360, 200, 3);
    WireCoveredL3->DefineSection(0, 0, 0.45, 0.4975000);
    WireCoveredL3->DefineSection(1, 0.7, 0.45, 0.4975000);
    WireCoveredL3->DefineSection(2, 0.8, 0.45, 0.4975 - .0268); // tan(15 deg) = 0.268

    TGeoTranslation *t1 = new TGeoTranslation("t1", 0, 0, 0.5);
    t1->RegisterYourself();

    TGeoTube *GasTubeShape;
    GasTubeShape = new TGeoTube("gts", 0, .04, 1.0);
    TGeoTranslation *t2 = new TGeoTranslation("t2", 0, 0.35, 0.5); // position not sure yet
    t2->RegisterYourself();

    TGeoCompositeShape *EndCapShape = new TGeoCompositeShape("EndCapShape", "WireCoveredLayer3:t1 + WireCoveredLayer2 - gts:t2");
    EndCap = new TGeoVolume("EndCapShape", EndCapShape, gGeoManager->GetMedium("ABS"));
    EndCap->SetLineColor(8);

    TGeoTube *CrimpPinShape; //	tubeTemp->AddNode(MylarLayer,5,new TGeoTranslation("z", 0, 0, tubeLength/2 + 3.5));
    //	tubeTemp->AddNode(AlLayer2,6,new TGeoTranslation("z", 0, 0, tubeLength/2 + 3.5));
    //	outfile.close();
    //	tubeTemp->CheckOverlaps(0.000001);
    CrimpPinShape = new TGeoTube("CrimpPinShape", 0.002, (0.1 + (.040 + .025) / 2) / 2, crimpPinLength / 2);
    CrimpPin = new TGeoVolume("w1", CrimpPinShape, gGeoManager->GetMedium("copper"));
    CrimpPin->SetLineColor(25);
    TGeoTranslation *t3 = new TGeoTranslation("t3", 0, 0, 0.7 - crimpPinLength / 2);
    t3->RegisterYourself();

    // Creat simple ring (Instead of Spring)
    TGeoTube *SmallRingShape;
    SmallRingShape = new TGeoTube("SmallRingShape", 0.2, 0.23, .01748);
    SpringSmallRing = new TGeoVolume("SRS", SmallRingShape, gGeoManager->GetMedium("CuBe2"));
    SpringSmallRing->SetLineColor(5);

    TGeoTube *BigRingShape;
    BigRingShape = new TGeoTube("BigRingShape", 0.995 / 2 - .03, 0.995 / 2, .01748);
    SpringBigRing = new TGeoVolume("BRS", BigRingShape, gGeoManager->GetMedium("CuBe2"));
    SpringBigRing->SetLineColor(5);

    // Create RetainingRing
    RetainingRing = gGeoManager->MakeTubs("RRS", gGeoManager->GetMedium("PVC"), 0.115, 0.25, 0.0525, 125, 55);
    RetainingRing->SetLineColor(35);

    // Create wire inside a tube
    TGeoTube *WireShape;
    WireShape = new TGeoTube("WireShape", 0, 0.001, wireLength / 2);
    Wire = new TGeoVolume("Wire", WireShape, gGeoManager->GetMedium("ReW"));
    Wire->SetLineColor(15);

    std::string tempt5 = tubeType + "t5";
    TGeoTranslation *t5 = new TGeoTranslation(tempt5.c_str(), 0, 0, tubeLength / 2 + 0.35);
    t5->RegisterYourself();

    std::string tempt6 = tubeType + "t6";
    TGeoTranslation *t6 = new TGeoTranslation(tempt6.c_str(), 0, 0, -tubeLength / 2 - 0.35);
    t6->RegisterYourself();

    // Create Outer tube layer
    TGeoTube *AlLayerShape1;
    std::string tempal1 = tubeType + "AluminiumLayerShape1";
    AlLayerShape1 = new TGeoTube(tempal1.c_str(), 0.4975000, 0.4975030, tubeLength / 2);
    AlLayer1 = new TGeoVolume("al1", AlLayerShape1, gGeoManager->GetMedium("Aluminum"));
    AlLayer1->SetLineColor(10);

    TGeoTube *MylarShape;
    std::string tempml1 = tubeType + "MylarLayerShape1";
    MylarShape = new TGeoTube(tempml1.c_str(), 0.4975030, 0.5002030, tubeLength / 2);
    MylarLayer = new TGeoVolume("ml1", MylarShape, gGeoManager->GetMedium("mylar"));

    MylarLayer->SetLineColor(9);

    TGeoTube *AlLayerShape2;
    std::string tempal2 = tubeType + "AluminiumLayerShape2";
    AlLayerShape2 = new TGeoTube(tempal2.c_str(), 0.5002030, 0.5002060, tubeLength / 2);
    AlLayer2 = new TGeoVolume("al2", AlLayerShape2, gGeoManager->GetMedium("Aluminum"));
    AlLayer2->SetLineColor(10);

    // Create Gas layer
    //		std::string tempt4 = tubeType + "t4";
    //		TGeoTranslation *t4 = new TGeoTranslation(tempt4.c_str(), 0, 0,
    //				(tubeLength - 0.4) / 2.0 + 0.55);
    //		t4->RegisterYourself();//0.7 - crimpPinLength / 2
    std::string tempt4 = tubeType + "t4";
    TGeoTranslation *t4 = new TGeoTranslation(tempt4.c_str(), 0, 0, 0.7 - crimpPinLength / 2 - tubeLength / 2);
    t4->RegisterYourself(); //

    std::string temptr1 = tubeType + "tr1";
    TGeoCombiTrans *tr1 = new TGeoCombiTrans(temptr1.c_str(), 0, 0, tubeLength / 2 + .35, new TGeoRotation("d", 0, 180, 180));
    tr1->RegisterYourself();

    std::string tempt7 = tubeType + "t7";
    TGeoTranslation *t7 = new TGeoTranslation(tempt7.c_str(), 0, 0, -.7 + tubeLength / 2 + crimpPinLength / 2);
    t7->RegisterYourself();

    TGeoTube *NobleGasTubeShape;
    std::string tempgs = tubeType + "GasShape";
    NobleGasTubeShape = new TGeoTube(tempgs.c_str(), 0, 0.4970 - fac, (tubeLength - 0.4) / 2.0 - fac);
    // std::clockt cstart4 = std::clock();
    std::string tempStr = tempgs + " -( EndCapShape:" + tempt6 + "+ EndCapShape:" + temptr1 + "+ CrimpPinShape:" + tempt4 + " + CrimpPinShape:" + tempt7 + "+ WireShape)";
    TGeoCompositeShape *NobleGasShape = new TGeoCompositeShape("r", tempStr.c_str());
    NobleGas = new TGeoVolume("ArCO2Sensitive", NobleGasShape, gGeoManager->GetMedium("ArCO2"));
    NobleGas->SetLineColor(20);

    TGeoVolume *testVol;
    TGeoTube *testShape;
    testShape = new TGeoTube("testShape", 0, 0.995 / 2, 0.1);
    testVol = new TGeoVolume("e1", testShape, gGeoManager->GetMedium("ABS"));
    testVol->SetLineColor(8);
    //        cap->AddNode(testVol, 1);
    cap->AddNode(CrimpPin, 0, t3);
    cap->AddNode(EndCap, 1);
    cap->AddNode(SpringSmallRing, 2, new TGeoTranslation("t", 0, 0, 0.2));
    cap->AddNode(SpringSmallRing, 3, new TGeoTranslation("t", 0, 0, 0.3));
    cap->AddNode(SpringSmallRing, 4, new TGeoTranslation("t", 0, 0, 0.4));
    cap->AddNode(SpringBigRing, 5, new TGeoTranslation("t", 0, 0, 0.5 - .02));
    cap->AddNode(RetainingRing, 6, new TGeoTranslation("t", 0, 0, 0.045 + 0.0525));

    tubeTemp->AddNode(Wire, 1);
    tubeTemp->AddNode(cap, 2, t6);
    tubeTemp->AddNode(cap, 3, tr1);
    tubeTemp->AddNode(NobleGas, 0);
    tubeTemp->AddNode(AlLayer1, 4);
    tubeTemp->AddNode(MylarLayer, 5);
    tubeTemp->AddNode(AlLayer2, 6);
    tubeArr.push_back(tubeTemp);
    //		tubeTemp->CheckOverlaps(0.00001);
  }
  //
  //	top->AddNode(tubeArr[7], 1, new TGeoTranslation("z", 0, 1 * 20, 0));
  //	for (Int_t i = 0; i < 8; i++) {
  //		top->AddNode(tubeArr[i], 1, new TGeoTranslation("z", 0, i * 20, 0));
  //	}
  //
  // ****************************** Arrange tubes ******************************
  std::vector<TGeoVolume *> sectorArr;
  for (Int_t sector = 0; sector < 3; sector++) {

    Int_t Node = 0;
    TGeoVolume *geoTemp;
    // std::string name = "sector-" + to_string((sPart - 1) * (sPart - 1));
    std::string name = "sector-" + to_string(sector);

    geoTemp = new TGeoVolumeAssembly(name.c_str());
    for (Int_t mPart = 0; mPart < 8; mPart++) {

      // Int_t Node=nNode;

      Double_t startingPointRow, startingPointCol;
      Int_t sIndex[10][2], eIndex[10][2];
      Int_t parity = 0, nRow, stack;
      Double_t sttRadius;
      Double_t deg = 2.9;
      Int_t tDeg;
      Int_t dPar0 = 0, dPar1 = 0;
      Int_t layerStart; // 2[]={0,6,12,16,18,22};
      Double_t stackArraymPart7[8] = {0, 20, 19, 16, 15, 12, 9, 4};
      Double_t dPar0mPart7[8] = {0, -1, -1, 0, 0, 0, 0, -1};
      Double_t dPar1mPart7[8] = {0, -4, -2, 0, 0, 0, 0, -1};

      if (sector == 0) {
        dPar0 = -1;
        dPar1 = -4;
      } else if (sector == 2) {
        dPar0 = -4;
        dPar1 = -1;
      }

      if (mPart == 0) {
        if (sector == 1) {
          parity = 0;
          nRow = 6;
          layerStart = 0;
        } else {
          parity = 1;
          nRow = 5;
          layerStart = 1;
        }
        sttRadius = 17.563 - 0.878 * 3;
        stack = 9;
        tDeg = 0;
      }

      if (mPart == 1) {
        parity = 0;
        sttRadius = 21.076 - 0.878;
        nRow = 4;
        stack = 12;
        tDeg = 0;
        layerStart = 6;
      }

      if (mPart == 3) {
        parity = 1;
        tDeg = -1;
        sttRadius = 26.616 - 0.878 - 0.878;
        nRow = 2;
        stack = 15;
        layerStart = 12;
      }
      if (mPart == 2) {
        parity = 1;
        tDeg = 1;
        sttRadius = 24.724 - 0.878 - 0.878;
        nRow = 2;
        stack = 14;
        layerStart = 10;
      }

      if (mPart == 4) {
        parity = 1;
        tDeg = -1;
        sttRadius = 30.4 - 0.878 - 0.878;
        nRow = 2;
        stack = 17;
        layerStart = 16;
      }
      if (mPart == 5) {
        parity = 1;
        tDeg = 1;
        sttRadius = 28.508 - 0.878 - 0.878;
        nRow = 2;
        stack = 16;
        layerStart = 14;
      }
      if (mPart == 6) {
        parity = 1;
        sttRadius = 33.37 - 0.878 * 3;
        nRow = 4;
        stack = 18;
        tDeg = 0;
        layerStart = 18;
      }

      if (mPart == 7) {
        parity = 1;
        sttRadius = 35.126 - 0.878;
        nRow = 7;
        tDeg = 0;
        layerStart = 22;
      }
      for (Int_t i = parity; i < nRow + parity; i++) {
        if (i == 0 && mPart == 0) {
          sIndex[i][0] = 7;
          sIndex[i][1] = 7;
        } else if (i == 1 && mPart == 0) {
          sIndex[i][0] = 4;
          sIndex[i][1] = 4;
        } else {
          sIndex[i][0] = 0;
          sIndex[i][1] = 0;
        }

        if (mPart == 7) {
          eIndex[i][0] = stackArraymPart7[i];
          eIndex[i][1] = stackArraymPart7[i];
          if (sector == 0) {
            eIndex[i][0] = stackArraymPart7[i] + dPar0mPart7[i];
            eIndex[i][1] = stackArraymPart7[i] + dPar1mPart7[i];
          } else if (sector == 2) {
            eIndex[i][0] = stackArraymPart7[i] + dPar1mPart7[i];
            eIndex[i][1] = stackArraymPart7[i] + dPar0mPart7[i];
          }

        } else {
          eIndex[i][0] = stack + dPar0 - 4 * tDeg * tDeg;
          //- 4 * (1 - (1 + tDeg) / 2) * tDeg * tDeg- 4 * (1 + tDeg) / 2 * tDeg * tDeg;
          eIndex[i][1] = stack + dPar1 - 4 * tDeg * tDeg;
          //- 4 * (1 + tDeg) / 2 * tDeg * tDeg- 4 * (1 - (1 + tDeg) / 2) * tDeg * tDeg;
          if (i % 2 == 1)
            stack++;
        }
      }

      startingPointRow = sttRadius;

      for (Int_t i = parity; i < nRow + parity; i++) {
        Int_t p = 0;
        TGeoVolume *tubeLayer;
        Int_t layer = layerStart + i;
        if (parity == 1) {
          layer = layerStart + i - 1;
        }
        std::string tubeLayerName = "strawLayer-" + std::to_string(layer);
        tubeLayer = new TGeoVolumeAssembly(tubeLayerName.c_str());

        if (i % 2 == 0) {
          startingPointCol = tubeRadius;
        } else {
          startingPointCol = tubeRadius * 2;
          if (sIndex[i][0] == 0 || sIndex[i][1] == 0) {
            Int_t colindex = eIndex[i][1];
            p = 1;
            tubeLayer->AddNode(tubeArr[0], colindex, new TGeoCombiTrans("tr", startingPointRow + i * 0.878, 0, 0, new TGeoRotation("r", 0, deg * tDeg, 0)));
            Node++;
          }
        }

        Int_t sign = 1;
        for (Int_t l = 0; l <= 1; l++) {
          for (Int_t j = sIndex[i][l]; j < eIndex[i][l]; j++) {

            Int_t colindex;
            if (l == 0)
              colindex = (eIndex[i][1] - sIndex[i][1]) + p;
            //+ (1 + tDeg) / 2 * 6 * tDeg * tDeg;
            else
              colindex = (eIndex[i][1] - sIndex[i][1]) - 1;
            //+ (1 + tDeg) / 2 * 6 * tDeg * tDeg;
            colindex = colindex + (j - sIndex[i][l]) * sign;

            tubeLayer->AddNode(tubeArr[0], colindex,
                               new TGeoCombiTrans("tr", startingPointRow + i * 0.878, (startingPointCol + j * tubeRadius * 2) * sign, 0, new TGeoRotation("d", 0, deg * tDeg, 0)));
            Node++;
          }
          sign = -1;
        }

        if (tDeg != 0) {
          for (Int_t k = 2; k < 8; k++) {
            //						Int_t colindex = k - 2;
            //						if (tDeg == -1) {
            //							colindex = k - 2 + (eIndex[i][1] - sIndex[i][1])
            //									+ (eIndex[i][0] - sIndex[i][0]) + p;
            //						}
            TGeoCombiTrans *trad1 = new TGeoCombiTrans(
              "tr", startingPointRow + i * 0.878,
              -tDeg * ((startingPointCol + (k + eIndex[i][(1 + tDeg) / 2] - 2) * tubeRadius * 2) - (tubeLengthArr[1] - tubeLengthArr[k]) / 2 * sin(2.9 * PI / 180.0)),
              -(tubeLengthArr[1] - tubeLengthArr[k]) * cos(2.9 * PI / 180.0) / 2, new TGeoRotation("r", 0, deg * tDeg, 0));
            trad1->RegisterYourself();

            TGeoCombiTrans *trad2 = new TGeoCombiTrans(
              "tr", startingPointRow + i * 0.878,
              tDeg * ((startingPointCol + (k + eIndex[i][1 - (1 + tDeg) / 2] - 2) * tubeRadius * 2) - (tubeLengthArr[1] - tubeLengthArr[k]) / 2 * sin(2.9 * PI / 180.0)),
              (tubeLengthArr[1] - tubeLengthArr[k]) * cos(2.9 * PI / 180.0) / 2, new TGeoRotation("r", 0, deg * tDeg, 0));
            trad2->RegisterYourself();
            tubeLayer->AddNode(tubeArr[k], 1 - (1 + tDeg) / 2, trad1);
            Node++;

            tubeLayer->AddNode(tubeArr[k], (1 + tDeg) / 2, trad2);
            Node++;
          }
        }
        geoTemp->AddNode(tubeLayer, 0);
      }
    }
    sectorArr.push_back(geoTemp);
  }
  halfSide->AddNode(sectorArr[0], 0, new TGeoCombiTrans(0, 0, 4 + tubeLengthArr[0] / 2, new TGeoRotation("d", 0, 0, -60)));
  halfSide->AddNode(sectorArr[1], 0, new TGeoTranslation(0, 0, 4.0 + tubeLengthArr[0] / 2));
  halfSide->AddNode(sectorArr[2], 0, new TGeoCombiTrans(0, 0, 4 + tubeLengthArr[0] / 2, new TGeoRotation("d", 0, 0, 60)));

  halfSide->AddNode(constructMechanicalFrame(), 1, new TGeoTranslation(0, 0, 0));
  halfSide->AddNode(constructGasManifold(), 2, new TGeoTranslation(0, 0, -.5));

  sttVol->AddNode(halfSide, 0, new TGeoTranslation("t1", 0, 0, -34.7));
  sttVol->AddNode(halfSide, 1, new TGeoCombiTrans(0, 0, -34.7, new TGeoRotation("d", 0, 0, 180)));

  top->AddNode(sttVol, 0);
  top->Write();
  fi->Close();

  gGeoMan->Export("detectorGeometries/sttGeometry.root","sttGeo","vg"); 

  return 0;
}

void createSttGeometry()
{
  createstt();
  exit(0);
}
