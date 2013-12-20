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
#include <TCut.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include "commonUtility.h"
// Basically our basic frame is collision frame, in this code all value means in collision frame.


void mcClosure(bool  isFW=1, bool isPrompt=1) { 
  
  TString direction;   
  if (isFW) direction="FW";
  else direction     ="BW";
				    
  TH1::SetDefaultSumw2();

  TFile* f1;
  if ( isPrompt)  f1 = new TFile("/home/samba/songkyo/efficiency/PRMC_eff_rootFile/merged_PRMC_eff_Histos.root"); 
  else f1 = new TFile("/home/samba/songkyo/efficiency/NPMC_eff_rootFile/merged_NPMC_eff_Histos.root");
  
  //read-in tree
  TTree *mytree = (TTree*)f1->Get("myTree");
  
  TFile * fEff;
  if ( isPrompt) fEff = new TFile("EffAna_PRMC_boosted.root");
  else           fEff = new TFile("EffAna_NPMC_boosted.root");

  TH1D* hEff[10];
  hEff[0] = (TH1D*)fEff->Get(Form("h1D_EffDimu_pt_bin_%s",direction.Data()));
  hEff[1] = (TH1D*)fEff->Get(Form("h1D_EffDimu_pt_bin_%s_trig",direction.Data()));
  hEff[2] = (TH1D*)fEff->Get(Form("h1D_EffDimu_pt_bin_%s_mupl",direction.Data()));
  hEff[3] = (TH1D*)fEff->Get(Form("h1D_EffDimu_pt_bin_%s_mumi",direction.Data()));
  hEff[4] = (TH1D*)fEff->Get(Form("h1D_EffDimu_pt_bin_%s_jpsi",direction.Data()));
  hEff[5] = (TH1D*)fEff->Get(Form("h1D_EffDimu_pt_bin_%s_mass",direction.Data()));
  cout << "///////////////////////////////////////////////////////////////////////" << endl;
  cout << " Acceptance is calculated by YONGSUN's code not from Hyunchul's code" << endl;
  cout << "///////////////////////////////////////////////////////////////////////" << endl;
  TH1D* hAcc  = (TH1D*)fEff->Get(Form("h1D_Acc_pt_bin_%s",direction.Data()));
  
  
  for ( int i=0 ; i<=5 ;i++) {
    handsomeTH1(hEff[i],i+1);
  }

  /*
  TFile * fAcc;
  if ( isPrompt) fAcc = new TFile("AccAna_20131219_PromptJpsi_boosted_binCode_1.root");
  else fAcc = new TFile("AccAna_20131219_PromptJpsi_boosted_binCode_1.root");
  
  TH1D* hAcc  = (TH1D*)fAcc->Get(Form("h1D_Acc_pt_bin2_%s_20131219",direction.Data()));
  */
  
  TCanvas* c0 = new TCanvas("c0","",1000,500);
  c0->Divide(2,1);
  c0->cd(1);
  hEff[0]->SetXTitle("p_{T} (GeV)");
  hEff[0]->SetYTitle("Efficiency");
  hEff[0]->SetAxisRange(0,1.1,"Y");
  hEff[0]->Draw();
  for (int i=1; i<=4 ;i++) {
    hEff[i]->Draw("same");
  }
  jumSun(6.5,1,30,1);
  
  c0->cd(2);

  handsomeTH1(hAcc,1);
  hAcc->SetXTitle("p_{T} (GeV)");
  hAcc->SetYTitle("Acceptance");
 
  hAcc->SetAxisRange(0,1.1,"Y");
  hAcc->Draw();
  jumSun(6.5,1,30,1);
  

  TCanvas* c1 = new TCanvas("c1","",500,500);
  c1->cd();

  double ptBin[] = {6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 13.0, 16.0, 20.0, 30.0};
  const int nPtBin = sizeof(ptBin)/sizeof(double) - 1;
  TH1D* hptGen = new TH1D("hptGen",";J/psi p_{T} (GeV)",nPtBin,ptBin);
  TH1D* hptReco = (TH1D*)hptGen->Clone("hptReco");

  // Cuts
  TCut genRapCut, recoRapCut; 
  if (isFW) {
    genRapCut = "(Gen_QQ_4mom[0].Rapidity()>-2.4) && (Gen_QQ_4mom[0].Rapidity()<-.47)";
    recoRapCut = "(Reco_QQ_4mom[0].Rapidity()>-2.4) && (Reco_QQ_4mom[0].Rapidity()<-.47) &&  (Reco_QQ_mupl_4mom[0].Rapidity()>-2.4) && (Reco_QQ_mupl_4mom[0].Rapidity()<-.47) && (Reco_QQ_mumi_4mom[0].Rapidity()>-2.4) && (Reco_QQ_mumi_4mom[0].Rapidity()<-.47) && (Reco_QQ_trig&1)==1"; 
  }
  else {
    genRapCut = "Gen_QQ_4mom[0].Rapidity()>-.47 && Gen_QQ_4mom[0].Rapidity()<1.47";
    recoRapCut = "(Reco_QQ_4mom[0].Rapidity()>-.47) && (Reco_QQ_4mom[0].Rapidity()<1.47) && (Reco_QQ_mupl_4mom[0].Rapidity()>-.47 && Reco_QQ_mupl_4mom[0].Rapidity()<1.47) && (Reco_QQ_mumi_4mom[0].Rapidity()>-.47 && Reco_QQ_mumi_4mom[0].Rapidity()<1.47) && (Reco_QQ_trig&1)==1";
  }
  TCut recoMassCut = "Reco_QQ_4mom[0].M()>2.95 && Reco_QQ_4mom[0].M()<3.25";
  TCut recoAccCutPlus  = ""; //" Reco_QQ_mupl_4mom[0].Eta()>-2.4 && Reco_QQ_mupl_4mom[0].Eta()<1.47 &&   (  ( abs(Reco_QQ_mupl_4mom[0].Eta())>2.2 && abs(Reco_QQ_mupl_4mom[0].Eta())<2.4 && Reco_QQ_mupl_4mom[0].Pt()>0.8 )   || ( abs(Reco_QQ_mupl_4mom[0].Eta())>1.3 && abs(Reco_QQ_mupl_4mom[0].Eta())<=2.2 && Reco_QQ_mupl_4mom[0].P()>2.9 )  || ( abs(Reco_QQ_mupl_4mom[0].Eta())<=1.3 && Reco_QQ_mupl_4mom[0].Pt()>3.3 ) )";
  TCut recoAccCutMinus  = ""; //" Reco_QQ_mumi_4mom[0].Eta()>-2.4 && Reco_QQ_mumi_4mom[0].Eta()<1.47 &&   (  ( abs(Reco_QQ_mumi_4mom[0].Eta())>2.2 && abs(Reco_QQ_mumi_4mom[0].Eta())<2.4 && Reco_QQ_mumi_4mom[0].Pt()>0.8 )   || ( abs(Reco_QQ_mumi_4mom[0].Eta())>1.3 && abs(Reco_QQ_mumi_4mom[0].Eta())<=2.2 && Reco_QQ_mumi_4mom[0].P()>2.9 )  || ( abs(Reco_QQ_mumi_4mom[0].Eta())<=1.3 && Reco_QQ_mumi_4mom[0].Pt()>3.3 ) )";
  TCut recoAccCut = recoAccCutPlus && recoAccCutMinus;
  
  Long64_t nEntry = 100000000;
  mytree->Draw(Form("Gen_QQ_4mom[0].Pt()>>%s",hptGen->GetName()), genRapCut, "",nEntry);
  mytree->Draw(Form("Reco_QQ_4mom[0].Pt()>>%s",hptReco->GetName()), recoRapCut && recoMassCut && recoAccCut, "",nEntry);

  TH1D* hptRecoCorr = (TH1D*)hptReco->Clone("hptRecoCorr");
  hptRecoCorr->Divide(hEff[0]);
  hptRecoCorr->Divide(hAcc);
  

  handsomeTH1(hptGen,1);
  hptGen->SetAxisRange(1,1.5e5,"Y");
  
  hptGen->Draw("hist");
  handsomeTH1(hptReco,2);
  hptReco->SetMarkerStyle(24);
  hptReco->Draw("same");
  handsomeTH1(hptRecoCorr,2);
  hptRecoCorr->Draw("same");
  
  TLegend *l = new TLegend(0.4233871,0.5042373,0.8689516,0.7521186,NULL,"brNDC");
  easyLeg(l,"Yield");
  l->AddEntry(hptGen,"Generated J/#psi","l");
  l->AddEntry(hptReco,"Reco yield","p");
  l->AddEntry(hptRecoCorr,"Reco/(Acc*Eff)","p");
  l->Draw();
}
