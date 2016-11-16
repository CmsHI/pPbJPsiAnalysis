#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RpPb_pt_Overlay(double ptmax=32, bool isPoint=true, bool isSmoothened=false)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 10;  // 0:pp, 1:pPb, 10 : pp & pPb together for RpPb plot
	int iPos=33;

  bool isPrompt=true;
  
  ///////////////////////////////////////////////////
  const int nRap = 8; 
  const int nRapTmp = nRap + 1;
	const int nRapRpPb = 7;
	const int nPtRpPb = 49;
  Double_t theory_px[nRapRpPb][nPtRpPb]; 
	Double_t theory_py[nRapRpPb][nPtRpPb];
	//Double_t theory_exlow_tmp[nRapRpPb][nPtRpPb];
	//Double_t theory_exhigh_tmp[nRapRpPb][nPtRpPb];
  Double_t theory_exlow[nRapRpPb][nPtRpPb];
  Double_t theory_exhigh[nRapRpPb][nPtRpPb];
	Double_t theory_eylow_tmp[nRapRpPb][nPtRpPb];
	Double_t theory_eyhigh_tmp[nRapRpPb][nPtRpPb];
  Double_t theory_eylow[nRapRpPb][nPtRpPb];
	Double_t theory_eyhigh[nRapRpPb][nPtRpPb];
  Double_t ptlimit[nRapRpPb] = {4.0, 6.5, 6.5, 6.5, 6.5, 5.0, 4.0};
  
  //// 1) y_CM array (from forward to backward)
  Double_t rapArrNumFB[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  //Double_t rapArrNumBF[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
  //// array string
  TString rapArr[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
    cout << iy <<"th rapArr = " << rapArr[iy] << endl;
  }
  
	///////////////////////////////////////////////////
  //////// experimental points	
  TFile *inFile = new TFile("../DrawFinalPlot/plot_RpPb/RpPb_pt_isPrompt1.root","READ");
  
  TGraphAsymmErrors* g_RpPb_sys[nRapRpPb];
  TGraphAsymmErrors* g_RpPb_sys_line[nRapRpPb];
  TGraphAsymmErrors* g_RpPb[nRapRpPb];
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    g_RpPb_sys[iy] = (TGraphAsymmErrors*)inFile->Get(Form("g_RpPb_sys_%d",iy));
    g_RpPb_sys[iy]->SetName(Form("g_RpPb_sys_%d",iy));
    g_RpPb_sys_line[iy] = (TGraphAsymmErrors*)inFile->Get(Form("g_RpPb_sys_%d",iy));
    g_RpPb_sys_line[iy]->SetName(Form("g_RpPb_sys_line_%d",iy));
    g_RpPb[iy] = (TGraphAsymmErrors*)inFile->Get(Form("g_RpPb_%d",iy));
    g_RpPb[iy]->SetName(Form("g_RpPb_%d",iy));
    g_RpPb[iy]->SetMarkerStyle(kFullCircle);
    g_RpPb[iy]->SetMarkerSize(1.1);
  } 
	
  ///////////////////////////////////////////////////
  //////// theory curves	
  const int nTheory = 3;
