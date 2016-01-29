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

#include "KYOcommonOptMulti.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

//// runCode // 0=merged, 1=1stRun, 2=2ndRun
void draw_1D_nSig_pt(char* dirName = "8rap9pt", bool is1st= true, bool isLog=false)
{
	gROOT->Macro("./JpsiStyle.C");
	
	//rap array in yCM (from forward to backward)
	Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	//Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	const Int_t nRap = sizeof(rapArrNumFB)/sizeof(Double_t)-1;
	cout << "nRap = " << nRap << endl;
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
		cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
	}
	//pt array
	Double_t ptArrNum[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
	const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;
	cout << "nPt = " << nPt << endl;
	Double_t ptBinW[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
		cout << ipt <<"th ptBinW = " << ptBinW[ipt] <<endl;
	}
	// array string
	string rapArr[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
		cout << iy <<"th rapArr = " << rapArr[iy] << endl;
	}
	string ptArr[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
		cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
	}
	
	// --- read-in file
	//TFile * f2D = new TFile(Form("./total2Dhist_%s.root",dirName));
	TFile * f2D = new TFile(Form("./total2Dhist_%s_noPtWeight.root",dirName));
	cout << "dirName = " << dirName << endl;

	// --- read-in 2D hist for data reco dist
	TH2D* h2D_fit_pt_y_PR_Pbp;
	TH2D* h2D_fit_pt_y_PR_pPb;
	h2D_fit_pt_y_PR_Pbp = (TH2D*)f2D->Get("otherFitInfo/h2D_fit_pt_y_nSig_Pbp");
	h2D_fit_pt_y_PR_pPb = (TH2D*)f2D->Get("otherFitInfo/h2D_fit_pt_y_nSig_pPb");
	h2D_fit_pt_y_PR_Pbp->SetName("h2D_fit_pt_y_nSig_Pbp");
	h2D_fit_pt_y_PR_pPb->SetName("h2D_fit_pt_y_nSig_pPb");
	cout << "h2D_fit_pt_y_PR_Pbp = " << h2D_fit_pt_y_PR_Pbp << endl;
	cout << "h2D_fit_pt_y_PR_pPb = " << h2D_fit_pt_y_PR_pPb << endl;

	const int nbinsX = h2D_fit_pt_y_PR_Pbp->GetNbinsX();
	const int nbinsY = h2D_fit_pt_y_PR_Pbp->GetNbinsY();
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
	// ---  projection to 1D hist
	TH1D* h1D_fit_PR_Pbp[nbinsX]; 
	TH1D* h1D_fit_PR_pPb[nbinsX]; 
	
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_fit_PR_Pbp[iy] = h2D_fit_pt_y_PR_Pbp->ProjectionY(Form("h1D_fit_nSig_Pbp_%d",iy),iy+1,iy+1);
		h1D_fit_PR_pPb[iy] = h2D_fit_pt_y_PR_pPb->ProjectionY(Form("h1D_fit_nSig_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
	// set values as zero for unused bins
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_fit_PR_Pbp[iy]->SetBinContent(1,0);
			h1D_fit_PR_Pbp[iy]->SetBinError(1,0);
			h1D_fit_PR_pPb[iy]->SetBinContent(1,0);
			h1D_fit_PR_pPb[iy]->SetBinError(1,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_fit_PR_Pbp[iy]->SetBinContent(2,0);
			h1D_fit_PR_Pbp[iy]->SetBinContent(3,0);
			h1D_fit_PR_Pbp[iy]->SetBinError(2,0);
			h1D_fit_PR_Pbp[iy]->SetBinError(3,0);
			h1D_fit_PR_pPb[iy]->SetBinContent(2,0);
			h1D_fit_PR_pPb[iy]->SetBinContent(3,0);
			h1D_fit_PR_pPb[iy]->SetBinError(2,0);
			h1D_fit_PR_pPb[iy]->SetBinError(3,0);
		}
		if (iy>=2 && iy<=4) {
			h1D_fit_PR_Pbp[iy]->SetBinContent(4,0);
			h1D_fit_PR_Pbp[iy]->SetBinError(4,0);
			h1D_fit_PR_pPb[iy]->SetBinContent(4,0);
			h1D_fit_PR_pPb[iy]->SetBinError(4,0);
		}
	}
	
	////// after zero-bin setting, normalize!
//	for (Int_t iy = 0; iy < nbinsX; iy++) {
//		h1D_fit_PR_Pbp[iy]->Scale(1./h1D_fit_PR_Pbp[iy]->Integral());
//		h1D_fit_NP_Pbp[iy]->Scale(1./h1D_fit_NP_Pbp[iy]->Integral());
//		h1D_fit_PR_Pbp[iy]->Scale(1,"width");
//		h1D_fit_NP_Pbp[iy]->Scale(1,"width");
//	}
	
	//////////////////////////////////////////////////////////////////
	//// --- Draw histograms

	//TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
	TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92); //upper left
	TLegend *legBL = new TLegend(0.18, 0.18, 0.59, 0.35); //upper left
	TLegend *legBRPbp = new TLegend(0.49, 0.38, 0.86, 0.50); //upper left
	TLegend *legBRpPb = new TLegend(0.49, 0.38, 0.86, 0.50); //upper left
	SetLegendStyle(legUR);
	SetLegendStyle(legBL);
	SetLegendStyle(legBRPbp);
	SetLegendStyle(legBRpPb);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	TCanvas* c_Pbp = new TCanvas("c_Pbp","c_Pbp",200,10,1600,800);
	c_Pbp->Divide(4,2);
	TCanvas* c_pPb = new TCanvas("c_pPb","c_pPb",200,10,1600,800);
	c_pPb->Divide(4,2);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c_Pbp->cd(iy+1);
		if (isLog) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetHistStyle(h1D_fit_PR_Pbp[iy],1,0);
		h1D_fit_PR_Pbp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_fit_PR_Pbp[iy]->GetXaxis()->CenterTitle();
		h1D_fit_PR_Pbp[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_fit_PR_Pbp[iy]->GetYaxis()->SetTitle("raw yield");
		h1D_fit_PR_Pbp[iy]->Draw("pe");
		if (iy==0) {
			legBRPbp -> SetHeader("Pbp");
			legBRPbp -> AddEntry(h1D_fit_PR_Pbp[iy],"Prompt J/#psi","lp");
			//legBRPbp->Draw();
		}
		latex->DrawLatex(0.49,0.80,Form("%s",rapArr[iy].c_str()));
		
		c_pPb->cd(iy+1);
		if (isLog) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetHistStyle(h1D_fit_PR_pPb[iy],1,0);
		h1D_fit_PR_pPb[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_fit_PR_pPb[iy]->GetXaxis()->CenterTitle();
		h1D_fit_PR_pPb[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_fit_PR_pPb[iy]->GetYaxis()->SetTitle("raw yield");
		h1D_fit_PR_pPb[iy]->Draw("pe");
		if (iy==0) {
			legBRpPb -> SetHeader("pPb");
			legBRpPb -> AddEntry(h1D_fit_PR_Pbp[iy],"Prompt J/#psi","lp");
			//legBRpPb ->Draw();
		}
		latex->DrawLatex(0.49,0.80,Form("%s",rapArr[iy].c_str()));
	}

	//c_Pbp->SaveAs(Form("dir_1D_%s/nSig_pt_isLog%d_Pbp.pdf",dirName,(int)isLog));
	//c_pPb->SaveAs(Form("dir_1D_%s/nSig_pt_isLog%d_pPb.pdf",dirName,(int)isLog));
	c_Pbp->SaveAs(Form("dir_1D_%s_noPtWeight/nSig_pt_isLog%d_Pbp.pdf",dirName,(int)isLog));
	c_pPb->SaveAs(Form("dir_1D_%s_noPtWeight/nSig_pt_isLog%d_pPb.pdf",dirName,(int)isLog));

	// root file
	//TFile *fOut = new TFile(Form("dir_1D_%s/nSig_pt_isLog%d.root",dirName,(int)isLog),"RECREATE");
	TFile *fOut = new TFile(Form("dir_1D_%s_noPtWeight/nSig_pt_isLog%d.root",dirName,(int)isLog),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_fit_PR_Pbp[iy]->Write();
		h1D_fit_PR_pPb[iy]->Write();
	}
	fOut->Close();
	return;

} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < y_{CM} < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < y_{CM} < %.2f", binmin, binmax);
	}
}

void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < |y_{CM}| < %.2f", binmin, binmax);
	}
}

void formPtArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
	}
}


