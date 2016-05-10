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

void draw_1D_data_mc_reco_rap_integPt(char* dirName = "8rap9pt", bool isPrompt = true)
{
	gROOT->Macro("./JpsiStyle.C");

	
	//rap array in yCM (from forward to backward)
	//Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	const Int_t nRap = sizeof(rapArrNumBF)/sizeof(Double_t)-1;
	cout << "nRap = " << nRap << endl;
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		//rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
		rapBinW[iy] = rapArrNumBF[iy+1]-rapArrNumBF[iy]; 
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
		formRapArr(rapArrNumBF[iy], rapArrNumBF[iy+1], &rapArr[iy]);
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
	TH1D* h1D_fit_pt_y_Pbp_tmp[nbinsY]; // in y_lab(1st)
	TH1D* h1D_fit_pt_y_pPb_tmp[nbinsY]; // in y_lab(2nd)
	TH1D* h1D_fit_pt_y_Pbp[nbinsY]; //in y_CM
	TH1D* h1D_fit_pt_y_pPb[nbinsY]; //in y_CM
	
	TH1D* h1D_Eff_Num_Pbp_tmp[nbinsY];
	TH1D* h1D_Eff_Num_pPb_tmp[nbinsY];
	TH1D* h1D_Eff_Num_Pbp[nbinsY];
	TH1D* h1D_Eff_Num_pPb[nbinsY];

	// iy=0 refers to forwards !!! (ordering here && in CM)
	double tmpvalDataPbp,tmpvalDatapPb,tmpvalMCPbp,tmpvalMCpPb;
	double tmperrDataPbp,tmperrDatapPb,tmperrMCPbp,tmperrMCpPb;
//	TCanvas *ctmp1 = new TCanvas("ctmp1","",600,600);
//	TCanvas *ctmp2 = new TCanvas("ctmp2","",600,600);
	for (Int_t ipt = 0; ipt < nbinsY; ipt++) {
		//if (ipt !=0) continue;
		h1D_fit_pt_y_Pbp_tmp[ipt] = h2D_fit_pt_y_Pbp->ProjectionX(Form("h1D_fit_pt_y_Pbp_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_fit_pt_y_pPb_tmp[ipt] = h2D_fit_pt_y_pPb->ProjectionX(Form("h1D_fit_pt_y_pPb_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_fit_pt_y_Pbp[ipt] = new TH1D(Form("h1D_fit_pt_y_Pbp_%d",ipt),Form("h1D_fit_pt_y_Pbp_%d",ipt),nRap,rapArrNumBF);
		h1D_fit_pt_y_pPb[ipt] = new TH1D(Form("h1D_fit_pt_y_pPb_%d",ipt),Form("h1D_fit_pt_y_pPb_%d",ipt),nRap,rapArrNumBF);
		h1D_fit_pt_y_Pbp[ipt]->Sumw2();
		h1D_fit_pt_y_pPb[ipt]->Sumw2();
		
		h1D_Eff_Num_Pbp_tmp[ipt] = h2D_Eff_Num_pt_y_Pbp->ProjectionX(Form("h1D_Eff_Num_Pbp_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_Eff_Num_pPb_tmp[ipt] = h2D_Eff_Num_pt_y_pPb->ProjectionX(Form("h1D_Eff_Num_pPb_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_Eff_Num_Pbp[ipt] = new TH1D(Form("h1D_Eff_Num_Pbp_%d",ipt),Form("h1D_Eff_Num_Pbp_%d",ipt),nRap,rapArrNumBF);
		h1D_Eff_Num_pPb[ipt] = new TH1D(Form("h1D_Eff_Num_pPb_%d",ipt),Form("h1D_Eff_Num_pPb_%d",ipt),nRap,rapArrNumBF);
		h1D_Eff_Num_Pbp[ipt]->Sumw2();
		h1D_Eff_Num_pPb[ipt]->Sumw2();
		
		for (Int_t iy=0; iy<nbinsX; iy++){
			tmpvalDataPbp=0;tmpvalDatapPb=0;tmpvalMCPbp=0;tmpvalMCpPb=0;
			tmperrDataPbp=0;tmperrDatapPb=0;tmperrMCPbp=0;tmperrMCpPb=0;
			//1st run
			tmpvalDataPbp = h1D_fit_pt_y_Pbp_tmp[ipt]->GetBinContent(iy+1);
			tmperrDataPbp = h1D_fit_pt_y_Pbp_tmp[ipt]->GetBinError(iy+1);
			h1D_fit_pt_y_Pbp[ipt]->SetBinContent(nbinsX-iy,tmpvalDataPbp);
			h1D_fit_pt_y_Pbp[ipt]->SetBinError(nbinsX-iy,tmperrDataPbp);
			tmpvalMCPbp = h1D_Eff_Num_Pbp_tmp[ipt]->GetBinContent(iy+1);
			tmperrMCPbp = h1D_Eff_Num_Pbp_tmp[ipt]->GetBinError(iy+1);
			h1D_Eff_Num_Pbp[ipt]->SetBinContent(nbinsX-iy,tmpvalMCPbp);
			h1D_Eff_Num_Pbp[ipt]->SetBinError(nbinsX-iy,tmperrMCPbp);
			//2nd run
			tmpvalDatapPb = h1D_fit_pt_y_pPb_tmp[ipt]->GetBinContent(iy+1);
			tmperrDatapPb = h1D_fit_pt_y_pPb_tmp[ipt]->GetBinError(iy+1);
			h1D_fit_pt_y_pPb[ipt]->SetBinContent(iy+1,tmpvalDatapPb);
			h1D_fit_pt_y_pPb[ipt]->SetBinError(iy+1,tmperrDatapPb);
			tmpvalMCpPb = h1D_Eff_Num_pPb_tmp[ipt]->GetBinContent(iy+1);
			tmperrMCpPb = h1D_Eff_Num_pPb_tmp[ipt]->GetBinError(iy+1);
			h1D_Eff_Num_pPb[ipt]->SetBinContent(iy+1,tmpvalMCpPb);
			h1D_Eff_Num_pPb[ipt]->SetBinError(iy+1,tmperrMCpPb);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////

	// set values as zero for unused bins
	for (Int_t ipt = 0; ipt < nbinsY; ipt++) {
		//if (ipt !=7) continue;
		if (ipt==0) {
			for (Int_t iy=1; iy< 7;iy++){
				h1D_fit_pt_y_Pbp[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_Pbp[ipt]->SetBinError(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinError(iy+1,0);
				h1D_Eff_Num_Pbp[ipt]->SetBinContent(iy+1,0);
				h1D_Eff_Num_Pbp[ipt]->SetBinError(iy+1,0);
				h1D_Eff_Num_pPb[ipt]->SetBinContent(iy+1,0);
				h1D_Eff_Num_pPb[ipt]->SetBinError(iy+1,0);
			}
		}
		else if (ipt==1 || ipt==2){
			for (Int_t iy=2; iy< 6;iy++){
				h1D_fit_pt_y_Pbp[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_Pbp[ipt]->SetBinError(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinError(iy+1,0);
				h1D_Eff_Num_Pbp[ipt]->SetBinContent(iy+1,0);
				h1D_Eff_Num_Pbp[ipt]->SetBinError(iy+1,0);
				h1D_Eff_Num_pPb[ipt]->SetBinContent(iy+1,0);
				h1D_Eff_Num_pPb[ipt]->SetBinError(iy+1,0);
			}
		}
		else if (ipt==3){
			for (Int_t iy=3; iy< 6;iy++){
				h1D_fit_pt_y_Pbp[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_Pbp[ipt]->SetBinError(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinError(iy+1,0);
				h1D_Eff_Num_Pbp[ipt]->SetBinContent(iy+1,0);
				h1D_Eff_Num_Pbp[ipt]->SetBinError(iy+1,0);
				h1D_Eff_Num_pPb[ipt]->SetBinContent(iy+1,0);
				h1D_Eff_Num_pPb[ipt]->SetBinError(iy+1,0);
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////
	////// after zero-bin setting, normalize!
/*
	for (Int_t ipt=0; ipt<nbinsY; ipt++){	
		h1D_fit_pt_y_Pbp[ipt]->Scale(1./h1D_fit_pt_y_Pbp[ipt]->Integral());	
		h1D_fit_pt_y_pPb[ipt]->Scale(1./h1D_fit_pt_y_pPb[ipt]->Integral());	
		h1D_Eff_Num_Pbp[ipt]->Scale(1./h1D_Eff_Num_Pbp[ipt]->Integral());	
		h1D_Eff_Num_pPb[ipt]->Scale(1./h1D_Eff_Num_pPb[ipt]->Integral());	
		h1D_fit_pt_y_Pbp[ipt]->Scale(1,"width");	
		h1D_fit_pt_y_pPb[ipt]->Scale(1,"width");	
		h1D_Eff_Num_Pbp[ipt]->Scale(1,"width");	
		h1D_Eff_Num_pPb[ipt]->Scale(1,"width");	
	}
*/

	//////////////////////////////////////////////////////////////////
	// *** merging all pT bins for rapidity dist
	TH1D* hDataReco_Pbp;
	TH1D* hDataReco_pPb;
	TH1D* hMCReco_Pbp;
	TH1D* hMCReco_pPb;
	hDataReco_Pbp=(TH1D*)h1D_fit_pt_y_Pbp[0]->Clone("hDataReco_Pbp");
	hDataReco_pPb=(TH1D*)h1D_fit_pt_y_pPb[0]->Clone("hDataReco_pPb");
	hMCReco_Pbp=(TH1D*)h1D_Eff_Num_Pbp[0]->Clone("hMCReco_Pbp");
	hMCReco_pPb=(TH1D*)h1D_Eff_Num_pPb[0]->Clone("hMCReco_pPb");
	for (Int_t ipt = 1; ipt < nbinsY; ipt++) {
		hDataReco_Pbp->Add(h1D_fit_pt_y_Pbp[ipt]);	
		hDataReco_pPb->Add(h1D_fit_pt_y_pPb[ipt]);	
		hMCReco_Pbp->Add(h1D_fit_pt_y_Pbp[ipt]);	
		hMCReco_pPb->Add(h1D_fit_pt_y_pPb[ipt]);	
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

	double maxVal = 0;
	//TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
	TLegend *legUR = new TLegend(0.52, 0.75, 0.86, 0.92); //upper left
	TLegend *legUM = new TLegend(0.38, 0.75, 0.72, 0.92); //upper left
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	// ---- 1st run Pbp
	TCanvas* c_Pbp = new TCanvas("c_Pbp","c_Pbp",200,10,600,600);
	c_Pbp->cd();
	gPad->SetLogy(0);
	SetHistStyle(hDataReco_Pbp,1,0);
	SetHistStyle(hMCReco_Pbp,2,0);
	hDataReco_Pbp->GetXaxis()->SetTitle("y_{CM}");
	hDataReco_Pbp->GetXaxis()->CenterTitle();
	hDataReco_Pbp->GetXaxis()->SetRangeUser(-3.,2.);
	hDataReco_Pbp->GetYaxis()->SetTitle("");
	maxVal = hDataReco_Pbp->GetMaximum();
	hDataReco_Pbp->GetYaxis()->SetRangeUser(0.,maxVal*1.5);
	hDataReco_Pbp->Draw("pe");
	hMCReco_Pbp->Draw("pe same");
	if (isPrompt) legUM -> SetHeader("Prompt J/#psi 1st run");
	else legUM -> SetHeader("Non-prompt J/#psi 1st run");
	legUM -> AddEntry(hDataReco_Pbp,"data RECO","lp");
	legUM -> AddEntry(hMCReco_Pbp,"MC RECO","lp");
	legUM->Draw();
//	latex->DrawLatex(0.38, 0.72, Form("%s",ptArr.c_str()));	
	c_Pbp->SaveAs(Form("DataMcReco_rapIntegPt_%s/data_mc_reco_rap_Pbp_isPrompt%d.pdf",dirName,(int)isPrompt));
	c_Pbp->Clear();
	legUM->Clear();

	// ---- 1st run pPb
	TCanvas* c_pPb = new TCanvas("c_pPb","c_pPb",200,10,600,600);
	c_pPb->cd();
	gPad->SetLogy(0);
	SetHistStyle(hDataReco_pPb,1,0);
	SetHistStyle(hMCReco_pPb,2,0);
	hDataReco_pPb->GetXaxis()->SetTitle("y_{CM}");
	hDataReco_pPb->GetXaxis()->CenterTitle();
	hDataReco_pPb->GetXaxis()->SetRangeUser(-3.,2.);
	hDataReco_pPb->GetYaxis()->SetTitle("");
	maxVal = hDataReco_pPb->GetMaximum();
	hDataReco_pPb->GetYaxis()->SetRangeUser(0.,maxVal*1.5);
	hDataReco_pPb->Draw("pe");
	hMCReco_pPb->Draw("pe same");
	if (isPrompt) legUM -> SetHeader("Prompt J/#psi 2nd run");
	else legUM -> SetHeader("Non-prompt J/#psi 2nd run");
	legUM -> AddEntry(hDataReco_pPb,"data RECO","lp");
	legUM -> AddEntry(hMCReco_pPb,"MC RECO","lp");
	legUM->Draw();
//	latex->DrawLatex(0.38, 0.72, Form("%s",ptArr.c_str()));	
	c_pPb->SaveAs(Form("DataMcReco_rapIntegPt_%s/data_mc_reco_rap_pPb_isPrompt%d.pdf",dirName,(int)isPrompt));
	c_pPb->Clear();
	legUM->Clear();

	//////////////////////////////////////////////////////////////////
	TH1D* hRatio_Pbp;
	TH1D* hRatio_pPb;

	// ---- 1st run Pbp
	c_Pbp->cd();
	gPad->SetLogy(0);
	hRatio_Pbp=(TH1D*)hDataReco_Pbp->Clone("hRatio_Pbp");
	hRatio_Pbp->Divide(hMCReco_Pbp);
	SetHistStyle(hRatio_Pbp,5,0);
	hRatio_Pbp->GetXaxis()->SetTitle("y_{CM}");
	hRatio_Pbp->GetXaxis()->CenterTitle();
	hRatio_Pbp->GetXaxis()->SetRangeUser(-3.,2.);
	hRatio_Pbp->GetYaxis()->SetTitle("Ratio");
	hRatio_Pbp->GetYaxis()->SetRangeUser(0.5,1.5);
	hRatio_Pbp->Draw("pe");
	if (isPrompt) legUM -> SetHeader("Prompt J/#psi 1st run");
	else legUM -> SetHeader("Non-prompt J/#psi 1st run");
	legUM -> AddEntry(hRatio_Pbp,"data/MC","lp");
	legUM->Draw();
//	latex->DrawLatex(0.38, 0.72, Form("%s",ptArr.c_str()));	
	dashedLine(-3.,1.,2.,1.,1,1);
	c_Pbp->SaveAs(Form("DataMcReco_rapIntegPt_%s/data_mc_reco_rap_ratio_Pbp_isPrompt%d.pdf",dirName,(int)isPrompt));
	c_Pbp->Clear();
	legUM->Clear();

	// ---- 1st run pPb
	c_pPb->cd();
	gPad->SetLogy(0);
	hRatio_pPb=(TH1D*)hDataReco_pPb->Clone("hRatio_pPb");
	hRatio_pPb->Divide(hMCReco_pPb);
	SetHistStyle(hRatio_pPb,5,0);
	hRatio_pPb->GetXaxis()->SetTitle("y_{CM}");
	hRatio_pPb->GetXaxis()->CenterTitle();
	hRatio_pPb->GetXaxis()->SetRangeUser(-3.,2.);
	hRatio_pPb->GetYaxis()->SetTitle("Ratio");
	hRatio_pPb->GetYaxis()->SetRangeUser(0.5,1.5);
	hRatio_pPb->Draw("pe");
	if (isPrompt) legUM -> SetHeader("Prompt J/#psi 2nd run");
	else legUM -> SetHeader("Non-prompt J/#psi 2nd run");
	legUM -> AddEntry(hRatio_pPb,"data/MC","lp");
	legUM->Draw();
//	latex->DrawLatex(0.38, 0.72, Form("%s",ptArr.c_str()));	
	dashedLine(-3.,1.,2.,1.,1,1);
	c_pPb->SaveAs(Form("DataMcReco_rapIntegPt_%s/data_mc_reco_rap_ratio_pPb_isPrompt%d.pdf",dirName,(int)isPrompt));
	c_pPb->Clear();
	legUM->Clear();

	// root file
	TFile *fOut = new TFile(Form("DataMcReco_rapIntegPt_%s/data_mc_reco_rap_isPrompt%d.root",dirName,(int)isPrompt),"RECREATE");
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


