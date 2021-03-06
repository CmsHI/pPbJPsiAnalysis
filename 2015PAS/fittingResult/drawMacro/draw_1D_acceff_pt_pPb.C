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
void draw_1D_acceff_pt_pPb(char* dirName = "8rap9pt2gev", bool isLog=false)
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
	//Double_t ptArrNum[] = {0.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
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
	TFile * f2D = new TFile(Form("../total2Dhist_%s.root",dirName));
	cout << "dirName = " << dirName << endl;

	// --- read-in 2D hist for data reco dist
	TH2D* h2D_Acc_pt_y_PRMC_pPb;
	TH2D* h2D_Acc_pt_y_NPMC_pPb;
	h2D_Acc_pt_y_PRMC_pPb = (TH2D*)f2D->Get("h2D_Acc_pt_y_PRMC_pPb");
	h2D_Acc_pt_y_NPMC_pPb = (TH2D*)f2D->Get("h2D_Acc_pt_y_NPMC_pPb");
	h2D_Acc_pt_y_PRMC_pPb->SetName("h2D_Acc_pt_y_PRMC_pPb");
	h2D_Acc_pt_y_NPMC_pPb->SetName("h2D_Acc_pt_y_NPMC_pPb");
	cout << "h2D_Acc_pt_y_PRMC_pPb = " << h2D_Acc_pt_y_PRMC_pPb << endl;
	cout << "h2D_Acc_pt_y_NPMC_pPb = " << h2D_Acc_pt_y_NPMC_pPb << endl;
	TH2D* h2D_Eff_pt_y_PRMC_pPb;
	TH2D* h2D_Eff_pt_y_NPMC_pPb;
	h2D_Eff_pt_y_PRMC_pPb = (TH2D*)f2D->Get("h2D_Eff_pt_y_PRMC_pPb");
	h2D_Eff_pt_y_NPMC_pPb = (TH2D*)f2D->Get("h2D_Eff_pt_y_NPMC_pPb");
	h2D_Eff_pt_y_PRMC_pPb->SetName("h2D_Eff_pt_y_PRMC_pPb");
	h2D_Eff_pt_y_NPMC_pPb->SetName("h2D_Eff_pt_y_NPMC_pPb");

	// Acc*Eff
	h2D_Acc_pt_y_PRMC_pPb->Multiply(h2D_Eff_pt_y_PRMC_pPb);	
	h2D_Acc_pt_y_NPMC_pPb->Multiply(h2D_Eff_pt_y_NPMC_pPb);	

	const int nbinsX = h2D_Acc_pt_y_PRMC_pPb->GetNbinsX();
	const int nbinsY = h2D_Acc_pt_y_PRMC_pPb->GetNbinsY();
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
	// ---  projection to 1D hist
	TH1D* h1D_acceff_PRMC[nbinsX]; 
	TH1D* h1D_acceff_NPMC[nbinsX]; 
	
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		////// for the 1st run
//		h1D_acceff_PRMC[iy] = h2D_Acc_pt_y_PRMC_pPb->ProjectionY(Form("h1D_acceff_PRMC_%d",iy),iy+1,iy+1);
//		h1D_acceff_NPMC[iy] = h2D_Acc_pt_y_NPMC_pPb->ProjectionY(Form("h1D_acceff_NPMC_%d",iy),iy+1,iy+1);
		////// for the 2nd run
		h1D_acceff_PRMC[iy] = h2D_Acc_pt_y_PRMC_pPb->ProjectionY(Form("h1D_acceff_PRMC_%d",iy),nbinsX-iy,nbinsX-iy);
		h1D_acceff_NPMC[iy] = h2D_Acc_pt_y_NPMC_pPb->ProjectionY(Form("h1D_acceff_NPMC_%d",iy),nbinsX-iy,nbinsX-iy);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
	// set values as zero for unused bins
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_acceff_PRMC[iy]->SetBinContent(1,0);
			h1D_acceff_PRMC[iy]->SetBinError(1,0);
			h1D_acceff_NPMC[iy]->SetBinContent(1,0);
			h1D_acceff_NPMC[iy]->SetBinError(1,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_acceff_PRMC[iy]->SetBinContent(2,0);
			h1D_acceff_PRMC[iy]->SetBinContent(3,0);
			h1D_acceff_PRMC[iy]->SetBinError(2,0);
			h1D_acceff_PRMC[iy]->SetBinError(3,0);
			h1D_acceff_NPMC[iy]->SetBinContent(2,0);
			h1D_acceff_NPMC[iy]->SetBinContent(3,0);
			h1D_acceff_NPMC[iy]->SetBinError(2,0);
			h1D_acceff_NPMC[iy]->SetBinError(3,0);
		}
		if (iy>=2 && iy<=4) {
			h1D_acceff_PRMC[iy]->SetBinContent(4,0);
			h1D_acceff_PRMC[iy]->SetBinError(4,0);
			h1D_acceff_NPMC[iy]->SetBinContent(4,0);
			h1D_acceff_NPMC[iy]->SetBinError(4,0);
		}
	}
	
	////// after zero-bin setting, normalize!
