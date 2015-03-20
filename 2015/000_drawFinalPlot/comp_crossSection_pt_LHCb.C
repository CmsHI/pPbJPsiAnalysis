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

#include "KYOcommonOpt.h"
#include <TBox.h>
//#include <vector>
//#include <list>
//#include <algorithm>
//#include <stdlib.h>

using namespace std;

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

int comp_crossSection_pt_LHCb(Int_t runCode=0, bool isScale =false, bool isLog=false)
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
	
	// read our crossSection_pt graph
	TFile *inFile = new TFile("cross_8rap9pt/crossSection_pt_isLog0_isScale0.root");
	TGraphAsymmErrors* gCross_pr_sys_0 = (TGraphAsymmErrors*)inFile->Get("gCross_pr_sys_0"); 	
	TGraphAsymmErrors* gCross_pr_0 = (TGraphAsymmErrors*)inFile->Get("gCross_pr_0"); 	
	cout << "gCross_pr_sys_0 = " << gCross_pr_sys_0 << endl;
	cout << "gCross_pr_0 = " << gCross_pr_0 << endl;

	///////////////////////////////////////////////////
	///////////////////// LHCb ////////////////////////
	///////////////////////////////////////////////////
	Double_t lhc_px[] = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 10.5};
	Double_t lhc_px_binW[] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 3.5};
	Double_t lhc_py_pr[] = {69.1, 160.2, 138.5, 85.6, 60.4, 34.2, 17.8, 2.8};
	Double_t lhc_py_np[] = {12.5, 15.6, 19.3, 14.9, 10.1, 6.6, 3.9, 0.9};

	Double_t lhc_ey_pr[] = {7.7, 11.9, 9.9, 7.0, 4.5, 3.0, 1.9, 0.2};
	Double_t lhc_ey_np[] = {2.7, 2.7, 2.9, 2.4, 1.6, 1.2, 0.8, 0.1};
	Double_t lhc_eysys01_pr[] = {3.7, 12.4, 10.0, 7.6, 4.0, 0.9, 0.6, 0.1};
	Double_t lhc_eysys02_pr[] = {3.1, 7.3, 6.3, 3.9, 2.7, 1.5, 0.8, 0.1};
	Double_t lhc_eysys01_np[] = {2.4, 2.7, 1.9, 1.3, 0.8, 0.3, 0.4, 0.0};
	Double_t lhc_eysys02_np[] = {0.6, 0.7, 0.9, 0.7, 0.5, 0.3, 0.2, 0.0};

	Double_t lhc_eysys_pr[] = {4.580392996, 13.86362146, 11.36485812, 8.285529555, 4.614108798, 1.58113883, 0.921954446, 0.141421356};
	Double_t lhc_eysys_np[] = {2.451530134, 2.765863337, 2.024845673, 1.431782106, 0.894427191, 0.424264069, 0.447213595, 0.};
	const Int_t nPtlhc = sizeof(lhc_pt)/sizeof(Double_t);
	cout << "nPtlhc : " << nPtlhc << endl; 

