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

using namespace std;

const int nAtlasBin = 5;

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);
void getMeanPtBins( Double_t* binArr, TF1* func);

int comp_crossSection_pt_ATLAS_pointMoved(bool shifted=false, Int_t runCode=0, bool isScale =false, bool isLog=true, bool isPtCut = false)
{
	gROOT->Macro("./JpsiStyleForFinalResult.C");
	cout << "HEEEEEEEEELLO"<<endl;
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
	//TFile *inFile = new TFile("cross_8rap9pt/crossSection_pt_isLog0_isScale0.root");
	//TFile *inFile = new TFile("cross_8rap10pt/crossSection_pt_isLog0_isScale0.root");
	//TFile *inFile = new TFile("/afs/cern.ch/work/k/kyolee/public/forLamia/cross_8rap9pt2gev/crossSection_pt_isLog0_isScale0.root");
	//TFile *inFile = new TFile("/afs/cern.ch/work/k/kyolee/public/forLamia/cross_8rap9pt2gev/crossSection_rap_integ_isScale0.root");



	//TFile *inFile = new TFile("/afs/cern.ch/work/k/kyolee/public/forLamia/cross_8rap9pt2gev/crossSection_pt_integ_isLog0_isScale0.root");
	TFile *inFile = new TFile("cross_8rap9pt2gev/crossSection_pt_integ_isLog0_isScale0.root");
	//TFile *inFile = new TFile("/afs/cern.ch/work/k/kyolee/public/forLamia/cross_8rap9pt2gev/crossSection_pt_isLog0_isScale0.root");
	TGraphAsymmErrors* gCross_pr_fwrap = (TGraphAsymmErrors*)inFile->Get("gCross_pr_fwrap"); 
	TGraphAsymmErrors* gCross_np_fwrap = (TGraphAsymmErrors*)inFile->Get("gCross_np_fwrap"); 
	TGraphAsymmErrors* gCross_pr_bwrap = (TGraphAsymmErrors*)inFile->Get("gCross_pr_bwrap"); 
	TGraphAsymmErrors* gCross_np_bwrap = (TGraphAsymmErrors*)inFile->Get("gCross_np_bwrap"); 

	TGraphAsymmErrors* gCross_pr_sys_fwrap = (TGraphAsymmErrors*)inFile->Get("gCross_pr_sys_fwrap"); 
	TGraphAsymmErrors* gCross_np_sys_fwrap = (TGraphAsymmErrors*)inFile->Get("gCross_np_sys_fwrap"); 
	TGraphAsymmErrors* gCross_pr_sys_bwrap = (TGraphAsymmErrors*)inFile->Get("gCross_pr_sys_bwrap"); 
	TGraphAsymmErrors* gCross_np_sys_bwrap = (TGraphAsymmErrors*)inFile->Get("gCross_np_sys_bwrap"); 




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


	Double_t atlas_px[] = {8.75, 10.5, 12.75, 17.0, 25.0};
	//Double_t atlas_px_binW[] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 3.5};
	Double_t atlas_ex[] = {0., 0., 0., 0., 0., 0., 0., 0.};
	Double_t atlas_py_prFW[] = {408, 159, 55.5, 11.9, 1.13};
	Double_t atlas_py_prBW[] = {414, 173, 58.2, 11.8, 1.41};

	Double_t atlas_py_npFW[] = {136, 69.9, 29.2, 9.06, 1.48 };
	Double_t atlas_py_npBW[] = { 167, 69.1, 32.3, 9.28, 1.43};


	Double_t atlas_ey_prFW[] = { 12, 4, 1.5, 0.3, 0.08 };
	Double_t atlas_ey_prBW[] = { 12, 4, 1.4, 0.4, 0.08 };
	Double_t atlas_ey_npFW[] = { 8, 2.8, 1.3, 0.33, 0.9 };
	Double_t atlas_ey_npBW[] = {9, 2.6, 1.2, 0.33, 0.08};

	Double_t atlas_exsys[] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};
	Double_t atlas_eysys01_prFW[] = {50.0, 15.0 , 5.7, 0.9, 0.07};
	Double_t atlas_eysys02_prFW[] = { 11, 4, 1.5, 0.3, 0.03 };
	Double_t atlas_eysys01_prBW[] = { 39, 16, 4.3, 0.8, 0.10};
	Double_t atlas_eysys02_prBW[] = { 11, 5, 1.6, 0.3, 0.04};

	Double_t atlas_eysys01_npFW[] = { 17, 6.6, 3.0, 0.7, 0.09};
	Double_t atlas_eysys02_npFW[] = { 4, 1.9, 0.8, 0.24, 0.04};
	Double_t atlas_eysys01_npBW[] = {16, 6.3, 2.4, 0.63, 0.10 };
	Double_t atlas_eysys02_npBW[] = {5, 1.9, 0.9, 0.25, 0.04 };
	
	const Int_t nBin_atlas = sizeof(atlas_px)/sizeof(Double_t);
	cout << "nBin_atlas : " << nBin_atlas << endl; 


