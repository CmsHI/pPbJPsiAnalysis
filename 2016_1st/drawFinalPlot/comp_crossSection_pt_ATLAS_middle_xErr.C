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
#include "KYOcommonOptFinal.h"
#include <TBox.h>
//#include <vector>
//#include <list>
//#include <algorithm>
//#include <stdlib.h>

#include "CMS_lumi.h"

using namespace std;

const int nAtlasBin = 5;

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);
void getMeanPtBins( Double_t* binArr, TF1* func);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

int comp_crossSection_pt_ATLAS_middle_xErr(bool shifted=false, Int_t runCode=0, bool isScale =false, bool isLog=true, bool isPtCut = false)
{
	//gROOT->Macro("./JpsiStyleForFinalResult.C");
	//cout << "HEEEEEEEEELLO"<<endl;
	gROOT->Macro("./tdrstyle_kyo.C");
	//gROOT->Macro("./CMS_lumi.C");// without compile
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

	//globtex box for beam, rapidity, pT info
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	//globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);
	
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
	//TFile *inFile = new TFile("cross_8rap9pt2gev/crossSection_pt_integ_middle_isLog0_isScale0.root");
	TFile *inFile = new TFile("cross_8rap9pt/crossSection_pt_integ_middle_isLog0_isScale0.root");
	TGraphAsymmErrors* gCross_pr_fwrap = (TGraphAsymmErrors*)inFile->Get("gCross_pr_fwrap"); 
	TGraphAsymmErrors* gCross_np_fwrap = (TGraphAsymmErrors*)inFile->Get("gCross_np_fwrap"); 
	TGraphAsymmErrors* gCross_pr_bwrap = (TGraphAsymmErrors*)inFile->Get("gCross_pr_bwrap"); 
	TGraphAsymmErrors* gCross_np_bwrap = (TGraphAsymmErrors*)inFile->Get("gCross_np_bwrap"); 

	TGraphAsymmErrors* gCross_pr_sys_fwrap = (TGraphAsymmErrors*)inFile->Get("gCross_pr_sys_fwrap"); 
	TGraphAsymmErrors* gCross_np_sys_fwrap = (TGraphAsymmErrors*)inFile->Get("gCross_np_sys_fwrap"); 
	TGraphAsymmErrors* gCross_pr_sys_bwrap = (TGraphAsymmErrors*)inFile->Get("gCross_pr_sys_bwrap"); 
	TGraphAsymmErrors* gCross_np_sys_bwrap = (TGraphAsymmErrors*)inFile->Get("gCross_np_sys_bwrap"); 



	/// put xErr
	const int nPt = 9;
	Double_t ex[nPt]= {0.5, 0.5, 0.5, 0.75, 0.5, 0.5, 0.75, 2., 8.};
	for (int ipt=0; ipt<nPt; ipt++) {
			gCross_pr_fwrap->SetPointEXlow(ipt, ex[ipt]);
			gCross_pr_fwrap->SetPointEXhigh(ipt, ex[ipt]);
			gCross_np_fwrap->SetPointEXlow(ipt, ex[ipt]);
			gCross_np_fwrap->SetPointEXhigh(ipt, ex[ipt]);
			gCross_pr_sys_fwrap->SetPointEXlow(ipt, 0.4);
			gCross_pr_sys_fwrap->SetPointEXhigh(ipt, 0.4);
			gCross_np_sys_fwrap->SetPointEXlow(ipt, 0.4);
			gCross_np_sys_fwrap->SetPointEXhigh(ipt, 0.4);
			gCross_pr_bwrap->SetPointEXlow(ipt, ex[ipt]);
			gCross_pr_bwrap->SetPointEXhigh(ipt, ex[ipt]);
			gCross_np_bwrap->SetPointEXlow(ipt, ex[ipt]);
			gCross_np_bwrap->SetPointEXhigh(ipt, ex[ipt]);
			gCross_pr_sys_bwrap->SetPointEXlow(ipt, 0.4);
			gCross_pr_sys_bwrap->SetPointEXhigh(ipt, 0.4);
			gCross_np_sys_bwrap->SetPointEXlow(ipt, 0.4);
			gCross_np_sys_bwrap->SetPointEXhigh(ipt, 0.4);
	}

	///////////////////////////////////////////////////
	///////////////////// LHCb ////////////////////////

	///////////////////////////////////////////////////
	///////////////////// ATLAS ////////////////////////
	///////////////////////////////////////////////////

	//cross section ds -1.94<y<0
	//vs pT
	// 414, 173, 58.2, 11.8, 1.41
	//stat 12, 4, 1.4, 0.4, 0.08
	//syst 39, 16, 4.3, 0.8, 0.10

	//cross section ds 0<y<1.94
	//408, 159, 55.5, 11.9, 1.13
	//stat 12, 4, 1.5, 0.3, 0.08
	//syst 50. 15, 5.7, 0.9, 0.07

	/// scaling nb -> mb
	Double_t scaleF = 1./1000.; 
	const int nPtATLAS = 5;
	Double_t atlas_px[nPtATLAS] = {8.75, 10.5, 12.75, 17.0, 25.0};
	//Double_t atlas_px_binW[] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 3.5};
	//Double_t atlas_ex[] = {0., 0., 0., 0., 0., 0., 0., 0.};
	Double_t atlas_ex[nPtATLAS] = {0.75, 0.5, 1.25, 3., 5.};
	Double_t atlas_py_prFW[nPtATLAS] = {408*scaleF, 159*scaleF, 55.5*scaleF, 11.9*scaleF, 1.13*scaleF};
	Double_t atlas_py_prBW[nPtATLAS] = {414*scaleF, 173*scaleF, 58.2*scaleF, 11.8*scaleF, 1.41*scaleF};

	Double_t atlas_py_npFW[nPtATLAS] = {136*scaleF, 69.9*scaleF, 29.2*scaleF, 9.06*scaleF, 1.48*scaleF };
	Double_t atlas_py_npBW[nPtATLAS] = { 167*scaleF, 69.1*scaleF, 32.3*scaleF, 9.28*scaleF, 1.43*scaleF};


	Double_t atlas_ey_prFW[nPtATLAS] = { 12*scaleF, 4*scaleF, 1.5*scaleF, 0.3*scaleF, 0.08*scaleF };
	Double_t atlas_ey_prBW[nPtATLAS] = { 12*scaleF, 4*scaleF, 1.4*scaleF, 0.4*scaleF, 0.08*scaleF };
	Double_t atlas_ey_npFW[nPtATLAS] = { 8*scaleF, 2.8*scaleF, 1.3*scaleF, 0.33*scaleF, 0.9*scaleF };
	Double_t atlas_ey_npBW[nPtATLAS] = {9*scaleF, 2.6*scaleF, 1.2*scaleF, 0.33*scaleF, 0.08*scaleF};

	//Double_t atlas_exsys[nPtATLAS] = {0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3};
	Double_t atlas_exsys[nPtATLAS] = {0.3, 0.3, 0.3, 0.3, 0.3};
	Double_t atlas_eysys01_prFW[nPtATLAS] = {50.0*scaleF, 15.0 *scaleF, 5.7*scaleF, 0.9*scaleF, 0.07*scaleF};
//	Double_t atlas_eysys02_prFW[] = { 11*scaleF, 4*scaleF, 1.5*scaleF, 0.3*scaleF, 0.03 *scaleF};
	Double_t atlas_eysys01_prBW[nPtATLAS] = { 39*scaleF, 16*scaleF, 4.3*scaleF, 0.8*scaleF, 0.10*scaleF};
//	Double_t atlas_eysys02_prBW[] = { 11*scaleF, 5*scaleF, 1.6*scaleF, 0.3*scaleF, 0.04*scaleF};

	Double_t atlas_eysys01_npFW[nPtATLAS] = { 17*scaleF, 6.6*scaleF, 3.0*scaleF, 0.7*scaleF, 0.09*scaleF};
//	Double_t atlas_eysys02_npFW[] = { 4*scaleF, 1.9*scaleF, 0.8*scaleF, 0.24*scaleF, 0.04*scaleF};
	Double_t atlas_eysys01_npBW[nPtATLAS] = {16*scaleF, 6.3*scaleF, 2.4*scaleF, 0.63*scaleF, 0.10 *scaleF};
//	Double_t atlas_eysys02_npBW[] = {5*scaleF, 1.9*scaleF, 0.9*scaleF, 0.25*scaleF, 0.04 *scaleF};
	
	//const Int_t nBin_atlas = sizeof(atlas_px)/sizeof(Double_t);
	//cout << "nBin_atlas : " << nBin_atlas << endl; 

// 	Double_t dummy01[]={0,0,0,0,0,0,0,0};
// 	Double_t dummy02[]={0,0,0,0,0,0,0,0};

	////////////////////
	////// Draw Plots
	////////////////////

	TLegend *legUL01 = new TLegend(0.165,0.80,0.55,0.92); //upper left
	SetLegendStyle(legUL01);
	TLegend *legBL01 = new TLegend(0.165,0.175,0.55,0.34); //upper left
	SetLegendStyle(legBL01);
	TLegend *legUC01 = new TLegend(0.30,0.67,0.58,0.83); //upper left
	SetLegendStyle(legUC01);
	TLegend *legUC02 = new TLegend(0.62,0.67,0.89,0.83); //upper left
	SetLegendStyle(legUC02);
  
	legUL01->SetTextSize(0.037);
	legBL01->SetTextSize(0.037);
	legUC01->SetTextSize(0.037);
	legUC02->SetTextSize(0.037);
  legUL01->SetTextFont(42);
  legBL01->SetTextFont(42);
  legUC01->SetTextFont(42);
  legUC02->SetTextFont(42);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	if (isLog) latex->SetTextAlign(12);
	else latex->SetTextAlign(32);
	latex->SetTextSize(0.04);
	

	TBox * globalbox = new TBox(0.5, 6.4, 1.5, 13.6);
	globalbox->SetFillColor(kYellow);


	/// ATLAS /////
	TGraphAsymmErrors * gCross_atlas_prFW_sys;
	TGraphAsymmErrors * gCross_atlas_prFW;
	TGraphAsymmErrors * gCross_atlas_npFW_sys;
	TGraphAsymmErrors * gCross_atlas_npFW;

	TGraphAsymmErrors * gCross_atlas_prBW_sys;
	TGraphAsymmErrors * gCross_atlas_prBW;
	TGraphAsymmErrors * gCross_atlas_npBW_sys;
	TGraphAsymmErrors * gCross_atlas_npBW;

	////// 01 ////////

	TCanvas *c_pr = new TCanvas("c_pr","", 200, 10, 600, 600);
	c_pr->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);

	TH1D *dummy1 = new TH1D("","",50,0.,400.);
	dummy1->GetXaxis()->CenterTitle();
	dummy1->GetXaxis()->SetLimits(5.0, 30.0);
