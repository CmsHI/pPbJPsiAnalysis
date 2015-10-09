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


void draw_OverLay(char *stringA = "6rap3pt", bool isScale=1)
{

	//gROOT->Macro("./JpsiStyle.C");
	gStyle->SetOptStat(0);
  gStyle->SetLabelFont(42,"xyz");
  gStyle->SetTitleSize(0.048,"xyz");
  
	gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadTopMargin(0.03);
  gStyle->SetPadRightMargin(0.07);
  gStyle->SetPadLeftMargin(0.12);

	TFile * fMB;
	TFile * fDimu;
	
	fMB = new TFile("ethf_MB_6rap3pt.root");
	fDimu = new TFile("ethf_data_6rap3pt.root");

	TH1D* h1D_et_min;
	TH1D* h1D_et_dimu;
	h1D_et_min = (TH1D*)fMB->Get("hEt_0");
	h1D_et_dimu = (TH1D*)fDimu->Get("hEt_0");

	//check
	cout <<"h1D_et_min =" <<h1D_et_min<<endl;
	cout <<"h1D_et_dimu =" <<h1D_et_dimu<<endl;

	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms
	TCanvas* c1 = new TCanvas("c1","c1",700,600);
	TLegend *legUR = new TLegend(0.52, 0.68, 0.85, 0.88); //ptupper left
	TLegend *legBM = new TLegend(0.20, 0.19, 0.45, 0.35); //ptupper left
	SetLegendStyle(legUR);
	SetLegendStyle(legBM);

	//////////////////////////////////////////////////////////////////
	c1->cd();
	SetHistStyle(h1D_et_min,3,0);	 // gray
	SetHistStyle(h1D_et_dimu,5,0); // orange	
			gPad->SetLogy(1);
			h1D_et_min->GetXaxis()->SetTitle("E_{T}^{HF |#eta|>4} (GeV)");
			h1D_et_min->GetYaxis()->SetTitle("Events/(GeV)");
		//	h1D_et_min->GetYaxis()->SetTitleOffset(1.8);
		//	h1D_et_min->SetMaximum(22000.);
		h1D_et_min->GetXaxis()->CenterTitle();
		//h1D_et_min->GetXaxis()->SetTitleOffset(1.3);
		
		if (isScale){
			h1D_et_min->Scale(1,"width");
			h1D_et_min->Scale(1/h1D_et_min->Integral());
			h1D_et_dimu->Scale(1,"width");
			h1D_et_dimu->Scale(1/h1D_et_dimu->Integral());
		}
		h1D_et_min->SetLineWidth(3.5);
		h1D_et_dimu->SetLineWidth(3.5);
		h1D_et_min->Draw("hist");
		h1D_et_dimu->Draw("hist same");
		legBM -> AddEntry(h1D_et_min,"minimum bias sample","l");
		legBM -> AddEntry(h1D_et_dimu,"dimuon sample","l");
		legBM -> Draw();
		c1->SaveAs("et_dimu_min.png");
		c1->SaveAs("et_dimu_min.pdf");
		c1->Clear();
		legBM -> Clear();

	return;

}