// 	Double_t dummy01[]={0,0,0,0,0,0,0,0};
// 	Double_t dummy02[]={0,0,0,0,0,0,0,0};

	////////////////////
	////// Draw Plots
	////////////////////

	//TLegend *legUR = new TLegend(0.52, 0.55, 0.85, 0.92); //upper left
	//TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
	//TLegend *legUR = new TLegend(0.52, 0.63, 0.86, 0.92); //upper left
	TLegend *legUR1 = new TLegend(0.419598,0.4503497,0.7600503,0.9405594); //upper left
	SetLegendStyle(legUR1);
	TLegend *legUR11 = new TLegend(0.619598,0.4503497,0.8600503,0.9405594); //upper left
	SetLegendStyle(legUR11);
	TLegend *legUR2 = new TLegend(0.419598,0.4503497,0.7600503,0.9405594); //upper left
	SetLegendStyle(legUR2);
	TLegend *legUR22 = new TLegend(0.619598,0.4503497,0.8600503,0.9405594); //upper left
	SetLegendStyle(legUR22);

	TLegend *legRR = new TLegend(0.52, 0.63, 0.86, 0.92); //upper left
	SetLegendStyle(legRR);

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

	TCanvas *c_pr = new TCanvas("c_pr","", 200, 10, 800, 600);
	c_pr->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);

	TH1D *dummy1 = new TH1D("","",50,0.,400.);
	dummy1->GetXaxis()->CenterTitle();
	dummy1->GetXaxis()->SetLimits(5.0, 30.0);
	dummy1->SetYTitle("d^{2}#sigma/dp_{T}dy x BR (J/#psi #rightarrow #mu #mu) [ #mub/(GeV/c)]");
	dummy1->SetXTitle("p_{T} [GeV/c]");

 	if (isLog){
 	  //dummy1->GetYaxis()->SetLimits(0.5, 1000.0);
	dummy1->SetMinimum(0.1);
	dummy1->SetMaximum(10000);
	//dummy1->SetMaximum(2000);
 	}
 	else {
	  dummy1->SetMinimum(-20.0);
	  dummy1->SetMaximum(1500.0);
	}
	//dummy1->GetYaxis()->SetLimits(0.5, 600.0);
	
	dummy1->Draw();
	
	// 	if (!isLog) {
	// 		if (isPtCut) dashedLine(3.0,0.0,30.0,0.0,1,1);
	// 		else dashedLine(0.0,0.0,30.0,0.0,1,1);
	// 	}
	

	gCross_atlas_prFW_sys = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_prFW, atlas_exsys, atlas_exsys, atlas_eysys01_prFW, atlas_eysys01_prFW);	
	gCross_atlas_prFW_sys->SetFillColor(kMagenta-9);
	gCross_atlas_prFW_sys->SetFillStyle(3001);

	
	gCross_atlas_prFW = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_prFW, atlas_ex, atlas_ex, atlas_ey_prFW, atlas_ey_prFW);	
	gCross_atlas_prFW->SetMarkerSize(1.5);
	gCross_atlas_prFW->SetMarkerStyle(33);
	gCross_atlas_prFW->SetMarkerColor(6);
	gCross_atlas_prFW->SetLineColor(6);
	//SetGraphStyle(gCross_atlas_prFW,2,10);
	TF1 *expoFunc_prFW = new TF1("expoFunc_prFW","[0]*exp(-[1]*x)", 8, 30);
	expoFunc_prFW->SetParameter(0,10000);
	expoFunc_prFW->SetParameter(1,1.5);
	expoFunc_prFW->SetLineColor(6);
	gCross_atlas_prFW->Fit("expoFunc_prFW","R LL M");

	double atlas_px_prFWShift[nAtlasBin];
	getMeanPtBins( atlas_px_prFWShift, expoFunc_prFW);
	TGraphAsymmErrors* gCross_atlas_prFW_shift =  new TGraphAsymmErrors(nBin_atlas, atlas_px_prFWShift, atlas_py_prFW, atlas_ex, atlas_ex, atlas_ey_prFW, atlas_ey_prFW);
	gCross_atlas_prFW_shift->SetMarkerSize(1.5);
	gCross_atlas_prFW_shift->SetMarkerStyle(33);
        gCross_atlas_prFW_shift->SetMarkerColor(6);
        gCross_atlas_prFW_shift->SetLineColor(6);
	
	TGraphAsymmErrors* gCross_atlas_prFW_sys_shift = new TGraphAsymmErrors(nBin_atlas, atlas_px_prFWShift, atlas_py_prFW, atlas_exsys, atlas_exsys, atlas_eysys01_prFW, atlas_eysys01_prFW);
	gCross_atlas_prFW_sys_shift->SetFillColor(kMagenta-9);
	gCross_atlas_prFW_sys_shift->SetFillStyle(3001);
	

	if ( shifted)  { 
	  gCross_atlas_prFW_sys_shift->Draw("2");
	  gCross_atlas_prFW_shift->Draw("p");
	}
	else {
	  gCross_atlas_prFW_sys->Draw("2");
	  gCross_atlas_prFW->Draw("P");
	}



	///
	gCross_atlas_prBW_sys = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_prBW, atlas_exsys, atlas_exsys, atlas_eysys01_prBW, atlas_eysys01_prBW);	
	gCross_atlas_prBW_sys->SetFillColor(kMagenta-5);
	gCross_atlas_prBW_sys->SetFillStyle(3001);

	SetGraphStyle(gCross_pr_fwrap,1,3);

	gCross_pr_fwrap->Draw("P");

	gCross_atlas_prBW = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_prBW, atlas_ex, atlas_ex, atlas_ey_prBW, atlas_ey_prBW);	
	gCross_atlas_prBW->SetMarkerSize(1.5);
	gCross_atlas_prBW->SetMarkerStyle(27);
	gCross_atlas_prBW->SetMarkerColor(6);
	gCross_atlas_prBW->SetLineColor(6);	
