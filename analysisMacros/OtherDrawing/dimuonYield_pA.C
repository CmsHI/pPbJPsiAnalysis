#include "SONGKYO.h"

void dimuonYield_pA(){
 
  gROOT->Macro("./Style2D.C");
  gStyle->SetTitleXOffset(1.10);
  gStyle->SetTitleYOffset(1.15);
  //gStyle->SetPadTickX(1);
  //gStyle->SetPadTickY(1);

  //// old cut
  //TString dirName = "Pbp_PRMC_oldacccut";
  //TString dirName = "Pbp_NPMC_oldacccut";
  //TString dirName = "Pbp_Data_oldacccut";
  //TString dirName = "pPb_PRMC_oldacccut";
  //TString dirName = "pPb_NPMC_oldacccut";
  //TString dirName = "pPb_Data_oldacccut";
  //// new cut
  //TString dirName = "Pbp_PRMC_newacccut";
  //TString dirName = "Pbp_NPMC_newacccut";
  //TString dirName = "Pbp_Data_newacccut";
  TString dirName = "pPb_PRMC_newacccut";
  //TString dirName = "pPb_NPMC_newacccut";
  //TString dirName = "pPb_Data_newacccut";
  
  //double nbin = 80;
  double nbin = 50;
  
  TChain* t01 = new TChain("myTree");
  t01->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCJPsiWithFSR_pa_1st_run_STARTHI53_V27_ext1_nocut.root");
  //t01->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCJPsiWithFSR_pa_2nd_run_STARTHI53_V27_ext1_nocut.root");
  //t01->Add("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_1st_run_210676-211256_GR_P_V43D_nocut.root");
  //t01->Add("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_1st_run_210498-210658_GR_P_V43F_nocut.root");
  //t01->Add("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_2nd_run_211313-211631_GR_P_V43D_nocut.root");

  TCanvas* c_tmp = new TCanvas("c_tmp","",300,300); //c_tmp->SetGridx(); c_tmp->SetGridy();
  TCanvas* c01 = new TCanvas("c01","",600,600); //c01->SetGridx(); c01->SetGridy();

  cout << "nbin = " << nbin << endl;
  TH2D* h2D_01 = new TH2D("h2D_01","J/#psi p_{T} vs y_{lab}^{2nd};J/#psi y_{lab}^{2nd};J/#psi p_{T} [GeV];",nbin,-2.4,2.4,nbin,0,9.);
  h2D_01->Sumw2();
  
  TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )";
  TCut recoPairCut = "Reco_QQ_sign==0 && Reco_QQ_VtxProb > 0.01";
  TCut massNarrow = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
  TCut massWide = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
  TCut ctauCut = "-1.5 < Reco_QQ_ctau && Reco_QQ_ctau < 3.0 && 0 < Reco_QQ_ctauErr && Reco_QQ_ctauErr < 1";
  TCut rapRange = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < 2.4";
  TCut ptRange = "Reco_QQ_4mom.Pt() > 0.0 && Reco_QQ_4mom.Pt() < 30.0";

  ////old cut
 TCut accOldRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 1.3 && Reco_QQ_mupl_4mom.Pt() >=3.3) || (1.3 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.2 && Reco_QQ_mupl_4mom.P() >=2.9) || (2.2 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt() >= 0.8)))";
