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

int comp_RFB_rap_LHCb_ALICE_ATLAS(Int_t runCode=0)
{

  //gROOT->LoadMacro("tdrstyle.C");
  //setTDRStyle();
  gROOT->LoadMacro("tdrstyle_kyo.C");
  
  gROOT->LoadMacro("CMS_lumi.C");
  //	gROOT->Macro("./JpsiStyleForFinalResult.C");
	
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
	
	// read our RFB_rap graph
 	//TFile *inFile_pr = new TFile("/afs/cern.ch/work/k/kyolee/public/forLamia/RFB_8rap9pt2gev/RFB_rap_isPrompt1.root");
	//TFile *inFile_np = new TFile("/afs/cern.ch/work/k/kyolee/public/forLamia/RFB_8rap9pt2gev/RFB_rap_isPrompt0.root");
 	TFile *inFile_pr = new TFile("./RFB_8rap9pt2gev/RFB_rap_isPrompt1.root");
	TFile *inFile_np = new TFile("./RFB_8rap9pt2gev/RFB_rap_isPrompt0.root");
       
	TGraphAsymmErrors* gRFB_pr_sys_lowpt = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_sys_lowpt"); 	
	TGraphAsymmErrors* gRFB_pr_lowpt = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_lowpt"); 	
	TGraphAsymmErrors* gRFB_pr_sys_highpt = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_sys_highpt"); 	
	TGraphAsymmErrors* gRFB_pr_highpt = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_highpt"); 	
	TGraphAsymmErrors* gRFB_np_sys_lowpt = (TGraphAsymmErrors*)inFile_np->Get("gRFB_sys_lowpt"); 	
	TGraphAsymmErrors* gRFB_np_lowpt = (TGraphAsymmErrors*)inFile_np->Get("gRFB_lowpt"); 	
	TGraphAsymmErrors* gRFB_np_sys_highpt = (TGraphAsymmErrors*)inFile_np->Get("gRFB_sys_highpt"); 	
	TGraphAsymmErrors* gRFB_np_highpt = (TGraphAsymmErrors*)inFile_np->Get("gRFB_highpt"); 	
	gRFB_pr_sys_lowpt->SetName("gRFB_pr_sys_lowpt");
	gRFB_pr_lowpt->SetName("gRFB_pr_lowpt");
	gRFB_pr_sys_highpt->SetName("gRFB_pr_sys_highpt");
	gRFB_pr_highpt->SetName("gRFB_pr_highpt");
	gRFB_np_sys_lowpt->SetName("gRFB_np_sys_lowpt");
	gRFB_np_lowpt->SetName("gRFB_np_lowpt");
	gRFB_np_sys_highpt->SetName("gRFB_np_sys_highpt");
	gRFB_np_highpt->SetName("gRFB_np_highpt");
	cout << "hello"<<"gRFB_pr_sys_lowpt = " << gRFB_pr_sys_lowpt << endl;
	cout << "gRFB_pr_lowpt = " << gRFB_pr_lowpt << endl;
	cout << "gRFB_pr_sys_highpt = " << gRFB_pr_sys_highpt << endl;
	cout << "gRFB_pr_highpt = " << gRFB_pr_highpt << endl;
	cout << "gRFB_np_sys_lowpt = " << gRFB_np_sys_lowpt << endl;
	cout << "gRFB_np_lowpt = " << gRFB_np_lowpt << endl;
	cout << "gRFB_np_sys_highpt = " << gRFB_np_sys_highpt << endl;
	cout << "gRFB_np_highpt = " << gRFB_np_highpt << endl;

	///////////////////////////////////////////////////
	///////////////////// LHCb ////////////////////////
	///////////////////////////////////////////////////
	Double_t lhcb_px[] = {2.75, 3.25, 3.75};
	//Double_t lhcb_px_binW[] = {0.25, 0.25, 0.25};
	Double_t lhcb_ex[] = {0., 0., 0.};
	Double_t lhcb_py_pr[] = {0.72, 0.62, 0.64};
	Double_t lhcb_py_np[] = {0.92, 0.84, 0.72};

	Double_t lhcb_ey_pr[] = { 0.05, 0.03, 0.04};
	Double_t lhcb_ey_np[] = {0.11, 0.08, 0.09};
	Double_t lhcb_exsys[] = {0.04, 0.04, 0.04};
	Double_t lhcb_eysys_pr[] = {0.07, 0.04, 0.04};
	Double_t lhcb_eysys_np[] = {0.09, 0.08, 0.07};
	
	const Int_t nBin_lhcb = sizeof(lhcb_px)/sizeof(Double_t);
	cout << "nBin_lhcb : " << nBin_lhcb << endl; 



	///////////////////////////////////////////////////
	///////////////////// ALICE ////////////////////////
	//////////////////////////////////////////////////

	Double_t alice_px[] = {3.0549999999999997, 3.245, 3.4349999999999996};
	Double_t alice_ex[] = {0., 0., 0.};
	Double_t alice_exsys[] = {0.04, 0.04, 0.04};

	Double_t alice_RFB[] = {0.58, 0.6, 0.61};
	Double_t alice_eysys_stat[] = {0.03, 0.02, 0.03};
	Double_t alice_eysys[] = {0.06, 0.05, 0.06};
	const Int_t nBin_alice = sizeof(alice_px)/sizeof(Double_t);
	cout << "nBin_alice : " << nBin_alice << endl; 


	///////////////////////////////////////////////////
	///////////////////// ATLAS ////////////////////////
	///////////////////////////////////////////////////

	Double_t atlas_px[] = {0.325, 1.125, 1.62};
	Double_t atlas_ex[] = {0., 0., 0.};
	Double_t atlas_exsys[] = {0.04, 0.04, 0.04};

	Double_t atlas_RFB_pr[] = {1.10, 0.90, 0.92};
	Double_t atlas_eysys_stat_pr[] = {0.04, 0.03, 0.04};
	Double_t atlas_eysys_pr[] = {0.13, 0.11, 0.18};

	Double_t atlas_RFB_np[] = {0.96, 0.95, 0.80};
	Double_t atlas_eysys_stat_np[] = {0.06, 0.06, 0.08};
	Double_t atlas_eysys_np[] = {0.11, 0.12, 0.15};

	const Int_t nBin_atlas = sizeof(atlas_px)/sizeof(Double_t);
	cout << "nBin_atlas : " << nBin_atlas << endl; 


	////////////////////
	////// Draw Plots
	////////////////////

	//TLegend *legUR = new TLegend(0.42, 0.65, 0.86, 0.92);
	TLegend *legUR = new TLegend(0.435, 0.62, 0.86, 0.855); 
	SetLegendStyle(legUR);
	TLegend *legUR02 = new TLegend(0.435, 0.625, 0.86, 0.855); // for prompt
	SetLegendStyle(legUR02);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	//latex->SetTextAlign(32);
	latex->SetTextSize(0.04);
	
	//globtex box for beam, rapidity, pT info
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	//globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);



	TBox * globalbox = new TBox(0.5, 6.4, 1.5, 13.6);
	globalbox->SetFillColor(kYellow);

	/// LHCb /////
	TGraphAsymmErrors * gRFB_lhcb_pr_sys;
	TGraphAsymmErrors * gRFB_lhcb_pr;
	TGraphAsymmErrors * gRFB_lhcb_np_sys;
	TGraphAsymmErrors * gRFB_lhcb_np;

	/// ALICE /////
	TGraphAsymmErrors * gRFB_alice;
	TGraphAsymmErrors * gRFB_alice_sys; 

	/// ATLAS /////
	TGraphAsymmErrors * gRFB_atlas_pr;
	TGraphAsymmErrors * gRFB_atlas_sys_pr; 	
	TGraphAsymmErrors * gRFB_atlas_np;
	TGraphAsymmErrors * gRFB_atlas_sys_np; 	
////////////////////////////////////////////////////////////////////////////////////
	// prompt
	TCanvas *c_pr = new TCanvas("c_pr","", 200, 10, 600, 600);
	c_pr->cd();
	gPad->SetLogy(0);
	// 1) lowpt
	gRFB_pr_sys_lowpt->SetTitle("");
	gRFB_pr_sys_lowpt->GetXaxis()->SetTitle("|y_{CM}|");
	gRFB_pr_sys_lowpt->GetXaxis()->CenterTitle();
	gRFB_pr_sys_lowpt->GetXaxis()->SetLimits(0.0 ,4.5);
	gRFB_pr_sys_lowpt->GetYaxis()->SetTitle("R_{FB}");
	gRFB_pr_sys_lowpt->SetMinimum(0.5);
	gRFB_pr_sys_lowpt->SetMaximum(1.4);
	gRFB_pr_sys_lowpt->SetFillColor(kRed-9);
	gRFB_pr_sys_lowpt->SetFillStyle(3001);
	gRFB_pr_sys_lowpt->Draw("A2");
	
	// 2) highpt
	gRFB_pr_sys_highpt->SetFillColor(kTeal+7);
	gRFB_pr_sys_highpt->SetFillStyle(3001);
	gRFB_pr_sys_highpt->Draw("2");

	// 3) lhcb
	gRFB_lhcb_pr_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_exsys, lhcb_exsys, lhcb_eysys_pr, lhcb_eysys_pr);	
	//gRFB_lhcb_pr_sys->SetFillColor(kBlue-9);
	gRFB_lhcb_pr_sys->SetFillColor(kGray);
	gRFB_lhcb_pr_sys->SetFillStyle(3001);
	gRFB_lhcb_pr_sys->Draw("2");



	SetGraphStyle(gRFB_pr_lowpt,1,3);
	gRFB_pr_lowpt->Draw("P");
	
	SetGraphStyle(gRFB_pr_highpt,0,5);
	gRFB_pr_highpt->SetMarkerSize(2.0);
	gRFB_pr_highpt->Draw("P");

	gRFB_lhcb_pr = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_ex, lhcb_ex, lhcb_ey_pr, lhcb_ey_pr);	
	SetGraphStyle(gRFB_lhcb_pr,1,10);
	gRFB_lhcb_pr->SetMarkerSize(1.5);
	gRFB_lhcb_pr->SetMarkerStyle(24);
	gRFB_lhcb_pr->SetMarkerColor(1);
	gRFB_lhcb_pr->SetLineColor(1);
	gRFB_lhcb_pr->Draw("PSAME");

	dashedLine(0.0,1.0,4.5,1.0,1,1);



	//4) ALICE
	gRFB_alice = new TGraphAsymmErrors(nBin_alice, alice_px, alice_RFB, alice_ex, alice_ex, alice_eysys_stat, alice_eysys_stat);	
	gRFB_alice->SetMarkerSize(1.5);
