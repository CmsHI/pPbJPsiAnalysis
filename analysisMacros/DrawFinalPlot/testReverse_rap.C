#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void testReverse_rap(bool isPrompt = true)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 10;
	int iPos=33;
  
  gStyle->SetPadBottomMargin(0.15); //KYO
  gStyle->SetPadLeftMargin(0.18); //KYO
  gStyle->SetPadRightMargin(0.05); //KYO
  
  //// read our result
	TFile *inFile01;
  if (isPrompt) inFile01 = new TFile("plot_RpPb/RpPb_rap_isPrompt1.root");
  else inFile01= new TFile("plot_RpPb/RpPb_rap_isPrompt0.root");
	TFile *inFile02;
  if (isPrompt) inFile02 = new TFile("plot_RpPb_testReverse/RpPb_rap_isPrompt1.root");
  else inFile02= new TFile("plot_RpPb_testReverse/RpPb_rap_isPrompt0.root");

  const int nPt = 2;
  
  //// array string
  Double_t ptArrNum[3] = {6.5, 10., 30.};
  TString ptArr[nPt];
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
    cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
  }
  
	TGraphAsymmErrors* g_RpPb_sys_highpt_01 = (TGraphAsymmErrors*)inFile01->Get("g_RpPb_sys_highpt"); 
	TGraphAsymmErrors* g_RpPb_highpt_01 = (TGraphAsymmErrors*)inFile01->Get("g_RpPb_highpt"); 
	TGraphAsymmErrors* g_RpPb_sys_lowpt_01 = (TGraphAsymmErrors*)inFile01->Get("g_RpPb_sys_lowpt"); 
	TGraphAsymmErrors* g_RpPb_lowpt_01 = (TGraphAsymmErrors*)inFile01->Get("g_RpPb_lowpt"); 
	TGraphAsymmErrors* g_RpPb_sys_highpt_02 = (TGraphAsymmErrors*)inFile02->Get("g_RpPb_sys_highpt"); 
	TGraphAsymmErrors* g_RpPb_highpt_02 = (TGraphAsymmErrors*)inFile02->Get("g_RpPb_highpt"); 
	TGraphAsymmErrors* g_RpPb_sys_lowpt_02 = (TGraphAsymmErrors*)inFile02->Get("g_RpPb_sys_lowpt"); 
	TGraphAsymmErrors* g_RpPb_lowpt_02 = (TGraphAsymmErrors*)inFile02->Get("g_RpPb_lowpt"); 
  g_RpPb_sys_highpt_01->SetFillColorAlpha(kRed-10,0.5);
  g_RpPb_sys_highpt_02->SetFillColorAlpha(kBlue-10,0.5);
  g_RpPb_highpt_02->SetMarkerColor(kBlue-2);
  g_RpPb_highpt_02->SetLineColor(kBlue-2);
  g_RpPb_sys_lowpt_01->SetFillColorAlpha(kRed-10,0.5);
  g_RpPb_sys_lowpt_02->SetFillColorAlpha(kBlue-10,0.5);
  g_RpPb_lowpt_02->SetMarkerColor(kBlue-2);
  g_RpPb_lowpt_02->SetLineColor(kBlue-2);

  //////////////////////////////////////////////////////////////
  
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(22); //1:left, 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.07);
  
  TLegend *leg = new TLegend(0.23, 0.40, 0.50, 0.52);
	SetLegendStyle(leg);
	leg->SetTextSize(0.04);

  ////// actual draw
  
  ////// lowpt
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1->cd();
  
  g_RpPb_sys_lowpt_01->Draw("A5");
  g_RpPb_sys_lowpt_02->Draw("5");
  solidLine(-2.7,1.,2.1,1.,1,1);
  g_RpPb_lowpt_01->Draw("P");
  g_RpPb_lowpt_02->Draw("P");
  
  leg->AddEntry(g_RpPb_lowpt_01,"original","lp");
  leg->AddEntry(g_RpPb_lowpt_02,"pp ref. flipped","lp");
  leg->Draw();
  
  globtex->SetTextAlign(22);
	globtex->SetTextFont(42);
  globtex->SetTextSize(0.05);
  globtex->DrawLatex(0.57, 0.25, "6.5 < p_{T} < 10 GeV/c");

  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextSize(0.048);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.82, 0.82, "Prompt J/#psi");
	else globtex->DrawLatex(0.82, 0.82, "Nonprompt J/#psi");
  
  c1->SaveAs(Form("plot_RpPb_testReverse/testReverse_rap_isPrompt%d_pt1.pdf",(int)isPrompt));

  ////// highpt
  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  c2->cd();
  
  g_RpPb_sys_highpt_01->Draw("A5");
  g_RpPb_sys_highpt_02->Draw("5");
  solidLine(-2.7,1.,2.1,1.,1,1);
  g_RpPb_highpt_01->Draw("P");
  g_RpPb_highpt_02->Draw("P");
  
  leg->Draw();
  
  globtex->SetTextAlign(22);
	globtex->SetTextFont(42);
  globtex->SetTextSize(0.05);
  globtex->DrawLatex(0.57, 0.25, "10 < p_{T} < 30 GeV/c");

  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextSize(0.048);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.82, 0.82, "Prompt J/#psi");
	else globtex->DrawLatex(0.82, 0.82, "Nonprompt J/#psi");
  
  c2->SaveAs(Form("plot_RpPb_testReverse/testReverse_rap_isPrompt%d_pt2.pdf",(int)isPrompt));



  
  return;

}

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( binmin == 1.93 || binmin == -1.93 || binmin == -2.87) {
		*arr = Form("%.2f < y_{CM} < %.1f", binmin, binmax);
  }
  else if ( binmax == 1.93 || binmax ==  -1.93 || binmax == -2.87 ) {
		*arr = Form("%.1f < y_{CM} < %.2f", binmin, binmax);
  }
  else if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < y_{CM} < %.1f", binmin, binmax);
  } else {
    *arr = Form("%.1f < y_{CM} < %.1f", binmin, binmax);
  }
}

void formPtArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < p_{T} < %.0f (GeV/c)", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < p_{T} < %.0f (GeV/c)", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < p_{T} < %.1f (GeV/c)", binmin, binmax);
  } else {
    *arr = Form("%.1f < p_{T} < %.1f (GeV/c)", binmin, binmax);
  }
}
