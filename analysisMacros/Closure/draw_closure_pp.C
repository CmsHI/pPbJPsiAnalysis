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

//// runCode // 0=merged, 1=1stRun, 2=2ndRun
void draw_1D_mc_closure_pt(char* dirName = "8rap9pt", int runCode=0, bool isPrompt=true, bool isLog=false)
{
	gROOT->Macro("./JpsiStyle.C");
/*
	// set info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	Double_t lumi_nb;
	Double_t lumi_nb_err;
	Double_t lumi_mub;
	Double_t lumi_mub_err;
	string runstring;
	string lumistring;
	string cmsstring = "CMS preliminary";
	string beamstring = "pPb #sqrt{s_{NN}} = 5.02 TeV";
	if (runCode ==0) { runstring = "All"; lumi_nb =34.622; lumi_nb_err=1.2; }
	else if (runCode == 1) { runstring = "Pbp"; lumi_nb =20.7; lumi_nb_err=0.7; }//1stRun
	else if (runCode == 2) { runstring = "pPb"; lumi_nb = 14.0; lumi_nb_err=0.5; }//2ndRun
	else { cout << " *** Error!!! choose runCode 0, 1, or 2 " << endl; return ; }
	lumistring = Form("L_{int} = %.1f nb^{  -1}", lumi_nb);
	lumi_mub = lumi_nb * 1000; // (nb)^{-1} -> {#mub}^{-1}
	lumi_mub_err = lumi_nb_err * 1000; // (nb)^{-1} -> {#mub}^{-1}
*/	
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
	
	// --- read-in file
	TFile * f2D = new TFile(Form("../000_fittingResult/total2Dhist_%s.root",dirName));
	//TFile * f2D = new TFile(Form("/home/songkyo/kyo/2015/000_fittingResult/total2Dhist_%s.root",dirName));
	cout << "dirName = " << dirName << endl;

	// --- read-in 2D hist for MC corrected yield
	TH2D* h2D_MCcorrY_PR_Pbp = (TH2D*)f2D->Get("otherMCInfo/h2D_MCcorrY_PR_Pbp");
	TH2D* h2D_MCcorrY_NP_Pbp = (TH2D*)f2D->Get("otherMCInfo/h2D_MCcorrY_NP_Pbp");
	TH2D* h2D_MCcorrY_PR_pPb = (TH2D*)f2D->Get("otherMCInfo/h2D_MCcorrY_PR_pPb");
	TH2D* h2D_MCcorrY_NP_pPb = (TH2D*)f2D->Get("otherMCInfo/h2D_MCcorrY_NP_pPb");
	cout << "h2D_MCcorrY_PR_Pbp = " << h2D_MCcorrY_PR_Pbp << endl;
	cout << "h2D_MCcorrY_NP_Pbp = " << h2D_MCcorrY_NP_Pbp << endl;
	cout << "h2D_MCcorrY_PR_pPb = " << h2D_MCcorrY_PR_pPb << endl;
	cout << "h2D_MCcorrY_NP_pPb = " << h2D_MCcorrY_NP_pPb << endl;

	const int nbinsX = h2D_MCcorrY_PR_Pbp->GetNbinsX();
	const int nbinsY = h2D_MCcorrY_PR_Pbp->GetNbinsY();
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
	// --- read-in 2D hist for GEN dist.
	TH2D* h2D_Acc_Den_pt_y_PRMC_Pbp = (TH2D*)f2D->Get("otherMCInfo/h2D_Acc_Den_pt_y_PRMC_Pbp");
	TH2D* h2D_Acc_Den_pt_y_NPMC_Pbp = (TH2D*)f2D->Get("otherMCInfo/h2D_Acc_Den_pt_y_NPMC_Pbp");
	TH2D* h2D_Acc_Den_pt_y_PRMC_pPb = (TH2D*)f2D->Get("otherMCInfo/h2D_Acc_Den_pt_y_PRMC_pPb");
	TH2D* h2D_Acc_Den_pt_y_NPMC_pPb = (TH2D*)f2D->Get("otherMCInfo/h2D_Acc_Den_pt_y_NPMC_pPb");
	cout << "h2D_Acc_Den_pt_y_PRMC_Pbp = " << h2D_Acc_Den_pt_y_PRMC_Pbp << endl;
	cout << "h2D_Acc_Den_pt_y_NPMC_Pbp = " << h2D_Acc_Den_pt_y_NPMC_Pbp << endl;
	cout << "h2D_Acc_Den_pt_y_PRMC_pPb = " << h2D_Acc_Den_pt_y_PRMC_pPb << endl;
	cout << "h2D_Acc_Den_pt_y_NPMC_pPb = " << h2D_Acc_Den_pt_y_NPMC_pPb << endl;

	// ---  projection to 1D hist
	TH1D* h1D_MCcorrY_PR_Pbp[nbinsX]; 
	TH1D* h1D_MCcorrY_NP_Pbp[nbinsX]; 
	TH1D* h1D_MCcorrY_PR_pPb[nbinsX]; 
	TH1D* h1D_MCcorrY_NP_pPb[nbinsX]; 
	TH1D* h1D_Acc_Den_PRMC_Pbp[nbinsX]; 
	TH1D* h1D_Acc_Den_NPMC_Pbp[nbinsX]; 
	TH1D* h1D_Acc_Den_PRMC_pPb[nbinsX]; 
	TH1D* h1D_Acc_Den_NPMC_pPb[nbinsX]; 
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		////// for corrY
		h1D_MCcorrY_PR_Pbp[iy] = h2D_MCcorrY_PR_Pbp->ProjectionY(Form("h1D_MCcorrY_PR_Pbp_%d",iy),iy+1,iy+1);
		h1D_MCcorrY_NP_Pbp[iy] = h2D_MCcorrY_NP_Pbp->ProjectionY(Form("h1D_MCcorrY_NP_Pbp_%d",iy),iy+1,iy+1);
		h1D_MCcorrY_PR_pPb[iy] = h2D_MCcorrY_PR_pPb->ProjectionY(Form("h1D_MCcorrY_PR_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
		h1D_MCcorrY_NP_pPb[iy] = h2D_MCcorrY_NP_pPb->ProjectionY(Form("h1D_MCcorrY_NP_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
		////// for GEN dist.
		h1D_Acc_Den_PRMC_Pbp[iy] = h2D_Acc_Den_pt_y_PRMC_Pbp->ProjectionY(Form("h1D_Acc_Den_PRMC_Pbp_%d",iy),iy+1,iy+1);
		h1D_Acc_Den_NPMC_Pbp[iy] = h2D_Acc_Den_pt_y_NPMC_Pbp->ProjectionY(Form("h1D_Acc_Den_NPMC_Pbp_%d",iy),iy+1,iy+1);
		h1D_Acc_Den_PRMC_pPb[iy] = h2D_Acc_Den_pt_y_PRMC_pPb->ProjectionY(Form("h1D_Acc_Den_PRMC_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
		h1D_Acc_Den_NPMC_pPb[iy] = h2D_Acc_Den_pt_y_NPMC_pPb->ProjectionY(Form("h1D_Acc_Den_NPMC_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
	// set values as zero for unused bins
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_MCcorrY_PR_Pbp[iy]->SetBinContent(1,0);
			h1D_MCcorrY_NP_Pbp[iy]->SetBinContent(1,0);
			h1D_MCcorrY_PR_Pbp[iy]->SetBinError(1,0);
			h1D_MCcorrY_NP_Pbp[iy]->SetBinError(1,0);
			h1D_MCcorrY_PR_pPb[iy]->SetBinContent(1,0);
			h1D_MCcorrY_NP_pPb[iy]->SetBinContent(1,0);
			h1D_MCcorrY_PR_pPb[iy]->SetBinError(1,0);
			h1D_MCcorrY_NP_pPb[iy]->SetBinError(1,0);
			h1D_Acc_Den_PRMC_Pbp[iy]->SetBinContent(1,0);
			h1D_Acc_Den_NPMC_Pbp[iy]->SetBinContent(1,0);
			h1D_Acc_Den_PRMC_Pbp[iy]->SetBinError(1,0);
			h1D_Acc_Den_NPMC_Pbp[iy]->SetBinError(1,0);
			h1D_Acc_Den_PRMC_pPb[iy]->SetBinContent(1,0);
			h1D_Acc_Den_NPMC_pPb[iy]->SetBinContent(1,0);
			h1D_Acc_Den_PRMC_pPb[iy]->SetBinError(1,0);
			h1D_Acc_Den_NPMC_pPb[iy]->SetBinError(1,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_MCcorrY_PR_Pbp[iy]->SetBinContent(2,0);
			h1D_MCcorrY_PR_Pbp[iy]->SetBinContent(3,0);
			h1D_MCcorrY_NP_Pbp[iy]->SetBinContent(2,0);
			h1D_MCcorrY_NP_Pbp[iy]->SetBinContent(3,0);
			h1D_MCcorrY_PR_Pbp[iy]->SetBinError(2,0);
			h1D_MCcorrY_PR_Pbp[iy]->SetBinError(3,0);
			h1D_MCcorrY_NP_Pbp[iy]->SetBinError(2,0);
			h1D_MCcorrY_NP_Pbp[iy]->SetBinError(3,0);
			h1D_MCcorrY_PR_pPb[iy]->SetBinContent(2,0);
			h1D_MCcorrY_PR_pPb[iy]->SetBinContent(3,0);
			h1D_MCcorrY_NP_pPb[iy]->SetBinContent(2,0);
			h1D_MCcorrY_NP_pPb[iy]->SetBinContent(3,0);
			h1D_MCcorrY_PR_pPb[iy]->SetBinError(2,0);
			h1D_MCcorrY_PR_pPb[iy]->SetBinError(3,0);
			h1D_MCcorrY_NP_pPb[iy]->SetBinError(2,0);
			h1D_MCcorrY_NP_pPb[iy]->SetBinError(3,0);
			h1D_Acc_Den_PRMC_Pbp[iy]->SetBinContent(2,0);
			h1D_Acc_Den_PRMC_Pbp[iy]->SetBinContent(3,0);
			h1D_Acc_Den_NPMC_Pbp[iy]->SetBinContent(2,0);
			h1D_Acc_Den_NPMC_Pbp[iy]->SetBinContent(3,0);
			h1D_Acc_Den_PRMC_Pbp[iy]->SetBinError(2,0);
			h1D_Acc_Den_PRMC_Pbp[iy]->SetBinError(3,0);
			h1D_Acc_Den_NPMC_Pbp[iy]->SetBinError(2,0);
			h1D_Acc_Den_NPMC_Pbp[iy]->SetBinError(3,0);
			h1D_Acc_Den_PRMC_pPb[iy]->SetBinContent(2,0);
			h1D_Acc_Den_PRMC_pPb[iy]->SetBinContent(3,0);
			h1D_Acc_Den_NPMC_pPb[iy]->SetBinContent(2,0);
			h1D_Acc_Den_NPMC_pPb[iy]->SetBinContent(3,0);
			h1D_Acc_Den_PRMC_pPb[iy]->SetBinError(2,0);
			h1D_Acc_Den_PRMC_pPb[iy]->SetBinError(3,0);
			h1D_Acc_Den_NPMC_pPb[iy]->SetBinError(2,0);
			h1D_Acc_Den_NPMC_pPb[iy]->SetBinError(3,0);
		}
		if (iy>=2 && iy<=4) {
			h1D_MCcorrY_PR_Pbp[iy]->SetBinContent(4,0);
			h1D_MCcorrY_NP_Pbp[iy]->SetBinContent(4,0);
			h1D_MCcorrY_PR_Pbp[iy]->SetBinError(4,0);
			h1D_MCcorrY_NP_Pbp[iy]->SetBinError(4,0);
			h1D_MCcorrY_PR_pPb[iy]->SetBinContent(4,0);
			h1D_MCcorrY_NP_pPb[iy]->SetBinContent(4,0);
			h1D_MCcorrY_PR_pPb[iy]->SetBinError(4,0);
			h1D_MCcorrY_NP_pPb[iy]->SetBinError(4,0);
			h1D_Acc_Den_PRMC_Pbp[iy]->SetBinContent(4,0);
			h1D_Acc_Den_NPMC_Pbp[iy]->SetBinContent(4,0);
			h1D_Acc_Den_PRMC_Pbp[iy]->SetBinError(4,0);
			h1D_Acc_Den_NPMC_Pbp[iy]->SetBinError(4,0);
			h1D_Acc_Den_PRMC_pPb[iy]->SetBinContent(4,0);
			h1D_Acc_Den_NPMC_pPb[iy]->SetBinContent(4,0);
			h1D_Acc_Den_PRMC_pPb[iy]->SetBinError(4,0);
			h1D_Acc_Den_NPMC_pPb[iy]->SetBinError(4,0);
		}
	}
	
	////// after zero-bin setting, normalize!
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_MCcorrY_PR_Pbp[iy]->Scale(1./h1D_MCcorrY_PR_Pbp[iy]->Integral());
		h1D_MCcorrY_NP_Pbp[iy]->Scale(1./h1D_MCcorrY_NP_Pbp[iy]->Integral());
		h1D_MCcorrY_PR_pPb[iy]->Scale(1./h1D_MCcorrY_PR_pPb[iy]->Integral());
		h1D_MCcorrY_NP_pPb[iy]->Scale(1./h1D_MCcorrY_NP_pPb[iy]->Integral());
		h1D_Acc_Den_PRMC_Pbp[iy]->Scale(1./h1D_Acc_Den_PRMC_Pbp[iy]->Integral());
		h1D_Acc_Den_NPMC_Pbp[iy]->Scale(1./h1D_Acc_Den_NPMC_Pbp[iy]->Integral());
		h1D_Acc_Den_PRMC_pPb[iy]->Scale(1./h1D_Acc_Den_PRMC_pPb[iy]->Integral());
		h1D_Acc_Den_NPMC_pPb[iy]->Scale(1./h1D_Acc_Den_NPMC_pPb[iy]->Integral());
		h1D_MCcorrY_PR_Pbp[iy]->Scale(1,"width");
		h1D_MCcorrY_NP_Pbp[iy]->Scale(1,"width");
		h1D_MCcorrY_PR_pPb[iy]->Scale(1,"width");
		h1D_MCcorrY_NP_pPb[iy]->Scale(1,"width");
		h1D_Acc_Den_PRMC_Pbp[iy]->Scale(1,"width");
		h1D_Acc_Den_NPMC_Pbp[iy]->Scale(1,"width");
		h1D_Acc_Den_PRMC_pPb[iy]->Scale(1,"width");
		h1D_Acc_Den_NPMC_pPb[iy]->Scale(1,"width");
	}
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

	// --- prompt Pbp
	TCanvas* c_PR_Pbp = new TCanvas("c_PR_Pbp","c_PR_Pbp",200,10,600,600);
	c_PR_Pbp->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		SetHistStyle(h1D_MCcorrY_PR_Pbp[iy],1,0);
		SetHistStyle(h1D_Acc_Den_PRMC_Pbp[iy],2,0);
		h1D_MCcorrY_PR_Pbp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_MCcorrY_PR_Pbp[iy]->GetXaxis()->CenterTitle();
		h1D_MCcorrY_PR_Pbp[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_MCcorrY_PR_Pbp[iy]->GetYaxis()->SetTitle("");
		//if (isLog) h1D_corrY_PR_Pbp[iy]->GetYaxis()->SetRangeUser(0.01,700);
		//else h1D_corrY_PR_Pbp[iy]->GetYaxis()->SetRangeUser(0.0,350);
		h1D_MCcorrY_PR_Pbp[iy]->Draw("pe");
		h1D_Acc_Den_PRMC_Pbp[iy]->Draw("pe same");
		legUR -> SetHeader("Prompt J/#psi 1st run");
		legUR -> AddEntry(h1D_MCcorrY_PR_Pbp[iy],"RECO/(Acc*Eff)","lp");
		legUR -> AddEntry(h1D_Acc_Den_PRMC_Pbp[iy],"GEN","lp");
		latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
		legUR->Draw();
		c_PR_Pbp->SaveAs(Form("MCClosure_%s/mc_closure_pt_PR_Pbp_isLog%d_%d.pdf",dirName,(int)isLog,iy));
		c_PR_Pbp->Clear();
		legUR->Clear();
	}
	
	// --- prompt pPb
	TCanvas* c_PR_pPb = new TCanvas("c_PR_pPb","c_PR_pPb",200,10,600,600);
	c_PR_pPb->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		SetHistStyle(h1D_MCcorrY_PR_pPb[iy],1,0);
		SetHistStyle(h1D_Acc_Den_PRMC_pPb[iy],2,0);
		h1D_MCcorrY_PR_pPb[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_MCcorrY_PR_pPb[iy]->GetXaxis()->CenterTitle();
		h1D_MCcorrY_PR_pPb[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_MCcorrY_PR_pPb[iy]->GetYaxis()->SetTitle("");
		//if (isLog) h1D_corrY_PR_pPb[iy]->GetYaxis()->SetRangeUser(0.01,700);
		//else h1D_corrY_PR_pPb[iy]->GetYaxis()->SetRangeUser(0.0,350);
		h1D_MCcorrY_PR_pPb[iy]->Draw("pe");
		h1D_Acc_Den_PRMC_pPb[iy]->Draw("pe same");
		legUR -> SetHeader("Prompt J/#psi 2nd run");
		legUR -> AddEntry(h1D_MCcorrY_PR_pPb[iy],"RECO/(Acc*Eff)","lp");
		legUR -> AddEntry(h1D_Acc_Den_PRMC_pPb[iy],"GEN","lp");
		latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
		legUR->Draw();
		c_PR_pPb->SaveAs(Form("MCClosure_%s/mc_closure_pt_PR_pPb_isLog%d_%d.pdf",dirName,(int)isLog,iy));
		c_PR_pPb->Clear();
		legUR->Clear();
	}
	// --- non-prompt Pbp
	TCanvas* c_NP_Pbp = new TCanvas("c_NP_Pbp","c_NP_Pbp",200,10,600,600);
	c_NP_Pbp->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		SetHistStyle(h1D_MCcorrY_NP_Pbp[iy],1,0);
		SetHistStyle(h1D_Acc_Den_NPMC_Pbp[iy],2,0);
		h1D_MCcorrY_NP_Pbp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_MCcorrY_NP_Pbp[iy]->GetXaxis()->CenterTitle();
		h1D_MCcorrY_NP_Pbp[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_MCcorrY_NP_Pbp[iy]->GetYaxis()->SetTitle("");
		//if (isLog) h1D_corrY_NP_Pbp[iy]->GetYaxis()->SetRangeUser(0.01,700);
		//else h1D_corrY_NP_Pbp[iy]->GetYaxis()->SetRangeUser(0.0,350);
		h1D_MCcorrY_NP_Pbp[iy]->Draw("pe");
		h1D_Acc_Den_NPMC_Pbp[iy]->Draw("pe same");
		legUR -> SetHeader("Non-prompt J/#psi 1st run");
		legUR -> AddEntry(h1D_MCcorrY_NP_Pbp[iy],"RECO/(Acc*Eff)","lp");
		legUR -> AddEntry(h1D_Acc_Den_NPMC_Pbp[iy],"GEN","lp");
		latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
		legUR->Draw();
		c_NP_Pbp->SaveAs(Form("MCClosure_%s/mc_closure_pt_NP_Pbp_isLog%d_%d.pdf",dirName,(int)isLog,iy));
		c_NP_Pbp->Clear();
		legUR->Clear();
	}
	
	// --- non-prompt pPb
	TCanvas* c_NP_pPb = new TCanvas("c_NP_pPb","c_NP_pPb",200,10,600,600);
	c_NP_pPb->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		SetHistStyle(h1D_MCcorrY_NP_pPb[iy],1,0);
		SetHistStyle(h1D_Acc_Den_NPMC_pPb[iy],2,0);
		h1D_MCcorrY_NP_pPb[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1D_MCcorrY_NP_pPb[iy]->GetXaxis()->CenterTitle();
		h1D_MCcorrY_NP_pPb[iy]->GetXaxis()->SetRangeUser(0.,30.);
		h1D_MCcorrY_NP_pPb[iy]->GetYaxis()->SetTitle("");
		//if (isLog) h1D_corrY_NP_pPb[iy]->GetYaxis()->SetRangeUser(0.01,700);
		//else h1D_corrY_NP_pPb[iy]->GetYaxis()->SetRangeUser(0.0,350);
		h1D_MCcorrY_NP_pPb[iy]->Draw("pe");
		h1D_Acc_Den_NPMC_pPb[iy]->Draw("pe same");
		legUR -> SetHeader("Non-prompt J/#psi 2nd run");
		legUR -> AddEntry(h1D_MCcorrY_NP_pPb[iy],"RECO/(Acc*Eff)","lp");
		legUR -> AddEntry(h1D_Acc_Den_NPMC_pPb[iy],"GEN","lp");
		latex->DrawLatex(0.46,0.68,Form("%s",rapArr[iy].c_str()));
		legUR->Draw();
		c_NP_pPb->SaveAs(Form("MCClosure_%s/mc_closure_pt_NP_pPb_isLog%d_%d.pdf",dirName,(int)isLog,iy));
		c_NP_pPb->Clear();
		legUR->Clear();
	}	
	
	//////////////////////////////////////////////////////////////////
	// hRatio
	TH1D* hRatio_PR_Pbp[nRap];
	TH1D* hRatio_PR_pPb[nRap];
	TH1D* hRatio_NP_Pbp[nRap];
	TH1D* hRatio_NP_pPb[nRap];
	
	// --- prompt Pbp
	c_PR_Pbp->cd();
	gPad->SetLogy(0);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		hRatio_PR_Pbp[iy]=(TH1D*)h1D_MCcorrY_PR_Pbp[iy]->Clone(Form("hRatio_PR_Pbp_%d",iy));
		hRatio_PR_Pbp[iy]->Divide(h1D_Acc_Den_PRMC_Pbp[iy]);
		SetHistStyle(hRatio_PR_Pbp[iy],5,0);
		hRatio_PR_Pbp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		hRatio_PR_Pbp[iy]->GetYaxis()->SetTitle("Ratio");
		hRatio_PR_Pbp[iy]->GetXaxis()->SetRangeUser(0.,30.);
		hRatio_PR_Pbp[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
		hRatio_PR_Pbp[iy]->Draw("pe");
		legBL -> SetHeader("Prompt J/#psi 1st run");
		legBL -> AddEntry(hRatio_PR_Pbp[iy],"[RECO/(Acc*Eff)] / GEN","lp");
		legBL->Draw();
		latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,30.,1.,1,1);
		c_PR_Pbp->SaveAs(Form("MCClosure_%s/mc_closure_ratio_PR_Pbp_isLog%d_%d.pdf",dirName,(int)isLog,iy));
		c_PR_Pbp->Clear();
		legBL->Clear();
	}

	// --- prompt pPb
	c_PR_pPb->cd();
	gPad->SetLogy(0);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		hRatio_PR_pPb[iy]=(TH1D*)h1D_MCcorrY_PR_pPb[iy]->Clone(Form("hRatio_PR_pPb_%d",iy));
		hRatio_PR_pPb[iy]->Divide(h1D_Acc_Den_PRMC_pPb[iy]);
		SetHistStyle(hRatio_PR_pPb[iy],5,0);
		hRatio_PR_pPb[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		hRatio_PR_pPb[iy]->GetYaxis()->SetTitle("Ratio");
		hRatio_PR_pPb[iy]->GetXaxis()->SetRangeUser(0.,30.);
		hRatio_PR_pPb[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
		hRatio_PR_pPb[iy]->Draw("pe");
		legBL -> SetHeader("Prompt J/#psi 1st run");
		legBL -> AddEntry(hRatio_PR_pPb[iy],"[RECO/(Acc*Eff)] / GEN","lp");
		legBL->Draw();
		latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,30.,1.,1,1);
		c_PR_pPb->SaveAs(Form("MCClosure_%s/mc_closure_ratio_PR_pPb_isLog%d_%d.pdf",dirName,(int)isLog,iy));
		c_PR_pPb->Clear();
		legBL->Clear();
	}

	// --- prompt Pbp
	c_NP_Pbp->cd();
	gPad->SetLogy(0);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		hRatio_NP_Pbp[iy]=(TH1D*)h1D_MCcorrY_NP_Pbp[iy]->Clone(Form("hRatio_NP_Pbp_%d",iy));
		hRatio_NP_Pbp[iy]->Divide(h1D_Acc_Den_NPMC_Pbp[iy]);
		SetHistStyle(hRatio_NP_Pbp[iy],5,0);
		hRatio_NP_Pbp[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		hRatio_NP_Pbp[iy]->GetYaxis()->SetTitle("Ratio");
		hRatio_NP_Pbp[iy]->GetXaxis()->SetRangeUser(0.,30.);
		hRatio_NP_Pbp[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
		hRatio_NP_Pbp[iy]->Draw("pe");
		legBL -> SetHeader("Non-prompt J/#psi 1st run");
		legBL -> AddEntry(hRatio_NP_Pbp[iy],"[RECO/(Acc*Eff)] / GEN","lp");
		legBL->Draw();
		latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,30.,1.,1,1);
		c_NP_Pbp->SaveAs(Form("MCClosure_%s/mc_closure_ratio_NP_Pbp_isLog%d_%d.pdf",dirName,(int)isLog,iy));
		c_NP_Pbp->Clear();
		legBL->Clear();
	}

	// --- prompt pPb
	c_NP_pPb->cd();
	gPad->SetLogy(0);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		hRatio_NP_pPb[iy]=(TH1D*)h1D_MCcorrY_NP_pPb[iy]->Clone(Form("hRatio_NP_pPb_%d",iy));
		hRatio_NP_pPb[iy]->Divide(h1D_Acc_Den_NPMC_pPb[iy]);
		SetHistStyle(hRatio_NP_pPb[iy],5,0);
		hRatio_NP_pPb[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		hRatio_NP_pPb[iy]->GetYaxis()->SetTitle("Ratio");
		hRatio_NP_pPb[iy]->GetXaxis()->SetRangeUser(0.,30.);
		hRatio_NP_pPb[iy]->GetYaxis()->SetRangeUser(0.5,1.5);
		hRatio_NP_pPb[iy]->Draw("pe");
		legBL -> SetHeader("Non-prompt J/#psi 1st run");
		legBL -> AddEntry(hRatio_NP_pPb[iy],"[RECO/(Acc*Eff)] / GEN","lp");
		legBL->Draw();
		latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,30.,1.,1,1);
		c_NP_pPb->SaveAs(Form("MCClosure_%s/mc_closure_ratio_NP_pPb_isLog%d_%d.pdf",dirName,(int)isLog,iy));
		c_NP_pPb->Clear();
		legBL->Clear();
	}

/*
	// root file
	TFile *fOut = new TFile(Form("1Dhist_%s/crossSection_pt_%s.root",dirName,runstring.c_str()),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_corrY_NP_Pbp[iy]->Write();
		h1D_corrY_NP_pPb[iy]->Write();
		h1D_corrY_NP_tot[iy]->Write();
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


