void UserDecayConfig1(){

cout << "Loading User Decay Config from macro" << endl;

Float_t bratio[6];
//Float_t bratio_XiRes[6];

Int_t mode_lam[6][3];
Int_t mode_lbar[6][3];


for (Int_t i=0; i<6; i++){

	bratio[i] = 0;
	mode_lam[i][0] = 0;
	mode_lam[i][1] = 0;
	mode_lam[i][2] = 0;

	mode_lbar[i][0] = 0;
	mode_lbar[i][1] = 0;
	mode_lbar[i][2] = 0;


}

//now set your new decay mode_lams ******************
//lambda decays into proton and pion

bratio[0] = 100;
mode_lam[0][0] = 2212; 
mode_lam[0][1] = -211;

mode_lbar[0][0] = -2212; 
mode_lbar[0][1] = 211;




//all other mode_lams are still set to 0

gMC->SetDecayMode(3122,bratio,mode_lam); //the lambda (PDG 3122) should decay according the defined ratio and mode_lam.

gMC->SetDecayMode(-3122,bratio,mode_lbar); //the lambdabar (PDG -3122) should decay according the defined ratio and mode_lbar.


}
