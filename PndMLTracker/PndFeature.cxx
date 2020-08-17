/*
 * PndFeature.cxx
 *
 *  Created on: Nov 8, 2017
 *      Author: Michael Papenbrock
 */

#include "PndFeature.h"

ClassImp(PndFeature)

PndFeature::PndFeature() {
  Clear();


}

PndFeature::~PndFeature() {

}

void PndFeature::Clear() {
  ftubeIDs.clear();
  fmomenta.clear();
  fsectorID = -1;
  fcount = 0;
}
bool PndFeature::IsEmpty() {
  if (ftubeIDs.size() == 0) {
    return true;
  } else {
    return false;
  }
}

void PndFeature::AddMomenta(std::vector<TVector3> momentaVector) {
  fmomenta.insert(std::end(fmomenta), std::begin(momentaVector), std::end(momentaVector));
}
