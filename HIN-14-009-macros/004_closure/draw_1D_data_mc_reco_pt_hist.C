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

void draw_1D_data_mc_reco_pt_hist(char* dirName = "8rap9pt", bool isPrompt=true, bool isLog=false)
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
		//rapBinW[iy] = rapArrNumBF[iy+1]-rapArrNumBF[iy]; 
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
	
	// --- read-in file
	TFile * f2D = new TFile(Form("../000_fittingResult/total2Dhist_%s.root",dirName));
	cout << "dirName = " << dirName << endl;

	// --- read-in 2D hist for data reco dist
	TH2D* h2D_fit_pt_y_Pbp;
	TH2D* h2D_fit_pt_y_pPb;
	if (isPrompt) {
		h2D_fit_pt_y_Pbp = (TH2D*)f2D->Get("h2D_fit_pt_y_PR_Pbp");
		h2D_fit_pt_y_pPb = (TH2D*)f2D->Get("h2D_fit_pt_y_PR_pPb");
	}
	else {
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
	if (isPrompt) {
		h2D_Eff_Num_pt_y_Pbp = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_pt_y_PRMC_Pbp");
		h2D_Eff_Num_pt_y_pPb = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_pt_y_PRMC_pPb");
	}
	else {
		h2D_Eff_Num_pt_y_Pbp = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_pt_y_NPMC_Pbp");
		h2D_Eff_Num_pt_y_pPb = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_pt_y_NPMC_pPb");
	}
	h2D_Eff_Num_pt_y_Pbp->SetName("h2D_Eff_Num_pt_y_Pbp");
	h2D_Eff_Num_pt_y_pPb->SetName("h2D_Eff_Num_pt_y_pPb");
	cout << "h2D_Eff_Num_pt_y_Pbp = " << h2D_Eff_Num_pt_y_Pbp << endl;
	cout << "h2D_Eff_Num_pt_y_pPb = " << h2D_Eff_Num_pt_y_pPb << endl;

	// ---  projection to 1D hist
	TH1D* h1D_data_Pbp[nbinsX]; 
	TH1D* h1D_data_pPb[nbinsX]; 
	TH1D* h1D_MC_Pbp[nbinsX]; 
	TH1D* h1D_MC_pPb[nbinsX]; 
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		////// for data RECO
		h1D_data_Pbp[iy] = h2D_fit_pt_y_Pbp->ProjectionY(Form("h1D_data_Pbp_%d",iy),iy+1,iy+1);
		h1D_data_pPb[iy] = h2D_fit_pt_y_pPb->ProjectionY(Form("h1D_data_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
		////// for MC RECO
		h1D_MC_Pbp[iy] = h2D_Eff_Num_pt_y_Pbp->ProjectionY(Form("h1D_MC_Pbp_%d",iy),iy+1,iy+1);
		h1D_MC_pPb[iy] = h2D_Eff_Num_pt_y_pPb->ProjectionY(Form("h1D_MC_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
	}
	
	// --- fot Pbp+pPb merged dist.
	TH1D* h1D_data_tot[nbinsX]; 
	TH1D* h1D_MC_tot[nbinsX];
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_data_tot[iy]=(TH1D*)h1D_data_Pbp[iy]->Clone(Form("h1D_data_tot_%d",iy));
		h1D_data_tot[iy]->Add(h1D_data_pPb[iy]);
		h1D_MC_tot[iy]=(TH1D*)h1D_MC_Pbp[iy]->Clone(Form("h1D_MC_tot_%d",iy));
		h1D_MC_tot[iy]->Add(h1D_MC_pPb[iy]);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
	// set values as zero for unused bins
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_data_Pbp[iy]->SetBinContent(1,0);
			h1D_data_Pbp[iy]->SetBinError(1,0);
			h1D_data_pPb[iy]->SetBinContent(1,0);
			h1D_data_pPb[iy]->SetBinError(1,0);
			h1D_data_tot[iy]->SetBinContent(1,0);
			h1D_data_tot[iy]->SetBinError(1,0);
			h1D_MC_Pbp[iy]->SetBinContent(1,0);
			h1D_MC_Pbp[iy]->SetBinError(1,0);
			h1D_MC_pPb[iy]->SetBinContent(1,0);
			h1D_MC_pPb[iy]->SetBinError(1,0);
			h1D_MC_tot[iy]->SetBinContent(1,0);
			h1D_MC_tot[iy]->SetBinError(1,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_data_Pbp[iy]->SetBinContent(2,0);
			h1D_data_Pbp[iy]->SetBinError(2,0);
			h1D_data_Pbp[iy]->SetBinContent(3,0);
			h1D_data_Pbp[iy]->SetBinError(3,0);
			h1D_data_pPb[iy]->SetBinContent(2,0);
			h1D_data_pPb[iy]->SetBinError(2,0);
			h1D_data_pPb[iy]->SetBinContent(3,0);
			h1D_data_pPb[iy]->SetBinError(3,0);
			h1D_data_tot[iy]->SetBinContent(2,0);
			h1D_data_tot[iy]->SetBinError(2,0);
			h1D_data_tot[iy]->SetBinContent(3,0);
			h1D_data_tot[iy]->SetBinError(3,0);
			h1D_MC_Pbp[iy]->SetBinContent(2,0);
			h1D_MC_Pbp[iy]->SetBinError(2,0);
			h1D_MC_Pbp[iy]->SetBinContent(3,0);
			h1D_MC_Pbp[iy]->SetBinError(3,0);
			h1D_MC_pPb[iy]->SetBinContent(2,0);
			h1D_MC_pPb[iy]->SetBinError(2,0);
			h1D_MC_pPb[iy]->SetBinContent(3,0);
			h1D_MC_pPb[iy]->SetBinError(3,0);
			h1D_MC_tot[iy]->SetBinContent(2,0);
			h1D_MC_tot[iy]->SetBinError(2,0);
			h1D_MC_tot[iy]->SetBinContent(3,0);
			h1D_MC_tot[iy]->SetBinError(3,0);
		}
		if (iy>=2 && iy<=4) {
			h1D_data_Pbp[iy]->SetBinContent(4,0);
			h1D_data_Pbp[iy]->SetBinError(4,0);
			h1D_data_pPb[iy]->SetBinContent(4,0);
			h1D_data_pPb[iy]->SetBinError(4,0);
			h1D_data_tot[iy]->SetBinContent(4,0);
			h1D_data_tot[iy]->SetBinError(4,0);
			h1D_MC_Pbp[iy]->SetBinContent(4,0);
			h1D_MC_Pbp[iy]->SetBinError(4,0);
			h1D_MC_pPb[iy]->SetBinContent(4,0);
			h1D_MC_pPb[iy]->SetBinError(4,0);
			h1D_MC_tot[iy]->SetBinContent(4,0);
			h1D_MC_tot[iy]->SetBinError(4,0);
		}
	}
	
	////// after zero-bin setting, normalize!
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_data_Pbp[iy]->Scale(1./h1D_data_Pbp[iy]->Integral());
		h1D_data_pPb[iy]->Scale(1./h1D_data_pPb[iy]->Integral());
		h1D_data_tot[iy]->Scale(1./h1D_data_tot[iy]->Integral());
		h1D_MC_Pbp[iy]->Scale(1./h1D_MC_Pbp[iy]->Integral());
		h1D_MC_pPb[iy]->Scale(1./h1D_MC_pPb[iy]->Integral());
		h1D_MC_tot[iy]->Scale(1./h1D_MC_tot[iy]->Integral());
		h1D_data_Pbp[iy]->Scale(1,"width");
		h1D_data_pPb[iy]->Scale(1,"width");
		h1D_data_tot[iy]->Scale(1,"width");
		h1D_MC_Pbp[iy]->Scale(1,"width");
		h1D_MC_pPb[iy]->Scale(1,"width");
		h1D_MC_tot[iy]->Scale(1,"width");
	}
	//////////////////////////////////////////////////////////////////
	//// --- Draw histograms

	double maxVal=0.;
	
	TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92); //upper left
	TLegend *legBL = new TLegend(0.18, 0.18, 0.59, 0.35); //upper left
	SetLegendStyle(legUR);
	SetLegendStyle(legBL);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);
	
	// --- Pbp (1st run)
	TCanvas* c_Pbp = new TCanvas("c_Pbp","c_Pbp",200,10,1600,800);
	c_Pbp->Divide(4,2);
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c_Pbp->cd(iy+1);
		SetHistStyle(h1D_data_Pbp[iy],1,0);
		SetHistStyle(h1D_MC_Pbp[iy],2,10);
		h1D_data_Pbp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_data_Pbp[iy]->GetXaxis()->CenterTitle();
		h1D_data_Pbp[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_data_Pbp[iy]->GetYaxis()->SetTitle("");
		if (!isLog){
			maxVal = h1D_data_Pbp[iy]->GetMaximum();
			h1D_data_Pbp[iy]->GetYaxis()->SetRangeUser(0.,maxVal*1.3);
		}
		h1D_data_Pbp[iy]->Draw("pe");
		h1D_MC_Pbp[iy]->Draw("pe same");
		if (iy==0){
			if (isPrompt) legUR -> SetHeader("Prompt J/#psi 1st run");
			else legUR -> SetHeader("Non-prompt J/#psi 1st run");
			legUR -> AddEntry(h1D_data_Pbp[iy],"data RECO","lp");
			legUR -> AddEntry(h1D_MC_Pbp[iy],"MC RECO","lp");
			legUR->Draw();
			latex->DrawLatex(0.56,0.68,Form("%s",rapArr[iy].c_str()));
		}
		else latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
	}
	c_Pbp->SaveAs(Form("DataMcReco_%s/DataMcRecoPt_Pbp_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
	legUR->Clear();
	
	// --- pPb (2nd run)
	TCanvas* c_pPb = new TCanvas("c_pPb","c_pPb",200,10,1600,800);
	c_pPb->Divide(4,2);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c_pPb->cd(iy+1);
		if (isLog) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetHistStyle(h1D_data_pPb[iy],1,0);
		SetHistStyle(h1D_MC_pPb[iy],2,10);
		h1D_data_pPb[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_data_pPb[iy]->GetXaxis()->CenterTitle();
		h1D_data_pPb[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_data_pPb[iy]->GetYaxis()->SetTitle("");
		if (!isLog){
			maxVal = h1D_data_pPb[iy]->GetMaximum();
			h1D_data_pPb[iy]->GetYaxis()->SetRangeUser(0.,maxVal*1.3);
		}
		h1D_data_pPb[iy]->Draw("pe");
		h1D_MC_pPb[iy]->Draw("pe same");
		if (iy==0){
			if (isPrompt) legUR -> SetHeader("Prompt J/#psi 2nd run");
			else legUR -> SetHeader("Non-prompt J/#psi 2nd run");
			legUR -> AddEntry(h1D_data_pPb[iy],"data RECO","lp");
			legUR -> AddEntry(h1D_MC_pPb[iy],"MC RECO","lp");
			legUR->Draw();
			latex->DrawLatex(0.56,0.68,Form("%s",rapArr[iy].c_str()));
		}
		else latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
	}
	c_pPb->SaveAs(Form("DataMcReco_%s/DataMcRecoPt_pPb_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
	legUR->Clear();
	
	// --- tot (1st+2nd)
	TCanvas* c_tot = new TCanvas("c_tot","c_tot",200,10,1600,800);
	c_tot->Divide(4,2);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c_tot->cd(iy+1);
		if (isLog) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetHistStyle(h1D_data_tot[iy],1,0);
		SetHistStyle(h1D_MC_tot[iy],2,10);
		h1D_data_tot[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_data_tot[iy]->GetXaxis()->CenterTitle();
		h1D_data_tot[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_data_tot[iy]->GetYaxis()->SetTitle("");
		if (!isLog){
			maxVal = h1D_data_tot[iy]->GetMaximum();
			h1D_data_tot[iy]->GetYaxis()->SetRangeUser(0.,maxVal*1.3);
		}
		h1D_data_tot[iy]->Draw("pe");
		h1D_MC_tot[iy]->Draw("pe same");
		if (iy==0){
			if (isPrompt) legUR -> SetHeader("Prompt J/#psi total");
			else legUR -> SetHeader("Non-prompt J/#psi total");
			legUR -> AddEntry(h1D_data_tot[iy],"data RECO","lp");
			legUR -> AddEntry(h1D_MC_tot[iy],"MC RECO","lp");
			legUR->Draw();
			latex->DrawLatex(0.56,0.68,Form("%s",rapArr[iy].c_str()));
		}
		else latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
	}
	c_tot->SaveAs(Form("DataMcReco_%s/DataMcRecoPt_tot_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
	legUR->Clear();




	//////////////////////////////////////////////////////////////////
	// hRatio
	TH1D* hRatio_Pbp[nRap];
	TH1D* hRatio_pPb[nRap];
	TH1D* hRatio_tot[nRap];
	
	// --- Pbp (1st run)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c_Pbp->cd(iy+1);
		gPad->SetLogy(0);
		hRatio_Pbp[iy]=(TH1D*)h1D_data_Pbp[iy]->Clone(Form("hRatio_Pbp_%d",iy));
		hRatio_Pbp[iy]->Divide(h1D_MC_Pbp[iy]);
		SetHistStyle(hRatio_Pbp[iy],5,0);
		hRatio_Pbp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		hRatio_Pbp[iy]->GetYaxis()->SetTitle("Ratio");
		hRatio_Pbp[iy]->GetXaxis()->SetRangeUser(0.,30.);
		//hRatio_Pbp[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
		hRatio_Pbp[iy]->GetYaxis()->SetRangeUser(0.0,2.0);
		hRatio_Pbp[iy]->Draw("pe");
		if (iy==0) {
			if (isPrompt) legBL -> SetHeader("Prompt J/#psi 1st run");
			else legBL -> SetHeader("Non-prompt J/#psi 1st run");
			legBL -> AddEntry(hRatio_Pbp[iy],"DATA/MC","lp");
			legBL->Draw();
		}
		else latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,30.,1.,1,1);
	}
	c_Pbp->SaveAs(Form("DataMcReco_%s/DataMcRecoPtRatio_Pbp_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
	legBL->Clear();

	// --- pPb (2nd run)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c_pPb->cd(iy+1);
		gPad->SetLogy(0);
		hRatio_pPb[iy]=(TH1D*)h1D_data_pPb[iy]->Clone(Form("hRatio_pPb_%d",iy));
		hRatio_pPb[iy]->Divide(h1D_MC_pPb[iy]);
		SetHistStyle(hRatio_pPb[iy],5,0);
		hRatio_pPb[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		hRatio_pPb[iy]->GetYaxis()->SetTitle("Ratio");
		hRatio_pPb[iy]->GetXaxis()->SetRangeUser(0.,30.);
		//hRatio_pPb[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
		hRatio_pPb[iy]->GetYaxis()->SetRangeUser(0.0,2.0);
		hRatio_pPb[iy]->Draw("pe");
		if (iy==0){
			if (isPrompt) legBL -> SetHeader("Prompt J/#psi 2nd run");
			else legBL -> SetHeader("Non-prompt J/#psi 2nd run");
			legBL -> AddEntry(hRatio_pPb[iy],"data/MC","lp");
			legBL->Draw();
		}
		else latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,30.,1.,1,1);
	}
	c_pPb->SaveAs(Form("DataMcReco_%s/DataMcRecoPtRatio_pPb_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
	legBL->Clear();
	
	// --- tot (1st + 2nd)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		c_tot->cd(iy+1);
		gPad->SetLogy(0);
		hRatio_tot[iy]=(TH1D*)h1D_data_tot[iy]->Clone(Form("hRatio_tot_%d",iy));
		hRatio_tot[iy]->Divide(h1D_MC_tot[iy]);
		SetHistStyle(hRatio_tot[iy],5,0);
		hRatio_tot[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		hRatio_tot[iy]->GetYaxis()->SetTitle("Ratio");
		hRatio_tot[iy]->GetXaxis()->SetRangeUser(0.,30.);
		//hRatio_tot[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
		hRatio_tot[iy]->GetYaxis()->SetRangeUser(0.0,2.0);
		hRatio_tot[iy]->Draw("pe");
		if (iy==0){
			if (isPrompt) legBL -> SetHeader("Prompt J/#psi total");
			else legBL -> SetHeader("Non-prompt J/#psi total");
			legBL -> AddEntry(hRatio_tot[iy],"data/MC","lp");
			legBL->Draw();
		}
		else latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,30.,1.,1,1);
	}
	c_tot->SaveAs(Form("DataMcReco_%s/DataMcRecoPtRatio_tot_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
	legBL->Clear();

	//////////////////////////////////////////////////////////////////
	// root file
	TFile *fOut = new TFile(Form("DataMcReco_%s/DataMcRecoPt_isPropmt%d.root",dirName,(int)isPrompt),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_data_Pbp[iy]->Write();
		h1D_MC_Pbp[iy]->Write();
		hRatio_Pbp[iy]->Write();
		h1D_data_pPb[iy]->Write();
		h1D_MC_pPb[iy]->Write();
		hRatio_pPb[iy]->Write();
		h1D_data_tot[iy]->Write();
		h1D_MC_tot[iy]->Write();
		hRatio_tot[iy]->Write();
	}
	fOut->Close();
	cout << "DataMcRecoPt_isPropmt" <<(int)isPrompt<<".root has been created :)" <<endl; 
	
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


