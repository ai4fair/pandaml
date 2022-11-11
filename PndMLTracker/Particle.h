/*
 * Particle.h
 *
 *  Created on: Nov 1, 2022
 *      Author: Adeel Akram
 */

#ifndef PNDTRACKERS_PNDMLTRACKER_PARTICLE_H_
#define PNDTRACKERS_PNDMLTRACKER_PARTICLE_H_

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"


class Particle // : public TObject {

{
public:
    
    // Constructor
    Particle();
    
    // Destructor
    virtual ~Particle();
    
        
    
    void Print() {
    
       cout << "\n Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV/c "
            << "\n Start Vertex (" << fStartX << ", " << fStartY << ", " << fStartZ << ") cm "
            << "\n Particle_Id:" << fParticleId
            << "\n Charge: " << fQ << " e"
            << "\n Pdgcode: " << fPdgCode
            << "\n StartTime: " << fStartT << " sec"
            << "\n IsGenCreated: " << fIsGenCreated
            << std::endl;
    }
    
    /*
    friend std::ostream &operator<<(std::ostream &out, Particle &p) {
        // FIXME: together with Print() 
        out << "\n Particle_Id:" << p.GetParticleId() 
            << "\n Start Vertex (" << p.fStartX << ", " << p.GetVy() << ", " << p.GetVz() << ") cm "
            << "\n Momentum (" << p.GetPx() << ", " << p.GetPy() << ", " << p.GetPz() << ") GeV/c "
            << "\n Charge: " << p.GetCharge() << " e"
            << "\n Pdgcode: " << p.GetPdgcode()
            << "\n StartTime: " << p.GetStartTime() << " sec"
            << "\n IsGenCreated: " << p.GetIsGenCreated()
            << std::endl;

        return out;
    }
    */
    
    
    
    // Setters
    void SetStartTime(Double_t t) { fStartT=t; }
    void SetStartVertex(TVector3 pos3);
    void SetMomentum(TVector3 mom3);
    void Set4Momentum(TLorentzVector mom4);
    
    void SetParticleId(Int_t pid) { fParticleId=pid; }
    void SetCharge(Int_t q) { fQ=q; }
    void SetNHits(Int_t nhits) { fNHits=nhits; }
    void SetPdgCode(Int_t pdgcode) { fPdgCode=pdgcode; }
    void SetIsGenCreated(Bool_t IsGen) { fIsGenCreated=IsGen; }
    
    
    // Accessors
    Double_t GetStartTime() const { return fStartT; }
    TVector3 GetStartVertex() const { return TVector3(fStartX, fStartY, fStartZ); }
    TVector3 GetMomentum() const { return TVector3(fPx, fPy, fPz); }
    TLorentzVector Get4Momentum() const { return TLorentzVector(fPx, fPy, fPz, fE); }
    Double_t GetPt() const { return TMath::Sqrt(fPx * fPx + fPy * fPy); }
	
    Int_t GetParticleId() const { return fParticleId; }
    Int_t GetCharge() const { return fQ; }
    Int_t GetNHits() const { return fNHits; }
    Int_t GetPdgCode() const { return fPdgCode; }
    Bool_t GetIsGenCreated() const { return fIsGenCreated; }
    
private:
    
    Double32_t fPx, fPy, fPz, fE;
    Double32_t fStartX, fStartY, fStartZ, fStartT;
    Int_t fParticleId;
    Int_t fQ;
    Int_t fNHits;
    Int_t fPdgCode;
    Bool_t fIsGenCreated;
    
    ClassDef(Particle,1);
};

#endif /* PNDTRACKERS_PNDMLTRACKER_PARTICLE_H_ */

