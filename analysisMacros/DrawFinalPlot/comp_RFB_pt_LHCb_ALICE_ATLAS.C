#include "CMS_lumi.h"
#include "../SONGKYO.h"

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

int comp_RFB_pt_LHCb_ALICE_ATLAS()
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
	TFile *inFile_pr = new TFile("./plot_RFB/RFB_pt_fineBinTest_isPrompt1.root");
	TFile *inFile_np = new TFile("./plot_RFB/RFB_pt_fineBinTest_isPrompt0.root");
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

	//Double_t alice_px[] = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 8, 10, 13};
	Double_t alice_px[] = {0.7, 1.7, 2.7, 3.7, 4.7, 5.7, 6.7, 8.2, 10.2, 13.2}; ////bins shifted by 0.2
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

	//// x point plotted at the middle of the bin
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

	TLegend *legBR = new TLegend(0.52, 0.195, 0.84, 0.315);
	TLegend *legBR2 = new TLegend(0.52, 0.195, 0.84, 0.315);
	TLegend *legUL = new TLegend(0.20, 0.78, 0.40, 0.90);
	//TLegend *legUL = new TLegend(0.61, 0.74, 0.89, 0.81);
	SetLegendStyle(legBR);
	SetLegendStyle(legBR2);
	SetLegendStyle(legUL);
	legBR->SetTextSize(0.035);
	legBR2->SetTextSize(0.035);
	legUL->SetTextSize(0.037);
  legBR->SetTextFont(42);
  legBR2->SetTextFont(42);
  legUL->SetTextFont(42);

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
	//// prompt
	TCanvas *c_pr = new TCanvas("c_pr","", 200, 10, 600, 600);

	c_pr->cd();
	gPad->SetLogy(0);

	TH1D *dummy = new TH1D("","",50,0.,400.);
	dummy->SetMinimum(0.0);
	dummy->SetMaximum(2.0);
	dummy->GetXaxis()->CenterTitle();
	dummy->GetYaxis()->CenterTitle();
	dummy->GetXaxis()->SetLimits(0.0, 32.0);
	dummy->SetXTitle("p_{T} [GeV/c]");
	dummy->SetYTitle("R_{FB}");
	dummy->Draw();

	// 2) lhcb
	gRFB_lhcb_pr_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_exsys, lhcb_exsys, lhcb_eysys_pr, lhcb_eysys_pr);	
	gRFB_lhcb_pr_sys->SetFillColor(kGray);
	//gRFB_lhcb_pr_sys->SetFillStyle(3001);

	gRFB_lhcb_pr = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_pr, lhcb_ex, lhcb_ex, lhcb_ey_pr, lhcb_ey_pr);	
	SetGraphStyleFinal(gRFB_lhcb_pr,2,10);
	gRFB_lhcb_pr->SetLineColor(1);
	gRFB_lhcb_pr->SetMarkerColor(1);
	gRFB_lhcb_pr->SetMarkerSize(1.5);
	//gRFB_lhcb_pr->SetMarkerStyle(20);

	//3) ALICE
	gRFB_alice = new TGraphAsymmErrors(nBin_alice, alice_px, alice_RFB, alice_ex, alice_ex, alice_ey_stat, alice_ey_stat);	
	gRFB_alice->SetMarkerSize(1.5);
	gRFB_alice->SetLineColor(1);
	gRFB_alice->SetMarkerColor(1);
	gRFB_alice->SetMarkerStyle(kOpenTriangleUp);

	gRFB_alice_sys = new TGraphAsymmErrors(nBin_alice, alice_px, alice_RFB, alice_exsys, alice_exsys, alice_ey_sys, alice_ey_sys);	
	gRFB_alice_sys->SetFillColor(kGray);
	//gRFB_alice_sys->SetMarkerSize(1.5);
	//gRFB_alice_sys->SetMarkerColor(1);
	//gRFB_alice_sys->SetLineColor(1);
	//gRFB_alice_sys->SetLineWidth(1);
	
	//4) ATLAS
	gRFB_atlas_pr = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_pr, atlas_ex, atlas_ex, atlas_ey_stat_pr, atlas_ey_stat_pr);	
	gRFB_atlas_pr->SetMarkerSize(2.0);
	gRFB_atlas_pr->SetMarkerStyle(27);
	gRFB_atlas_pr->SetMarkerColor(1);
	gRFB_atlas_pr->SetLineColor(1);
	//gRFB_atlas_pr->SetMarkerColor(2);
	//gRFB_atlas_pr->SetLineColor(2);

	gRFB_atlas_sys_pr = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_pr, atlas_exsys, atlas_exsys, atlas_ey_sys_pr, atlas_ey_sys_pr);	
	gRFB_atlas_sys_pr->SetFillColor(kGray);
	//gRFB_atlas_sys_pr->SetMarkerSize(0);
	//gRFB_atlas_sys_pr->SetLineColor(1);
	//gRFB_atlas_sys_pr->SetLineWidth(1);

	// 1) CMS
	//gRFB_pr_sys_0->SetLineColor(kGreen+3);
	//gRFB_pr_sys_0->SetFillColor(kGreen-10);
	//gRFB_pr_0->SetMarkerSize(1.5);
	//gRFB_pr_0->SetMarkerColor(kGreen+3);
	//gRFB_pr_0->SetMarkerStyle(kFullSquare);
	//gRFB_pr_0->SetLineColor(kGreen+3);


  /////////////////////////////////	
	gRFB_lhcb_pr_sys->Draw("2");
	gRFB_alice_sys->Draw("2");
	gRFB_atlas_sys_pr->Draw("2");
  gRFB_pr_sys_0->Draw("5");
	gRFB_lhcb_pr->Draw("P");
	gRFB_alice->Draw("P");
	gRFB_atlas_pr->Draw("P");
	gRFB_pr_0->Draw("P");

	dashedLine(0.0,1.0,32.0,1.0,1,1);
	legUL->AddEntry(gRFB_pr_0,"1.5 < |y_{CM}| < 1.93","lp");
	legUL->Draw("SAME");
	legBR->AddEntry(gRFB_atlas_pr,"ATLAS: |y_{CM}|<1.94","lp");
	legBR->AddEntry(gRFB_lhcb_pr,"LHCb: 2.5<|y_{CM}|<4 ","lp");
	legBR->AddEntry(gRFB_alice,"ALICE : 2.96<|y_{CM}|<3.53 ","lp");
	legBR->Draw("SAME");
	globtex->SetTextSize(0.035);
	globtex->SetTextFont(42);
	globtex->SetTextAlign(12); //1:left, 2:vertical center
	globtex->DrawLatex(0.69, 0.17, "inclusive J/#psi");
	
	globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
  globtex->SetTextAlign(32); //3:right 2:vertical center
	globtex->DrawLatex(0.89, 0.85, "Prompt J/#psi");
	
  CMS_lumi( c_pr, isPA, iPos );
	c_pr->Update();
	c_pr->SaveAs("plot_otherExp/comp_RFB_pt_LHCb_ALICE_ATLAS_isPrompt1.pdf");
	c_pr->SaveAs("plot_otherExp/comp_RFB_pt_LHCb_ALICE_ATLAS_isPrompt1.png");

	////////////////////////////////////////////////////////////////////////////////////
	//// non-prompt
	
	TCanvas *c_np = new TCanvas("c_np","", 200, 10, 600, 600);
	c_np->cd();
	gPad->SetLogy(0);

	TH1D *dummynp = new TH1D("","",50,0.,400.);
	dummynp->SetMinimum(0.0);
	dummynp->SetMaximum(2.0);
	dummynp->GetXaxis()->CenterTitle();
	dummynp->GetYaxis()->CenterTitle();
	dummynp->GetXaxis()->SetLimits(0.0, 32.0);
	dummynp->SetXTitle("p_{T} [GeV/c]");
	dummynp->SetYTitle("R_{FB}");
	dummynp->Draw();

	// 2) lhcb
	gRFB_lhcb_np_sys = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_exsys, lhcb_exsys, lhcb_eysys_np, lhcb_eysys_np);	
	gRFB_lhcb_np_sys->SetFillColor(kGray);
	
	gRFB_lhcb_np = new TGraphAsymmErrors(nBin_lhcb, lhcb_px, lhcb_py_np, lhcb_ex, lhcb_ex, lhcb_ey_np, lhcb_ey_np);	
	SetGraphStyleFinal(gRFB_lhcb_np,2,10);
	gRFB_lhcb_np->SetMarkerSize(1.5);
	gRFB_lhcb_np->SetLineColor(1);
	gRFB_lhcb_np->SetMarkerColor(1);

	// 3) ATLAS
	gRFB_atlas_np = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_np, atlas_ex, atlas_ex, atlas_ey_stat_np, atlas_ey_stat_np);	
 	gRFB_atlas_np->SetMarkerSize(2.0);
	gRFB_atlas_np->SetMarkerStyle(27);
	gRFB_atlas_np->SetMarkerColor(1);
	gRFB_atlas_np->SetLineColor(1);
	//gRFB_atlas_np->SetMarkerColor(2);
	//gRFB_atlas_np->SetLineColor(2);

 	gRFB_atlas_sys_np = new TGraphAsymmErrors(nBin_atlas, atlas_px, atlas_RFB_np, atlas_exsys, atlas_exsys, atlas_ey_sys_np, atlas_ey_sys_np);	
	gRFB_atlas_sys_np->SetFillColor(kGray);
	gRFB_atlas_sys_np->SetMarkerSize(0);
	gRFB_atlas_sys_np->SetLineColor(1);
	gRFB_atlas_sys_np->SetLineWidth(1);
	
  // 1) CMS
	//gRFB_np_sys_0->SetLineColor(kGreen+3);
	//gRFB_np_sys_0->SetFillColor(kGreen-10);
	//gRFB_np_0->SetMarkerSize(1.5);
	//gRFB_np_0->SetMarkerColor(kGreen+3);
	//gRFB_np_0->SetMarkerStyle(kFullSquare);
	//gRFB_np_0->SetLineColor(kGreen+3);

  /////////////////////////////////	
	gRFB_lhcb_np_sys->Draw("2");
	gRFB_atlas_sys_np->Draw("2");
  gRFB_np_sys_0->Draw("5");
	gRFB_lhcb_np->Draw("P");
	gRFB_atlas_np->Draw("P");
	gRFB_np_0->Draw("P");
	
  dashedLine(0.0,1.0,32.0,1.0,1,1);
	
	legUL->Draw("SAME");
	legBR2 -> AddEntry(gRFB_atlas_np,"ATLAS: |y_{CM}|<1.94","lp");
	legBR2 -> AddEntry(gRFB_lhcb_np,"LHCb: 2.5<|y_{CM}|<4 ","lp");
	legBR2 -> Draw();

	globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	globtex->DrawLatex(0.89, 0.85, "Non-prompt J/#psi");
	
  CMS_lumi( c_np, isPA, iPos );
	c_np->Update();
	c_np->SaveAs("plot_otherExp/comp_RFB_pt_LHCb_ALICE_ATLAS_isPrompt0.pdf");
	c_np->SaveAs("plot_otherExp/comp_RFB_pt_LHCb_ALICE_ATLAS_isPrompt0.png");
	
	return 0;
}

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
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

void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr) {
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

void formPtArr(Double_t binmin, Double_t binmax, TString* arr) {
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
