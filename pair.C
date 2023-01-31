#ifdef __CLING__
#pragma cling optimize(0)
#endif
void pair()
{
//=========Macro generated from canvas: can/Histograms
//=========  (Sat Jan 28 11:24:37 2023) by ROOT version 6.26/08
   TCanvas *can = new TCanvas("can", "Histograms",83,76,600,500);
   can->SetHighLightColor(2);
   can->Range(50,-1.05,550,9.45);
   can->SetFillColor(0);
   can->SetBorderMode(0);
   can->SetBorderSize(2);
   can->SetFrameBorderMode(0);
   can->SetFrameBorderMode(0);
   
   TH1F *hc1__1 = new TH1F("hc1__1","Number of Hits",200,100,500);
   hc1__1->SetBinContent(39,1);
   hc1__1->SetBinContent(40,1);
   hc1__1->SetBinContent(49,1);
   hc1__1->SetBinContent(50,1);
   hc1__1->SetBinContent(51,1);
   hc1__1->SetBinContent(52,1);
   hc1__1->SetBinContent(53,1);
   hc1__1->SetBinContent(54,3);
   hc1__1->SetBinContent(55,2);
   hc1__1->SetBinContent(56,1);
   hc1__1->SetBinContent(57,2);
   hc1__1->SetBinContent(58,2);
   hc1__1->SetBinContent(59,1);
   hc1__1->SetBinContent(60,1);
   hc1__1->SetBinContent(61,3);
   hc1__1->SetBinContent(62,3);
   hc1__1->SetBinContent(63,5);
   hc1__1->SetBinContent(64,3);
   hc1__1->SetBinContent(65,7);
   hc1__1->SetBinContent(67,3);
   hc1__1->SetBinContent(69,8);
   hc1__1->SetBinContent(70,3);
   hc1__1->SetBinContent(71,4);
   hc1__1->SetBinContent(72,6);
   hc1__1->SetBinContent(73,5);
   hc1__1->SetBinContent(74,6);
   hc1__1->SetBinContent(75,5);
   hc1__1->SetBinContent(76,3);
   hc1__1->SetBinContent(77,7);
   hc1__1->SetBinContent(78,2);
   hc1__1->SetBinContent(79,2);
   hc1__1->SetBinContent(80,2);
   hc1__1->SetBinContent(82,1);
   hc1__1->SetBinContent(83,1);
   hc1__1->SetBinContent(86,1);
   hc1__1->SetBinContent(89,1);
   hc1__1->SetEntries(100);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *ptstats_LaTex = ptstats->AddText("hc1");
   ptstats_LaTex->SetTextSize(0.0368);
   ptstats_LaTex = ptstats->AddText("Entries = 100    ");
   ptstats_LaTex = ptstats->AddText("Mean  =  234.3");
   ptstats_LaTex = ptstats->AddText("Std Dev   =  18.61");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   hc1__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(hc1__1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hc1__1->SetLineColor(ci);
   hc1__1->GetXaxis()->SetTitle("No. of Hits");
   hc1__1->GetXaxis()->SetLabelFont(42);
   hc1__1->GetXaxis()->SetTitleOffset(1.5);
   hc1__1->GetXaxis()->SetTitleFont(42);
   hc1__1->GetYaxis()->SetTitle("Events");
   hc1__1->GetYaxis()->SetLabelFont(42);
   hc1__1->GetYaxis()->SetTitleFont(42);
   hc1__1->GetZaxis()->SetLabelFont(42);
   hc1__1->GetZaxis()->SetTitleOffset(1);
   hc1__1->GetZaxis()->SetTitleFont(42);
   hc1__1->Draw("");
   
   TH1F *hc2__2 = new TH1F("hc2__2","Number of Hits",200,100,500);
   hc2__2->SetBinContent(201,100);
   hc2__2->SetEntries(100);

   ci = TColor::GetColor("#000099");
   hc2__2->SetLineColor(ci);
   hc2__2->GetXaxis()->SetTitle("No. of Hits");
   hc2__2->GetXaxis()->SetLabelFont(42);
   hc2__2->GetXaxis()->SetTitleOffset(1);
   hc2__2->GetXaxis()->SetTitleFont(42);
   hc2__2->GetYaxis()->SetTitle("Events");
   hc2__2->GetYaxis()->SetLabelFont(42);
   hc2__2->GetYaxis()->SetTitleFont(42);
   hc2__2->GetZaxis()->SetLabelFont(42);
   hc2__2->GetZaxis()->SetTitleOffset(1);
   hc2__2->GetZaxis()->SetTitleFont(42);
   hc2__2->Draw("SAME");
   
   TH1F *hc3__3 = new TH1F("hc3__3","Number of Hits",200,100,500);
   hc3__3->SetBinContent(201,100);
   hc3__3->SetEntries(100);

   ci = TColor::GetColor("#000099");
   hc3__3->SetLineColor(ci);
   hc3__3->GetXaxis()->SetTitle("No. of Hits");
   hc3__3->GetXaxis()->SetLabelFont(42);
   hc3__3->GetXaxis()->SetTitleOffset(1);
   hc3__3->GetXaxis()->SetTitleFont(42);
   hc3__3->GetYaxis()->SetTitle("Events");
   hc3__3->GetYaxis()->SetLabelFont(42);
   hc3__3->GetYaxis()->SetTitleFont(42);
   hc3__3->GetZaxis()->SetLabelFont(42);
   hc3__3->GetZaxis()->SetTitleOffset(1);
   hc3__3->GetZaxis()->SetTitleFont(42);
   hc3__3->Draw("SAME");
   
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
