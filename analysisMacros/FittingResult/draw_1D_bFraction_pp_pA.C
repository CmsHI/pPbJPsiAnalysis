#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void draw_1D_bFraction_pp_pA(TString szBinning="8rap9pt", bool isLog=false)
{
  gROOT->Macro("../Style.C");
  
  ////rap array in yCM (from forward to backward)
  Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  //Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
  const Int_t nRap = sizeof(rapArrNumFB)/sizeof(Double_t)-1;
  cout << "nRap = " << nRap << endl;
  Double_t rapBinW[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
    cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
  }
  ////pt array
  Double_t ptArrNum[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.}; //8rap9pt
  //Double_t ptArrNum[] = {5.0, 6.5, 10., 30.}; //8rap3pt
  const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;
  cout << "nPt = " << nPt << endl;
  Double_t ptBinW[nPt];
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
    cout << ipt <<"th ptBinW = " << ptBinW[ipt] <<endl;
  }
  //// array TString
  TString rapArr[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
    cout << iy <<"th rapArr = " << rapArr[iy] << endl;
  }
  TString ptArr[nPt];
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
    cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
  }
  
  // --- read-in file
  TFile * f2D_pA = new TFile(Form("../FittingResult/totalHist_pA_%s_newcut_nominal_Zvtx1_SF1_etOpt0.root",szBinning.Data()));
  TFile * f2D_pp = new TFile(Form("../FittingResult/totalHist_pp_%s_newcut_nominal_Zvtx0_SF1_etOpt0.root",szBinning.Data()));
  //TFile * f2D_pA = new TFile("../FittingSys/fitResHist_8rap9pt_pA_newcut_sys04_01_etOpt0.root");
  //TFile * f2D_pp = new TFile("../FittingSys/fitResHist_8rap9pt_pp_newcut_sys04_01_etOpt0.root");

  // --- read-in 2D hist for data reco dist
  TH2D* h2D_pA = (TH2D*)f2D_pA->Get("otherFitInfo/h2D_Fit_bFrac_pA");
  TH2D* h2D_pp = (TH2D*)f2D_pp->Get("otherFitInfo/h2D_Fit_bFrac_pp");
  //TH2D* h2D_pA = (TH2D*)f2D_pA->Get("h2D_bFraction");
  //TH2D* h2D_pp = (TH2D*)f2D_pp->Get("h2D_bFraction");
  cout << "h2D_pA = " << h2D_pA << endl;
  cout << "h2D_pp = " << h2D_pp << endl;

  const int nbinsX = h2D_pA->GetNbinsX();
  const int nbinsY = h2D_pA->GetNbinsY();
  cout << "nbinsX = " << nbinsX << endl;
  cout << "nbinsY = " << nbinsY << endl;
  if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
  if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
  
  // ---  projection to 1D hist
  TH1D* h1D_pA[nbinsX]; 
  //TH1D* h1D_Pbp[nbinsX]; 
  //TH1D* h1D_pPb[nbinsX]; 
  TH1D* h1D_pp[nbinsX]; 
  
  //// iy=0 refers to forwards !!! (ordering here)
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    h1D_pA[iy] = h2D_pA->ProjectionY(Form("h1D_pA_%d",iy),iy+1,iy+1);
    //h1D_Pbp[iy] = h2D_Pbp->ProjectionY(Form("h1D_Pbp_%d",iy),iy+1,iy+1);
    //h1D_pPb[iy] = h2D_pPb->ProjectionY(Form("h1D_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
    h1D_pp[iy] = h2D_pp->ProjectionY(Form("h1D_pp_%d",iy),nbinsX-iy,nbinsX-iy);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  
  //// --- set values as zero for unused bins
  ////8rap9pt
  for (Int_t iy = 0; iy < nbinsX; iy++) {
      h1D_pA[iy]->SetBinContent(1,0);
      h1D_pA[iy]->SetBinError(1,0);
      //h1D_Pbp[iy]->SetBinContent(1,0);
      //h1D_Pbp[iy]->SetBinError(1,0);
      //h1D_pPb[iy]->SetBinContent(1,0);
      //h1D_pPb[iy]->SetBinError(1,0);
      if(iy!=nbinsX-1) h1D_pp[iy+1]->SetBinContent(1,0);
      if(iy!=nbinsX-1) h1D_pp[iy+1]->SetBinError(1,0);
    if (iy>=1 && iy<=6) {
      h1D_pA[iy]->SetBinContent(2,0);
      h1D_pA[iy]->SetBinError(2,0);
      //h1D_Pbp[iy]->SetBinContent(2,0);
      //h1D_Pbp[iy]->SetBinError(2,0);
      //h1D_pPb[iy]->SetBinContent(2,0);
      //h1D_pPb[iy]->SetBinError(2,0);
      h1D_pp[iy+1]->SetBinContent(2,0);
      h1D_pp[iy+1]->SetBinError(2,0);
    }
    if (iy>=2 && iy<=5) {
      h1D_pA[iy]->SetBinContent(3,0);
      h1D_pA[iy]->SetBinError(3,0);
      //h1D_Pbp[iy]->SetBinContent(3,0);
      //h1D_Pbp[iy]->SetBinError(3,0);
      //h1D_pPb[iy]->SetBinContent(3,0);
      //h1D_pPb[iy]->SetBinError(3,0);
      h1D_pp[iy+1]->SetBinContent(3,0);
      h1D_pp[iy+1]->SetBinError(3,0);
    }
    if (iy>=2 && iy<=4) {
      h1D_pA[iy]->SetBinContent(4,0);
      h1D_pA[iy]->SetBinError(4,0);
      //h1D_Pbp[iy]->SetBinContent(4,0);
      //h1D_Pbp[iy]->SetBinError(4,0);
      //h1D_pPb[iy]->SetBinContent(4,0);
      //h1D_pPb[iy]->SetBinError(4,0);
      h1D_pp[iy+1]->SetBinContent(4,0);
      h1D_pp[iy+1]->SetBinError(4,0);
    }
  }
  /*
  //// 8rap3pt
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    if (iy>=1 && iy<=4) {
      h1D_pA[iy]->SetBinContent(1,0);
      h1D_pA[iy]->SetBinError(1,0);
      //h1D_Pbp[iy]->SetBinContent(1,0);
      //h1D_Pbp[iy]->SetBinError(1,0);
      //h1D_pPb[iy]->SetBinContent(1,0);
      //h1D_pPb[iy]->SetBinError(1,0);
      h1D_pp[iy+1]->SetBinContent(1,0);
      h1D_pp[iy+1]->SetBinError(1,0);
    }
  }
  */
  ////// after zero-bin setting, normalize!
