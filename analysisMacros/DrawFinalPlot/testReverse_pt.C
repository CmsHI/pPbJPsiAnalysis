#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void testReverse_pt(bool isPrompt = true)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 10;
	int iPos=33;
  
  gStyle->SetPadBottomMargin(0.15); //KYO
  gStyle->SetPadLeftMargin(0.18); //KYO
  gStyle->SetPadRightMargin(0.05); //KYO
  
  //// read our result
	TFile *inFile01;
  if (isPrompt) inFile01 = new TFile("plot_RpPb/RpPb_pt_isPrompt1.root");
  else inFile01= new TFile("plot_RpPb/RpPb_pt_isPrompt0.root");
	TFile *inFile02;
  if (isPrompt) inFile02 = new TFile("plot_RpPb_testReverse/RpPb_pt_isPrompt1.root");
  else inFile02= new TFile("plot_RpPb_testReverse/RpPb_pt_isPrompt0.root");

  const int nRapRpPb =7;	
  
  //// array string
  Double_t rapArrNumFB[8] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};
  TString rapArr[nRapRpPb];
  for (Int_t iy=0; iy<nRapRpPb; iy++) {
    formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
    cout << iy <<"th rapArr = " << rapArr[iy] << endl;
  }
  
  TGraphAsymmErrors* g_RpPb_sys_01[nRapRpPb];
	TGraphAsymmErrors* g_RpPb_01[nRapRpPb];
	TGraphAsymmErrors* g_RpPb_sys_02[nRapRpPb]; 
	TGraphAsymmErrors* g_RpPb_02[nRapRpPb];
  for (int iy=0; iy<nRapRpPb; iy++) {
    g_RpPb_sys_01[iy]= (TGraphAsymmErrors*)inFile01->Get(Form("g_RpPb_sys_%d",iy)); 
    g_RpPb_01[iy]= (TGraphAsymmErrors*)inFile01->Get(Form("g_RpPb_%d",iy)); 
    g_RpPb_sys_02[iy]= (TGraphAsymmErrors*)inFile02->Get(Form("g_RpPb_sys_%d",iy)); 
    g_RpPb_02[iy]= (TGraphAsymmErrors*)inFile02->Get(Form("g_RpPb_%d",iy)); 
    g_RpPb_sys_01[iy]->SetFillColorAlpha(kRed-10,0.5);
    g_RpPb_sys_02[iy]->SetFillColorAlpha(kBlue-10,0.5);
    g_RpPb_02[iy]->SetMarkerColor(kBlue-2);
    g_RpPb_02[iy]->SetLineColor(kBlue-2);
	}

  //////////////////////////////////////////////////////////////
  
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(22); //1:left, 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.07);
	
  TLegend *leg = new TLegend(0.25, 0.22, 0.75, 0.46);
	SetLegendStyle(leg);
	leg->SetTextSize(0.07);

  ////// actual draw
  TCanvas *c_all = new TCanvas("c_all","c_all",1400,700);
  c_all->Divide(4,2); 
  for (Int_t iy = 0; iy < nRapRpPb; iy++) { 
    if (iy==0) c_all->cd(3);
    else if (iy==1) c_all->cd(2);
    else if (iy==2) c_all->cd(1);
    else c_all->cd(iy+2);
    //g_RpPb_sys_01[iy]->GetXaxis()->SetTitleSize(0.050);
    //g_RpPb_sys_01[iy]->GetYaxis()->SetTitleSize(0.050);
    g_RpPb_sys_01[iy]->Draw("A5");
    g_RpPb_sys_02[iy]->Draw("5");
    solidLine(0.,1.,32.,1.,1,1);
    g_RpPb_01[iy]->Draw("p");
    g_RpPb_02[iy]->Draw("p");
    globtex->DrawLatex(0.54, 0.35, rapArr[iy].Data());
  }
	c_all->cd(4);
  leg->AddEntry(g_RpPb_01[0],"original","lp");
  leg->AddEntry(g_RpPb_02[0],"pp ref. flipped","lp");
  leg->Draw();
	globtex->SetTextSize(0.09);
  if (isPrompt) globtex->DrawLatex(0.51, 0.7, "Prompt J/#psi");
	else globtex->DrawLatex(0.51, 0.7, "Nonprompt J/#psi");
	
  c_all->SaveAs(Form("plot_RpPb_testReverse/testReverse_pt_isPrompt%d.pdf",(int)isPrompt));
  
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
