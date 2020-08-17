/*
 * PndFeature.h
 *
 *  Created on: Nov 8, 2017
 *      Author: Michael Papenbrock
 */

#ifndef PNDTRACKERS_PNDMLTRACKER_PNDFEATURE_H_
#define PNDTRACKERS_PNDMLTRACKER_PNDFEATURE_H_

#include <set>
#include <map>
#include <vector>
#include <TVector3.h>

class PndFeature : public TObject {
public:
	PndFeature();
	virtual ~PndFeature();

	void Clear();

	void SetSectorID(int sectorID) {fsectorID = sectorID;}
	void AddMomentum(TVector3 momentum) {fmomenta.push_back(momentum);}
	void AddMomenta(std::vector<TVector3> momenta);
	void AddTubeID(int tubeID) {ftubeIDs.insert(tubeID);}
	void SetTubeIDs(std::set<int> tubeIDs) {ftubeIDs = tubeIDs;}
	void RaisePatternCount() {fcount++;}
	void SetPatternCount(int count) {fcount = count;}
	bool IsEmpty();

	std::set<int> GetTubeIDs() const {return ftubeIDs;}
	short GetSectorID() const {return fsectorID;}
	int GetCount() const {return fcount;}
	std::vector<TVector3> GetMomenta() const {return fmomenta;}

private:
	std::set<int> ftubeIDs;
	short fpdgCode;
	short fsectorID;
	int fcount;
	std::vector<TVector3> fmomenta;

	ClassDef(PndFeature,1);
};

#endif /* PNDTRACKERS_PNDMLTRACKER_PNDFEATURE_H_ */
