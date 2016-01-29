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


void draw_1D_eff(bool isPrompt = true, bool is1st = true, bool useCtErrCut =true, bool useSF=true, bool useZvtxWeight=true,char* dirName = "dir_1D_eff")
{
	gROOT->Macro("./JpsiStyle.C");

	// --- read-in file
	TFile * f2D;
	char* strPrompt;
	char* str1st;
	if (isPrompt) strPrompt = "PRMC";
	else strPrompt = "NPMC";
	if (is1st) str1st = "Pbp";
	else str1st = "pPb";
	const char* sampleName = Form("%spythia_%s",strPrompt,str1st);
	cout << "sampleName = " << sampleName << endl;
	
	f2D = new TFile(Form("EffAna_8rap9pt_%s_CtErr%d_SF%d_Zvtx%d.root",sampleName,(int)useCtErrCut,(int)useSF,(int)useZvtxWeight));

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
	TCanvas* c1 = new TCanvas("c1","c1",1600,800);
	c1->Divide(4,2);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c1->cd(iy+1);
		SetHistStyle(h1D_EffPt[iy],2,0);
		h1D_EffPt[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1D_EffPt[iy]->GetYaxis()->SetTitle("Efficiency");
		h1D_EffPt[iy]->GetXaxis()->SetRangeUser(0,30);
		h1D_EffPt[iy]->SetMinimum(0.);
		h1D_EffPt[iy]->SetMaximum(1.);
		h1D_EffPt[iy]->Draw("pe");
		if (is1st) {
			if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,1,1);
			else if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,1,1);
			else if (iy==5)  dashedLine (5.,0.,5.,1.,1,1);
			else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,1,1);
		}
		else {
			if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,1,1);
			else if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,1,1);
			else if (iy==2)  dashedLine (5.,0.,5.,1.,1,1);
			else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,1,1);
		}
		latex->DrawLatex(0.50,0.31,Form("%s",strPrompt));
		latex->DrawLatex(0.50,0.25,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		if (is1st) {latex->DrawLatex(0.50,0.21,Form("%.1f < y_{CM} < %.1f",-1*rapEdge[iy+1]-0.47,-1*rapEdge[iy]-0.47));}
		else {latex->DrawLatex(0.50,0.21,Form("%.1f < y_{CM} < %.1f",rapEdge[iy]+0.47,rapEdge[iy+1]+0.47));}
	}
		c1->SaveAs(Form("%s/%s_EffPt.pdf",dirName,sampleName));
	
	// root file
	cout << "sampleName = " << sampleName << endl;
	TFile *fOut = new TFile(Form("%s/1Dhist_EffAna_8rap9pt_%s_CtErr%d_useSF%d_useZvtx%d.root",dirName,sampleName,(int)useCtErrCut,(int)useSF,(int)useZvtxWeight),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_EffPt[iy]->Write();
	}
	tRap->Write();
	fOut->Close();

	return;

}

