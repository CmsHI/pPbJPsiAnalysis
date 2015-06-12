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

void draw_1D_bfrac_rap(char* dirName = "8rap9pt")
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
		h2D_fit_pt_y_Pbp = (TH2D*)f2D->Get("otherFitInfo/h2D_fit_pt_y_bFrac_Pbp");
		h2D_fit_pt_y_pPb = (TH2D*)f2D->Get("otherFitInfo/h2D_fit_pt_y_bFrac_pPb");
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

	// ---  projection to 1D hist
	TH1D* h1D_fit_pt_y_Pbp_tmp[nbinsY]; // in y_lab(1st)
	TH1D* h1D_fit_pt_y_pPb_tmp[nbinsY]; // in y_lab(2nd)
	TH1D* h1D_fit_pt_y_Pbp[nbinsY]; //in y_CM
	TH1D* h1D_fit_pt_y_pPb[nbinsY]; //in y_CM
	
	// iy=0 refers to forwards !!! (ordering here && in CM)
	double tmpvalDataPbp,tmpvalDatapPb,tmpvalMCPbp,tmpvalMCpPb;
	double tmperrDataPbp,tmperrDatapPb,tmperrMCPbp,tmperrMCpPb;
	for (Int_t ipt = 0; ipt < nbinsY; ipt++) {
		//if (ipt !=0) continue;
		h1D_fit_pt_y_Pbp_tmp[ipt] = h2D_fit_pt_y_Pbp->ProjectionX(Form("h1D_fit_pt_y_Pbp_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_fit_pt_y_pPb_tmp[ipt] = h2D_fit_pt_y_pPb->ProjectionX(Form("h1D_fit_pt_y_pPb_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_fit_pt_y_Pbp[ipt] = new TH1D(Form("h1D_fit_pt_y_Pbp_%d",ipt),Form("h1D_fit_pt_y_Pbp_%d",ipt),nRap,rapArrNumBF);
		h1D_fit_pt_y_pPb[ipt] = new TH1D(Form("h1D_fit_pt_y_pPb_%d",ipt),Form("h1D_fit_pt_y_pPb_%d",ipt),nRap,rapArrNumBF);
		h1D_fit_pt_y_Pbp[ipt]->Sumw2();
		h1D_fit_pt_y_pPb[ipt]->Sumw2();
		
		for (Int_t iy=0; iy<nbinsX; iy++){
			tmpvalDataPbp=0;tmpvalDatapPb=0;tmpvalMCPbp=0;tmpvalMCpPb=0;
			tmperrDataPbp=0;tmperrDatapPb=0;tmperrMCPbp=0;tmperrMCpPb=0;
			//1st run
			tmpvalDataPbp = h1D_fit_pt_y_Pbp_tmp[ipt]->GetBinContent(iy+1);
			tmperrDataPbp = h1D_fit_pt_y_Pbp_tmp[ipt]->GetBinError(iy+1);
			h1D_fit_pt_y_Pbp[ipt]->SetBinContent(nbinsX-iy,tmpvalDataPbp);
			h1D_fit_pt_y_Pbp[ipt]->SetBinError(nbinsX-iy,tmperrDataPbp);
			//2nd run
			tmpvalDatapPb = h1D_fit_pt_y_pPb_tmp[ipt]->GetBinContent(iy+1);
			tmperrDatapPb = h1D_fit_pt_y_pPb_tmp[ipt]->GetBinError(iy+1);
			h1D_fit_pt_y_pPb[ipt]->SetBinContent(iy+1,tmpvalDatapPb);
			h1D_fit_pt_y_pPb[ipt]->SetBinError(iy+1,tmperrDatapPb);
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
			}
		}
		else if (ipt==1 || ipt==2){
			for (Int_t iy=2; iy< 6;iy++){
				h1D_fit_pt_y_Pbp[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_Pbp[ipt]->SetBinError(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinError(iy+1,0);
			}
		}
		else if (ipt==3){
			for (Int_t iy=3; iy< 6;iy++){
				h1D_fit_pt_y_Pbp[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_Pbp[ipt]->SetBinError(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinContent(iy+1,0);
				h1D_fit_pt_y_pPb[ipt]->SetBinError(iy+1,0);
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////
	////// after zero-bin setting, normalize!
//	for (Int_t ipt=0; ipt<nbinsY; ipt++){	
//		h1D_fit_pt_y_Pbp[ipt]->Scale(1./h1D_fit_pt_y_Pbp[ipt]->Integral());	
//		h1D_fit_pt_y_pPb[ipt]->Scale(1./h1D_fit_pt_y_pPb[ipt]->Integral());	
//		h1D_fit_pt_y_Pbp[ipt]->Scale(1,"width");	
//		h1D_fit_pt_y_pPb[ipt]->Scale(1,"width");	
//	}

/*	
	// *** bin merging
	cout << "*** low pT bin starts from : " << ptArr[low_init].c_str() << endl;
	for (Int_t ipt = low_init+1; ipt < high_init; ipt++) {
		h1D_corrY_PR_tot[low_init]->Add(h1D_corrY_PR_tot[ipt]);
		cout << " *** and to low pT, merging : " << ptArr[ipt].c_str() << endl; 
	}
	cout << "*** high pT bin starts from : " << ptArr[high_init].c_str() << endl;
	for (Int_t ipt = high_init+1; ipt < nbinsY; ipt++) {
		h1D_corrY_PR_tot[high_init]->Add(h1D_corrY_PR_tot[ipt]);
		cout << " *** and to high pT, merging : " << ptArr[ipt].c_str() << endl; 
	}
*/	
	
	//////////////////////////////////////////////////////////////////
	//// --- Draw histograms

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

	//TCanvas* c_multi = new TCanvas("c_multi","c_multi",200,10,600,600);
	TCanvas* c_multi = new TCanvas("c_multi","c_multi",200,10,1500,600);
	c_multi->Divide(5,2);
	gPad->SetLogy(0);
	for (Int_t ipt=0; ipt< nbinsY; ipt++) {
		c_multi->cd(ipt+1);
		SetHistStyle(h1D_fit_pt_y_Pbp[ipt],1,0);
		SetHistStyle(h1D_fit_pt_y_pPb[ipt],2,0);
		h1D_fit_pt_y_Pbp[ipt]->GetXaxis()->SetTitle("y_{CM}");
		h1D_fit_pt_y_Pbp[ipt]->GetXaxis()->CenterTitle();
		h1D_fit_pt_y_Pbp[ipt]->GetXaxis()->SetRangeUser(-3.,2.);
		h1D_fit_pt_y_Pbp[ipt]->GetYaxis()->SetTitle("raw yield");
		double maxVal = h1D_fit_pt_y_Pbp[ipt]->GetMaximum();
		h1D_fit_pt_y_Pbp[ipt]->GetYaxis()->SetRangeUser(0.,maxVal*1.5);
		h1D_fit_pt_y_Pbp[ipt]->Draw("pe");
		h1D_fit_pt_y_pPb[ipt]->Draw("pe same");
		if (ipt==0) {
//			if (isPrompt) legUM -> SetHeader("Prompt J/#psi");
//			else legUM -> SetHeader("Non-prompt J/#psi");
			legUM -> AddEntry(h1D_fit_pt_y_Pbp[ipt],"1st run (Pbp)","lp");
			legUM -> AddEntry(h1D_fit_pt_y_pPb[ipt],"2nd run (pPb)","lp");
			//legUM->Draw();
		}
//		latex->DrawLatex(0.38, 0.72, Form("%s",ptArr[ipt].c_str()));	
		latex->DrawLatex(0.38, 0.85, Form("%s",ptArr[ipt].c_str()));	
	}
	c_multi->Update();
	c_multi->cd(10);//tmp
	legUM->Draw();
	c_multi->Update();
	c_multi->SaveAs(Form("bfrac_%s/bfrac_rap.pdf",dirName));
//		legUM->Clear();


/*
	// root file
	//TFile *fOut = new TFile(Form("1Dhist_%s/tmp_crossSection_pt.root",dirName),"RECREATE");
	TFile *fOut = new TFile(Form("1Dhist_%s/crossSection_pt_%s.root",dirName,runstring.c_str()),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_corrY_PR_pPb[iy]->Write();
		h1D_corrY_PR_pPb[iy]->Write();
		hRatio_pPb[iy]->Write();
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