//	dummy1->SetYTitle("d^{2}#sigma/dp_{T}dy x BR (J/#psi #rightarrow #mu #mu) [ #mub/(GeV/c)]");
	dummy1->SetYTitle("B x d^{2}#sigma/dp_{T}dy [ #mub/(GeV/c)]");
	dummy1->SetXTitle("p_{T} [GeV/c]");

 	if (isLog){
	dummy1->SetMinimum(0.0002);
	//dummy1->SetMinimum(0.0005);
	dummy1->SetMaximum(20);
//	dummy1->SetMaximum(50);
 	}
 	else {
	  dummy1->SetMinimum(-0.05);
	  dummy1->SetMaximum(1.5);
	}
	//dummy1->GetYaxis()->SetLimits(0.5, 600.0);
	
	//dummy1->Draw();
	dummy1->SetLineColor(kWhite); //KYO
	dummy1->Draw(); //KYO

	if (!isLog) dashedLine(5.0,0.0,30.0,0.0,1,1);

	gCross_atlas_prFW_sys = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_prFW, atlas_exsys, atlas_exsys, atlas_eysys01_prFW, atlas_eysys01_prFW);	
	gCross_atlas_prFW_sys->SetFillColor(kGray+1);
	//gCross_atlas_prFW_sys->SetFillStyle(3001);

	
	gCross_atlas_prFW = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_prFW, atlas_ex, atlas_ex, atlas_ey_prFW, atlas_ey_prFW);	
	//gCross_atlas_prFW->SetMarkerSize(1.5);
	//gCross_atlas_prFW->SetMarkerStyle(33);
	//gCross_atlas_prFW->SetMarkerColor(6);
	//gCross_atlas_prFW->SetLineColor(6);
	SetGraphStyle(gCross_atlas_prFW,9,10);
	TF1 *expoFunc_prFW = new TF1("expoFunc_prFW","[0]*exp(-[1]*x)", 8, 30);
	//expoFunc_prFW->SetParameter(0,10000);
	expoFunc_prFW->SetParameter(0,10);
	expoFunc_prFW->SetParameter(1,1.5);
	expoFunc_prFW->SetLineColor(6);
	gCross_atlas_prFW->Fit("expoFunc_prFW","R LL M N");

	double atlas_px_prFWShift[nAtlasBin];
	getMeanPtBins( atlas_px_prFWShift, expoFunc_prFW);
	TGraphAsymmErrors* gCross_atlas_prFW_shift =  new TGraphAsymmErrors(nPtATLAS, atlas_px_prFWShift, atlas_py_prFW, atlas_ex, atlas_ex, atlas_ey_prFW, atlas_ey_prFW);
   SetGraphStyle(gCross_atlas_prFW_shift,9,10);
