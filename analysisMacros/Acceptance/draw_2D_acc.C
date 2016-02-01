#include "SONGKYO.h"


void draw_2D_acc(int mrapnpt=89, bool isPrompt = false)
{
	gROOT->Macro("./Style2D.C");

	// --- read-in file
	TFile * f2D;
	char* sampleName;
	char* strPrompt;
	if (isPrompt) { strPrompt = "PRMC";}
	else { strPrompt = "NPMC";}
	char* strBinning;
	if (mrapnpt==89) strBinning = "8rap9pt";
	else if (mrapnpt==83) strBinning = "8rap3pt";
	else if (mrapnpt==63) strBinning = "6rap3pt";
	else if (mrapnpt==62) strBinning = "6rap2pt";
	else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return; }
	
	f2D = new TFile(Form("AccAna_%s_%s_boosted.root",strBinning,strPrompt));
	sampleName = Form("%s_%s",strBinning,strPrompt);
	cout << "sampleName = " << sampleName << endl;

	// --- read-in 2D hist
	TH2D* h2D_Den = (TH2D*)f2D->Get("h2D_Den_pt_y_Pbp");
	TH2D* h2D_Num = (TH2D*)f2D->Get("h2D_Num_pt_y_Pbp");
	TH2D* h2D_Acc = (TH2D*)f2D->Get("h2D_Acc_pt_y_Pbp");

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
/*
	h2D_Den->GetXaxis()->SetTitle("y_{lab}");
	h2D_Den->GetXaxis()->CenterTitle();
	h2D_Den->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Den->Draw("colz");
	c1->Update();
	pal = (TPaletteAxis*)h2D_Den->GetListOfFunctions()->FindObject("palette"); 
	pal->SetX2NDC(0.92);
	c1->Modified();
	c1->Update();
	c1->SaveAs(Form("dir_acc/h2D_Den_isPrompt%d.pdf",(int)isPrompt));

	h2D_Num->GetXaxis()->SetTitle("y_{lab}");
	h2D_Num->GetXaxis()->CenterTitle();
	h2D_Num->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Num->Draw("colz");
	c1->Update();
	pal = (TPaletteAxis*)h2D_Num->GetListOfFunctions()->FindObject("palette"); 
	pal->SetX2NDC(0.92);
	c1->Modified();
	c1->Update();
	c1->SaveAs(Form("dir_acc/h2D_Num_isPrompt%d.pdf",(int)isPrompt));
*/
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
	if (mrapnpt=89) {
		dashedLine(-2.4,2.0,-1.97,2.0,1,4);
		dashedLine(-1.97,2.0,-1.97,3.0,1,4);
		dashedLine(-1.97,3.0,-1.37,3.0,1,4);
		dashedLine(-1.37,3.0,-1.37,6.5,1,4);
		dashedLine(-1.37,6.5,1.03,6.5,1,4);
		dashedLine(1.03,5.0,1.03,6.5,1,4);
		dashedLine(1.03,5.0,1.46,5.0,1,4);
		dashedLine(1.46,3.0,1.46,5.0,1,4);
		dashedLine(1.46,3.0,1.93,3.0,1,4);
		dashedLine(1.93,2.0,1.93,3.0,1,4);
		dashedLine(1.93,2.0,2.4,2.0,1,4);
		dashedLine(2.4,2.0,2.4,30.0,1,4);
		dashedLine(-2.4,30.0,2.4,30.0,1,4);
		dashedLine(-2.4,2.0,-2.4,30.0,1,4);
	}
	else {
		dashedLine(-2.4,5.0,-1.97,5.0,1,4);
		dashedLine(-1.97,5.0,-1.97,6.5,1,4);
		dashedLine(-1.97,6.5,1.03,6.5,1,4);
		dashedLine(1.03,5.0,1.03,6.5,1,4);
		dashedLine(1.03,5.0,1.46,5.0,1,4);
		dashedLine(1.46,5.0,1.46,30.0,1,4);
		dashedLine(-2.4,30.0,1.46,30.0,1,4);
		dashedLine(-2.4,5.0,-2.4,30.0,1,4);
	}
	c1->SaveAs(Form("dir_acc/h2D_Acc_%s.pdf",sampleName));

	return;

}

