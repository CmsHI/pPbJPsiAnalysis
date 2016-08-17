#include "CMS_lumi.h"
#include "../SONGKYO.h"

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RFB_pt_Vogt(double ptmax=32, bool isLine=true, bool isSmoothened=false)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 1;  // 0:pp, 1:pPb
	int iPos=0;

  bool isPrompt=true;
	
  ///////////////////////////////////////////////////
	///////// from Ramona
	const int nRapRFB = 3;
	//const int nPtRFB = 3;
	const int nPtRFB = 5;
	//// *** NOTE ***
  //// *** low_pT(5 or 6.5) and high_pT(20) points for bin edge are added artificially for isSmoothened
  //// *** Also theory values are copied from 0.45 to 0.0, and from 1.71 to 1.93.
	Double_t theory_px[nRapRFB][nPtRFB] = {
    {5., 5.740347, 7.90416, 13.21239, 30.}, //1.5-1.93
    {6.5, 6.5, 7.95946, 13.31027, 30.}, //0.9-1.5 
    {6.5, 6.5, 8.25131, 13.61970, 30.} //0.0-0.9
  };
	Double_t theory_py[nRapRFB][nPtRFB] = {
    {9.4911E-01, 9.4911E-01, 9.2275E-01, 9.0718E-01, 9.0718E-01},
    {9.5308E-01, 9.5308E-01, 9.5308E-01, 9.4434E-01, 9.4434E-01},
    {9.8764E-01, 9.8764E-01, 9.8764E-01, 9.8128E-01, 9.8128E-01}
  };
	Double_t theory_exlow_tmp[nRapRFB][nPtRFB] = {
    {5., 5., 6.5, 10., 30.},
    {6.5, 6.5, 6.5, 10, 30.},
    {6.5, 6.5, 6.5, 10., 30.}
	};
  Double_t theory_exhigh_tmp[nRapRFB][nPtRFB] = {
    {5., 6.5, 10., 30., 30.},
    {6.5, 6.5, 10, 30., 30.},
    {6.5, 6.5, 10., 30., 30.}
  };
	Double_t theory_exlow[nRapRFB][nPtRFB];
	Double_t theory_exhigh[nRapRFB][nPtRFB];
	Double_t theory_eylow_tmp[nRapRFB][nPtRFB] = {
    {8.9151E-01, 8.9151E-01, 8.7758E-01, 8.6649E-01, 8.6649E-01},
    {9.2059E-01, 9.2059E-01, 9.2059E-01, 9.1307E-01, 9.1307E-01},
    {9.6577E-01, 9.6577E-01, 9.6577E-01, 9.6745E-01, 9.6745E-01}
  };
	Double_t theory_eyhigh_tmp[nRapRFB][nPtRFB] = {
    {9.8209E-01, 9.8209E-01, 9.8312E-01, 9.6590E-01, 9.6590E-01},
    {1.0079E+00, 1.0079E+00, 1.0079E+00, 9.7488E-01, 9.7488E-01}, 
    {9.8810E-01, 9.8810E-01, 9.8810E-01, 9.9042E-01, 9.9042E-01}
	};
  Double_t theory_eylow[nRapRFB][nPtRFB];
	Double_t theory_eyhigh[nRapRFB][nPtRFB];
	
  for (int ir = 0 ; ir < nRapRFB; ir ++ ) {
    for (int ipt = 0 ; ipt < nPtRFB; ipt ++ ) {
		  theory_exlow[ir][ipt] = fabs(theory_px[ir][ipt] - theory_exlow_tmp[ir][ipt]);
		  theory_exhigh[ir][ipt] = fabs(theory_px[ir][ipt] - theory_exhigh_tmp[ir][ipt]);
		  theory_eylow[ir][ipt] = fabs(theory_py[ir][ipt] - theory_eylow_tmp[ir][ipt]);
		  theory_eyhigh[ir][ipt] = fabs(theory_py[ir][ipt] - theory_eyhigh_tmp[ir][ipt]);
  	}
  }
	
  TGraphAsymmErrors* gRFB_theory[nRapRFB];
  for (int ir = 0 ; ir < nRapRFB; ir ++ ) {
    gRFB_theory[ir]= new TGraphAsymmErrors(nPtRFB, theory_px[ir], theory_py[ir], theory_exlow[ir], theory_exhigh[ir], theory_eylow[ir], theory_eyhigh[ir]);	
  }
	
  for (int iy=0; iy<nRapRFB; iy++) {
	  gRFB_theory[iy]->GetXaxis()->SetLimits(0.,ptmax);
	  gRFB_theory[iy]->SetMinimum(0.0);
	  gRFB_theory[iy]->SetMaximum(1.8);
    gRFB_theory[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    gRFB_theory[iy]->GetXaxis()->CenterTitle();
    gRFB_theory[iy]->GetYaxis()->SetTitle("R_{FB}");
    gRFB_theory[iy]->GetYaxis()->CenterTitle();
    gRFB_theory[iy]->SetFillColorAlpha(kYellow,0.5);
	  gRFB_theory[iy]->SetLineColor(kOrange+7);
	  //gRFB_theory[iy]->SetFillStyle(3004);
    if(!isLine) gRFB_theory[iy]->SetLineWidth(0);
  }
  
  //if (isSmoothened) gRFB_theory[0]->SetFillStyle(3245);
  //else gRFB_theory[0]->SetFillStyle(3004);
  //if (isSmoothened) gRFB_theory[1]->SetFillStyle(3254);
  //else gRFB_theory[1]->SetFillStyle(3004);
  //gRFB_theory[2]->SetFillStyle(3002);
	
	///////////////////////////////////////////////////
  //////// experimental points	
  TFile *inFile = new TFile("../DrawFinalPlot/plot_RFB/RFB_pt_isPrompt1.root");
  
  TGraphAsymmErrors* gRFB_sys[nRapRFB];
  TGraphAsymmErrors* gRFB[nRapRFB];
  for (int ir = 0 ; ir < nRapRFB; ir ++ ) {
    gRFB_sys[ir] = (TGraphAsymmErrors*)inFile->Get(Form("gRFB_sys_%d",ir));
    gRFB[ir] = (TGraphAsymmErrors*)inFile->Get(Form("gRFB_%d",ir));
  } 
  
  gRFB_sys[0]->SetFillColor(kGreen-10);
  gRFB_sys[1]->SetFillColor(kRed-10);
  gRFB_sys[2]->SetFillColor(kBlue-10);
  gRFB_sys[0]->SetLineColor(kGreen+3);
  gRFB_sys[1]->SetLineColor(kPink-6);
  gRFB_sys[2]->SetLineColor(kBlue-3);

  SetGraphStyleFinal(gRFB[0], 0, 5);
  SetGraphStyleFinal(gRFB[1], 1, 3);
  SetGraphStyleFinal(gRFB[2], 2, 0);
  gRFB[0]->SetMarkerSize(2.6);
  gRFB[1]->SetMarkerSize(1.4);
  gRFB[2]->SetMarkerSize(1.4);
  
  ///////////////////////////////////////////////////////////////////////
	
  //TLegend *legBL = new TLegend(0.50, 0.18, 0.70, 0.39);
  TLegend *legBL1 = new TLegend(0.20, 0.18, 0.40, 0.34);
  TLegend *legBL2 = new TLegend(0.20, 0.18, 0.40, 0.34);
  TLegend *legBL3 = new TLegend(0.20, 0.18, 0.40, 0.34);
	SetLegendStyle(legBL1);
	SetLegendStyle(legBL2);
	SetLegendStyle(legBL3);
	legBL1->SetTextSize(0.05);
	legBL2->SetTextSize(0.05);
	legBL3->SetTextSize(0.05);
	
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);
  
  ///////////////// CANVAS 1  
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1->cd(); 
  gRFB_sys[0]->Draw("A5");
	if (isSmoothened) gRFB_theory[0]->Draw("3");
  else gRFB_theory[0]->Draw("5");
  gRFB[0]->Draw("P");
	dashedLine(0.,1.,32.,1.,1,1);
	
  //// Legend for experiments	
  TLegendEntry *le3=legBL1->AddEntry("le3","1.5 < |y_{CM}| < 1.93","lpf");
	le3->SetFillColor(kGreen-10);
	le3->SetFillStyle(1001);
	le3->SetLineColor(kGreen+3);
	le3->SetMarkerStyle(kFullDiamond);
	le3->SetMarkerColor(kGreen+3);
	le3->SetMarkerSize(3.3);
  TLegendEntry *ent3_thr=legBL1->AddEntry("ent3_thr","EPS09 NLO + CEM (Vogt)","f");
	ent3_thr->SetFillColor(kYellow);
  ent3_thr->SetFillStyle(3001);