//	gCross_atlas_prFW_shift->SetMarkerSize(1.5);
//	gCross_atlas_prFW_shift->SetMarkerStyle(33);
//	      gCross_atlas_prFW_shift->SetMarkerColor(6);
//        gCross_atlas_prFW_shift->SetLineColor(6);
	
	TGraphAsymmErrors* gCross_atlas_prFW_sys_shift = new TGraphAsymmErrors(nPtATLAS, atlas_px_prFWShift, atlas_py_prFW, atlas_exsys, atlas_exsys, atlas_eysys01_prFW, atlas_eysys01_prFW);
//	gCross_atlas_prFW_sys_shift->SetFillColor(kMagenta-9);
	gCross_atlas_prFW_sys_shift->SetFillColor(kGray+1);
//	gCross_atlas_prFW_sys_shift->SetFillStyle(3001);
	

	if ( shifted)  { 
	  gCross_atlas_prFW_sys_shift->Draw("2");
	  gCross_atlas_prFW_shift->Draw("p");
	}
	else {
	  gCross_atlas_prFW_sys->Draw("2");
	  gCross_atlas_prFW->Draw("P");
	}



	///
	gCross_atlas_prBW_sys = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_prBW, atlas_exsys, atlas_exsys, atlas_eysys01_prBW, atlas_eysys01_prBW);	