/*	
	
	
	
	
	
	////////////////////
	////// Draw Plots
	////////////////////
	///////// TCanvs c_yield for yield plots
	///////// TCanvs c_correc for acc, eff correction check
	TCanvas *c_yield = new TCanvas("c_yield","", 200, 10, 800, 600);
	//TCanvas *c_correc = new TCanvas("c_correc","", 200, 10, 600, 600);

	//TLegend *legUR = new TLegend(0.52, 0.55, 0.85, 0.92); //upper left
	TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
	SetLegendStyle(legUR);
	TLegend *legUL = new TLegend(0.20, 0.73, 0.40, 0.92); //upper left
	SetLegendStyle(legUL);
	TLegend *legUM = new TLegend(0.40, 0.75, 0.58, 0.90); //upper middle
	SetLegendStyle(legUM);
	TLegend *legBL = new TLegend(0.20, 0.20, 0.48, 0.37); //bottom left
	SetLegendStyle(legBL);
	TLegend *legBM = new TLegend(0.40, 0.20, 0.58, 0.37); //bottom middle
	SetLegendStyle(legBM);
	TLegend *legMR = new TLegend(0.54, 0.34, 0.87, 0.57); //bottom left
	SetLegendStyle(legMR);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//TBox * globalbox = new TBox(0, 6.4, 1, 13.6);
	//globalbox->SetFillColor(kViolet-9);
	TBox * globalbox = new TBox(0.5, 6.4, 1.5, 13.6);
	globalbox->SetFillColor(kYellow);



	/// LHCb /////
	TGraphAsymmErrors * cross_lhc_prompt;
	TGraphAsymmErrors * cross_lhc_prompt_sys;
	TGraphAsymmErrors * cross_lhc_nonprompt;
	TGraphAsymmErrors * cross_lhc_nonprompt_sys;



			////// 01 ////////
			// prompt
			c_yield->cd();
			if (isLog) gPad->SetLogy(1);
			else gPad->SetLogy(0);
			cross_prompt_sys01 = new TGraphAsymmErrors(nPt01, meanPt01, nPrCross01, exsys_01, exsys_01, errsysPR01, errsysPR01);
			cross_prompt_sys01->SetTitle("");
			cross_prompt_sys01->GetXaxis()->SetTitle("p_{T} (GeV/c)");
			cross_prompt_sys01->GetXaxis()->CenterTitle();
			if (isPt3) cross_prompt_sys01->GetXaxis()->SetLimits(3.0,30.0);
			else cross_prompt_sys01->GetXaxis()->SetLimits(0.0,30.0);
			cross_prompt_sys01->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy [ #mub/(GeV/c)]");
			if (isLog){
	  		cross_prompt_sys01->SetMinimum(0.01);
		  	cross_prompt_sys01->SetMaximum(5000.);
			}
			else {
	  		cross_prompt_sys01->SetMinimum(-10.0);
		  	if (isPt3) {cross_prompt_sys01->SetMaximum(110.);}
		  	else { cross_prompt_sys01->SetMaximum(250.);}
			}
	  	cross_prompt_sys01->SetFillColor(kRed-9);
	  	cross_prompt_sys01->Draw("A2");

			cross_lhc_prompt_sys = new TGraphAsymmErrors(nPtlhc, lhc_pt, lhc_PR, lhc_pt_binW, lhc_pt_binW, lhc_PRSysErr, lhc_PRSysErr);	
	  	cross_lhc_prompt_sys->SetFillColor(kTeal-9);
	  	cross_lhc_prompt_sys->Draw("2");

			//real
			cross_prompt01 = new TGraphAsymmErrors(nPt01, meanPt01, nPrCross01, exl_01, exh_01, nPrCrossErr01, nPrCrossErr01);
			SetGraphStyle(cross_prompt01,1,3); 
			cross_prompt01->Draw("P");
		
			cross_lhc_prompt = new TGraphAsymmErrors(nPtlhc, lhc_pt, lhc_PR, lhc_pt_binW, lhc_pt_binW, lhc_PRErr, lhc_PRErr);	
			SetGraphStyle(cross_lhc_prompt,4,10); 
			cross_lhc_prompt->Draw("P");

			legMR -> SetHeader("Prompt J/#psi");
			if (isScale==1){
				legMR -> AddEntry(cross_prompt01,Form("%s    (x100)",rapArr[0].c_str()),"lp");
			}
			else {
				legMR -> AddEntry(cross_prompt01,Form("CMS : %s",rapArr[0].c_str()),"lp");
				legMR -> AddEntry(cross_lhc_prompt,"LHCb : 1.5 < y_{CM} < 2","lp");
			}
			legMR -> Draw();
			latex->SetTextSize(0.04);
			latex->DrawLatex(0.56, 0.71, lumistring.c_str());
			latex->SetTextSize(0.04);
			latex->DrawLatex(0.56, 0.78, beamstring.c_str());
			latex->SetTextSize(0.05);
			latex->DrawLatex(0.55, 0.86, cmsstring.c_str());
			//globalbox->Draw();
			c_yield->Update();
			c_yield->SaveAs(Form("cross_prompt_%s_%d_%d_%d_LHCb.pdf",runstring.c_str(),isScale, isLog, isPt3));
			c_yield->SaveAs(Form("cross_prompt_%s_%d_%d_%d_LHCb.png",runstring.c_str(),isScale, isLog, isPt3));
			legMR->Clear();
			c_yield->Clear();


			
			
			////// 02 ////////
			// non-nonprompt
			c_yield->cd();
			if (isLog) gPad->SetLogy(1);
			else gPad->SetLogy(0);
			cross_nonprompt_sys01 = new TGraphAsymmErrors(nPt01, meanPt01, nNpCross01, exsys_01, exsys_01, errsysNP01, errsysNP01);
			cross_nonprompt_sys01->SetTitle("");
			cross_nonprompt_sys01->GetXaxis()->SetTitle("p_{T} (GeV/c)");
			cross_nonprompt_sys01->GetXaxis()->CenterTitle();
			if (isPt3) cross_nonprompt_sys01->GetXaxis()->SetLimits(3.0,30.0);
			else cross_nonprompt_sys01->GetXaxis()->SetLimits(0.0,30.0);
			cross_nonprompt_sys01->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy [ #mub/(GeV/c)]");
			if (isLog){
	  		cross_nonprompt_sys01->SetMinimum(0.01);
		  	cross_nonprompt_sys01->SetMaximum(5000.);
			}
			else {
	  		cross_nonprompt_sys01->SetMinimum(-3.0);
		  	if (isPt3) { cross_nonprompt_sys01->SetMaximum(22.);}
		  	else { cross_nonprompt_sys01->SetMaximum(50.);}
			}
	  	cross_nonprompt_sys01->SetFillColor(kRed-9);
	  	cross_nonprompt_sys01->Draw("A2");

			cross_lhc_nonprompt_sys = new TGraphAsymmErrors(nPtlhc, lhc_pt, lhc_NP, lhc_pt_binW, lhc_pt_binW, lhc_NPSysErr, lhc_NPSysErr);	
	  	cross_lhc_nonprompt_sys->SetFillColor(kTeal-9);
	  	cross_lhc_nonprompt_sys->Draw("2");

			//real
			cross_nonprompt01 = new TGraphAsymmErrors(nPt01, meanPt01, nNpCross01, exl_01, exh_01, nNpCrossErr01, nNpCrossErr01);
			SetGraphStyle(cross_nonprompt01,1,3); 
			cross_nonprompt01->Draw("P");
		
			cross_lhc_nonprompt = new TGraphAsymmErrors(nPtlhc, lhc_pt, lhc_NP, lhc_pt_binW, lhc_pt_binW, lhc_NPErr, lhc_NPErr);	
			SetGraphStyle(cross_lhc_nonprompt,4,10); 
			cross_lhc_nonprompt->Draw("P");

			legMR -> SetHeader("Non-prompt J/#psi");
			if (isScale==1){
				legMR -> AddEntry(cross_nonprompt01,Form("%s    (x100)",rapArr[0].c_str()),"lp");
			}
			else {
				legMR -> AddEntry(cross_nonprompt01,Form("CMS : %s",rapArr[0].c_str()),"lp");
				legMR -> AddEntry(cross_lhc_nonprompt,"LHCb : 1.5 < y_{CM} < 2","lp");
			}
			legMR -> Draw();
			latex->SetTextSize(0.04);
			latex->DrawLatex(0.56, 0.71, lumistring.c_str());
			latex->SetTextSize(0.04);
			latex->DrawLatex(0.56, 0.78, beamstring.c_str());
			latex->SetTextSize(0.05);
			latex->DrawLatex(0.55, 0.86, cmsstring.c_str());
			//globalbox->Draw();
			c_yield->Update();
			c_yield->SaveAs(Form("cross_nonprompt_%s_%d_%d_%d_LHCb.pdf",runstring.c_str(),isScale, isLog, isPt3));
			c_yield->SaveAs(Form("cross_nonprompt_%s_%d_%d_%d_LHCb.png",runstring.c_str(),isScale, isLog, isPt3));
			legMR->Clear();
			c_yield->Clear();











*/












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
		*arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
	}
}

