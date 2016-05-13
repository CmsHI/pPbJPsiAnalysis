#include "../SONGKYO.h"


void draw_2D_eff_fine(int MrapNpt=89, int isPA = 1, int accCutType =2, bool isPrompt = false, bool useZvtxWeight=false, bool useSF=false, TString szDir="dir_eff")
{
  gROOT->Macro("../Style2D.C");
  
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return; }
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="Pbp";
  else if (isPA==2) szPA="pPb";
  else {cout << "select among isPA = 0, 1, or 2 (pp, Pbp or pPb) "<< endl; return; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 1 or 2"<< endl; return; }  
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  const TString szFinal = Form("%s_%s_%s_%s",szBinning.Data(),szPA.Data(),szPrompt.Data(),szAccCut.Data());
  std::cout << "szFinal: " << szFinal << std::endl;

  TFile * f2D = new TFile(Form("EffAna_%s_Zvtx%d_SF%d.root",szFinal.Data(),(int)useZvtxWeight,(int)useSF));

  // --- read-in 2D hist
  TH2D* h2D_Den = (TH2D*)f2D->Get("h2D_Den_pt_y_fine");
  TH2D* h2D_Num = (TH2D*)f2D->Get("h2D_Num_pt_y_fine");
  TH2D* h2D_Eff = (TH2D*)f2D->Get("h2D_Eff_pt_y_fine");

  //latex box for beam, rapidity, pT info
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  //////////////////////////////////////////////////////////////////
  // --- Draw histograms
  TCanvas* c1 = new TCanvas("c1","c1",700,600);
  TPaletteAxis* pal; 
  c1->cd();
  h2D_Den->GetXaxis()->SetTitle("y_{lab}");
  h2D_Den->GetXaxis()->CenterTitle();
  h2D_Den->GetYaxis()->SetTitle("p_{T} (GeV/c)");
  h2D_Den->Draw("colz");
  c1->Update();
  pal = (TPaletteAxis*)h2D_Den->GetListOfFunctions()->FindObject("palette"); 
  pal->SetX2NDC(0.92);
  c1->Modified();
  c1->Update();
  c1->SaveAs(Form("%s/h2D_Den_%s_fine.pdf",szDir.Data(),szFinal.Data()));

  h2D_Num->GetXaxis()->SetTitle("y_{lab}");
  h2D_Num->GetXaxis()->CenterTitle();
  h2D_Num->GetYaxis()->SetTitle("p_{T} (GeV/c)");
  h2D_Num->Draw("colz");
  c1->Update();
  pal = (TPaletteAxis*)h2D_Num->GetListOfFunctions()->FindObject("palette"); 
  pal->SetX2NDC(0.92);
  c1->Modified();
  c1->Update();
  c1->SaveAs(Form("%s/h2D_Num_%s_fine.pdf",szDir.Data(),szFinal.Data()));

  h2D_Eff->GetXaxis()->SetTitle("y_{lab}");
  h2D_Eff->GetXaxis()->CenterTitle();
  h2D_Eff->GetYaxis()->SetTitle("p_{T} (GeV/c)");
  h2D_Eff->SetMaximum(1.);
  h2D_Eff->Draw("colz");
  c1->Update();
  pal = (TPaletteAxis*)h2D_Eff->GetListOfFunctions()->FindObject("palette"); 
  pal->SetX2NDC(0.92);
  c1->Modified();
  c1->Update();
  c1->SaveAs(Form("%s/h2D_Eff_%s_fine.pdf",szDir.Data(),szFinal.Data()));

  return;

}

