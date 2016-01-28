
#include <TROOT.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
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

//#include "../001_Acceptance_weight_8rap9pt2gev/KYOcommonOptFinal.h"
#include "KYOcommonOptFinal.h"
#include <TBox.h>
//#include <vector>
//#include <list>
//#include <algorithm>
//#include <stdlib.h>
#include <CMS_lumi.h>

using namespace std;

const int nLHCbBin = 8;

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);
void getMeanPtBins( Double_t * binArr, TF1* func);
void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

int comp_crossSection_pt_LHCb_middle_xErr(bool shifted=false, Int_t runCode=0, bool isScale =false, bool isLog=false, bool isPtCut = false)
{
	//gROOT->Macro("./JpsiStyleForFinalResult.C");
	gROOT->Macro("./tdrstyle_kyo.C");
	gStyle->SetTitleSize(0.046, "XYZ");
  gStyle->SetEndErrorSize(0);
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
	
	writeExtraText = true;
	extraText  = "Preliminary";
	lumi_502TeV  = "34.6 nb^{-1}";
	int iPeriod = 0; 
	int iPos=0;
	
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
	TFile *inFile = new TFile("cross_8rap9pt2gev/crossSection_pt_middle_isLog0_isScale0.root");
	//TFile *inFile = new TFile("cross_8rap9pt/crossSection_pt_middle_isLog0_isScale0.root");
	TGraphAsymmErrors* gCross_pr_sys_0 = (TGraphAsymmErrors*)inFile->Get("gCross_pr_sys_0"); 	
	TGraphAsymmErrors* gCross_pr_0 = (TGraphAsymmErrors*)inFile->Get("gCross_pr_0"); 	
	TGraphAsymmErrors* gCross_np_sys_0 = (TGraphAsymmErrors*)inFile->Get("gCross_np_sys_0"); 	
	TGraphAsymmErrors* gCross_np_0 = (TGraphAsymmErrors*)inFile->Get("gCross_np_0"); 	
	cout << "gCross_pr_sys_0 = " << gCross_pr_sys_0 << endl;
	cout << "gCross_pr_0 = " << gCross_pr_0 << endl;
	cout << "gCross_np_sys_0 = " << gCross_np_sys_0 << endl;
	cout << "gCross_np_0 = " << gCross_np_0 << endl;

	/// put xErr
	Double_t ex[]= {0.5, 0.5, 0.5, 0.75, 0.5, 0.5, 0.75, 2., 8.};
	for (int ipt=0; ipt<9; ipt++) {
			gCross_pr_0->SetPointEXlow(ipt, ex[ipt]);
			gCross_pr_0->SetPointEXhigh(ipt, ex[ipt]);
			gCross_np_0->SetPointEXlow(ipt, ex[ipt]);
			gCross_np_0->SetPointEXhigh(ipt, ex[ipt]);
			gCross_pr_sys_0->SetPointEXlow(ipt, 0.4);
			gCross_pr_sys_0->SetPointEXhigh(ipt, 0.4);
			gCross_np_sys_0->SetPointEXlow(ipt, 0.4);
			gCross_np_sys_0->SetPointEXhigh(ipt, 0.4);
	}

	///////////////////////////////////////////////////
	///////////////////// LHCb ////////////////////////
	///////////////////////////////////////////////////
	Double_t lhcb_px[] = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 10.5};
	//Double_t lhcb_ex[] = {0., 0., 0., 0., 0., 0., 0., 0.};
	Double_t lhcb_ex[] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 3.5}; //binW
