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

void setAliases(TTree* t);

void drawSingleMuEff() {
  TH1::SetDefaultSumw2();
  //int nEntries = 50000;
  int nEntries = 1e9;
  // mc 
  TFile *fMc = new TFile("../oniaFiles/tot_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_noMuID_sglTrig_genMatch_20150205.root");
  TTree* t = (TTree*)fMc->Get("myTree");
  setAliases(t);

  // rapidity bins = 3
  const int nEta = 3;
  const int var = 3;     // 1:MuonId,  2: Trigger 3: MuId+Trigger
  TH1D* hTrMu1[nEta+1][var+1];
  TH1D* hQlfed1[nEta+1][var+1];
  TH1D* hEff1[nEta+1][var+1];
  TH1D* hTrMu2[nEta+1][var+1];
  TH1D* hQlfed2[nEta+1][var+1];
  TH1D* hEff2[nEta+1][var+1];
  TH1D* hTrMu0[nEta+1][var+1]; // mupl & mumi
  TH1D* hQlfed0[nEta+1][var+1];
  TH1D* hEff0[nEta+1][var+1];
  
  
  
  TCut accep1 = "(abs(recoEta1)<=1.3 && recoPt1>3.3) || ( abs(recoEta1)>1.3 && (abs(recoEta1)<=2.2) && recoP1 > 2.9) || (abs(recoEta1)>2.2 &&  abs(recoEta1)<=2.4 && recoPt1 > 0.8)";
  TCut accep2 = "(abs(recoEta2)<=1.3 && recoPt2>3.3) || ( abs(recoEta2)>1.3 && (abs(recoEta2)<=2.2) && recoP2 > 2.9) || (abs(recoEta2)>2.2 &&  abs(recoEta2)<=2.4 && recoPt2 > 0.8)";

  TCut accepGen1 = "(abs(genEta1)<=1.3 && genPt1>3.3) || ( abs(genEta1)>1.3 && (abs(genEta1)<=2.2) && genP1 > 2.9) || (abs(genEta1)>2.2 &&  abs(genEta1)<=2.4 && genPt1 > 0.8)";
  TCut accepGen2 = "(abs(genEta2)<=1.3 && genPt2>3.3) || ( abs(genEta2)>1.3 && (abs(genEta2)<=2.2) && genP2 > 2.9) || (abs(genEta2)>2.2 &&  abs(genEta2)<=2.4 && genPt2 > 0.8)";


  TString etaCut1="";
  TString etaCut2= "";
  TString etaCutGen1= "";
  TString etaCutGen2= "";

  TCut trackCut1 = "";
  TCut trackCut2 = "";
  
  TCut theCut1 = "";
  TCut theCut2 = "";

  TCut jPsiKineCut = "";
  //TCut jPsiKineCut = "";
  
  TCanvas* c1 = new TCanvas("c1","",1200,800);
  c1->Divide(3,2);

  int iv = 1;
  for ( int ieta = 1; ieta<=nEta ; ieta++) { 

    float etaBin[nEta+1] = {0, 1.2, 1.6, 2.4};
    etaCut1   = Form( "abs(recoEta1)>%.2f && abs(recoEta1)<=%.2f", etaBin[ieta-1], etaBin[ieta]);
    etaCut2   = Form( "abs(recoEta2)>%.2f && abs(recoEta2)<=%.2f", etaBin[ieta-1], etaBin[ieta]);
    etaCutGen1   = Form( "abs(genEta1)>%.2f && abs(genEta1)<=%.2f", etaBin[ieta-1], etaBin[ieta]);
    etaCutGen2   = Form( "abs(genEta2)>%.2f && abs(genEta2)<=%.2f", etaBin[ieta-1], etaBin[ieta]);
    
    
    
    if ( iv == 1) {  
      theCut1 = "Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_isHighPurity && (Reco_QQ_mupl_nPixWMea>0) && (Reco_QQ_mupl_nTrkWMea>5) && (Reco_QQ_mupl_dxy<0.3) && (Reco_QQ_mupl_dz<20) &&(Reco_QQ_mupl_trig&1) ";
      theCut2 = "Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_isHighPurity && (Reco_QQ_mumi_nPixWMea>0) && (Reco_QQ_mumi_nTrkWMea>5) && (Reco_QQ_mumi_dxy<0.3) && (Reco_QQ_mumi_dz<20) &&(Reco_QQ_mumi_trig&1)";
    }
    
    
    hTrMu1[ieta][iv] = new TH1D(Form("hTrMu1_ieta%d_iv%d",ieta,iv),";p_{T} (GeV/c) ; Efficiency",50,0,20);
    hQlfed1[ieta][iv] = (TH1D*)hTrMu1[ieta][iv]->Clone(Form("hQlfed1_ieta%d_iv%d",ieta,iv));
    hEff1[ieta][iv] = (TH1D*)hTrMu1[ieta][iv]->Clone(Form("hEff1_ieta%d_iv%d",ieta,iv));
    hTrMu2[ieta][iv] = (TH1D*)hTrMu1[ieta][iv]->Clone(Form("hTrMu2_ieta%d_iv%d",ieta,iv));
    hQlfed2[ieta][iv] = (TH1D*)hTrMu1[ieta][iv]->Clone(Form("hQlfed2_ieta%d_iv%d",ieta,iv));
    hEff2[ieta][iv] = (TH1D*)hTrMu1[ieta][iv]->Clone(Form("hEff2_ieta%d_iv%d",ieta,iv));
    hTrMu0[ieta][iv] = (TH1D*)hTrMu1[ieta][iv]->Clone(Form("hTrMu0_ieta%d_iv%d",ieta,iv));
    hQlfed0[ieta][iv] = (TH1D*)hTrMu1[ieta][iv]->Clone(Form("hQlfed0_ieta%d_iv%d",ieta,iv));
    hEff0[ieta][iv] = (TH1D*)hTrMu1[ieta][iv]->Clone(Form("hEff0_ieta%d_iv%d",ieta,iv));
    
    c1->cd(ieta);
    
    t->Draw( Form("recoPt1>>%s", hQlfed1[ieta][iv]->GetName()), jPsiKineCut && accep1 && etaCut1 && theCut1 && trackCut1,"", nEntries);
    t->Draw( Form("recoPt1>>%s", hTrMu1[ieta][iv]->GetName()),  jPsiKineCut && accep1 && etaCut1,"", nEntries);
    t->Draw( Form("recoPt2>>%s", hQlfed2[ieta][iv]->GetName()), jPsiKineCut && accep2 && etaCut2 && theCut2 && trackCut2,"", nEntries);
    t->Draw( Form("recoPt2>>%s", hTrMu2[ieta][iv]->GetName()),  jPsiKineCut && accep2 && etaCut2,"", nEntries);
    //  t->Draw( Form("genPt1>>%s",  hTrMu1 [ieta][iv]->GetName()), accepGen1 && etaCutGen1                     ,"", nEntries) ;
    
    //    cout << " cut1 = " << (TCut)(accep1&&etaCut1&&theCut1&&trackCut1).GetTitle() << endl;
    //   cout << " cut2 = " << (TCut)(accep2&&etaCut2&&theCut2&&trackCut2).GetTitle() << endl;
    
    hTrMu1[ieta][iv]->DrawCopy( "hist");
    hQlfed1[ieta][iv]->DrawCopy("same");
    
    handsomeTH1(hTrMu2[ieta][iv],4);
    handsomeTH1(hQlfed2[ieta][iv],4);
    
    hTrMu2[ieta][iv]->DrawCopy("hist same");
    hQlfed2[ieta][iv]->DrawCopy("same");
    
    c1->cd(ieta+nEta);
    
    hEff1[ieta][iv]->Divide( hQlfed1[ieta][iv], hTrMu1[ieta][iv], 1, 1, "B");
    hEff2[ieta][iv]->Divide( hQlfed2[ieta][iv], hTrMu2[ieta][iv], 1, 1, "B");
    
    hQlfed0[ieta][iv]->Add(hQlfed1[ieta][iv]);
    hQlfed0[ieta][iv]->Add(hQlfed2[ieta][iv]);
    hTrMu0[ieta][iv]->Add(hTrMu1[ieta][iv]);
    hTrMu0[ieta][iv]->Add(hTrMu2[ieta][iv]);
    hEff0[ieta][iv]->Divide( hQlfed0[ieta][iv], hTrMu0[ieta][iv], 1, 1, "B");
    
    handsomeTH1(hEff2[ieta][iv],4);
    hEff1[ieta][iv]->Draw();
    hEff2[ieta][iv]->Draw("same");
    
  }
  
  TFile* fout = new TFile("mcSingleMuEff.root","create");
  for ( int ieta = 1; ieta<=nEta ; ieta++) {
    hEff0[ieta][iv]->Write();
    hEff1[ieta][iv]->Write();
    hEff2[ieta][iv]->Write();
    hQlfed0[ieta][iv]->Write();
    hQlfed1[ieta][iv]->Write();
    hQlfed2[ieta][iv]->Write();
    hTrMu0[ieta][iv]->Write();
    hTrMu1[ieta][iv]->Write();
    hTrMu2[ieta][iv]->Write();

  }
  fout->Close();
}

 
 
