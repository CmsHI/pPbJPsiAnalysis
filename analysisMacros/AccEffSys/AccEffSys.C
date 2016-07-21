#include <iostream>
#include <sstream>
#include "TH1.h"
#include "TROOT.h"
#include "TFile.h"
#include "TMath.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include "../SONGKYO.h"

using namespace std;

void AccEffSys(int isPA=0, int rapPt = 89, int opt=0, int zVtx = 0)
{
  gROOT->Macro("../Style.C");
  
  TString fcoll;
  if(isPA==0) fcoll = "pp";
  else if(isPA==1) fcoll = "pA";

  TString frapPt;
  if(rapPt == 89) frapPt = "8rap9pt";
  else if(rapPt == 62) frapPt = "6rap2pt";

  TFile *f1 = new TFile(Form("../FittingResult/totalHist_%s_%s_newcut_nominal_Zvtx%d_SF1_etOpt%d.root",fcoll.Data(),frapPt.Data(),zVtx,opt),"READ");
  TFile *f2 = new TFile(Form("../FittingResult/totalHist_%s_%s_newcut_nominal_Zvtx%d_SF1_etOpt%d_noPtWeight.root",fcoll.Data(),frapPt.Data(),zVtx,opt),"READ");
	
  TFile *wf = new TFile(Form("AccEffSys_%s_%s_newcut_nominal_Zvtx%d_SF1_etOpt%d.root",fcoll.Data(),frapPt.Data(),zVtx,opt),"recreate");

  TH2D* h2D_Acc_PR;
  TH2D* h2D_Acc_NP;
  TH2D* h2D_Eff_PR;
  TH2D* h2D_Eff_NP;

  TH2D* h2D_Acc_PR_noWeight;
  TH2D* h2D_Acc_NP_noWeight;
  TH2D* h2D_Eff_PR_noWeight;
  TH2D* h2D_Eff_NP_noWeight;


  if(isPA==0)
  {
    h2D_Acc_PR = (TH2D*) f1->Get("h2D_Acc_PR_pp");
    h2D_Acc_NP = (TH2D*) f1->Get("h2D_Acc_NP_pp");
    h2D_Acc_PR_noWeight = (TH2D*) f2->Get("h2D_Acc_PR_pp");
    h2D_Acc_NP_noWeight = (TH2D*) f2->Get("h2D_Acc_NP_pp");
    h2D_Eff_PR = (TH2D*) f1->Get("h2D_Eff_PR_pp");
    h2D_Eff_NP = (TH2D*) f1->Get("h2D_Eff_NP_pp");
    h2D_Eff_PR_noWeight = (TH2D*) f2->Get("h2D_Eff_PR_pp");
    h2D_Eff_NP_noWeight = (TH2D*) f2->Get("h2D_Eff_NP_pp");
  }
  else if(isPA==1)
  {
    h2D_Acc_PR = (TH2D*) f1->Get("h2D_Acc_PR_pA");
    h2D_Acc_NP = (TH2D*) f1->Get("h2D_Acc_NP_pA");
    h2D_Acc_PR_noWeight = (TH2D*) f2->Get("h2D_Acc_PR_pA");
    h2D_Acc_NP_noWeight = (TH2D*) f2->Get("h2D_Acc_NP_pA");
    h2D_Eff_PR = (TH2D*) f1->Get("h2D_Eff_PR_pA");
    h2D_Eff_NP = (TH2D*) f1->Get("h2D_Eff_NP_pA");
    h2D_Eff_PR_noWeight = (TH2D*) f2->Get("h2D_Eff_PR_pA");
    h2D_Eff_NP_noWeight = (TH2D*) f2->Get("h2D_Eff_NP_pA");
  }

  TH2D* h2D_Acc_PR_final = (TH2D*)h2D_Acc_PR->Clone(Form("h2D_Acc_PR_%s_final",fcoll.Data()));
  TH2D* h2D_Acc_NP_final = (TH2D*)h2D_Acc_NP->Clone(Form("h2D_Acc_NP_%s_final",fcoll.Data()));
  TH2D* h2D_Eff_PR_final = (TH2D*)h2D_Eff_PR->Clone(Form("h2D_Eff_PR_%s_final",fcoll.Data()));
  TH2D* h2D_Eff_NP_final = (TH2D*)h2D_Eff_NP->Clone(Form("h2D_Eff_NP_%s_final",fcoll.Data()));
  
  h2D_Acc_PR_final -> Add(h2D_Acc_PR_noWeight,-1);
  h2D_Acc_NP_final -> Add(h2D_Acc_NP_noWeight,-1);
  h2D_Eff_PR_final -> Add(h2D_Eff_PR_noWeight,-1);
  h2D_Eff_NP_final -> Add(h2D_Eff_NP_noWeight,-1);
 
  h2D_Acc_PR_final -> Divide(h2D_Acc_PR);
  h2D_Acc_NP_final -> Divide(h2D_Acc_NP);
  h2D_Eff_PR_final -> Divide(h2D_Eff_PR);
  h2D_Eff_NP_final -> Divide(h2D_Eff_NP);

  wf->cd();

  h2D_Acc_PR->SetName("h2D_Acc_PR_pp");
  h2D_Acc_PR->Write();
  h2D_Acc_PR_noWeight->SetName("h2D_Acc_PR_pp_noWeight");
  h2D_Acc_PR_noWeight->Write();

  h2D_Acc_PR_final->Write(); 
  h2D_Acc_NP_final->Write(); 
  h2D_Eff_PR_final->Write(); 
  h2D_Eff_NP_final->Write(); 

}

















  
