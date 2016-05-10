#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

int draw_closure_pA(int MrapNpt=89, int isPA =3, int accCutType = 2, bool useZvtxWeight=false, bool useSF = false)
{
  gROOT->Macro("../Style.C");
  using namespace std;

 bool isLog = false;  
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return 0; }
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==3) szPA="pA";
  else {cout << "select among isPA = 0 or 3"<< endl; return 0; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 0 or 1"<< endl; return 0; }
  const TString szFinal = Form("%s_%s_%s",szBinning.Data(),szPA.Data(),szAccCut.Data());
  std::cout << "szFinal: " << szFinal << std::endl;
  
  //rap array in yCM (from forward to backward)
  //// for pp
  //Double_t rapArrNumFB[] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};// for pt dist.
  //Double_t rapArrNumBF[] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};// for rap dist.
  //// for pA
  Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
  const Int_t nRap = sizeof(rapArrNumFB)/sizeof(Double_t)-1;
  cout << "nRap = " << nRap << endl;
  Double_t rapBinW[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
    cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
  }
  //pt array
  Double_t ptArrNum[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
  const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;
  cout << "nPt = " << nPt << endl;
  Double_t ptBinW[nPt];
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
    cout << ipt <<"th ptBinW = " << ptBinW[ipt] <<endl;
  }
  // array string
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
  //TFile * f2D = new TFile(Form("../000_fittingResult/total2Dhist_%s.root",dirName));
  TFile * f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_Zvtx0_SF0_noPtWeight.root","READ");
  //cout << "dirName = " << dirName << endl;

  // --- read-in 2D hist for MC corrected yield
  TH2D* h2D_MCCorrY_PR_pA = (TH2D*)f2D->Get("otherMCInfo/h2D_MCCorrY_PR_pA");
  TH2D* h2D_MCCorrY_NP_pA = (TH2D*)f2D->Get("otherMCInfo/h2D_MCCorrY_NP_pA");
  cout << "h2D_MCCorrY_PR_pA = " << h2D_MCCorrY_PR_pA << endl;
  cout << "h2D_MCCorrY_NP_pA = " << h2D_MCCorrY_NP_pA << endl;

  const int nbinsX = h2D_MCCorrY_PR_pA->GetNbinsX();
  const int nbinsY = h2D_MCCorrY_PR_pA->GetNbinsY();
  cout << "nbinsX = " << nbinsX << endl;
  cout << "nbinsY = " << nbinsY << endl;
  if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return 0; };
  if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return 0; };
  
  // --- read-in 2D hist for GEN dist.
  TH2D* h2D_Acc_Den_PR_pA = (TH2D*)f2D->Get("otherMCInfo/h2D_Acc_Den_PR_pA");
  TH2D* h2D_Acc_Den_NP_pA = (TH2D*)f2D->Get("otherMCInfo/h2D_Acc_Den_NP_pA");
  cout << "h2D_Acc_Den_PR_pA = " << h2D_Acc_Den_PR_pA << endl;
  cout << "h2D_Acc_Den_NP_pA = " << h2D_Acc_Den_NP_pA << endl;

  // ---  projection to 1D hist
  TH1D* h1D_MCCorrY_PR_pA[nbinsX]; 
  TH1D* h1D_MCCorrY_NP_pA[nbinsX]; 
  TH1D* h1D_Acc_Den_PR_pA[nbinsX]; 
  TH1D* h1D_Acc_Den_NP_pA[nbinsX]; 
  // iy=0 refers to forwards !!! (ordering here)
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    ////// for corrY
    h1D_MCCorrY_PR_pA[iy] = h2D_MCCorrY_PR_pA->ProjectionY(Form("h1D_MCCorrY_PR_pA_%d",iy),iy+1,iy+1);
    h1D_MCCorrY_NP_pA[iy] = h2D_MCCorrY_NP_pA->ProjectionY(Form("h1D_MCCorrY_NP_pA_%d",iy),iy+1,iy+1);
    //h1D_MCCorrY_PR_pPb[iy] = h2D_MCCorrY_PR_pPb->ProjectionY(Form("h1D_MCCorrY_PR_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
    //h1D_MCCorrY_NP_pPb[iy] = h2D_MCCorrY_NP_pPb->ProjectionY(Form("h1D_MCCorrY_NP_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
    ////// for GEN dist.
    h1D_Acc_Den_PR_pA[iy] = h2D_Acc_Den_PR_pA->ProjectionY(Form("h1D_Acc_Den_PR_pA_%d",iy),iy+1,iy+1);
    h1D_Acc_Den_NP_pA[iy] = h2D_Acc_Den_NP_pA->ProjectionY(Form("h1D_Acc_Den_NP_pA_%d",iy),iy+1,iy+1);
    //h1D_Acc_Den_PR_pPb[iy] = h2D_Acc_Den_PR_pPb->ProjectionY(Form("h1D_Acc_Den_PR_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
    //h1D_Acc_Den_NP_pPb[iy] = h2D_Acc_Den_NP_pPb->ProjectionY(Form("h1D_Acc_Den_NP_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  
  // set values as zero for unused bins
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    if (iy>=1 && iy<=6) {
      h1D_MCCorrY_PR_pA[iy]->SetBinContent(1,0);
      h1D_MCCorrY_NP_pA[iy]->SetBinContent(1,0);
      h1D_MCCorrY_PR_pA[iy]->SetBinError(1,0);
      h1D_MCCorrY_NP_pA[iy]->SetBinError(1,0);
      h1D_Acc_Den_PR_pA[iy]->SetBinContent(1,0);
      h1D_Acc_Den_NP_pA[iy]->SetBinContent(1,0);
      h1D_Acc_Den_PR_pA[iy]->SetBinError(1,0);
      h1D_Acc_Den_NP_pA[iy]->SetBinError(1,0);
    }
    if (iy>=2 && iy<=5) {
      h1D_MCCorrY_PR_pA[iy]->SetBinContent(2,0);
      h1D_MCCorrY_PR_pA[iy]->SetBinContent(3,0);
      h1D_MCCorrY_NP_pA[iy]->SetBinContent(2,0);
      h1D_MCCorrY_NP_pA[iy]->SetBinContent(3,0);
      h1D_MCCorrY_PR_pA[iy]->SetBinError(2,0);
      h1D_MCCorrY_PR_pA[iy]->SetBinError(3,0);
      h1D_MCCorrY_NP_pA[iy]->SetBinError(2,0);
      h1D_MCCorrY_NP_pA[iy]->SetBinError(3,0);
      h1D_Acc_Den_PR_pA[iy]->SetBinContent(2,0);
      h1D_Acc_Den_PR_pA[iy]->SetBinContent(3,0);
      h1D_Acc_Den_NP_pA[iy]->SetBinContent(2,0);
      h1D_Acc_Den_NP_pA[iy]->SetBinContent(3,0);
      h1D_Acc_Den_PR_pA[iy]->SetBinError(2,0);
      h1D_Acc_Den_PR_pA[iy]->SetBinError(3,0);
      h1D_Acc_Den_NP_pA[iy]->SetBinError(2,0);
      h1D_Acc_Den_NP_pA[iy]->SetBinError(3,0);
    }
    if (iy>=2 && iy<=4) {
      h1D_MCCorrY_PR_pA[iy]->SetBinContent(4,0);
      h1D_MCCorrY_NP_pA[iy]->SetBinContent(4,0);
      h1D_MCCorrY_PR_pA[iy]->SetBinError(4,0);
      h1D_MCCorrY_NP_pA[iy]->SetBinError(4,0);
      h1D_Acc_Den_PR_pA[iy]->SetBinContent(4,0);
      h1D_Acc_Den_NP_pA[iy]->SetBinContent(4,0);
      h1D_Acc_Den_PR_pA[iy]->SetBinError(4,0);
      h1D_Acc_Den_NP_pA[iy]->SetBinError(4,0);
    }
  }
  
  ////// after zero-bin setting, normalize!
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    h1D_MCCorrY_PR_pA[iy]->Scale(1./h1D_MCCorrY_PR_pA[iy]->Integral());
    h1D_MCCorrY_NP_pA[iy]->Scale(1./h1D_MCCorrY_NP_pA[iy]->Integral());
    h1D_Acc_Den_PR_pA[iy]->Scale(1./h1D_Acc_Den_PR_pA[iy]->Integral());
    h1D_Acc_Den_NP_pA[iy]->Scale(1./h1D_Acc_Den_NP_pA[iy]->Integral());
    h1D_MCCorrY_PR_pA[iy]->Scale(1,"width");
    h1D_MCCorrY_NP_pA[iy]->Scale(1,"width");
    h1D_Acc_Den_PR_pA[iy]->Scale(1,"width");
    h1D_Acc_Den_NP_pA[iy]->Scale(1,"width");
  }
  //////////////////////////////////////////////////////////////////
  //// --- Draw histograms

  //TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upAer left
  TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92); //upAer left
  TLegend *legBL = new TLegend(0.18, 0.18, 0.59, 0.35); //upAer left
  SetLegendStyle(legUR);
  SetLegendStyle(legBL);
    
  //latex box for beam, rapidity, pT info
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  // --- prompt pA
  //TCanvas* c_PR_pA = new TCanvas("c_PR_pA","c_PR_pA",200,10,600,600);
  //c_PR_pA->cd();
  TCanvas* c_PR_pA = new TCanvas("c_PR_pA","c_PR_pA",1600,800);
  c_PR_pA->Divide(4,2);
  if (isLog) gPad->SetLogy(1);
  else gPad->SetLogy(0);
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_PR_pA->cd(iy+1);
    SetHistStyle(h1D_MCCorrY_PR_pA[iy],1,0);
    SetHistStyle(h1D_Acc_Den_PR_pA[iy],2,10);
    h1D_MCCorrY_PR_pA[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h1D_MCCorrY_PR_pA[iy]->GetXaxis()->CenterTitle();
    h1D_MCCorrY_PR_pA[iy]->GetXaxis()->SetRangeUser(0.,30.);
    h1D_MCCorrY_PR_pA[iy]->GetYaxis()->SetTitle("");
    //if (isLog) h1D_corrY_PR_pA[iy]->GetYaxis()->SetRangeUser(0.01,700);
    //else h1D_corrY_PR_pA[iy]->GetYaxis()->SetRangeUser(0.0,350);
    h1D_MCCorrY_PR_pA[iy]->Draw("pe");
    h1D_Acc_Den_PR_pA[iy]->Draw("pe same");
    if (iy==0) {
      legUR -> SetHeader("Prompt J/#psi 1st run");
      legUR -> AddEntry(h1D_MCCorrY_PR_pA[iy],"RECO/(Acc*Eff)","lp");
      legUR -> AddEntry(h1D_Acc_Den_PR_pA[iy],"GEN","lp");
      //legUR->Draw();
    }
    latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
//    c_PR_pA->SaveAs(Form("MCClosure_%s/mc_closure_pt_PR_pA_isLog%d_%d.pdf",dirName,(int)isLog,iy));
//    c_PR_pA->Clear();
//    legUR->Clear();
  }
  
  // --- non-prompt pA
  //TCanvas* c_NP_pA = new TCanvas("c_NP_pA","c_NP_pA",200,10,600,600);
  //c_NP_pA->cd();
  TCanvas* c_NP_pA = new TCanvas("c_NP_pA","c_NP_pA",1600,800);
  c_NP_pA->Divide(4,2);
  if (isLog) gPad->SetLogy(1);
  else gPad->SetLogy(0);
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_NP_pA->cd(iy+1);
    SetHistStyle(h1D_MCCorrY_NP_pA[iy],1,0);
    SetHistStyle(h1D_Acc_Den_NP_pA[iy],2,10);
    h1D_MCCorrY_NP_pA[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h1D_MCCorrY_NP_pA[iy]->GetXaxis()->CenterTitle();
    h1D_MCCorrY_NP_pA[iy]->GetXaxis()->SetRangeUser(0.,30.);
    h1D_MCCorrY_NP_pA[iy]->GetYaxis()->SetTitle("");
    //if (isLog) h1D_corrY_NP_pA[iy]->GetYaxis()->SetRangeUser(0.01,700);
    //else h1D_corrY_NP_pA[iy]->GetYaxis()->SetRangeUser(0.0,350);
    h1D_MCCorrY_NP_pA[iy]->Draw("pe");
    h1D_Acc_Den_NP_pA[iy]->Draw("pe same");
    if (iy==0) {
      legUR -> SetHeader("Non-prompt J/#psi 1st run");
      //legUR -> AddEntry(h1D_MCCorrY_PR_pA[iy],"RECO/(Acc*Eff)","lp");
      //legUR -> AddEntry(h1D_Acc_Den_PR_pA[iy],"GEN","lp");
      legUR->Draw();
    }
    latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
//    c_NP_pA->SaveAs(Form("MCClosure_%s/mc_closure_pt_NP_pA_isLog%d_%d.pdf",dirName,(int)isLog,iy));
//    c_NP_pA->Clear();
//    legUR->Clear();
  }
  
  //////////////////////////////////////////////////////////////////
  // hRatio
  TH1D* hRatio_PR_pA[nRap];
  TH1D* hRatio_NP_pA[nRap];
  
  // --- prompt pA
  gPad->SetLogy(0);
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_PR_pA->cd(iy+1);
    hRatio_PR_pA[iy]=(TH1D*)h1D_MCCorrY_PR_pA[iy]->Clone(Form("hRatio_PR_pA_%d",iy));
    hRatio_PR_pA[iy]->Divide(h1D_Acc_Den_PR_pA[iy]);
    SetHistStyle(hRatio_PR_pA[iy],5,0);
    hRatio_PR_pA[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hRatio_PR_pA[iy]->GetYaxis()->SetTitle("Ratio");
    hRatio_PR_pA[iy]->GetXaxis()->SetRangeUser(0.,30.);
    hRatio_PR_pA[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
    hRatio_PR_pA[iy]->Draw("pe");
    if (iy==0) {
      legBL -> SetHeader("Prompt J/#psi 1st run");
      legBL -> AddEntry(hRatio_PR_pA[iy],"[RECO/(Acc*Eff)] / GEN","lp");
      latex->DrawLatex(0.56,0.78,"[RECO/(Acc*Eff)] / GEN");
      //legBL->Draw();
    }
    latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
    dashedLine(0.,1.,30.,1.,1,1);
//    c_PR_pA->SaveAs(Form("MCClosure_%s/mc_closure_ratio_PR_pA_isLog%d_%d.pdf",dirName,(int)isLog,iy));
//    c_PR_pA->Clear();
//    legBL->Clear();
  }

  // --- non-prompt pA
  gPad->SetLogy(0);
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c_NP_pA->cd(iy+1);
    hRatio_NP_pA[iy]=(TH1D*)h1D_MCCorrY_NP_pA[iy]->Clone(Form("hRatio_NP_pA_%d",iy));
    hRatio_NP_pA[iy]->Divide(h1D_Acc_Den_NP_pA[iy]);
    SetHistStyle(hRatio_NP_pA[iy],5,0);
    hRatio_NP_pA[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hRatio_NP_pA[iy]->GetYaxis()->SetTitle("Ratio");
    hRatio_NP_pA[iy]->GetXaxis()->SetRangeUser(0.,30.);
    hRatio_NP_pA[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
    hRatio_NP_pA[iy]->Draw("pe");
    //legBL -> SetHeader("Non-prompt J/#psi 1st run");
    //legBL -> AddEntry(hRatio_NP_pA[iy],"[RECO/(Acc*Eff)] / GEN","lp");
    //legBL->Draw();
    latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
    dashedLine(0.,1.,30.,1.,1,1);
//    c_NP_pA->SaveAs(Form("MCClosure_%s/mc_closure_ratio_NP_pA_isLog%d_%d.pdf",dirName,(int)isLog,iy));
//    c_NP_pA->Clear();
//    legBL->Clear();
  }

/*
  // root file
  TFile *fOut = new TFile(Form("1Dhist_%s/crossSection_pt_%s.root",dirName,runstring.c_str()),"RECREATE");
  fOut->cd();
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    h1D_corrY_NP_pA[iy]->Write();
    h1D_corrY_NP_pPb[iy]->Write();
    h1D_corrY_NP_tot[iy]->Write();
  }
  //tRap->Write();
  fOut->Close();
*/
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


