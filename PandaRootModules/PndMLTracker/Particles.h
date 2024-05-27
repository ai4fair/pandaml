/*
 * Particles.h
 *
 *  Created on: Nov 1, 2022
 *      Author: Adeel Akram
 */

#ifndef PNDTRACKERS_PNDMLTRACKER_PARTICLE_H_
#define PNDTRACKERS_PNDMLTRACKER_PARTICLE_H_

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"


class Particles // : public TObject
{

public:
    
    // Constructor
    Particles();
    
    // Destructor
    virtual ~Particles();
    
        
    /*
    void Print() {
       // FIXME: Not functioning properly
       cout << "\n Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV/c "
            << "\n Start Vertex (" << fStartX << ", " << fStartY << ", " << fStartZ << ") cm "
            << "\n Particle_Id:" << fParticleId
            << "\n Charge: " << fQ << " e"
            << "\n Pdgcode: " << fPdgCode
            << "\n StartTime: " << fStartT << " sec"
            << "\n IsGenCreated: " << fIsGenCreated
            << std::endl;
    }
    
    
    friend std::ostream &operator<<(std::ostream &out, Particles &p) {
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
    
    // FIXME: Why these functions defined in Particles.cxx doesn't work?
    //void SetMomentum(TVector3 m);
    //void Set4Momentum(TLorentzVector m4);
    //void SetStartVertex(TVector3 v);
    
    void SetMomentum(TVector3 m) { fPx=m.Px(); fPy=m.Py(); fPz=m.Pz(); }
    void Set4Momentum(TLorentzVector m4) { fPx=m4.Px(); fPy=m4.Py(); fPz=m4.Pz(); fE =m4.E(); }
    void SetStartVertex(TVector3 v) { fStartX=v.X(); fStartY=v.Y(); fStartZ=v.Z(); }
    
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
    
    ClassDef(Particles,1);
};

#endif /* PNDTRACKERS_PNDMLTRACKER_PARTICLE_H_ */

