#include "CMS_lumi.h"
#include "../SONGKYO.h"

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RFB_pt_Vogt(double ptmax=20, bool isLine=true, bool isSmoothened=false)
{
	gROOT->Macro("./tdrstyle_kyo.C");
  
  TCanvas* c1 = new TCanvas("c1","c1",200,10,600,600);
	c1->cd();
	int isPA = 1;  // 0:pp, 1:pPb
	int iPos=0;

	///////////////////////////////////////////////////
	///////// from Ramona
	const int nRapRFB = 3;
	//const int nPtRFB = 3;
	const int nPtRFB = 5;
	//// *** NOTE ***
  //// *** low_pT(5 or 6.5) and high_pT(20) points for bin edge are added artificially for isSmoothend
  //// *** Also theory values are copied from 0.45 to 0.0, and from 1.71 to 1.93.
	Double_t theory_px[nRapRFB][nPtRFB] = {
    {5., 5.740347, 7.90416, 13.21239, ptmax}, //1.5-1.93
    {6.5, 6.5, 7.95946, 13.31027, ptmax}, //0.9-1.5 
    {6.5, 6.5, 8.25131, 13.61970, ptmax} //0.0-0.9
  };
	Double_t theory_py[nRapRFB][nPtRFB] = {
    {9.4911E-01, 9.4911E-01, 9.2275E-01, 9.0718E-01, 9.0718E-01},
    {9.5308E-01, 9.5308E-01, 9.5308E-01, 9.4434E-01, 9.4434E-01},
    {9.8764E-01, 9.8764E-01, 9.8764E-01, 9.8128E-01, 9.8128E-01}
  };
	Double_t theory_exlow_tmp[nRapRFB][nPtRFB] = {
    {5., 5., 6.5, 10., ptmax},
    {6.5, 6.5, 6.5, 10, ptmax},
    {6.5, 6.5, 6.5, 10., ptmax}
	};
  Double_t theory_exhigh_tmp[nRapRFB][nPtRFB] = {
    {5., 6.5, 10., ptmax, ptmax},
    {6.5, 6.5, 10, ptmax, ptmax},
    {6.5, 6.5, 10., ptmax, ptmax}
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
	gRFB_theory[0]->SetFillColor(kGreen-4);
  if (isSmoothened) gRFB_theory[0]->SetFillStyle(3245);
  else gRFB_theory[0]->SetFillStyle(3005);
	if(!isLine) gRFB_theory[0]->SetLineWidth(0);
	gRFB_theory[0]->SetLineColor(kGreen-4);
	gRFB_theory[0]->GetXaxis()->SetLimits(0.,ptmax);
	gRFB_theory[0]->SetMinimum(0.5);
	gRFB_theory[0]->SetMaximum(1.15);
  gRFB_theory[0]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  gRFB_theory[0]->GetXaxis()->CenterTitle();
  gRFB_theory[0]->GetYaxis()->SetTitle("R_{FB}");
  gRFB_theory[0]->GetYaxis()->CenterTitle();
	if (isSmoothened) gRFB_theory[0]->Draw("A3");
  else gRFB_theory[0]->Draw("A5");
  gRFB_theory[1]->SetFillColor(kRed-7);
  if (isSmoothened) gRFB_theory[1]->SetFillStyle(3254);
  else gRFB_theory[1]->SetFillStyle(3004);
  if(!isLine) gRFB_theory[1]->SetLineWidth(0);
  gRFB_theory[1]->SetLineColor(kRed-7);
	if (isSmoothened) gRFB_theory[1]->Draw("3");
	else gRFB_theory[1]->Draw("5");
	gRFB_theory[2]->SetFillColor(kAzure+1);
  gRFB_theory[2]->SetFillStyle(3002);
  //gRFB_theory[2]->SetFillStyle(3295);
	if(!isLine) gRFB_theory[2]->SetLineWidth(0);
	gRFB_theory[2]->SetLineColor(kAzure+1);
	if (isSmoothened) gRFB_theory[2]->Draw("3");
	else gRFB_theory[2]->Draw("5");

	dashedLine(0.,1.,ptmax,1.,1,1);
  
  //// Legend for experiments	
  TLegend *leg_exp = new TLegend(0.58,0.25,0.85,0.45,NULL,"brNDC");
  SetLegendStyle(leg_exp);
  //leg_exp->SetTextSize(0.033);
  TLegendEntry *ent1_exp=leg_exp->AddEntry("ent1_exp","0 < |y_{CM}| < 0.9","lpf");
  ent1_exp->SetFillColor(kBlue-10);
  ent1_exp->SetFillStyle(1001);
  ent1_exp->SetLineColor(kBlue-3);
  ent1_exp->SetMarkerStyle(kFullCircle);
  ent1_exp->SetMarkerColor(kBlue-3);
  ent1_exp->SetMarkerSize(2.1);
  TLegendEntry *ent2_exp=leg_exp->AddEntry("ent2_exp","0.9 < |y_{CM}| < 1.5","lpf");
  ent2_exp->SetFillColor(kRed-10);
  ent2_exp->SetFillStyle(1001);
  ent2_exp->SetLineColor(kPink-6);
  ent2_exp->SetMarkerStyle(kFullSquare);
  ent2_exp->SetMarkerColor(kPink-6);
  ent2_exp->SetMarkerSize(2.1);
	TLegendEntry *ent3_exp=leg_exp->AddEntry("ent3_exp","1.5 < |y_{CM}| < 1.93","lpf");
  ent3_exp->SetFillColor(kTeal-9);
  ent3_exp->SetFillStyle(1001);
  ent3_exp->SetLineColor(kGreen+3);
  ent3_exp->SetMarkerStyle(kFullDiamond);
  ent3_exp->SetMarkerColor(kGreen+3);
  ent3_exp->SetMarkerSize(3.3);
	leg_exp->Draw();
  
  ////Legend-like boxes for theory 
  TBox * box1_thr = new TBox(5.0, 0.54, 6.0, 0.58);
	box1_thr->SetFillColor(kAzure+1);
  if (isSmoothened) box1_thr->SetFillStyle(3002);
  else box1_thr->SetFillStyle(3002);
  if(!isLine) box1_thr->SetLineWidth(0);
  box1_thr->SetLineColor(kAzure+1);
  box1_thr->Draw("l");
  
  TBox * box2_thr = new TBox(6.25, 0.54, 7.25, 0.58);
	box2_thr->SetFillColor(kRed-7);
  if (isSmoothened) box2_thr->SetFillStyle(3254);
  else box2_thr->SetFillStyle(3004);
  if(isLine) box2_thr->SetLineWidth(1);
  else box2_thr->SetLineWidth(0);
  box2_thr->SetLineColor(kRed-7);
  box2_thr->Draw("l");
	
  TBox * box3_thr = new TBox(7.5, 0.54, 8.5, 0.58);
  box3_thr->SetFillColor(kGreen-4);
  if (isSmoothened) box3_thr->SetFillStyle(3245);
  else box3_thr->SetFillStyle(3005);
  if(!isLine) box3_thr->SetLineWidth(0);
  box3_thr->SetLineColor(kGreen-4);
  box3_thr->Draw("l");
  
  TLatex* tex_thr = new TLatex();
  tex_thr->SetNDC();
  tex_thr->SetTextAlign(32); 
  tex_thr->SetTextSize(0.040);
  tex_thr->SetTextFont(42);
  tex_thr->DrawLatex(0.93, 0.20, "EPS09 NLO + CEM (Vogt)");

/*
  //// Legend	
  TLegend *legBR = new TLegend(0.45,0.15,0.70,0.42,NULL,"brNDC");
  SetLegendStyle(legBR);
  legBR->SetTextSize(0.033);
  TLegendEntry *ent1_exp=legBR->AddEntry("ent1_exp","CMS, 0 < |y_{CM}| < 0.9","lpf");
  ent1_exp->SetFillColor(kBlue-10);
  ent1_exp->SetFillStyle(1001);
  ent1_exp->SetLineColor(kBlue-3);
  ent1_exp->SetMarkerStyle(kFullCircle);
  ent1_exp->SetMarkerColor(kBlue-3);
  ent1_exp->SetMarkerSize(2.1);
  TLegendEntry *ent1_thr=legBR->AddEntry("ent1_thr","EPS09 NLO + CEM (Vogt)","f");
	ent1_thr->SetFillColor(kAzure+1);
  if (isSmoothened) ent1_thr->SetFillStyle(3002);
  else ent1_thr->SetFillStyle(3002);
  if(!isLine) ent1_thr->SetLineWidth(0);
  ent1_thr->SetLineColor(kAzure+1);
 
  TLegendEntry *ent2_exp=legBR->AddEntry("ent2_exp","CMS, 0.9 < |y_{CM}| < 1.5","lpf");
  ent2_exp->SetFillColor(kRed-10);
  ent2_exp->SetFillStyle(1001);
  ent2_exp->SetLineColor(kPink-6);
  ent2_exp->SetMarkerStyle(kFullSquare);
  ent2_exp->SetMarkerColor(kPink-6);
  ent2_exp->SetMarkerSize(2.1);
  TLegendEntry *ent2_thr=legBR->AddEntry("ent2_thrr","EPS09 NLO + CEM (Vogt)","f");
	ent2_thr->SetFillColor(kRed-7);
  if (isSmoothened) ent2_thr->SetFillStyle(3254);
  else ent2_thr->SetFillStyle(3004);
  if(!isLine) ent2_thr->SetLineWidth(0);
  ent2_thr->SetLineColor(kRed-7);
  
	TLegendEntry *ent3_exp=legBR->AddEntry("ent3_exp","CMS, 1.5 < |y_{CM}| < 1.93","lpf");
  ent3_exp->SetFillColor(kTeal-9);
  ent3_exp->SetFillStyle(1001);
  ent3_exp->SetLineColor(kGreen+3);
  ent3_exp->SetMarkerStyle(kFullDiamond);
  ent3_exp->SetMarkerColor(kGreen+3);
  ent3_exp->SetMarkerSize(3.3);
  TLegendEntry *ent3_thr=legBR->AddEntry("ent3_thr","EPS09 NLO + CEM (Vogt)","f");
	ent3_thr->SetFillColor(kGreen-4);
  if (isSmoothened) ent3_thr->SetFillStyle(3245);
  else ent3_thr->SetFillStyle(3005);
  if(!isLine) ent3_thr->SetLineWidth(0);
  ent3_thr->SetLineColor(kGreen-4);
	legBR->Draw();
*/

  TLatex* globtex = new TLatex();
  globtex->SetNDC();
  globtex->SetTextAlign(12); 
  globtex->SetTextSize(0.055);
  globtex->SetTextFont(42);
  globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	
	
  ///////////////////////////////////////////////////
	//////// overlay experimental points	
	///////////////////////////////////////////////////

  //// PAS
  TFile *inFile = new TFile("../../2015PAS/drawFinalPlot/RFB_8rap9pt2gev/RFB_pt_isPrompt1.root");
  //// NEW
  //TFile *inFile = new TFile("../DrawFinalPlot/plot_RFB/RFB_pt_isPrompt1_noPtWeight.root");
  
  TGraphAsymmErrors* gRFB_sys[nRapRFB];
  TGraphAsymmErrors* gRFB[nRapRFB];
  for (int ir = 0 ; ir < nRapRFB; ir ++ ) {
    gRFB_sys[ir] = (TGraphAsymmErrors*)inFile->Get(Form("gRFB_sys_%d",ir));
    gRFB[ir] = (TGraphAsymmErrors*)inFile->Get(Form("gRFB_%d",ir));
    // tmp for PAS (sys erro box width)
    for (int ipt=0; ipt < nPtRFB; ipt++) {
      gRFB_sys[ir]->SetPointEXlow(ipt, 0.3);
      gRFB_sys[ir]->SetPointEXhigh(ipt, 0.3);
    } 
  } 
  
  gRFB_sys[0]->SetFillColor(kTeal-9);
	gRFB_sys[0]->Draw("2");
  gRFB_sys[1]->SetFillColor(kRed-10);
	gRFB_sys[1]->Draw("2");
  gRFB_sys[2]->SetFillColor(kBlue-10);
	gRFB_sys[2]->Draw("2");

  SetGraphStyleFinal(gRFB[0], 0, 5);
  SetGraphStyleFinal(gRFB[1], 1, 3);
  SetGraphStyleFinal(gRFB[2], 2, 0);
  gRFB[0]->SetMarkerSize(3.3);
	gRFB[0]->Draw("P");
	gRFB[1]->Draw("P");
	gRFB[2]->Draw("P");
	

	CMS_lumi( c1, isPA, iPos );
	c1->Update();
  if (isSmoothened) {
    c1->SaveAs("plot_theory/comp_RFB_pt_Vogt_smoothened.pdf");
    c1->SaveAs("plot_theory/comp_RFB_pt_Vogt_smoothened.png");
  } else {
    c1->SaveAs("plot_theory/comp_RFB_pt_Vogt.pdf");
    c1->SaveAs("plot_theory/comp_RFB_pt_Vogt.png");
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

