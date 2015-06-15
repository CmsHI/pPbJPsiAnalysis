#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TLatex.h"
#include "stdio.h"
#include "../../tagAndProbe/commonUtility.h"
//last forward run is 211256

//const Double_t hfBins[] = {0, 20, 30, 1000}; //last entry is upper bound on last bin
//const Int_t nhfBins = 3;

//int returnHFBin(double hf);
TH2D* compareTwo2D(TH2D* h1=0 ,TH2D* h2=0, TString txt = "");
void quadSum2D(TH2D* h1=0 ,TH2D* h2=0);

void compileSysUnc(bool isPrompt=1){

  TH1::SetDefaultSumw2();

  TFile *inf1;  // Kisoo's nominal
  if (isPrompt)  inf1 = new TFile("systematics/EffCounting_8rap9pt2gev_PRMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_Calo_PAMu3_expo_nopairpt_widemass_finelowpt_nominal.root");
  else           inf1 = new TFile("systematics/EffCounting_8rap9pt2gev_NPMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_Calo_PAMu3_expo_nopairpt_widemass_finelowpt_nominal.root");

  TH2D* hNominal = (TH2D*)inf1->Get("h2D_Eff_pt_y");

  TFile *inf2; // Yeonju's nominal + spectra weight
  if (isPrompt)  inf2 = new TFile("../001_Acceptance_weight_8rap9pt2gev/EffCounting_8rap9pt2gev_PRMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_kyo01.root");
  else           inf2 = new TFile("../001_Acceptance_weight_8rap9pt2gev/EffCounting_8rap9pt2gev_NPMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_kyo01.root");

  TH2D* hNominal1 = (TH2D*)inf2->Get("h2D_Eff_noWeight_pt_y");
  hNominal1->SetName("Nominal_y");


  TH2D* hSpecWgt = (TH2D*)inf2->Get("h2D_Eff_pt_y");
  hSpecWgt->SetName("hSpecWgt");
  TH2D* hSpecWgt_sys = compareTwo2D(hSpecWgt,hNominal1,"Spectra Reweighted");
  

  TH2D* hTrackerEffVar = (TH2D*)hNominal1->Clone("hTrackEffVar");
  for ( int ix=1 ; ix<=hTrackerEffVar->GetNbinsX(); ix++) {
    for ( int iy=1 ; iy<=hTrackerEffVar->GetNbinsY(); iy++) {
      hTrackerEffVar->SetBinContent(ix,iy, hTrackerEffVar->GetBinContent(ix,iy) * 1.02);
    }
  }
  TH2D* hTrackEffVar_sys = compareTwo2D(hTrackerEffVar, hNominal1,"Tracking sys.");
  

  TFile *infTnp1;
  if (isPrompt)   infTnp1 = new TFile("systematics/Stat_Err_PRMCpythia_Pbp_RMS_5eta.root");
  else            infTnp1 = new TFile("systematics/Stat_Err_NPMCpythia_Pbp_RMS_5eta.root");

  //  TH2D* hTnp1Var = (TH2D*)infTnp1->Get("h2D_Eff_pt_y");  in infTnp1 file, the histogram is already the relative uncertainty
  //  hTnp1Var->SetName("tnp1Var");
  TH2D* hTnp1Var_sys = (TH2D*)infTnp1->Get("h2Dtoy");

  /*
  TFile *infTnp2;
  if (isPrompt)   infTnp2 = new TFile("systematics/EffCounting_8rap9pt2gev_PRMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_GTrack_PAMu3_probeMulti.root");
  else            infTnp2 = new TFile("systematics/EffCounting_8rap9pt2gev_NPMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_GTrack_PAMu3_probeMulti.root");

  TH2D* hTnp2Var = (TH2D*)infTnp2->Get("h2D_Eff_pt_y");
  hTnp2Var->SetName("tnp2Var");
  TH2D* hTnp2Var_sys = compareTwo2D(hTnp2Var,hNominal1,"Tnp2 Var VS Nominal");
  */


  cout << "Data/MC spectra reweighting for Acceptance"  << endl;
  TH2D* tot_sys = (TH2D*)hSpecWgt_sys->Clone("tot_sys");  // 1. Data/MC spectra
  if (isPrompt)       tot_sys->SetName("totEffSys_prompt");
  else                tot_sys->SetName("totEffSys_nonprompt");


  cout << "Tracking efficiency sys." << endl;
  quadSum2D(tot_sys,hTrackEffVar_sys);                    // 2. TnP Tracking


  cout << "T&P" << endl;
  quadSum2D(tot_sys,hTnp1Var_sys);                         // 3. TnP MuonID+Trigger
  
    
  TCanvas* c=  new TCanvas("cTot","", 500,500);
  tot_sys->Draw("colz TEXT45");



  for ( int ix=1 ; ix<=tot_sys->GetNbinsX(); ix++) {
    
    cout << endl << " x bin : " << tot_sys->GetXaxis()->GetBinLowEdge(ix) << endl;
    for ( int iy=1 ; iy<=tot_sys->GetNbinsY(); iy++) {
      cout << "    y bin : " << tot_sys->GetYaxis()->GetBinLowEdge(iy) << "   : ";
      cout << hNominal1->GetBinContent(ix,iy) << " $\pm$"<< hNominal1->GetBinError(ix,iy) << "  ";
      cout << tot_sys->GetBinContent(ix,iy) * 100 << endl;
    }
  }
  
  TFile* output = new TFile("total_sys_unc_from_efficiency.root","update");
  tot_sys->Write();
  output->Close();
}

TH2D* compareTwo2D(TH2D* h1, TH2D* h2, TString txt) {
  
  TH2D* hh1 = (TH2D*)h1->Clone(Form("%s_sys",h1->GetName()));
  TH2D* hh2 = (TH2D*)h2->Clone("hh2");
  
  TCanvas* c=  new TCanvas(Form("c_%s",h1->GetName()),"", 800,800);
  c->Divide(2,2);
  c->cd(1);
  hh1->DrawCopy("colz");
  c->cd(2);
  hh2->DrawCopy("colz");
  c->cd(3);
  hh1->Add(hh2,-1);
  hh1->Divide(hh2);
  hh1->SetAxisRange(-0.3,0.3,"Z");
  hh1->DrawCopy("colz TEXT45");
  c->cd(4);
  TH1D* htemp = new TH1D("htemp","",1,0,1);
  htemp->DrawCopy();
  drawText(txt.Data(), 0.3, 0.5);
  
  return hh1;
  
}

void quadSum2D(TH2D* h1 ,TH2D* h2) {
  for ( int ix=1 ; ix<=h1->GetNbinsX(); ix++) {
    for ( int iy=1 ; iy<=h1->GetNbinsY(); iy++) {
      double z1 = h1->GetBinContent(ix,iy);
      double z2 = h2->GetBinContent(ix,iy);
      h1->SetBinContent(ix,iy, sqrt(z1*z1 + z2*z2));
    }
  }  
  
}
