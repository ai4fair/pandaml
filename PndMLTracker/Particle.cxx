/*
 * Particle.cxx
 *
 *  Created on: Nov 1, 2022
 *      Author: Adeel Akram
 */

#include "Particle.h"
#include "TVector3.h"
#include "TLorentzVector.h"


Particle::Particle()
    : fPx(0)
    , fPy(0)
    , fPz(0)
    , fE(0)
    , fStartX(0)
    , fStartY(0)
    , fStartZ(0)
    , fStartT(0)
    , fParticleId(0)
    , fQ(0)
    , fNHits(1)
    , fPdgCode(0)
    , fIsGenCreated(0) {

    /* Constructor */
}

Particle::~Particle() {

    /* Destructor */
}


void Particle::SetMomentum(TVector3 mom3) {
    
    fPx = mom3.Px();
    fPy = mom3.Py();
    fPz = mom3.Pz(); 
}

void Particle::Set4Momentum(TLorentzVector mom4) {
    
    fPx = mom4.Px();
    fPy = mom4.Py();
    fPz = mom4.Pz();
    fE  = mom4.E()
}

void Particle::SetStartVertex(auto pos3) {
    
    std::cout << pos3.X() << "," << pos3.Y() << "," << pos3.Z() << std::endl;
     
    fStartX = pos3.X();
    fStartY = pos3.Y();
    fStartZ = pos3.Z();
}

ClassImp(Particle)