//	SetGraphStyle(gCross_atlas_prBW,2,10);
	TF1 *expoFunc_prBW = new TF1("expoFunc_prBW","[0]*exp(-[1]*x)", 8, 30);
	expoFunc_prBW->SetParameter(0,10000);
	expoFunc_prBW->SetParameter(1,1.5);
	expoFunc_prBW->SetLineColor(6);
	expoFunc_prBW->SetLineStyle(2);
	gCross_atlas_prBW->Fit("expoFunc_prBW","R LL M");

	double atlas_px_prBWShift[nAtlasBin];
        getMeanPtBins( atlas_px_prBWShift, expoFunc_prBW);
        TGraphAsymmErrors* gCross_atlas_prBW_shift =  new TGraphAsymmErrors(nBin_atlas, atlas_px_prBWShift, atlas_py_prBW, atlas_ex, atlas_ex, atlas_ey_prBW, atlas_ey_prBW);
        gCross_atlas_prBW_shift->SetMarkerSize(1.5);
        gCross_atlas_prBW_shift->SetMarkerStyle(27);
        gCross_atlas_prBW_shift->SetMarkerColor(6);
        gCross_atlas_prBW_shift->SetLineColor(6);

        TGraphAsymmErrors* gCross_atlas_prBW_sys_shift = new TGraphAsymmErrors(nBin_atlas, atlas_px_prBWShift, atlas_py_prBW, atlas_exsys, atlas_exsys, atlas_eysys01_prBW, atlas_eysys01_prBW);
        gCross_atlas_prBW_sys_shift->SetFillColor(kMagenta-5);
        gCross_atlas_prBW_sys_shift->SetFillStyle(3001);
       
	if ( shifted)  { 
	  gCross_atlas_prBW_sys_shift->Draw("2");
	  gCross_atlas_prBW_shift->Draw("p");
	}
	else {
	  gCross_atlas_prBW_sys->Draw("2");
	  gCross_atlas_prBW->Draw("P");
	}
	
	gCross_pr_sys_fwrap->SetFillColor(kRed+2);
	gCross_pr_sys_fwrap->SetFillStyle(3001);
	gCross_pr_sys_fwrap->Draw("2");
	gCross_pr_sys_bwrap->SetFillColor(kRed-10);
	gCross_pr_sys_bwrap->SetFillStyle(3001);
	gCross_pr_sys_bwrap->Draw("2");

	SetGraphStyle(gCross_pr_fwrap,1,3);
	SetGraphStyle(gCross_pr_bwrap,1,13);
	gCross_pr_fwrap->Draw("EP");
	gCross_pr_bwrap->Draw("EP");

	if (isLog){
	  legUR11 -> SetHeader("Prompt J/#psi");
	  legUR11 -> AddEntry(gCross_pr_fwrap,"CMS Preliminary"," ");
	  legUR11 -> AddEntry(gCross_pr_fwrap,"0 < y_{CM} < 1.93","lp");
	  legUR11 -> AddEntry(gCross_pr_bwrap,"-1.93 < y_{CM} < 0","lp");
	  legUR11 -> AddEntry(gCross_pr_fwrap,"ATLAS"," ");
	  legUR11 -> AddEntry(gCross_atlas_prFW,"0 < y_{CM} < 1.93","lp");
	  legUR11 -> AddEntry(gCross_atlas_prBW,"-1.94 < y_{CM} < 0","lp");
	  
	  legUR11-> Draw();
	}

       else{
 	legUR1 -> SetHeader("Prompt J/#psi");
	legUR1 -> AddEntry(gCross_pr_fwrap,"CMS Preliminary"," ");
	legUR1 -> AddEntry(gCross_pr_fwrap,"0 < y_{CM} < 1.93","lp");
 	legUR1 -> AddEntry(gCross_pr_bwrap,"-1.93 < y_{CM} < 0","lp");
	legUR1 -> AddEntry(gCross_pr_fwrap,"ATLAS"," ");
	legUR1 -> AddEntry(gCross_atlas_prFW,"0 < y_{CM} < 1.93","lp");
	legUR1 -> AddEntry(gCross_atlas_prBW,"-1.94 < y_{CM} < 0","lp");
	//legUR -> AddEntry(gCross_pr_fwrap,"CMS Preliminary : 0 < y_{CM} < 1.93","lp");
 	//legUR -> AddEntry(gCross_pr_bwrap,"CMS Preliminary : -1.93 < y_{CM} < 0","lp");
 	//legUR -> AddEntry(gCross_atlas_prFW,"ATLAS : 0 < y_{CM} < 1.94","lp");
 	//legUR -> AddEntry(gCross_atlas_prBW,"ATLAS : -1.94 < y_{CM} < 0","lp");
 	legUR1-> Draw();
       }
	/*
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.56, 0.71, lumistring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.56, 0.78, beamstring.c_str());
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.55, 0.86, cmsstring.c_str());
	*/
	if (isLog){
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.2, 0.2, beamstring.c_str());
	}
	else {
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.6, 0.2, beamstring.c_str());
	}

	c_pr->Update();
	c_pr->SaveAs(Form("comparisonLHCb/comp_crossSection_pt_ATLAS_pr_isLog%d_%d_shifted_%d.pdf",(int)isLog,(int)isPtCut,(int)shifted));
	c_pr->SaveAs(Form("comparisonLHCb/comp_crossSection_pt_ATLAS_pr_isLog%d_%d_shifted_%d.png",(int)isLog,(int)isPtCut,(int)shifted));
	legUR1->Clear();
	//c_pr->Clear();


	////// 02 ////////
	// non-prompt
	TCanvas *c_np = new TCanvas("c_np","", 200, 10, 800, 600);
	c_np->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);

	TH1D *dummy2 = new TH1D("","",50,0.,400.);
	//dummy2->SetMinimum(0.5);
	//dummy2->SetMaximum(1000);
	dummy2->GetXaxis()->CenterTitle();
	dummy2->GetXaxis()->SetLimits(5.0, 30.0);
	dummy2->SetYTitle("d^{2}#sigma/dp_{T}dy x BR (J/#psi #rightarrow #mu #mu) [ #mub/GeV/c]");
	dummy2->SetXTitle("p_{T} [GeV/c]");

 	if (isLog){
 	  //dummy2->GetYaxis()->SetLimits(0.5, 1000.0);
	dummy2->SetMinimum(0.5);
	dummy2->SetMaximum(1000);
 	}
 	else {
	  dummy2->SetMinimum(-10.0);
	  dummy2->SetMaximum(350);
	}
	//dummy2->GetYaxis()->SetLimits(0.5, 600.0);
	
	dummy2->Draw();


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
	gCross_np_sys_fwrap->SetFillColor(kRed+2);
	gCross_np_sys_fwrap->SetFillStyle(3001);
	gCross_np_sys_bwrap->SetFillColor(kRed-10);
	gCross_np_sys_bwrap->SetFillStyle(3001);
	gCross_np_sys_fwrap->Draw("2");
	gCross_np_sys_bwrap->Draw("2");

	SetGraphStyle(gCross_np_fwrap,1,3);
	gCross_np_fwrap->Draw("P");

	SetGraphStyle(gCross_np_bwrap,1,13);
	gCross_np_bwrap->Draw("P");
	
	