//  ent3_thr->SetFillStyle(3004);
  if(!isLine) ent3_thr->SetLineWidth(0);
  ent3_thr->SetLineColor(kOrange+7);
	legBL1->Draw();
  
  //globtex->SetTextAlign(32); //3:right 2:vertical center
	//globtex->SetTextFont(42);
  //globtex->SetTextSize(0.05);
  //globtex->DrawLatex(0.9, 0.23, "1.5 < |y_{CM}| < 1.93");
	
	globtex->SetTextAlign(12); //1:left, 2:vertical center
	globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");

	CMS_lumi( c1, isPA, iPos );
	c1->Update();
  if (isSmoothened) {
    c1->SaveAs("plot_theory/comp_RFB_pt_Vogt_smoothened_rap1.pdf");
    c1->SaveAs("plot_theory/comp_RFB_pt_Vogt_smoothened_rap1.png");
  } else {
    c1->SaveAs("plot_theory/comp_RFB_pt_Vogt_rap1.pdf");
    c1->SaveAs("plot_theory/comp_RFB_pt_Vogt_rap1.png");
  }
  
  ///////////////// CANVAS 2	
  
  TCanvas* c2 = new TCanvas("c2","c2",600,600);
	c2->cd();
	gRFB_sys[1]->Draw("A5");
	if (isSmoothened) gRFB_theory[1]->Draw("3");
	else gRFB_theory[1]->Draw("5");
	gRFB[1]->Draw("P");
	dashedLine(0.,1.,32.,1.,1,1);
  
  //// Legend for experiments	
  TLegendEntry *le2=legBL2->AddEntry("le2","0.9 < |y_{CM}| < 1.5","lpf");
	le2->SetFillColor(kRed-10);
	le2->SetFillStyle(1001);
	le2->SetLineColor(kPink-6);
	le2->SetMarkerStyle(kFullSquare);
	le2->SetMarkerColor(kPink-6);
	le2->SetMarkerSize(2.1);
  TLegendEntry *ent2_thr=legBL2->AddEntry("ent2_thr","EPS09 NLO + CEM (Vogt)","f");
	ent2_thr->SetFillColor(kYellow);
  ent2_thr->SetFillStyle(3001);