//	gCross_atlas_prBW_sys->SetFillColor(kMagenta-5);
	gCross_atlas_prBW_sys->SetFillColor(kGray);
//	gCross_atlas_prBW_sys->SetFillStyle(3002);

	SetGraphStyle(gCross_pr_fwrap,1,3);

	gCross_pr_fwrap->Draw("P");

	gCross_atlas_prBW = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_prBW, atlas_ex, atlas_ex, atlas_ey_prBW, atlas_ey_prBW);	
//	gCross_atlas_prBW->SetMarkerSize(1.5);
//	gCross_atlas_prBW->SetMarkerStyle(27);
//	gCross_atlas_prBW->SetMarkerColor(6);
//	gCross_atlas_prBW->SetLineColor(6);	
	SetGraphStyle(gCross_atlas_prBW,9,11);
	TF1 *expoFunc_prBW = new TF1("expoFunc_prBW","[0]*exp(-[1]*x)", 8, 30);
	expoFunc_prBW->SetParameter(0,10000);
	expoFunc_prBW->SetParameter(1,1.5);
	expoFunc_prBW->SetLineColor(6);
	expoFunc_prBW->SetLineStyle(2);
	gCross_atlas_prBW->Fit("expoFunc_prBW","R LL M N");

	double atlas_px_prBWShift[nAtlasBin];
        getMeanPtBins( atlas_px_prBWShift, expoFunc_prBW);
        TGraphAsymmErrors* gCross_atlas_prBW_shift =  new TGraphAsymmErrors(nPtATLAS, atlas_px_prBWShift, atlas_py_prBW, atlas_ex, atlas_ex, atlas_ey_prBW, atlas_ey_prBW);
//        gCross_atlas_prBW_shift->SetMarkerSize(1.5);
//        gCross_atlas_prBW_shift->SetMarkerStyle(27);
//        gCross_atlas_prBW_shift->SetMarkerColor(6);
//        gCross_atlas_prBW_shift->SetLineColor(6);
					SetGraphStyle(gCross_atlas_prBW_shift,9,11);

        TGraphAsymmErrors* gCross_atlas_prBW_sys_shift = new TGraphAsymmErrors(nPtATLAS, atlas_px_prBWShift, atlas_py_prBW, atlas_exsys, atlas_exsys, atlas_eysys01_prBW, atlas_eysys01_prBW);
//        gCross_atlas_prBW_sys_shift->SetFillColor(kMagenta-5);
        gCross_atlas_prBW_sys_shift->SetFillColor(kGray);
