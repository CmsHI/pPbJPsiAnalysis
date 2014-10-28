#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>

#include <TROOT.h>
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
#include <math.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "TClonesArray.h"
#include <TAxis.h>
#include <cmath>
#include <TLorentzRotation.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>

#include "../commonUtility.h"



void drawEff_vs_vtx(){
  
  const int nVzBin = 4;
  int vzBin[nVzBin+1] = {0,3,6,9,12};


  TFile* f[20];
  TH2D* eff2d[20];
  TH1D* ptEff[20][20];
  TH1D* etaEff[20][20];
  TH1D* ptEffRatio[20][20];
  TH1D* etaEffRatio[20][20];

  int color[10] = {0,1,2,4,8,1,1,1,1};
  Double_t yBinsArr[] = {-2.4, -1.97, -1.47, -0.47, 0.53, 1.03, 1.93, 2.4}; // cross76
  int nRapBin = sizeof(yBinsArr)/sizeof(double)-1;

  for ( int ivz=1; ivz<=nVzBin; ivz++ ) {
    int low  = vzBin[ivz-1];
    int high = vzBin[ivz];
    f[ivz]= new TFile(Form("EffAna_PRMC_boosted_useCtErrRange0_useDataDriven0_useZvtxWeight0_mcOneWay_zVtx_%d-%d.root",high, low));
    eff2d[ivz] = (TH2D*)f[ivz]->Get("h2D_Eff_pt_y_Pbp");
    
    
    for ( int iy = 1 ; iy<=nRapBin; iy++) {
      ptEff[ivz][iy] = (TH1D*)eff2d[ivz]->ProjectionY(Form("_rapBin%d",iy), iy,iy,"e");
    }
    
  }

  TCanvas* c[20];
  for (int iy = 1; iy<=nRapBin ; iy++) {
    c[iy] = new TCanvas(Form("c_%d",iy),"",500,500);
    TLegend *lpt = new TLegend(0.4879032,0.220339,1,0.4682203,NULL,"brNDC");
    easyLeg(lpt,"z vertex (cm)");
    for ( int ivz=1; ivz<=nVzBin; ivz++ ) {
      handsomeTH1(ptEff[ivz][iy], color[ivz]);
      ptEff[ivz][iy]->SetAxisRange(0.01,1.01,"Y");
      ptEff[ivz][iy]->SetXTitle("J/psi p_{T} (GeV/c)");
      ptEff[ivz][iy]->SetYTitle("Efficiency");
      if ( ivz==1)  ptEff[ivz][iy]->Draw();
      else ptEff[ivz][iy]->Draw("same");
      lpt->AddEntry(ptEff[ivz][iy],Form("%d<z_{vtx}<=%d",vzBin[ivz-1], vzBin[ivz]));
    }
    lpt->Draw();
    drawText(Form("%.2f < y < %.2f", yBinsArr[iy-1], yBinsArr[iy]), 0.4, 0.85, 1,24);
  }
  
  // efficiency ratio
  for (int iy = 1; iy<=nRapBin ; iy++) {
    for ( int ivz=1; ivz<=nVzBin; ivz++ ) {
      ptEffRatio[ivz][iy] = (TH1D*)ptEff[ivz][iy]->Clone(Form("%s_ratio",ptEff[ivz][iy]->GetName()));
      ptEffRatio[ivz][iy]->Divide(ptEff[1][iy]);
    }
  }


  TCanvas* c1[20];
  for (int iy = 1; iy<=nRapBin ; iy++) {
    c1[iy] = new TCanvas(Form("c1_%d",iy),"",500,500);
    TLegend *lpt = new TLegend(0.4879032,0.220339,1,0.4682203,NULL,"brNDC");
    easyLeg(lpt,"z vertex (cm)");
    for ( int ivz=1; ivz<=nVzBin; ivz++ ) {
      handsomeTH1(ptEffRatio[ivz][iy], color[ivz]);
      ptEffRatio[ivz][iy]->SetAxisRange(0.5,1.5,"Y");
      ptEffRatio[ivz][iy]->SetXTitle("J/psi p_{T} (GeV/c)");
      ptEffRatio[ivz][iy]->SetYTitle("Eff. / Eff (z_{vtx} ~ 0cm)");
      if ( ivz==1)  ptEffRatio[ivz][iy]->Draw("hist");
      else ptEffRatio[ivz][iy]->Draw("same");
      lpt->AddEntry(ptEffRatio[ivz][iy],Form("%d<z_{vtx}<=%d",vzBin[ivz-1], vzBin[ivz]));
    }
    lpt->Draw();
    jumSun(0,1,30,1);
    drawText(Form("%.2f < y < %.2f", yBinsArr[iy-1], yBinsArr[iy]), 0.4, 0.85, 1,24);
  }
}