//  ent2_thr->SetFillStyle(3004);
  if(!isLine) ent2_thr->SetLineWidth(0);
  ent2_thr->SetLineColor(kOrange+7);
	legBL2->Draw();
  
  //globtex->SetTextAlign(32); //3:right 2:vertical center
	//globtex->SetTextFont(42);
  //globtex->SetTextSize(0.05);
  //globtex->DrawLatex(0.9, 0.23, "0.9 < |y_{CM}| < 1.5");
  
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
	
  CMS_lumi( c2, isPA, iPos );
	c2->Update();
  
  if (isSmoothened) {
    c2->SaveAs("plot_theory/comp_RFB_pt_Vogt_smoothened_rap2.pdf");
    c2->SaveAs("plot_theory/comp_RFB_pt_Vogt_smoothened_rap2.png");
  } else {
    c2->SaveAs("plot_theory/comp_RFB_pt_Vogt_rap2.pdf");
    c2->SaveAs("plot_theory/comp_RFB_pt_Vogt_rap2.png");
  }
	
  ///////////////// CANVAS 3	
  
  TCanvas* c3 = new TCanvas("c3","c3",600,600);
	c3->cd();
	gRFB_sys[2]->Draw("A5");
	if (isSmoothened) gRFB_theory[2]->Draw("3");
	else gRFB_theory[2]->Draw("5");
	gRFB[2]->Draw("P");
	dashedLine(0.,1.,32.,1.,1,1);
  
  //// Legend for experiments	
  TLegendEntry *le1=legBL3->AddEntry("le1","0 < |y_{CM}| < 0.9","lpf");
	le1->SetFillColor(kBlue-10);
	le1->SetFillStyle(1001);
	le1->SetLineColor(kBlue-3);
	le1->SetMarkerStyle(kFullCircle);
	le1->SetMarkerColor(kBlue-3);
	le1->SetMarkerSize(2.1);
  TLegendEntry *ent1_thr=legBL3->AddEntry("ent1_thr","EPS09 NLO + CEM (Vogt)","f");
	ent1_thr->SetFillColor(kYellow);
  ent1_thr->SetFillStyle(3001);
//  ent1_thr->SetFillStyle(3004);
  if(!isLine) ent1_thr->SetLineWidth(0);
  ent1_thr->SetLineColor(kOrange+7);
	legBL3->Draw();
  
  //globtex->SetTextAlign(32); //3:right 2:vertical center
	//globtex->SetTextFont(42);
  //globtex->SetTextSize(0.05);
  //globtex->DrawLatex(0.9, 0.23, "0.0 < |y_{CM}| < 0.9");

	globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");

  CMS_lumi( c3, isPA, iPos );
	c3->Update();

  if (isSmoothened) {
    c3->SaveAs("plot_theory/comp_RFB_pt_Vogt_smoothened_rap3.pdf");
    c3->SaveAs("plot_theory/comp_RFB_pt_Vogt_smoothened_rap3.png");
  } else {
    c3->SaveAs("plot_theory/comp_RFB_pt_Vogt_rap3.pdf");
    c3->SaveAs("plot_theory/comp_RFB_pt_Vogt_rap3.png");
  }
  return;
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