//        gCross_atlas_prBW_sys_shift->SetFillStyle(3002);
       
	if ( shifted)  { 
	  gCross_atlas_prBW_sys_shift->Draw("2");
	  gCross_atlas_prBW_shift->Draw("p");
	}
	else {
	  gCross_atlas_prBW_sys->Draw("2");
	  gCross_atlas_prBW->Draw("P");
	}
	
	//gCross_pr_sys_fwrap->SetFillColor(kRed+2);
	gCross_pr_sys_fwrap->SetFillColor(kRed-9);
//	gCross_pr_sys_fwrap->SetFillStyle(3001);
	gCross_pr_sys_fwrap->Draw("2");
//	gCross_pr_sys_bwrap->SetFillColor(kRed-10);
	gCross_pr_sys_bwrap->SetFillColor(kRed-10);
//	gCross_pr_sys_bwrap->SetFillStyle(3001);
	gCross_pr_sys_bwrap->Draw("2");

	SetGraphStyle(gCross_pr_fwrap,1,3);
	SetGraphStyle(gCross_pr_bwrap,1,13);
	gCross_pr_fwrap->Draw("P");
	gCross_pr_bwrap->Draw("P");

	//REDRAW
	if ( shifted)  { 
	 gCross_atlas_prFW_shift->Draw("p");
	 gCross_atlas_prBW_shift->Draw("p");
	}
	else {
	 gCross_atlas_prFW->Draw("p");
	 gCross_atlas_prBW->Draw("p");
	}
	gCross_pr_fwrap->Draw("P");
	gCross_pr_bwrap->Draw("P");

	if (isLog){
//	  legUL01 -> AddEntry(gCross_pr_fwrap,"CMS Preliminary"," ");
	  legUL01 -> AddEntry(gCross_pr_fwrap,"0 < y_{CM} < 1.93","lp");
	  legUL01 -> AddEntry(gCross_pr_bwrap,"-1.93 < y_{CM} < 0","lp");
	  legUL01-> Draw();
	  
		legBL01 -> SetHeader("ATLAS");
	  legBL01 -> AddEntry(gCross_atlas_prFW,"0 < y_{CM} < 1.94","lp");
	  legBL01 -> AddEntry(gCross_atlas_prBW,"-1.94 < y_{CM} < 0","lp");
	  legBL01-> Draw();
	}

       else{
	legUC01 -> AddEntry(gCross_pr_fwrap,"CMS"," ");
	legUC01 -> AddEntry(gCross_pr_fwrap,"0 < y_{CM} < 1.93","lp");
 	legUC01 -> AddEntry(gCross_pr_bwrap,"-1.93 < y_{CM} < 0","lp");
 	legUC01-> Draw();
	legUC02 -> AddEntry(gCross_pr_fwrap,"ATLAS"," ");
	legUC02 -> AddEntry(gCross_atlas_prFW,"0 < y_{CM} < 1.94","lp");
	legUC02 -> AddEntry(gCross_atlas_prBW,"-1.94 < y_{CM} < 0","lp");
 	legUC02-> Draw();
       }
	
	globtex->SetTextSize(0.045);
	globtex->SetTextFont(62);
	globtex->DrawLatex(0.88, 0.88, "Prompt J/#psi");
	
//	if (isLog) {
//		latex->DrawLatex(0.17, 0.28, Form("%s",beamstring.c_str()));
//	}
//	else {
//		latex->DrawLatex(0.89, 0.28, Form("%s",beamstring.c_str()));
//	}	

	CMS_lumi( c_pr, iPeriod, iPos );
	c_pr->Update();
	c_pr->SaveAs(Form("comparisonLHCb/comp_crossSection_pt_ATLAS_pr_isLog%d_%d_shifted_%d_middle_xErr.pdf",(int)isLog,(int)isPtCut,(int)shifted));
	c_pr->SaveAs(Form("comparisonLHCb/comp_crossSection_pt_ATLAS_pr_isLog%d_%d_shifted_%d_middle_xErr.png",(int)isLog,(int)isPtCut,(int)shifted));
	//c_pr->Clear();
	legUL01->Clear();
	legBL01->Clear();
	legUC01->Clear();
	legUC02->Clear();

	////// 02 ////////
	// non-prompt
	TCanvas *c_np = new TCanvas("c_np","", 200, 10, 600, 600);
	c_np->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);

	TH1D *dummy2 = new TH1D("","",50,0.,400.);
	dummy2->GetXaxis()->CenterTitle();
	dummy2->GetXaxis()->SetLimits(5.0, 30.0);
	dummy2->SetYTitle("B x d^{2}#sigma/dp_{T}dy [ #mub/GeV/c]");
	dummy2->SetXTitle("p_{T} [GeV/c]");

 	if (isLog){
	//dummy2->SetMinimum(0.0005);
	//dummy2->SetMaximum(1.);
	dummy2->SetMinimum(0.0004);
//	dummy2->SetMaximum(3.);
	dummy2->SetMaximum(2.);
 	}
 	else {
	  //dummy2->SetMinimum(-10.0);
	  //dummy2->SetMaximum(350);
	  dummy2->SetMinimum(-0.01);
	  dummy2->SetMaximum(0.35);
	}
	//dummy2->GetYaxis()->SetLimits(0.5, 600.0);
	
	dummy2->SetLineColor(kWhite);
	dummy2->Draw();
	if (!isLog) dashedLine(5.0,0.0,30.0,0.0,1,1);


