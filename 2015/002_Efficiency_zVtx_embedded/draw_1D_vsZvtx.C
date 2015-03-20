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


void draw_1D_vsZvtx(bool isPrompt = false, bool is1st = true, bool isEmbedded = false, bool useCtErrRangeEff =false, bool useDataDrivenEff=false,  bool useZvtxWeightStep1 = false, bool useZvtxWeightStep2=false,char* dirName = "draw_1D_vsZvtx")
{
	gROOT->Macro("./JpsiStyle.C");
	//gStyle->SetOptStat(0);
  //gStyle->SetLabelFont(42,"xyz");
  //gStyle->SetTitleSize(0.048,"xyz");
	//gStyle->SetPaintTextFormat(".3f"); // for text colz

	// --- read-in file
	TFile * f2D;
	char* strEmbd;
	char* strPrompt;
	char* str1st;

	if (isEmbedded) strEmbd = "embedded";
	else strEmbd = "pythia";
	if (isPrompt) strPrompt = "PRMC";
	else strPrompt = "NPMC";
	if (is1st) str1st = "Pbp";
	else str1st = "pPb";

	const char* sampleName = Form("%s%s_%s",strPrompt,strEmbd,str1st);
	cout << "sampleName = " << sampleName << endl;
	
	f2D = new TFile(Form("vsZvtx_EffCounting_8rap9pt_%s_useCtErr_%d_useDataDriven_%d_useZvtxStep1_%d_Step2_%d.root",sampleName,(int)useCtErrRangeEff,(int)useDataDrivenEff,(int)useZvtxWeightStep1,(int)useZvtxWeightStep2));
	//f2D = new TFile(Form("EffCounting_8rap9pt_%s_useCtErr_%d_useDataDriven_%d_useZvtxStep1_%d_Step2_%d.root",sampleName,(int)useCtErrRangeEff,(int)useDataDrivenEff,(int)useZvtxWeightStep1,(int)useZvtxWeightStep2));
	//f2D = new TFile(Form("EffGenmatching_8rap9pt_%s_useCtErr_%d_useDataDriven_%d_useZvtxStep1_%d_Step2_%d.root",sampleName,(int)useCtErrRangeEff,(int)useDataDrivenEff,(int)useZvtxWeightStep1,(int)useZvtxWeightStep2));

	const int nYBins = 8;
	TH1D* h1D_EffvsZvtx[nYBins];
	for (int in=0; in <nYBins; in++) {
		h1D_EffvsZvtx[in] = (TH1D*)f2D->Get(Form("h1D_Eff_zVtx_%d",in));
		h1D_EffvsZvtx[in]->SetName(Form("h1D_Eff_zVtx_%d",in));
		cout << in << "th _EffvsZvtx[in] = " << h1D_EffvsZvtx[in] << endl;
		h1D_EffvsZvtx[in]->GetYaxis()->SetRangeUser(0.,1.);
	}
	
	Double_t ptLimit[nYBins];
	if (is1st) ptLimit= {0.0, 3.0, 6.5, 6.5, 6.5, 5.0, 3.0, 0.};
	else ptLimit = {0.0, 3.0, 5.0, 6.5, 6.5, 6.5, 3.0, 0.};
	for (Int_t i=0; i<nYBins; i++){	
		cout << i <<"th ptLimit = " << ptLimit[i] <<endl;
	}

	
	double rapEdge[nYBins+1];
	if (is1st) rapEdge = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
	else rapEdge = {-2.4, -1.93, -1.46, -1.03, -0.43, 0.47, 1.37, 1.97, 2.4};
	for (Int_t i=0; i<nYBins; i++){	
		cout << i <<"th rapEdge = " << rapEdge[i] <<endl;
	}


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
	cout << "sampleName = " << sampleName << endl;

	for (Int_t iy = 0; iy < nYBins; iy++) {
		c1->cd(iy+1);
		SetHistStyle(h1D_EffvsZvtx[iy],0,0);
		h1D_EffvsZvtx[iy]->GetXaxis()->SetTitle("primary Z vertex");
		h1D_EffvsZvtx[iy]->GetYaxis()->SetTitle("Efficiency");
		h1D_EffvsZvtx[iy]->SetMinimum(0.);
		h1D_EffvsZvtx[iy]->SetMaximum(1.);
		h1D_EffvsZvtx[iy]->Draw("pe");
		//if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,1,1);
		//else if (iy==2)  dashedLine (5.,0.,5.,1.,1,1);
		//else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,1,1);
		//latex->DrawLatex(0.50,0.31,sampleName);
		//latex->DrawLatex(0.50,0.25,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		if (iy==0) latex->DrawLatex(0.50,0.87,sampleName);
		latex->DrawLatex(0.50,0.80,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		latex->DrawLatex(0.50,0.73,Form("%.1f < p_{T} <30 GeV/c",ptLimit[iy]));
//		c1->SaveAs(Form("%s/%s_EffPt_%d.pdf",dirName,sampleName,iy));
//		c1->SaveAs(Form("%s/%s_sf%d_EffPt_%d.pdf",dirName,sampleName,(int)useDataDrivenEff,iy));
	}

	c1->SaveAs(Form("%s/vsZvtx_%s.pdf",dirName,sampleName));
/*	
	// root file
	cout << "sampleName = " << sampleName << endl;
	//TFile *fOut = new TFile(Form("%s/%s_EffPt.root",dirName,sampleName),"RECREATE");
	//TFile *fOut = new TFile(Form("%s/%s_sf%d_EffPt.root",dirName,sampleName,(int)useDataDrivenEff),"RECREATE");
	TFile *fOut = new TFile(Form("%s/1Dhist_EffCounting_8rap9pt_%s_useCtErr_%d_useDataDriven_%d_useZvtxStep1_%d_Step2_%d.root",dirName,sampleName,(int)useCtErrRangeEff,(int)useDataDrivenEff,(int)useZvtxWeightStep1,(int)useZvtxWeightStep2),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_EffvsZvtx[iy]->Write();
	}
	tRap->Write();
	fOut->Close();
*/
	return;

}