//	Double_t lhcb_py_pr[] = {69.1, 160.2, 138.5, 85.6, 60.4, 34.2, 17.8, 2.8};
//	Double_t lhcb_py_np[] = {12.5, 15.6, 19.3, 14.9, 10.1, 6.6, 3.9, 0.9};
	Double_t lhcb_py_pr[] = {69.1*br, 160.2*br, 138.5*br, 85.6*br, 60.4*br, 34.2*br, 17.8*br, 2.8*br};
	Double_t lhcb_py_np[] = {12.5*br, 15.6*br, 19.3*br, 14.9*br, 10.1*br, 6.6*br, 3.9*br, 0.9*br};

	Double_t lhcb_ey_pr[] = {7.7*br, 11.9*br, 9.9*br, 7.0*br, 4.5*br, 3.0*br, 1.9*br, 0.2*br};
	Double_t lhcb_ey_np[] = {2.7*br, 2.7*br, 2.9*br, 2.4*br, 1.6*br, 1.2*br, 0.8*br, 0.1*br};
	Double_t lhcb_exsys[] = {0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4};
	//Double_t lhcb_exsys[] = {0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6};
	Double_t lhcb_eysys01_pr[] = {3.7, 12.4, 10.0, 7.6, 4.0, 0.9, 0.6, 0.1};
	Double_t lhcb_eysys02_pr[] = {3.1, 7.3, 6.3, 3.9, 2.7, 1.5, 0.8, 0.1};
	Double_t lhcb_eysys01_np[] = {2.4, 2.7, 1.9, 1.3, 0.8, 0.3, 0.4, 0.0};
	Double_t lhcb_eysys02_np[] = {0.6, 0.7, 0.9, 0.7, 0.5, 0.3, 0.2, 0.0};
	
	const Int_t nBin_lhcb = sizeof(lhcb_px)/sizeof(Double_t);
	cout << "nBin_lhcb : " << nBin_lhcb << endl; 

	Double_t dummy01[]={0,0,0,0,0,0,0,0};
	Double_t dummy02[]={0,0,0,0,0,0,0,0};
	Double_t dummy03[nBin_lhcb];
	Double_t lhcb_eysys_pr[nBin_lhcb];
	Double_t lhcb_eysys_np[nBin_lhcb];

	// total systematic uncertainties = sqrt( (sys01)^2 + (sys02)^2 )
	for (int ipt =0; ipt< nBin_lhcb; ipt ++) {
		AddValue(dummy01[ipt], lhcb_eysys01_pr[ipt], dummy02[ipt], lhcb_eysys02_pr[ipt], &dummy03[ipt], &lhcb_eysys_pr[ipt]);
		lhcb_eysys_pr[ipt]=lhcb_eysys_pr[ipt]*br;
		cout << ipt << "th lhcb_eysys_pr = " << lhcb_eysys_pr[ipt] << endl;
		AddValue(dummy01[ipt], lhcb_eysys01_np[ipt], dummy02[ipt], lhcb_eysys02_np[ipt], &dummy03[ipt], &lhcb_eysys_np[ipt]);
		lhcb_eysys_np[ipt]=lhcb_eysys_np[ipt]*br;
		cout << ipt << "th lhcb_eysys_np = " << lhcb_eysys_np[ipt] << endl;
	}
	//Double_t lhcb_eysys_pr_tmp[] = {4.580392996, 13.86362146, 11.36485812, 8.285529555, 4.614108798, 1.58113883, 0.921954446, 0.141421356};
	//Double_t lhcb_eysys_np_tmp[] = {2.451530134, 2.765863337, 2.024845673, 1.431782106, 0.894427191, 0.424264069, 0.447213595, 0.};

	////////////////////
	////// Draw Plots
	////////////////////

