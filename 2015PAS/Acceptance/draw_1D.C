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

// Pbp only
void draw_1D(bool isPrompt = true, bool is8rap9pt = true, char* dirName = "draw_1D_acc_newcut")
{
	gROOT->Macro("./JpsiStyle.C");
	gStyle->SetOptStat(0);
  //gStyle->SetLabelFont(42,"xyz");
  //gStyle->SetTitleSize(0.048,"xyz");
	//gStyle->SetPaintTextFormat(".3f"); // for text colz

	// --- read-in file
	TFile * f2D;
	char* strPrompt;
	char* strBin;

	if (isPrompt) strPrompt = "PRMC";
	else strPrompt = "NPMC";
	//if (is8rap9pt) { strBin = "8rap9pt";}
	//if (is8rap9pt) { strBin = "8rap9pt2gev";}
	if (is8rap9pt) { strBin = "8rap9pt2gev_newcut";}
	else { strBin = "6rap3pt";}
	
	f2D = new TFile(Form("AccAna_%s_%s_boosted.root",strBin,strPrompt));
	char* sampleName = Form("%s_%s",strBin,strPrompt);	
	cout << "sampleName = " << sampleName << endl;

	// --- read-in 2D hist
	TH2D* h2D_Acc = (TH2D*)f2D->Get("h2D_Acc_pt_y_Pbp");
	const int nbinsX = h2D_Acc->GetNbinsX();
	const int nbinsY = h2D_Acc->GetNbinsY();
	cout << "h2D_Acc = " << h2D_Acc << endl;
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;

	// --- read bin edges (rapidity) and fill to Tree
	int nEdge = nbinsX+1; 
	double rapEdge[nEdge];
	for (Int_t ie = 0; ie < nEdge; ie++) {
		rapEdge[ie] = h2D_Acc->GetXaxis()->GetBinLowEdge(ie+1);
		cout << "rapEdge["<<ie<<"] = " << rapEdge[ie] << endl;
	}
	TTree* tRap = new TTree("tRap","");
	tRap->Branch("nEdge",&nEdge,"nEdge/I");
	tRap->Branch("rapEdge",&rapEdge,"rapEdge[nEdge]/D");
	tRap->Fill();
	
	// ---  projection to 1D hist
	TH1D* h1D_AccPt[nbinsX]; 
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_AccPt[iy] = h2D_Acc->ProjectionY(Form("h1D_AccPt_%d",iy),iy+1,iy+1);
		// test
		cout <<""<<endl;
		cout << iy <<"th rap " << endl;
		for (int ipt=0; ipt <nbinsY; ipt ++ ){
			cout << ipt <<"th pt" << endl;
			cout << "h1D_AccPt genBinCenter = " << h1D_AccPt[iy]->GetXaxis()->GetBinCenter(ipt+1) << endl;
			cout << "h1D_AccPt value = " << h1D_AccPt[iy]->GetBinContent(ipt+1) << endl;
		}
	}

	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms
	//TCanvas* c1 = new TCanvas("c1","c1",600,600);
	TCanvas* c1 = new TCanvas("c1","c1",1600,800);
	c1->Divide(4,2);
	cout << "sampleName = " << sampleName << endl;

	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c1->cd(iy+1);
		SetHistStyle(h1D_AccPt[iy],0,0);
		h1D_AccPt[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1D_AccPt[iy]->GetYaxis()->SetTitle("Acceptance");
		h1D_AccPt[iy]->SetMinimum(0.);
		h1D_AccPt[iy]->SetMaximum(1.);
		h1D_AccPt[iy]->Draw("pe");
		if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,1,1);
		else if (iy==5)  dashedLine (5.,0.,5.,1.,1,1);
		else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,1,1);
		latex->DrawLatex(0.50,0.31,sampleName);
		latex->DrawLatex(0.50,0.25,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
	}
	c1->SaveAs(Form("%s/%s_AccPt.pdf",dirName,sampleName));
	
	// root file
	cout << "sampleName = " << sampleName << endl;
	TFile *fOut = new TFile(Form("%s/%s_AccPt.root",dirName,sampleName),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_AccPt[iy]->Write();
	}
	tRap->Write();
	fOut->Close();

	return;

}

