//void nc_totalComp()
{
  gStyle->SetOptStat(0);
  //gStyle->SetPaintTextFormat(".3f");
  gStyle->SetPaintTextFormat(".5f");
  
  TFile* f01 = TFile::Open("fitResHist_8rap9pt_pA_newcut_nominal_etOpt0.root");
  //TFile* f01 = TFile::Open("fitResHist_8rap9pt_pp_newcut_nominal_etOpt0.root");
  //TFile* f01 = TFile::Open("fitResHist_8rap9pt_pA_newcut_nominal_etOpt0.root");
  TFile* f02 = TFile::Open("../FittingResult_0817/fitResHist_8rap9pt_pA_newcut_nominal_etOpt0.root");
  
  TCanvas* c1 = new TCanvas("c1","",600,600);
  TH2D* h2D_01 = (TH2D*)f01->Get("h2D_nPrompt_Raw");
//  TH2D* h2D_01 = (TH2D*)f01->Get("h2D_nNonPrompt_Raw");
//  TH2D* h2D_01 = (TH2D*)f01->Get("h2D_bFraction");
  h2D_01->SetName("h2D_01");

  TH2D* h2D_02 = (TH2D*)f02->Get("h2D_nPrompt_Raw");
//  TH2D* h2D_02 = (TH2D*)f02->Get("h2D_nNonPrompt_Raw");
//  TH2D* h2D_02 = (TH2D*)f02->Get("h2D_bFraction");
  h2D_02->SetName("h2D_02");
  
  h2D_01->Divide(h2D_02);
  //h2D_01->SetMaximum(1.3);  
  //h2D_01->SetMaximum(1.7);  
  //h2D_01->Draw("text e colz");
  h2D_01->Draw("text colz");

}