// 	if (!isLog) {
// 		if (isPtCut) dashedLine(3.0,0.0,30.0,0.0,1,1);
// 		else dashedLine(0.0,0.0,30.0,0.0,1,1);
// 	}

	//ATLAS

	gCross_atlas_npFW_sys = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_npFW, atlas_exsys, atlas_exsys, atlas_eysys01_npFW, atlas_eysys01_npFW);	
	gCross_atlas_npFW_sys->SetFillColor(kMagenta-9);
	gCross_atlas_npFW_sys->SetFillStyle(3001);

	//SetGraphStyle(gCross_np_fwrap,1,3);
	//gCross_np_fwrap->Draw("P");

	gCross_atlas_npFW = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_npFW, atlas_ex, atlas_ex, atlas_ey_npFW, atlas_ey_npFW);	
	gCross_atlas_npFW->SetMarkerSize(1.5);
	gCross_atlas_npFW->SetMarkerStyle(33);
	gCross_atlas_npFW->SetMarkerColor(6);
	gCross_atlas_npFW->SetLineColor(6);	
//SetGraphStyle(gCross_atlas_npFW,2,10);

	TF1 *expoFunc_npFW = new TF1("expoFunc_npFW","[0]*exp(-[1]*x)", 8, 30);
	expoFunc_npFW->SetParameter(0,10000);
	expoFunc_npFW->SetParameter(1,1.5);
	expoFunc_npFW->SetLineColor(6);
	gCross_atlas_npFW->Fit("expoFunc_npFW","R LL M");

	double atlas_px_npFWShift[nAtlasBin];
        getMeanPtBins( atlas_px_npFWShift, expoFunc_npFW);
        TGraphAsymmErrors* gCross_atlas_npFW_shift =  new TGraphAsymmErrors(nBin_atlas, atlas_px_npFWShift, atlas_py_npFW, atlas_ex, atlas_ex, atlas_ey_npFW, atlas_ey_npFW);
        gCross_atlas_npFW_shift->SetMarkerSize(1.5);
        gCross_atlas_npFW_shift->SetMarkerStyle(33);
        gCross_atlas_npFW_shift->SetMarkerColor(6);
        gCross_atlas_npFW_shift->SetLineColor(6);

	TGraphAsymmErrors* gCross_atlas_npFW_sys_shift = new TGraphAsymmErrors(nBin_atlas, atlas_px_npFWShift, atlas_py_npFW, atlas_exsys, atlas_exsys, atlas_eysys01_npFW, atlas_eysys01_npFW);
	gCross_atlas_npFW_sys_shift->SetFillColor(kMagenta-9);
	gCross_atlas_npFW_sys_shift->SetFillStyle(3001);


	if ( shifted) {
	  gCross_atlas_npFW_sys_shift->Draw("2");
	  gCross_atlas_npFW_shift->Draw("p");
	} 
	else  {
	  gCross_atlas_npFW_sys->Draw("2");
	  gCross_atlas_npFW->Draw("p");
	}



	///
	gCross_atlas_npBW_sys = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_npBW, atlas_exsys, atlas_exsys, atlas_eysys01_npBW, atlas_eysys01_npBW);	
	gCross_atlas_npBW_sys->SetFillColor(kMagenta-5);
	gCross_atlas_npBW_sys->SetFillStyle(3001);

	SetGraphStyle(gCross_np_fwrap,1,3);
	gCross_np_fwrap->Draw("P");

	gCross_atlas_npBW = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_npBW, atlas_ex, atlas_ex, atlas_ey_npBW, atlas_ey_npBW);
	gCross_atlas_npBW->SetMarkerSize(1.5);
	gCross_atlas_npBW->SetMarkerStyle(27);
	gCross_atlas_npBW->SetMarkerColor(6);
	gCross_atlas_npBW->SetLineColor(6);
	
	//	SetGraphStyle(gCross_atlas_npBW,2,10);
	
	TF1 *expoFunc_npBW = new TF1("expoFunc_npBW","[0]*exp(-[1]*x)", 8, 30);
	expoFunc_npBW->SetParameter(0,10000);
	expoFunc_npBW->SetParameter(1,1.5);
	expoFunc_npBW->SetLineColor(6);
	expoFunc_npBW->SetLineStyle(2);
	gCross_atlas_npBW->Fit("expoFunc_npBW","R LL M");

	double atlas_px_npBWShift[nAtlasBin];
	getMeanPtBins( atlas_px_npBWShift, expoFunc_npBW);
        TGraphAsymmErrors* gCross_atlas_npBW_shift =  new TGraphAsymmErrors(nBin_atlas, atlas_px_npBWShift, atlas_py_npBW, atlas_ex, atlas_ex, atlas_ey_npBW, atlas_ey_npBW);
        gCross_atlas_npBW_shift->SetMarkerSize(1.5);
        gCross_atlas_npBW_shift->SetMarkerStyle(33);
        gCross_atlas_npBW_shift->SetMarkerColor(6);
        gCross_atlas_npBW_shift->SetLineColor(6);

	TGraphAsymmErrors* gCross_atlas_npBW_sys_shift = new TGraphAsymmErrors(nBin_atlas, atlas_px_npBWShift, atlas_py_npBW, atlas_exsys, atlas_exsys, atlas_eysys01_npBW, atlas_eysys01_npBW);
        gCross_atlas_npBW_sys_shift->SetFillColor(kMagenta-5);
        gCross_atlas_npBW_sys_shift->SetFillStyle(3001);

	if ( shifted) {
          gCross_atlas_npBW_sys_shift->Draw("2");
	  gCross_atlas_npBW_shift->Draw("p");
        }
        else  {
          gCross_atlas_npBW_sys->Draw("2");
          gCross_atlas_npBW->Draw("p");
	}



