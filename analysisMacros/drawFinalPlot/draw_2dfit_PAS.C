#include <Riostream.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <vector>
#include <TSystem.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include "TStyle.h"
#include "TSystem.h"
#include <TMath.h>
#include <math.h>
#include <sstream>
#include <string>

//#include "CMS_lumi.h"
//#include "CMS_lumi_2dfit.h"
#include "CMS_lumi_2dfit_new.h"
#include "KYOcommonOptFinal.h"


void CMS_lumi_2dfit( TPad* pad, int iPeriod, int iPosX );

void draw_2dfit_PAS(bool isPbp=false, bool isMass=false){
	
	//gROOT->Macro("./JpsiStyleForFinalResult.C");
	gROOT->Macro("./tdrstyle_kyo.C");
	gStyle->SetTitleSize(0.046, "XYZ");
  gStyle->SetEndErrorSize(0);
// Margins:
//  gStyle->SetPadTopMargin(0.05); // KYO
//  gStyle->SetPadBottomMargin(0.132); //KYO
//  gStyle->SetPadLeftMargin(0.132); //KYO
	gStyle->SetOptTitle(0);
  gStyle->SetPadTopMargin(0.075);
  gStyle->SetPadBottomMargin(0.13); //KYO
  gStyle->SetPadLeftMargin(0.13); //KYO
  gStyle->SetPadRightMargin(0.075);
	gStyle->SetTitleXOffset(1.15);
	gStyle->SetTitleYOffset(1.22);
	
	writeExtraText = true;
	extraText  = "Preliminary";
	lumi_502TeV  = "34.6 nb^{-1}";
	int iPeriod = 0; 
	int iPos=0; 

	lumiTextSize = 0.6;
	cmsTextSize      = 0.75;
	//lumiTextOffset=-0.2;

	TFile* f2D;
	TCanvas *c1_n2;

	if (isPbp && isMass) {
		f2D = new TFile("./2dfit_root_pas/Pbp_8rap8pt_20150503_rap-2.4--1.97_pT3.0-4.0_cent0.0-100.0_dPhi0.000-1.571_ntrk0.0-350.0_ET0.0-120.0_massfit_wopull.root");
		c1_n2 = (TCanvas*)f2D->Get("c1_n2");
	}
	else if (isPbp && !isMass) {	
		f2D = new TFile("./2dfit_root_pas/Pbp_8rap8pt_20150503_rap-2.4--1.97_pT3.0-4.0_cent0.0-100.0_dPhi0.000-1.571_ntrk0.0-350.0_ET0.0-120.0_timefit_Log_wopull.root");
		c1_n2 = (TCanvas*)f2D->Get("c2b");
	}	
	else if (!isPbp && isMass) {
		f2D = new TFile("./2dfit_root_pas/pPb_8rap8pt_20150503_rap-0.43-0.47_pT14.0-30.0_cent0.0-100.0_dPhi0.000-1.571_ntrk0.0-350.0_ET0.0-120.0_massfit_wopull.root");
		c1_n2 = (TCanvas*)f2D->Get("c1_n2");
	}
	else {
		f2D = new TFile("./2dfit_root_pas/pPb_8rap8pt_20150503_rap-0.43-0.47_pT14.0-30.0_cent0.0-100.0_dPhi0.000-1.571_ntrk0.0-350.0_ET0.0-120.0_timefit_Log_wopull.root");
		c1_n2 = (TCanvas*)f2D->Get("c2b");
	}	
	//CMS_lumi_2dfit( c1_n2, iPeriod, iPos );
  //gStyle->SetPadTopMargin(0.15); // KYO
	c1_n2->SetTopMargin(0.06);
	c1_n2->SetFrameBorderMode(0);
	c1_n2->SetBorderSize(0);
   
	 gStyle->SetOptTitle(0);
   c1_n2->SetHighLightColor(2);
   c1_n2->Range(2.466667,-115.3873,3.577778,846.1737);
   c1_n2->SetFillColor(0);
   c1_n2->SetBorderMode(0);
   c1_n2->SetBorderSize(0);
   c1_n2->SetTickx(1);
   c1_n2->SetTicky(1);
   c1_n2->SetLeftMargin(0.12);
   c1_n2->SetRightMargin(0.07);
   //c1_n2->SetTopMargin(0.05);
   c1_n2->SetTopMargin(0.07);
   gStyle->SetPadTopMargin(0.07);
   //c1_n2->SetTopMargin(0.10);
   c1_n2->SetBottomMargin(0.12);
   c1_n2->SetFrameBorderMode(0);
   c1_n2->SetFrameBorderMode(0);
 
  /* 
   TH1D *frame_7223b00__1 = new TH1D("frame_7223b00__1","A RooPlot of \"m_{#mu#mu}\"",45,2.6,3.5);
   frame_7223b00__1->SetMinimum(0);
   frame_7223b00__1->SetMaximum(798.0956);
   frame_7223b00__1->SetDirectory(0);
   frame_7223b00__1->SetStats(0);
   frame_7223b00__1->SetMarkerStyle(20);
   frame_7223b00__1->SetMarkerSize(0.8);
   frame_7223b00__1->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
   frame_7223b00__1->GetXaxis()->CenterTitle(true);
   frame_7223b00__1->GetXaxis()->SetLabelFont(42);
   frame_7223b00__1->GetXaxis()->SetTitleSize(0.048);
   frame_7223b00__1->GetXaxis()->SetTitleOffset(1.15);
   frame_7223b00__1->GetXaxis()->SetTitleFont(42);
   frame_7223b00__1->GetYaxis()->SetTitle("Counts / 20 (MeV/c^{2})");
   frame_7223b00__1->GetYaxis()->SetLabelFont(42);
   frame_7223b00__1->GetYaxis()->SetTitleSize(0.048);
   frame_7223b00__1->GetYaxis()->SetTitleOffset(1.2);
   frame_7223b00__1->GetYaxis()->SetTitleFont(42);
   frame_7223b00__1->GetZaxis()->SetLabelFont(42);
   frame_7223b00__1->GetZaxis()->SetTitleSize(0.048);
   frame_7223b00__1->GetZaxis()->SetTitleFont(42);
   frame_7223b00__1->Draw("");
   
   TLegend *leg = new TLegend(0.5956376,0.6013986,0.9244966,0.7814685,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(19);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("Graph","data","p");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("hfake21","total fit","lf");
   entry->SetFillColor(1);
   entry->SetFillStyle(3354);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(4);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("hfake31","bkgd + non-prompt","lf");

    TLatex *     tex = new TLatex(0.9295302,0.9615385,"34.6 nb^{-1} (5.02 TeV)");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.045);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.1560403,0.9213287,"CMS");
tex->SetNDC();
   tex->SetTextAlign(13);
   tex->SetTextFont(61);
   tex->SetTextSize(0.05625);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.1560403,0.8741259,"Preliminary");
tex->SetNDC();
   tex->SetTextAlign(13);
   tex->SetTextFont(52);
   tex->SetTextSize(0.05594406);
   tex->SetLineWidth(2);
   tex->Draw();
   c1_n2->Modified();
   c1_n2->cd();
   c1_n2->SetSelected(c1_n2);
*/
	
	//latex box for beam, rapidity, pT info
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12);
	//globtex->SetTextSize(0.04);
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.035);

	if (isPbp && isMass) { 
		globtex->DrawLatex(0.17, 0.71, "1.5 < y_{CM} < 1.93");
		globtex->DrawLatex(0.17, 0.66, "3 < p_{T} < 4  GeV/c");
	} 
	else if (isPbp && !isMass) {
		globtex->DrawLatex(0.61, 0.86, "1.5 < y_{CM} < 1.93");
		globtex->DrawLatex(0.61, 0.81, "3 < p_{T} < 4  GeV/c");
	} 
	else if (!isPbp && isMass) {
		globtex->DrawLatex(0.17, 0.71, "-0.9 < y_{CM} < 0");
		globtex->DrawLatex(0.17, 0.66, "14 < p_{T} < 30  GeV/c");
	} 
	else {
		globtex->DrawLatex(0.61, 0.86, "-0.9 < y_{CM} < 0");
		globtex->DrawLatex(0.61, 0.81, "14 < p_{T} < 30  GeV/c");
	} 

	
	CMS_lumi_2dfit( c1_n2, iPeriod, iPos );
	c1_n2->Draw();
	c1_n2->Modified();
	c1_n2->Update();
	c1_n2->Draw();
	
	if (isPbp && isMass) { c1_n2->SaveAs("./2dfit_root_pas/Pbpmass.pdf"); }
	else if (isPbp && !isMass) {c1_n2->SaveAs("./2dfit_root_pas/Pbptime.pdf");}
	else if (!isPbp && isMass) {c1_n2->SaveAs("./2dfit_root_pas/pPbmass.pdf");}
	else c1_n2->SaveAs("./2dfit_root_pas/pPbtime.pdf");
	
	if (isPbp && isMass) { c1_n2->SaveAs("./2dfit_root_pas/Pbpmass.png"); }
	else if (isPbp && !isMass) {c1_n2->SaveAs("./2dfit_root_pas/Pbptime.png");}
	else if (!isPbp && isMass) {c1_n2->SaveAs("./2dfit_root_pas/pPbmass.png");}
	else c1_n2->SaveAs("./2dfit_root_pas/pPbtime.png");




	return;
}


