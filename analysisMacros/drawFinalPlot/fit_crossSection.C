#include "TFile.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TLegend.h>
#include <TMath.h>
#include <TPaveText.h>
#include <TText.h>
#include <TLatex.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include "CMS_lumi.h"
#include "KYOcommonOptFinal.h"
#include <TBox.h>

using namespace std;

//void formRapArr(Double_t binmin, Double_t binmax, string* arr);
//void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
//void formPtArr(Double_t binmin, Double_t binmax, string* arr);
void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void fit_crossSection(Int_t runCode=0, char* dirName = "FitCrossRap",bool is3pnt = true)
{
	//gROOT->Macro("./JpsiStyleForFinalResult.C");
	gROOT->Macro("./tdrstyle_kyo.C");
	gStyle->SetTitleSize(0.046, "XYZ");
  gStyle->SetEndErrorSize(0);
// Margins:
//  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.132); //KYO
  gStyle->SetPadLeftMargin(0.132); //KYO
  //gStyle->SetPadRightMargin(0.04);
	gStyle->SetOptTitle(0);
	
	writeExtraText = true;
	extraText  = "Preliminary";
	lumi_502TeV  = "34.6 nb^{-1}";
	int iPeriod = 0; 
	int iPos=11; //left

	double leftlimit;
	if (is3pnt) leftlimit = -1.93;
	else leftlimit = -2.87;

	// set info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	Double_t lumi_nb;
	Double_t lumi_nb_err;
	Double_t lumi_mub;
	Double_t lumi_mub_err;
	string runstring;
	string lumistring;
	string cmsstring = "CMS preliminary";
	string beamstring = "pPb #sqrt{s_{NN}} = 5.02 TeV";
	if (runCode ==0) { runstring = "All"; lumi_nb =34.622; lumi_nb_err=1.2; }
	else if (runCode == 1) { runstring = "Pbp"; lumi_nb =20.7; lumi_nb_err=0.7; }//1stRun
	else if (runCode == 2) { runstring = "pPb"; lumi_nb = 14.0; lumi_nb_err=0.5; }//2ndRun
	lumistring = Form("L_{int} = %.1f nb^{-1}", lumi_nb);
	lumi_mub = lumi_nb * 1000; // (nb)^{-1} -> {#mub}^{-1}
	lumi_mub_err = lumi_nb_err * 1000; // (nb)^{-1} -> {#mub}^{-1}
	
	// read our crossSection_pt graph
	TFile *inFile = new TFile("cross_8rap9pt2gev/crossSection_rap_isScale0.root");
	TGraphAsymmErrors* gCross_pr_sys_lowpt = (TGraphAsymmErrors*)inFile->Get("gCross_pr_sys_lowpt"); 	
	TGraphAsymmErrors* gCross_pr_lowpt = (TGraphAsymmErrors*)inFile->Get("gCross_pr_lowpt"); 	
	TGraphAsymmErrors* gCross_pr_sys_highpt = (TGraphAsymmErrors*)inFile->Get("gCross_pr_sys_highpt"); 	
	TGraphAsymmErrors* gCross_pr_highpt = (TGraphAsymmErrors*)inFile->Get("gCross_pr_highpt"); 	
	cout << "gCross_pr_sys_lowpt = " << gCross_pr_sys_lowpt << endl;
	cout << "gCross_pr_lowpt = " << gCross_pr_lowpt << endl;
	cout << "gCross_pr_sys_highpt = " << gCross_pr_sys_highpt << endl;
	cout << "gCross_pr_highpt = " << gCross_pr_highpt << endl;
	TGraphAsymmErrors* gCross_np_sys_lowpt = (TGraphAsymmErrors*)inFile->Get("gCross_np_sys_lowpt"); 	
	TGraphAsymmErrors* gCross_np_lowpt = (TGraphAsymmErrors*)inFile->Get("gCross_np_lowpt"); 	
	TGraphAsymmErrors* gCross_np_sys_highpt = (TGraphAsymmErrors*)inFile->Get("gCross_np_sys_highpt"); 	
	TGraphAsymmErrors* gCross_np_highpt = (TGraphAsymmErrors*)inFile->Get("gCross_np_highpt"); 	
	cout << "gCross_np_sys_lowpt = " << gCross_np_sys_lowpt << endl;
	cout << "gCross_np_lowpt = " << gCross_np_lowpt << endl;
	cout << "gCross_np_sys_highpt = " << gCross_np_sys_highpt << endl;
	cout << "gCross_np_highpt = " << gCross_np_highpt << endl;


	//////////////////////////////////////////////
	// gaussian function :: pr
	
	//double meanval = 0.01; 
	double meanval_pr = 0.0001; 
	Double_t gpar_pr_low_R[3];
	Double_t gpar_pr_low_L[3];
	Double_t gpar_pr_high_R[3];
	Double_t gpar_pr_high_L[3];
	Double_t gparerr_pr_low_R[3];
	Double_t gparerr_pr_low_L[3];
	Double_t gparerr_pr_high_R[3];
	Double_t gparerr_pr_high_L[3];

	TF1* g_pr_low_R= new TF1("g_pr_low_R", "gaus", 0., 1.93);
	g_pr_low_R->SetRange(0.,1.93);
	g_pr_low_R->SetParLimits(1,-meanval_pr,+meanval_pr);
	g_pr_low_R->SetLineColor(kRed);
	gCross_pr_sys_lowpt->Fit("g_pr_low_R","R");
	g_pr_low_R->GetParameters(&gpar_pr_low_R[0]);
	gparerr_pr_low_R[2] = g_pr_low_R->GetParError(2);

	TF1* g_pr_low_L= new TF1("g_pr_low_L", "gaus", leftlimit, 0.);
	g_pr_low_L->SetRange(leftlimit,0.);
	g_pr_low_L->SetParLimits(1,-meanval_pr,+meanval_pr);
	g_pr_low_L->SetLineColor(kBlue);
	gCross_pr_sys_lowpt->Fit("g_pr_low_L","R");
	g_pr_low_L->GetParameters(&gpar_pr_low_L[0]);
	gparerr_pr_low_L[2] = g_pr_low_L->GetParError(2);
	
	TF1* g_pr_high_R= new TF1("g_pr_high_R", "gaus", 0., 1.93);
	g_pr_high_R->SetRange(0.,1.93);
	g_pr_high_R->SetParLimits(1,-meanval_pr,+meanval_pr);
	g_pr_high_R->SetLineColor(kRed);
	gCross_pr_sys_highpt->Fit("g_pr_high_R","R");
	g_pr_high_R->GetParameters(&gpar_pr_high_R[0]);
	gparerr_pr_high_R[2] = g_pr_high_R->GetParError(2);
	
	TF1* g_pr_high_L= new TF1("g_pr_high_L", "gaus", leftlimit, 0.);
	g_pr_high_L->SetRange(leftlimit,0.);
	g_pr_high_L->SetParLimits(1,-meanval_pr,+meanval_pr);
	g_pr_high_L->SetLineColor(kBlue);
	gCross_pr_sys_highpt->Fit("g_pr_high_L","R");
	g_pr_high_L->GetParameters(&gpar_pr_high_L[0]);
	gparerr_pr_high_L[2] = g_pr_high_L->GetParError(2);

	
	//////////////////////////////////////////////
	TLegend *legUR = new TLegend(0.57, 0.70, 0.90, 0.88); //upper left
	SetLegendStyle(legUR);

	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12);
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.035);
	
	////////////1) prompt
	TCanvas* c_pr = new TCanvas("c_pr","c_pr",200,10,800,600);
	c_pr->cd();

	gCross_pr_sys_lowpt->GetXaxis()->SetTitle("y_{CM}");	
	gCross_pr_sys_lowpt->GetXaxis()->CenterTitle();	
	gCross_pr_sys_lowpt->GetYaxis()->SetTitle("d#sigma/dy [#mub]");	
	gCross_pr_sys_lowpt->GetXaxis()->SetLimits(-2.87,1.93);	
		gCross_pr_sys_lowpt->SetMinimum(0.0);	
		gCross_pr_sys_lowpt->SetMaximum(65.);	
	
	gCross_pr_sys_lowpt->SetFillColor(kRed-9);	
	gCross_pr_sys_lowpt->Draw("A2");

	gCross_pr_sys_highpt->SetFillColor(kTeal+7);	
	gCross_pr_sys_highpt->Draw("2");

	SetGraphStyle(gCross_pr_lowpt,1,3);
	gCross_pr_lowpt->SetMarkerSize(1.2);
	gCross_pr_lowpt->Draw("P");

	SetGraphStyle(gCross_pr_highpt,0,5);
	gCross_pr_highpt->SetMarkerSize(1.9);
	gCross_pr_highpt->Draw("P");

	legUR -> SetHeader("Prompt J/#psi");
	legUR -> AddEntry(gCross_pr_lowpt,"6.5 < p_{T} < 10 GeV/c","lp");
	legUR -> AddEntry(gCross_pr_highpt,"10 < p_{T} < 30 GeV/c","lp");
	legUR->Draw();
	globtex->SetTextSize(0.035);
	globtex->DrawLatex(0.17, 0.71, "Global uncertainty : 3.6 \%");
	CMS_lumi( c_pr, iPeriod, iPos );
	
	g_pr_low_R->Draw("l same");
	g_pr_low_L->Draw("l same");
	g_pr_high_R->Draw("l same");
	g_pr_high_L->Draw("l same");
	//globtex->DrawLatex(0.59, 0.42, Form("low p_{T} R (#sigma : %.1f #pm %.1f)",gpar_pr_low_R[2], gparerr_pr_low_R[2]));
	globtex->SetTextSize(0.030);
	globtex->DrawLatex(0.61, 0.45, Form("(low p_{T} R) #sigma : %.1f #pm %.1f",gpar_pr_low_R[2], gparerr_pr_low_R[2]));
	globtex->DrawLatex(0.33, 0.45, Form("(low p_{T} L) #sigma : %.1f #pm %.1f",gpar_pr_low_L[2], gparerr_pr_low_L[2]));
	globtex->DrawLatex(0.61, 0.27, Form("(high p_{T} R) #sigma : %.1f #pm %.1f",gpar_pr_high_R[2], gparerr_pr_high_R[2]));
	globtex->DrawLatex(0.33, 0.27, Form("(high p_{T} L) #sigma : %.1f #pm %.1f",gpar_pr_high_L[2], gparerr_pr_high_L[2]));
	c_pr->Update();
	c_pr->SaveAs(Form("%s/fit_rap_pr_is3pnt%d.pdf",dirName,(int)is3pnt));
	//c_pr->SaveAs(Form("cross_%s/crossSection_rap_pr_isScale%d.pdf",dirName,(int)isScale));
	legUR->Clear();

	
	//////////////////////////////////////////////
	// gaussian function :: non-pr
	
	//double meanval_np = 0.01; 
	double meanval_np = 0.0001; 
	Double_t gpar_np_low_R[3];
	Double_t gpar_np_low_L[3];
	Double_t gpar_np_high_R[3];
	Double_t gpar_np_high_L[3];
	Double_t gparerr_np_low_R[3];
	Double_t gparerr_np_low_L[3];
	Double_t gparerr_np_high_R[3];
	Double_t gparerr_np_high_L[3];

	TF1* g_np_low_R= new TF1("g_np_low_R", "gaus", 0., 1.93);
	g_np_low_R->SetRange(0.,1.93);
	g_np_low_R->SetParLimits(1,-meanval_np,+meanval_np);
	g_np_low_R->SetLineColor(kRed);
	gCross_np_sys_lowpt->Fit("g_np_low_R","R");
	g_np_low_R->GetParameters(&gpar_np_low_R[0]);
	gparerr_np_low_R[2] = g_np_low_R->GetParError(2);

	TF1* g_np_low_L= new TF1("g_np_low_L", "gaus", leftlimit, 0.);
	g_np_low_L->SetRange(leftlimit,0.);
	g_np_low_L->SetParLimits(1,-meanval_np,+meanval_np);
	g_np_low_L->SetLineColor(kBlue);
	gCross_np_sys_lowpt->Fit("g_np_low_L","R");
	g_np_low_L->GetParameters(&gpar_np_low_L[0]);
	gparerr_np_low_L[2] = g_np_low_L->GetParError(2);
	
	TF1* g_np_high_R= new TF1("g_np_high_R", "gaus", 0., 1.93);
	g_np_high_R->SetRange(0.,1.93);
	g_np_high_R->SetParLimits(1,-meanval_np,+meanval_np);
	g_np_high_R->SetLineColor(kRed);
	gCross_np_sys_highpt->Fit("g_np_high_R","R");
	g_np_high_R->GetParameters(&gpar_np_high_R[0]);
	gparerr_np_high_R[2] = g_np_high_R->GetParError(2);
	
	TF1* g_np_high_L= new TF1("g_np_high_L", "gaus", leftlimit, 0.);
	g_np_high_L->SetRange(leftlimit,0.);
	g_np_high_L->SetParLimits(1,-meanval_np,+meanval_np);
	g_np_high_L->SetLineColor(kBlue);
	gCross_np_sys_highpt->Fit("g_np_high_L","R");
	g_np_high_L->GetParameters(&gpar_np_high_L[0]);
	//gparerr_np_high_L[2] = g_np_high_L->GetParError(gpar_np_high_L[2]);
	gparerr_np_high_L[2] = g_np_high_L->GetParError(2);
	cout << "gparerr_np_high_L[2] = " << gparerr_np_high_L[2] << endl;

	////////////2) non-prompt
	TCanvas* c_np = new TCanvas("c_np","c_np",200,10,800,600);
	c_np->cd();
	
	gCross_np_sys_lowpt->GetXaxis()->SetTitle("y_{CM}");	
	gCross_np_sys_lowpt->GetXaxis()->CenterTitle();	
	gCross_np_sys_lowpt->GetYaxis()->SetTitle("d#sigma/dy [#mub]");	
	gCross_np_sys_lowpt->GetXaxis()->SetLimits(-2.87,1.93);	
		gCross_np_sys_lowpt->SetMinimum(0.0);	
		gCross_np_sys_lowpt->SetMaximum(18.0);	
	gCross_np_sys_lowpt->SetFillColor(kRed-9);	
	gCross_np_sys_lowpt->Draw("A2");
	
	gCross_np_sys_highpt->SetFillColor(kTeal+7);	
	gCross_np_sys_highpt->Draw("2");
	
	SetGraphStyle(gCross_np_lowpt,1,3);
	gCross_np_lowpt->SetMarkerSize(1.2);
	gCross_np_lowpt->Draw("P");
	
	SetGraphStyle(gCross_np_highpt,0,5);
	gCross_np_highpt->SetMarkerSize(1.9);
	gCross_np_highpt->Draw("P");

	legUR -> SetHeader("Non-prompt J/#psi");
	legUR -> AddEntry(gCross_np_lowpt,"6.5 < p_{T} < 10 GeV/c","lp");
	legUR -> AddEntry(gCross_np_highpt,"10 < p_{T} < 30 GeV/c","lp");
	legUR->Draw();
	globtex->SetTextSize(0.035);
	globtex->DrawLatex(0.17, 0.71, "Global uncertainty : 3.6 \%");
	CMS_lumi( c_np, iPeriod, iPos );
	
	g_np_low_R->Draw("l same");
	g_np_low_L->Draw("l same");
	g_np_high_R->Draw("l same");
	g_np_high_L->Draw("l same");
	globtex->SetTextSize(0.030);
	globtex->DrawLatex(0.61, 0.45, Form("(low p_{T} R) #sigma : %.1f #pm %.1f",gpar_np_low_R[2], gparerr_np_low_R[2]));
	globtex->DrawLatex(0.33, 0.45, Form("(low p_{T} L) #sigma : %.1f #pm %.1f",gpar_np_low_L[2], gparerr_np_low_L[2]));
	globtex->DrawLatex(0.61, 0.20, Form("(high p_{T} R) #sigma : %.1f #pm %.1f",gpar_np_high_R[2], gparerr_np_high_R[2]));
	globtex->DrawLatex(0.33, 0.20, Form("(high p_{T} L) #sigma : %.1f #pm %.1f",gpar_np_high_L[2], gparerr_np_high_L[2]));

	//cout << "gparerr_np_low_L[2] = " << gparerr_np_low_L[2] << endl;
	cout << "gparerr_np_high_L[2] = " << gparerr_np_high_L[2] << endl;
	c_np->Update();
	c_np->SaveAs(Form("%s/fit_rap_np_is3pnt%d.pdf",dirName,(int)is3pnt));
//	c_np->SaveAs(Form("cross_%s/crossSection_rap_np_isScale%d.pdf",dirName,(int)isScale));
	legUR->Clear();


	
	return;

}


void CMS_lumi( TPad* pad, int iPeriod, int iPosX )
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
  if( iPosX == 0  ) relPosX = 0.12;
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








