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


void draw_compRatio(bool isPrompt = true, bool is1st = false, char* dirName = "draw_1D_0113", char* dirName02 = "draw_compRatio_0113")
{
	gROOT->Macro("./JpsiStyle.C");

	// --- read-in file
	TFile * f01; //pythia
	TFile * f02; //embedded
	char* sampleName;

	if (is1st) {
		if (isPrompt) {
			f01 = new TFile(Form("%s/PRMCpythia_Pbp_EffPt.root",dirName));
			f02 = new TFile(Form("%s/PRMCembedded_Pbp_EffPt.root",dirName));
			sampleName = "PRMC_Pbp";
		}
		else {
			f01 = new TFile(Form("%s/NPMCpythia_Pbp_EffPt.root",dirName));
			f02 = new TFile(Form("%s/NPMCembedded_Pbp_EffPt.root",dirName));
			sampleName = "NPMC_Pbp";
		}	 
	}
	else {
		if (isPrompt) {
			f01 = new TFile(Form("%s/PRMCpythia_pPb_EffPt.root",dirName));
			f02 = new TFile(Form("%s/PRMCembedded_pPb_EffPt.root",dirName));
			sampleName = "PRMC_pPb";
		}
		else {
			f01 = new TFile(Form("%s/NPMCpythia_pPb_EffPt.root",dirName));
			f02 = new TFile(Form("%s/NPMCembedded_pPb_EffPt.root",dirName));
			sampleName = "NPMC_pPb";
		}	 
	}
	cout << "sampleName = " << sampleName << endl;

	// --- read-in tree
	TTree *tr = (TTree*)f01->Get("tRap");
	int nEdge;
	double rapEdge[531];
	TBranch *b_nEdge;
	TBranch *b_rapEdge;
	tr->SetBranchAddress("nEdge",&nEdge,&b_nEdge);
	tr->SetBranchAddress("rapEdge",&rapEdge,&b_rapEdge);
	cout << "tr entries = " << tr->GetEntries() << endl;
	for (int ev=0; ev <tr->GetEntries(); ev++){
		tr->GetEntry(ev);
	}
	cout << "nEdge = " << nEdge << endl;
	for (Int_t i=0; i< nEdge; i++) {
		cout << "rapEdge["<<i<<"] = " << rapEdge[i] << endl;
	}

	// --- read-in eff hist
	const int nRap = nEdge -1 ; 
	cout << "nRap = " << nRap << endl;
	TH1D* h1D_EffPt01[nRap]; 
	TH1D* h1D_EffPt02[nRap]; 
	TH1D *hRatio[nRap];
	for (int iy=0; iy < nRap; iy ++){
		h1D_EffPt01[iy] = (TH1D*)f01->Get(Form("h1D_EffPt_%d",iy));
		h1D_EffPt02[iy] = (TH1D*)f02->Get(Form("h1D_EffPt_%d",iy));
	cout << "h1D_EffPt01["<<iy<<"] = " << h1D_EffPt01[iy] << endl;
	cout << "h1D_EffPt02["<<iy<<"] = " << h1D_EffPt02[iy] << endl;
		hRatio[iy]=(TH1D*)h1D_EffPt01[iy]->Clone(Form("hRatio_%d",iy));
		hRatio[iy]->Divide(h1D_EffPt02[iy]);
	}
	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms

	TCanvas* c1 = new TCanvas("c1","c1",600,600);
	TCanvas* c2 = new TCanvas("c2","c2",600,600);
	//TLegend *legBR = new TLegend(0.58,0.30,0.85,0.40,NULL,"brNDC");
	TLegend *legBR = new TLegend(0.50,0.30,0.80,0.41,NULL,"brNDC");
	SetLegendStyle(legBR);

	for (Int_t iy = 0; iy < nRap; iy++) {
		// --- pT dist
		c1->cd();
		SetHistStyle(h1D_EffPt01[iy],3,0);
		SetHistStyle(h1D_EffPt02[iy],4,0);
		h1D_EffPt01[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1D_EffPt01[iy]->GetYaxis()->SetTitle("Efficiency");
		h1D_EffPt01[iy]->SetMinimum(0.);
		h1D_EffPt01[iy]->SetMaximum(1.);
		h1D_EffPt01[iy]->Draw("pe");
		h1D_EffPt02[iy]->Draw("pe same");
		latex->DrawLatex(0.53,0.25,sampleName);
		latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		legBR->AddEntry(h1D_EffPt01[iy],"MC pythia","lp");
		legBR->AddEntry(h1D_EffPt02[iy],"MC embedded","lp");
		legBR->Draw();
		c1->SaveAs(Form("%s/%s_compEff_%d.pdf",dirName02,sampleName,iy));
		c1->SaveAs(Form("%s/%s_compEff_%d.png",dirName02,sampleName,iy));
		legBR->Clear();
		//c1->Clear();

		// --- ratio
		c2->cd();
		SetHistStyle(hRatio[iy],1,0);
		hRatio[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		hRatio[iy]->GetYaxis()->SetTitle("pythia/embedded");
		hRatio[iy]->SetMinimum(0.5);
		hRatio[iy]->SetMaximum(1.5);
		hRatio[iy]->Draw("pe");
		dashedLine(0.,1.,30.,1.,1,1);
		latex->DrawLatex(0.53,0.25,sampleName);
		latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		c2->SaveAs(Form("%s/%s_compRatio_%d.pdf",dirName02,sampleName,iy));
		c2->SaveAs(Form("%s/%s_compRatio_%d.png",dirName02,sampleName,iy));
		//c2->Clear();
	}

/*	
	// root file
	TFile *fOut = new TFile(Form("%s/%s_EffPt.root",dirName02,sampleName),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nRap; iy++) {
		h1D_EffPt[iy]->Write();
	}
	fOut->Close();
*/
	return;

}