void CMS_lumi_2dfit( TPad* pad, int iPeriod, int iPosX )
{            
  bool outOfFrame    = false;
  if( iPosX/10==0 ) 
    {
      outOfFrame = true;
    }
  int alignY_=3;
  int alignX_=2;
  if( iPosX/10==0 ) alignX_=1;
  if( iPosX==0    ) alignX_=1;
  if( iPosX==0    ) alignY_=1;
  if( iPosX/10==1 ) alignX_=1;
  if( iPosX/10==2 ) alignX_=2;
  if( iPosX/10==3 ) alignX_=3;
  //if( iPosX == 0  ) relPosX = 0.12;
  if( iPosX == 0  ) relPosX = 0.15;
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
  if( iPeriod==0 )
    {
      lumiText += lumi_502TeV;
      lumiText += " (pPb 5.02 TeV)";
    }
   
  cout << lumiText << endl;

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);    

  float extraTextSize = extraOverCmsTextSize*cmsTextSize;

  latex.SetTextFont(42);
  latex.SetTextAlign(31); 
  latex.SetTextSize(lumiTextSize*t);    
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

  if( outOfFrame )
    {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextAlign(11); 
      latex.SetTextSize(cmsTextSize*t);    
      latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
    }
  
  pad->cd();

  float posX_=0;
  if( iPosX%10<=1 )
    {
      posX_ =   l + relPosX*(1-l-r);
    }
  else if( iPosX%10==2 )
    {
      posX_ =  l + 0.5*(1-l-r);
    }
  else if( iPosX%10==3 )
    {
      posX_ =  1-r - relPosX*(1-l-r);
    }
  float posY_ = 1-t - relPosY*(1-t-b);
  if( !outOfFrame )
    {
      if( drawLogo )
	{
	  posX_ =   l + 0.045*(1-l-r)*W/H;
	  posY_ = 1-t - 0.045*(1-t-b);
	  float xl_0 = posX_;
	  float yl_0 = posY_ - 0.15;
	  float xl_1 = posX_ + 0.15*H/W;
	  float yl_1 = posY_;
	  TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
	  TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
	  pad_logo->Draw();
	  pad_logo->cd();
	  CMS_logo->Draw("X");
	  pad_logo->Modified();
	  pad->cd();
	}
      else
	{
	  latex.SetTextFont(cmsTextFont);
	  latex.SetTextSize(cmsTextSize*t);
	  latex.SetTextAlign(align_);
	  latex.DrawLatex(posX_, posY_, cmsText);
	  if( writeExtraText ) 
	    {
	      latex.SetTextFont(extraTextFont);
	      latex.SetTextAlign(align_);
	      latex.SetTextSize(extraTextSize*t);
	      latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
	    }
	}
    }
  else if( writeExtraText )
    {
      if( iPosX==0) 
	{
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










