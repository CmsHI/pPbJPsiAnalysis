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

  TFile *inf1;
  if (isPrompt)  inf1 = new TFile("EffCounting_8rap9pt_PRMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1.root");
  else           inf1 = new TFile("EffCounting_8rap9pt_NPMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1.root");

  TH2D* hNominal = (TH2D*)inf1->Get("h2D_Eff_pt_y");

  TFile *inf2;
  if (isPrompt)  inf2 = new TFile("systematics/EffSys_SpectraVar_EffCounting_8rap9pt_PRMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1.root");
  else           inf2 = new TFile("systematics/EffSys_SpectraVar_EffCounting_8rap9pt_NPMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1.root");

  TH2D* hNominal1 = (TH2D*)inf2->Get("h2D_Eff_pt_y");
  hNominal1->SetName("spectraVar");
  TH2D* hNominal1_sys = compareTwo2D(hNominal1,hNominal,"Nominal_y VS Nominal");

  TH2D* hSpecWgt = (TH2D*)inf2->Get("h2D_Eff_weight_pt_y");
  hSpecWgt->SetName("hSpecWgt");
  TH2D* hSpecWgt_sys = compareTwo2D(hSpecWgt,hNominal,"Spectra Reweighted");


  TH2D* hTrackerEffVar = (TH2D*)hNominal->Clone("hTrackEffVar");
  for ( int ix=1 ; ix<=hTrackerEffVar->GetNbinsX(); ix++) {
    for ( int iy=1 ; iy<=hTrackerEffVar->GetNbinsY(); iy++) {
      hTrackerEffVar->SetBinContent(ix,iy, hTrackerEffVar->GetBinContent(ix,iy) * 1.03);
    }
  }
  TH2D* hTrackEffVar_sys = compareTwo2D(hTrackerEffVar, hNominal,"Tracking eff. sys.");
  

  TFile *inf3;
  if (isPrompt) inf3 = new TFile("systematics/EffSys_ZVtx_tnp_expo_PRMCpythia_Pbp_useCtErr_1_useDataDriven_V17_ZV_0_1_useZvtxStep1_0_Step2_1.root");
  else          inf3 = new TFile("systematics/EffSys_ZVtx_tnp_expo_NPMCpythia_Pbp_useCtErr_1_useDataDriven_V17_ZV_0_1_useZvtxStep1_0_Step2_1.root");

  TH2D* hNominal2 = (TH2D*)inf2->Get("h2D_Eff_pt_y");
  hNominal2->SetName("spectraVar");
  TH2D* hNominal2_sys = compareTwo2D(hNominal2,hNominal,"Nominal_tnp VS Nominal");
  
  TFile *infTnp;
  if (isPrompt)   infTnp = new TFile("systematics/EffSys_ZVtx_tnp_plo1_PRMCpythia_Pbp_useCtErr_1_useDataDriven_V17_ZV_1_1_useZvtxStep1_0_Step2_1.root");
  else            infTnp = new TFile("systematics/EffSys_ZVtx_tnp_pol1_NPMCpythia_Pbp_useCtErr_1_useDataDriven_V17_ZV_1_1_useZvtxStep1_0_Step2_1.root");

  TH2D* hTnpVar = (TH2D*)infTnp->Get("h2D_Eff_pt_y");
  hTnpVar->SetName("tnpMuIdTriggerVar");
  TH2D* hTnpVar_sys = compareTwo2D(hTnpVar,hNominal,"Tnp Var VS Nominal");



  TH2D* tot_sys = (TH2D*)hSpecWgt_sys->Clone("tot_sys");  // 1. Data/MC spectra
  if (isPrompt)       tot_sys->SetName("totEffSys_prompt");
  else                tot_sys->SetName("totEffSys_nonprompt");
  cout << "Data/MC spectra reweighting for Acceptance"  << endl;
  quadSum2D(tot_sys,hTrackEffVar_sys);                    // 2. TnP Tracking
  cout << "Tracking efficiency sys." << endl;
  quadSum2D(tot_sys,hTnpVar_sys);                         // 3. TnP MuonID+Trigger
  cout << "MuonID and Trigger efficiency sys." << endl;
  
    
  TCanvas* c=  new TCanvas("cTot","", 500,500);
  tot_sys->Draw("colz TEXT45");
  
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
