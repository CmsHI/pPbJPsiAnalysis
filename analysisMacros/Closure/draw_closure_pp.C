#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

int draw_closure_pp(bool useZvtxWeight=false, bool useSF = false, bool isLog = false)
{
  gROOT->Macro("../Style.C");
  using namespace std;

  ////rap array in yCM (from forward to backward)
  //-- for pp
  Double_t rapArrNumFB[] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};// for pt dist.
  Double_t rapArrNumBF[] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};// for rap dist.
  //-- for pA
  //Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  //Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
  const Int_t nRap = sizeof(rapArrNumFB)/sizeof(Double_t)-1;
  cout << "nRap = " << nRap << endl;
  ////pt array
  Double_t ptArrNum[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
  const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;
  cout << "nPt = " << nPt << endl;
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
  TFile * f2D = new TFile(Form("../FittingResult/totalHist_pp_8rap9pt_newcut_Zvtx%d_SF%d_noPtWeight.root",(int)useZvtxWeight,(int)useSF),"READ");
  // --- read-in 2D hist for MC corrected yield
  TH2D* h2D_MCCorrY_PR_pp = (TH2D*)f2D->Get("otherMCInfo/h2D_MCCorrY_PR_pp");
  TH2D* h2D_MCCorrY_NP_pp = (TH2D*)f2D->Get("otherMCInfo/h2D_MCCorrY_NP_pp");
  // --- read-in 2D hist for GEN dist.
  TH2D* h2D_Acc_Den_PR_pp = (TH2D*)f2D->Get("otherMCInfo/h2D_Acc_Den_PR_pp");
  TH2D* h2D_Acc_Den_NP_pp = (TH2D*)f2D->Get("otherMCInfo/h2D_Acc_Den_NP_pp");
  
  const int nbinsX = h2D_MCCorrY_PR_pp->GetNbinsX();
  const int nbinsY = h2D_MCCorrY_PR_pp->GetNbinsY();
  cout << "nbinsX = " << nbinsX << ", nbinsY = " << nbinsY << endl;
  if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return 0; };
  if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return 0; };
  
  // ---  projection to 1D hist
  TH1D* h1D_MCCorrY_PR_pp[nbinsX]; 
  TH1D* h1D_MCCorrY_NP_pp[nbinsX]; 
  TH1D* h1D_Acc_Den_PR_pp[nbinsX]; 
  TH1D* h1D_Acc_Den_NP_pp[nbinsX]; 
  // iy=0 refers to forwards !!! (ordering here)
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    ////// for corrY
    h1D_MCCorrY_PR_pp[iy] = h2D_MCCorrY_PR_pp->ProjectionY(Form("h1D_MCCorrY_PR_pp_%d",iy),iy+1,iy+1);
    h1D_MCCorrY_NP_pp[iy] = h2D_MCCorrY_NP_pp->ProjectionY(Form("h1D_MCCorrY_NP_pp_%d",iy),iy+1,iy+1);
    //h1D_MCCorrY_PR_pPb[iy] = h2D_MCCorrY_PR_pPb->ProjectionY(Form("h1D_MCCorrY_PR_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
    //h1D_MCCorrY_NP_pPb[iy] = h2D_MCCorrY_NP_pPb->ProjectionY(Form("h1D_MCCorrY_NP_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
    ////// for GEN dist.
    h1D_Acc_Den_PR_pp[iy] = h2D_Acc_Den_PR_pp->ProjectionY(Form("h1D_Acc_Den_PR_pp_%d",iy),iy+1,iy+1);
    h1D_Acc_Den_NP_pp[iy] = h2D_Acc_Den_NP_pp->ProjectionY(Form("h1D_Acc_Den_NP_pp_%d",iy),iy+1,iy+1);
    //h1D_Acc_Den_PR_pPb[iy] = h2D_Acc_Den_PR_pPb->ProjectionY(Form("h1D_Acc_Den_PR_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
    //h1D_Acc_Den_NP_pPb[iy] = h2D_Acc_Den_NP_pPb->ProjectionY(Form("h1D_Acc_Den_NP_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
  }

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  
  // set values as zero for unused bins
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    if (iy>=1 && iy<=6) {
      h1D_MCCorrY_PR_pp[iy]->SetBinContent(1,0);
      h1D_MCCorrY_NP_pp[iy]->SetBinContent(1,0);
      h1D_MCCorrY_PR_pp[iy]->SetBinError(1,0);
      h1D_MCCorrY_NP_pp[iy]->SetBinError(1,0);
      h1D_MCCorrY_PR_pp[iy]->SetBinContent(2,0);
      h1D_MCCorrY_NP_pp[iy]->SetBinContent(2,0);
      h1D_MCCorrY_PR_pp[iy]->SetBinError(2,0);
      h1D_MCCorrY_NP_pp[iy]->SetBinError(2,0);
      h1D_Acc_Den_PR_pp[iy]->SetBinContent(1,0);
      h1D_Acc_Den_NP_pp[iy]->SetBinContent(1,0);
      h1D_Acc_Den_PR_pp[iy]->SetBinError(1,0);
      h1D_Acc_Den_NP_pp[iy]->SetBinError(1,0);
      h1D_Acc_Den_PR_pp[iy]->SetBinContent(2,0);
      h1D_Acc_Den_NP_pp[iy]->SetBinContent(2,0);
      h1D_Acc_Den_PR_pp[iy]->SetBinError(2,0);
      h1D_Acc_Den_NP_pp[iy]->SetBinError(2,0);
    }
    if (iy>=2 && iy<=5) {
      h1D_MCCorrY_PR_pp[iy]->SetBinContent(3,0);
      h1D_MCCorrY_NP_pp[iy]->SetBinContent(3,0);
      h1D_MCCorrY_PR_pp[iy]->SetBinError(3,0);
      h1D_MCCorrY_NP_pp[iy]->SetBinError(3,0);
      h1D_Acc_Den_PR_pp[iy]->SetBinContent(3,0);
      h1D_Acc_Den_NP_pp[iy]->SetBinContent(3,0);
      h1D_Acc_Den_PR_pp[iy]->SetBinError(3,0);
      h1D_Acc_Den_NP_pp[iy]->SetBinError(3,0);
    }
    //if (iy>=2 && iy<=4) { // for pA
    if (iy>=2 && iy<=5) { // for PP
      h1D_MCCorrY_PR_pp[iy]->SetBinContent(4,0);
      h1D_MCCorrY_NP_pp[iy]->SetBinContent(4,0);
      h1D_MCCorrY_PR_pp[iy]->SetBinError(4,0);
      h1D_MCCorrY_NP_pp[iy]->SetBinError(4,0);
      h1D_Acc_Den_PR_pp[iy]->SetBinContent(4,0);
      h1D_Acc_Den_NP_pp[iy]->SetBinContent(4,0);
      h1D_Acc_Den_PR_pp[iy]->SetBinError(4,0);
      h1D_Acc_Den_NP_pp[iy]->SetBinError(4,0);
    }
  }
  
  ////// after zero-bin setting, normalize!
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    h1D_MCCorrY_PR_pp[iy]->Scale(1./h1D_MCCorrY_PR_pp[iy]->Integral());
    h1D_MCCorrY_NP_pp[iy]->Scale(1./h1D_MCCorrY_NP_pp[iy]->Integral());
    h1D_Acc_Den_PR_pp[iy]->Scale(1./h1D_Acc_Den_PR_pp[iy]->Integral());
    h1D_Acc_Den_NP_pp[iy]->Scale(1./h1D_Acc_Den_NP_pp[iy]->Integral());
    h1D_MCCorrY_PR_pp[iy]->Scale(1,"width");
    h1D_MCCorrY_NP_pp[iy]->Scale(1,"width");
    h1D_Acc_Den_PR_pp[iy]->Scale(1,"width");
    h1D_Acc_Den_NP_pp[iy]->Scale(1,"width");
  }

  //////////////////////////////////////////////////////////////////
  //// --- Draw histograms
  
  TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92);
  SetLegendStyle(legUR);
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  // --- prompt pp
  TCanvas* c_PR_pp = new TCanvas("c_PR_pp","c_PR_pp",1600,800);
  c_PR_pp->Divide(4,2);
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_PR_pp->cd(iy+1);
    if (isLog) gPad->SetLogy(1);
    else gPad->SetLogy(0);
    SetHistStyle(h1D_MCCorrY_PR_pp[iy],1,0);
    SetHistStyle(h1D_Acc_Den_PR_pp[iy],2,10);
    h1D_MCCorrY_PR_pp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h1D_MCCorrY_PR_pp[iy]->GetXaxis()->CenterTitle();
    //h1D_MCCorrY_PR_pp[iy]->GetXaxis()->SetRangeUser(0.,30.);
    //h1D_Acc_Den_PR_pp[iy]->GetXaxis()->SetRangeUser(0.,30.);
    h1D_MCCorrY_PR_pp[iy]->GetYaxis()->SetTitle("");
    h1D_MCCorrY_PR_pp[iy]->Draw("pe");
    h1D_Acc_Den_PR_pp[iy]->Draw("pe same");
    if (iy==0) {
      legUR -> SetHeader("pp Prompt J/#psi");
      legUR -> AddEntry(h1D_MCCorrY_PR_pp[iy],"RECO/(Acc*Eff)","lp");
      legUR -> AddEntry(h1D_Acc_Den_PR_pp[iy],"GEN","lp");
      legUR->Draw();
    }
    latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
  }
  c_PR_pp->SaveAs(Form("dir_closure/pp_PR_isLog%d.pdf",(int)isLog));
  
  // --- non-prompt pp
  TCanvas* c_NP_pp = new TCanvas("c_NP_pp","c_NP_pp",1600,800);
  c_NP_pp->Divide(4,2);
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_NP_pp->cd(iy+1);
    if (isLog) gPad->SetLogy(1);
    else gPad->SetLogy(0);
    SetHistStyle(h1D_MCCorrY_NP_pp[iy],1,0);
    SetHistStyle(h1D_Acc_Den_NP_pp[iy],2,10);
    h1D_MCCorrY_NP_pp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h1D_MCCorrY_NP_pp[iy]->GetXaxis()->CenterTitle();
    //h1D_MCCorrY_NP_pp[iy]->GetXaxis()->SetRangeUser(0.,30.);
    //h1D_MCCorrY_NP_pp[iy]->GetYaxis()->SetTitle("");
    h1D_MCCorrY_NP_pp[iy]->Draw("pe");
    h1D_Acc_Den_NP_pp[iy]->Draw("pe same");
    if (iy==0) {
      legUR -> SetHeader("pp Non-prompt J/#psi");
      legUR->Draw();
    }
    latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
  }
  c_NP_pp->SaveAs(Form("dir_closure/pp_NP_isLog%d.pdf",(int)isLog));
  
  //////////////////////////////////////////////////////////////////
  // hRatio
  TH1D* hRatio_PR_pp[nRap];
  TH1D* hRatio_NP_pp[nRap];
  
  // --- prompt pp
  TCanvas* c_PR_pp_2 = new TCanvas("c_PR_pp_2","c_PR_pp_2",1600,800);
  c_PR_pp_2->Divide(4,2);
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_PR_pp_2->cd(iy+1);
    gPad->SetLogy(0);
    hRatio_PR_pp[iy]=(TH1D*)h1D_MCCorrY_PR_pp[iy]->Clone(Form("hRatio_PR_pp_%d",iy));
    hRatio_PR_pp[iy]->Divide(h1D_Acc_Den_PR_pp[iy]);
    SetHistStyle(hRatio_PR_pp[iy],5,0);
    hRatio_PR_pp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hRatio_PR_pp[iy]->GetYaxis()->SetTitle("corrected RECO / GEN");
    //hRatio_PR_pp[iy]->GetXaxis()->SetRangeUser(0.,30.);
    hRatio_PR_pp[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
    hRatio_PR_pp[iy]->Draw("pe");
    latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
    dashedLine(0.,1.,30.,1.,1,1);
  }
  c_PR_pp_2->SaveAs("dir_closure/pp_PR_ratio.pdf");
  
  // --- non-prompt pp
  TCanvas* c_NP_pp_2 = new TCanvas("c_NP_pp_2","c_NP_pp_2",1600,800);
  c_NP_pp_2->Divide(4,2);
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_NP_pp_2->cd(iy+1);
    gPad->SetLogy(0);
    hRatio_NP_pp[iy]=(TH1D*)h1D_MCCorrY_NP_pp[iy]->Clone(Form("hRatio_NP_pp_%d",iy));
    hRatio_NP_pp[iy]->Divide(h1D_Acc_Den_NP_pp[iy]);
    SetHistStyle(hRatio_NP_pp[iy],5,0);
    hRatio_NP_pp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hRatio_NP_pp[iy]->GetYaxis()->SetTitle("corrected RECO / GEN");
    //hRatio_NP_pp[iy]->GetXaxis()->SetRangeUser(0.,30.);
    hRatio_NP_pp[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
    hRatio_NP_pp[iy]->Draw("pe");
    latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
    dashedLine(0.,1.,30.,1.,1,1);
  }
  c_NP_pp_2->SaveAs("dir_closure/pp_NP_ratio.pdf");
  
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