// 	gRFB_alice->SetLineColor(kOrange+7);
// 	gRFB_alice->SetMarkerColor(kOrange+7);
	gRFB_alice->SetLineColor(1);
	gRFB_alice->SetMarkerColor(1);
	//gRFB_alice->SetMarkerStyle(20);
	gRFB_alice->SetMarkerStyle(kOpenTriangleUp);

	gRFB_alice_sys = new TGraphAsymmErrors(nBin_alice, alice_px, alice_RFB, alice_exsys, alice_exsys, alice_eysys, alice_eysys);	
	//gRFB_alice_sys->SetFillColor(kOrange-4);
	gRFB_alice_sys->SetFillColor(kGray);
	gRFB_alice_sys->SetFillStyle(3001);
	gRFB_alice_sys->SetMarkerSize(2.0);
// 	gRFB_alice_sys->SetMarkerColor(kOrange+1);
// 	gRFB_alice_sys->SetLineColor(kOrange+1);
	gRFB_alice_sys->SetMarkerColor(1);
	gRFB_alice_sys->SetLineColor(1);
	gRFB_alice_sys->SetLineWidth(1);
	gRFB_alice_sys->Draw("2SAME");
	gRFB_alice->Draw("PSAME");


	//4) ATLAS
	gRFB_atlas_pr = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_pr, atlas_ex, atlas_ex, atlas_eysys_stat_pr, atlas_eysys_stat_pr);	
	gRFB_atlas_pr->SetMarkerSize(2.0);
	gRFB_atlas_pr->SetMarkerStyle(27);
	//gRFB_atlas_pr->SetMarkerColor(6);
	gRFB_atlas_pr->SetMarkerColor(1);
	gRFB_atlas_pr->SetLineColor(1);

	gRFB_atlas_sys_pr = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_pr, atlas_exsys, atlas_exsys, atlas_eysys_pr, atlas_eysys_pr);	
	//gRFB_atlas_sys_pr->SetFillColor(kMagenta-10);
	gRFB_atlas_sys_pr->SetFillColor(kGray);
	gRFB_atlas_sys_pr->SetFillStyle(3001);
	gRFB_atlas_sys_pr->SetMarkerSize(0);
	gRFB_atlas_sys_pr->SetLineColor(1);
	gRFB_atlas_sys_pr->SetLineWidth(1);
	gRFB_atlas_sys_pr->Draw("2SAME");
	gRFB_atlas_pr->Draw("PSAME");

