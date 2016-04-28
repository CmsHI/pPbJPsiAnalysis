#include "SONGKYO.h"
#include "TF1.h"

double tfAcceptable(double *x, double *par);
//double tfAcceptable(double *x);

int draw_pT_eta_figure() {

	gROOT->Macro("./Style.C");
  gStyle->SetTitleXOffset(1.10);
  gStyle->SetTitleYOffset(1.15);

	TCanvas* c1 = new TCanvas("c1","",600,600);
	c1->SetGridx();
	c1->SetGridy();

	//Dummy hist
	TH1D* hDummy = new TH1D("hDummy","hDummy",48,-2.4,2.4);
	hDummy->GetXaxis()->SetRangeUser(-3.,3.);
//	hDummy->GetYaxis()->SetRangeUser(0.,3.5);
//	hDummy->GetYaxis()->SetRangeUser(0.,5.0);
	hDummy->GetYaxis()->SetRangeUser(0.,4.0);
	hDummy->GetXaxis()->SetTitle("#eta_{lab}^{#mu}");
	hDummy->GetXaxis()->CenterTitle();
	hDummy->GetYaxis()->SetTitle("p_{T}^{#mu} (GeV/c)");
	//hDummy->GetYaxis()->SetNdivisions(520);
	hDummy->GetYaxis()->SetNdivisions(510);
	hDummy->Draw();

	// Draw sgl mu acc cut lines

/*	
	// 0) original
	TLine* tOrig1 = new TLine(-2.2 ,0.64, -2.2, 0.8);
	tOrig1->SetLineWidth(4);
	tOrig1->SetLineColor(kGreen+1);
	TLine* tOrig2 = new TLine(2.2 ,0.64, 2.2, 0.8);
	tOrig2->SetLineWidth(4);
	tOrig2->SetLineColor(kGreen+1);
	TLine* tOrig3 = new TLine(-1.3 ,1.475, -1.3, 3.3);
	tOrig3->SetLineWidth(4);
	tOrig3->SetLineColor(kGreen+1);
	TLine* tOrig4 = new TLine(1.3 ,1.475, 1.3, 3.3);
	tOrig4->SetLineWidth(4);
	tOrig4->SetLineColor(kGreen+1);
	//solidLine(-1.3,1.475,-1.3,3.3,2,3.);
	//solidLine(1.3,1.475,1.3,3.3,2,3.);
	TF1* fOrig_v1 = new TF1("fOrig_v1","0.8",-2.4,-2.2);
	TF1* fOrig_v2 = new TF1("fOrig_v2","0.8",2.2,2.4);
	TF1* fOrig_v3 = new TF1("fOrig_v3","3.3",-1.3,1.3);
	TF1* fOrig_v4 = new TF1("fOrig_v4","2.9/cosh(x)",-2.2,-1.3);
	TF1* fOrig_v5 = new TF1("fOrig_v5","2.9/cosh(x)",1.3,2.2);
	fOrig_v1->SetLineColor(kGreen+1);
	fOrig_v2->SetLineColor(kGreen+1);
	fOrig_v3->SetLineColor(kGreen+1);
	fOrig_v4->SetLineColor(kGreen+1);
	fOrig_v5->SetLineColor(kGreen+1);
	// 1) loose
	TLine* tCutA1 = new TLine(-1.2 ,2.6, -1.2, 3.3);
	tCutA1->SetLineWidth(4);
	tCutA1->SetLineColor(kViolet);
	tCutA1->SetLineStyle(5);
	TLine* tCutA2 = new TLine(1.2 ,2.6, 1.2, 3.3);
	tCutA2->SetLineWidth(4);
	tCutA2->SetLineColor(kViolet);
	tCutA2->SetLineStyle(5);
	TF1* fCutA_v1 = new TF1("fCutA_v1","0.8",-2.4,-2.1);
	TF1* fCutA_v2 = new TF1("fCutA_v2","0.8",2.1,2.4);
	TF1* fCutA_v3 = new TF1("fCutA_v3","3.3",-1.2,1.2);
	TF1* fCutA_v4 = new TF1("fCutA_v4","-(1.4/0.4)*(-x)+1.2*(1.4/0.4)+2.6",-1.6,-1.2);
	TF1* fCutA_v5 = new TF1("fCutA_v5","-(1.4/0.4)*x+1.2*(1.4/0.4)+2.6",1.2,1.6);
	TF1* fCutA_v6 = new TF1("fCutA_v6","-(0.4/0.5)*x+1.6*(0.4/0.5)+1.2",1.6,2.1);
	TF1* fCutA_v7 = new TF1("fCutA_v7","-(0.4/0.5)*(-x)+1.6*(0.4/0.5)+1.2",-2.1,-1.6);
	fCutA_v1->SetLineColor(kViolet);
	fCutA_v2->SetLineColor(kViolet);
	fCutA_v3->SetLineColor(kViolet);
	fCutA_v4->SetLineColor(kViolet);
	fCutA_v5->SetLineColor(kViolet);
	fCutA_v6->SetLineColor(kViolet);
	fCutA_v7->SetLineColor(kViolet);
	fCutA_v1->SetLineStyle(5);
	fCutA_v2->SetLineStyle(5);
	fCutA_v3->SetLineStyle(5);
	fCutA_v4->SetLineStyle(5);
	fCutA_v5->SetLineStyle(5);
	fCutA_v6->SetLineStyle(5);
	fCutA_v7->SetLineStyle(5);
	fCutA_v1->SetLineWidth(4);
	fCutA_v2->SetLineWidth(4);
	fCutA_v3->SetLineWidth(4);
	fCutA_v4->SetLineWidth(4);
	fCutA_v5->SetLineWidth(4);
	fCutA_v6->SetLineWidth(4);
	fCutA_v7->SetLineWidth(4);
	//2) tight
	TLine* tCutC1 = new TLine(-1.2 ,2.6, -1.2, 3.5);
	tCutC1->SetLineWidth(4);
	tCutC1->SetLineColor(kBlack);
	tCutC1->SetLineStyle(9);
	TLine* tCutC2 = new TLine(1.2 ,2.6, 1.2, 3.5);
	tCutC2->SetLineWidth(4);
	tCutC2->SetLineColor(kBlack);
	tCutC2->SetLineStyle(9);
	TF1* fCutC_v1 = new TF1("fCutC_v1","1.5",-2.4,-2.1);
	TF1* fCutC_v2 = new TF1("fCutC_v2","1.5",2.1,2.4);
	TF1* fCutC_v3 = new TF1("fCutC_v3","3.5",-1.2,1.2);
	TF1* fCutC_v4 = new TF1("fCutC_v4","2.6",-1.6,-1.2);
	TF1* fCutC_v5 = new TF1("fCutC_v5","2.6",1.2,1.6);
	TF1* fCutC_v6 = new TF1("fCutC_v6","-(1.1/0.5)*x+1.6*(1.1/0.5)+2.6",1.6,2.1);
	TF1* fCutC_v7 = new TF1("fCutC_v7","-(1.1/0.5)*(-x)+1.6*(1.1/0.5)+2.6",-2.1,-1.6);
	fCutC_v1->SetLineColor(kBlack);
	fCutC_v2->SetLineColor(kBlack);
	fCutC_v3->SetLineColor(kBlack);
	fCutC_v4->SetLineColor(kBlack);
	fCutC_v5->SetLineColor(kBlack);
	fCutC_v6->SetLineColor(kBlack);
	fCutC_v7->SetLineColor(kBlack);
	fCutC_v1->SetLineStyle(9);
	fCutC_v2->SetLineStyle(9);
	fCutC_v3->SetLineStyle(9);
	fCutC_v4->SetLineStyle(9);
	fCutC_v5->SetLineStyle(9);
	fCutC_v6->SetLineStyle(9);
	fCutC_v7->SetLineStyle(9);
	fCutC_v1->SetLineWidth(4);
	fCutC_v2->SetLineWidth(4);
	fCutC_v3->SetLineWidth(4);
	fCutC_v4->SetLineWidth(4);
	fCutC_v5->SetLineWidth(4);
	fCutC_v6->SetLineWidth(4);
	fCutC_v7->SetLineWidth(4);
	
	// 3) middle
	TLine* tCutB1 = new TLine(-1.2 ,2.0, -1.2, 3.3);
	tCutB1->SetLineWidth(4);
	tCutB1->SetLineStyle(7);
	tCutB1->SetLineColor(kRed);
	TLine* tCutB2 = new TLine(1.2 ,2.0, 1.2, 3.3);
	tCutB2->SetLineWidth(4);
	tCutB2->SetLineStyle(7);
	tCutB2->SetLineColor(kRed);
	TF1* fCutB_v1 = new TF1("fCutB_v1","1.1",-2.4,-2.1);
	TF1* fCutB_v2 = new TF1("fCutB_v2","1.1",2.1,2.4);
	TF1* fCutB_v3 = new TF1("fCutB_v3","3.3",-1.2,1.2);
	TF1* fCutB_v4 = new TF1("fCutC_v4","2.0",-1.6,-1.2);
	TF1* fCutB_v5 = new TF1("fCutC_v5","2.0",1.2,1.6);
	TF1* fCutB_v6 = new TF1("fCutB_v6","-(0.9/0.5)*x+1.6*(0.9/0.5)+2.0",1.6,2.1);
	TF1* fCutB_v7 = new TF1("fCutB_v7","-(0.9/0.5)*(-x)+1.6*(0.9/0.5)+2.0",-2.1,-1.6);
	fCutB_v1->SetLineColor(kRed);
	fCutB_v2->SetLineColor(kRed);
	fCutB_v3->SetLineColor(kRed);
	fCutB_v4->SetLineColor(kRed);
	fCutB_v5->SetLineColor(kRed);
	fCutB_v6->SetLineColor(kRed);
	fCutB_v7->SetLineColor(kRed);
	fCutB_v1->SetLineStyle(7);
	fCutB_v2->SetLineStyle(7);
	fCutB_v3->SetLineStyle(7);
	fCutB_v4->SetLineStyle(7);
	fCutB_v5->SetLineStyle(7);
	fCutB_v6->SetLineStyle(7);
	fCutB_v7->SetLineStyle(7);
	fCutB_v1->SetLineWidth(4);
	fCutB_v2->SetLineWidth(4);
	fCutB_v3->SetLineWidth(4);
	fCutB_v4->SetLineWidth(4);
	fCutB_v5->SetLineWidth(4);
	fCutB_v6->SetLineWidth(4);
	fCutB_v7->SetLineWidth(4);
	
	// 4) middle modified
	TLine* tCutD1 = new TLine(-1.2 ,2.0, -1.2, 3.3);
	tCutD1->SetLineWidth(4);
	tCutD1->SetLineStyle(7);
	tCutD1->SetLineColor(kRed);
	TLine* tCutD2 = new TLine(1.2 ,2.0, 1.2, 3.3);
	tCutD2->SetLineWidth(4);
	tCutD2->SetLineStyle(7);
	tCutD2->SetLineColor(kRed);
	TLine* tCutD3 = new TLine(-2.1 ,1.3, -2.1, 1.6);
	tCutD3->SetLineWidth(4);
	tCutD3->SetLineStyle(6);
	tCutD3->SetLineColor(kRed);
	TLine* tCutD4 = new TLine(2.1 ,1.3, 2.1, 1.6);
	tCutD4->SetLineWidth(4);
	tCutD4->SetLineStyle(7);
	tCutD4->SetLineColor(kRed);
	TF1* fCutD_v1 = new TF1("fCutD_v1","1.3",-2.4,-2.1);
	TF1* fCutD_v2 = new TF1("fCutD_v2","1.3",2.1,2.4);
	TF1* fCutD_v3 = new TF1("fCutD_v3","3.3",-1.2,1.2);
	TF1* fCutD_v4 = new TF1("fCutC_v4","2.0",-1.6,-1.2);
	TF1* fCutD_v5 = new TF1("fCutC_v5","2.0",1.2,1.6);
	TF1* fCutD_v6 = new TF1("fCutD_v6","-(0.4/0.5)*x+1.6*(0.4/0.5)+2.0",1.6,2.1);
	TF1* fCutD_v7 = new TF1("fCutD_v7","-(0.4/0.5)*(-x)+1.6*(0.4/0.5)+2.0",-2.1,-1.6);
	fCutD_v1->SetLineColor(kRed);
	fCutD_v2->SetLineColor(kRed);
	fCutD_v3->SetLineColor(kRed);
	fCutD_v4->SetLineColor(kRed);
	fCutD_v5->SetLineColor(kRed);
	fCutD_v6->SetLineColor(kRed);
	fCutD_v7->SetLineColor(kRed);
	fCutD_v1->SetLineStyle(7);
	fCutD_v2->SetLineStyle(7);
	fCutD_v3->SetLineStyle(7);
	fCutD_v4->SetLineStyle(7);
	fCutD_v5->SetLineStyle(7);
	fCutD_v6->SetLineStyle(7);
	fCutD_v7->SetLineStyle(7);
	fCutD_v1->SetLineWidth(4);
	fCutD_v2->SetLineWidth(4);
	fCutD_v3->SetLineWidth(4);
	fCutD_v4->SetLineWidth(4);
	fCutD_v5->SetLineWidth(4);
	fCutD_v6->SetLineWidth(4);
	fCutD_v7->SetLineWidth(4);
	
	// 4) middle modified + cutA
	TLine* tCutE1 = new TLine(-1.2 ,2.6, -1.2, 3.3);
	tCutE1->SetLineWidth(4);
	tCutE1->SetLineStyle(7);
	tCutE1->SetLineColor(kRed);
	TLine* tCutE2 = new TLine(1.2 ,2.6, 1.2, 3.3);
	tCutE2->SetLineWidth(4);
	tCutE2->SetLineStyle(7);
	tCutE2->SetLineColor(kRed);
	TLine* tCutE3 = new TLine(-2.1 ,1.3, -2.1, 1.6);
	tCutE3->SetLineWidth(4);
	tCutE3->SetLineStyle(6);
	tCutE3->SetLineColor(kRed);
	TLine* tCutE4 = new TLine(2.1 ,1.3, 2.1, 1.6);
	tCutE4->SetLineWidth(4);
	tCutE4->SetLineStyle(7);
	tCutE4->SetLineColor(kRed);
	TF1* fCutE_v1 = new TF1("fCutE_v1","1.3",-2.4,-2.1);
	TF1* fCutE_v2 = new TF1("fCutE_v2","1.3",2.1,2.4);
	TF1* fCutE_v3 = new TF1("fCutE_v3","3.3",-1.2,1.2);
	TF1* fCutE_v8 = new TF1("fCutE_v8","-(1.4/0.4)*(x)+1.2*(1.4/0.4)+2.6",1.2,1.38);
	TF1* fCutE_v9 = new TF1("fCutE_v9","-(1.4/0.4)*(-x)+1.2*(1.4/0.4)+2.6",-1.38,-1.2);
	TF1* fCutE_v4 = new TF1("fCutC_v4","2.0",-1.6,-1.38);
	TF1* fCutE_v5 = new TF1("fCutC_v5","2.0",1.38,1.6);
	TF1* fCutE_v6 = new TF1("fCutE_v6","-(0.4/0.5)*x+1.6*(0.4/0.5)+2.0",1.6,2.1);
	TF1* fCutE_v7 = new TF1("fCutE_v7","-(0.4/0.5)*(-x)+1.6*(0.4/0.5)+2.0",-2.1,-1.6);
	fCutE_v1->SetLineColor(kRed);
	fCutE_v2->SetLineColor(kRed);
	fCutE_v3->SetLineColor(kRed);
	fCutE_v4->SetLineColor(kRed);
	fCutE_v5->SetLineColor(kRed);
	fCutE_v6->SetLineColor(kRed);
	fCutE_v7->SetLineColor(kRed);
	fCutE_v8->SetLineColor(kRed);
	fCutE_v9->SetLineColor(kRed);
	fCutE_v1->SetLineStyle(7);
	fCutE_v2->SetLineStyle(7);
	fCutE_v3->SetLineStyle(7);
	fCutE_v4->SetLineStyle(7);
	fCutE_v5->SetLineStyle(7);
	fCutE_v6->SetLineStyle(7);
	fCutE_v7->SetLineStyle(7);
	fCutE_v8->SetLineStyle(7);
	fCutE_v9->SetLineStyle(7);
	fCutE_v1->SetLineWidth(4);
	fCutE_v2->SetLineWidth(4);
	fCutE_v3->SetLineWidth(4);
	fCutE_v4->SetLineWidth(4);
	fCutE_v5->SetLineWidth(4);
	fCutE_v6->SetLineWidth(4);
	fCutE_v7->SetLineWidth(4);
	fCutE_v8->SetLineWidth(4);
	fCutE_v9->SetLineWidth(4);
	
	// 4) cutF
	TLine* tCutF1 = new TLine(-1.2 ,2.6, -1.2, 3.3);
	tCutF1->SetLineWidth(4);
	tCutF1->SetLineStyle(7);
	tCutF1->SetLineColor(kRed);
	TLine* tCutF2 = new TLine(1.2 ,2.6, 1.2, 3.3);
	tCutF2->SetLineWidth(4);
	tCutF2->SetLineStyle(7);
	tCutF2->SetLineColor(kRed);
	TLine* tCutF3 = new TLine(-2.1 ,1.3, -2.1, 1.6);
	tCutF3->SetLineWidth(4);
	tCutF3->SetLineStyle(6);
	tCutF3->SetLineColor(kRed);
	TLine* tCutF4 = new TLine(2.1 ,1.3, 2.1, 1.6);
	tCutF4->SetLineWidth(4);
	tCutF4->SetLineStyle(7);
	tCutF4->SetLineColor(kRed);
	TF1* fCutF_v1 = new TF1("fCutF_v1","1.3",-2.4,-2.1);
	TF1* fCutF_v2 = new TF1("fCutF_v2","1.3",2.1,2.4);
	TF1* fCutF_v3 = new TF1("fCutF_v3","3.3",-1.2,1.2);
	TF1* fCutF_v4 = new TF1("fCutF_v4","-(0.6/0.4)*(x)+1.2*(0.6/0.4)+2.6",1.2,1.6);
	TF1* fCutF_v5 = new TF1("fCutF_v5","-(0.6/0.4)*(-x)+1.2*(0.6/0.4)+2.6",-1.6,-1.2);
	TF1* fCutF_v6 = new TF1("fCutF_v6","-(0.4/0.5)*x+1.6*(0.4/0.5)+2.0",1.6,2.1);
	TF1* fCutF_v7 = new TF1("fCutF_v7","-(0.4/0.5)*(-x)+1.6*(0.4/0.5)+2.0",-2.1,-1.6);
	fCutF_v1->SetLineColor(kRed);
	fCutF_v2->SetLineColor(kRed);
	fCutF_v3->SetLineColor(kRed);
	fCutF_v4->SetLineColor(kRed);
	fCutF_v5->SetLineColor(kRed);
	fCutF_v6->SetLineColor(kRed);
	fCutF_v7->SetLineColor(kRed);
	fCutF_v1->SetLineStyle(7);
	fCutF_v2->SetLineStyle(7);
	fCutF_v3->SetLineStyle(7);
	fCutF_v4->SetLineStyle(7);
	fCutF_v5->SetLineStyle(7);
	fCutF_v6->SetLineStyle(7);
	fCutF_v7->SetLineStyle(7);
	fCutF_v1->SetLineWidth(4);
	fCutF_v2->SetLineWidth(4);
	fCutF_v3->SetLineWidth(4);
	fCutF_v4->SetLineWidth(4);
	fCutF_v5->SetLineWidth(4);
	fCutF_v6->SetLineWidth(4);
	fCutF_v7->SetLineWidth(4);
*/

	// 4) CutG // final!!!
	TLine* tCutG1 = new TLine(-1.2 ,2.6, -1.2, 3.3);
	tCutG1->SetLineWidth(4);
	tCutG1->SetLineColor(kRed);
	TLine* tCutG2 = new TLine(1.2 ,2.6, 1.2, 3.3);
	tCutG2->SetLineWidth(4);
	tCutG2->SetLineColor(kRed);
	TLine* tCutG3 = new TLine(-2.1 ,1.3, -2.1, 1.6);
	tCutG3->SetLineWidth(4);
	tCutG3->SetLineColor(kRed);
	TLine* tCutG4 = new TLine(2.1 ,1.3, 2.1, 1.6);
	tCutG4->SetLineWidth(4);
	tCutG4->SetLineColor(kRed);
	TF1* fCutG_v1 = new TF1("fCutG_v1","1.3",-2.4,-2.1);
	TF1* fCutG_v2 = new TF1("fCutG_v2","1.3",2.1,2.4);
	TF1* fCutG_v3 = new TF1("fCutG_v3","3.3",-1.2,1.2);
	//TF1* fCutG_v4 = new TF1("fCutG_v4","-(1.0/0.9)*(x)+1.2*(1.0/0.9)+2.6",1.2,2.1);
	//TF1* fCutG_v5 = new TF1("fCutG_v5","-(1.0/0.9)*(-x)+1.2*(1.0/0.9)+2.6",-2.1,-1.2);
	TF1* fCutG_v4 = new TF1("fCutG_v4","3.93-1.11*(x)",1.2,2.1);
	TF1* fCutG_v5 = new TF1("fCutG_v5","3.93-1.11*(-x)",-2.1,-1.2);
	fCutG_v1->SetLineColor(kRed);
	fCutG_v2->SetLineColor(kRed);
	fCutG_v3->SetLineColor(kRed);
	fCutG_v4->SetLineColor(kRed);
	fCutG_v5->SetLineColor(kRed);
	fCutG_v1->SetLineWidth(4);
	fCutG_v2->SetLineWidth(4);
	fCutG_v3->SetLineWidth(4);
	fCutG_v4->SetLineWidth(4);
	fCutG_v5->SetLineWidth(4);
	
	/*
	// 5) Cut 2015 Glb // final!!!
	TF1* fCutGlb_v1 = new TF1("fCutG_v3","3.4",-1.0,1.0);
	TF1* fCutGlb_v2 = new TF1("fCutG_v4","5.8-2.4*(x)",1.0,1.5);
	TF1* fCutGlb_v3 = new TF1("fCutG_v5","5.8-2.4*(-x)",-1.5,-1.0);
	TF1* fCutGlb_v4 = new TF1("fCutG_v2","3.3667-7.0/9.0*(x)",1.5,2.4);
	TF1* fCutGlb_v5 = new TF1("fCutG_v1","3.3667-7.0/9.0*(-x)",-2.4,-1.5);
	fCutGlb_v1->SetLineStyle(7);
	fCutGlb_v2->SetLineStyle(7);
	fCutGlb_v3->SetLineStyle(7);
	fCutGlb_v4->SetLineStyle(7);
	fCutGlb_v5->SetLineStyle(7);
	*/

	////// pre-filter for cross-check
	TF1* fAfter = new TF1("fAfter","2.5/cosh(x)",-2.4, 2.4);
	fAfter->SetLineColor(kBlue);
	fAfter->SetLineWidth(4);
	
	TF1* fAfter32 = new TF1("fAfter32","3.2/cosh(x)",-2.4, 2.4);
	fAfter32->SetLineColor(kViolet);
	fAfter32->SetLineWidth(4);
	
	TF1* fAfter12 = new TF1("fAfter12","1.2",-2.4, 2.4);
	fAfter12->SetLineColor(kCyan);
	fAfter12->SetLineWidth(4);

	///////////////////////// drawing //////////////////////////
	/*
	fOrig_v1->Draw("same");
	fOrig_v2->Draw("same");
	fOrig_v3->Draw("same");
	fOrig_v4->Draw("same");
	fOrig_v5->Draw("same");
	tOrig1->Draw("same");
	tOrig2->Draw("same");
	tOrig3->Draw("same");
	tOrig4->Draw("same");
	fCutA_v1->Draw("same");
	fCutA_v2->Draw("same");
	fCutA_v3->Draw("same");
	fCutA_v4->Draw("same");
	fCutA_v5->Draw("same");
	fCutA_v6->Draw("same");
	fCutA_v7->Draw("same");
	tCutA1->Draw("same");
	tCutA2->Draw("same");
	fCutC_v1->Draw("same");
	fCutC_v2->Draw("same");
	fCutC_v3->Draw("same");
	fCutC_v4->Draw("same");
	fCutC_v5->Draw("same");
	fCutC_v6->Draw("same");
	fCutC_v7->Draw("same");
	tCutC1->Draw("same");
	tCutC2->Draw("same");
	fCutB_v1->Draw("same");
	fCutB_v2->Draw("same");
	fCutB_v3->Draw("same");
	fCutB_v4->Draw("same");
	fCutB_v5->Draw("same");
	fCutB_v6->Draw("same");
	fCutB_v7->Draw("same");
	tCutB1->Draw("same");
	tCutB2->Draw("same");
	fCutD_v1->Draw("same");
	fCutD_v2->Draw("same");
	fCutD_v3->Draw("same");
	fCutD_v4->Draw("same");
	fCutD_v5->Draw("same");
	fCutD_v6->Draw("same");
	fCutD_v7->Draw("same");
	tCutD1->Draw("same");
	tCutD2->Draw("same");
	tCutD3->Draw("same");
	tCutD4->Draw("same");
	fCutE_v1->Draw("same");
	fCutE_v2->Draw("same");
	fCutE_v3->Draw("same");
	fCutE_v4->Draw("same");
	fCutE_v5->Draw("same");
	fCutE_v6->Draw("same");
	fCutE_v7->Draw("same");
	fCutE_v8->Draw("same");
	fCutE_v9->Draw("same");
	tCutE1->Draw("same");
	tCutE2->Draw("same");
	tCutE3->Draw("same");
	tCutE4->Draw("same");
	fCutF_v1->Draw("same");
	fCutF_v2->Draw("same");
	fCutF_v3->Draw("same");
	fCutF_v4->Draw("same");
	fCutF_v5->Draw("same");
	fCutF_v6->Draw("same");
	fCutF_v7->Draw("same");
	tCutF1->Draw("same");
	tCutF2->Draw("same");
	tCutF3->Draw("same");
	tCutF4->Draw("same");
*/	

	fCutG_v1->Draw("same");
	fCutG_v2->Draw("same");
	fCutG_v3->Draw("same");
	fCutG_v4->Draw("same");
	fCutG_v5->Draw("same");
	tCutG1->Draw("same");
	tCutG2->Draw("same");
	tCutG3->Draw("same");
	tCutG4->Draw("same");

/*
	fCutGlb_v1->Draw("same");
	fCutGlb_v2->Draw("same");
	fCutGlb_v3->Draw("same");
	fCutGlb_v4->Draw("same");
	fCutGlb_v5->Draw("same");
	
*/
	fAfter->Draw("same");

//	TLegend *legB = new TLegend(0.38,0.19,0.64,0.30,NULL,"brNDC");
	TLegend *legB = new TLegend(0.33,0.19,0.64,0.30,NULL,"brNDC");
	SetLegendStyle(legB);
	legB->SetTextSize(0.030);
	//legB->AddEntry(fOrig_v1,"(old cut)","l");
	//legB->AddEntry(fCutA_v1,"(Data OniaTree cut)","l");
	//legB->AddEntry(fCutA_v1,"1) cut A : without trig","l");
	//legB->AddEntry(fCutB_v1,"2) cut B : with trig. and correl.","l");
	//legB->AddEntry(fCutB_v1,"new cut","l");
	//legB->AddEntry(fCutC_v1,"3) cut C: with trig.","l");
	//legB->AddEntry(fCutD_v1,"new cut","l");
	legB->AddEntry(fCutG_v1,"muon acceptance cut","l");
	//legB->AddEntry(fCutGlb_v1,"GlbTrk acceptance cut","l");
	legB->AddEntry(fAfter,"pre-filter at GEN level","l");
	//legB->AddEntry(fAfter,"pre-filter p > 2.5 GeV","l");
	//legB->AddEntry(fAfter32,"pre-filter p > 3.2 GeV","l");
	//legB->AddEntry(fAfter12,"pre-filter p_{T} > 1.2 GeV","l");
	legB->Draw();

	c1->SaveAs("pT_eta_figure_two_prefilter_cutG.pdf");
	c1->SaveAs("pT_eta_figure_two_prefilter_cutG.png");
	//c1->SaveAs("pT_eta_figure_three_prefilter_old_cutG.pdf");
	//c1->SaveAs("pT_eta_figure_three_prefilter_cutA_cutG.pdf");
	//c1->SaveAs("pT_eta_figure_three_prefilter_prefilter32_cutG.pdf");
	//c1->SaveAs("pT_eta_figure_three_prefilter_prefilter32_cutGlb.pdf");
	//c1->SaveAs("pT_eta_figure_four_prefilter_old_cutA_cutG.pdf");
	//c1->SaveAs("pT_eta_figure_four_prefilter_prefilter32_cutG_cutGlb.pdf");
	//c1->SaveAs("pT_eta_figure_four_prefilter_prefilter32_prefilter12_cutG.pdf");
	//c1->SaveAs("pT_eta_figure_five_prefilter_prefilter32_prefilter12_cutG_gutGlb.pdf");
	
	//TF1* fTmp = new TF1("fTmp","cosh(x)",-2.4, 2.4);
	
	//TF1* fTmp = new TF1("fTmp","cosh(x)",-2.4, 2.4);
	//fTmp->Draw("same");
	
	return 0;
}

double tfAcceptable(double *x, double *par){
//double tfAcceptable(double *x){
	return par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) );
//	return 2.9/cosh(x[0]);
}

