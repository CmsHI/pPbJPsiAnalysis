#include <TStyle.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCut.h>
#include <TClonesArray.h>
#include <TRandom3.h>

#include <RooFit.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooArgSet.h>

#include <TStyle.h>
#include <TLatex.h>
#include <TDirectory.h>
#include <TCollection.h>
#include <TPostScript.h>

#include "commonUtility.h"

using namespace RooFit;
using namespace std;

void mcClosure() {
  // mc gen matching
  TFile *f1 = new TFile("mcSingleMuEff.root");
  TH1D* eff1[10];
    
    const int nEtaBin = 3; 
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++) {
      eff1[ieta] = (TH1D*)f1->Get(Form("hEff0_ieta%d_iv1",ieta));
      eff1[ieta]->SetLineColor(1);
      eff1[ieta]->SetMarkerColor(1);
      
    }
    
    // mc tag and probe
    TFile *f2 = new TFile("tnpRate.root"); 
    TGraphAsymmErrors *eff2[10];
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++) {
      eff2[ieta] = (TGraphAsymmErrors*)f2->Get(Form("eff_mc_MuIdAndTrig_etaBin%d",ieta));
      eff2[ieta]->SetLineColor(2);
      eff2[ieta]->SetMarkerColor(2);
    }
    TCanvas*c1 = new TCanvas("c1","",1000,400);
    c1->Divide(nEtaBin,1,0.0,0.0);
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++) {
      c1->cd(ieta);
      eff1[ieta]->SetAxisRange(0.0,1.2,"Y");
      eff1[ieta]->Draw();
      eff2[ieta]->Draw("same p");
    }
}