TCut accOldRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 1.3 && Reco_QQ_mumi_4mom.Pt() >=3.3) || (1.3 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.2 && Reco_QQ_mumi_4mom.P() >=2.9) || (2.2 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt() >= 0.8)))";
  //// new cut
  TCut accNewRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 1.2 && Reco_QQ_mupl_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.1 && Reco_QQ_mupl_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mupl_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt() >= 1.3)))";
  TCut accNewRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 1.2 && Reco_QQ_mumi_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.1 && Reco_QQ_mumi_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mumi_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt() >= 1.3)))";

  //// muID
  TCut muIDPlus = "Reco_QQ_mupl_isHighPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
  TCut muIDMinus = "Reco_QQ_mumi_isHighPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  //TCut muIDPlus = "Reco_QQ_mupl_highPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
  //TCut muIDMinus = "Reco_QQ_mumi_highPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //// *** no cut at all
  TCut totalCut = "";
  //totalCut = "Reco_QQ_sign>=0";
  //// *** old cut
  //totalCut = trigCut && recoPairCut && massNarrow && rapRange && ptRange && accOldRecoPlus && accOldRecoMinus && muIDPlus && muIDMinus && ctauCut;
  //// *** new cut
  totalCut = trigCut && recoPairCut && massNarrow && rapRange && ptRange && accNewRecoPlus && accNewRecoMinus && muIDPlus && muIDMinus && ctauCut;
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  c_tmp->cd();  
  t01->Draw("Reco_QQ_4mom.Pt():Reco_QQ_4mom.Rapidity()>>h2D_01",totalCut,"colz");

  c01->cd();
  h2D_01->GetXaxis()->CenterTitle(1);
  h2D_01->GetYaxis()->CenterTitle(1);
  h2D_01->Draw("colz");
  cout << "entries = " << h2D_01->GetEntries() << endl;

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //// binning
  TLine* tPAbin01 = new TLine(-1.93, 0, -1.93, 9.);
  tPAbin01->SetLineWidth(4); tPAbin01->SetLineColor(kBlack); tPAbin01->SetLineStyle(2); tPAbin01->Draw();
  TLine* tPAbin02 = new TLine(-1.46, 0, -1.46, 9.);
  tPAbin02->SetLineWidth(4); tPAbin02->SetLineColor(kBlack); tPAbin02->SetLineStyle(2); tPAbin02->Draw();
  TLine* tPAbin03 = new TLine(-1.03, 0, -1.03, 9.);
  tPAbin03->SetLineWidth(4); tPAbin03->SetLineColor(kBlack); tPAbin03->SetLineStyle(2); tPAbin03->Draw();
  TLine* tPAbin04 = new TLine(-0.43, 0, -0.43 , 9.);
  tPAbin04->SetLineWidth(4); tPAbin04->SetLineColor(kBlack); tPAbin04->SetLineStyle(2); tPAbin04->Draw();
  TLine* tPAbin05 = new TLine(0.47, 0, 0.47, 9.);
  tPAbin05->SetLineWidth(4); tPAbin05->SetLineColor(kBlack); tPAbin05->SetLineStyle(1); tPAbin05->Draw();
  TLine* tPAbin06 = new TLine(1.37, 0, 1.37, 9.);
  tPAbin06->SetLineWidth(4); tPAbin06->SetLineColor(kBlack); tPAbin06->SetLineStyle(2); tPAbin06->Draw();
  TLine* tPAbin07 = new TLine(1.97, 0, 1.97, 9.);
  tPAbin07->SetLineWidth(4); tPAbin07->SetLineColor(kBlack); tPAbin07->SetLineStyle(2); tPAbin07->Draw();
  
  //// cut
  TLine* tPA01 = new TLine(-2.4 ,2., -1.93, 2.);
  tPA01->SetLineWidth(6); tPA01->SetLineColor(kRed); tPA01->Draw();
  //TLine* tPA02 = new TLine(-1.93 ,2., -1.93, 3.);
  TLine* tPA02 = new TLine(-1.93 ,2., -1.93, 4.);
  tPA02->SetLineWidth(6); tPA02->SetLineColor(kRed); tPA02->Draw();
  //TLine* tPA03 = new TLine(-1.93 ,3., -1.46, 3.);
  TLine* tPA03 = new TLine(-1.93 ,4., -1.46, 4.);
  tPA03->SetLineWidth(6); tPA03->SetLineColor(kRed); tPA03->Draw();
  //TLine* tPA04 = new TLine(-1.46 ,3., -1.46, 5.);
  TLine* tPA04 = new TLine(-1.46 ,4., -1.46, 5.);
  tPA04->SetLineWidth(6); tPA04->SetLineColor(kRed); tPA04->Draw();
  TLine* tPA05 = new TLine(-1.46 ,5., -1.03, 5.);
  tPA05->SetLineWidth(6); tPA05->SetLineColor(kRed); tPA05->Draw();
  TLine* tPA06 = new TLine(-1.03 ,5., -1.03, 6.5);
  tPA06->SetLineWidth(6); tPA06->SetLineColor(kRed); tPA06->Draw();
  TLine* tPA07 = new TLine(-1.03 ,6.5, 1.37, 6.5);
  tPA07->SetLineWidth(6); tPA07->SetLineColor(kRed); tPA07->Draw();
  //TLine* tPA08 = new TLine(1.37 ,6.5, 1.37, 3.);
  TLine* tPA08 = new TLine(1.37 ,6.5, 1.37, 4.);
  tPA08->SetLineWidth(6); tPA08->SetLineColor(kRed); tPA08->Draw();
  //TLine* tPA09 = new TLine(1.37 ,3, 1.97, 3.);
  TLine* tPA09 = new TLine(1.37 ,4, 1.97, 4.);
  tPA09->SetLineWidth(6); tPA09->SetLineColor(kRed); tPA09->Draw();
  //TLine* tPA10 = new TLine(1.97 ,3, 1.97, 2.);
  TLine* tPA10 = new TLine(1.97 ,4, 1.97, 2.);
  tPA10->SetLineWidth(6); tPA10->SetLineColor(kRed); tPA10->Draw();
  TLine* tPA11 = new TLine(1.97 ,2, 2.4, 2.);
  tPA11->SetLineWidth(6); tPA11->SetLineColor(kRed); tPA11->Draw();
  
  c01->SaveAs(Form("plot_dimuonYield_coarse/%s.pdf",dirName.Data()));

}
