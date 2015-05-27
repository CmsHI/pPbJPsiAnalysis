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

int comp_RFB_pt_LHCb(Int_t runCode=0)
{
	gROOT->Macro("./JpsiStyleForFinalResult.C");

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
	TFile *inFile_pr = new TFile("RFB_8rap9pt2gev/RFB_pt_isPrompt1.root");
	TFile *inFile_np = new TFile("RFB_8rap9pt2gev/RFB_pt_isPrompt0.root");
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

	///////////////////////////////////////////////////
	///////////////////// LHCb ////////////////////////
	///////////////////////////////////////////////////
	Double_t lhcb_px[] = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.75, 11.25};
	Double_t lhcb_ex[] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
	Double_t lhcb_py_pr[] = {0.62, 0.58, 0.60, 0.66, 0.75, 0.75, 0.77, 0.83, 0.67};
	Double_t lhcb_py_np[] = {0.84, 0.97, 0.85, 0.96, 0.89, 0.77, 0.91, 1.08, 0.74};

	Double_t lhcb_ey_pr[] = {0.06, 0.04, 0.04, 0.04, 0.06, 0.07, 0.08, 0.11, 0.10};
	Double_t lhcb_ey_np[] = {0.18, 0.15, 0.12, 0.14, 0.16, 0.16, 0.22, 0.27, 0.19};
	//Double_t lhcb_exsys[] = {0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4};
	Double_t lhcb_exsys[] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};
	Double_t lhcb_eysys_pr[] = {0.04, 0.04, 0.04, 0.04, 0.05, 0.05, 0.05, 0.06, 0.05};
	Double_t lhcb_eysys_np[] = {0.07, 0.16, 0.15, 0.12, 0.14, 0.06, 0.15, 0.11, 0.06};
	
	const Int_t nBin_lhcb = sizeof(lhcb_px)/sizeof(Double_t);
	cout << "nBin_lhcb : " << nBin_lhcb << endl; 

	////////////////////
	////// Draw Plots
	////////////////////

	//TLegend *legUR = new TLegend(0.52, 0.55, 0.85, 0.92); //upper left
	TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
	SetLegendStyle(legUR);
	TLegend *legUL = new TLegend(0.20, 0.73, 0.40, 0.92); //upper left
	SetLegendStyle(legUL);
	TLegend *legUM = new TLegend(0.40, 0.75, 0.58, 0.90); //upper middle
	SetLegendStyle(legUM);
