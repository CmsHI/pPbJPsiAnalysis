#include "CMS_lumi.h"
#include "../SONGKYO.h"

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RFB_rap_Vogt(bool isLine=true, bool isSmoothened=false)
{
	gROOT->Macro("./tdrstyle_kyo.C");
  //gStyle->SetHatchesSpacing(5.);
  //gStyle->SetLineScalePS(5.); 
  
  TCanvas* c1 = new TCanvas("c1","c1",200,10,600,600);
  c1->UseCurrentStyle();
	c1->cd();
	int isPA = 1;  // 0:pp, 1:pPb
	int iPos=0;

	///////////////////////////////////////////////////
	///////// Make TGraph from Ramona;s txt files
	//const int nRapRFB = 3;
	const int nRapRFB = 5;
	//// *** NOTE ***
  //// *** 0.0 and 1.93 points for bin edge are added artificially for isSmoothend
  //// *** Also theory values are copied from 0.45 to 0.0, and from 1.71 to 1.93.
  //Double_t theory_px[nRapRFB] = {0.45, 1.20, 1.71};
  Double_t theory_px[nRapRFB] = {0.0, 0.45, 1.20, 1.71, 1.93};
	Double_t theory_exlow_tmp[nRapRFB] = {0, 0, 0.9, 1.5, 1.93};
	Double_t theory_exhigh_tmp[nRapRFB] = {0, 0.9, 1.5, 1.93, 1.93};
	Double_t theory_exlow[nRapRFB];
	Double_t theory_exhigh[nRapRFB];
	Double_t theory_py_lowpt[nRapRFB] = {9.8764E-01, 9.8764E-01, 9.5308E-01, 9.2275E-01, 9.2275E-01};
	Double_t theory_eylow_lowpt_tmp[nRapRFB] = {9.6577E-01, 9.6577E-01, 9.2059E-01, 8.7758E-01, 8.7758E-01};
	Double_t theory_eyhigh_lowpt_tmp[nRapRFB] = {9.8810E-01, 9.8810E-01, 1.0079E+00, 9.8312E-01, 9.8312E-01};
	Double_t theory_eylow_lowpt[nRapRFB];
	Double_t theory_eyhigh_lowpt[nRapRFB];
	Double_t theory_py_highpt[nRapRFB] = {9.8810E-01, 9.8128E-01, 9.4434E-01, 9.0718E-01, 9.0718E-01};
	Double_t theory_eylow_highpt_tmp[nRapRFB] = {9.6745E-01, 9.6745E-01, 9.1307E-01, 8.6649E-01, 8.6649E-01};
	Double_t theory_eyhigh_highpt_tmp[nRapRFB] = {9.9042E-01, 9.9042E-01, 9.7488E-01, 9.6590E-01, 9.6590E-01};
	Double_t theory_eylow_highpt[nRapRFB];
	Double_t theory_eyhigh_highpt[nRapRFB];
	for (int iy = 0 ; iy < nRapRFB; iy ++ ) {
		theory_exlow[iy] = fabs(theory_px[iy] - theory_exlow_tmp[iy]);
		theory_exhigh[iy] = fabs(theory_px[iy] - theory_exhigh_tmp[iy]);
		theory_eylow_lowpt[iy] = fabs(theory_py_lowpt[iy] - theory_eylow_lowpt_tmp[iy]);
		theory_eyhigh_lowpt[iy] = fabs(theory_py_lowpt[iy] - theory_eyhigh_lowpt_tmp[iy]);
		theory_eylow_highpt[iy] = fabs(theory_py_highpt[iy] - theory_eylow_highpt_tmp[iy]);
		theory_eyhigh_highpt[iy] = fabs(theory_py_highpt[iy] - theory_eyhigh_highpt_tmp[iy]);
	}
	
  TGraphAsymmErrors* gRFB_theory_lowpt = new TGraphAsymmErrors(nRapRFB, theory_px, theory_py_lowpt, theory_exlow, theory_exhigh, theory_eylow_lowpt, theory_eyhigh_lowpt);	
	gRFB_theory_lowpt->SetFillColor(kRed-7);
	gRFB_theory_lowpt->SetLineColor(kRed-7);
  if (isSmoothened) gRFB_theory_lowpt->SetFillStyle(3254);
  else gRFB_theory_lowpt->SetFillStyle(3004);
  if(!isLine) gRFB_theory_lowpt->SetLineWidth(0);
//  cout << "GetHatchesSpacing = " << gRFB_theory_lowpt->GetHatchesSpacing() << endl;
/*
	gRFB_theory_lowpt->GetXaxis()->SetLimits(0., 2.1);
	gRFB_theory_lowpt->SetMinimum(0.0);
	gRFB_theory_lowpt->SetMaximum(1.8);
  gRFB_theory_lowpt->GetXaxis()->SetTitle("|y_{CM}|");
  gRFB_theory_lowpt->GetXaxis()->CenterTitle();
  gRFB_theory_lowpt->GetYaxis()->SetTitle("R_{FB}");
  gRFB_theory_lowpt->GetYaxis()->CenterTitle();
*/	
  TGraphAsymmErrors* gRFB_theory_highpt = new TGraphAsymmErrors(nRapRFB, theory_px, theory_py_highpt, theory_exlow, theory_exhigh, theory_eylow_highpt, theory_eyhigh_highpt);	
	gRFB_theory_highpt->SetFillColor(kGreen-4);
	gRFB_theory_highpt->SetLineColor(kGreen-4);
  if (isSmoothened) gRFB_theory_highpt->SetFillStyle(3245);
  else gRFB_theory_highpt->SetFillStyle(3005);
  if(!isLine) gRFB_theory_highpt->SetLineWidth(0);

	///////////////////////////////////////////////////
	//// overlay experimental points	
	///////////////////////////////////////////////////
  //// NEW
	TFile *inFile = new TFile("../DrawFinalPlot/plot_RFB/RFB_rap_isPrompt1.root");
	TGraphAsymmErrors* gRFB_sys_lowpt = (TGraphAsymmErrors*)inFile->Get("gRFB_sys_lowpt"); 	
	TGraphAsymmErrors* gRFB_sys_lowpt_line = (TGraphAsymmErrors*)inFile->Get("gRFB_sys_lowpt"); 	
	TGraphAsymmErrors* gRFB_lowpt = (TGraphAsymmErrors*)inFile->Get("gRFB_lowpt"); 	
	TGraphAsymmErrors* gRFB_sys_highpt = (TGraphAsymmErrors*)inFile->Get("gRFB_sys_highpt"); 	
	TGraphAsymmErrors* gRFB_sys_highpt_line = (TGraphAsymmErrors*)inFile->Get("gRFB_sys_highpt"); 	
	TGraphAsymmErrors* gRFB_highpt = (TGraphAsymmErrors*)inFile->Get("gRFB_highpt"); 	
	
	gRFB_sys_lowpt->SetFillColorAlpha(kRed-10,0.5);
	gRFB_sys_lowpt_line->SetFillColorAlpha(kRed-10,0.0);
	gRFB_sys_highpt->SetFillColorAlpha(kGreen-10,0.5);
	gRFB_sys_highpt_line->SetFillColorAlpha(kGreen-10,0.0);
	SetGraphStyleFinal(gRFB_lowpt,1,3);
	gRFB_lowpt->SetMarkerSize(1.4);
	SetGraphStyleFinal(gRFB_highpt,0,5);
	gRFB_highpt->SetMarkerSize(2.1);
	
  //////////////// Draw

  gRFB_sys_lowpt->Draw("A5");
	gRFB_sys_highpt->Draw("5");
  gRFB_sys_lowpt_line->Draw("5");
	gRFB_sys_highpt_line->Draw("5");
	if (isSmoothened) gRFB_theory_lowpt->Draw("3");
	else gRFB_theory_lowpt->Draw("5");
	if (isSmoothened) gRFB_theory_highpt->Draw("3");
  else gRFB_theory_highpt->Draw("5");
	gRFB_lowpt->Draw("P");
	gRFB_highpt->Draw("P");
  
  dashedLine(0.,1.,2.,1.,1,1);

  ////Legend for experiment 
  TLegend *leg_exp = new TLegend(0.21,0.24,0.50,0.37,NULL,"brNDC");
  SetLegendStyle(leg_exp);
  //leg_exp->SetHeader("CMS");
  TLegendEntry *ent1_exp=leg_exp->AddEntry("ent1_exp","6.5 < p_{T} < 10 GeV/c","lpf");
  ent1_exp->SetFillColor(kRed-10);
  ent1_exp->SetFillStyle(1001);
  ent1_exp->SetLineColor(kPink-6);
  ent1_exp->SetMarkerStyle(kFullSquare);
  ent1_exp->SetMarkerColor(kPink-6);
  ent1_exp->SetMarkerSize(1.9);
	TLegendEntry *ent2_exp=leg_exp->AddEntry("ent2_exp","10 < p_{T} < 30 GeV/c","lpf");
  ent2_exp->SetFillColor(kGreen-10);
  ent2_exp->SetFillStyle(1001);
  ent2_exp->SetLineColor(kGreen+3);
  ent2_exp->SetMarkerStyle(kFullDiamond);
  ent2_exp->SetMarkerColor(kGreen+3);
  ent2_exp->SetMarkerSize(3.1);
  leg_exp->Draw();
  
  ////Legend-like boxes for theory 
  //TBox * box1_thr = new TBox(0.145, 0.54, 0.245, 0.58);
  TBox * box1_thr = new TBox(0.145, 0.1, 0.245, 0.2);
	box1_thr->SetFillColor(kRed-7);
  if (isSmoothened) box1_thr->SetFillStyle(3254);
  else box1_thr->SetFillStyle(3004);
  if(isLine) box1_thr->SetLineWidth(1);
  else box1_thr->SetLineWidth(0);
  box1_thr->SetLineColor(kRed-7);
  box1_thr->Draw("l");
  
  //TBox * box2_thr = new TBox(0.275, 0.54, 0.375, 0.58);
  TBox * box2_thr = new TBox(0.275, 0.1, 0.375, 0.2);
	box2_thr->SetFillColor(kGreen-4);
  if (isSmoothened) box2_thr->SetFillStyle(3245);
  else box2_thr->SetFillStyle(3005);
  if(!isLine) box2_thr->SetLineWidth(0);
  box2_thr->SetLineColor(kGreen-4);
  box2_thr->Draw("l");
  
  TLatex* tex_thr = new TLatex();
  tex_thr->SetNDC();
  tex_thr->SetTextAlign(12); 
  tex_thr->SetTextSize(0.040);
  tex_thr->SetTextFont(42);
  tex_thr->DrawLatex(0.33, 0.20, "EPS09 NLO + CEM (Vogt)");
	
  TLatex* globtex = new TLatex();
  globtex->SetNDC();
  globtex->SetTextAlign(12); 
  globtex->SetTextSize(0.055);
  globtex->SetTextFont(42);
  globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	
	
  
  
  CMS_lumi( c1, isPA, iPos );
  c1->Update();
  if (isSmoothened) {
    c1->SaveAs("plot_theory/comp_RFB_rap_Vogt_smoothened.pdf");
    c1->SaveAs("plot_theory/comp_RFB_rap_Vogt_smoothened.png");
  } else {
    c1->SaveAs("plot_theory/comp_RFB_rap_Vogt.pdf");
    c1->SaveAs("plot_theory/comp_RFB_rap_Vogt.png");
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

