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


void draw_1D(bool isPrompt = true, bool is1st = true, bool isEmbedded = false, bool useCtErrRangeEff =true, bool useDataDrivenEff=false,  bool useZvtxWeightStep1 = false, bool useZvtxWeightStep2=true,char* dirName = "draw_1D_tnpV14")
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
	
	f2D = new TFile(Form("EffCounting_8rap9pt_%s_useCtErr_%d_useDataDriven_%d_useZvtxStep1_%d_Step2_%d.root",sampleName,(int)useCtErrRangeEff,(int)useDataDrivenEff,(int)useZvtxWeightStep1,(int)useZvtxWeightStep2));

	// --- read-in 2D hist
	TH2D* h2D_Eff = (TH2D*)f2D->Get("h2D_Eff_pt_y");
	const int nbinsX = h2D_Eff->GetNbinsX();
	const int nbinsY = h2D_Eff->GetNbinsY();
	cout << "h2D_Eff = " << h2D_Eff << endl;
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;

	// --- read bin edges (rapidity) and fill to Tree
	int nEdge = nbinsX+1; 
	double rapEdge[nEdge];
	for (Int_t ie = 0; ie < nEdge; ie++) {
		rapEdge[ie] = h2D_Eff->GetXaxis()->GetBinLowEdge(ie+1);
		cout << "rapEdge["<<ie<<"] = " << rapEdge[ie] << endl;
	}
	TTree* tRap = new TTree("tRap","");
	tRap->Branch("nEdge",&nEdge,"nEdge/I");
	tRap->Branch("rapEdge",&rapEdge,"rapEdge[nEdge]/D");
	tRap->Fill();
	
	// ---  projection to 1D hist
	TH1D* h1D_EffPt[nbinsX]; 
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_EffPt[iy] = h2D_Eff->ProjectionY(Form("h1D_EffPt_%d",iy),iy+1,iy+1);
		// test
		cout <<""<<endl;
		cout << iy <<"th rap " << endl;
		for (int ipt=0; ipt <nbinsY; ipt ++ ){
			cout << ipt <<"th pt" << endl;
			cout << "h1D_EffPt genBinCenter = " << h1D_EffPt[iy]->GetXaxis()->GetBinCenter(ipt+1) << endl;
			cout << "h1D_EffPt value = " << h1D_EffPt[iy]->GetBinContent(ipt+1) << endl;
		}
	}

	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms
	TCanvas* c1 = new TCanvas("c1","c1",600,600);
	c1->cd();
	cout << "sampleName = " << sampleName << endl;

	for (Int_t iy = 0; iy < nbinsX; iy++) {
		SetHistStyle(h1D_EffPt[iy],0,0);
		h1D_EffPt[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1D_EffPt[iy]->GetYaxis()->SetTitle("Efficiency");
		h1D_EffPt[iy]->SetMinimum(0.);
		h1D_EffPt[iy]->SetMaximum(1.);
		h1D_EffPt[iy]->Draw("pe");
		if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,1,1);
		else if (iy==2)  dashedLine (5.,0.,5.,1.,1,1);
		else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,1,1);
		latex->DrawLatex(0.50,0.31,sampleName);
		latex->DrawLatex(0.50,0.25,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		c1->SaveAs(Form("%s/%s_sf%d_EffPt_%d.pdf",dirName,sampleName,(int)useDataDrivenEff,iy));
	}
	
	// root file
	cout << "sampleName = " << sampleName << endl;
	//TFile *fOut = new TFile(Form("%s/%s_EffPt.root",dirName,sampleName),"RECREATE");
	TFile *fOut = new TFile(Form("%s/%s_sf%d_EffPt.root",dirName,sampleName,(int)useDataDrivenEff),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_EffPt[iy]->Write();
	}
	tRap->Write();
	fOut->Close();

	return;

}

