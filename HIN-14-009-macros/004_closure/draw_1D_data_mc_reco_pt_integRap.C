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

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

void draw_1D_data_mc_reco_pt_integRap(char* dirName = "8rap9pt", bool isPrompt = true)
{
	gROOT->Macro("./JpsiStyle.C");
	
	///////////////////////////////////////////////////////////////////	
	// --- bin information
	
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
	Double_t ptArrNum[] = {0.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
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

	///////////////////////////////////////////////////////////////////	
	// --- read-in file
	TFile * f2D = new TFile(Form("../000_fittingResult/total2Dhist_%s.root",dirName));
	//TFile * f2D = new TFile(Form("/home/songkyo/kyo/2015/000_fittingResult/total2Dhist_%s.root",dirName));
	cout << "dirName = " << dirName << endl;

	// --- read-in 2D hist for data reco dist
	TH2D* h2D_fit_pt_y_Pbp;
	TH2D* h2D_fit_pt_y_pPb;
	if (isPrompt){
		h2D_fit_pt_y_Pbp = (TH2D*)f2D->Get("h2D_fit_pt_y_PR_Pbp");
		h2D_fit_pt_y_pPb = (TH2D*)f2D->Get("h2D_fit_pt_y_PR_pPb");
	}else{
		h2D_fit_pt_y_Pbp = (TH2D*)f2D->Get("h2D_fit_pt_y_NP_Pbp");
		h2D_fit_pt_y_pPb = (TH2D*)f2D->Get("h2D_fit_pt_y_NP_pPb");
	}	
	h2D_fit_pt_y_Pbp->SetName("h2D_fit_pt_y_Pbp");
	h2D_fit_pt_y_pPb->SetName("h2D_fit_pt_y_pPb");
	cout << "h2D_fit_pt_y_Pbp = " << h2D_fit_pt_y_Pbp << endl;
	cout << "h2D_fit_pt_y_pPb = " << h2D_fit_pt_y_pPb << endl;

	const int nbinsX = h2D_fit_pt_y_Pbp->GetNbinsX();
	const int nbinsY = h2D_fit_pt_y_Pbp->GetNbinsY();
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
	// --- read-in 2D hist for MC reco dist.
	TH2D* h2D_Eff_Num_pt_y_Pbp;
	TH2D* h2D_Eff_Num_pt_y_pPb;
	if (isPrompt){
		h2D_Eff_Num_pt_y_Pbp = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_pt_y_PRMC_Pbp");
		h2D_Eff_Num_pt_y_pPb = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_pt_y_PRMC_pPb");
	}
	else{
		h2D_Eff_Num_pt_y_Pbp = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_pt_y_NPMC_Pbp");
		h2D_Eff_Num_pt_y_pPb = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_pt_y_NPMC_pPb");
	}
	h2D_Eff_Num_pt_y_Pbp->SetName("h2D_Eff_Num_pt_y_Pbp");
	h2D_Eff_Num_pt_y_pPb->SetName("h2D_Eff_Num_pt_y_pPb");
	cout << "h2D_Eff_Num_pt_y_Pbp = " << h2D_Eff_Num_pt_y_Pbp << endl;
	cout << "h2D_Eff_Num_pt_y_pPb = " << h2D_Eff_Num_pt_y_pPb << endl;

	// ---  projection to 1D hist
	TH1D* h1D_fit_pt_y_Pbp[nbinsX]; 
	TH1D* h1D_fit_pt_y_pPb[nbinsX]; 
	TH1D* h1D_Eff_Num_Pbp[nbinsX]; 
	TH1D* h1D_Eff_Num_pPb[nbinsX]; 
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		////// for fit_pt_y
		h1D_fit_pt_y_Pbp[iy] = h2D_fit_pt_y_Pbp->ProjectionY(Form("h1D_fit_pt_y_Pbp_%d",iy),iy+1,iy+1);
		h1D_fit_pt_y_pPb[iy] = h2D_fit_pt_y_pPb->ProjectionY(Form("h1D_fit_pt_y_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
		////// for GEN dist.
		h1D_Eff_Num_Pbp[iy] = h2D_Eff_Num_pt_y_Pbp->ProjectionY(Form("h1D_Eff_Num_Pbp_%d",iy),iy+1,iy+1);
		h1D_Eff_Num_pPb[iy] = h2D_Eff_Num_pt_y_pPb->ProjectionY(Form("h1D_Eff_Num_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
	// set values as zero for unused bins
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_fit_pt_y_Pbp[iy]->SetBinContent(1,0);
			h1D_fit_pt_y_Pbp[iy]->SetBinError(1,0);
			h1D_fit_pt_y_pPb[iy]->SetBinContent(1,0);
			h1D_fit_pt_y_pPb[iy]->SetBinError(1,0);
			h1D_Eff_Num_Pbp[iy]->SetBinContent(1,0);
			h1D_Eff_Num_Pbp[iy]->SetBinError(1,0);
			h1D_Eff_Num_pPb[iy]->SetBinContent(1,0);
			h1D_Eff_Num_pPb[iy]->SetBinError(1,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_fit_pt_y_Pbp[iy]->SetBinContent(2,0);
			h1D_fit_pt_y_Pbp[iy]->SetBinContent(3,0);
			h1D_fit_pt_y_Pbp[iy]->SetBinError(2,0);
			h1D_fit_pt_y_Pbp[iy]->SetBinError(3,0);
			h1D_fit_pt_y_pPb[iy]->SetBinContent(2,0);
			h1D_fit_pt_y_pPb[iy]->SetBinContent(3,0);
			h1D_fit_pt_y_pPb[iy]->SetBinError(2,0);
			h1D_fit_pt_y_pPb[iy]->SetBinError(3,0);
			h1D_Eff_Num_Pbp[iy]->SetBinContent(2,0);
			h1D_Eff_Num_Pbp[iy]->SetBinContent(3,0);
			h1D_Eff_Num_Pbp[iy]->SetBinError(2,0);
			h1D_Eff_Num_Pbp[iy]->SetBinError(3,0);
			h1D_Eff_Num_pPb[iy]->SetBinContent(2,0);
			h1D_Eff_Num_pPb[iy]->SetBinContent(3,0);
			h1D_Eff_Num_pPb[iy]->SetBinError(2,0);
			h1D_Eff_Num_pPb[iy]->SetBinError(3,0);
		}
		if (iy>=2 && iy<=4) {
			h1D_fit_pt_y_Pbp[iy]->SetBinContent(4,0);
			h1D_fit_pt_y_Pbp[iy]->SetBinError(4,0);
			h1D_fit_pt_y_pPb[iy]->SetBinContent(4,0);
			h1D_fit_pt_y_pPb[iy]->SetBinError(4,0);
			h1D_Eff_Num_Pbp[iy]->SetBinContent(4,0);
			h1D_Eff_Num_Pbp[iy]->SetBinError(4,0);
			h1D_Eff_Num_pPb[iy]->SetBinContent(4,0);
			h1D_Eff_Num_pPb[iy]->SetBinError(4,0);
		}
	}

	////////////////////////////////////////////////
	//merging  all rapidity for pT dist
	TH1D* hDataReco_Pbp;	
	TH1D* hDataReco_pPb;	
	TH1D* hMCReco_Pbp;	
	TH1D* hMCReco_pPb;	
	hDataReco_Pbp=(TH1D*)h1D_fit_pt_y_Pbp[0]->Clone("hDataReco_Pbp");
	hDataReco_pPb=(TH1D*)h1D_fit_pt_y_pPb[0]->Clone("hDataReco_pPb");
	hMCReco_Pbp=(TH1D*)h1D_Eff_Num_Pbp[0]->Clone("hMCReco_Pbp");
	hMCReco_pPb=(TH1D*)h1D_Eff_Num_pPb[0]->Clone("hMCReco_pPb");
	for (Int_t iy = 1; iy < nbinsX; iy++) {
		hDataReco_Pbp->Add(h1D_fit_pt_y_Pbp[iy]);	
		hDataReco_pPb->Add(h1D_fit_pt_y_pPb[iy]);	
		hMCReco_Pbp->Add(h1D_fit_pt_y_Pbp[iy]);	
		hMCReco_pPb->Add(h1D_fit_pt_y_pPb[iy]);	
	}
	// normalize
	hDataReco_Pbp->Scale(1./hDataReco_Pbp->Integral());
	hDataReco_pPb->Scale(1./hDataReco_pPb->Integral());
	hMCReco_Pbp->Scale(1./hMCReco_Pbp->Integral());
	hMCReco_pPb->Scale(1./hMCReco_pPb->Integral());
	hDataReco_Pbp->Scale(1.,"width");
	hDataReco_pPb->Scale(1.,"width");
	hMCReco_Pbp->Scale(1.,"width");
	hMCReco_pPb->Scale(1.,"width");

	//////////////////////////////////////////////////////////////////
	//// --- Draw histograms

	//TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
	TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92); //upper left
	TLegend *legBL = new TLegend(0.18, 0.18, 0.59, 0.35); //upper left
	SetLegendStyle(legUR);
	SetLegendStyle(legBL);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);
	
	// --- 1st run (Pbp)
	TCanvas* c_tot_Pbp = new TCanvas("c_tot_Pbp","c_tot_Pbp",200,10,600,600);
	c_tot_Pbp->cd();	
	gPad->SetLogy(0);
	SetHistStyle(hDataReco_Pbp,1,0);
	SetHistStyle(hMCReco_Pbp,2,0);
	hDataReco_Pbp->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	hDataReco_Pbp->GetXaxis()->CenterTitle();
	hDataReco_Pbp->GetXaxis()->SetRangeUser(0.,30.);
	hDataReco_Pbp->GetYaxis()->SetTitle("");
	hDataReco_Pbp->GetYaxis()->SetRangeUser(0.,0.15);
	hDataReco_Pbp->Draw("pe");
	hMCReco_Pbp->Draw("pe same");
	if (isPrompt) legUR -> SetHeader("Prompt J/#psi 1st run");
	else legUR -> SetHeader("Non-prompt J/#psi 1st run");
	legUR -> AddEntry(hDataReco_Pbp,"data RECO","lp");
	legUR -> AddEntry(hMCReco_Pbp,"MC RECO","lp");
	legUR->Draw();
	c_tot_Pbp->SaveAs(Form("DataMcRecoIntegRap_%s/data_mc_reco_Pbp_isPrompt%d.pdf",dirName,(int)isPrompt));
	c_tot_Pbp->Clear();
	legUR->Clear();
	
	
	// --- 2nd run (pPb)
	TCanvas* c_tot_pPb = new TCanvas("c_tot_pPb","c_tot_pPb",200,10,600,600);
	c_tot_pPb->cd();	
	gPad->SetLogy(0);
	SetHistStyle(hDataReco_pPb,1,0);
	SetHistStyle(hMCReco_pPb,2,0);
	hDataReco_pPb->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	hDataReco_pPb->GetXaxis()->CenterTitle();
	hDataReco_pPb->GetXaxis()->SetRangeUser(0.,30.);
	hDataReco_pPb->GetYaxis()->SetTitle("");
	hDataReco_pPb->GetYaxis()->SetRangeUser(0.,0.15);
	hDataReco_pPb->Draw("pe");
	hMCReco_pPb->Draw("pe same");
	if (isPrompt) legUR -> SetHeader("Prompt J/#psi 2nd run");
	else legUR -> SetHeader("Non-prompt J/#psi 2nd run");
	legUR -> AddEntry(hDataReco_pPb,"data RECO","lp");
	legUR -> AddEntry(hMCReco_pPb,"MC RECO","lp");
	legUR->Draw();
	c_tot_pPb->SaveAs(Form("DataMcRecoIntegRap_%s/data_mc_reco_pPb_isPrompt%d.pdf",dirName,(int)isPrompt));
	c_tot_pPb->Clear();
	legUR->Clear();
	
	//////////////////////////////////////////////////////////////////
	// hRatio
	TH1D* hRatio_Pbp;
	TH1D* hRatio_pPb;
	
	c_tot_Pbp->cd();	
	hRatio_Pbp=(TH1D*)hDataReco_Pbp->Clone("hRatio_Pbp");
	hRatio_Pbp->Divide(hMCReco_Pbp);
	SetHistStyle(hRatio_Pbp,5,0);
	hRatio_Pbp->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	hRatio_Pbp->GetYaxis()->SetTitle("Ratio");
	hRatio_Pbp->GetXaxis()->SetRangeUser(0.,30.);
	hRatio_Pbp->GetYaxis()->SetRangeUser(0.5,1.5);
	hRatio_Pbp->Draw("pe");
	if (isPrompt) legBL -> SetHeader("Prompt J/#psi 1st run");
	else legBL -> SetHeader("Non-prompt J/#psi 1st run");
	legBL -> AddEntry(hRatio_Pbp,"data/MC","lp");
	legBL->Draw();
	dashedLine(0.,1.,30.,1.,1,1);
	c_tot_Pbp->SaveAs(Form("DataMcRecoIntegRap_%s/data_mc_reco_ratio_Pbp_isPrompt%d.pdf",dirName,(int)isPrompt));
	c_tot_Pbp->Clear();
	legBL->Clear();

	c_tot_pPb->cd();	
	hRatio_pPb=(TH1D*)hDataReco_pPb->Clone("hRatio_pPb");
	hRatio_pPb->Divide(hMCReco_pPb);
	SetHistStyle(hRatio_pPb,5,0);
	hRatio_pPb->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	hRatio_pPb->GetYaxis()->SetTitle("Ratio");
	hRatio_pPb->GetXaxis()->SetRangeUser(0.,30.);
	hRatio_pPb->GetYaxis()->SetRangeUser(0.5,1.5);
	hRatio_pPb->Draw("pe");
	if (isPrompt) legBL -> SetHeader("Prompt J/#psi 1st run");
	else legBL -> SetHeader("Non-prompt J/#psi 1st run");
	legBL -> AddEntry(hRatio_pPb,"data/MC","lp");
	legBL->Draw();
	dashedLine(0.,1.,30.,1.,1,1);
	c_tot_pPb->SaveAs(Form("DataMcRecoIntegRap_%s/data_mc_reco_ratio_pPb_isPrompt%d.pdf",dirName,(int)isPrompt));
	c_tot_pPb->Clear();
	legBL->Clear();

	// root file
	TFile *fOut = new TFile(Form("DataMcRecoIntegRap_%s/data_mc_reco_isPrompt%d.root",dirName,(int)isPrompt),"RECREATE");
	fOut->cd();
	hDataReco_Pbp->Write();
	hMCReco_Pbp->Write();
	hRatio_Pbp->Write();
	hDataReco_pPb->Write();
	hMCReco_pPb->Write();
	hRatio_pPb->Write();
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


