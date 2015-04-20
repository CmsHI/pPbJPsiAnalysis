#include <Riostream.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <vector>
#include <TSystem.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include "TStyle.h"
#include "TSystem.h"

#include <TMath.h>
#include <math.h>

#include <sstream>
#include <string>

#include "KYOcommonOpt.h"

// no ordering in rap (just y_lab)
void draw_compRatio_finePt()
{
	gROOT->Macro("./JpsiStyle.C");

	// --- read-in file
	TFile * f01;
	TFile * f02;
	//char* f01name="with SF";
	//char* f02name="without SF";
	char* f01name="T&P corr'ed eff.";
	char* f02name="Raw efficiency";
	//char* f01name="reco pt filled";
	//char* f02name="gen pt filled";
	
	//char* sampleName;
	//double ratiomin=0.8; 
	//double ratiomax=1.2; 
	double ratiomin=0.7; 
	double ratiomax=1.3; 

	char* strPrompt;
	char* str1st;


	f01 = new TFile("h1Eff_EffCounting_8rap9pt_PRMCpythia_Pbp_useCtErr_0_useDataDriven_1_useZvtxStep1_0_Step2_1_finePtBin.root");
	f02 = new TFile("h1Eff_EffCounting_8rap9pt_PRMCpythia_Pbp_useCtErr_0_useDataDriven_0_useZvtxStep1_0_Step2_1_finePtBin.root");


	// --- read-in tree

	const int nRap = 8 ; 
	cout << "nRap = " << nRap << endl;
	TH1D* h1D_EffPt01[nRap]; 
	TH1D* h1D_EffPt02[nRap]; 
	TH1D *hRatio[nRap]; //f01/f02
	TH1D *hRel[nRap]; // (f01-f02)/f02
	for (int iy=0; iy < nRap; iy ++){
	  h1D_EffPt01[iy] = (TH1D*)f01->Get(Form("h1D_EffPt_%d",iy));
	  h1D_EffPt02[iy] = (TH1D*)f02->Get(Form("h1D_EffPt_%d",iy));
	  cout << "h1D_EffPt01["<<iy<<"] = " << h1D_EffPt01[iy] << endl;
	  cout << "h1D_EffPt02["<<iy<<"] = " << h1D_EffPt02[iy] << endl;
	  hRatio[iy]=(TH1D*)h1D_EffPt01[iy]->Clone(Form("hRatio_%d",iy));
	  hRatio[iy]->Divide(h1D_EffPt02[iy]);
	  hRel[iy]=(TH1D*)h1D_EffPt01[iy]->Clone(Form("hRel_%d",iy));
	  hRel[iy]->Add(h1D_EffPt02[iy],-1);
	  hRel[iy]->Divide(h1D_EffPt02[iy]);
	}
	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms

//	TCanvas* c1 = new TCanvas("c1","c1",600,600);
//	TCanvas* c2 = new TCanvas("c2","c2",600,600);
//	TCanvas* c3 = new TCanvas("c3","c3",600,600);
	TCanvas* c1 = new TCanvas("c1","c1",1600,800);
	c1->Divide(4,2);
	TCanvas* c2 = new TCanvas("c2","c2",1600,800);
	c2->Divide(4,2);
//	TCanvas* c3 = new TCanvas("c3","c3",1600,800);
//	c3->Divide(4,2);
	//TLegend *legBR = new TLegend(0.58,0.30,0.85,0.40,NULL,"brNDC");
	TLegend *legBR = new TLegend(0.50,0.30,0.80,0.41,NULL,"brNDC");
	SetLegendStyle(legBR);

	for (Int_t iy = 0; iy < nRap; iy++) {
		//// 1)  distributions
		//c1->cd();
		c1->cd(iy+1);
		SetHistStyle(h1D_EffPt01[iy],3,0);
		SetHistStyle(h1D_EffPt02[iy],4,0);
		h1D_EffPt01[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1D_EffPt01[iy]->GetYaxis()->SetTitle("Efficiency");
		h1D_EffPt01[iy]->SetMinimum(0.);
		h1D_EffPt01[iy]->SetMaximum(1.);
		h1D_EffPt01[iy]->Draw("pe");
		h1D_EffPt02[iy]->Draw("pe same");
		if (iy==0) {
			legBR->AddEntry(h1D_EffPt01[iy],f01name,"lp");
			legBR->AddEntry(h1D_EffPt02[iy],f02name,"lp");
			legBR->Draw();
		}


		//// 2) ratio f01/f02
		//c2->cd();
		c2->cd(iy+1);
		SetHistStyle(hRatio[iy],5,0);
		hRatio[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		hRatio[iy]->GetYaxis()->SetTitle(Form("[ %s ]/[ %s ]",f01name,f02name));
		hRatio[iy]->SetMinimum(ratiomin);
		hRatio[iy]->SetMaximum(ratiomax);
		for ( int ib= 1 ; ib<=hRatio[iy]->GetNbinsX();ib++) {
		  hRatio[iy]->SetBinError(ib,0);
		}
		hRatio[iy]->Draw("p");
		dashedLine(0.,1.,30.,1.,1,.8);
	}
	return;
	
}