//	for (Int_t iy = 0; iy < nbinsX; iy++) {
//		h1D_acceff_PRMC[iy]->Scale(1./h1D_acceff_PRMC[iy]->Integral());
//		h1D_acceff_NPMC[iy]->Scale(1./h1D_acceff_NPMC[iy]->Integral());
//		h1D_acceff_PRMC[iy]->Scale(1,"width");
//		h1D_acceff_NPMC[iy]->Scale(1,"width");
//	}
	
	//////////////////////////////////////////////////////////////////
	//// --- Draw histograms

	//TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
	TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92); //upper left
	TLegend *legUL = new TLegend(0.18, 0.75, 0.59, 0.92); //upper left
	TLegend *legBL = new TLegend(0.18, 0.18, 0.59, 0.35); //upper left
	TLegend *legBR = new TLegend(0.49, 0.28, 0.86, 0.40); //upper left
	SetLegendStyle(legUR);
	SetLegendStyle(legUL);
	SetLegendStyle(legBL);
	SetLegendStyle(legBR);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//TCanvas* c_multi = new TCanvas("c_multi","c_multi",200,10,600,600);
	//TCanvas* c_multi = new TCanvas("c_multi","c_multi",200,10,1500,600);
	//c_multi->Divide(5,2);
	TCanvas* c_multi = new TCanvas("c_multi","c_multi",200,10,1600,800);
	c_multi->Divide(4,2);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c_multi->cd(iy+1);
		if (isLog) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetHistStyle(h1D_acceff_PRMC[iy],1,0);
		SetHistStyle(h1D_acceff_NPMC[iy],2,10);
		h1D_acceff_PRMC[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_acceff_PRMC[iy]->GetXaxis()->CenterTitle();
		h1D_acceff_PRMC[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_acceff_PRMC[iy]->GetYaxis()->SetTitle("Acceptance*Efficiency");
		//h1D_acceff_PRMC[iy]->GetYaxis()->SetRangeUser(0,0.6);
		h1D_acceff_PRMC[iy]->GetYaxis()->SetRangeUser(0,0.7);
		h1D_acceff_PRMC[iy]->Draw("pe");
		h1D_acceff_NPMC[iy]->Draw("pe same");
		if (iy==0) {
//			if (isPrompt) legBR -> SetHeader("Prompt J/#psi ");
//			else legBR -> SetHeader("Non-prompt J/#psi ");
//			legBR -> AddEntry(h1D_acceff_PRMC[iy],"1st run (pPb)","lp");
//			legBR -> AddEntry(h1D_acceff_NPMC[iy],"2nd run (pPb)","lp");
			legUL -> SetHeader("2nd run");
			legUL -> AddEntry(h1D_acceff_PRMC[iy],"Prompt J/#psi","lp");
			legUL -> AddEntry(h1D_acceff_NPMC[iy],"Non-prompt J/#psi","lp");
			legUL->Draw();
		}
		//latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
		//latex->DrawLatex(0.46,0.85,Form("%s",rapArr[iy].c_str()));
		//latex->DrawLatex(0.46,0.20,Form("%s",rapArr[iy].c_str()));
		latex->DrawLatex(0.49,0.20,Form("%s",rapArr[iy].c_str()));
	}
//	c_multi->cd(10);//tmp
//	legBR->Draw();
	c_multi->SaveAs(Form("acc_%s/acceff_pt_pPb_isLog%d.pdf",dirName,(int)isLog));







/*
	// root file
	TFile *fOut = new TFile(Form("1Dhist_%s/crossSection_pt_%s.root",dirName,runstring.c_str()),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_fit_pt_y_NP_pPb[iy]->Write();
		h1D_fit_pt_y_NP_pPb[iy]->Write();
		h1D_fit_pt_y_NP_tot[iy]->Write();
	}
	//tRap->Write();
	fOut->Close();
*/
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