//  const int nTheory = 1;
  TFile *inFileTh[nTheory];
  inFileTh[0]= new TFile("./plot_theory/comp_RpPb_pt_isSmoothened0_Vogt.root","READ");
  inFileTh[1]= new TFile("./plot_theory/comp_RpPb_pt_isSmoothened0_Lansberg_EPS09NLO.root","READ");
  inFileTh[2]= new TFile("./plot_theory/comp_RpPb_pt_isSmoothened0_Lansberg_nCTEQ15.root","READ");
  
  TGraphAsymmErrors* g_RpPb_theory[nTheory][nRapRpPb];
  TGraphAsymmErrors* g_RpPb_theory_line[nTheory][nRapRpPb];
  for (int ith = 0 ; ith < nTheory; ith ++ ) {
    for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
      g_RpPb_theory[ith][iy] = (TGraphAsymmErrors*)inFileTh[ith]->Get(Form("g_RpPb_theory_%d",iy));
      g_RpPb_theory[ith][iy]->SetName(Form("g_RpPb_theory_%d_%d",ith,iy));
      g_RpPb_theory_line[ith][iy] = (TGraphAsymmErrors*)inFileTh[ith]->Get(Form("g_RpPb_theory_%d",iy));
      g_RpPb_theory_line[ith][iy]->SetName(Form("g_RpPb_theory_line_%d_%d",ith,iy));
    } 
  } 
  
  //// color
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    g_RpPb_theory[0][iy]->SetFillColorAlpha(kAzure+1,0.5);
    g_RpPb_theory[0][iy]->SetLineColor(kAzure+1);
    g_RpPb_theory[0][iy]->SetFillStyle(1001);
    g_RpPb_theory[1][iy]->SetFillColor(kOrange+2);
    g_RpPb_theory[1][iy]->SetLineColor(kOrange+2);
    g_RpPb_theory[1][iy]->SetFillStyle(3545);
    g_RpPb_theory_line[1][iy]->SetFillColorAlpha(kOrange+2,0.);
    g_RpPb_theory_line[1][iy]->SetLineColor(kOrange+2);
    g_RpPb_theory_line[1][iy]->SetFillStyle(1001);
    g_RpPb_theory[2][iy]->SetFillColor(kGreen+2);
    g_RpPb_theory[2][iy]->SetLineColor(kGreen+2);
    g_RpPb_theory[2][iy]->SetFillStyle(3754);
    g_RpPb_theory_line[2][iy]->SetFillColorAlpha(kGreen+2,0.);
    g_RpPb_theory_line[2][iy]->SetLineColor(kGreen+2);
    g_RpPb_theory_line[2][iy]->SetFillStyle(1001);
  } 
  
  ///////////////////////////////////////////////////
	//// Draw
  ///////////////////////////////////////////////////
	
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.075);

  //// global uncertainty from lumi
  TBox * globbox_pp = new TBox(0.0, 0.96, 1.5, 1.04);
  globbox_pp->SetFillColorAlpha(kGray+2,0.5);
  globbox_pp->SetLineColor(kBlack);
  TBox * globbox_pa = new TBox(1.5, 0.965, 3.0, 1.035);
  globbox_pa->SetFillColorAlpha(kWhite,0.5);
  globbox_pa->SetLineColor(kBlack);
  TBox * globbox_all = new TBox(0.0, 0.9468493, 1.5, 1.053151);
  globbox_all->SetFillColorAlpha(kGray+2,0.5);
  globbox_all->SetLineColor(kBlack);

  //// Draw 
	//TBox* emptybox = new TBox(0.0, 0.0, 0.97, 0.86);
	TBox* emptybox = new TBox(0.0, 0.0, 1.0, 0.86);
	emptybox->SetFillColor(kWhite);
	emptybox->SetFillStyle(4000);
	emptybox->SetLineColor(kBlack);
	emptybox->SetLineWidth(1);
  
  //TLegend *legBL1 = new TLegend(0.04, 0.08, 0.40, 0.5);
  TLegend *legBL1 = new TLegend(0.04, 0.08, 0.40, 0.37);
  //TLegend *legBL1 = new TLegend(0.04, 0.08, 0.40, 0.27);
	SetLegendStyle(legBL1);
	legBL1->SetTextSize(0.07);
	
  TCanvas* c_all = new TCanvas("c_all","c_all",1200,680);
  //CMS_lumi( c_all, isPA, iPos );
  c_all->Divide(5,2);
  const int nPad = 10;
  double xmargin = 0.00;
  double ymargin = 0.00;
  double xpad0 = 0.060;
  double xpadw = 0.233;
  TVirtualPad* pad_all[nPad]; // 2 pads for y axis, 8 pads for actual plots
  pad_all[0] = new TPad("pad_all_0", "",0, 0.506, xpad0, 1.0);
  pad_all[1] = new TPad("pad_all_1", "",xpad0, 0.506, xpad0+xpadw, 1.0);
  pad_all[2] = new TPad("pad_all_2", "",xpad0+xpadw, 0.506, xpad0+2*xpadw, 1.0);
  pad_all[3] = new TPad("pad_all_3", "",xpad0+2*xpadw, 0.506, xpad0+3*xpadw, 1.0);
  pad_all[4] = new TPad("pad_all_4", "",xpad0+3*xpadw, 0.506, xpad0+4*xpadw, 1.0);
  pad_all[5] = new TPad("pad_all_5", "",0, 0.0, xpad0, 0.506);
  pad_all[6] = new TPad("pad_all_6", "",xpad0, 0.0, xpad0+xpadw, 0.506);
  pad_all[7] = new TPad("pad_all_7", "",xpad0+xpadw, 0.0, xpad0+2*xpadw, 0.506);
  pad_all[8] = new TPad("pad_all_8", "",xpad0+2*xpadw, 0.0, xpad0+3*xpadw, 0.506);
  pad_all[9] = new TPad("pad_all_9", "",xpad0+3*xpadw, 0.0, xpad0+4*xpadw, 0.506);
  
  double topmargin = 0.14;
  double bottommargin = 0.161;
  for (Int_t iy = 0; iy < nPad; iy++) { 
    pad_all[iy]->Draw();
    if (iy<=4) { 
      pad_all[iy]->SetTopMargin(topmargin); 
      pad_all[iy]->SetBottomMargin(0.0); 
    }
    else { 
      pad_all[iy]->SetTopMargin(0.0); 
      pad_all[iy]->SetBottomMargin(bottommargin); 
    }
    pad_all[iy]->SetLeftMargin(0.0);
    pad_all[iy]->SetRightMargin(0.0);
  }
  
  for (int iy=0; iy<nRapRpPb; iy++) {
    //// reverse the order for forward
    if (iy==0) pad_all[3]->cd();
    else if (iy==1) pad_all[2]->cd();
    else if (iy==2) pad_all[1]->cd();
    else pad_all[iy+3]->cd();
    if (isPoint) {
//      g_RpPb_sys[iy]->SetFillColorAlpha(kGray,0.5);
//      g_RpPb_sys[iy]->SetLineColor(kGray+3);
      g_RpPb_sys[iy]->SetFillColorAlpha(kRed-10,0.5);
      g_RpPb_sys[iy]->SetLineColor(kPink-6);
      g_RpPb_sys_line[iy]->SetFillColorAlpha(kRed-10,0.);
      g_RpPb_sys_line[iy]->SetLineColor(kPink-6);
/*
      g_RpPb_sys[0]->SetFillColorAlpha(kGreen-10,0.5);
      g_RpPb_sys[1]->SetFillColorAlpha(kRed-10,0.5);
      g_RpPb_sys[2]->SetFillColorAlpha(kBlue-10,0.5);
      g_RpPb_sys[3]->SetFillColorAlpha(kBlue-10,0.5);
      g_RpPb_sys[4]->SetFillColorAlpha(kRed-10,0.5);
      g_RpPb_sys[5]->SetFillColorAlpha(kGreen-10,0.5);
      g_RpPb_sys[6]->SetFillColorAlpha(kMagenta-10,0.5);

      g_RpPb_sys[0]->SetLineColor(kGreen+3);
      g_RpPb_sys[1]->SetLineColor(kPink-6);
      g_RpPb_sys[2]->SetLineColor(kBlue-3);
      g_RpPb_sys[3]->SetLineColor(kBlue-3);
      g_RpPb_sys[4]->SetLineColor(kPink-6);
      g_RpPb_sys[5]->SetLineColor(kGreen+3);
      g_RpPb_sys[6]->SetLineColor(kViolet-6);
*/
    } else {
      g_RpPb_sys[iy]->SetFillColor(kWhite);
      g_RpPb_sys[iy]->SetLineColor(kWhite);
    }
//    g_RpPb[iy]->SetMarkerColor(kGray+3);
//    g_RpPb[iy]->SetLineColor(kGray+3);
    g_RpPb[iy]->SetMarkerColor(kPink-6);
    g_RpPb[iy]->SetLineColor(kPink-6);
    g_RpPb_sys[iy]->Draw("A5");
    globbox_all->Draw("lf");
    for (int ith = 0 ; ith < nTheory; ith ++ ) {
      if (ith==0){ g_RpPb_theory[ith][iy]->Draw("3"); }  
    }
    if (isPoint) { g_RpPb_sys[iy]->Draw("5"); }
    for (int ith = 0 ; ith < nTheory; ith ++ ) {
      if (ith!=0){
        g_RpPb_theory[ith][iy]->Draw("5");
        g_RpPb_theory_line[ith][iy]->Draw("5");
      }
    }
    solidLine(0.,1.,32.,1.,1,1);
    if (isPoint) {
      g_RpPb_sys_line[iy]->Draw("5");
      g_RpPb[iy]->Draw("p");
    }
    //globtex->SetTextAlign(32); //3:right 2:vertical center
	  globtex->SetTextAlign(22);
    globtex->SetTextFont(42);
	  globtex->SetTextSize(0.078);
    //if (iy<3) globtex->DrawLatex(0.9, 0.09, rapArr[iy].Data());
    //else globtex->DrawLatex(0.9, 0.25, rapArr[iy].Data());
    if (iy<3) globtex->DrawLatex(0.53, 0.2, rapArr[iy].Data());
    else globtex->DrawLatex(0.53, 0.35, rapArr[iy].Data());
  }
  pad_all[4]->cd();
  emptybox->Draw("l");
  
  globtex->SetTextSize(0.09);
  globtex->SetTextFont(42); 
  globtex->SetTextAlign(32); //1:left, 2:vertical center
  if (isPrompt) globtex->DrawLatex(0.90, 0.55, "Prompt J/#psi");
  else globtex->DrawLatex(0.90, 0.55, "Nonprompt J/#psi");
  globtex->SetTextAlign(12); //1:left 2:vertical center
  globtex->SetTextSize(0.07);
  globtex->SetTextFont(42);  

  TLegendEntry *ent1_thr=legBL1->AddEntry("ent1_thr"," EPS09 NLO (Vogt)","f");
	ent1_thr->SetFillColorAlpha(kAzure+1,0.5);
	ent1_thr->SetLineColor(kAzure+5);
  ent1_thr->SetFillStyle(1001);
  TLegendEntry *ent2_thr=legBL1->AddEntry("ent2_thr"," EPS09 NLO (Lansberg-Shao)","f");
	ent2_thr->SetFillColor(kOrange+1);
	ent2_thr->SetLineColor(kOrange+1);
  ent2_thr->SetFillStyle(3545);
  TLegendEntry *ent3_thr=legBL1->AddEntry("ent3_thr"," nCTEQ15 NLO","f");
	ent3_thr->SetFillColor(kGreen+2);
	ent3_thr->SetLineColor(kGreen+2);
  ent3_thr->SetFillStyle(3754);
  TLegendEntry *ent4_thr=legBL1->AddEntry("ent4_thr"," (Lansberg-Shao)","f");
	ent4_thr->SetFillColor(kWhite);
	ent4_thr->SetLineColor(kWhite);
  ent4_thr->SetFillStyle(1001);
	legBL1->Draw();

  //////////////////// y axis
      //g_RpPb_sys[iy]->GetYaxis()->SetTitle("R_{pPb}");
      //g_RpPb_sys[iy]->GetYaxis()->SetTitleOffset(1.1);
      //g_RpPb_sys[iy]->GetYaxis()->SetTitleSize(0.075);
      //g_RpPb_sys[iy]->GetYaxis()->SetLabelSize(0.055);
  //// 1) Forward 
  cout << pad_all[1]->GetUxmin() << endl;
  cout << pad_all[1]->GetUxmax() << endl;
  pad_all[0]->cd();
  //cout << g_RpPb_sys[2]->GetYaxis()->GetXmin() << endl;
  TGaxis *yaxis01 = new TGaxis(1.0,0.0,1.0,1.0-topmargin,0.0,1.6,510,"");
  yaxis01->SetTitleFont(42);
  yaxis01->SetLabelFont(42);
  yaxis01->SetTitle("R_{pPb}");
  yaxis01->SetTitleOffset(1.15);
  yaxis01->SetLabelOffset(0.05);
  yaxis01->CenterTitle(1);
  yaxis01->SetTitleSize(0.075*(0.29/0.07));
  yaxis01->SetLabelSize(0.055*(0.29/0.07));
  yaxis01->Draw();  
  
  TGaxis *yaxis02 = new TGaxis(1.0,0.0+bottommargin,1.0,1.0,0.0,1.6,510,"");
  yaxis02->SetTitleFont(42);
  yaxis02->SetLabelFont(42);
  yaxis02->SetTitle("R_{pPb}");
  yaxis02->SetTitleOffset(1.15);
  yaxis02->SetLabelOffset(0.05);
  yaxis02->CenterTitle(1);
  yaxis02->SetTitleSize(0.075*(0.29/0.07));
  yaxis02->SetLabelSize(0.055*(0.29/0.07));
  pad_all[5]->cd();
  yaxis02->Draw();  

  c_all->cd();
