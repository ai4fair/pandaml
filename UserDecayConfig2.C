/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

void UserDecayConfig2() {
   cout << "Loading User Decay Config for Lambda0LambdaBar0"<< endl;  


   Float_t bratio[6];	 // you can set up to 6 possible decay modes!  (at least I think so)
   Int_t mode[6][3];     // and for every Decay mode up to 3 decay particles (at least I think so)
   
   
   // delete all possible decay modes ratios to 0;
   for (Int_t i = 0; i < 6; i++) {   
      bratio[i] = 0.;
      mode[i][0] = 0;
      mode[i][1] = 0;
      mode[i][2] = 0;
   }
   
   // now set your new decay modes
   // ****************************
   
   // in this example lambdas should decay into electrons!
   bratio[0] = 100.;                                      // only one decay mode => 100%
   mode[0][0] = 2212;                                     // proton
   mode[0][1] = -211;                                     // pi minus
   
   // this is the important methode for this macro implementet in the instance of the 
   // transport system used by the Virtual Monte Carlo system it changes the Decay 
   // modes and ratios for the specified particle (pdg code), all other decays stay 
   // untouched (at least I think so)
   gMC->SetDecayMode(3122,bratio,mode);  



   // lets do the same for Anti Lambdas 
   // *********************************
   
   // delete all possible loaded/definded decay modes and set ratios to 0;
   for (Int_t i = 0; i < 6; i++) {   
      bratio[i] = 0.;
      mode[i][0] = 0;
      mode[i][1] = 0;
      mode[i][2] = 0;
   }
   
   // now set your new decay modes 
   // ****************************
   
   // in this example anti lambdas should decay into electrons!
   
   bratio[0] = 100.;        // only one decay mode => 100%
   mode[0][0] = -2212;      // anti-proton
   mode[0][1] = 211;        // pi plus

   gMC->SetDecayMode(-3122,bratio,mode);

   std::cout<< "############# USER DECAY LOADED #############" << std::endl;

}
