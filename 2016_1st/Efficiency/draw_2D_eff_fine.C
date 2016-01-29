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
#include "TPaletteAxis.h"

#include <TMath.h>
#include <math.h>

#include <sstream>
#include <string>

#include "KYOcommonOpt.h"


void draw_2D_eff_fine(bool isPrompt =false)
{
	gROOT->Macro("./JpsiStyle2D.C");

	// --- read-in file
	TFile * f2D;
	char* sampleName;
	if (isPrompt) {
		f2D = new TFile("EffAna_fine_PRMCpythia_Pbp_CtErr0_SF0_Zvtx1.root");
		sampleName = "PRMCpythia_Pbp";
	} 
	else {
		f2D = new TFile("EffAna_fine_NPMCpythia_Pbp_CtErr0_SF0_Zvtx1.root");
		sampleName = "NPMCpythia_Pbp";
	}
	cout << "sampleName = " << sampleName << endl;

	// --- read-in 2D hist
	TH2D* h2D_Den = (TH2D*)f2D->Get("h2D_Den_pt_y");
	TH2D* h2D_Num = (TH2D*)f2D->Get("h2D_Num_pt_y");
	TH2D* h2D_Eff = (TH2D*)f2D->Get("h2D_Eff_pt_y");

	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms
	TCanvas* c1 = new TCanvas("c1","c1",700,600);
	TPaletteAxis* pal; 
	c1->cd();
	h2D_Den->GetXaxis()->SetTitle("y_{lab}");
	h2D_Den->GetXaxis()->CenterTitle();
	h2D_Den->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Den->Draw("colz");
	c1->Update();
	pal = (TPaletteAxis*)h2D_Den->GetListOfFunctions()->FindObject("palette"); 
	pal->SetX2NDC(0.92);
	c1->Modified();
	c1->Update();
	c1->SaveAs(Form("dir_2D_eff_fine/h2D_Den_isPrompt%d.pdf",(int)isPrompt));

	h2D_Num->GetXaxis()->SetTitle("y_{lab}");
	h2D_Num->GetXaxis()->CenterTitle();
	h2D_Num->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Num->Draw("colz");
	c1->Update();
	pal = (TPaletteAxis*)h2D_Num->GetListOfFunctions()->FindObject("palette"); 
	pal->SetX2NDC(0.92);
	c1->Modified();
	c1->Update();
	c1->SaveAs(Form("dir_2D_eff_fine/h2D_Num_isPrompt%d.pdf",(int)isPrompt));

	h2D_Eff->GetXaxis()->SetTitle("y_{lab}");
	h2D_Eff->GetXaxis()->CenterTitle();
	h2D_Eff->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Eff->SetMaximum(1.);
	h2D_Eff->Draw("colz");
	c1->Update();
	pal = (TPaletteAxis*)h2D_Eff->GetListOfFunctions()->FindObject("palette"); 
	pal->SetX2NDC(0.92);
	c1->Modified();
	c1->Update();
	c1->SaveAs(Form("dir_2D_eff_fine/h2D_Eff_isPrompt%d.pdf",(int)isPrompt));

	return;

}

