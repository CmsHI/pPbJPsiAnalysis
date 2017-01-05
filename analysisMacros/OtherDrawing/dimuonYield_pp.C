#include "../SONGKYO.h"

void dimuonYield_pp(){
 
  gROOT->Macro("../Style2D.C");
  gStyle->SetTitleXOffset(1.10);
  gStyle->SetTitleYOffset(1.00);
  gStyle->SetPadBottomMargin(0.125);
  gStyle->SetPadRightMargin(0.125);
  gStyle->SetPadLeftMargin(0.11);
  //gStyle->SetPadTickX(1);
  //gStyle->SetPadTickY(1);

  //// old cut
  //TString dirName = "pp_PRMC_oldacccut";
  //TString dirName = "pp_NPMC_oldacccut";
  //TString dirName = "pp_Data_oldacccut";
  //// new cut
  TString dirName = "pp_PRMC_newacccut";
  //TString dirName = "pp_NPMC_newacccut";
  //TString dirName = "pp_Data_newacccut";
  
  //double nbin = 80;
  double nbin = 50;
  //double nbin = 40;
  
  TChain* t01 = new TChain("hionia/myTree");
  t01->Add("/home/samba/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1_Extended.root");
//  t01->Add("/home/samba/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
  ////t01->Add("/home/samba/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_BJpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
  ////t01->Add("/home/samba/OniaTree/Onia5TeV/ppData/OniaTree_DoubleMu_Run2015E-PromptReco-v1_Run_262157_262328_noCUT_TRKMU.root");
  
  TCanvas* c_tmp = new TCanvas("c_tmp","",300,300); //c_tmp->SetGridx(); c_tmp->SetGridy();
  TCanvas* c01 = new TCanvas("c01","",600,600); //c01->SetGridx(); c01->SetGridy();

  cout << "nbin = " << nbin << endl;
  TH2D* h2D_01 = new TH2D("h2D_01","J/#psi p_{T} vs y_{lab};J/#psi y_{lab};J/#psi p_{T} (GeV/c);",nbin,-2.4,2.4,nbin,0,9.);
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
  //TCut muIDPlus = "Reco_QQ_mupl_isHighPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
  //TCut muIDMinus = "Reco_QQ_mumi_isHighPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  TCut muIDPlus = "Reco_QQ_mupl_highPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
  TCut muIDMinus = "Reco_QQ_mumi_highPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";

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

  c01->Update();
  TPaletteAxis* pal; 
  pal = (TPaletteAxis*)h2D_01->GetListOfFunctions()->FindObject("palette"); 
  pal->SetX2NDC(0.915);
  c01->Modified();
  c01->Update();

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //// 1) y binning
  TLine* tPPbin01 = new TLine(-1.93, 0, -1.93, 9.);
  tPPbin01->SetLineWidth(4); tPPbin01->SetLineColor(kBlack); tPPbin01->SetLineStyle(2); tPPbin01->Draw();
  TLine* tPPbin02 = new TLine(-1.5, 0, -1.5, 9.);
  tPPbin02->SetLineWidth(4); tPPbin02->SetLineColor(kBlack); tPPbin02->SetLineStyle(2); tPPbin02->Draw();
  TLine* tPPbin03 = new TLine(-0.9, 0, -0.9, 9.);
  tPPbin03->SetLineWidth(4); tPPbin03->SetLineColor(kBlack); tPPbin03->SetLineStyle(2); tPPbin03->Draw();
  TLine* tPPbin04 = new TLine(-0., 0, 0. , 9.);
  tPPbin04->SetLineWidth(4); tPPbin04->SetLineColor(kBlack); tPPbin04->SetLineStyle(1); tPPbin04->Draw();
  TLine* tPPbin05 = new TLine(0.9, 0, 0.9, 9.);
  tPPbin05->SetLineWidth(4); tPPbin05->SetLineColor(kBlack); tPPbin05->SetLineStyle(2); tPPbin05->Draw();
  TLine* tPPbin06 = new TLine(1.5, 0, 1.5, 9.);
  tPPbin06->SetLineWidth(4); tPPbin06->SetLineColor(kBlack); tPPbin06->SetLineStyle(2); tPPbin06->Draw();
  TLine* tPPbin07 = new TLine(1.93, 0, 1.93, 9.);
  tPPbin07->SetLineWidth(4); tPPbin07->SetLineColor(kBlack); tPPbin07->SetLineStyle(2); tPPbin07->Draw();
  
  //// 2) pT cut
  TLine* tPP01 = new TLine(-2.4 ,2., -1.93, 2.);
  tPP01->SetLineWidth(6); tPP01->SetLineColor(kRed); tPP01->Draw();
  //TLine* tPP02 = new TLine(-1.93 ,2., -1.93, 3.);
  TLine* tPP02 = new TLine(-1.93 ,2., -1.93, 4.);
  tPP02->SetLineWidth(6); tPP02->SetLineColor(kRed); tPP02->Draw();
  //TLine* tPP03 = new TLine(-1.93 ,3., -1.5, 3.);
  TLine* tPP03 = new TLine(-1.93 ,4., -1.5, 4.);
  tPP03->SetLineWidth(6); tPP03->SetLineColor(kRed); tPP03->Draw();
  //TLine* tPP04 = new TLine(-1.5 ,3., -1.5, 6.5);
  TLine* tPP04 = new TLine(-1.5 ,4., -1.5, 6.5);
  tPP04->SetLineWidth(6); tPP04->SetLineColor(kRed); tPP04->Draw();
  TLine* tPP05 = new TLine(-1.5 ,6.5, 1.5, 6.5);
  tPP05->SetLineWidth(6); tPP05->SetLineColor(kRed); tPP05->Draw();
  //TLine* tPP06 = new TLine(1.5 ,6.5, 1.5, 3.);
  TLine* tPP06 = new TLine(1.5 ,6.5, 1.5, 4.);
  tPP06->SetLineWidth(6); tPP06->SetLineColor(kRed); tPP06->Draw();
  //TLine* tPP07 = new TLine(1.5 ,3, 1.93, 3.);
  TLine* tPP07 = new TLine(1.5 ,4, 1.93, 4.);
  tPP07->SetLineWidth(6); tPP07->SetLineColor(kRed); tPP07->Draw();
  //TLine* tPP08 = new TLine(1.93 ,3, 1.93, 2.);
  TLine* tPP08 = new TLine(1.93 ,4, 1.93, 2.);
  tPP08->SetLineWidth(6); tPP08->SetLineColor(kRed); tPP08->Draw();
  TLine* tPP09 = new TLine(1.93 ,2, 2.4, 2.);
  tPP09->SetLineWidth(6); tPP09->SetLineColor(kRed); tPP09->Draw();
  



  c01->SaveAs(Form("plot_dimuonYield/%s.pdf",dirName.Data()));

}
