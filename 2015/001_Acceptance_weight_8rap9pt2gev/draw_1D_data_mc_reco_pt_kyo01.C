#include <Riostream.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
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

void draw_1D_data_mc_reco_pt_kyo01(char* dirName = "8rap9pt", bool isPrompt=true, bool isLog=false)
{
	gROOT->Macro("./JpsiStyle.C");

	const int ntmp = 9;
	Double_t px[8][ntmp]; //x point
	Double_t pxtmp[8][ntmp]; //x point
	Double_t pytmp[8][ntmp]; //y point
	Double_t ex[ntmp]; // x error
	
	px[0] = {1.80888, 3.48529, 4.47275, 5.68792, 6.9644, 7.96271, 9.1754, 11.5315, 17.7588}; 
	px[1] = {0.00000, 3.53123, 4.5027, 5.71709, 6.96523, 7.9693, 9.17314, 11.4952, 17.6927}; 
	px[2] = {0.00000, 0.00000, 0.00000, 0.00000, 7.01977, 7.99712, 9.19936, 11.5743, 17.7732}; 
	px[3] = {0.00000, 0.00000, 0.00000, 0.00000, 7.12292, 8.01305, 9.22816, 11.6279, 17.8879}; 
	px[4] = {0.00000, 0.00000, 0.00000, 0.00000, 7.05476, 8.00208, 9.21589, 11.5645, 17.7176}; 
	px[5] = {0.00000, 0.00000, 0.00000, 5.82095, 6.97886, 7.96732, 9.18979, 11.5158, 17.4116}; 
	px[6] = {0.00000, 3.52893, 4.48328, 5.69351, 6.96188, 7.95707, 9.14886, 11.4747, 17.231}; 
	px[7] = {1.78552, 3.47853, 4.46938, 5.6761, 6.96419, 7.97702, 9.16158, 11.5077, 17.3061}; 
	ex = {0,0,0,0,0,0,0,0,0};
	
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
	TFile * f2D = new TFile(Form("./total2Dhist_%s_kyo01.root",dirName));
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
	c_Pbp->SaveAs(Form("DataMcReco_%s_kyo01/DataMcRecoPt_Pbp_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
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
	c_pPb->SaveAs(Form("DataMcReco_%s_kyo01/DataMcRecoPt_pPb_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
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
	c_tot->SaveAs(Form("DataMcReco_%s_kyo01/DataMcRecoPt_tot_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
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
//	c_Pbp->SaveAs(Form("DataMcReco_%s/DataMcRecoPtRatio_Pbp_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
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
//	c_pPb->SaveAs(Form("DataMcReco_%s/DataMcRecoPtRatio_pPb_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
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
//	c_tot->SaveAs(Form("DataMcReco_%s/DataMcRecoPtRatio_tot_isPrompt%d_isLog%d.pdf",dirName,(int)isPrompt,(int)isLog));
	legBL->Clear();

	c_Pbp->Clear();
	c_pPb->Clear();
	c_tot->Clear();

	//////////////////////////////////////////////////////////////////
	// convert to TGraphAsymErrors only for ratio
	TGraphAsymmErrors* gRatio_Pbp[nbinsX];
	TGraphAsymmErrors* gRatio_pPb[nbinsX];
	TGraphAsymmErrors* gRatio_tot[nbinsX];
	
	TCanvas* c_Pbp2 = new TCanvas("c_Pbp2","c_Pbp2",200,10,1600,800);
	c_Pbp2->Divide(4,2);	
	TCanvas* c_pPb2 = new TCanvas("c_pPb2","c_pPb2",200,10,1600,800);
	c_pPb2->Divide(4,2);	
	TCanvas* c_tot2 = new TCanvas("c_tot2","c_tot2",200,10,1600,800);
	c_tot2->Divide(4,2);	
	
	
	// --- Pbp (1st run)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		gRatio_Pbp[iy]=new TGraphAsymmErrors(hRatio_Pbp[iy]);
		gRatio_Pbp[iy]->SetName(Form("gRatio_Pbp_%d",iy));
		for (Int_t ipt=0; ipt<nbinsY; ipt++ ){
			gRatio_Pbp[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);	
			gRatio_Pbp[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gRatio_Pbp[iy]->SetPointEXlow(ipt, ex[ipt]);
			gRatio_Pbp[iy]->SetPointEXhigh(ipt, ex[ipt]);
			cout << "pr : pytmp["<<iy<<"]["<<ipt<<"] = " << pytmp[iy][ipt]<<endl;
		}
		c_Pbp2->cd(iy+1);
		gRatio_Pbp[iy]->GetXaxis()->SetLimits(0.0,25.);
		gRatio_Pbp[iy]->SetMinimum(0.2);
		gRatio_Pbp[iy]->SetMaximum(1.8);
		gRatio_Pbp[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
		gRatio_Pbp[iy]->GetYaxis()->SetTitle("Ratio");
		gRatio_Pbp[iy]->Draw("AP");
		if (iy==0){
			if (isPrompt) latex->DrawLatex(0.23,0.23,"Prompt J/#psi Pbp");
			else latex->DrawLatex(0.23, 0.23,"Non-prompt J/#psi Pbp");
			//latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		}
		latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,25.,1.,1,1);
	}	
	c_Pbp2->SaveAs(Form("DataMcReco_%s_kyo01/DataMcRecoPt_gRatio_Pbp_isPrompt%d.pdf",dirName,(int)isPrompt));

	// --- pPb (2nd run)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		gRatio_pPb[iy]=new TGraphAsymmErrors(hRatio_pPb[iy]);
		gRatio_pPb[iy]->SetName(Form("gRatio_pPb_%d",iy));
		for (Int_t ipt=0; ipt<nbinsY; ipt++ ){
			gRatio_pPb[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);	
			gRatio_pPb[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gRatio_pPb[iy]->SetPointEXlow(ipt, ex[ipt]);
			gRatio_pPb[iy]->SetPointEXhigh(ipt, ex[ipt]);
			cout << "pr : pytmp["<<iy<<"]["<<ipt<<"] = " << pytmp[iy][ipt]<<endl;
		}
		c_pPb2->cd(iy+1);
		gRatio_pPb[iy]->GetXaxis()->SetLimits(0.0,25.);
		gRatio_pPb[iy]->SetMinimum(0.2);
		gRatio_pPb[iy]->SetMaximum(1.8);
		gRatio_pPb[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
		gRatio_pPb[iy]->GetYaxis()->SetTitle("Ratio");
		gRatio_pPb[iy]->Draw("AP");
		if (iy==0){
			if (isPrompt) latex->DrawLatex(0.23,0.23,"Prompt J/#psi pPb");
			else latex->DrawLatex(0.23, 0.23,"Non-prompt J/#psi pPb");
			//latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		}
		latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,25.,1.,1,1);
	}	
	c_pPb2->SaveAs(Form("DataMcReco_%s_kyo01/DataMcRecoPt_gRatio_pPb_isPrompt%d.pdf",dirName,(int)isPrompt));



	// --- tot (1st + 2nd)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		gRatio_tot[iy]=new TGraphAsymmErrors(hRatio_tot[iy]);
		gRatio_tot[iy]->SetName(Form("gRatio_tot_%d",iy));
		for (Int_t ipt=0; ipt<nbinsY; ipt++ ){
			gRatio_tot[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);	
			gRatio_tot[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gRatio_tot[iy]->SetPointEXlow(ipt, ex[ipt]);
			gRatio_tot[iy]->SetPointEXhigh(ipt, ex[ipt]);
			cout << "pr : pytmp["<<iy<<"]["<<ipt<<"] = " << pytmp[iy][ipt]<<endl;
		}
		c_tot2->cd(iy+1);
		gRatio_tot[iy]->GetXaxis()->SetLimits(0.0,25.);
		gRatio_tot[iy]->SetMinimum(0.2);
		gRatio_tot[iy]->SetMaximum(1.8);
		gRatio_tot[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
		gRatio_tot[iy]->GetYaxis()->SetTitle("Ratio");
		gRatio_tot[iy]->Draw("AP");
		if (iy==0){
			if (isPrompt) latex->DrawLatex(0.23,0.23,"Prompt J/#psi tot");
			else latex->DrawLatex(0.23, 0.23,"Non-prompt J/#psi tot");
			//latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		}
		latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,25.,1.,1,1);
	}	
	c_tot2->SaveAs(Form("DataMcReco_%s_kyo01/DataMcRecoPt_gRatio_tot_isPrompt%d.pdf",dirName,(int)isPrompt));

	//////////////////////////////////////////////////////////////////
	// root file
	TFile *fOut = new TFile(Form("DataMcReco_%s_kyo01/DataMcRecoPt_isPropmt%d.root",dirName,(int)isPrompt),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_data_Pbp[iy]->Write();
		h1D_MC_Pbp[iy]->Write();
		hRatio_Pbp[iy]->Write();
		gRatio_Pbp[iy]->Write();
		h1D_data_pPb[iy]->Write();
		h1D_MC_pPb[iy]->Write();
		hRatio_pPb[iy]->Write();
		gRatio_pPb[iy]->Write();
		h1D_data_tot[iy]->Write();
		h1D_MC_tot[iy]->Write();
		hRatio_tot[iy]->Write();
		gRatio_tot[iy]->Write();
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


