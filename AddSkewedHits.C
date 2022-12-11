	
void PndHoughTrackFinder::AddSkewedHits(int i)
{
  PndTrack track = fPndHoughMerge->GetMergedTrack(i);
  PndTrackCand trackCand = track.GetTrackCand();
  std::vector<double> circle = fPndHoughUtilities->getCircleFromPndTrack(track);
  FairLink FirstLink = trackCand.GetSortedHit(0);
  TVector3 Hitmin(fData->GetMapFairLinktoFairHit()[FirstLink]->GetX(), fData->GetMapFairLinktoFairHit()[FirstLink]->GetY(), fData->GetMapFairLinktoFairHit()[FirstLink]->GetZ());
  TVector3 Track(circle[0], circle[1], circle[2]);

  TVector2 PhiRange = calcPhiRange(trackCand);
  double phiLow = PhiRange.X();
  double phiHigh = PhiRange.Y();
  std::cout << "merged track " << i << std::endl;

  std::vector<int> sectors;
  sectors.clear();
  for (int j = 0; j < trackCand.GetNHits(); j++) {
    if (ioman->GetBranchName(trackCand.GetSortedHit(j).GetType()) != "STTHit")
      continue;
    PndSttHit *sttHit = (PndSttHit *)(fData->GetMapFairLinktoFairHit()[(FairLink)trackCand.GetSortedHit(j)]);
    std::cout << "merged track (" << sttHit->GetX() << "," << sttHit->GetY() << ")" << std::endl;
    int sectorId = fData->GetStrawMap()->GetSector(sttHit->GetTubeID());
    if (std::find(sectors.begin(), sectors.end(), sectorId) == sectors.end()) {
      sectors.push_back(sectorId);
    }
  }

  for (int j = 0; j < fCombinedSkewed->GetEntriesFast(); j++) {
    PndSttSkewedHit *SkewedSttHit = (PndSttSkewedHit *)fCombinedSkewed->At(j);
    double d =
      sqrt((SkewedSttHit->GetX() - circle[0]) * (SkewedSttHit->GetX() - circle[0]) + (SkewedSttHit->GetY() - circle[1]) * (SkewedSttHit->GetY() - circle[1])) - abs(circle[2]);
    std::pair<Int_t, Int_t> tubeIDs = SkewedSttHit->GetTubeIDs();
    std::cout << "Skewed stt hit (" << SkewedSttHit->GetX() << "," << SkewedSttHit->GetY() << ") has distance " << abs(d) << std::endl;
    if (abs(d) < fDistanceThresholdSTTCombinedSkewed) {

  PndSttHit *sttHit1 = (PndSttHit *)(fData->GetMapTubetoHit()[tubeIDs.first]);
  PndSttHit *sttHit2 = (PndSttHit *)(fData->GetMapTubetoHit()[tubeIDs.second]);
  std::cout << fData->GetMapTubetoLink()[tubeIDs.first] << std::endl;
  FairLink link1 = fData->GetMapTubetoLink()[tubeIDs.first];
  FairLink link2 = fData->GetMapTubetoLink()[tubeIDs.second];
  std::cout << "tube1: " << tubeIDs.first << " link1: " << link1 << " tube 2 " << tubeIDs.second << " link2 " << link2 << std::endl;
  /*
  
  std::vector<PndTrackCandHit> vec = trackCand.GetSortedHits();
  bool IsIn1 = kFALSE;
  bool IsIn2 = kFALSE;
  for (auto iter = vec.begin(); iter != vec.end(); iter++) {
    if ((FairLink)(*iter) == link1)
      IsIn1 = kTRUE;
  }
  for (auto iter = vec.begin(); iter != vec.end(); iter++) {
    if ((FairLink)(*iter) == link2)
      IsIn2 = kTRUE;
  }

  bool SameSectorRange1 = IsInPhiRange((FairHit *)sttHit1, phiLow, phiHigh);
  bool SameSectorRange2 = IsInPhiRange((FairHit *)sttHit2, phiLow, phiHigh);
  */
  auto iter1 = std::find_if(trackCand.GetSortedHits().begin(), trackCand.GetSortedHits().end(), [&link1](const PndTrackCandHit &obj) { return (FairLink)obj == link1; });
  auto iter2 = std::find_if(trackCand.GetSortedHits().begin(), trackCand.GetSortedHits().end(), [&link2](const PndTrackCandHit &obj) { return (FairLink)obj == link2; });
  auto iterSec1 = std::find(sectors.begin(), sectors.end(), fData->GetStrawMap()->GetSector(sttHit1->GetTubeID()));
  auto iterSec2 = std::find(sectors.begin(), sectors.end(), fData->GetStrawMap()->GetSector(sttHit2->GetTubeID()));
  // if skewed hit in sector range  and if skewed hit not in already existing track
  // auto iter1 = std::find_if(vec.begin(), vec.end(), [&link1](const PndTrackCandHit &obj) { return (FairLink)obj == link1; });
  // auto iter2 = std::find_if(vec.begin(), vec.end(), [&link2](const PndTrackCandHit &obj) { return (FairLink)obj == link2; });
  // auto iterSec1 = std::find(sectors.begin(), sectors.end(), fData->GetStrawMap()->GetSector(sttHit1->GetTubeID()));
  // auto iterSec2 = std::find(sectors.begin(), sectors.end(), fData->GetStrawMap()->GetSector(sttHit2->GetTubeID()));
  // if skewed hit in sector range  and if skewed hit not in already existing track
  
  /*
  if (SameSectorRange1) // iterSec1 != sectors.end())
    std::cout << "sttHit1 (" << sttHit1->GetX() << "," << sttHit1->GetY() << ") is within sector" << std::endl;
  if (!IsIn1)
    std::cout << "sttHit1 (" << sttHit1->GetX() << "," << sttHit1->GetY() << ") is still not found" << std::endl;
  if (SameSectorRange2) // iterSec2 != sectors.end())
    std::cout << "sttHit2 (" << sttHit2->GetX() << "," << sttHit2->GetY() << ") is within sector" << std::endl;
  if (!IsIn2)
    std::cout << "sttHit2 (" << sttHit2->GetX() << "," << sttHit2->GetY() << ") is still not found" << std::endl;
  */
  if (iterSec1 != sectors.end() && iter1 == trackCand.GetSortedHits().end()) {//!IsIn1){//
  //if (SameSectorRange1 && !IsIn1) {
    TVector3 Hit1(sttHit1->GetX(), sttHit1->GetY(), sttHit1->GetZ());
    double rho_temp1 = fPndHoughTrackCorrection->calc_rho(Hit1, Track, Hitmin);
    trackCand.AddHit(link1, rho_temp1);
    std::cout << "add skewed hit: " << link1 << " (" << sttHit1->GetX() << "," << sttHit1->GetY() << ")" << std::endl;
  }
  if (iterSec2 != sectors.end() && iter2 == trackCand.GetSortedHits().end()) {//!IsIn2){//
  //if (SameSectorRange2 && !IsIn2) {
    TVector3 Hit2(sttHit2->GetX(), sttHit2->GetY(), sttHit2->GetZ());
    double rho_temp2 = fPndHoughTrackCorrection->calc_rho(Hit2, Track, Hitmin);
    trackCand.AddHit(link2, rho_temp2);
    std::cout << "add skewed hit: " << link2 << " (" << sttHit2->GetX() << "," << sttHit2->GetY() << ")" << std::endl;
  }
}
  }

  track.SetTrackCand(trackCand);
  fPndHoughMerge->UpdateMergedTrack(i, track);
}


