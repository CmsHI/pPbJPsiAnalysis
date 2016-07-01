//void nc_totalComp()
{
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat(".3f");
  //gStyle->SetPaintTextFormat(".5f");
  
  //TFile* f01 = TFile::Open("EffAna_8rap9pt_pp_PR_newcut_Zvtx0_SF0.root");
  //TFile* f02 = TFile::Open("EffAna_8rap9pt_pp_PR_newcut_Zvtx1_SF0.root");
  //TFile* f01 = TFile::Open("EffAna_8rap9pt_pp_PR_newcut_Zvtx1_SF0_test02.root");
  //TFile* f02 = TFile::Open("EffAna_8rap9pt_pp_PR_newcut_Zvtx0_SF0.root");
  //TFile* f01 = TFile::Open("EffAna_8rap9pt_Pbp_PR_newcut_Zvtx1_SF1.root");
  //TFile* f02 = TFile::Open("OldCutTest/EffAna_8rap9pt_Pbp_PR_newcut_Zvtx1_SF1.root");
  TFile* f01 = TFile::Open("EffAna_8rap9pt_pPb_PR_newcut_Zvtx1_SF1.root");
  TFile* f02 = TFile::Open("OldCutTest/EffAna_8rap9pt_pPb_PR_newcut_Zvtx1_SF1.root");
  
  TCanvas* c1 = new TCanvas("c1","",600,600);
  TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Eff_pt_y");
  h2D_01->SetName("h2D_01");

  TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Eff_pt_y");
  h2D_02->SetName("h2D_02");
  
  h2D_01->Divide(h2D_02);
  //h2D_01->SetMaximum(1.3);  
  //h2D_01->SetMaximum(1.7);  
  h2D_01->Draw("text e colz");

}
