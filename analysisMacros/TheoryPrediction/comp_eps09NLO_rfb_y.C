#include "CMS_lumi.h"
#include "../SONGKYO.h"

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_eps09NLO_rfb_y()
{
	gROOT->Macro("./tdrstyle_kyo.C");
  
  TCanvas* c1 = new TCanvas("c1","c1",200,10,600,600);
	c1->cd();
	int isPA = 1;  // 0:pp, 1:pPb
	int iPos=0;

	///////////////////////////////////////////////////
	///////// Make TGraph from Ramona;s txt files
	const int nRapRFB = 3;
	Double_t eps09NLO_px[nRapRFB] = {0.45, 1.20, 1.71};
	//Double_t eps09NLO_px[nRapRFB] = {0.0, 1.20, 1.93};
	Double_t eps09NLO_exlow_tmp[nRapRFB] = {0, 0.9, 1.5};
	Double_t eps09NLO_exhigh_tmp[nRapRFB] = {0.9, 1.5, 1.93};
	Double_t eps09NLO_exlow[nRapRFB];
	Double_t eps09NLO_exhigh[nRapRFB];
	Double_t eps09NLO_py_lowpt[nRapRFB] = {9.8764E-01, 9.5308E-01, 9.2275E-01};
	Double_t eps09NLO_eylow_lowpt_tmp[nRapRFB] = {9.6577E-01, 9.2059E-01, 8.7758E-01};
	Double_t eps09NLO_eyhigh_lowpt_tmp[nRapRFB] = {9.8810E-01, 1.0079E+00, 9.8312E-01};
	Double_t eps09NLO_eylow_lowpt[nRapRFB];
	Double_t eps09NLO_eyhigh_lowpt[nRapRFB];
	Double_t eps09NLO_py_highpt[nRapRFB] = {9.8128E-01, 9.4434E-01, 9.0718E-01};
	Double_t eps09NLO_eylow_highpt_tmp[nRapRFB] = {9.6745E-01, 9.1307E-01, 8.6649E-01};
	Double_t eps09NLO_eyhigh_highpt_tmp[nRapRFB] = {9.9042E-01, 9.7488E-01, 9.6590E-01};
	Double_t eps09NLO_eylow_highpt[nRapRFB];
	Double_t eps09NLO_eyhigh_highpt[nRapRFB];
	for (int iy = 0 ; iy < nRapRFB; iy ++ ) {
		eps09NLO_exlow[iy] = fabs(eps09NLO_px[iy] - eps09NLO_exlow_tmp[iy]);
		eps09NLO_exhigh[iy] = fabs(eps09NLO_px[iy] - eps09NLO_exhigh_tmp[iy]);
		eps09NLO_eylow_lowpt[iy] = fabs(eps09NLO_py_lowpt[iy] - eps09NLO_eylow_lowpt_tmp[iy]);
		eps09NLO_eyhigh_lowpt[iy] = fabs(eps09NLO_py_lowpt[iy] - eps09NLO_eyhigh_lowpt_tmp[iy]);
		eps09NLO_eylow_highpt[iy] = fabs(eps09NLO_py_highpt[iy] - eps09NLO_eylow_highpt_tmp[iy]);
		eps09NLO_eyhigh_highpt[iy] = fabs(eps09NLO_py_highpt[iy] - eps09NLO_eyhigh_highpt_tmp[iy]);
	}
	
  TGraphAsymmErrors* gRFB_eps09NLO_lowpt = new TGraphAsymmErrors(nRapRFB, eps09NLO_px, eps09NLO_py_lowpt, eps09NLO_exlow, eps09NLO_exhigh, eps09NLO_eylow_lowpt, eps09NLO_eyhigh_lowpt);	
	gRFB_eps09NLO_lowpt->SetFillColor(kRed-4);
  gRFB_eps09NLO_lowpt->SetFillStyle(3004);
	gRFB_eps09NLO_lowpt->SetLineColor(kRed-4);
	gRFB_eps09NLO_lowpt->GetXaxis()->SetLimits(0., 2.0);
	gRFB_eps09NLO_lowpt->SetMinimum(0.5);
	gRFB_eps09NLO_lowpt->SetMaximum(1.15);
  gRFB_eps09NLO_lowpt->GetXaxis()->SetTitle("|y_{CM}|");
  gRFB_eps09NLO_lowpt->GetXaxis()->CenterTitle();
  gRFB_eps09NLO_lowpt->GetYaxis()->SetTitle("R_{FB}");
  gRFB_eps09NLO_lowpt->GetYaxis()->CenterTitle();
	//gRFB_eps09NLO_lowpt->Draw("A5");
	gRFB_eps09NLO_lowpt->Draw("A3");
	
  TGraphAsymmErrors* gRFB_eps09NLO_highpt = new TGraphAsymmErrors(nRapRFB, eps09NLO_px, eps09NLO_py_highpt, eps09NLO_exlow, eps09NLO_exhigh, eps09NLO_eylow_highpt, eps09NLO_eyhigh_highpt);	
	gRFB_eps09NLO_highpt->SetFillColor(kGreen-4);
  gRFB_eps09NLO_highpt->SetFillStyle(3005);
	gRFB_eps09NLO_highpt->SetLineColor(kGreen-4);
	//gRFB_eps09NLO_highpt->Draw("5");
	gRFB_eps09NLO_highpt->Draw("3");
  
  dashedLine(0.,1.,2.,1.,1,1);

  //// Legend	
  TLegend *leg_lowpt = new TLegend(0.20,0.28,0.45,0.40,NULL,"brNDC");
  SetLegendStyle(leg_lowpt);
  TLegendEntry *ent1_exp=leg_lowpt->AddEntry("ent1_exp","CMS, 6.5 < p_{T} < 10 GeV/c","lpf");
  ent1_exp->SetFillColor(kRed-10);
  ent1_exp->SetFillStyle(1001);
  ent1_exp->SetLineColor(kPink-6);
  ent1_exp->SetMarkerStyle(kFullSquare);
  ent1_exp->SetMarkerColor(kPink-6);
  ent1_exp->SetMarkerSize(2.1);
  TLegendEntry *ent1_thr=leg_lowpt->AddEntry("ent1_thr","EPS09 NLO + CEM (Vogt et al.)","f");
	ent1_thr->SetFillColor(kRed-4);
  ent1_thr->SetFillStyle(3004);
  ent1_thr->SetLineColor(kRed-4);
	leg_lowpt->Draw();
  
  TLegend *leg_highpt = new TLegend(0.20,0.16,0.45,0.28,NULL,"brNDC");
  SetLegendStyle(leg_highpt);
	TLegendEntry *ent2_exp=leg_highpt->AddEntry("ent2_exp","CMS, 10 < p_{T} < 30 GeV/c","lpf");
  ent2_exp->SetFillColor(kTeal-9);
  ent2_exp->SetFillStyle(1001);
  ent2_exp->SetLineColor(kGreen+3);
  ent2_exp->SetMarkerStyle(kFullDiamond);
  ent2_exp->SetMarkerColor(kGreen+3);
  ent2_exp->SetMarkerSize(3.3);
  TLegendEntry *ent2_thr=leg_highpt->AddEntry("ent2_thr","EPS09 NLO + CEM (Vogt et al.)","f");
	ent2_thr->SetFillColor(kGreen-4);
  ent2_thr->SetFillStyle(3005);
  ent2_thr->SetLineColor(kGreen-4);
	leg_highpt->Draw();

  TLatex* globtex = new TLatex();
  globtex->SetNDC();
  globtex->SetTextAlign(12); 
  globtex->SetTextSize(0.055);
  globtex->SetTextFont(42);
  globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	
	///////////////////////////////////////////////////
	//// overlay experimental points	
	///////////////////////////////////////////////////
  //// PAS
	TFile *inFile = new TFile("../../2015PAS/drawFinalPlot/RFB_8rap9pt2gev/RFB_rap_isPrompt1.root");
  //// NEW
	//TFile *inFile = new TFile("../DrawFinalPlot/plot_RFB/RFB_rap_isPrompt1_noPtWeight.root");
	TGraphAsymmErrors* gRFB_sys_lowpt = (TGraphAsymmErrors*)inFile->Get("gRFB_sys_lowpt"); 	
	TGraphAsymmErrors* gRFB_lowpt = (TGraphAsymmErrors*)inFile->Get("gRFB_lowpt"); 	
	TGraphAsymmErrors* gRFB_sys_highpt = (TGraphAsymmErrors*)inFile->Get("gRFB_sys_highpt"); 	
	TGraphAsymmErrors* gRFB_highpt = (TGraphAsymmErrors*)inFile->Get("gRFB_highpt"); 	
	
	gRFB_sys_lowpt->SetFillColor(kRed-10); //tmp
	gRFB_sys_lowpt->Draw("2");
	gRFB_sys_highpt->SetFillColor(kTeal-9); //tmp
	gRFB_sys_highpt->Draw("2");
	SetGraphStyleFinal(gRFB_lowpt,1,3);
	gRFB_lowpt->Draw("P");
	SetGraphStyleFinal(gRFB_highpt,0,5);
	gRFB_highpt->SetMarkerSize(3.3);
	gRFB_highpt->Draw("P");

	CMS_lumi( c1, isPA, iPos );
  c1->Update();
  c1->SaveAs("plot_theory/eps09NLO_rfb_y.pdf");
  c1->SaveAs("plot_theory/eps09NLO_rfb_y.png");
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

