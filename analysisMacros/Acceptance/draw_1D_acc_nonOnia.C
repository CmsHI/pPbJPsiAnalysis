#include "SONGKYO.h"

//// for acceptance, use Pbp only
void draw_1D_acc(int mrapnpt=89, bool isPrompt=false, char* dirName="dir_acc")
{
	gROOT->Macro("./Style.C");

	TFile * f2D;
	char* strPrompt;
	if (isPrompt) strPrompt = "PRMC";
	else strPrompt = "NPMC";
	char* strBinning;
	if (mrapnpt==89) strBinning = "8rap9pt";
	else if (mrapnpt==83) strBinning = "8rap3pt";
	else if (mrapnpt==63) strBinning = "6rap3pt";
	else if (mrapnpt==62) strBinning = "6rap2pt";
	else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return; }
	
	f2D = new TFile(Form("AccAna_%s_%s_boosted.root",strBinning,strPrompt));
	char* sampleName = Form("%s_%s",strBinning,strPrompt);	
	cout << "sampleName = " << sampleName << endl;

	TH2D* h2D_Acc = (TH2D*)f2D->Get("h2D_Acc_pt_y_Pbp");
	const int nRap = h2D_Acc->GetNbinsX();
	const int nPt = h2D_Acc->GetNbinsY();

	// ----- read bin edges (rapidity) and fill to Tree
	int nEdge = nRap+1; 
	double rapEdge[nEdge];
	for (Int_t ie = 0; ie < nEdge; ie++) {
		rapEdge[ie] = h2D_Acc->GetXaxis()->GetBinLowEdge(ie+1);
		cout << "rapEdge["<<ie<<"] = " << rapEdge[ie] << endl;
	}
	TTree* tRap = new TTree("tRap","");
	tRap->Branch("nEdge",&nEdge,"nEdge/I");
	tRap->Branch("rapEdge",&rapEdge,"rapEdge[nEdge]/D");
	tRap->Fill();
	
	// -----  projection to 1D hist
	TH1D* h1D_AccPt[nRap]; 
	for (Int_t iy = 0; iy < nRap; iy++) {
		h1D_AccPt[iy] = h2D_Acc->ProjectionY(Form("h1D_AccPt_%d",iy),iy+1,iy+1);
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
	cout << "sampleName = " << sampleName << endl;

	for (Int_t iy = 0; iy < nRap; iy++) {
		c1->cd(iy+1);
		SetHistStyle(h1D_AccPt[iy],1,0);
		h1D_AccPt[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1D_AccPt[iy]->GetYaxis()->SetTitle("Acceptance");
		h1D_AccPt[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_AccPt[iy]->SetMinimum(0.);
		h1D_AccPt[iy]->SetMaximum(1.);
		h1D_AccPt[iy]->Draw("pe");
		//// low pT limit for Pbp
		if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,1,1);
		else if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,1,1);
		else if (iy==5)  dashedLine (5.,0.,5.,1.,1,1);
		else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,1,1);
		//latex->DrawLatex(0.50,0.31,sampleName);
		latex->DrawLatex(0.50,0.31,Form("%s",strPrompt));
		latex->DrawLatex(0.50,0.25,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		latex->DrawLatex(0.50,0.21,Form("%.1f < y_{CM} < %.1f",-1*rapEdge[iy+1]-0.47,-1*rapEdge[iy]-0.47));
	}
	c1->SaveAs(Form("%s/%s_AccPt.pdf",dirName,sampleName));
	
	//// save as a root file
	cout << "sampleName = " << sampleName << endl;
	TFile *fOut = new TFile(Form("%s/%s_AccPt.root",dirName,sampleName),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nRap; iy++) {
		h1D_AccPt[iy]->Write();
	}
	tRap->Write();
	fOut->Close();

	return;

}
