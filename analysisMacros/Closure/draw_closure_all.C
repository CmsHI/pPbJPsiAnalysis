#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

int draw_closure_all(int isPA=0, bool useZvtxWeight=false, bool useSF = false, bool isLog = false)
{
  gROOT->Macro("../Style.C");
  using namespace std;
  
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0, or 1"<< endl; return 0; }

  ////rap array in yCM (from forward to backward)
  const Int_t nRap = 8; 
  const Int_t nPt = 9; 
  const int nRapTmp = nRap +1; 
  const int nPtTmp = nPt +1; 
  
  Double_t rapArrNumFB[nRapTmp];
  Double_t rapArrNumFB_pp[nRapTmp] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};// for pt dist.
  Double_t rapArrNumFB_pA[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  //Double_t rapArrNumBF[nRapTmp];
  //Double_t rapArrNumBF_pp[nRapTmp] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};// for rap dist.
  //Double_t rapArrNumBF_pA[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
  for (int iy=0; iy<nRapTmp; iy++){
    if (isPA==0) { rapArrNumFB[iy]=rapArrNumFB_pp[iy];}
    else { rapArrNumFB[iy]=rapArrNumFB_pA[iy];}
  } 
  
  ////pt array
  Double_t ptArrNum[10] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
  
  //// array string
  string rapArr[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
    cout << iy <<"th rapArr = " << rapArr[iy] << endl;
  }
  string ptArr[nPt];
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
    cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
  }
 
  // --- read-in file
  TFile * f2D = new TFile(Form("../FittingResult/totalHist_%s_8rap9pt_newcut_nominal_Zvtx%d_SF%d_noPtWeight.root",szPA.Data(),(int)useZvtxWeight,(int)useSF),"READ");
  // --- read-in 2D hist for "MC corrected yield" and "gen distributions"
  TH2D* h2D_MCCorrY_PR = (TH2D*)f2D->Get(Form("otherMCInfo/h2D_MCCorrY_PR_%s",szPA.Data()));
  TH2D* h2D_MCCorrY_NP = (TH2D*)f2D->Get(Form("otherMCInfo/h2D_MCCorrY_NP_%s",szPA.Data()));
  TH2D* h2D_Acc_Den_PR = (TH2D*)f2D->Get(Form("otherMCInfo/h2D_Acc_Den_PR_%s",szPA.Data()));
  TH2D* h2D_Acc_Den_NP = (TH2D*)f2D->Get(Form("otherMCInfo/h2D_Acc_Den_NP_%s",szPA.Data()));
  
  const int nbinsX = h2D_MCCorrY_PR->GetNbinsX();
  const int nbinsY = h2D_MCCorrY_PR->GetNbinsY();
	cout << "nbinsX = " << nbinsX << ", nbinsY = " << nbinsY << endl;
  if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return 0; };
  if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return 0; };
  
  // ---  projection to 1D hist
  TH1D* h1D_MCCorrY_PR[nbinsX]; 
  TH1D* h1D_MCCorrY_NP[nbinsX]; 
  TH1D* h1D_Acc_Den_PR[nbinsX]; 
  TH1D* h1D_Acc_Den_NP[nbinsX]; 

  // --- projection to rapidity : x-axis
  TH1D* h1D_MCCorrY_PR_rap[nbinsY]; 
  TH1D* h1D_MCCorrY_NP_rap[nbinsY]; 
  TH1D* h1D_Acc_Den_PR_rap[nbinsY];
  TH1D* h1D_Acc_Den_NP_rap[nbinsY];

  // iy=0 refers to forwards !!! (ordering here)
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    if (isPA==0){
      h1D_MCCorrY_PR[iy] = h2D_MCCorrY_PR->ProjectionY(Form("h1D_MCCorrY_PR_%d",iy),nbinsX-iy,nbinsX-iy);
      h1D_MCCorrY_NP[iy] = h2D_MCCorrY_NP->ProjectionY(Form("h1D_MCCorrY_NP_%d",iy),nbinsX-iy,nbinsX-iy);
      h1D_Acc_Den_PR[iy] = h2D_Acc_Den_PR->ProjectionY(Form("h1D_Acc_Den_PR_%d",iy),nbinsX-iy,nbinsX-iy);
      h1D_Acc_Den_NP[iy] = h2D_Acc_Den_NP->ProjectionY(Form("h1D_Acc_Den_NP_%d",iy),nbinsX-iy,nbinsX-iy);
    } else {
      h1D_MCCorrY_PR[iy] = h2D_MCCorrY_PR->ProjectionY(Form("h1D_MCCorrY_PR_%d",iy),iy+1,iy+1);
      h1D_MCCorrY_NP[iy] = h2D_MCCorrY_NP->ProjectionY(Form("h1D_MCCorrY_NP_%d",iy),iy+1,iy+1);
      h1D_Acc_Den_PR[iy] = h2D_Acc_Den_PR->ProjectionY(Form("h1D_Acc_Den_PR_%d",iy),iy+1,iy+1);
      h1D_Acc_Den_NP[iy] = h2D_Acc_Den_NP->ProjectionY(Form("h1D_Acc_Den_NP_%d",iy),iy+1,iy+1);
    }
  }

  for (Int_t iy = 0; iy < nbinsY; iy++) {
    if (isPA==0){
      h1D_MCCorrY_PR_rap[iy] = h2D_MCCorrY_PR->ProjectionX(Form("h1D_MCCorrY_PR_rap_%d",iy),iy+1,iy+1);
      h1D_MCCorrY_NP_rap[iy] = h2D_MCCorrY_NP->ProjectionX(Form("h1D_MCCorrY_NP_rap_%d",iy),iy+1,iy+1);
      h1D_Acc_Den_PR_rap[iy] = h2D_Acc_Den_PR->ProjectionX(Form("h1D_Acc_Den_PR_rap_%d",iy),iy+1,iy+1);
      h1D_Acc_Den_NP_rap[iy] = h2D_Acc_Den_NP->ProjectionX(Form("h1D_Acc_Den_NP_rap_%d",iy),iy+1,iy+1);
    } else {
      h1D_MCCorrY_PR_rap[iy] = h2D_MCCorrY_PR->ProjectionX(Form("h1D_MCCorrY_PR_rap_%d",iy),iy+1,iy+1);
      h1D_MCCorrY_NP_rap[iy] = h2D_MCCorrY_NP->ProjectionX(Form("h1D_MCCorrY_NP_rap_%d",iy),iy+1,iy+1);
      h1D_Acc_Den_PR_rap[iy] = h2D_Acc_Den_PR->ProjectionX(Form("h1D_Acc_Den_PR_rap_%d",iy),iy+1,iy+1);
      h1D_Acc_Den_NP_rap[iy] = h2D_Acc_Den_NP->ProjectionX(Form("h1D_Acc_Den_NP_rap_%d",iy),iy+1,iy+1);
    }
  }

  
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  
  //// set values as zero for unused bins
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    if (iy>=1 && iy<=6) {
      h1D_MCCorrY_PR[iy]->SetBinContent(1,0);
      h1D_MCCorrY_NP[iy]->SetBinContent(1,0);
      h1D_MCCorrY_PR[iy]->SetBinError(1,0);
      h1D_MCCorrY_NP[iy]->SetBinError(1,0);
      h1D_MCCorrY_PR[iy]->SetBinContent(2,0);
      h1D_MCCorrY_NP[iy]->SetBinContent(2,0);
      h1D_MCCorrY_PR[iy]->SetBinError(2,0);
      h1D_MCCorrY_NP[iy]->SetBinError(2,0);
      h1D_Acc_Den_PR[iy]->SetBinContent(1,0);
      h1D_Acc_Den_NP[iy]->SetBinContent(1,0);
      h1D_Acc_Den_PR[iy]->SetBinError(1,0);
      h1D_Acc_Den_NP[iy]->SetBinError(1,0);
      h1D_Acc_Den_PR[iy]->SetBinContent(2,0);
      h1D_Acc_Den_NP[iy]->SetBinContent(2,0);
      h1D_Acc_Den_PR[iy]->SetBinError(2,0);
      h1D_Acc_Den_NP[iy]->SetBinError(2,0);
    }
    if (iy>=2 && iy<=5) {
      h1D_MCCorrY_PR[iy]->SetBinContent(3,0);
      h1D_MCCorrY_NP[iy]->SetBinContent(3,0);
      h1D_MCCorrY_PR[iy]->SetBinError(3,0);
      h1D_MCCorrY_NP[iy]->SetBinError(3,0);
      h1D_Acc_Den_PR[iy]->SetBinContent(3,0);
      h1D_Acc_Den_NP[iy]->SetBinContent(3,0);
      h1D_Acc_Den_PR[iy]->SetBinError(3,0);
      h1D_Acc_Den_NP[iy]->SetBinError(3,0);
    }
    if (isPA ==0){ //for_pp 
      if (iy>=2 && iy<=5) { 
        h1D_MCCorrY_PR[iy]->SetBinContent(4,0);
        h1D_MCCorrY_NP[iy]->SetBinContent(4,0);
        h1D_MCCorrY_PR[iy]->SetBinError(4,0);
        h1D_MCCorrY_NP[iy]->SetBinError(4,0);
        h1D_Acc_Den_PR[iy]->SetBinContent(4,0);
        h1D_Acc_Den_NP[iy]->SetBinContent(4,0);
        h1D_Acc_Den_PR[iy]->SetBinError(4,0);
        h1D_Acc_Den_NP[iy]->SetBinError(4,0);
      }
    }
    else {
      if (iy>=2 && iy<=4) { // for_pA
        h1D_MCCorrY_PR[iy]->SetBinContent(4,0);
        h1D_MCCorrY_NP[iy]->SetBinContent(4,0);
        h1D_MCCorrY_PR[iy]->SetBinError(4,0);
        h1D_MCCorrY_NP[iy]->SetBinError(4,0);
        h1D_Acc_Den_PR[iy]->SetBinContent(4,0);
        h1D_Acc_Den_NP[iy]->SetBinContent(4,0);
        h1D_Acc_Den_PR[iy]->SetBinError(4,0);
        h1D_Acc_Den_NP[iy]->SetBinError(4,0);
      }
    }
  }
  
  //// set values as zero for unused bins
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    if (iy-1>=1 && iy-1<=6) {
      h1D_MCCorrY_PR_rap[0]->SetBinContent(iy,0);
      h1D_MCCorrY_NP_rap[0]->SetBinContent(iy,0);
      h1D_MCCorrY_PR_rap[1]->SetBinContent(iy,0);
      h1D_MCCorrY_NP_rap[1]->SetBinContent(iy,0);
      h1D_MCCorrY_PR_rap[0]->SetBinError(iy,0);
      h1D_MCCorrY_NP_rap[0]->SetBinError(iy,0);
      h1D_MCCorrY_PR_rap[1]->SetBinError(iy,0);
      h1D_MCCorrY_NP_rap[1]->SetBinError(iy,0);
      h1D_Acc_Den_PR_rap[0]->SetBinContent(iy,0);
      h1D_Acc_Den_NP_rap[0]->SetBinContent(iy,0);
      h1D_Acc_Den_PR_rap[1]->SetBinContent(iy,0);
      h1D_Acc_Den_NP_rap[1]->SetBinContent(iy,0);
      h1D_Acc_Den_PR_rap[0]->SetBinError(iy,0);
      h1D_Acc_Den_NP_rap[0]->SetBinError(iy,0);
      h1D_Acc_Den_PR_rap[1]->SetBinError(iy,0);
      h1D_Acc_Den_NP_rap[1]->SetBinError(iy,0);
    }
    if (iy-1>=2 && iy-1<=5) {
      h1D_MCCorrY_PR_rap[2]->SetBinContent(iy,0);
      h1D_MCCorrY_NP_rap[2]->SetBinContent(iy,0);
      h1D_MCCorrY_PR_rap[2]->SetBinError(iy,0);
      h1D_MCCorrY_NP_rap[2]->SetBinError(iy,0);
      h1D_Acc_Den_PR_rap[2]->SetBinContent(iy,0);
      h1D_Acc_Den_NP_rap[2]->SetBinContent(iy,0);
      h1D_Acc_Den_PR_rap[2]->SetBinError(iy,0);
      h1D_Acc_Den_NP_rap[2]->SetBinError(iy,0);
    }
    if (isPA ==0){ //for_pp 
      if (iy-1>=2 && iy-1<=5) { 
        h1D_MCCorrY_PR_rap[3]->SetBinContent(iy,0);
        h1D_MCCorrY_NP_rap[3]->SetBinContent(iy,0);
        h1D_MCCorrY_PR_rap[3]->SetBinError(iy,0);
        h1D_MCCorrY_NP_rap[3]->SetBinError(iy,0);
        h1D_Acc_Den_PR_rap[3]->SetBinContent(iy,0);
        h1D_Acc_Den_NP_rap[3]->SetBinContent(iy,0);
        h1D_Acc_Den_PR_rap[3]->SetBinError(iy,0);
        h1D_Acc_Den_NP_rap[3]->SetBinError(iy,0);
      }
    }
    else {
      if (iy-1>=2 && iy-1<=4) { // for_pA
        h1D_MCCorrY_PR_rap[3]->SetBinContent(iy,0);
        h1D_MCCorrY_NP_rap[3]->SetBinContent(iy,0);
        h1D_MCCorrY_PR_rap[3]->SetBinError(iy,0);
        h1D_MCCorrY_NP_rap[3]->SetBinError(iy,0);
        h1D_Acc_Den_PR_rap[3]->SetBinContent(iy,0);
        h1D_Acc_Den_NP_rap[3]->SetBinContent(iy,0);
        h1D_Acc_Den_PR_rap[3]->SetBinError(iy,0);
        h1D_Acc_Den_NP_rap[3]->SetBinError(iy,0);
      }
    }
  }

  TH1D* hPRRatio_All;
  TH1D* hNPRatio_All;
  TH1D* hPRRatio_All_rap;
  TH1D* hNPRatio_All_rap;
  
  TH1D* h1D_MCCorrY_PR_All = (TH1D*) h1D_MCCorrY_PR[0]->Clone("h1D_MCCorrY_PR_All");
  TH1D* h1D_MCCorrY_NP_All = (TH1D*) h1D_MCCorrY_NP[0]->Clone("h1D_MCCorrY_NP_All");
  TH1D* h1D_Acc_Den_PR_All = (TH1D*)h1D_Acc_Den_PR[0]->Clone("h1D_Acc_Den_PR_All");
  TH1D* h1D_Acc_Den_NP_All = (TH1D*)h1D_Acc_Den_NP[0]->Clone("h1D_Acc_Den_NP_All");
  
  TH1D* h1D_MCCorrY_PR_All_rap = (TH1D*) h1D_MCCorrY_PR_rap[0]->Clone("h1D_MCCorrY_PR_All_rap");
  TH1D* h1D_MCCorrY_NP_All_rap = (TH1D*) h1D_MCCorrY_NP_rap[0]->Clone("h1D_MCCorrY_NP_All_rap");
  TH1D* h1D_Acc_Den_PR_All_rap = (TH1D*)h1D_Acc_Den_PR_rap[0]->Clone("h1D_Acc_Den_PR_All_rap");
  TH1D* h1D_Acc_Den_NP_All_rap = (TH1D*)h1D_Acc_Den_NP_rap[0]->Clone("h1D_Acc_Den_NP_All_rap");

  h1D_MCCorrY_PR_All->Reset();
  h1D_MCCorrY_NP_All->Reset();
  h1D_Acc_Den_PR_All->Reset();
  h1D_Acc_Den_NP_All->Reset();
  
  h1D_MCCorrY_PR_All_rap->Reset();
  h1D_MCCorrY_NP_All_rap->Reset();
  h1D_Acc_Den_PR_All_rap->Reset();
  h1D_Acc_Den_NP_All_rap->Reset();

  TList *listMCCorrPR = new TList;
  TList *listMCCorrNP = new TList;
  TList *listAccDenPR = new TList;
  TList *listAccDenNP = new TList;
  
  TList *listMCCorrPR_rap = new TList;
  TList *listMCCorrNP_rap = new TList;
  TList *listAccDenPR_rap = new TList;
  TList *listAccDenNP_rap = new TList;
  


  for(int i=0;i<nbinsX;i++)
  {
    listMCCorrPR -> Add(h1D_MCCorrY_PR[i]);
    listMCCorrNP -> Add(h1D_MCCorrY_NP[i]);
    listAccDenPR -> Add(h1D_Acc_Den_PR[i]);
    listAccDenNP -> Add(h1D_Acc_Den_NP[i]);
  }
  for(int i=0;i<nbinsY;i++)
  {
    listMCCorrPR_rap -> Add(h1D_MCCorrY_PR_rap[i]);
    listMCCorrNP_rap -> Add(h1D_MCCorrY_NP_rap[i]);
    listAccDenPR_rap -> Add(h1D_Acc_Den_PR_rap[i]);
    listAccDenNP_rap -> Add(h1D_Acc_Den_NP_rap[i]);
  }

  h1D_MCCorrY_PR_All->Merge(listMCCorrPR);
  h1D_MCCorrY_NP_All->Merge(listMCCorrNP);
  h1D_Acc_Den_PR_All->Merge(listAccDenPR);
  h1D_Acc_Den_NP_All->Merge(listAccDenNP);
  
  h1D_MCCorrY_PR_All_rap->Merge(listMCCorrPR_rap);
  h1D_MCCorrY_NP_All_rap->Merge(listMCCorrNP_rap);
  h1D_Acc_Den_PR_All_rap->Merge(listAccDenPR_rap);
  h1D_Acc_Den_NP_All_rap->Merge(listAccDenNP_rap);

  double err1 = 0;
  for(int i=0;i<nbinsX;i++)
  {
    err1 += TMath::Power(h1D_MCCorrY_PR[i]->GetBinError(1),2);
  }
  err1 = TMath::Sqrt(err1);

  h1D_MCCorrY_PR_All->Scale(1./h1D_MCCorrY_PR_All->Integral());
  h1D_MCCorrY_NP_All->Scale(1./h1D_MCCorrY_NP_All->Integral());
  h1D_MCCorrY_PR_All->Scale(1,"width");
  h1D_MCCorrY_NP_All->Scale(1,"width");
  
  h1D_Acc_Den_PR_All->Scale(1./h1D_Acc_Den_PR_All->Integral());
  h1D_Acc_Den_NP_All->Scale(1./h1D_Acc_Den_NP_All->Integral());
  h1D_Acc_Den_PR_All->Scale(1,"width");
  h1D_Acc_Den_NP_All->Scale(1,"width");
  
  h1D_MCCorrY_PR_All_rap->Scale(1./h1D_MCCorrY_PR_All_rap->Integral());
  h1D_MCCorrY_NP_All_rap->Scale(1./h1D_MCCorrY_NP_All_rap->Integral());
  h1D_MCCorrY_PR_All_rap->Scale(1,"width");
  h1D_MCCorrY_NP_All_rap->Scale(1,"width");
  
  h1D_Acc_Den_PR_All_rap->Scale(1./h1D_Acc_Den_PR_All_rap->Integral());
  h1D_Acc_Den_NP_All_rap->Scale(1./h1D_Acc_Den_NP_All_rap->Integral());
  h1D_Acc_Den_PR_All_rap->Scale(1,"width");
  h1D_Acc_Den_NP_All_rap->Scale(1,"width");
  
  TCanvas *ccc = new TCanvas("ccc","",1600,800);
  ccc->Divide(1,2);
  ccc->cd(1);
  h1D_MCCorrY_PR_All_rap->Draw();
  ccc->cd(2);
  h1D_Acc_Den_PR_All_rap->Draw();

  hPRRatio_All = (TH1D*)h1D_MCCorrY_PR_All->Clone("PRRatio_All");
  hNPRatio_All = (TH1D*)h1D_MCCorrY_NP_All->Clone("NPRatio_All");
  
  hPRRatio_All->Divide(h1D_Acc_Den_PR_All);
  hNPRatio_All->Divide(h1D_Acc_Den_NP_All);
  
  hPRRatio_All_rap = (TH1D*)h1D_MCCorrY_PR_All_rap->Clone("PRRatio_All_rap");
  hNPRatio_All_rap = (TH1D*)h1D_MCCorrY_NP_All_rap->Clone("NPRatio_All_rap");
  
  hPRRatio_All_rap->Divide(h1D_Acc_Den_PR_All_rap);
  hNPRatio_All_rap->Divide(h1D_Acc_Den_NP_All_rap);
  
  ////// after zero-bin setting, normalize!
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    h1D_MCCorrY_PR[iy]->Scale(1./h1D_MCCorrY_PR[iy]->Integral());
    h1D_MCCorrY_NP[iy]->Scale(1./h1D_MCCorrY_NP[iy]->Integral());
    h1D_Acc_Den_PR[iy]->Scale(1./h1D_Acc_Den_PR[iy]->Integral());
    h1D_Acc_Den_NP[iy]->Scale(1./h1D_Acc_Den_NP[iy]->Integral());
    h1D_MCCorrY_PR[iy]->Scale(1,"width");
    h1D_MCCorrY_NP[iy]->Scale(1,"width");
    h1D_Acc_Den_PR[iy]->Scale(1,"width");
    h1D_Acc_Den_NP[iy]->Scale(1,"width");
  }
  
  // hRatio
  TH1D* hRatio_PR[nRap];
  TH1D* hRatio_NP[nRap];
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    hRatio_PR[iy]=(TH1D*)h1D_MCCorrY_PR[iy]->Clone(Form("hRatio_PR_%d",iy));
    hRatio_PR[iy]->Divide(h1D_Acc_Den_PR[iy]);
    hRatio_NP[iy]=(TH1D*)h1D_MCCorrY_NP[iy]->Clone(Form("hRatio_NP_%d",iy));
    hRatio_NP[iy]->Divide(h1D_Acc_Den_NP[iy]);
  }
  

  //////////////////////////////////////////////////////////////////
  //// --- Draw histograms
  
  TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92);
  SetLegendStyle(legUR);
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  ///// prompt
  TCanvas* c_PR_01 = new TCanvas("c_PR_01","c_PR_01",1600,800);
  c_PR_01->Divide(4,2);
  
  TGraphAsymmErrors* g_MCCorrY_PR[nbinsX];
  TGraphAsymmErrors* g_Acc_Den_PR[nbinsX];
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_PR_01->cd(iy+1);
    if (isLog) gPad->SetLogy(1);
    else gPad->SetLogy(0);
    g_MCCorrY_PR[iy]=new TGraphAsymmErrors(h1D_MCCorrY_PR[iy]);
    g_Acc_Den_PR[iy]=new TGraphAsymmErrors(h1D_Acc_Den_PR[iy]);
    SetGraphStyle(g_MCCorrY_PR[iy],1,0);
    SetGraphStyle(g_Acc_Den_PR[iy],2,10);
    g_MCCorrY_PR[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    g_MCCorrY_PR[iy]->GetXaxis()->CenterTitle();
    g_MCCorrY_PR[iy]->GetXaxis()->SetLimits(0.,30.);
    g_MCCorrY_PR[iy]->GetYaxis()->SetTitle("");
    g_MCCorrY_PR[iy]->Draw("ap");
    g_Acc_Den_PR[iy]->Draw("p");
    if (iy==0) {
      legUR -> SetHeader(Form("%s Prompt J/#psi",szPA.Data()));
      legUR -> AddEntry(h1D_MCCorrY_PR[iy],"RECO/(Acc*Eff)","lp");
      legUR -> AddEntry(h1D_Acc_Den_PR[iy],"GEN","lp");
      legUR->Draw();
    }
    latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
  }
  c_PR_01->SaveAs(Form("dir_closure/%s_PR_differential_isLog%d.pdf",szPA.Data(),(int)isLog));
  
  ///// non-prompt
  TCanvas* c_NP_01 = new TCanvas("c_NP_01","c_NP_01",1600,800);
  c_NP_01->Divide(4,2);
  
  TGraphAsymmErrors* g_MCCorrY_NP[nbinsX];
  TGraphAsymmErrors* g_Acc_Den_NP[nbinsX];
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_NP_01->cd(iy+1);
    if (isLog) gPad->SetLogy(1);
    else gPad->SetLogy(0);
    g_MCCorrY_NP[iy]=new TGraphAsymmErrors(h1D_MCCorrY_NP[iy]);
    g_Acc_Den_NP[iy]=new TGraphAsymmErrors(h1D_Acc_Den_NP[iy]);
    SetGraphStyle(g_MCCorrY_NP[iy],1,0);
    SetGraphStyle(g_Acc_Den_NP[iy],2,10);
    g_MCCorrY_NP[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    g_MCCorrY_NP[iy]->GetXaxis()->CenterTitle();
    g_MCCorrY_NP[iy]->GetXaxis()->SetLimits(0.,30.);
    g_MCCorrY_NP[iy]->GetYaxis()->SetTitle("");
    g_MCCorrY_NP[iy]->Draw("ap");
    g_Acc_Den_NP[iy]->Draw("p");
    if (iy==0) {
      legUR -> SetHeader(Form("%s Non-prompt J/#psi",szPA.Data()));
      legUR->Draw();
    }
    latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
  }
  c_NP_01->SaveAs(Form("dir_closure/%s_NP_differential_isLog%d.pdf",szPA.Data(),(int)isLog));
  
  //////////////////////////////////////////////////////////////////
  //// ratio
  //////////////////////////////////////////////////////////////////
  
  ///// prompt
  TCanvas* c_PR_02 = new TCanvas("c_PR_02","c_PR_02",1600,800);
  c_PR_02->Divide(4,2);
  
  TGraphAsymmErrors* gRatio_PR[nbinsX];
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_PR_02->cd(iy+1);
    gPad->SetLogy(0);
    gRatio_PR[iy]=new TGraphAsymmErrors(hRatio_PR[iy]);
    SetGraphStyle(gRatio_PR[iy],1,0);
    gRatio_PR[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    gRatio_PR[iy]->GetYaxis()->SetTitle("corrected RECO / GEN");
    gRatio_PR[iy]->GetXaxis()->SetLimits(0.,30.);
    gRatio_PR[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
    gRatio_PR[iy]->Draw("ap");
    latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
    dashedLine(0.,1.,30.,1.,1,1);
  }
  c_PR_02->SaveAs(Form("dir_closure/%s_PR_differential_ratio.pdf",szPA.Data()));
  
  // --- non-prompt pp
  TCanvas* c_NP_02 = new TCanvas("c_NP_02","c_NP_02",1600,800);
  c_NP_02->Divide(4,2);
  
  TGraphAsymmErrors* gRatio_NP[nbinsX];
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_NP_02->cd(iy+1);
    gPad->SetLogy(0);
    gRatio_NP[iy]=new TGraphAsymmErrors(hRatio_NP[iy]);
    SetGraphStyle(gRatio_NP[iy],1,0);
    gRatio_NP[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    gRatio_NP[iy]->GetYaxis()->SetTitle("corrected RECO / GEN");
    gRatio_NP[iy]->GetXaxis()->SetLimits(0.,30.);
    gRatio_NP[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
    gRatio_NP[iy]->Draw("ap");
    latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
    dashedLine(0.,1.,30.,1.,1,1);
  }
  c_NP_02->SaveAs(Form("dir_closure/%s_NP_differential_ratio.pdf",szPA.Data()));
  
  TCanvas* c_ratio_all = new TCanvas("c_ratio_all","c_ratio_all",1600,800);
  c_ratio_all -> Divide(1,2);
  TGraphAsymmErrors* g_ratio_PR_All;
  TGraphAsymmErrors* g_ratio_NP_All;
  g_ratio_PR_All = new TGraphAsymmErrors(hPRRatio_All);
  g_ratio_NP_All = new TGraphAsymmErrors(hNPRatio_All);

  c_ratio_all->cd(1);
  gPad->SetLogy(0);
  SetGraphStyle(g_ratio_PR_All,1,0);

  g_ratio_PR_All->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  g_ratio_PR_All->GetYaxis()->SetTitle("corrected RECO / GEN");
  g_ratio_PR_All->GetXaxis()->SetLimits(0.,30.);
  g_ratio_PR_All->GetYaxis()->SetRangeUser(0.9,1.1);
  g_ratio_PR_All->Draw("ap");
  latex->DrawLatex(0.56,0.88," Prompt J/#psi ");
  dashedLine(0., 1., 30., 1., 1,1);
  
  c_ratio_all->cd(2);
  gPad->SetLogy(0);
  SetGraphStyle(g_ratio_NP_All,1,0);

  g_ratio_NP_All->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  g_ratio_NP_All->GetYaxis()->SetTitle("corrected RECO / GEN");
  g_ratio_NP_All->GetXaxis()->SetLimits(0.,30.);
  g_ratio_NP_All->GetYaxis()->SetRangeUser(0.9,1.1);
  g_ratio_NP_All->Draw("ap");
  latex->DrawLatex(0.56,0.88," Non-prompt J/#psi ");
  dashedLine(0., 1., 30., 1., 1,1);

  TString wString;
  if(isPA==0) wString = "pp";
  else if(isPA==1) wString = "pA";
  c_ratio_all->SaveAs(Form("dir_closure/%s_RECOGen_Ratio_all_pT.pdf",wString.Data()));

  TCanvas* c_ratio_all_rap = new TCanvas("c_ratio_all_rap","c_ratio_all_rap",1600,800);
  c_ratio_all_rap -> Divide(1,2);
  TGraphAsymmErrors* g_ratio_PR_All_rap;
  TGraphAsymmErrors* g_ratio_NP_All_rap;
  g_ratio_PR_All_rap = new TGraphAsymmErrors(hPRRatio_All_rap);
  g_ratio_NP_All_rap = new TGraphAsymmErrors(hNPRatio_All_rap);

  c_ratio_all_rap->cd(1);
  gPad->SetLogy(0);
  SetGraphStyle(g_ratio_PR_All_rap,1,0);

  g_ratio_PR_All_rap->GetXaxis()->SetTitle("y");
  g_ratio_PR_All_rap->GetYaxis()->SetTitle("corrected RECO / GEN");
  g_ratio_PR_All_rap->GetXaxis()->SetLimits(-2.5,2.5);
  g_ratio_PR_All_rap->GetYaxis()->SetRangeUser(0.9,1.1);
  g_ratio_PR_All_rap->Draw("ap");
  latex->DrawLatex(0.56,0.88," Prompt J/#psi ");
  dashedLine(-2.5, 1., 2.5, 1., 1,1);
  
  c_ratio_all_rap->cd(2);
  gPad->SetLogy(0);
  SetGraphStyle(g_ratio_NP_All_rap,1,0);

  g_ratio_NP_All_rap->GetXaxis()->SetTitle("y");
  g_ratio_NP_All_rap->GetYaxis()->SetTitle("corrected RECO / GEN");
  g_ratio_NP_All_rap->GetXaxis()->SetLimits(-2.5,2.5);
  g_ratio_NP_All_rap->GetYaxis()->SetRangeUser(0.9,1.1);
  g_ratio_NP_All_rap->Draw("ap");
  latex->DrawLatex(0.56,0.88," Non-prompt J/#psi ");
  dashedLine(-2.5, 1., 2.5, 1., 1,1);

  TString wString_rap;
  if(isPA==0) wString_rap = "pp";
  else if(isPA==1) wString_rap = "pA";
  c_ratio_all_rap->SaveAs(Form("dir_closure/%s_RECOGen_Ratio_all_rap.pdf",wString_rap.Data()));

  return 0;

} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, string* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.2f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < y_{CM} < %.2f", binmin, binmax);
  } else {
    *arr = Form("%.2f < y_{CM} < %.2f", binmin, binmax);
  }
}

void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.2f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.2f", binmin, binmax);
  } else {
    *arr = Form("%.2f < |y_{CM}| < %.2f", binmin, binmax);
  }
}

void formPtArr(Double_t binmin, Double_t binmax, string* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
  } else {
    *arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
  }
}

