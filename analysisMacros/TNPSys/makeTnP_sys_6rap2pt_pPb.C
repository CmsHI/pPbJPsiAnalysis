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

void makeTnP_sys_6rap2pt_pPb(int isPrompt=1)
{
  gROOT->Macro("../Style.C");

  TString fcoll;
  if(isPrompt==1) fcoll = "PR";
  else if(isPrompt==0) fcoll = "NP";

  TFile *f1 = new TFile("../AccEffSys/AccEffSys_pA_6rap2pt_newcut_nominal_Zvtx1_SF1_etOpt1.root","READ");

  TFile *wf = new TFile(Form("Total_TnP_sys_6rap2pt_Pbp_%s.root",fcoll.Data()),"recreate");
  TH2D *hTNP = (TH2D*) f1 -> Get("h2D_Acc_PR_pA");
  hTNP->SetName("htot");

  double bincontent1[6][2] = { 
                              {0.0470,0.0450},
                              {0.0311,0.0313},
                              {0.0209,0.0253},
                              {0.0076,0.0186},
                              {0.0140,0.0247},
                              {0.0250,0.0279}
                            };

  double bincontent2[6][2] = { 
                              {0.0470,0.0443},
                              {0.0308,0.0309},
                              {0.0209,0.0249},
                              {0.0066,0.0180},
                              {0.0146,0.0243},
                              {0.0247,0.0275}
                            };
  
  double bincontent[6][2];

  for(int i=0;i<6;i++)
  {
    for(int j=0;j<2;j++)
    {
      if(isPrompt==1)
      {
        bincontent[i][j] = bincontent1[i][j];
      }
      else if(isPrompt==0)
      {
        bincontent[i][j] = bincontent2[i][j];
      }
    }
  }

  for(int i=0;i<6;i++)
  {
    hTNP->SetBinContent(i+1,1,bincontent[i][0]);
    hTNP->SetBinError(i+1,1,0);
    hTNP->SetBinContent(i+1,2,bincontent[i][1]);
    hTNP->SetBinError(i+1,2,0);
  
    if(i==2 || i==3 || i==4) hTNP->SetBinContent(i+1,1,0);
  }

  wf->cd();
  hTNP->Write();
}