// 	if (isLog) gPad->SetLogy(1);
// 	else gPad->SetLogy(0);
// 	gCross_np_sys_fwrap->SetTitle("");
// 	gCross_np_sys_fwrap->GetXaxis()->SetTitle("p_{T} [GeV/c]");
// 	gCross_np_sys_fwrap->GetXaxis()->CenterTitle();
// 	if(isPtCut) gCross_np_sys_fwrap->GetXaxis()->SetLimits(6.0,30.0);
// 	else gCross_np_sys_fwrap->GetXaxis()->SetLimits(6.0,30.0);
// 	gCross_np_sys_fwrap->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy x BR (J/#psi #rightarrow #mu #mu) [ #mub/(GeV/c)]");
// 	if (isLog){
// 		gCross_np_sys_fwrap->SetMinimum(0.3);
// 		if (isPtCut) gCross_np_sys_fwrap->SetMaximum(500.);
// 		//else gCross_np_sys_fwrap->SetMaximum(100.);
// 		else gCross_np_sys_fwrap->SetMaximum(500.);
// 	}
// 	else {
// 		gCross_np_sys_fwrap->SetMinimum(5.0);
// 		if (isPtCut) gCross_np_sys_fwrap->SetMaximum(30.);
// 		else gCross_np_sys_fwrap->SetMaximum(300.);
// 	}
	gCross_np_sys_fwrap->SetFillColor(kRed-9);
//	gCross_np_sys_fwrap->SetFillStyle(3001);
	gCross_np_sys_bwrap->SetFillColor(kRed-10);
//	gCross_np_sys_bwrap->SetFillStyle(3001);
	gCross_np_sys_fwrap->Draw("2");
	gCross_np_sys_bwrap->Draw("2");

	SetGraphStyle(gCross_np_fwrap,1,3);
	gCross_np_fwrap->Draw("P");

	SetGraphStyle(gCross_np_bwrap,1,13);
	gCross_np_bwrap->Draw("P");
	
	/////////////
	//ATLAS

	gCross_atlas_npFW_sys = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_npFW, atlas_exsys, atlas_exsys, atlas_eysys01_npFW, atlas_eysys01_npFW);	
	gCross_atlas_npFW_sys->SetFillColor(kGray+1);
//	gCross_atlas_npFW_sys->SetFillStyle(3001);

	//SetGraphStyle(gCross_np_fwrap,1,3);
	//gCross_np_fwrap->Draw("P");

	gCross_atlas_npFW = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_npFW, atlas_ex, atlas_ex, atlas_ey_npFW, atlas_ey_npFW);	