// 	gCross_atlas_np_sys = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_np, atlas_exsys, atlas_exsys, atlas_eysys_np, atlas_eysysn_np);	
// 	gCross_atlas_np_sys->SetFillColor(kAzure-9);
// 	gCross_atlas_np_sys->SetFillStyle(3001);
// 	gCross_atlas_np_sys->Draw("2");

// 	SetGraphStyle(gCross_np_fwrap,1,3);
// 	gCross_np_fwrap->Draw("P");

// 	gCross_atlas_np = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_py_np, atlas_ex, atlas_ex, atlas_ey_np, atlas_ey_np);	
// 	SetGraphStyle(gCross_atlas_np,2,10);
// 	gCross_atlas_np->Draw("P");
	if (isLog){
	  legUR22 -> SetHeader("Non-Prompt J/#psi");
	  legUR22 -> AddEntry(gCross_np_fwrap,"CMS Npeliminary"," ");
	  legUR22 -> AddEntry(gCross_np_fwrap,"0 < y_{CM} < 1.93","lp");
	  legUR22 -> AddEntry(gCross_np_bwrap,"-1.93 < y_{CM} < 0","lp");
	  legUR22 -> AddEntry(gCross_np_fwrap,"ATLAS"," ");
	  legUR22 -> AddEntry(gCross_atlas_npFW,"0 < y_{CM} < 1.93","lp");
	  legUR22 -> AddEntry(gCross_atlas_npBW,"-1.94 < y_{CM} < 0","lp");
	  legUR22-> Draw();
	}
	else{
	legUR2 -> SetHeader("Non-prompt J/#psi");
	legUR2 -> AddEntry(gCross_np_fwrap,"CMS Preliminary"," ");
	legUR2 -> AddEntry(gCross_np_fwrap,"0 < y_{CM} < 1.93","lp");
 	legUR2 -> AddEntry(gCross_np_bwrap,"-1.93 < y_{CM} < 0","lp");
	legUR2 -> AddEntry(gCross_np_fwrap,"ATLAS"," ");
	legUR2 -> AddEntry(gCross_atlas_npFW,"0 < y_{CM} < 1.93","lp");
	legUR2 -> AddEntry(gCross_atlas_npBW,"-1.94 < y_{CM} < 0","lp");


// 	legUR2 -> AddEntry(gCross_np_fwrap,"CMS Preliminary : 0 < y_{CM} < 1.93","lp");
// 	legUR2 -> AddEntry(gCross_np_bwrap,"CMS Preliminary : -1.93 < y_{CM} < 0","lp");
// 	legUR2 -> AddEntry(gCross_atlas_npFW,"ATLAS : 0 < y_{CM} < 1.94","lp");
// 	legUR2 -> AddEntry(gCross_atlas_npBW,"ATLAS : -1.94 < y_{CM} < 0","lp");
	legUR2 -> Draw();
	}	
	if (isLog){
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.2, 0.2, beamstring.c_str());
	}
	else {
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.6, 0.2, beamstring.c_str());
	}


// 	latex->SetTextSize(0.04);
// 	latex->DrawLatex(0.56, 0.71, "");
	/*latex->SetTextSize(0.04);
	latex->DrawLatex(0.56, 0.78, beamstring.c_str());
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.55, 0.86, cmsstring.c_str());
	*/
	c_np->Update();
	c_np->SaveAs(Form("comparisonLHCb/comp_crossSection_pt_ATLAS_np_isLog%d_%d_shifted_%d.pdf",(int)isLog,(int)isPtCut,(int)shifted));
	c_np->SaveAs(Form("comparisonLHCb/comp_crossSection_pt_ATLAS_np_isLog%d_%d_shifted_%d.png",(int)isLog,(int)isPtCut,(int)shifted));
	legUR2->Clear();
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
