//void nc_totalComp()
{
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat(".3f");
  //gStyle->SetPaintTextFormat(".5f");
  
  //TFile* f01 = TFile::Open("totalHist_pA_8rap9pt_newcut_Zvtx1_SF1_noPtWeight_Raw.root");
  //TFile* f02 = TFile::Open("totalHist_pA_8rap9pt_newcut_Zvtx1_SF1_noPtWeight.root");
  //TFile* f01 = TFile::Open("totalHist_pp_8rap9pt_newcut_Zvtx0_SF0_noPtWeight_Raw.root");
  //TFile* f02 = TFile::Open("totalHist_pp_8rap9pt_newcut_Zvtx0_SF0_noPtWeight.root");
  //TFile* f01 = TFile::Open("totalHist_PbppPb_8rap9pt_oldcut_Zvtx1_SF1_noPtWeight_privFit_privMC_PASB_PASP.root");
  //TFile* f02 = TFile::Open("totalHist_PbppPb_8rap9pt_oldcut_Zvtx1_SF1_noPtWeight_privFit_privMC_PASB_PASP_zcut.root");
  TFile* f01 = TFile::Open("fitResHist_8rap9pt_Pbp_oldcut_privtest_PASB_PASP.root");
  TFile* f02 = TFile::Open("fitResHist_8rap9pt_Pbp_oldcut_privtest_PASB_PASP_zcut.root");
  
  TCanvas* c1 = new TCanvas("c1","",600,600);
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Fit_PR_pA");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Fit_NP_pA");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Fit_PR_pp");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Fit_NP_pp");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Fit_PR_Pbp");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Fit_NP_Pbp");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Fit_PR_pPb");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Fit_NP_pPb");
  TH2D* h2D_01 = (TH2D*)f01->Get("h2D_nSig");
  h2D_01->SetName("h2D_01");

  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Fit_PR_pA");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Fit_NP_pA");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Fit_PR_pp");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Fit_NP_pp");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Fit_PR_Pbp");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Fit_NP_Pbp");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Fit_PR_pPb");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Fit_NP_pPb");
  TH2D* h2D_02 = (TH2D*)f02->Get("h2D_nSig");
  h2D_02->SetName("h2D_02");
  
  h2D_01->Divide(h2D_02);
  //h2D_01->SetMaximum(1.3);  
  //h2D_01->SetMaximum(1.7);  
  h2D_01->Draw("text e colz");

}
