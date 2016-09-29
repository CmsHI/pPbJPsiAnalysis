#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

int comp_RFB_rap_LHCb_ALICE_ATLAS()
{

  gROOT->LoadMacro("tdrstyle_kyo.C");
	int isPA = 1;  // 0:pp, 1:pPb
	int iPos=0;
	
  //// BR and lumi info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	const Double_t pPb_lumi_nb = 34.622; // 34.6/nb
	const Double_t pPb_lumi_nb_err = 1.2; // 3.5 %
	const Double_t pPb_lumi_mub = pPb_lumi_nb * 1000; // (nb)^{-1} -> {#mub}^{-1}
	const Double_t pPb_lumi_mub_err = pPb_lumi_nb_err * 1000; // (nb)^{-1} -> {#mub}^{-1}
	
	// read our results
 	TFile *inFile_pr = new TFile("./plot_RFB/RFB_rap_isPrompt1.root");
	TFile *inFile_np = new TFile("./plot_RFB/RFB_rap_isPrompt0.root");
       
	TGraphAsymmErrors* gRFB_pr_sys_lowpt = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_sys_lowpt"); 	
	TGraphAsymmErrors* gRFB_pr_sys_lowpt_line = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_sys_lowpt"); 	
	TGraphAsymmErrors* gRFB_pr_lowpt = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_lowpt"); 	
	TGraphAsymmErrors* gRFB_pr_sys_highpt = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_sys_highpt"); 	
	TGraphAsymmErrors* gRFB_pr_sys_highpt_line = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_sys_highpt"); 	
	TGraphAsymmErrors* gRFB_pr_highpt = (TGraphAsymmErrors*)inFile_pr->Get("gRFB_highpt"); 	
	TGraphAsymmErrors* gRFB_np_sys_lowpt = (TGraphAsymmErrors*)inFile_np->Get("gRFB_sys_lowpt"); 	
	TGraphAsymmErrors* gRFB_np_sys_lowpt_line = (TGraphAsymmErrors*)inFile_np->Get("gRFB_sys_lowpt"); 	
	TGraphAsymmErrors* gRFB_np_lowpt = (TGraphAsymmErrors*)inFile_np->Get("gRFB_lowpt"); 	
	TGraphAsymmErrors* gRFB_np_sys_highpt = (TGraphAsymmErrors*)inFile_np->Get("gRFB_sys_highpt"); 	
	TGraphAsymmErrors* gRFB_np_sys_highpt_line = (TGraphAsymmErrors*)inFile_np->Get("gRFB_sys_highpt"); 	
	TGraphAsymmErrors* gRFB_np_highpt = (TGraphAsymmErrors*)inFile_np->Get("gRFB_highpt"); 	
	gRFB_pr_sys_lowpt->SetName("gRFB_pr_sys_lowpt");
	gRFB_pr_sys_lowpt_line->SetName("gRFB_pr_sys_lowpt_line");
	gRFB_pr_lowpt->SetName("gRFB_pr_lowpt");
	gRFB_pr_sys_highpt->SetName("gRFB_pr_sys_highpt");
	gRFB_pr_sys_highpt_line->SetName("gRFB_pr_sys_highpt_line");
	gRFB_pr_highpt->SetName("gRFB_pr_highpt");
	gRFB_np_sys_lowpt->SetName("gRFB_np_sys_lowpt");
	gRFB_np_sys_lowpt_line->SetName("gRFB_np_sys_lowpt_line");
	gRFB_np_lowpt->SetName("gRFB_np_lowpt");
	gRFB_np_sys_highpt->SetName("gRFB_np_sys_highpt");
	gRFB_np_sys_highpt_line->SetName("gRFB_np_sys_highpt_line");
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

	//TLegend *legUL = new TLegend(0.435, 0.62, 0.86, 0.855); 
	TLegend *legUL = new TLegend(0.18,0.79,0.565,0.89); //upper left
	TLegend *legBL01 = new TLegend(0.18, 0.20, 0.4, 0.32); // for prompt
	TLegend *legBL02 = new TLegend(0.18, 0.16, 0.4, 0.26); // for prompt
	SetLegendStyle(legUL);
	SetLegendStyle(legBL01);
	SetLegendStyle(legBL02);
	legUL->SetTextSize(0.035);
	legBL01->SetTextSize(0.035);
	legBL02->SetTextSize(0.035);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);
	
	//// globtex box for beam, rapidity, pT info
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	//globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

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
	//gRFB_pr_sys_lowpt->SetMinimum(0.5);
	//gRFB_pr_sys_lowpt->SetMaximum(1.4);
	gRFB_pr_sys_lowpt->SetMinimum(0.0);
	gRFB_pr_sys_lowpt->SetMaximum(2.0);
	gRFB_pr_sys_lowpt->SetFillColorAlpha(kRed-10,0.5);	
	gRFB_pr_sys_lowpt_line->SetFillColorAlpha(kRed-10,0.);	
	gRFB_pr_sys_lowpt->SetLineColor(kPink-6);	
	gRFB_pr_sys_lowpt_line->SetLineColor(kPink-6);	
	//gRFB_pr_sys_lowpt->SetFillStyle(3001);
	gRFB_pr_sys_lowpt->Draw("A5");
	gRFB_pr_sys_lowpt_line->Draw("5");
	
	// 2) highpt
	gRFB_pr_sys_highpt->SetFillColorAlpha(kBlue-10,0.5);
	gRFB_pr_sys_highpt_line->SetFillColorAlpha(kBlue-10,0.);
	gRFB_pr_sys_highpt->SetLineColor(kBlue-2);
	gRFB_pr_sys_highpt_line->SetLineColor(kBlue-2);
	//gRFB_pr_sys_highpt->SetFillStyle(3001);
	gRFB_pr_sys_highpt->Draw("5");
	gRFB_pr_sys_highpt_line->Draw("5");

	// 3) lhcb
	gRFB_lhcb_pr_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_exsys, lhcb_exsys, lhcb_eysys_pr, lhcb_eysys_pr);	
	gRFB_lhcb_pr_sys->SetFillColor(kGray);
	//gRFB_lhcb_pr_sys->SetFillStyle(3001);
	gRFB_lhcb_pr_sys->Draw("2");
	
  //4) ALICE
	gRFB_alice_sys = new TGraphAsymmErrors(nBin_alice, alice_px, alice_RFB, alice_exsys, alice_exsys, alice_eysys, alice_eysys);	
	gRFB_alice_sys->SetFillColor(kGray);
	gRFB_alice_sys->SetMarkerSize(2.0);
	gRFB_alice_sys->SetMarkerColor(1);
	gRFB_alice_sys->SetLineColor(1);
	gRFB_alice_sys->SetLineWidth(1);
	gRFB_alice_sys->Draw("2");

  //5) ATLAS
	gRFB_atlas_sys_pr = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_pr, atlas_exsys, atlas_exsys, atlas_eysys_pr, atlas_eysys_pr);	
	gRFB_atlas_sys_pr->SetFillColor(kGray);
	//gRFB_atlas_sys_pr->SetFillStyle(3001);
	gRFB_atlas_sys_pr->SetMarkerSize(0);
	gRFB_atlas_sys_pr->SetLineColor(1);
	gRFB_atlas_sys_pr->SetLineWidth(1);
	gRFB_atlas_sys_pr->Draw("2");
	
  //gRFB_pr_sys_lowpt->Draw("2");
  //gRFB_pr_sys_highpt->Draw("2");
 
  //////////////////////////////////////////////////////////////////////// 
	
  solidLine(0.0,1.0,4.5,1.0,1,1);

	gRFB_lhcb_pr = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_ex, lhcb_ex, lhcb_ey_pr, lhcb_ey_pr);	
	SetGraphStyleFinal(gRFB_lhcb_pr,1,10);
	gRFB_lhcb_pr->SetMarkerSize(1.5);
	gRFB_lhcb_pr->SetMarkerStyle(24);
	gRFB_lhcb_pr->SetMarkerColor(1);
	gRFB_lhcb_pr->SetLineColor(1);
	gRFB_lhcb_pr->Draw("P");
	
  gRFB_alice = new TGraphAsymmErrors(nBin_alice, alice_px, alice_RFB, alice_ex, alice_ex, alice_eysys_stat, alice_eysys_stat);	
	gRFB_alice->SetMarkerSize(1.5);
	gRFB_alice->SetLineColor(1);
	gRFB_alice->SetMarkerColor(1);
	gRFB_alice->SetMarkerStyle(kOpenSquare);
	gRFB_alice->Draw("P");
	
  gRFB_atlas_pr = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_pr, atlas_ex, atlas_ex, atlas_eysys_stat_pr, atlas_eysys_stat_pr);	
	gRFB_atlas_pr->SetMarkerSize(2.0);
	gRFB_atlas_pr->SetMarkerStyle(27);
	gRFB_atlas_pr->SetMarkerColor(1);
	gRFB_atlas_pr->SetLineColor(1);
	gRFB_atlas_pr->Draw("P");
	
  SetGraphStyleFinal(gRFB_pr_lowpt,1,0);
	gRFB_pr_lowpt->SetMarkerSize(1.4);
  gRFB_pr_lowpt->Draw("P");
  
  SetGraphStyleFinal(gRFB_pr_highpt,2,3);
	gRFB_pr_highpt->SetMarkerSize(1.4);
	gRFB_pr_highpt->Draw("P");




	legUL -> AddEntry(gRFB_pr_lowpt,"6.5 < p_{T} < 10 GeV/c","lp");
	legUL -> AddEntry(gRFB_pr_highpt,"10 < p_{T} < 30 GeV/c","lp");
	legBL01 -> AddEntry(gRFB_atlas_pr,"ATLAS: 8 < p_{T} < 30 GeV/c","lp");
	legBL01 -> AddEntry(gRFB_lhcb_pr,"LHCb : p_{T} < 14 GeV/c","lp");
	legBL01 -> AddEntry(gRFB_alice,"ALICE: p_{T} < 14 GeV/c","lp");
	legUL -> Draw();
	legBL01 -> Draw();
	
  globtex->SetTextAlign(12);
	globtex->SetTextSize(0.037);
	globtex->SetTextFont(42);
	globtex->DrawLatex(0.33, 0.176, "inclusive J/#psi");
  
  globtex->SetTextAlign(32);
	globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	globtex->DrawLatex(0.92, 0.85, "Prompt J/#psi");
	
  CMS_lumi( c_pr, isPA, iPos );
	c_pr->Update();
	c_pr->SaveAs("plot_otherExp/comp_RFB_rap_LHCb_ALICE_ATLAS_isPrompt1.pdf");
	c_pr->SaveAs("plot_otherExp/comp_RFB_rap_LHCb_ALICE_ATLAS_isPrompt1.png");
	
  legUL->Clear();
  legBL01->Clear();
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
	gRFB_np_sys_lowpt->SetMinimum(0.0);
	gRFB_np_sys_lowpt->SetMaximum(2.0);
	gRFB_np_sys_lowpt->SetFillColorAlpha(kRed-10,0.5);	
	gRFB_np_sys_lowpt_line->SetFillColorAlpha(kRed-10,0.);	
	gRFB_np_sys_lowpt->SetLineColor(kPink-6);	
	gRFB_np_sys_lowpt_line->SetLineColor(kPink-6);	
	gRFB_np_sys_lowpt->Draw("A5");
	gRFB_np_sys_lowpt_line->Draw("5");
	
	// 2) highpt
	gRFB_np_sys_highpt->SetFillColorAlpha(kBlue-10,0.5);
	gRFB_np_sys_highpt_line->SetFillColorAlpha(kBlue-10,0.);
	gRFB_np_sys_highpt->SetLineColor(kBlue-2);
	gRFB_np_sys_highpt_line->SetLineColor(kBlue-2);
	gRFB_np_sys_highpt->Draw("5");
	gRFB_np_sys_highpt_line->Draw("5");

	// 3) lhcb
	gRFB_lhcb_np_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_exsys, lhcb_exsys, lhcb_eysys_np, lhcb_eysys_np);	
	gRFB_lhcb_np_sys->SetFillColor(kGray);
	gRFB_lhcb_np_sys->SetMarkerColor(1);
	gRFB_lhcb_np_sys->SetLineColor(1);
	gRFB_lhcb_np_sys->Draw("2");
	
	//4) ATLAS
  gRFB_atlas_sys_np = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_np, atlas_exsys, atlas_exsys, atlas_eysys_np, atlas_eysys_np);
	gRFB_atlas_sys_np->SetFillColor(kGray);
	gRFB_atlas_sys_np->SetMarkerSize(0);
	gRFB_atlas_sys_np->SetLineColor(1);
	gRFB_atlas_sys_np->SetLineWidth(1);
	gRFB_atlas_sys_np->Draw("2");
	
	//gRFB_np_sys_highpt->Draw("2");
	//gRFB_np_sys_highpt->Draw("2");
	
  //////////////////////////////////////////////////////////////////////////
	
  solidLine(0.0,1.0,4.5,1.0,1,1);
  
	gRFB_lhcb_np = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_ex, lhcb_ex, lhcb_ey_np, lhcb_ey_np);	
	SetGraphStyleFinal(gRFB_lhcb_np,1,10);
	gRFB_lhcb_np->SetMarkerSize(1.5);
	gRFB_lhcb_np->SetMarkerColor(1);
	gRFB_lhcb_np->SetLineColor(1);
	gRFB_lhcb_np->Draw("P");

	gRFB_atlas_np = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_np, atlas_ex, atlas_ex, atlas_eysys_stat_np, atlas_eysys_stat_np);	
	gRFB_atlas_np->SetMarkerSize(2.0);
	gRFB_atlas_np->SetMarkerStyle(27);
	gRFB_atlas_np->SetMarkerColor(1);
	gRFB_atlas_np->SetLineColor(1);
	gRFB_atlas_np->Draw("P");	
	
  SetGraphStyleFinal(gRFB_np_lowpt,1,0);
	gRFB_np_lowpt->SetMarkerSize(1.4);
	gRFB_np_lowpt->Draw("P");
  
  SetGraphStyleFinal(gRFB_np_highpt,2,3);
	gRFB_np_highpt->SetMarkerSize(1.4);
	gRFB_np_highpt->Draw("P");


	
  legUL -> AddEntry(gRFB_pr_lowpt,"6.5 < p_{T} < 10 GeV/c","lp");
	legUL -> AddEntry(gRFB_pr_highpt,"10 < p_{T} < 30 GeV/c","lp");
	legBL02 -> AddEntry(gRFB_atlas_pr,"ATLAS: 8 < p_{T} < 30 GeV/c","lp");
	legBL02 -> AddEntry(gRFB_lhcb_pr,"LHCb : p_{T} < 14 GeV/c","lp");
	legUL -> Draw();
	legBL02 -> Draw();

  globtex->SetTextAlign(32);
	globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	globtex->DrawLatex(0.92, 0.85, "Nonprompt J/#psi");
	
  CMS_lumi( c_np, isPA, iPos );
	c_np->Update();
	c_np->SaveAs("plot_otherExp/comp_RFB_rap_LHCb_ALICE_ATLAS_isPrompt0.pdf");
	c_np->SaveAs("plot_otherExp/comp_RFB_rap_LHCb_ALICE_ATLAS_isPrompt0.png");
	//legUL->Clear();
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