//  double tmpx = -0.004;
//  double tmpy = -0.016;
//  globtex->SetTextColor(kRed);
//	globtex->SetTextSize(0.02825);
//  globtex->DrawLatex(xpad0+xpadw+tmpx, 0.0+(bottommargin/2.)+tmpy, "0");
//  globtex->DrawLatex(xpad0+2*xpadw+tmpx, 0.0+(bottommargin/2.)+tmpy, "0");
//  globtex->DrawLatex(xpad0+3*xpadw+tmpx, 0.0+(bottommargin/2.)+tmpy, "0");
////	globtex->SetTextSize(0.1);
////  globtex->DrawLatex(0.5, 0.5, "XIA");

  //// re-draw 0 for x axis 
  double tmpx = 0.005;
  double tmpy = 0.0415;
  TPaveText *pave00 = new TPaveText(xpad0+0*xpadw-tmpx,0.0+(bottommargin/2.)-tmpy,xpad0+0*xpadw+tmpx,0.0+(bottommargin/2.)-0.0115);
  //pave00->SetTextColor(kRed);
  pave00->SetTextFont(42);
  pave00->SetTextAlign(31); //3:right 1:top
  pave00->SetFillColor(kWhite);
  pave00->SetLineColor(kWhite);
  pave00->SetShadowColor(kWhite);
  pave00->AddText("0");
  pave00->Draw();
  TPaveText *pave01 = new TPaveText(xpad0+1*xpadw-tmpx,0.0+(bottommargin/2.)-tmpy,xpad0+1*xpadw+tmpx,0.0+(bottommargin/2.)-0.0115);
  //pave01->SetTextColor(kRed);
  pave01->SetTextFont(42);
  pave01->SetTextAlign(31); //3:right 1:top
  pave01->SetFillColor(kWhite);
  pave01->SetLineColor(kWhite);
  pave01->SetShadowColor(kWhite);
  pave01->AddText("0");
  pave01->Draw();
  TPaveText *pave02 = new TPaveText(xpad0+2*xpadw-tmpx,0.0+(bottommargin/2.)-tmpy,xpad0+2*xpadw+tmpx,0.0+(bottommargin/2.)-0.0115);
  //pave02->SetTextColor(kRed);
  pave02->SetTextFont(42);
  pave02->SetTextAlign(31); //3:right 1:top
  pave02->SetFillColor(kWhite);
  pave02->SetLineColor(kWhite);
  pave02->SetShadowColor(kWhite);
  pave02->AddText("0");
  pave02->Draw();
  TPaveText *pave03 = new TPaveText(xpad0+3*xpadw-tmpx,0.0+(bottommargin/2.)-tmpy,xpad0+3*xpadw+tmpx,0.0+(bottommargin/2.)-0.0115);
  //pave03->SetTextColor(kRed);
  pave03->SetTextFont(42);
  pave03->SetTextAlign(31); //3:right 1:top
  pave03->SetFillColor(kWhite);
  pave03->SetLineColor(kWhite);
  pave03->SetShadowColor(kWhite);
  pave03->AddText("0");
  pave03->Draw();
 
  //// re-draw 0 for y axis 
  TPaveText *pave_Y_01 = new TPaveText(xpad0-0.02, 0.506-0.015, xpad0-0.002, 0.506+0.015,"BL");
  pave_Y_01->SetFillColor(kWhite);
  pave_Y_01->SetLineColor(kWhite);
  pave_Y_01->SetShadowColor(kWhite);
  pave_Y_01->Draw();
  TPaveText *pave_Y_02 = new TPaveText(xpad0-0.01, 0.506-0.015, xpad0-0.002, 0.506+0.015,"BL");
  //pave_Y_02->SetTextColor(kRed);
  pave_Y_02->SetTextFont(42);
  pave_Y_02->SetTextAlign(32); //3:right 2:vertical center
  pave_Y_02->SetFillColor(kWhite);
  pave_Y_02->SetLineColor(kWhite);
  pave_Y_02->SetShadowColor(kWhite);
  pave_Y_02->AddText("0");
  pave_Y_02->Draw();