//  for (Int_t iy = 0; iy < nbinsX; iy++) {
//    h1D_Pbp[iy]->Scale(1./h1D_Pbp[iy]->Integral());
//    h1D_Pbp[iy]->Scale(1,"width");
//  }
  
  //////////////////////////////////////////////////////////////////
  //// --- Draw histograms

  TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92);
  TLegend *legBL = new TLegend(0.18, 0.18, 0.59, 0.35);
  TLegend *legBR = new TLegend(0.49, 0.28, 0.86, 0.40);
  SetLegendStyle(legUR);
  SetLegendStyle(legBL);
  SetLegendStyle(legBR);
    
  //latex box for beam, rapidity, pT info
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  TCanvas* c_multi = new TCanvas("c_multi","c_multi",200,10,1600,800);
  c_multi->Divide(4,2);
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_multi->cd(iy+1);
    if (isLog) gPad->SetLogy(1);
    else gPad->SetLogy(0);
    //SetHistStyle(h1D_pA[iy],0,0);
    //SetHistStyle(h1D_Pbp[iy],3,10);
    //SetHistStyle(h1D_pPb[iy],5,10);
    //SetHistStyle(h1D_pp[iy],6,13);
    //SetHistStyle(h1D_pA[iy],1,0);
    //SetHistStyle(h1D_pp[iy],2,10);
    SetHistStyle(h1D_pA[iy],3,0);
    SetHistStyle(h1D_pp[iy],5,10);
    
    h1D_pA[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h1D_pA[iy]->GetXaxis()->CenterTitle();
    h1D_pA[iy]->GetXaxis()->SetRangeUser(0.,30.);
    h1D_pA[iy]->GetYaxis()->SetTitle("Bfraction");
    //h1D_pA[iy]->GetYaxis()->SetRangeUser(0,1.0);
    h1D_pA[iy]->GetYaxis()->SetRangeUser(0,0.6);
    
    /*
    h1D_pp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h1D_pp[iy]->GetXaxis()->CenterTitle();
    h1D_pp[iy]->GetXaxis()->SetRangeUser(0.,30.);
    h1D_pp[iy]->GetYaxis()->SetTitle("Bfraction");
    h1D_pp[iy]->GetYaxis()->SetRangeUser(0,1.0);
    h1D_pp[iy]->GetYaxis()->SetRangeUser(0,0.6);
    */
    h1D_pA[iy]->Draw("pe");
    if(iy!=nbinsX-1) h1D_pp[iy+1]->Draw("pe same");
    else h1D_pp[0]->Draw("pe same");
    if (iy==0) {
      //legBR -> AddEntry(h1D_Pbp[iy],"Pbp","lp");
      //legBR -> AddEntry(h1D_pPb[iy],"pPb","lp");
      legBR -> AddEntry(h1D_pA[iy],"pA","lp");
      legBR -> AddEntry(h1D_pp[iy],"pp","lp");
      //legBR->Draw();
    }
    if (iy==nbinsX-1) { 
      latex->SetTextColor(kBlack);
      latex->DrawLatex(0.49,0.32,"un-overlapped region");
      latex->SetTextColor(kAzure+9);
      latex->DrawLatex(0.49,0.26,"pp : 1.93 < y_{CM} < 2.4");
      latex->SetTextColor(kOrange+7);
      latex->DrawLatex(0.49,0.20,"pPb : -2.87 < y_{CM} < -2.4");
    }
    else  { 
      latex->SetTextColor(kBlack);
      latex->DrawLatex(0.49,0.20,Form("%s",rapArr[iy].Data()));
    }
  }
  c_multi->Modified();
  c_multi->Update();
  c_multi->SaveAs(Form("dir_1D_%s/bFraction_isLog%d_pp_pA.pdf",szBinning.Data(),(int)isLog));
  //c_multi->SaveAs(Form("dir_1D_%s/bFraction_isLog%d_wPA_nopp.pdf",szBinning.Data(),(int)isLog));
/*
  // save as a root file
  TFile *fOut = new TFile(Form("dir_1D_%s/bFraction_isLog%d.root",szBinning.Data(),(int)isLog),"RECREATE");
  fOut->cd();
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    h1D_Pbp[iy]->Write();
    h1D_pPb[iy]->Write();
    if(iy!=nbinsX-1) h1D_pp[iy+1]->Write();
  }
  fOut->Close();
*/
  return;

} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
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

void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr) {
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

void formPtArr(Double_t binmin, Double_t binmax, TString* arr) {
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