//	gCross_atlas_npFW->SetMarkerSize(1.5);
//	gCross_atlas_npFW->SetMarkerStyle(33);
//	gCross_atlas_npFW->SetMarkerColor(6);
//	gCross_atlas_npFW->SetLineColor(6);	
SetGraphStyle(gCross_atlas_npFW,9,10);

	TF1 *expoFunc_npFW = new TF1("expoFunc_npFW","[0]*exp(-[1]*x)", 8, 30);
	expoFunc_npFW->SetParameter(0,10000);
	expoFunc_npFW->SetParameter(1,1.5);
	expoFunc_npFW->SetLineColor(6);
	gCross_atlas_npFW->Fit("expoFunc_npFW","R LL M N");

	double atlas_px_npFWShift[nAtlasBin];
        getMeanPtBins( atlas_px_npFWShift, expoFunc_npFW);
        TGraphAsymmErrors* gCross_atlas_npFW_shift =  new TGraphAsymmErrors(nPtATLAS, atlas_px_npFWShift, atlas_py_npFW, atlas_ex, atlas_ex, atlas_ey_npFW, atlas_ey_npFW);
        gCross_atlas_npFW_shift->SetMarkerSize(1.5);
        gCross_atlas_npFW_shift->SetMarkerStyle(33);
        gCross_atlas_npFW_shift->SetMarkerColor(6);
        gCross_atlas_npFW_shift->SetLineColor(6);
				SetGraphStyle(gCross_atlas_npFW_shift,9,10);

	TGraphAsymmErrors* gCross_atlas_npFW_sys_shift = new TGraphAsymmErrors(nPtATLAS, atlas_px_npFWShift, atlas_py_npFW, atlas_exsys, atlas_exsys, atlas_eysys01_npFW, atlas_eysys01_npFW);
	gCross_atlas_npFW_sys_shift->SetFillColor(kGray+1);
//	gCross_atlas_npFW_sys_shift->SetFillStyle(3001);


	if ( shifted) {
	  gCross_atlas_npFW_sys_shift->Draw("2");
	  gCross_atlas_npFW_shift->Draw("p");
	} 
	else  {
	  gCross_atlas_npFW_sys->Draw("2");
	  gCross_atlas_npFW->Draw("p");
	}



	///
	gCross_atlas_npBW_sys = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_npBW, atlas_exsys, atlas_exsys, atlas_eysys01_npBW, atlas_eysys01_npBW);	
	gCross_atlas_npBW_sys->SetFillColor(kGray);
//	gCross_atlas_npBW_sys->SetFillStyle(3002);

	SetGraphStyle(gCross_np_fwrap,1,3);
	gCross_np_fwrap->Draw("P");

	gCross_atlas_npBW = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_npBW, atlas_ex, atlas_ex, atlas_ey_npBW, atlas_ey_npBW);
	//gCross_atlas_npBW->SetMarkerSize(1.5);
	//gCross_atlas_npBW->SetMarkerStyle(27);
	//gCross_atlas_npBW->SetMarkerColor(6);
	//gCross_atlas_npBW->SetLineColor(6);
	SetGraphStyle(gCross_atlas_npBW,9,11);
	
	TF1 *expoFunc_npBW = new TF1("expoFunc_npBW","[0]*exp(-[1]*x)", 8, 30);
	expoFunc_npBW->SetParameter(0,10000);
	expoFunc_npBW->SetParameter(1,1.5);
	expoFunc_npBW->SetLineColor(6);
	expoFunc_npBW->SetLineStyle(2);
	gCross_atlas_npBW->Fit("expoFunc_npBW","R LL M N");

	double atlas_px_npBWShift[nAtlasBin];
	getMeanPtBins( atlas_px_npBWShift, expoFunc_npBW);
        TGraphAsymmErrors* gCross_atlas_npBW_shift =  new TGraphAsymmErrors(nPtATLAS, atlas_px_npBWShift, atlas_py_npBW, atlas_ex, atlas_ex, atlas_ey_npBW, atlas_ey_npBW);
//        gCross_atlas_npBW_shift->SetMarkerSize(1.5);
//        gCross_atlas_npBW_shift->SetMarkerStyle(33);
//        gCross_atlas_npBW_shift->SetMarkerColor(6);
//        gCross_atlas_npBW_shift->SetLineColor(6);
					SetGraphStyle(gCross_atlas_npBW_shift,9,11);

	TGraphAsymmErrors* gCross_atlas_npBW_sys_shift = new TGraphAsymmErrors(nPtATLAS, atlas_px_npBWShift, atlas_py_npBW, atlas_exsys, atlas_exsys, atlas_eysys01_npBW, atlas_eysys01_npBW);
        gCross_atlas_npBW_sys_shift->SetFillColor(kGray);
