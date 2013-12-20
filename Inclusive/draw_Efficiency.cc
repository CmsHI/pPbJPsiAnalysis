#include <Riostream.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <vector>
#include <TSystem.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include "TStyle.h"
#include "TSystem.h"

#include <sstream>
#include <string>


void SetGraphStyle(TGraph* gr, Int_t c, Int_t m);
void SetHistStyle(TH1* h, Int_t c, Int_t m);
TGraph* referenceLine(Double_t nBins, Double_t xMin, Double_t xMax);
void SetLegendStyle(TLegend* l);
void SetTextStyle(TPaveText* t);


void draw_Efficiency(int fileCode=1)
{

	gROOT->Macro("./JpsiStyle.C");

	TFile * refFile;
	string mctype;

	if (fileCode==0) {
		refFile = new TFile("EffAna_m24_p147_pt_65300_PRMC_boosted.root","READ");
		mctype = "PR";
	} else if (fileCode==1) {
		refFile = new TFile("EffAna_m24_p147_pt_65300_NPMC_boosted.root","READ");
		mctype = "NP";
	} else {
		std::cout << " *** error! fileCode shoulde be 0(PR) or 1(NP) " << std::endl; 
	}
	//check 2D
/*	
	TH2D* h2D_NoCut_Reco_pt_y_m24_p147_pt_65300 = (TH2D*)refFile->Get("h2D_NoCut_Reco_pt_y_m24_p147_pt_65300");
	TH2D* h2D_NoCut_Gen_pt_y_m24_p147_pt_65300 = (TH2D*)refFile->Get("h2D_NoCut_Gen_pt_y_m24_p147_pt_65300");
	TH2D* h2D_Den_pt_y_m24_p147_pt_65300 = (TH2D*)refFile->Get("h2D_Den_pt_y_m24_p147_pt_65300");
	TH2D* h2D_Num_pt_y_m24_p147_pt_65300 = (TH2D*)refFile->Get("h2D_Num_pt_y_m24_p147_pt_65300");
	TH2D* h2D_Eff_pt_y_m24_p147_pt_65300 = (TH2D*)refFile->Get("h2D_Eff_pt_y_m24_p147_pt_65300");
*/
	//check 1D - real acceptance plots
	TH1D* h1D_Eff_y_bin1_m24_p147_pt_65300 = (TH1D*)refFile->Get("h1D_Eff_y_bin1_m24_p147_pt_65300");
	TH1D* h1D_Eff_pt_bin2_FW_m24_p147_pt_65300 = (TH1D*)refFile->Get("h1D_Eff_pt_bin2_FW_m24_p147_pt_65300");
	TH1D* h1D_Eff_pt_bin2_BW_m24_p147_pt_65300 = (TH1D*)refFile->Get("h1D_Eff_pt_bin2_BW_m24_p147_pt_65300");
	
   //the Legend
 //  Legend *legUR = new TLegend(0.55, 0.74, 0.80, 0.90); //upper right
 //  TLegend *leg = new TLegend(0.20, 0.70, 0.50, 0.90); //upper left
//   TLegend *leg = new TLegend(0.20, 0.15, 0.48, 0.33); //bottom left
//   SetLegendStyle(leg);
 
	//latex box for beam, rapidity, pT info
	TLatex* tex = new TLatex();
	tex->SetNDC();
	tex->SetTextAlign(12);
	tex->SetTextSize(0.04);

	// --- Draw histograms
/*	// c1 for 2D
	TCanvas* c1 = new TCanvas("c1","c1",800,600);
	c1->cd();
	// for example
	gPad->SetLeftMargin(0.13);
  //gStyle->SetPadRightMargin(0.15); //KYO
	h2D_NoCut_Reco_pt_y_m24_p147_pt_65300->GetXaxis()->SetTitle("y_{lab}");
	h2D_NoCut_Reco_pt_y_m24_p147_pt_65300->GetXaxis()->CenterTitle();
	h2D_NoCut_Reco_pt_y_m24_p147_pt_65300->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_NoCut_Reco_pt_y_m24_p147_pt_65300->GetXaxis()->SetRangeUser(-2.4,1.47);
	h2D_NoCut_Reco_pt_y_m24_p147_pt_65300->GetYaxis()->SetRangeUser(0.,30.);
	h2D_NoCut_Reco_pt_y_m24_p147_pt_65300->Draw("colz");
	c1->SaveAs("h2D_NoCut_Reco_pt_y_m24_p147_pt_65300.pdf");
	c1->Clear();
*/

	// c2 for 1D
	TCanvas* c2 = new TCanvas("c2","c2",600,600);
	c2->cd();

	gPad->SetLeftMargin(0.13);
	SetHistStyle(h1D_Eff_y_bin1_m24_p147_pt_65300,1,0);
	h1D_Eff_y_bin1_m24_p147_pt_65300->GetXaxis()->SetTitle("y_{lab}");
	h1D_Eff_y_bin1_m24_p147_pt_65300->GetYaxis()->SetTitle("Efficiency");
	h1D_Eff_y_bin1_m24_p147_pt_65300->GetXaxis()->CenterTitle();
	h1D_Eff_y_bin1_m24_p147_pt_65300->GetXaxis()->SetRangeUser(-2.4,1.47);
	h1D_Eff_y_bin1_m24_p147_pt_65300->GetYaxis()->SetRangeUser(0.,1);
	h1D_Eff_y_bin1_m24_p147_pt_65300->Draw("ep");
	if (fileCode ==0) {
		tex->DrawLatex(0.49, 0.44, "prompt J/#psi MC");
	} else if (fileCode ==1) {
		tex->DrawLatex(0.49, 0.44, "non-prompt J/#psi MC");
	}
	tex->DrawLatex(0.49, 0.37, "pp boosted #sqrt{s_{NN}} = 5 TeV");
	tex->DrawLatex(0.49, 0.23, "6.5 < p_{T} < 30 GeV/c");
	tex->DrawLatex(0.49, 0.30, "centrality 0-100 %");
	c2->SaveAs(Form("h1D_%s_Eff_y_bin1_m24_p147_pt_65300.pdf",mctype.c_str()));
	//	c2->Clear();
	//textUR->Clear();
	TCanvas* c3 = new TCanvas("c3","c3",600,600);

	SetHistStyle(h1D_Eff_pt_bin2_FW_m24_p147_pt_65300,1,0);
	h1D_Eff_pt_bin2_FW_m24_p147_pt_65300->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	h1D_Eff_pt_bin2_FW_m24_p147_pt_65300->GetYaxis()->SetTitle("Efficiency");
	h1D_Eff_pt_bin2_FW_m24_p147_pt_65300->GetXaxis()->CenterTitle();
	h1D_Eff_pt_bin2_FW_m24_p147_pt_65300->GetXaxis()->SetRangeUser(6.5,30.);
	h1D_Eff_pt_bin2_FW_m24_p147_pt_65300->GetYaxis()->SetRangeUser(0.,1);
	h1D_Eff_pt_bin2_FW_m24_p147_pt_65300->Draw("ep");
	if (fileCode ==0) {
		tex->DrawLatex(0.49, 0.44, "prompt J/#psi MC");
	} else if (fileCode ==1) {
		tex->DrawLatex(0.49, 0.44, "non-prompt J/#psi MC");
	}
	tex->DrawLatex(0.49, 0.37, "pp boosted #sqrt{s_{NN}} = 5 TeV");
	tex->DrawLatex(0.49, 0.23, "-2.4 < y_{lab} < -0.47");
	tex->DrawLatex(0.49, 0.30, "centrality 0-100 %");
	//	c3->SaveAs(Form("h1D_%s_Eff_pt_bin2_FW_m24_p147_pt_65300.pdf",mctype.c_str()));
	//textBR->Clear();
	TCanvas* c4 = new TCanvas("c2","c2",600,600);

	SetHistStyle(h1D_Eff_pt_bin2_BW_m24_p147_pt_65300,1,0);
	h1D_Eff_pt_bin2_BW_m24_p147_pt_65300->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	h1D_Eff_pt_bin2_BW_m24_p147_pt_65300->GetYaxis()->SetTitle("Efficiency");
	h1D_Eff_pt_bin2_BW_m24_p147_pt_65300->GetXaxis()->CenterTitle();
	h1D_Eff_pt_bin2_BW_m24_p147_pt_65300->GetXaxis()->SetRangeUser(6.5,30.);
	h1D_Eff_pt_bin2_BW_m24_p147_pt_65300->GetYaxis()->SetRangeUser(0.,1);
	h1D_Eff_pt_bin2_BW_m24_p147_pt_65300->Draw("ep");
	if (fileCode ==0) {
		tex->DrawLatex(0.49, 0.44, "prompt J/#psi MC");
	} else if (fileCode ==1) {
		tex->DrawLatex(0.49, 0.44, "non-prompt J/#psi MC");
	}
	tex->DrawLatex(0.49, 0.37, "pp boosted #sqrt{s_{NN}} = 5 TeV");
	tex->DrawLatex(0.49, 0.23, "-0.47 < y_{lab} < 1.47");
	tex->DrawLatex(0.49, 0.30, "centrality 0-100 %");
	c4->SaveAs(Form("h1D_%s_Eff_pt_bin2_BW_m24_p147_pt_65300.pdf",mctype.c_str()));
	//textBR->Clear();

	return;

}

