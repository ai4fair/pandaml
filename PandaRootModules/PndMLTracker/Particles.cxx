/*
 * Particles.cxx
 *
 *  Created on: Nov 1, 2022
 *      Author: Adeel Akram
 */

#include "Particles.h"
#include "TVector3.h"
#include "TLorentzVector.h"


Particles::Particles()
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

Particles::~Particles() {

    /* Destructor */
}


/*
void Particles::SetMomentum(TVector3 m) {
    
    fPx = m.Px();
    fPy = m.Py();
    fPz = m.Pz(); 
}

void Particles::Set4Momentum(TLorentzVector m4) {
    
    fPx = m4.Px();
    fPy = m4.Py();
    fPz = m4.Pz();
    fE  = m4.E();
}

void Particles::SetStartVertex(TVector3 v) {
    
    fStartX = v.X();
    fStartY = v.Y();
    fStartZ = v.Z();
}
*/

ClassImp(Particles)
