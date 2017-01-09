//void nc_totalComp()
{
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat(".3f");
  //gStyle->SetPaintTextFormat(".5f");
  
  //TFile* f01 = TFile::Open("fitResHist_8rap9pt_pA_newcut_sys01_01_etOpt0.root ");
  //TFile* f02 = TFile::Open("/home/songkyo/kyo/pPbJPsiAnalysis_20161231/analysisMacros/FittingSys/fitResHist_8rap9pt_pA_newcut_sys01_01_etOpt0.root");
  //TFile* f01 = TFile::Open("fitResHist_8rap9pt_pA_newcut_sys01_01.root");
  //TFile* f02 = TFile::Open("fitResHist_8rap9pt_pA_newcut_sys04_01.root");
  
  //TFile* f01 = TFile::Open("fitSysErr_8rap9pt_pp_newcut_etOpt0_max.root");
  //TFile* f02 = TFile::Open("/home/songkyo/kyo/pPbJPsiAnalysis_20161231/analysisMacros/FittingSys/fitSysErr_8rap9pt_pp_newcut_etOpt0_max.root");
  //TFile* f01 = TFile::Open("fitSysErr_8rap9pt_pA_newcut_etOpt0_max.root");
  //TFile* f02 = TFile::Open("/home/songkyo/kyo/pPbJPsiAnalysis_20161231/analysisMacros/FittingSys/fitSysErr_8rap9pt_pA_newcut_etOpt0_max.root");
  //TFile* f01 = TFile::Open("fitSysErr_6rap2pt_pA_newcut_etOpt1_max.root");
  //TFile* f02 = TFile::Open("/home/songkyo/kyo/pPbJPsiAnalysis_20161231/analysisMacros/FittingSys/fitSysErr_6rap2pt_pA_newcut_etOpt1_max.root");
  //TFile* f01 = TFile::Open("fitSysErr_6rap2pt_pA_newcut_etOpt2_max.root");
  //TFile* f02 = TFile::Open("/home/songkyo/kyo/pPbJPsiAnalysis_20161231/analysisMacros/FittingSys/fitSysErr_6rap2pt_pA_newcut_etOpt2_max.root");
  TFile* f01 = TFile::Open("fitSysErr_6rap2pt_pA_newcut_etOpt3_max.root");
  TFile* f02 = TFile::Open("/home/songkyo/kyo/pPbJPsiAnalysis_20161231/analysisMacros/FittingSys/fitSysErr_6rap2pt_pA_newcut_etOpt3_max.root");
  
  TCanvas* c1 = new TCanvas("c1","",600,600);
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_PR_tot");
  TH2D* h2D_01 = (TH2D*)f01->Get("h2D_NP_tot");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_nPrompt_Raw");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_nNonPrompt_Raw");
  h2D_01->SetName("h2D_01");

  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_PR_tot");
  TH2D* h2D_02 = (TH2D*)f02->Get("h2D_NP_tot");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_nPrompt_Raw");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_nNonPrompt_Raw");
  h2D_02->SetName("h2D_02");
  
  h2D_01->Divide(h2D_02);
  //h2D_01->SetMaximum(1.3);  
  //h2D_01->SetMaximum(1.7);  
  //h2D_01->Draw("text e colz");
  h2D_01->Draw("text colz");

}
