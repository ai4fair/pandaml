#ifdef __CLING__
#pragma cling optimize(0)
#endif
void nhits()
{
//=========Macro generated from canvas: can/Histograms
//=========  (Wed Jan 25 18:59:59 2023) by ROOT version 6.26/08
   TCanvas *can = new TCanvas("can", "Histograms",108,101,600,500);
   can->SetHighLightColor(2);
   can->Range(50,-1485.225,550,13367.03);
   can->SetFillColor(0);
   can->SetBorderMode(0);
   can->SetBorderSize(2);
   can->SetFrameBorderMode(0);
   can->SetFrameBorderMode(0);
   
   TH1F *hc__1 = new TH1F("htemp","Number of Hits",100,100,500);
   hc__1->SetBinContent(4,1);
   hc__1->SetBinContent(6,2);
   hc__1->SetBinContent(7,2);
   hc__1->SetBinContent(8,2);
   hc__1->SetBinContent(9,2);
   hc__1->SetBinContent(10,4);
   hc__1->SetBinContent(11,3);
   hc__1->SetBinContent(12,9);
   hc__1->SetBinContent(13,14);
   hc__1->SetBinContent(14,28);
   hc__1->SetBinContent(15,33);
   hc__1->SetBinContent(16,45);
   hc__1->SetBinContent(17,80);
   hc__1->SetBinContent(18,105);
   hc__1->SetBinContent(19,171);
   hc__1->SetBinContent(20,257);
   hc__1->SetBinContent(21,355);
   hc__1->SetBinContent(22,476);
   hc__1->SetBinContent(23,678);
   hc__1->SetBinContent(24,978);
   hc__1->SetBinContent(25,1393);
   hc__1->SetBinContent(26,1871);
   hc__1->SetBinContent(27,2216);
   hc__1->SetBinContent(28,2761);
   hc__1->SetBinContent(29,3724);
   hc__1->SetBinContent(30,4762);
   hc__1->SetBinContent(31,6236);
   hc__1->SetBinContent(32,7187);
   hc__1->SetBinContent(33,7020);
   hc__1->SetBinContent(34,7218);
   hc__1->SetBinContent(35,8190);
   hc__1->SetBinContent(36,10038);
   hc__1->SetBinContent(37,11316);
   hc__1->SetBinContent(38,10577);
   hc__1->SetBinContent(39,6381);
   hc__1->SetBinContent(40,2773);
   hc__1->SetBinContent(41,1017);
   hc__1->SetBinContent(42,470);
   hc__1->SetBinContent(43,314);
   hc__1->SetBinContent(44,239);
   hc__1->SetBinContent(45,237);
   hc__1->SetBinContent(46,168);
   hc__1->SetBinContent(47,148);
   hc__1->SetBinContent(48,125);
   hc__1->SetBinContent(49,80);
   hc__1->SetBinContent(50,84);
   hc__1->SetBinContent(51,75);
   hc__1->SetBinContent(52,41);
   hc__1->SetBinContent(53,31);
   hc__1->SetBinContent(54,25);
   hc__1->SetBinContent(55,17);
   hc__1->SetBinContent(56,9);
   hc__1->SetBinContent(57,4);
   hc__1->SetBinContent(58,2);
   hc__1->SetBinContent(60,1);
   hc__1->SetBinContent(61,1);
   hc__1->SetBinContent(63,1);
   hc__1->SetBinContent(64,1);
   hc__1->SetBinContent(65,1);
   hc__1->SetBinContent(76,1);
   hc__1->SetEntries(100000);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *ptstats_LaTex = ptstats->AddText("hc");
   ptstats_LaTex->SetTextSize(0.0368);
   ptstats_LaTex = ptstats->AddText("Entries = 100000 ");
   ptstats_LaTex = ptstats->AddText("Mean  =  233.8");
   ptstats_LaTex = ptstats->AddText("Std Dev   =  18.45");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   hc__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(hc__1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hc__1->SetLineColor(ci);
   hc__1->GetXaxis()->SetTitle("No. of Hits/Event");
   hc__1->GetXaxis()->SetLabelFont(42);
   hc__1->GetXaxis()->SetTitleOffset(1.5);
   hc__1->GetXaxis()->SetTitleFont(42);
   hc__1->GetYaxis()->SetTitle("Counts");
   hc__1->GetYaxis()->SetLabelFont(42);
   hc__1->GetYaxis()->SetTitleFont(42);
   hc__1->GetZaxis()->SetLabelFont(42);
   hc__1->GetZaxis()->SetTitleOffset(1);
   hc__1->GetZaxis()->SetTitleFont(42);
   hc__1->Draw("");
   
   TPaveText *pt = new TPaveText(0.3461873,0.94,0.6538127,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *pt_LaTex = pt->AddText("Number of Hits");
   pt->Draw();
   can->Modified();
   can->cd();
   can->SetSelected(can);
}
