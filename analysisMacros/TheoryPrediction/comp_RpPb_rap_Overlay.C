#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
//void formPtArr(Double_t binmin, Double_t binmax, TString* arr);
void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RpPb_rap_Overlay(bool isPoint=true, bool isSmoothened=false)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 10;  // 0:pp, 1:pPb, 10 : pp & pPb together for RpPb plot
	int iPos=33;

  bool isPrompt=true;
	
  ///////////////////////////////////////////////////
  const int nRap = 8; 
  const int nRapTmp = nRap + 1;
	const int nRapRpPb = 7;
	const int nRapRpPbTheory = 6; //Ramona..
	//const int nPtRpPb = 49;
	const int nPtRpPb = 2;
  Double_t theory_px[nPtRpPb][nRapRpPbTheory]; 
	Double_t theory_py[nPtRpPb][nRapRpPbTheory];
	//Double_t theory_exlow_tmp[nPtRpPb][nRapRpPbTheory];
	//Double_t theory_exhigh_tmp[nPtRpPb][nRapRpPbTheory];
  Double_t theory_exlow[nPtRpPb][nRapRpPbTheory];
  Double_t theory_exhigh[nPtRpPb][nRapRpPbTheory];
	Double_t theory_eylow_tmp[nPtRpPb][nRapRpPbTheory];
	Double_t theory_eyhigh_tmp[nPtRpPb][nRapRpPbTheory];
  Double_t theory_eylow[nPtRpPb][nRapRpPbTheory];
	Double_t theory_eyhigh[nPtRpPb][nRapRpPbTheory];
  
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
  TFile *inFile = new TFile("../DrawFinalPlot/plot_RpPb/RpPb_rap_isPrompt1.root");
  
  TGraphAsymmErrors* g_RpPb_sys[nRapRpPb];
  TGraphAsymmErrors* g_RpPb_sys_line[nRapRpPb];
  TGraphAsymmErrors* g_RpPb[nRapRpPb];
  g_RpPb_sys[0] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_lowpt");
  g_RpPb_sys_line[0] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_lowpt_line");
  g_RpPb[0] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_lowpt");
  g_RpPb_sys[1] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_highpt");
  g_RpPb_sys_line[1] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_highpt_line");
  g_RpPb[1] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_highpt");
  g_RpPb_sys[0]->SetName("g_RpPb_sys_lowpt");
  g_RpPb_sys_line[0]->SetName("g_RpPb_sys_lowpt_line");
  g_RpPb_sys[1]->SetName("g_RpPb_sys_highpt");
  g_RpPb_sys_line[1]->SetName("g_RpPb_sys_highpt_line");
  //g_RpPb_sys[0]->SetFillColorAlpha(kRed-10,0.5);
  //g_RpPb_sys_line[0]->SetFillColorAlpha(kRed-10,0.);
  //g_RpPb_sys_line[0]->SetLineColor(kPink-6);
  //g_RpPb_sys[1]->SetFillColorAlpha(kGreen-10,0.5);
  //g_RpPb_sys_line[1]->SetFillColorAlpha(kGreen-10,0.);
  //g_RpPb_sys_line[1]->SetLineColor(kGreen+3);
  for (int ipt = 0 ; ipt < nPtRpPb; ipt ++ ) {
    if (isPoint) {
      g_RpPb_sys[ipt]->SetFillColorAlpha(kGray,0.5);
      g_RpPb_sys[ipt]->SetLineColor(kGray+3);
    } else {
      g_RpPb_sys[ipt]->SetFillColor(kWhite);
      g_RpPb_sys[ipt]->SetLineColor(kWhite);
    }
    g_RpPb[ipt]->SetMarkerColor(kGray+3);
    g_RpPb[ipt]->SetLineColor(kGray+3);
    g_RpPb[ipt]->SetMarkerStyle(kFullCircle);
    g_RpPb[ipt]->SetMarkerSize(1.4);
  }
  
  ///////////////////////////////////////////////////
  //////// theory curves	
  const int nTheory = 4;
