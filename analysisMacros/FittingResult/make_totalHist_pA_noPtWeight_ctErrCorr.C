#include "../SONGKYO.h"

/////// main func. ///////
int make_totalHist_pA_noPtWeight_ctErrCorr(int MrapNpt=89, int accCutType = 2, bool useZvtxWeight=false, bool useSF = false){

  using namespace std;
  //// scaling efficiency (1st+2nd) based on lumi.
  //// Pbp=20.7(20.664), pPb=14.0(13.958), tot=34.6(34.622)
  double scalePbp = 20.664/34.622;
  double scalepPb = 13.958/34.622;
  
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return 0; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 0 or 1"<< endl; return 0; }
  const TString szFinal = Form("%s_%s",szBinning.Data(),szAccCut.Data());
  std::cout << "szFinal: " << szFinal << std::endl;

  //////////////////////////////////////////////////////////////////////////////////////
  //// Definition of the binning
  int nRap, nPt;
  if (MrapNpt==89) { nRap = 8; nPt = 9; }
  else if (MrapNpt==83) { nRap = 8; nPt = 3; }
  else if (MrapNpt==63) { nRap = 6; nPt = 3;}
  else { nRap = 6; nPt = 2;}

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// read in Acceptance file
  // *** without pt weight
  TFile *fAccPR = new TFile(Form("../Acceptance/AccAna_%s_pA_PR_%s.root",szBinning.Data(),szAccCut.Data()));
  TFile *fAccNP = new TFile(Form("../Acceptance/AccAna_%s_pA_NP_%s.root",szBinning.Data(),szAccCut.Data()));
  //////////////////////// Acc :: directly get Pbp as pA
  TH2D* h2D_Acc_PR_pA = (TH2D*)fAccPR->Get("h2D_Acc_pt_y_Pbp"); 
  TH2D* h2D_Acc_NP_pA = (TH2D*)fAccNP->Get("h2D_Acc_pt_y_Pbp");
  h2D_Acc_PR_pA->SetName("h2D_Acc_PR_pA");
  h2D_Acc_NP_pA->SetName("h2D_Acc_NP_pA");
  cout << "2 Acc TH2D : "<<h2D_Acc_PR_pA<<" "<<h2D_Acc_NP_pA<<endl;
  
  TH2D* h2D_Acc_Den_PR_pA = (TH2D*)fAccPR->Get("h2D_Den_pt_y_Pbp");
  TH2D* h2D_Acc_Den_NP_pA = (TH2D*)fAccNP->Get("h2D_Den_pt_y_Pbp");
  h2D_Acc_Den_PR_pA->SetName("h2D_Acc_Den_PR_pA");
  h2D_Acc_Den_NP_pA->SetName("h2D_Acc_Den_NP_pA");
  TH2D* h2D_Acc_Num_PR_pA = (TH2D*)fAccPR->Get("h2D_Num_pt_y_Pbp");
  TH2D* h2D_Acc_Num_NP_pA = (TH2D*)fAccNP->Get("h2D_Num_pt_y_Pbp");
  h2D_Acc_Num_PR_pA->SetName("h2D_Acc_Num_PR_pA");
  h2D_Acc_Num_NP_pA->SetName("h2D_Acc_Num_NP_pA");

  
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// read in Efficiency file
  // *** without pt weight
  TFile *fEffPRPbp = new TFile(Form("../Efficiency/EffAna_%s_Pbp_PR_%s_Zvtx%d_SF%d.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TFile *fEffPRpPb = new TFile(Form("../Efficiency/EffAna_%s_pPb_PR_%s_Zvtx%d_SF%d.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TFile *fEffNPPbp = new TFile(Form("../Efficiency/EffAna_%s_Pbp_NP_%s_Zvtx%d_SF%d.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TFile *fEffNPpPb = new TFile(Form("../Efficiency/EffAna_%s_pPb_NP_%s_Zvtx%d_SF%d.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TH2D* h2D_Eff_PR_Pbp = (TH2D*)fEffPRPbp->Get("h2D_Eff_pt_y");
  TH2D* h2D_Eff_PR_pPb = (TH2D*)fEffPRpPb->Get("h2D_Eff_pt_y");
  TH2D* h2D_Eff_NP_Pbp = (TH2D*)fEffNPPbp->Get("h2D_Eff_pt_y");
  TH2D* h2D_Eff_NP_pPb = (TH2D*)fEffNPpPb->Get("h2D_Eff_pt_y");
  h2D_Eff_PR_Pbp->SetName("h2D_Eff_PR_Pbp");
  h2D_Eff_PR_pPb->SetName("h2D_Eff_PR_pPb");
  h2D_Eff_NP_Pbp->SetName("h2D_Eff_NP_Pbp");
  h2D_Eff_NP_pPb->SetName("h2D_Eff_NP_pPb");
  cout << "4 Eff TH2D : "<<h2D_Eff_PR_Pbp<<" "<<h2D_Eff_PR_pPb<<" "<<h2D_Eff_NP_Pbp<<" "<<h2D_Eff_NP_pPb<<endl;
  
  TH2D* h2D_Eff_Den_PR_Pbp = (TH2D*)fEffPRPbp->Get("h2D_Den_pt_y");
  TH2D* h2D_Eff_Den_PR_pPb = (TH2D*)fEffPRpPb->Get("h2D_Den_pt_y");
  TH2D* h2D_Eff_Den_NP_Pbp = (TH2D*)fEffNPPbp->Get("h2D_Den_pt_y");
  TH2D* h2D_Eff_Den_NP_pPb = (TH2D*)fEffNPpPb->Get("h2D_Den_pt_y");
  h2D_Eff_Den_PR_Pbp->SetName("h2D_Eff_Den_PR_Pbp");
  h2D_Eff_Den_PR_pPb->SetName("h2D_Eff_Den_PR_pPb");
  h2D_Eff_Den_NP_Pbp->SetName("h2D_Eff_Den_NP_Pbp");
  h2D_Eff_Den_NP_pPb->SetName("h2D_Eff_Den_NP_pPb");
  TH2D* h2D_Eff_Num_PR_Pbp = (TH2D*)fEffPRPbp->Get("h2D_Num_pt_y");
  TH2D* h2D_Eff_Num_PR_pPb = (TH2D*)fEffPRpPb->Get("h2D_Num_pt_y");
  TH2D* h2D_Eff_Num_NP_Pbp = (TH2D*)fEffNPPbp->Get("h2D_Num_pt_y");
  TH2D* h2D_Eff_Num_NP_pPb = (TH2D*)fEffNPpPb->Get("h2D_Num_pt_y");
  h2D_Eff_Num_PR_Pbp->SetName("h2D_Eff_Num_PR_Pbp");
  h2D_Eff_Num_PR_pPb->SetName("h2D_Eff_Num_PR_pPb");
  h2D_Eff_Num_NP_Pbp->SetName("h2D_Eff_Num_NP_Pbp");
  h2D_Eff_Num_NP_pPb->SetName("h2D_Eff_Num_NP_pPb");

  //////////////////////// Eff :: Merge Pbp and pPb with proper scale
  int tmpbinPbp, tmpbinpPb;
  float tmpVal, tmpErr;
  //// 1) clone "Pbp" binning to "Pbp_tuned" and "pPb_tuned"
  TH2D* h2D_Eff_PR_Pbp_tuned = (TH2D*)h2D_Eff_PR_Pbp->Clone("h2D_Eff_PR_Pbp_tuned");  
  TH2D* h2D_Eff_PR_pPb_tuned = (TH2D*)h2D_Eff_PR_Pbp->Clone("h2D_Eff_PR_pPb_tuned");  
  //// 2) values from pPb to pPb_tuned
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpVal = h2D_Eff_PR_pPb->GetBinContent(iy+1,ipt+1);
      tmpErr = h2D_Eff_PR_pPb->GetBinError(iy+1,ipt+1);
      h2D_Eff_PR_pPb_tuned->SetBinContent(nRap-iy,ipt+1,tmpVal);
      h2D_Eff_PR_pPb_tuned->SetBinError(nRap-iy,ipt+1,tmpErr);
    }
  }
  //// 3) scale and merge
  h2D_Eff_PR_Pbp_tuned->Scale(scalePbp);
  h2D_Eff_PR_pPb_tuned->Scale(scalepPb);
  TH2D* h2D_Eff_PR_pA = (TH2D*)h2D_Eff_PR_Pbp_tuned->Clone("h2D_Eff_PR_pA");
  h2D_Eff_PR_pA->Add(h2D_Eff_PR_pPb_tuned);
  
  //// 1) clone "Pbp" binning to "Pbp_tuned" and "pPb_tuned"
  TH2D* h2D_Eff_Den_PR_Pbp_tuned = (TH2D*)h2D_Eff_Den_PR_Pbp->Clone("h2D_Eff_Den_PR_Pbp_tuned");  
  TH2D* h2D_Eff_Den_PR_pPb_tuned = (TH2D*)h2D_Eff_Den_PR_Pbp->Clone("h2D_Eff_Den_PR_pPb_tuned");  
  //// 2) values from pPb to pPb_tuned
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpVal = h2D_Eff_Den_PR_pPb->GetBinContent(iy+1,ipt+1);
      tmpErr = h2D_Eff_Den_PR_pPb->GetBinError(iy+1,ipt+1);
      h2D_Eff_Den_PR_pPb_tuned->SetBinContent(nRap-iy,ipt+1,tmpVal);
      h2D_Eff_Den_PR_pPb_tuned->SetBinError(nRap-iy,ipt+1,tmpErr);
    }
  }
  //// 3) scale and merge
  h2D_Eff_Den_PR_Pbp_tuned->Scale(scalePbp);
  h2D_Eff_Den_PR_pPb_tuned->Scale(scalepPb);
  TH2D* h2D_Eff_Den_PR_pA = (TH2D*)h2D_Eff_Den_PR_Pbp_tuned->Clone("h2D_Eff_Den_PR_pA");
  h2D_Eff_Den_PR_pA->Add(h2D_Eff_Den_PR_pPb_tuned);
  
  //// 1) clone "Pbp" binning to "Pbp_tuned" and "pPb_tuned"
  TH2D* h2D_Eff_Num_PR_Pbp_tuned = (TH2D*)h2D_Eff_Num_PR_Pbp->Clone("h2D_Eff_Num_PR_Pbp_tuned");  
  TH2D* h2D_Eff_Num_PR_pPb_tuned = (TH2D*)h2D_Eff_Num_PR_Pbp->Clone("h2D_Eff_Num_PR_pPb_tuned");  
  //// 2) values from pPb to pPb_tuned
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpVal = h2D_Eff_Num_PR_pPb->GetBinContent(iy+1,ipt+1);
      tmpErr = h2D_Eff_Num_PR_pPb->GetBinError(iy+1,ipt+1);
      h2D_Eff_Num_PR_pPb_tuned->SetBinContent(nRap-iy,ipt+1,tmpVal);
      h2D_Eff_Num_PR_pPb_tuned->SetBinError(nRap-iy,ipt+1,tmpErr);
    }
  }
  //// 3) scale and merge
  h2D_Eff_Num_PR_Pbp_tuned->Scale(scalePbp);
  h2D_Eff_Num_PR_pPb_tuned->Scale(scalepPb);
  TH2D* h2D_Eff_Num_PR_pA = (TH2D*)h2D_Eff_Num_PR_Pbp_tuned->Clone("h2D_Eff_Num_PR_pA");
  h2D_Eff_Num_PR_pA->Add(h2D_Eff_Num_PR_pPb_tuned);
  
  //// 1) clone "Pbp" binning to "Pbp_tuned" and "pPb_tuned"
  TH2D* h2D_Eff_NP_Pbp_tuned = (TH2D*)h2D_Eff_NP_Pbp->Clone("h2D_Eff_NP_Pbp_tuned");  
  TH2D* h2D_Eff_NP_pPb_tuned = (TH2D*)h2D_Eff_NP_Pbp->Clone("h2D_Eff_NP_pPb_tuned");  
  //// 2) values from pPb to pPb_tuned
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpVal = h2D_Eff_NP_pPb->GetBinContent(iy+1,ipt+1);
      tmpErr = h2D_Eff_NP_pPb->GetBinError(iy+1,ipt+1);
      h2D_Eff_NP_pPb_tuned->SetBinContent(nRap-iy,ipt+1,tmpVal);
      h2D_Eff_NP_pPb_tuned->SetBinError(nRap-iy,ipt+1,tmpErr);
    }
  }
  //// 3) scale and merge
  h2D_Eff_NP_Pbp_tuned->Scale(scalePbp);
  h2D_Eff_NP_pPb_tuned->Scale(scalepPb);
  TH2D* h2D_Eff_NP_pA = (TH2D*)h2D_Eff_NP_Pbp_tuned->Clone("h2D_Eff_NP_pA");
  h2D_Eff_NP_pA->Add(h2D_Eff_NP_pPb_tuned);
  
  //// 1) clone "Pbp" binning to "Pbp_tuned" and "pPb_tuned"
  TH2D* h2D_Eff_Den_NP_Pbp_tuned = (TH2D*)h2D_Eff_Den_NP_Pbp->Clone("h2D_Eff_Den_NP_Pbp_tuned");  
  TH2D* h2D_Eff_Den_NP_pPb_tuned = (TH2D*)h2D_Eff_Den_NP_Pbp->Clone("h2D_Eff_Den_NP_pPb_tuned");  
  //// 2) values from pPb to pPb_tuned
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpVal = h2D_Eff_Den_NP_pPb->GetBinContent(iy+1,ipt+1);
      tmpErr = h2D_Eff_Den_NP_pPb->GetBinError(iy+1,ipt+1);
      h2D_Eff_Den_NP_pPb_tuned->SetBinContent(nRap-iy,ipt+1,tmpVal);
      h2D_Eff_Den_NP_pPb_tuned->SetBinError(nRap-iy,ipt+1,tmpErr);
    }
  }
  //// 3) scale and merge
  h2D_Eff_Den_NP_Pbp_tuned->Scale(scalePbp);
  h2D_Eff_Den_NP_pPb_tuned->Scale(scalepPb);
  TH2D* h2D_Eff_Den_NP_pA = (TH2D*)h2D_Eff_Den_NP_Pbp_tuned->Clone("h2D_Eff_Den_NP_pA");
  h2D_Eff_Den_NP_pA->Add(h2D_Eff_Den_NP_pPb_tuned);
  
  //// 1) clone "Pbp" binning to "Pbp_tuned" and "pPb_tuned"
  TH2D* h2D_Eff_Num_NP_Pbp_tuned = (TH2D*)h2D_Eff_Num_NP_Pbp->Clone("h2D_Eff_Num_NP_Pbp_tuned");  
  TH2D* h2D_Eff_Num_NP_pPb_tuned = (TH2D*)h2D_Eff_Num_NP_Pbp->Clone("h2D_Eff_Num_NP_pPb_tuned");  
  //// 2) values from pPb to pPb_tuned
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpVal = h2D_Eff_Num_NP_pPb->GetBinContent(iy+1,ipt+1);
      tmpErr = h2D_Eff_Num_NP_pPb->GetBinError(iy+1,ipt+1);
      h2D_Eff_Num_NP_pPb_tuned->SetBinContent(nRap-iy,ipt+1,tmpVal);
      h2D_Eff_Num_NP_pPb_tuned->SetBinError(nRap-iy,ipt+1,tmpErr);
    }
  }
  //// 3) scale and merge
  h2D_Eff_Num_NP_Pbp_tuned->Scale(scalePbp);
  h2D_Eff_Num_NP_pPb_tuned->Scale(scalepPb);
  TH2D* h2D_Eff_Num_NP_pA = (TH2D*)h2D_Eff_Num_NP_Pbp_tuned->Clone("h2D_Eff_Num_NP_pA");
  h2D_Eff_Num_NP_pA->Add(h2D_Eff_Num_NP_pPb_tuned);
  
  
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// read in from fit file
  TFile* fFitpA = new TFile(Form("./fitResHist_%s_pA_%s.root",szBinning.Data(),szAccCut.Data()));
  //TH2D* h2D_Fit_PR_pA = (TH2D*)fFitpA->Get("h2D_nPrompt_Raw");  
  //TH2D* h2D_Fit_NP_pA = (TH2D*)fFitpA->Get("h2D_nNonPrompt_Raw"); 
  //TH2D* h2D_Fit_nSig_pA = (TH2D*)fFitpA->Get("h2D_nSig_Raw"); 
  //TH2D* h2D_Fit_nBkg_pA = (TH2D*)fFitpA->Get("h2D_nBkg_Raw"); 
  TH2D* h2D_Fit_PR_pA = (TH2D*)fFitpA->Get("h2D_nPrompt");  
  TH2D* h2D_Fit_NP_pA = (TH2D*)fFitpA->Get("h2D_nNonPrompt"); 
  TH2D* h2D_Fit_nSig_pA = (TH2D*)fFitpA->Get("h2D_nSig"); 
  TH2D* h2D_Fit_nBkg_pA = (TH2D*)fFitpA->Get("h2D_nBkg"); 
  TH2D* h2D_Fit_bFrac_pA = (TH2D*)fFitpA->Get("h2D_bFraction"); 
  TH2D* h2D_Fit_ctErrmin_pA = (TH2D*)fFitpA->Get("h2D_ctErrmin"); 
  TH2D* h2D_Fit_ctErrmax_pA = (TH2D*)fFitpA->Get("h2D_ctErrmax"); 
  TH2D* h2D_Fit_NoCutEntry_pA = (TH2D*)fFitpA->Get("h2D_NoCutEntry"); 
  TH2D* h2D_Fit_CutEntry_pA = (TH2D*)fFitpA->Get("h2D_CutEntry"); 
  TH2D* h2D_Fit_CutRatio_pA = (TH2D*)fFitpA->Get("h2D_CutRatio"); 
  h2D_Fit_PR_pA->SetName("h2D_Fit_PR_pA");
  h2D_Fit_NP_pA->SetName("h2D_Fit_NP_pA");
  h2D_Fit_nSig_pA->SetName("h2D_Fit_nSig_pA");
  h2D_Fit_nBkg_pA->SetName("h2D_Fit_nBkg_pA");
  h2D_Fit_bFrac_pA->SetName("h2D_Fit_bFrac_pA");
  h2D_Fit_ctErrmin_pA->SetName("h2D_Fit_ctErrmin_pA");
  h2D_Fit_ctErrmax_pA->SetName("h2D_Fit_ctErrmax_pA");
  h2D_Fit_NoCutEntry_pA->SetName("h2D_Fit_NoCutEntry_pA");
  h2D_Fit_CutEntry_pA->SetName("h2D_Fit_CutEntry_pA");
  h2D_Fit_CutRatio_pA->SetName("h2D_Fit_CutRatio_pA");
  
  cout << "2 yield TH2D : "<<h2D_Fit_PR_pA<<" "<<h2D_Fit_NP_pA<<endl;
  cout << "1 nSig TH2D : "<<h2D_Fit_nSig_pA<<" "<<endl;
  cout << "1 nBkg TH2D : "<<h2D_Fit_nBkg_pA<<" "<<endl;
  cout << "1 bFrac TH2D : "<<h2D_Fit_bFrac_pA<<" "<<endl;
  cout << "1 ctErrmin TH2D : "<<h2D_Fit_ctErrmin_pA<<" "<<endl;
  cout << "1 ctErrmax TH2D : "<<h2D_Fit_ctErrmax_pA<<" "<<endl;
  cout << "1 NoCutEntry TH2D : "<<h2D_Fit_NoCutEntry_pA<<" "<<endl;
  cout << "1 CutEntry TH2D : "<<h2D_Fit_CutEntry_pA<<" "<<endl;
  cout << "1 CutRatio TH2D : "<<h2D_Fit_CutRatio_pA<<" "<<endl;
  
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// [Data] calculate corrected yield = rawYield/(Acc*Eff)
  TH2D* h2D_CorrY_PR_pA = (TH2D*)h2D_Fit_PR_pA->Clone("h2D_CorrY_PR_pA");
  h2D_CorrY_PR_pA->SetName("h2D_CorrY_PR_pA");
  h2D_CorrY_PR_pA->Divide(h2D_Acc_PR_pA);
  h2D_CorrY_PR_pA->Divide(h2D_Eff_PR_pA);
  TH2D* h2D_CorrY_NP_pA = (TH2D*)h2D_Fit_NP_pA->Clone("h2D_CorrY_NP_pA");
  h2D_CorrY_NP_pA->SetName("h2D_CorrY_NP_pA");
  h2D_CorrY_NP_pA->Divide(h2D_Acc_NP_pA);
  h2D_CorrY_NP_pA->Divide(h2D_Eff_NP_pA);
  
  ////////////////////////////////////////////////
  ////// [MC] corrected yield RECO/(Acc*Eff)
  TH2D* h2D_MCCorrY_PR_pA = (TH2D*)h2D_Eff_Num_PR_pA->Clone("h2D_MCCorrY_PR_pA");
  h2D_MCCorrY_PR_pA->SetName("h2D_MCCorrY_PR_pA");
  h2D_MCCorrY_PR_pA->Divide(h2D_Acc_PR_pA);
  h2D_MCCorrY_PR_pA->Divide(h2D_Eff_PR_pA);
  TH2D* h2D_MCCorrY_NP_pA = (TH2D*)h2D_Eff_Num_NP_pA->Clone("h2D_MCCorrY_NP_pA");
  h2D_MCCorrY_NP_pA->SetName("h2D_MCCorrY_NP_pA");
  h2D_MCCorrY_NP_pA->Divide(h2D_Acc_NP_pA);
  h2D_MCCorrY_NP_pA->Divide(h2D_Eff_NP_pA);
  
  ////////////////////////////////////////////////
  ////// save as a root file
  TFile *outFile = new TFile(Form("totalHist_pA_%s_Zvtx%d_SF%d_noPtWeight_ctErrCorr.root",szFinal.Data(),(int)useZvtxWeight,(int)useSF),"RECREATE");
  std::cout << "szFinal: " << szFinal << std::endl;
  cout << "totalHist_pA_"<<szFinal<<"_Zvtx"<<(int)useZvtxWeight<<"_SF"<<(int)useSF<<"_noPtWeight_ctErrCorr.root has been created :) " <<endl; 

  outFile->cd();
  //Acc 
  h2D_Acc_PR_pA->Write();
  h2D_Acc_NP_pA->Write();
  //Eff
  h2D_Eff_PR_Pbp->Write();
  h2D_Eff_PR_pPb->Write();
  h2D_Eff_PR_Pbp_tuned->Write();
  h2D_Eff_PR_pPb_tuned->Write();
  h2D_Eff_PR_pA->Write();
  h2D_Eff_NP_Pbp->Write();
  h2D_Eff_NP_pPb->Write();
  h2D_Eff_NP_Pbp_tuned->Write();
  h2D_Eff_NP_pPb_tuned->Write();
  h2D_Eff_NP_pA->Write();
  //raw yield
  h2D_Fit_PR_pA->Write(); 
  h2D_Fit_NP_pA->Write(); 
  //CorrYield
  h2D_CorrY_PR_pA->Write(); 
  h2D_CorrY_NP_pA->Write(); 
  
  // DIRECTORY : num, den distributions for acc & eff
  TDirectoryFile *otherMCInfo = new TDirectoryFile("otherMCInfo","otherMCInfo");
  otherMCInfo->cd();
  // acc den & num
  h2D_Acc_Den_PR_pA->Write();
  h2D_Acc_Den_NP_pA->Write();
  h2D_Acc_Num_PR_pA->Write();
  h2D_Acc_Num_NP_pA->Write();
  // eff den & num
  h2D_Eff_Den_PR_Pbp->Write();
  h2D_Eff_Den_PR_pPb->Write();
  h2D_Eff_Den_PR_Pbp_tuned->Write();
  h2D_Eff_Den_PR_pPb_tuned->Write();
  h2D_Eff_Den_PR_pA->Write();
  h2D_Eff_Den_NP_Pbp->Write();
  h2D_Eff_Den_NP_pPb->Write();
  h2D_Eff_Den_NP_Pbp_tuned->Write();
  h2D_Eff_Den_NP_pPb_tuned->Write();
  h2D_Eff_Den_NP_pA->Write();
  h2D_Eff_Num_PR_Pbp->Write();
  h2D_Eff_Num_PR_pPb->Write();
  h2D_Eff_Num_PR_Pbp_tuned->Write();
  h2D_Eff_Num_PR_pPb_tuned->Write();
  h2D_Eff_Num_PR_pA->Write();
  h2D_Eff_Num_NP_Pbp->Write();
  h2D_Eff_Num_NP_pPb->Write();
  h2D_Eff_Num_NP_Pbp_tuned->Write();
  h2D_Eff_Num_NP_pPb_tuned->Write();
  h2D_Eff_Num_NP_pA->Write();
  // MC CorrYield
  h2D_MCCorrY_PR_pA->Write(); 
  h2D_MCCorrY_NP_pA->Write(); 
  otherMCInfo->Write();
  
  // DIRECTORY : other information for fittng
  outFile->cd();
  TDirectoryFile *otherFitInfo = new TDirectoryFile("otherFitInfo","otherFitInfo");
  otherFitInfo->cd();
  h2D_Fit_nSig_pA->Write(); 
  h2D_Fit_nBkg_pA->Write(); 
  h2D_Fit_bFrac_pA->Write();  
  h2D_Fit_ctErrmin_pA->Write(); 
  h2D_Fit_ctErrmax_pA->Write(); 
  h2D_Fit_NoCutEntry_pA->Write(); 
  h2D_Fit_CutEntry_pA->Write(); 
  h2D_Fit_CutRatio_pA->Write(); 
  otherFitInfo->Write();
  
  outFile->Close();
  
  return 0;

} // end of main func