//	TLegend *legUR = new TLegend(0.50, 0.71, 0.83, 0.85); //upper left
	TLegend *legUR = new TLegend(0.28, 0.71, 0.83, 0.85); //upper left
	SetLegendStyle(legUR);
	//TLegend *legUL = new TLegend(0.165,0.80,0.55,0.92); //upper left
	TLegend *legUL = new TLegend(0.165,0.85,0.53,0.92); //upper left
	SetLegendStyle(legUL);
	//TLegend *legBL = new TLegend(0.165,0.175,0.55,0.34); //upper left
	TLegend *legBL = new TLegend(0.165,0.195,0.55,0.32); //upper left
	SetLegendStyle(legBL);
	TLegend *legUC01 = new TLegend(0.28,0.70,0.56,0.83); //upper left
	SetLegendStyle(legUC01);
	TLegend *legUC02 = new TLegend(0.63,0.70,0.89,0.83); //upper left
	SetLegendStyle(legUC02);
	legUR->SetTextSize(0.037);
	legUL->SetTextSize(0.037);
	legBL->SetTextSize(0.037);
	legUC01->SetTextSize(0.037);
	legUC02->SetTextSize(0.037);
  legUR->SetTextFont(42);
  legUL->SetTextFont(42);
  legBL->SetTextFont(42);
  legUC01->SetTextFont(42);
  legUC02->SetTextFont(42);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	if (isLog) latex->SetTextAlign(12);
	else latex->SetTextAlign(32);
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
	TGraphAsymmErrors * gCross_lhcb_pr_sys;
	TGraphAsymmErrors * gCross_lhcb_pr;
	TGraphAsymmErrors * gCross_lhcb_np_sys;
	TGraphAsymmErrors * gCross_lhcb_np;

	////// 01 ////////
	// prompt
	TCanvas *c_pr = new TCanvas("c_pr","", 200, 10, 600, 600);
	c_pr->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	gCross_pr_sys_0->SetTitle("");
	gCross_pr_sys_0->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gCross_pr_sys_0->GetXaxis()->CenterTitle();
	if (isPtCut) gCross_pr_sys_0->GetXaxis()->SetLimits(3.0,30.0);
	else gCross_pr_sys_0->GetXaxis()->SetLimits(0.0,30.0);
	gCross_pr_sys_0->GetYaxis()->SetTitle("B x d^{2}#sigma/dp_{T}dy [#mub/(GeV/c)]");
	if (isLog){
		gCross_pr_sys_0->SetMinimum(0.001);
		if (isPtCut) gCross_pr_sys_0->SetMaximum(500.);
		else gCross_pr_sys_0->SetMaximum(100.);
	}
	else {
		gCross_pr_sys_0->SetMinimum(-1.0);
		if(isPtCut) gCross_pr_sys_0->SetMaximum(150.);
//		else gCross_pr_sys_0->SetMaximum(300.);
		else gCross_pr_sys_0->SetMaximum(20.);
	}
	//gCross_pr_sys_0->SetFillColor(kViolet-9);
	gCross_pr_sys_0->SetFillColor(kTeal-9);
	gCross_pr_sys_0->Draw("A2");

	gCross_lhcb_pr_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_exsys, lhcb_exsys, lhcb_eysys_pr, lhcb_eysys_pr);	
	//gCross_lhcb_pr_sys->SetFillColor(kAzure-9);
	gCross_lhcb_pr_sys->SetFillColor(kGray);
//	gCross_lhcb_pr_sys->SetFillStyle(3001);

	//SetGraphStyle(gCross_pr_0,8,12);
	SetGraphStyle(gCross_pr_0,0,5);
	gCross_pr_0->SetMarkerSize(2.1);
	gCross_pr_0->Draw("P");


	gCross_lhcb_pr = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_ex, lhcb_ex, lhcb_ey_pr, lhcb_ey_pr);	
	SetGraphStyle(gCross_lhcb_pr,9,10);

	TF1 *expoFunc_pr = new TF1("expoFunc_pr","[0]*exp(-[1]*x)", 4, 14);
	expoFunc_pr->SetParameter(0,10000);
	expoFunc_pr->SetParameter(1,1.5);
	expoFunc_pr->SetLineColor(6);
	//gCross_lhcb_pr->Fit("expoFunc_pr","R LL M");
	gCross_lhcb_pr->Fit("expoFunc_pr","R LL M N");
	double lhcb_px_prShift[nLHCbBin];
	getMeanPtBins( lhcb_px_prShift, expoFunc_pr);
	TGraphAsymmErrors* gCross_lhcb_pr_shifted = new TGraphAsymmErrors(nBin_lhcb, lhcb_px_prShift, lhcb_py_pr, lhcb_ex, lhcb_ex, lhcb_ey_pr, lhcb_ey_pr);
	SetGraphStyle(gCross_lhcb_pr_shifted,9,10);

	TGraphAsymmErrors* gCross_lhcb_pr_sys_shifted = new TGraphAsymmErrors(nBin_lhcb, lhcb_px_prShift, lhcb_py_pr, lhcb_exsys, lhcb_exsys, lhcb_eysys_pr, lhcb_eysys_pr);
//        gCross_lhcb_pr_sys_shifted->SetFillColor(kAzure-9);
        gCross_lhcb_pr_sys_shifted->SetFillColor(kGray);
