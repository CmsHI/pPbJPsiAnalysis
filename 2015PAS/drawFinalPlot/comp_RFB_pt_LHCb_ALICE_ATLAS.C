#include "TFile.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
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

#include "KYOcommonOptFinal.h"
#include <TBox.h>
//#include <vector>
//#include <list>
//#include <algorithm>
//#include <stdlib.h>

using namespace std;

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

int comp_RFB_pt_LHCb_ALICE_ATLAS(Int_t runCode=0)
{
  //
  //
  //gROOT->Macro("./JpsiStyleForFinalResult.C");

  //gROOT->LoadMacro("tdrstyle.C");
  gROOT->LoadMacro("tdrstyle_kyo.C");
  //setTDRStyle();  
  gROOT->LoadMacro("CMS_lumi.C");
	
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
  gStyle->SetPadTopMargin(0.075);
  gStyle->SetPadBottomMargin(0.13); //KYO
  gStyle->SetPadLeftMargin(0.13); //KYO
  gStyle->SetPadRightMargin(0.075);
	gStyle->SetTitleXOffset(1.15);
	gStyle->SetTitleYOffset(1.22);
  //gStyle->SetTitleFontSize(0.03); // KYO
  gStyle->SetTitleSize(0.05, "XYZ");
	

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
	
	// read our RFB_pt graph
	///////////////////////////////////////
//	TFile *inFile_pr = new TFile("/afs/cern.ch/work/k/kyolee/public/forLamia/RFB_8rap9pt2gev/RFB_pt_isPrompt1.root");
//	TFile *inFile_np = new TFile("/afs/cern.ch/work/k/kyolee/public/forLamia/RFB_8rap9pt2gev/RFB_pt_isPrompt0.root");
	TFile *inFile_pr = new TFile("./RFB_8rap9pt2gev/RFB_pt_isPrompt1.root");
	TFile *inFile_np = new TFile("./RFB_8rap9pt2gev/RFB_pt_isPrompt0.root");
	TGraphAsymmErrors* gRFB_pr_sys_0 = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_sys_0"); 	
	TGraphAsymmErrors* gRFB_pr_0 = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_0"); 	
	TGraphAsymmErrors* gRFB_np_sys_0 = (TGraphAsymmErrors*)inFile_np->Get("gRFB_sys_0"); 	
	TGraphAsymmErrors* gRFB_np_0 = (TGraphAsymmErrors*)inFile_np->Get("gRFB_0"); 	
	gRFB_pr_sys_0->SetName("gRFB_pr_sys_0");
	gRFB_pr_0->SetName("gRFB_pr_0");
	gRFB_np_sys_0->SetName("gRFB_np_sys_0");
	gRFB_np_0->SetName("gRFB_np_0");
	cout << "gRFB_pr_sys_0 = " << gRFB_pr_sys_0 << endl;
	cout << "gRFB_pr_0 = " << gRFB_pr_0 << endl;
	cout << "gRFB_np_sys_0 = " << gRFB_np_sys_0 << endl;
	cout << "gRFB_np_0 = " << gRFB_np_0 << endl;

	TLatex* latex = new TLatex();
	latex->SetNDC();
	//latex->SetTextAlign(12);
	latex->SetTextAlign(32);
	latex->SetTextSize(0.04);
	
	//globtex box for beam, rapidity, pT info
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	//globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);


	///////////////////////////////////////////////////
	///////////////////// LHCb ////////////////////////
	///////////////////////////////////////////////////
	Double_t lhcb_px[] = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.75, 11.25};
	Double_t lhcb_ex[] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
	Double_t lhcb_py_pr[] = {0.62, 0.58, 0.60, 0.66, 0.75, 0.75, 0.77, 0.83, 0.67};
	Double_t lhcb_py_np[] = {0.84, 0.97, 0.85, 0.96, 0.89, 0.77, 0.91, 1.08, 0.74};

	Double_t lhcb_ey_pr[] = {0.06, 0.04, 0.04, 0.04, 0.06, 0.07, 0.08, 0.11, 0.10};
	Double_t lhcb_ey_np[] = {0.18, 0.15, 0.12, 0.14, 0.16, 0.16, 0.22, 0.27, 0.19};
	Double_t lhcb_exsys[] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};
	Double_t lhcb_eysys_pr[] = {0.04, 0.04, 0.04, 0.04, 0.05, 0.05, 0.05, 0.06, 0.05};
	Double_t lhcb_eysys_np[] = {0.07, 0.16, 0.15, 0.12, 0.14, 0.06, 0.15, 0.11, 0.06};
	
	const Int_t nBin_lhcb = sizeof(lhcb_px)/sizeof(Double_t);
	cout << "nBin_lhcb : " << nBin_lhcb << endl; 

	///////////////////////////////////////////////////
	///////////////////// ALICE ////////////////////////
	///////////////////////////////////////////////////

	//	Double_t alice_px[] = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 8, 10, 13};
	//bins shifted by 0.2
	Double_t alice_px[] = {0.7, 1.7, 2.7, 3.7, 4.7, 5.7, 6.7, 8.2, 10.2, 13.2};
	Double_t alice_ex[] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
	Double_t alice_exsys[] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};
	Double_t alice_RFB[] = {0.62, 0.57, 0.57, 0.64, 0.69, 0.76, 0.81, 0.69, 0.82, 0.95};
	Double_t alice_ey_stat[] = {0.03, 0.02, 0.02, 0.03, 0.04, 0.05, 0.06, 0.05, 0.12, 0.24};
	Double_t alice_ey_sys[] = {0.06, 0.05, 0.05, 0.06, 0.06, 0.07, 0.07, 0.09, 0.08, 0.13};


	const Int_t nBin_alice = sizeof(alice_px)/sizeof(Double_t);
	cout << "nBin_alice : " << nBin_alice << endl; 

	///////////////////////////////////////////////////
	///////////////////// ATLAS ////////////////////////
	///////////////////////////////////////////////////

	//x point plotted at the middle of the bin
	Double_t atlas_px[] = { 8.75, 10.5, 12.75, 17.0, 25.0 };
	Double_t atlas_ex[] = {0., 0., 0., 0., 0.};
	Double_t atlas_exsys[] = {0.2, 0.2, 0.2, 0.2, 0.2};
	Double_t atlas_RFB_pr[] = {0.98, 0.92, 0.95, 1.01, 0.80};
	Double_t atlas_RFB_np[] = {0.81, 1.01, 0.90, 0.98, 1.04};

	Double_t atlas_ey_stat_pr[] = {0.04, 0.03, 0.03, 0.04, 0.07};
	Double_t atlas_ey_stat_np[] = {0.07, 0.05, 0.05, 0.05, 0.09};

	Double_t atlas_ey_sys_pr[] = {0.011, 0.09, 0.09, 0.07, 0.05};
	Double_t atlas_ey_sys_np[] = {0.09, 0.09, 0.08, 0.07, 0.07 };

	const Int_t nBin_atlas = sizeof(atlas_px)/sizeof(Double_t);
	cout << "nBin_atlas : " << nBin_atlas << endl; 


	////////////////////
	////// Draw Plots
	////////////////////

	//TLegend *legUR = new TLegend(0.40, 0.75, 0.86, 0.92); //upper left
	TLegend *legUR = new TLegend(0.44, 0.71, 0.86, 0.85); //upper left
	SetLegendStyle(legUR);
	//TLegend *legUL = new TLegend(0.19, 0.73, 0.40, 0.92); //upper left
	TLegend *legUL = new TLegend(0.165, 0.73, 0.40, 0.91); //upper left
	SetLegendStyle(legUL);
	legUR->SetTextSize(0.037);
	legUL->SetTextSize(0.037);
  legUR->SetTextFont(42);
  legUL->SetTextFont(42);

	TBox * globalbox = new TBox(0.5, 6.4, 1.5, 13.6);
	//globalbox->SetFillColor(kYellow);
	
	/// LHCb /////
	TGraphAsymmErrors * gRFB_lhcb_pr_sys;
	TGraphAsymmErrors * gRFB_lhcb_pr;
	TGraphAsymmErrors * gRFB_lhcb_np_sys;
	TGraphAsymmErrors * gRFB_lhcb_np;

	/// ALICE /////
	TGraphAsymmErrors * gRFB_alice;
	TGraphAsymmErrors *gRFB_alice_sys; 

	/// ATLAS /////
	TGraphAsymmErrors * gRFB_atlas_np;
	TGraphAsymmErrors *gRFB_atlas_sys_np; 

	TGraphAsymmErrors * gRFB_atlas_pr;
	TGraphAsymmErrors *gRFB_atlas_sys_pr; 

	////////////////////////////////////////////////////////////////////////////////////
	// prompt
	//TCanvas *c_pr = new TCanvas("c_pr","", 200, 10, 800, 600);
	TCanvas *c_pr = new TCanvas("c_pr","", 200, 10, 600, 600);

	c_pr->cd();
	gPad->SetLogy(0);

	TH1D *dummy = new TH1D("","",50,0.,400.);
	dummy->SetMinimum(0.5);
	dummy->SetMaximum(1.4);
	dummy->GetXaxis()->CenterTitle();
	dummy->GetXaxis()->SetLimits(0.0, 30.0);
	dummy->SetXTitle("p_{T} [GeV/c]");
	dummy->SetYTitle("R_{FB}");
	dummy->Draw();

	// 2) lhcb
	gRFB_lhcb_pr_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_exsys, lhcb_exsys, lhcb_eysys_pr, lhcb_eysys_pr);	
	//	gRFB_lhcb_pr_sys->SetFillColor(kBlue-9);
	gRFB_lhcb_pr_sys->SetFillColor(kGray);
	gRFB_lhcb_pr_sys->SetFillStyle(3001);
	gRFB_lhcb_pr_sys->Draw("2");

	gRFB_lhcb_pr = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_ex, lhcb_ex, lhcb_ey_pr, lhcb_ey_pr);	
	SetGraphStyle(gRFB_lhcb_pr,2,10);
	gRFB_lhcb_pr->SetLineColor(1);
	gRFB_lhcb_pr->SetMarkerColor(1);
	gRFB_lhcb_pr->SetMarkerSize(1.5);
	//gRFB_lhcb_pr->SetMarkerStyle(20);
	gRFB_lhcb_pr->Draw("PSAME");

	//3) ALICE
	gRFB_alice = new TGraphAsymmErrors(nBin_alice, alice_px, alice_RFB, alice_ex, alice_ex, alice_ey_stat, alice_ey_stat);	
	gRFB_alice->SetMarkerSize(1.5);
	gRFB_alice->SetLineColor(1);
	gRFB_alice->SetMarkerColor(1);
	gRFB_alice->SetMarkerStyle(kOpenTriangleUp);



	gRFB_alice_sys = new TGraphAsymmErrors(nBin_alice, alice_px, alice_RFB, alice_exsys, alice_exsys, alice_ey_sys, alice_ey_sys);	
	//	gRFB_alice_sys->SetFillColor(kOrange-4);
	gRFB_alice_sys->SetFillColor(kGray);
	gRFB_alice_sys->SetFillStyle(3001);
	gRFB_alice_sys->SetMarkerSize(1.5);
	//gRFB_alice_sys->SetMarkerColor(kOrange+1);
	//gRFB_alice_sys->SetLineColor(kOrange+1);
	gRFB_alice_sys->SetMarkerColor(1);
	gRFB_alice_sys->SetLineColor(1);
	gRFB_alice_sys->SetLineWidth(1);
	gRFB_alice_sys->Draw("2SAME");
	gRFB_alice->Draw("PSAME");
	
	//3) ATLAS
	gRFB_atlas_pr = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_pr, atlas_ex, atlas_ex, atlas_ey_stat_pr, atlas_ey_stat_pr);	
	gRFB_atlas_pr->SetMarkerSize(2.0);
	gRFB_atlas_pr->SetMarkerStyle(27);