//	legUR02 -> SetHeader("Prompt J/#psi");
	legUR02 -> AddEntry(gRFB_pr_lowpt,"CMS : 6.5 < p_{T} < 10 GeV","lp");
	legUR02 -> AddEntry(gRFB_pr_highpt,"CMS : 10 < p_{T} < 30 GeV","lp");
	legUR02 -> AddEntry(gRFB_atlas_pr,"ATLAS : 8 < p_{T} < 30 GeV","lp");
	legUR02 -> AddEntry(gRFB_lhcb_pr,"LHCb : p_{T} < 14 GeV","lp");
	//legUR02 -> AddEntry(gRFB_alice,"ALICE (inclusive  J/#psi) : p_{T} < 14 GeV","lp");
	legUR02 -> AddEntry(gRFB_alice,"ALICE (inclusive  J/#psi)","lp");
	globtex->SetTextSize(0.037);
	globtex->SetTextFont(42);
	globtex->DrawLatex(0.83, 0.60, ": p_{T} < 14 GeV");
	legUR02 -> Draw();
	globtex->SetTextSize(0.045);
	globtex->SetTextFont(62);
//	globtex->DrawLatex(0.88, 0.86, "Prompt J/#psi");
	globtex->DrawLatex(0.88, 0.88, "Prompt J/#psi");
	
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.19, 0.25, "Preliminary");
	latex->DrawLatex(0.19, 0.19, Form("%s",beamstring.c_str()));

	c_pr->Update();
	c_pr->SaveAs("compOtherExp/comp_RFB_rap_LHCb_ALICE_ATLAS_pr.pdf");
	c_pr->SaveAs("compOtherExp/comp_RFB_rap_LHCb_ALICE_ATLAS_pr.png");
	legUR02->Clear();
	//c_pr->Clear();





	////////////////////////////////////////////////////////////////////////////////////
	// non-prompt
	TCanvas *c_np = new TCanvas("c_np","", 200, 10, 600, 600);
	c_np->cd();
	gPad->SetLogy(0);
	// 1) lowpt
	gRFB_np_sys_lowpt->SetTitle("");
	gRFB_np_sys_lowpt->GetXaxis()->SetTitle("|y_{CM}|");
	gRFB_np_sys_lowpt->GetXaxis()->CenterTitle();
	gRFB_np_sys_lowpt->GetXaxis()->SetLimits(0.0,4.5);
	gRFB_np_sys_lowpt->GetYaxis()->SetTitle("R_{FB}");
	gRFB_np_sys_lowpt->SetMinimum(0.5);
	gRFB_np_sys_lowpt->SetMaximum(1.4);
	gRFB_np_sys_lowpt->SetFillColor(kRed-9);
	gRFB_np_sys_lowpt->SetFillStyle(3001);
	gRFB_np_sys_lowpt->Draw("A2");
	
	// 2) highpt
	gRFB_np_sys_highpt->SetFillColor(kTeal+7);
	gRFB_np_sys_lowpt->SetFillStyle(3001);

	gRFB_np_sys_highpt->Draw("2");

	// 3) lhcb
	gRFB_lhcb_np_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_exsys, lhcb_exsys, lhcb_eysys_np, lhcb_eysys_np);	
	//gRFB_lhcb_np_sys->SetFillColor(kAzure-9);
	gRFB_lhcb_np_sys->SetFillColor(kGray);
	gRFB_lhcb_np_sys->SetFillStyle(3001);
	gRFB_lhcb_np_sys->SetMarkerColor(1);
	gRFB_lhcb_np_sys->SetLineColor(1);
	gRFB_lhcb_np_sys->Draw("2");

	SetGraphStyle(gRFB_np_lowpt,1,3);
	gRFB_np_lowpt->Draw("P");
	
	SetGraphStyle(gRFB_np_highpt,0,5);
	gRFB_np_highpt->SetMarkerSize(2.0);
	gRFB_np_highpt->Draw("P");

	gRFB_lhcb_np = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_ex, lhcb_ex, lhcb_ey_np, lhcb_ey_np);	
	SetGraphStyle(gRFB_lhcb_np,1,10);
	gRFB_lhcb_np->SetMarkerColor(1);
	gRFB_lhcb_np->SetLineColor(1);
	gRFB_lhcb_np->Draw("P");

	//4) ATLAS
	gRFB_atlas_np = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_np, atlas_ex, atlas_ex, atlas_eysys_stat_np, atlas_eysys_stat_np);	
	gRFB_atlas_np->SetMarkerSize(2.0);
	gRFB_atlas_np->SetMarkerStyle(27);
	//gRFB_atlas_np->SetMarkerColor(6);
	gRFB_atlas_np->SetMarkerColor(1);
	gRFB_atlas_np->SetLineColor(1);

	gRFB_atlas_sys_np = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_np, atlas_exsys, atlas_exsys, atlas_eysys_np, atlas_eysys_np);
	//	gRFB_atlas_sys_np->SetFillColor(kMagenta-10);
	gRFB_atlas_sys_np->SetFillColor(kGray);
	gRFB_atlas_sys_np->SetFillStyle(3001);
	gRFB_atlas_sys_np->SetMarkerSize(0);
	gRFB_atlas_sys_np->SetLineColor(1);
	gRFB_atlas_sys_np->SetLineWidth(1);
	gRFB_atlas_sys_np->Draw("2SAME");
	gRFB_atlas_np->Draw("PSAME");	

	dashedLine(0.0,1.0,4.5,1.0,1,1);

//	legUR -> SetHeader("Non-prompt J/#psi");
	legUR -> AddEntry(gRFB_np_lowpt,"CMS : 6.5 < p_{T} < 10 GeV","lp");
	legUR -> AddEntry(gRFB_np_highpt,"CMS : 10 < p_{T} < 30 GeV","lp");
	legUR -> AddEntry(gRFB_atlas_np,"ATLAS : 8 < p_{T} < 30 GeV","lp");
	legUR -> AddEntry(gRFB_lhcb_np,"LHCb : p_{T} < 14 GeV","lp");
	legUR -> Draw();
	globtex->SetTextSize(0.045);
	globtex->SetTextFont(62);
	globtex->DrawLatex(0.88, 0.88, "Non-prompt J/#psi");
	
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.19, 0.25, "Preliminary");
	latex->DrawLatex(0.19, 0.19, Form("%s",beamstring.c_str()));

	c_np->Update();
	c_np->SaveAs("compOtherExp/comp_RFB_rap_LHCb_ATLAS_np.pdf");
	c_np->SaveAs("compOtherExp/comp_RFB_rap_LHCb_ATLAS_np.png");
	//legUR->Clear();
	//c_np->Clear();

	return 0;
}

void formRapArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
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