// sub-routines

void SetGraphStyle(TGraph* gr, Int_t c, Int_t m) {
	Int_t colorArr[] = { kGray+3, kRed-4, kBlue, kOrange+7, kGreen+3, kViolet-4, kAzure+8, kGreen+1, kBlack };
	Int_t markerFullArr[] = {kFullCircle, kFullTriangleUp, kFullTriangleDown, kFullSquare};
	//Int_t markerFullArr[] = {kFullCircle, kFullTriangleUp, kFullTriangleDown, kFullSquare, kFullStar, kFullDiamond};
	//Int_t markerOpenArr[] = {kOpenCircle, kOpenTriangleUp, kOpenTriangleDown, kOpenSquare, kOpenStar, kOpenDiamond};

	gr-> SetMarkerColor(colorArr[c]);
	gr-> SetMarkerStyle(markerFullArr[m]);
	gr-> SetMarkerSize(1.2);
	gr-> SetLineColor(colorArr[c]);
	gr-> SetLineWidth(0.8);
}

void SetHistStyle(TH1* h, Int_t c, Int_t m) {
	Int_t colorArr[] = { kGray+3, kRed-4, kBlue, kOrange+7, kGreen+3, kAzure+8, kViolet-4, kBlack };
	//Int_t colorArr[] = { kGray+3, kRed-4, kBlue, kOrange+1, kGreen-3, kAzure+8, kViolet-4, kBlack };
	Int_t markerFullArr[] = {kFullCircle, kFullTriangleUp, kFullTriangleDown, kFullSquare};
	//Int_t markerOpenArr[] = {kOpenCircle, kOpenTriangleUp, kOpenTriangleDown, kOpenSquare};
	h-> SetMarkerColor(colorArr[c]);
	h-> SetMarkerStyle(markerFullArr[m]);
	h-> SetMarkerSize(1.2);
	h-> SetLineColor(colorArr[c]);
	h-> SetLineWidth(1.8);
}

TGraph* referenceLine(Double_t nBins, Double_t xMin, Double_t xMax) {
	Double_t x[531];
	Double_t y[531];
	for(Int_t n=0; n<nBins; n++) {
		//x[n]=binSet[0]+1.2*n*((binSet[nBins]-binSet[0])/nBins);
		x[n]=xMin+1.2*n*((xMax-xMin)/nBins);
			x[n]=xMin+n*((xMax-xMin)/nBins);
		y[n]=1;
		}
	TGraph* gr_ref = new TGraph(nBins, x+1, y);
	gr_ref->SetLineColor(kRed);
	//gr_ref->Draw("L");
	return gr_ref;
}

void SetLegendStyle(TLegend* l) {
	l->SetFillColor(0);
	l->SetFillStyle(4000);
	l->SetBorderSize(0);
	l->SetMargin(0.2);
	l->SetTextSize(0.040);
}

void SetTextStyle(TPaveText* t) {
	t->SetFillColor(0);
	t->SetFillStyle(4000);
	t->SetBorderSize(0);
	t->SetMargin(0.2);
	t->SetTextSize(0.035);
}
