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

#include "KYOcommonOptMulti.h"


void draw_1D_vsZvtx_overlay(bool is1st = true, bool isEmbedded = false, bool useCtErrRangeEff =false, bool useDataDrivenEff=false,  bool useZvtxWeightStep1 = false, bool useZvtxWeightStep2=false,char* dirName = "draw_1D_vsZvtx")
{
	gROOT->Macro("./JpsiStyle.C");

	// --- read-in file
	TFile * f01;
	TFile * f02;
	char* f01name="prompt";
	char* f02name="non-prompt";

	char* str1st;
	if (is1st) str1st = "Pbp";
	else str1st = "pPb";

	f01 = new TFile(Form("%s/vsZvtx_PRMCpythia_%s.root",dirName,str1st));	
	f02 = new TFile(Form("%s/vsZvtx_NPMCpythia_%s.root",dirName,str1st));	
	
	const int nYBins = 8;
	TH1D* h1D_Eff01[nYBins];
	TH1D* h1D_Eff02[nYBins];
	for (int in=0; in <nYBins; in++) {
		h1D_Eff01[in] = (TH1D*)f01->Get(Form("h1D_Eff_zVtx_%d",in));
		h1D_Eff02[in] = (TH1D*)f02->Get(Form("h1D_Eff_zVtx_%d",in));
		h1D_Eff01[in]->SetName(Form("h1D_Eff01_%d",in));
		h1D_Eff02[in]->SetName(Form("h1D_Eff02_%d",in));
		cout << in << "th h1D_Eff01 = " << h1D_Eff01[in] << endl;
		cout << in << "th h1D_Eff02 = " << h1D_Eff02[in] << endl;
		h1D_Eff01[in]->GetYaxis()->SetRangeUser(0.,1.);
	}
	
	Double_t ptLimit[nYBins];
	if (is1st) ptLimit= {2.0, 3.0, 6.5, 6.5, 6.5, 5.0, 3.0, 2.0};
	else ptLimit = {2.0, 3.0, 5.0, 6.5, 6.5, 6.5, 3.0, 2.0};
	for (Int_t i=0; i<nYBins; i++){	
		cout << i <<"th ptLimit = " << ptLimit[i] <<endl;
	}
	
	double rapEdge[nYBins+1];
	if (is1st) rapEdge = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
	else rapEdge = {-2.4, -1.93, -1.46, -1.03, -0.43, 0.47, 1.37, 1.97, 2.4};
	for (Int_t i=0; i<nYBins; i++){	
		cout << i <<"th rapEdge = " << rapEdge[i] <<endl;
	}

	TLegend *legMR = new TLegend(0.53,0.50,0.87,0.67,NULL,"brNDC");
	SetLegendStyle(legMR);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms
	//TCanvas* c1 = new TCanvas("c1","c1",600,600);
	//c1->cd();
	TCanvas* c1 = new TCanvas("c1","c1",1600,800);
	c1->Divide(4,2);

	for (Int_t iy = 0; iy < nYBins; iy++) {
		c1->cd(iy+1);
		//SetHistStyle(h1D_Eff01[iy],0,0);
		h1D_Eff01[iy]->GetXaxis()->SetTitle("primary Z vertex (cm)");
		h1D_Eff01[iy]->GetYaxis()->SetTitle("Efficiency");
		h1D_Eff01[iy]->SetMinimum(0.);
		h1D_Eff01[iy]->SetMaximum(1.);
		SetHistStyle(h1D_Eff01[iy],1,0);
		SetHistStyle(h1D_Eff02[iy],2,10);
		h1D_Eff01[iy]->Draw("pe");
		h1D_Eff02[iy]->Draw("pe same");
		//if (iy==0) latex->DrawLatex(0.50,0.87,sampleName);
		//latex->DrawLatex(0.50,0.80,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		//latex->DrawLatex(0.50,0.73,Form("%.1f < p_{T} <30 GeV/c",ptLimit[iy]));
		latex->DrawLatex(0.53,0.87,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		latex->DrawLatex(0.53,0.80,Form("%.1f < p_{T} <30 (GeV/c)",ptLimit[iy]));
		if (iy==0){
			legMR->AddEntry(h1D_Eff01[iy],f01name,"lp");
			legMR->AddEntry(h1D_Eff02[iy],f02name,"lp");
			legMR->Draw();
		}
	}

	c1->SaveAs(Form("%s/vsZvtx_overlay.pdf",dirName));

	return;

}