void setAliases(TTree* t) {
  t->SetAlias("recoP1",  "Reco_QQ_mupl_4mom.P()");
  t->SetAlias("recoPt1", "Reco_QQ_mupl_4mom.Pt()");
  t->SetAlias("recoEta1","Reco_QQ_mupl_4mom.Eta()");
  t->SetAlias("genP1",  "Gen_QQ_mupl_4mom.P()");
  t->SetAlias("genPt1", "Gen_QQ_mupl_4mom.Pt()");
  t->SetAlias("genEta1","Gen_QQ_mupl_4mom.Eta()");

  t->SetAlias("recoP2",  "Reco_QQ_mumi_4mom.P()");
  t->SetAlias("recoPt2", "Reco_QQ_mumi_4mom.Pt()");
  t->SetAlias("recoEta2","Reco_QQ_mumi_4mom.Eta()");
  t->SetAlias("genP2",  "Gen_QQ_mumi_4mom.P()");
  t->SetAlias("genPt2", "Gen_QQ_mumi_4mom.Pt()");
  t->SetAlias("genEta2","Gen_QQ_mumi_4mom.Eta()");

  t->SetAlias("recoJpPt",  "Reco_QQ_4mom.Pt()");
  t->SetAlias("recoJpRap",  "Reco_QQ_4mom.Y()");

  
  
}
 