// 	gRFB_atlas_pr->SetMarkerColor(6);
// 	gRFB_atlas_pr->SetLineColor(6);
	gRFB_atlas_pr->SetMarkerColor(1);
	gRFB_atlas_pr->SetLineColor(1);

	gRFB_atlas_sys_pr = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_pr, atlas_exsys, atlas_exsys, atlas_ey_sys_pr, atlas_ey_sys_pr);	
	//	gRFB_atlas_sys_pr->SetFillColor(kMagenta-10);
	gRFB_atlas_sys_pr->SetFillColor(kGray);
	gRFB_atlas_sys_pr->SetFillStyle(3001);
	//gRFB_atlas_sys_pr->SetFillStyle(3003);
	gRFB_atlas_sys_pr->SetMarkerSize(0);
	gRFB_atlas_sys_pr->SetLineColor(1);
	gRFB_atlas_sys_pr->SetLineWidth(1);
	gRFB_atlas_sys_pr->Draw("2SAME");
	gRFB_atlas_pr->Draw("PSAME");

	// 1) CMS
	gRFB_pr_sys_0->SetLineColor(kGreen-2);
	gRFB_pr_sys_0->SetFillColor(kGreen-7);
	gRFB_pr_sys_0->SetFillStyle(3001);
	gRFB_pr_0->SetMarkerSize(1.5);
	gRFB_pr_0->SetMarkerColor(kGreen-2);
	//gRFB_pr_0->SetMarkerStyle(25);
	gRFB_pr_0->SetMarkerStyle(kFullSquare);
	gRFB_pr_0->SetLineColor(kGreen-2);
	gRFB_pr_sys_0->Draw("2SAME");
	gRFB_pr_0->Draw("PSAME");

	dashedLine(0.0,1.0,30.0,1.0,1,1);
	//	TLegend *leg = new TLegend(0.3517588,0.1660839,0.5527638,0.4265734); //bottom left
	//SetLegendStyle(leg);
	//legUL->SetHeader("Prompt J/#psi");
	//legUL->AddEntry(gRFB_pr_0,"CMS Preliminary: 1.5 < |y_{CM}| < 1.93","lp");
	legUL->AddEntry(gRFB_pr_0,"CMS : 1.5 < |y_{CM}| < 1.93","lp");
	legUL->AddEntry(gRFB_atlas_pr,"ATLAS: |y_{CM}| < 1.94","lp");
	legUL->AddEntry(gRFB_lhcb_pr,"LHCb: 2.5 <|y_{CM}| < 4.0 ","lp");
	legUL->AddEntry(gRFB_alice,"ALICE (inclusive  J/#psi): 2.96 <|y_{CM}| < 3.53 ","lp");
	legUL->Draw("SAME");
	
	globtex->SetTextSize(0.045);
	globtex->SetTextFont(62);
	globtex->DrawLatex(0.88, 0.88, "Prompt J/#psi");
	
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.89, 0.25, "Preliminary");
	latex->DrawLatex(0.89, 0.19, Form("%s",beamstring.c_str()));

	c_pr->Update();
	c_pr->SaveAs("compOtherExp/comp_RFB_pt_LHCb_ALICE_ATLAS_pr.pdf");
	c_pr->SaveAs("compOtherExp/comp_RFB_pt_LHCb_ALICE_ATLAS_pr.png");
	legUL->Clear();
	//latex->SetTextSize(0.05);
	//latex->DrawLatex(0.54, 0.35, cmsstring.c_str());

	//latex->SetTextSize(0.04);
	//latex->DrawLatex(0.56, 0.20, lumistring.c_str());

	////////////////////////////////////////////////////////////////////////////////////
	// non-prompt
	
	//TCanvas *c_np = new TCanvas("c_np","", 200, 10, 800, 600);
	TCanvas *c_np = new TCanvas("c_np","", 200, 10, 600, 600);
	c_np->cd();
	gPad->SetLogy(0);

	TH1D *dummynp = new TH1D("","",50,0.,400.);
	dummynp->SetMinimum(0.5);
	dummynp->SetMaximum(1.4);
	dummynp->GetXaxis()->CenterTitle();
	dummynp->GetXaxis()->SetLimits(0.0, 30.0);
	dummynp->SetXTitle("p_{T} [GeV/c]");
	dummynp->SetYTitle("R_{FB}");
	dummynp->Draw();
	// 1) CMS
	gRFB_np_sys_0->SetLineColor(kGreen-2);
	gRFB_np_sys_0->SetFillColor(kGreen-7);
	gRFB_np_sys_0->SetFillStyle(3001);
	gRFB_np_0->SetMarkerSize(1.5);
	gRFB_np_0->SetMarkerColor(kGreen-2);
	//gRFB_np_0->SetMarkerStyle(25);
	gRFB_np_0->SetMarkerStyle(kFullSquare);
	gRFB_np_0->SetLineColor(kGreen-2);
	gRFB_np_sys_0->Draw("2SAME");
	gRFB_np_0->Draw("PSAME");

	// 2) lhcb

	gRFB_lhcb_np_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_exsys, lhcb_exsys, lhcb_eysys_np, lhcb_eysys_np);	
	//gRFB_lhcb_np_sys->SetFillColor(kBlue-9);
	gRFB_lhcb_np_sys->SetFillColor(kGray);
	gRFB_lhcb_np_sys->SetFillStyle(3001);
	gRFB_lhcb_np_sys->Draw("2");

	
	gRFB_lhcb_np = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_ex, lhcb_ex, lhcb_ey_np, lhcb_ey_np);	
	SetGraphStyle(gRFB_lhcb_np,2,10);
	gRFB_lhcb_np->SetMarkerSize(1.5);
	//gRFB_lhcb_np->SetMarkerStyle(20);
	gRFB_lhcb_np->SetLineColor(1);
	gRFB_lhcb_np->SetMarkerColor(1);
	gRFB_lhcb_np->Draw("PSAME");

	// 2) ATLAS
	gRFB_atlas_np = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_np, atlas_ex, atlas_ex, atlas_ey_stat_np, atlas_ey_stat_np);	
 	gRFB_atlas_np->SetMarkerSize(2.0);
	gRFB_atlas_np->SetMarkerStyle(27);
	gRFB_atlas_np->SetMarkerColor(1);
	gRFB_atlas_np->SetLineColor(1);


 	gRFB_atlas_sys_np = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_np, atlas_exsys, atlas_exsys, atlas_ey_sys_np, atlas_ey_sys_np);	
	//gRFB_atlas_sys_np->SetFillColor(kMagenta-10);
	gRFB_atlas_sys_np->SetFillColor(kGray);
	gRFB_atlas_sys_np->SetFillStyle(3001);
	gRFB_atlas_sys_np->SetMarkerSize(0);
	gRFB_atlas_sys_np->SetLineColor(1);
	gRFB_atlas_sys_np->SetLineWidth(1);
	gRFB_atlas_sys_np->Draw("2SAME");
	gRFB_atlas_np->Draw("PSAME");


	dashedLine(0.0,1.0,30.0,1.0,1,1);
	
	//legUR -> SetHeader("Non-prompt J/#psi");
	legUR -> AddEntry(gRFB_np_0,"CMS : 1.5 < |y_{CM}| < 1.93","lp");
	legUR -> AddEntry(gRFB_atlas_np,"ATLAS: |y_{CM}| < 1.94","lp");
	legUR -> AddEntry(gRFB_lhcb_np,"LHCb: 2.5 <|y_{CM}| < 4.0 ","lp");
	legUR -> Draw();

	globtex->SetTextSize(0.045);
	globtex->SetTextFont(62);
	globtex->DrawLatex(0.88, 0.88, "Non-prompt J/#psi");
	
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.89, 0.25, "Preliminary");
	latex->DrawLatex(0.89, 0.19, Form("%s",beamstring.c_str()));

	c_np->Update();
	c_np->SaveAs("compOtherExp/comp_RFB_pt_LHCb_ATLAS_np.pdf");
	c_np->SaveAs("compOtherExp/comp_RFB_pt_LHCb_ATLAS_np.png");
	legUR->Clear();
	
	return 0;
}

void formRapArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < y_{CM} < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < y_{CM} < %.2f", binmin, binmax);
	}
}

void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < |y_{CM}| < %.2f", binmin, binmax);
	}
}

void formPtArr(Double_t binmin, Double_t binmax, string* arr) {
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