//	TLegend *legBL = new TLegend(0.20, 0.20, 0.48, 0.37); //bottom left
	TLegend *legBL = new TLegend(0.18, 0.18, 0.48, 0.48); //bottom left
	SetLegendStyle(legBL);
	TLegend *legBM = new TLegend(0.40, 0.20, 0.58, 0.37); //bottom middle
	SetLegendStyle(legBM);
	TLegend *legMR = new TLegend(0.54, 0.34, 0.87, 0.57); //bottom left
	SetLegendStyle(legMR);
	TLegend *legBR = new TLegend(0.54, 0.20, 0.87, 0.37); //bottom left
	SetLegendStyle(legBR);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	TBox * globalbox = new TBox(0.5, 6.4, 1.5, 13.6);
	globalbox->SetFillColor(kYellow);
	
	/// LHCb /////
	TGraphAsymmErrors * gRFB_lhcb_pr_sys;
	TGraphAsymmErrors * gRFB_lhcb_pr;
	TGraphAsymmErrors * gRFB_lhcb_np_sys;
	TGraphAsymmErrors * gRFB_lhcb_np;

	////////////////////////////////////////////////////////////////////////////////////
	// prompt
	TCanvas *c_pr = new TCanvas("c_pr","", 200, 10, 800, 600);
	c_pr->cd();
	gPad->SetLogy(0);
	// 1) CMS
	gRFB_pr_sys_0->SetTitle("");
	gRFB_pr_sys_0->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gRFB_pr_sys_0->GetXaxis()->CenterTitle();
	gRFB_pr_sys_0->GetXaxis()->SetLimits(0.0,20.0);
	gRFB_pr_sys_0->GetYaxis()->SetTitle("R_{FB}");
	gRFB_pr_sys_0->SetMinimum(0.0);
	gRFB_pr_sys_0->SetMaximum(1.4);
	gRFB_pr_sys_0->SetFillColor(kTeal+7);
	gRFB_pr_sys_0->Draw("A2");
	// 2) lhcb
	gRFB_lhcb_pr_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_exsys, lhcb_exsys, lhcb_eysys_pr, lhcb_eysys_pr);	
	gRFB_lhcb_pr_sys->SetFillColor(kAzure-9);
	gRFB_lhcb_pr_sys->Draw("2");

	SetGraphStyle(gRFB_pr_0,0,5);
	gRFB_pr_0->SetMarkerSize(1.9);
	gRFB_pr_0->Draw("P");
	
	gRFB_lhcb_pr = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_ex, lhcb_ex, lhcb_ey_pr, lhcb_ey_pr);	
	//SetGraphStyle(gRFB_lhcb_pr,2,0);
	SetGraphStyle(gRFB_lhcb_pr,2,10);
	gRFB_lhcb_pr->Draw("P");
	
	dashedLine(0.0,1.0,20.0,1.0,1,1);
	
	legBR -> SetHeader("Prompt J/#psi");
	legBR -> AddEntry(gRFB_pr_0,"CMS :1.5 < |y_{CM}| < 1.93","lp");
	legBR -> AddEntry(gRFB_lhcb_pr,"LHCb : 2.5 <|y_{CM}| < 4.0 ","lp");
	legBR -> Draw();
	c_pr->Update();
	c_pr->SaveAs("comparisonLHCb/comp_RFB_pt_LHCb_pr.pdf");
	legBR->Clear();
	//c_pr->Clear();

	////////////////////////////////////////////////////////////////////////////////////
	// non-prompt
	TCanvas *c_np = new TCanvas("c_np","", 200, 10, 800, 600);
	c_np->cd();
	gPad->SetLogy(0);
	// 1) CMS
	gRFB_np_sys_0->SetTitle("");
	gRFB_np_sys_0->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gRFB_np_sys_0->GetXaxis()->CenterTitle();
	gRFB_np_sys_0->GetXaxis()->SetLimits(0.0,20.0);
	gRFB_np_sys_0->GetYaxis()->SetTitle("R_{FB}");
	gRFB_np_sys_0->SetMinimum(0.0);
	gRFB_np_sys_0->SetMaximum(1.4);
	gRFB_np_sys_0->SetFillColor(kTeal+7);
	gRFB_np_sys_0->Draw("A2");
	// 2) lhcb
	gRFB_lhcb_np_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_exsys, lhcb_exsys, lhcb_eysys_np, lhcb_eysys_np);	
	gRFB_lhcb_np_sys->SetFillColor(kAzure-9);
	gRFB_lhcb_np_sys->Draw("2");

	SetGraphStyle(gRFB_np_0,0,5);
	gRFB_np_0->SetMarkerSize(1.9);
	gRFB_np_0->Draw("P");
	
	gRFB_lhcb_np = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_ex, lhcb_ex, lhcb_ey_np, lhcb_ey_np);	
	//SetGraphStyle(gRFB_lhcb_np,2,0);
	SetGraphStyle(gRFB_lhcb_np,2,10);
	gRFB_lhcb_np->Draw("P");
	
	dashedLine(0.0,1.0,20.0,1.0,1,1);
	
	legBR -> SetHeader("Non-prompt J/#psi");
	legBR -> AddEntry(gRFB_np_0,"CMS :1.5 < |y_{CM}| < 1.93","lp");
	legBR -> AddEntry(gRFB_lhcb_np,"LHCb : 2.5 <|y_{CM}| < 4.0 ","lp");
	legBR -> Draw();
	c_np->Update();
	c_np->SaveAs("comparisonLHCb/comp_RFB_pt_LHCb_np.pdf");
	legBR->Clear();
	//c_np->Clear();

	
	
	
	
	
	
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