//  const int nTheory = 1;
  TFile *inFileTh[nTheory];
  inFileTh[0]= new TFile("./plot_theory/comp_RpPb_rap_isSmoothened0_Vogt.root","READ");
  inFileTh[1]= new TFile("./plot_theory/comp_RpPb_rap_isSmoothened0_Lansberg_EPS09LO.root","READ");
  inFileTh[2]= new TFile("./plot_theory/comp_RpPb_rap_isSmoothened0_Lansberg_EPS09NLO.root","READ");
  inFileTh[3]= new TFile("./plot_theory/comp_RpPb_rap_isSmoothened0_Lansberg_nCTEQ15.root","READ");
  
  TGraphAsymmErrors* g_RpPb_theory[nTheory][nPtRpPb];
  for (int ith = 0 ; ith < nTheory; ith ++ ) {
    for (int ipt = 0 ; ipt < nPtRpPb; ipt ++ ) {
      g_RpPb_theory[ith][ipt] = (TGraphAsymmErrors*)inFileTh[ith]->Get(Form("g_RpPb_theory_%d",ipt));
      g_RpPb_theory[ith][ipt]->SetName(Form("g_RpPb_theory_%d_%d",ith,ipt));
    } 
  } 

  //// color
  for (int ipt = 0 ; ipt < nPtRpPb; ipt ++ ) {
    //g_RpPb_theory[0][ipt]->SetFillColorAlpha(kYellow,0.5);
    g_RpPb_theory[0][ipt]->SetFillColorAlpha(kYellow,1.);
    g_RpPb_theory[0][ipt]->SetLineColor(kYellow);
    g_RpPb_theory[0][ipt]->SetFillStyle(1001);
    g_RpPb_theory[1][ipt]->SetFillColor(kRed+1);
    g_RpPb_theory[1][ipt]->SetLineColor(kRed+1);
    //g_RpPb_theory[1][ipt]->SetFillStyle(3004);
    //g_RpPb_theory[1][ipt]->SetFillStyle(3254);
    g_RpPb_theory[1][ipt]->SetFillStyle(3454);
    g_RpPb_theory[2][ipt]->SetFillColor(kGreen+1);
    g_RpPb_theory[2][ipt]->SetLineColor(kGreen+1);
    //g_RpPb_theory[2][ipt]->SetFillStyle(3005);
    //g_RpPb_theory[2][ipt]->SetFillStyle(3245);
    g_RpPb_theory[2][ipt]->SetFillStyle(3445);
    g_RpPb_theory[3][ipt]->SetFillColor(kBlue+1);
    g_RpPb_theory[3][ipt]->SetLineColor(kBlue+1);
    g_RpPb_theory[3][ipt]->SetFillStyle(3003);
    //g_RpPb_theory[3][ipt]->SetFillStyle(3400);
  } 
	///////////////////////////////////////////////////
	//// Draw
  ///////////////////////////////////////////////////
	
	TLegend *legBR = new TLegend(0.45, 0.18, 0.70, 0.32);
	SetLegendStyle(legBR);
	legBR->SetTextSize(0.05);
	TLegend *legBR2 = new TLegend(0.45, 0.18, 0.70, 0.32);
	SetLegendStyle(legBR2);
	legBR2->SetTextSize(0.05);
  
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.075);

  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1->cd();
  
  //g_RpPb_sys[0]->Draw("A2"); 
  g_RpPb_sys[0]->Draw("A5"); 
  for (int ith = 0 ; ith < nTheory; ith ++ ) {
    if (ith==0){
      g_RpPb_theory[ith][0]->Draw("3");
    } else {
      if (isSmoothened) {
        g_RpPb_theory[ith][0]->Draw("3");
      } else {
        g_RpPb_theory[ith][0]->Draw("5");
      }
    }
  }
  if (isPoint) {
    g_RpPb_sys[0]->Draw("5"); 
    g_RpPb[0]->Draw("P"); 
  } 
  dashedLine(-2.5,1.,2.1,1.,1,1);

	TLegendEntry *le1=legBR->AddEntry("le1","6.5 < p_{T} < 10 GeV/c","lpf");
  le1->SetFillColor(kRed-10);
  le1->SetFillStyle(1001);
  le1->SetLineColor(kPink-6);
  le1->SetLineWidth(1);
  le1->SetMarkerStyle(kFullSquare);
  le1->SetMarkerColor(kPink-6);
  le1->SetMarkerSize(1.9);
	legBR->Draw();
	
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
	globtex->SetTextSize(0.035);
	globtex->SetTextFont(42);
  globtex->DrawLatex(0.21, 0.78, "Global uncertainty : 5.3 \%");
	
  CMS_lumi( c1, isPA, iPos );
    
  c1->SaveAs(Form("plot_theory/comp_RpPb_rap_pt1_isSmoothened%d_Overlay.pdf",(int)isSmoothened));
  c1->SaveAs(Form("plot_theory/comp_RpPb_rap_pt1_isSmoothened%d_Overlay.png",(int)isSmoothened));
 
 
  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  c2->cd();
  
  g_RpPb_sys[1]->Draw("A5"); 
  //g_RpPb_sys_line[1]->Draw("5"); 
  for (int ith = 0 ; ith < nTheory; ith ++ ) {
    if (ith==0){
      g_RpPb_theory[ith][1]->Draw("3");
    } else {
      if (isSmoothened) {
        g_RpPb_theory[ith][1]->Draw("3");
      } else {
        g_RpPb_theory[ith][1]->Draw("5");
      }
    }
  }
  if (isPoint) {
    g_RpPb_sys[1]->Draw("5"); 
    g_RpPb[1]->Draw("P"); 
  }  
  dashedLine(-2.5,1.,2.1,1.,1,1);

	TLegendEntry *le2=legBR2->AddEntry("le2","10 < p_{T} < 30 GeV/c","lpf");
  le2->SetFillColor(kGreen-10);
  le2->SetFillStyle(1001);
  le2->SetLineColor(kGreen+3);
  le2->SetMarkerStyle(kFullDiamond);
  le2->SetMarkerColor(kGreen+3);
  le2->SetMarkerSize(3.1);
	legBR2->Draw();
	
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
	globtex->SetTextSize(0.035);
	globtex->SetTextFont(42);
  globtex->DrawLatex(0.21, 0.78, "Global uncertainty : 5.3 \%");
	
  CMS_lumi( c2, isPA, iPos );
    
  c2->SaveAs(Form("plot_theory/comp_RpPb_rap_pt2_isSmoothened%d_Overlay.pdf",(int)isSmoothened));
  c2->SaveAs(Form("plot_theory/comp_RpPb_rap_pt2_isSmoothened%d_Overlay.png",(int)isSmoothened));
 
 
 
  
  ///////////////////////////////////////////////////////////////////
  // save as a root file
//  TFile* outFile = new TFile(Form("plot_theory/comp_RpPb_rap_isSmoothened%d_Overlay.root",(int)isSmoothened),"RECREATE");
//  outFile->cd();
//  for (int ipt = 0 ; ipt < nPtRpPb; ipt ++ ) {
//    g_RpPb_theory[ipt]->Write();
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
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);
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
      if (iPosX==33) {posX_ -= 0.03; posY_-=0.03; } // KYO
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