//        gCross_lhcb_pr_sys_shifted->SetFillStyle(3001);
	
	if ( shifted)  {
	  gCross_lhcb_pr_sys_shifted->Draw("2");
	  gCross_lhcb_pr_shifted->Draw("P");
	}
	else {  
	  gCross_lhcb_pr_sys->Draw("2");
	  gCross_lhcb_pr->Draw("P");
	}

	gCross_pr_0->Draw("P");







	if (!isLog) {
		if (isPtCut) dashedLine(3.0,0.0,30.0,0.0,1,1);
		else dashedLine(0.0,0.0,30.0,0.0,1,1);
	}
	
	if (isLog){	
		legUL -> AddEntry(gCross_pr_0,"1.5 < y_{CM} < 1.93","lp");
		legUL -> Draw();
		legBL -> SetHeader("LHCb");
		legBL -> AddEntry(gCross_lhcb_pr,"1.5 < y_{CM} < 2","lp");
		legBL -> Draw();
	}
	else {
		legUC01 -> SetHeader("CMS");
		legUC01 -> AddEntry(gCross_pr_0,"1.5 < y_{CM} < 1.93","lp");
 		legUC01-> Draw();
		legUC02 -> SetHeader("LHCb");
		legUC02 -> AddEntry(gCross_lhcb_pr,"1.5 < y_{CM} < 2","lp");
 		legUC02-> Draw();
	}	
	globtex->SetTextSize(0.045);
	globtex->SetTextFont(62);
	globtex->DrawLatex(0.88, 0.88, "Prompt J/#psi");
	
	CMS_lumi( c_pr, iPeriod, iPos );
	c_pr->Update();
	c_pr->SaveAs(Form("compOtherExp/comp_crossSection_pt_LHCb_pr_isLog%d_isPtCut_%d_shifted_%d_middle_xErr.pdf",(int)isLog,(int)isPtCut,(int)shifted));
	c_pr->SaveAs(Form("compOtherExp/comp_crossSection_pt_LHCb_pr_isLog%d_isPtCut_%d_shifted_%d_middle_xErr.png",(int)isLog,(int)isPtCut,(int)shifted));
	legUL->Clear();
	legBL->Clear();
	legUC01->Clear();
	legUC02->Clear();
	//c_pr->Clear();


	////// 02 ////////
	// non-prompt
	TCanvas *c_np = new TCanvas("c_np","", 200, 10, 600, 600);
	c_np->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	gCross_np_sys_0->SetTitle("");
	gCross_np_sys_0->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gCross_np_sys_0->GetXaxis()->CenterTitle();
	if(isPtCut) gCross_np_sys_0->GetXaxis()->SetLimits(3.0,30.0);
	else gCross_np_sys_0->GetXaxis()->SetLimits(0.0,30.0);
	gCross_np_sys_0->GetYaxis()->SetTitle("B x d^{2}#sigma/dp_{T}dy [ #mub/(GeV/c)]");
	if (isLog){
		gCross_np_sys_0->SetMinimum(0.001);
		if (isPtCut) gCross_np_sys_0->SetMaximum(500.);
		//else gCross_np_sys_0->SetMaximum(100.);
		else gCross_np_sys_0->SetMaximum(10.);
	}
	else {
		gCross_np_sys_0->SetMinimum(-0.1);
		if (isPtCut) gCross_np_sys_0->SetMaximum(25.);
		else gCross_np_sys_0->SetMaximum(2.5);
	}
	gCross_np_sys_0->SetFillColor(kTeal-9);
	gCross_np_sys_0->Draw("A2");

	gCross_lhcb_np_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_exsys, lhcb_exsys, lhcb_eysys_np, lhcb_eysys_np);	
	gCross_lhcb_np_sys->SetFillColor(kGray);
//	gCross_lhcb_np_sys->SetFillStyle(3001);

	SetGraphStyle(gCross_np_0,0,5);
	gCross_np_0->SetMarkerSize(2.1);
	gCross_np_0->Draw("P");

	gCross_lhcb_np = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_ex, lhcb_ex, lhcb_ey_np, lhcb_ey_np);	
	SetGraphStyle(gCross_lhcb_np,9,10);
	
	TF1 *expoFunc_np = new TF1("expoFunc_np","[0]*exp(-[1]*x)", 4, 14);
        expoFunc_np->SetParameter(0,10000);
        expoFunc_np->SetParameter(1,1.5);
        expoFunc_np->SetLineColor(6);
        //gCross_lhcb_np->Fit("expoFunc_np","R LL M");
        gCross_lhcb_np->Fit("expoFunc_np","R LL M N");
	double lhcb_px_npShift[nLHCbBin];
        getMeanPtBins( lhcb_px_npShift, expoFunc_np);
        TGraphAsymmErrors* gCross_lhcb_np_shifted = new TGraphAsymmErrors(nBin_lhcb, lhcb_px_npShift, lhcb_py_np, lhcb_ex, lhcb_ex, lhcb_ey_np, lhcb_ey_np);
        SetGraphStyle(gCross_lhcb_np_shifted,9,10);

        TGraphAsymmErrors* gCross_lhcb_np_sys_shifted = new TGraphAsymmErrors(nBin_lhcb, lhcb_px_npShift, lhcb_py_np, lhcb_exsys, lhcb_exsys, lhcb_eysys_np, lhcb_eysys_np);
        gCross_lhcb_np_sys_shifted->SetFillColor(kGray);
