#include "../SONGKYO.h"

void draw_ZvtxAfterWeight(){
  
  gROOT->Macro("../Style.C");
  
  ///TFile* f1 = TFile::Open("zVtx201604/zVtx_G1_pp_isPrompt1_isNewAccCut1_isOfficial1.root"); //Data
  //TFile* f2 = TFile::Open("EffAna_8rap9pt_pp_PR_newcut_Zvtx1_SF0.root"); // MC
  //TString sz01 = "pp Data";
  //TString sz02 = "pp PRMC reweighted";
  TFile* f1 = TFile::Open("zVtx201603/zVtx_G1_Pbp_isPrompt1_isNewAccCut1_isOfficial1.root"); //Data
  TFile* f2 = TFile::Open("EffAna_8rap9pt_Pbp_PR_newcut_Zvtx1_SF0.root"); // MC
  TString sz01 = "Pbp Data";
  TString sz02 = "Pbp PRMC reweighted";
  
  TCanvas* c1 = new TCanvas("c1","",600,600); c1->SetGridx(); c1->SetGridy();
  TH1D* h1 = (TH1D*)f1->Get("h1D_01");
  h1->SetName("h1");
  SetHistStyle(h1,1,0);
  h1->SetLineWidth(2);
  h1->SetFillColor(kRed+2);
  h1->SetFillStyle(3004);
  TH1D* h2 = (TH1D*)f2->Get("h1D_zVtx"); //weighted
  h2->SetName("h2");
  h2->Scale(1,"width");
  h2->Scale(1./h2->Integral());
  SetHistStyle(h2,2,3);
  h2->SetLineWidth(2);
  h2->SetFillColor(kBlue+1);
  h2->SetFillStyle(3005);
  h2->GetXaxis()->SetTitle("Z_{vtx} [cm]");
  h2->GetXaxis()->CenterTitle();
  h2->GetXaxis()->SetRangeUser(-30, 30);
  h2->GetYaxis()->SetRangeUser(0, 0.1);
  //h2->Draw("pe");
  //h1->Draw("pe same");
  h2->Draw("hist");
  h1->Draw("hist same");

  //TLegend *legUR = new TLegend(0.58, 0.82, 0.89, 0.94);
  TLegend *legUR = new TLegend(0.18, 0.80, 0.80, 0.92);
  SetLegendStyle(legUR);
  legUR->AddEntry(h1,sz01.Data(),"f");
  legUR->AddEntry(h2,sz02.Data(),"f");
  legUR->Draw();
  c1->SaveAs("dir_zvtxcomp/zvtxComp_Pbp.pdf");

  TCanvas* c2 = new TCanvas("c2","",600,600); c2->SetGridx(); c2->SetGridy();
  c2->cd();
  TH1D* hRatio = (TH1D*)h1->Clone("hRatio");
  hRatio->Divide(h2);
  hRatio->SetMinimum(0.0);
  hRatio->SetMaximum(2.0);
  hRatio->Draw("pe");
  c1->SaveAs("dir_zvtxcomp/zvtxRatio_Pbp.pdf");

  return;
}