/*  
  globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.09/2.5);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.08, 0.885, "Prompt J/#psi");
	else globtex->DrawLatex(0.08, 0.885, "Non-prompt J/#psi");
  globtex->SetTextAlign(12); //1:left 2:vertical center
	globtex->SetTextSize(0.07/2.5);
	globtex->SetTextFont(42);
  globtex->DrawLatex(0.08, 0.84, "Global uncertainty : 5.3 \%");
*/
  CMS_lumi( c_all, isPA, iPos );
    
  c_all->SaveAs(Form("plot_theory/comp_RpPb_pt_isSmoothened%d_Overlay.pdf",(int)isSmoothened));
  c_all->SaveAs(Form("plot_theory/comp_RpPb_pt_isSmoothened%d_Overlay.png",(int)isSmoothened));
 
  ///////////////////////////////////////////////////////////////////
  // save as a root file
//  TFile* outFile = new TFile(Form("plot_theory/comp_RpPb_pt_isSmoothened%d_Overlay.root",(int)isSmoothened),"RECREATE");
//  outFile->cd();
//  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
//    g_RpPb_theory[iy]->Write();
//  } 
   
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

void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( binmin == 1.93 || binmin == -1.93 || binmin == -2.87) {
    *arr = Form("%.2f < |y_{CM}| < %.1f", binmin, binmax);
  }
  else if ( binmax == 1.93 || binmax ==  -1.93 || binmax == -2.87 ) {
    *arr = Form("%.1f < |y_{CM}| < %.2f", binmin, binmax);
  }
  else if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.1f", binmin, binmax);
  } else {
    *arr = Form("%.1f < |y_{CM}| < %.1f", binmin, binmax);
  }
}

void formPtArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < p_{T} < %.0f [GeV/c]", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < p_{T} < %.0f [GeV/c]", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < p_{T} < %.1f [GeV/c]", binmin, binmax);
  } else {
    *arr = Form("%.1f < p_{T} < %.1f [GeV/c]", binmin, binmax);
  }
}

void CMS_lumi( TPad* pad, int iPeriod, int iPosX )
{            
  bool outOfFrame    = false;
  if( iPosX/10==0 ) { outOfFrame = true; }
  int alignY_=3;
  int alignX_=2;
  if( iPosX/10==0 ) alignX_=1;
  if( iPosX==0    ) alignX_=1;
  if( iPosX==0    ) alignY_=1;
  if( iPosX/10==1 ) alignX_=1;
  if( iPosX/10==2 ) alignX_=2;
  if( iPosX/10==3 ) alignX_=3;
  //if( iPosX == 0  ) relPosX = 0.12;
  if( iPosX == 0  ) relPosX = 0.15; // KYO
  int align_ = 10*alignX_ + alignY_;

  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  //  float e = 0.025;

  pad->cd();

  TString lumiText;
  if( iPeriod==0 ) {
    lumiText += lumi_pp502TeV;
    lumiText += " (pp 5.02 TeV)";
  } else if( iPeriod==1 || iPeriod==2 || iPeriod==3){
    lumiText += lumi_pPb502TeV;
    lumiText += " (pPb 5.02 TeV)";
  } else if( iPeriod==10){
    lumiText += "pPb ";
    lumiText += lumi_pPb502TeV;
    lumiText += ", pp ";
    lumiText += lumi_pp502TeV;
    lumiText += " (5.02 TeV)";
  } else {
    lumiText += "LumiText Not Selected";
  }
   
  cout << lumiText << endl;

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);    

  //float extraTextSize = extraOverCmsTextSize*cmsTextSize;
  float extraTextSize = extraOverCmsTextSize*cmsTextSize*1.1;

  latex.SetTextFont(42);
  latex.SetTextAlign(31); 
  latex.SetTextSize(lumiTextSize*t);    
  //latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);
  //latex.DrawLatex(1-r+0.01,1-t+lumiTextOffset*t+0.01,lumiText);//KYO
  latex.DrawLatex(1-r+0.02,1-t+lumiTextOffset*t+0.01,lumiText);//KYO
  if( outOfFrame ) {
    latex.SetTextFont(cmsTextFont);
    latex.SetTextAlign(11); 
    latex.SetTextSize(cmsTextSize*t);    
    latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
  }
  
  pad->cd();

  float posX_=0;
  if( iPosX%10<=1 ) { posX_ =   l + relPosX*(1-l-r); }
  else if( iPosX%10==2 ) { posX_ =  l + 0.5*(1-l-r); }
  else if( iPosX%10==3 ) { posX_ =  1-r - relPosX*(1-l-r); }
  float posY_ = 1-t - relPosY*(1-t-b);
  if( !outOfFrame ) {
    if( drawLogo ) {
      posX_ =   l + 0.045*(1-l-r)*W/H;
      posY_ = 1-t - 0.045*(1-t-b);
      float xl_0 = posX_;
      float yl_0 = posY_ - 0.15;
      float xl_1 = posX_ + 0.15*H/W;
      float yl_1 = posY_;
      //TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
      TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
      pad_logo->Draw();
      pad_logo->cd();
      //CMS_logo->Draw("X");
      pad_logo->Modified();
      pad->cd();
    } else {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextSize(cmsTextSize*t);
      latex.SetTextAlign(align_);
      //cout << "posX_ = " << posX_ << ", posY_ = " << posY_ << endl;
      //if (iPosX==33) {posX_ -= 0.03; posY_-=0.03; } // KYO
      //if (iPosX==33) {posX_ += 0.03; posY_-=0.01; } // KYO
      if (iPosX==33) {
        posX_ += 0.03; posY_-=0.01; 
        latex.SetTextSize(cmsTextSize*t*1.3);
      } // KYO
      latex.DrawLatex(posX_, posY_, cmsText);
      if( writeExtraText ) {
        latex.SetTextFont(extraTextFont);
        latex.SetTextAlign(align_);
        latex.SetTextSize(extraTextSize*t);
        latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
      }
    }
  } else if( writeExtraText ) {
    if( iPosX==0) {
      posX_ =   l +  relPosX*(1-l-r);
      posY_ =   1-t+lumiTextOffset*t;
    }
    latex.SetTextFont(extraTextFont);
    latex.SetTextSize(extraTextSize*t);
    latex.SetTextAlign(align_);
    latex.DrawLatex(posX_, posY_, extraText);      
  }
  return;
}