//        gCross_lhcb_np_sys_shifted->SetFillStyle(3001);

        if ( shifted)  {
          gCross_lhcb_np_sys_shifted->Draw("2");
          gCross_lhcb_np_shifted->Draw("P");
        }
        else {
          gCross_lhcb_np_sys->Draw("2");
          gCross_lhcb_np->Draw("P");
	}
	if (!isLog) {
		if (isPtCut) dashedLine(3.0,0.0,30.0,0.0,1,1);
		else dashedLine(0.0,0.0,30.0,0.0,1,1);
	}
	
	gCross_np_0->Draw("P");

	if (isLog){	
		legUL -> AddEntry(gCross_pr_0,"1.5 < y_{CM} < 1.93","lp");
		legUL -> Draw();
		legBL -> SetHeader("LHCb");
		legBL -> AddEntry(gCross_lhcb_pr,"1.5 < y_{CM} < 2","lp");
		legBL -> Draw();
	}
	else {
		legUC01 -> SetHeader("CMS");
		legUC01 -> AddEntry(gCross_np_0,"1.5 < y_{CM} < 1.93","lp");
 		legUC01-> Draw();
		legUC02 -> SetHeader("LHCb");
		legUC02 -> AddEntry(gCross_lhcb_np,"1.5 < y_{CM} < 2","lp");
 		legUC02-> Draw();
	}	

	globtex->SetTextSize(0.045);
	globtex->SetTextFont(62);
	globtex->DrawLatex(0.88, 0.88, "Non-prompt J/#psi");
	
//	latex->SetTextSize(0.04);
//	if (isLog) {
//		latex->DrawLatex(0.17, 0.28, Form("%s",beamstring.c_str()));
//	}
//	else {
//		latex->DrawLatex(0.89, 0.28, Form("%s",beamstring.c_str()));
//	}	
	
	CMS_lumi( c_np, iPeriod, iPos );
	c_np->Update();
	c_np->SaveAs(Form("compOtherExp/comp_crossSection_pt_LHCb_np_isLog%d_isPtCut_%d_shifted_%d_middle_xErr.pdf",(int)isLog,(int)isPtCut,(int)shifted));
	c_np->SaveAs(Form("compOtherExp/comp_crossSection_pt_LHCb_np_isLog%d_isPtCut_%d_shifted_%d_middle_xErr.png",(int)isLog,(int)isPtCut,(int)shifted));
	legUL->Clear();
	legBL->Clear();
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




void getMeanPtBins( Double_t * binArr, TF1* func) {
  Double_t lhcb_Bin[nLHCbBin+1] = {0,1,2,3,4,5,6,7,14};
  
  TH1D* firhist = (TH1D*)func->GetHistogram();
  for ( int i=1 ; i<= nLHCbBin ; i++) {
    float lowPtCut = lhcb_Bin[i-1];
    float highPtCut = lhcb_Bin[i];

    TH1D* tempHist = (TH1D*)firhist->Clone("tempHist");
    for ( int xbin = 1 ;xbin<=tempHist->GetNbinsX() ; xbin++) {
      if ( (tempHist->GetBinCenter(xbin) > highPtCut) || (tempHist->GetBinCenter(xbin) < lowPtCut) )
	tempHist->SetBinContent(xbin,0);
    }
    float meanPt = tempHist->GetMean();
    if ( meanPt <0.1 )   // fit is not applied
      meanPt = (lowPtCut+highPtCut)/2.;

    cout << " meanPt = " << meanPt << endl;
    binArr[i-1] = meanPt;
    delete tempHist;

  }

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


