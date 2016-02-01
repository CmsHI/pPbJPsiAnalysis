#include "SONGKYO.h"

void draw_2D_acc_fine(bool isPrompt = true)
{
	gROOT->Macro("./Style2D.C");

	// --- read-in file
	TFile * f2D;
	char* sampleName;
	if (isPrompt) {
		f2D = new TFile("AccAna_8rap9pt_PRMC_boosted.root");
		sampleName = "PRMCpythia_Pbp";
	} 
	else {
		f2D = new TFile("AccAna_8rap9pt_NPMC_boosted.root");
		sampleName = "NPMCpythia_Pbp";
	}
	cout << "sampleName = " << sampleName << endl;

	// --- read-in 2D hist
	TH2D* h2D_Den = (TH2D*)f2D->Get("h2D_Den_pt_y_Pbp_fine");
	TH2D* h2D_Num = (TH2D*)f2D->Get("h2D_Num_pt_y_Pbp_fine");
	TH2D* h2D_Acc = (TH2D*)f2D->Get("h2D_Acc_pt_y_Pbp_fine");

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
	c1->SaveAs(Form("dir_acc/h2D_Den_isPrompt%d_fine.pdf",(int)isPrompt));

	h2D_Num->GetXaxis()->SetTitle("y_{lab}");
	h2D_Num->GetXaxis()->CenterTitle();
	h2D_Num->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Num->Draw("colz");
	c1->Update();
	pal = (TPaletteAxis*)h2D_Num->GetListOfFunctions()->FindObject("palette"); 
	pal->SetX2NDC(0.92);
	c1->Modified();
	c1->Update();
	c1->SaveAs(Form("dir_acc/h2D_Num_isPrompt%d_fine.pdf",(int)isPrompt));

	h2D_Acc->GetXaxis()->SetTitle("y_{lab}");
	h2D_Acc->GetXaxis()->CenterTitle();
	h2D_Acc->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Acc->SetMaximum(1.);
	h2D_Acc->Draw("colz");
	c1->Update();
	pal = (TPaletteAxis*)h2D_Acc->GetListOfFunctions()->FindObject("palette"); 
	pal->SetX2NDC(0.92);
	c1->Modified();
	c1->Update();
	c1->SaveAs(Form("dir_acc/h2D_Acc_isPrompt%d_fine.pdf",(int)isPrompt));

	return;

}