//        gCross_atlas_npBW_sys_shift->SetFillStyle(3002);

	if ( shifted) {
          gCross_atlas_npBW_sys_shift->Draw("2");
	  gCross_atlas_npBW_shift->Draw("p");
        }
        else  {
          gCross_atlas_npBW_sys->Draw("2");
          gCross_atlas_npBW->Draw("p");
	}
	
	//REDRAW
	if ( shifted)  { 
	 gCross_atlas_npFW_shift->Draw("p");
	 gCross_atlas_npBW_shift->Draw("p");
	}
	else {
	 gCross_atlas_npFW->Draw("p");
	 gCross_atlas_npBW->Draw("p");
	}
	gCross_np_fwrap->Draw("P");
	gCross_np_bwrap->Draw("P");

// 	gCross_atlas_np_sys = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_np, atlas_exsys, atlas_exsys, atlas_eysys_np, atlas_eysysn_np);	
// 	gCross_atlas_np_sys->SetFillColor(kAzure-9);
// 	gCross_atlas_np_sys->SetFillStyle(3001);
// 	gCross_atlas_np_sys->Draw("2");

// 	SetGraphStyle(gCross_np_fwrap,1,3);
// 	gCross_np_fwrap->Draw("P");

// 	gCross_atlas_np = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_np, atlas_ex, atlas_ex, atlas_ey_np, atlas_ey_np);	
// 	SetGraphStyle(gCross_atlas_np,2,10);
// 	gCross_atlas_np->Draw("P");
	if (isLog){
	  legUL01 -> AddEntry(gCross_np_fwrap,"0 < y_{CM} < 1.93","lp");
	  legUL01 -> AddEntry(gCross_np_bwrap,"-1.93 < y_{CM} < 0","lp");
	  legUL01-> Draw();
		legBL01 -> SetHeader("ATLAS");
	  legBL01 -> AddEntry(gCross_atlas_npFW,"0 < y_{CM} < 1.94","lp");
	  legBL01 -> AddEntry(gCross_atlas_npBW,"-1.94 < y_{CM} < 0","lp");
	  legBL01-> Draw();
	}
	else{
	legUC01 -> AddEntry(gCross_np_fwrap,"CMS"," ");
	legUC01 -> AddEntry(gCross_np_fwrap,"0 < y_{CM} < 1.93","lp");
 	legUC01 -> AddEntry(gCross_np_bwrap,"-1.93 < y_{CM} < 0","lp");
	legUC01 -> Draw();
	legUC02 -> AddEntry(gCross_np_fwrap,"ATLAS"," ");
	legUC02 -> AddEntry(gCross_atlas_npFW,"0 < y_{CM} < 1.94","lp");
	legUC02 -> AddEntry(gCross_atlas_npBW,"-1.94 < y_{CM} < 0","lp");
	legUC02 -> Draw();
	}	
	globtex->SetTextSize(0.045);
	globtex->SetTextFont(62);
	globtex->DrawLatex(0.88, 0.88, "Non-prompt J/#psi");
	
//	if (isLog) {
//		latex->DrawLatex(0.17, 0.28, Form("%s",beamstring.c_str()));
//	}
//	else {
//		latex->DrawLatex(0.89, 0.28, Form("%s",beamstring.c_str()));
//	}	

	CMS_lumi( c_np, iPeriod, iPos );
	c_np->Update();
	c_np->SaveAs(Form("comparisonLHCb/comp_crossSection_pt_ATLAS_np_isLog%d_%d_shifted_%d_middle_xErr.pdf",(int)isLog,(int)isPtCut,(int)shifted));
	c_np->SaveAs(Form("comparisonLHCb/comp_crossSection_pt_ATLAS_np_isLog%d_%d_shifted_%d_middle_xErr.png",(int)isLog,(int)isPtCut,(int)shifted));
	legUL01->Clear();
	legBL01->Clear();
	legUC01->Clear();
	legUC02->Clear();

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
  Double_t atlas_Bin[nAtlasBin+1] = {8, 9.5, 11.5, 14, 20, 30};
  
  TH1D* firhist = (TH1D*)func->GetHistogram();
  for ( int i=1 ; i<= nAtlasBin ; i++) {
    float lowPtCut = atlas_Bin[i-1];
    float highPtCut = atlas_Bin[i];
    
    TH1D* tempHist = (TH1D*)firhist->Clone("tempHist");
    for ( int xbin = 1 ;xbin<=tempHist->GetNbinsX() ; xbin++) {
      if ( (tempHist->GetBinCenter(xbin) > highPtCut) || (tempHist->GetBinCenter(xbin) < lowPtCut) \
	   )
      tempHist->SetBinContent(xbin,0);
    }
    float meanPt = tempHist->GetMean();
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


