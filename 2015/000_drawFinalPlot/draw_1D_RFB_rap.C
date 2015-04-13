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

#include "KYOcommonOptFinal.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

//// runCode // 0=merged, 1=1stRun, 2=2ndRun
void draw_1D_RFB_rap(char* dirName = "8rap9pt", int runCode=0, bool isPrompt = false)
{
	gROOT->Macro("./JpsiStyleForFinalResult.C");

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

	/////////////////////////////////////////////////////////////////////////
	/// systematic uncertainties by hand KYO
	const int ntmp=3;
	Double_t pxtmp_lowpt[ntmp]; // x point
	Double_t pxtmp_highpt[ntmp]; // x point
	Double_t pytmp_lowpt[ntmp]; // y point
	Double_t pytmp_highpt[ntmp]; // y point
	Double_t eytmp[ntmp]; // y point error
	Double_t ex[ntmp]; //x error
	Double_t exsys[ntmp]; //sys x error
	Double_t eysys_lowpt[ntmp]; //sys y error
	Double_t eysys_highpt[ntmp]; //sys y error
	ex = {0.0, 0.0, 0.0};
	exsys = {0.04, 0.04, 0.04};
	if (isPrompt) {
		eysys_lowpt = {
		0.039700808,
		0.03048564,
		0.036009465};
		eysys_highpt = {
		0.040365942,
		0.034592218,
		0.055332865};
	}
	else {
		eysys_lowpt = {
		0.053403861,
		0.06900436,
		0.071267586};
		eysys_highpt = {
		0.044216832,
		0.040294776,
		0.080713354};
	}
			
	
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

	// --- read-in file
	TFile * f2D = new TFile(Form("../000_fittingResult/total2Dhist_%s.root",dirName));
	cout << "dirName = " << dirName << endl;
	cout << "runCode = " << runCode << ", runstring = " << runstring.c_str() << endl;

	// --- read-in 2D hist for corrected yield
	TH2D* h2D_corrY_Pbp;
	TH2D* h2D_corrY_pPb;
	if (isPrompt) h2D_corrY_Pbp = (TH2D*)f2D->Get("h2D_corrY_PR_Pbp");
	else h2D_corrY_Pbp = (TH2D*)f2D->Get("h2D_corrY_NP_Pbp");
	if (isPrompt) h2D_corrY_pPb = (TH2D*)f2D->Get("h2D_corrY_PR_pPb");
	else h2D_corrY_pPb = (TH2D*)f2D->Get("h2D_corrY_NP_pPb");
	cout << "h2D_corrY_Pbp = " << h2D_corrY_Pbp << endl;
	cout << "h2D_corrY_pPb = " << h2D_corrY_pPb << endl;
	const int nbinsX = h2D_corrY_Pbp->GetNbinsX();
	const int nbinsY = h2D_corrY_Pbp->GetNbinsY();
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };

	// ---  projection to 1D hist
	TH1D* h1D_corrY_Pbp_tmp[nbinsY]; // in y_lab(1st)
	TH1D* h1D_corrY_Pbp[nbinsY]; 
	TH1D* h1D_corrY_pPb_tmp[nbinsY]; // in y_lab(2nd)
	TH1D* h1D_corrY_pPb[nbinsY]; //in y_CM

	// iy=0 refers to forwards !!! (ordering here && in CM)
	int tmpbin;
	double tmpval=0;
	double tmperr=0;
	for (Int_t ipt = 0; ipt < nbinsY; ipt++) {
		h1D_corrY_Pbp_tmp[ipt] = h2D_corrY_Pbp->ProjectionX(Form("h1D_corrY_Pbp_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_corrY_pPb_tmp[ipt] = h2D_corrY_pPb->ProjectionX(Form("h1D_corrY_pPb_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_corrY_Pbp[ipt] = new TH1D(Form("h1D_corrY_Pbp_%d",ipt),Form("h1D_corrY_Pbp_%d",ipt),nRap,rapArrNumBF);
		h1D_corrY_pPb[ipt] = new TH1D(Form("h1D_corrY_pPb_%d",ipt),Form("h1D_corrY_pPb_%d",ipt),nRap,rapArrNumBF);
		h1D_corrY_Pbp[ipt]->Sumw2();
		h1D_corrY_pPb[ipt]->Sumw2();
		
		for (Int_t iy=0; iy<nbinsX; iy++){
			//1st run
			tmpval = h1D_corrY_Pbp_tmp[ipt]->GetBinContent(iy+1);
			tmperr = h1D_corrY_Pbp_tmp[ipt]->GetBinError(iy+1);
			h1D_corrY_Pbp[ipt]->SetBinContent(nbinsX-iy,tmpval);
			h1D_corrY_Pbp[ipt]->SetBinError(nbinsX-iy,tmperr);
			//2nd run
			tmpval = h1D_corrY_pPb_tmp[ipt]->GetBinContent(iy+1);
			tmperr = h1D_corrY_pPb_tmp[ipt]->GetBinError(iy+1);
			h1D_corrY_pPb[ipt]->SetBinContent(iy+1,tmpval);
			h1D_corrY_pPb[ipt]->SetBinError(iy+1,tmperr);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	// 1) merge Pbp+pPb corrected yield
	
	TH1D* h1D_corrY_tot[nbinsY]; 
	
	for (Int_t ipt = 0; ipt < nbinsY; ipt++) {
		// --- merging
		if (runCode ==0) {	
			h1D_corrY_tot[ipt] = (TH1D*)h1D_corrY_Pbp[ipt]->Clone(Form("h1D_corrY_tot_%d",ipt));
			h1D_corrY_tot[ipt]->Add(h1D_corrY_pPb[ipt]);	
		}
		else if (runCode ==1) {
			h1D_corrY_tot[ipt] = (TH1D*)h1D_corrY_Pbp[ipt]->Clone(Form("h1D_corrY_tot_%d",ipt));
		}
		else if (runCode ==2) {
			h1D_corrY_tot[ipt] = (TH1D*)h1D_corrY_pPb[ipt]->Clone(Form("h1D_corrY_tot_%d",ipt));
		}
		cout <<" "<<endl;
		cout <<ipt<<"th pt !!!"<<endl;
		for (int iy=0; iy <nbinsX; iy ++ ){ 
			cout << iy <<"th rap" << endl;
			cout << "h1D_corrY_tot = " << h1D_corrY_tot[ipt]->GetBinContent(iy+1)<<endl; 
		}		
	}
	//////////////////////////////////////////////////////////////////
	/////////// calculate RFB
	
	const int lowpt_init=4;
	const int highpt_init=7;	

	// *** pt bin merging
	cout << "*** low pT bin starts from : " << ptArr[lowpt_init].c_str() << endl;
	for (Int_t ipt = lowpt_init+1; ipt < highpt_init; ipt++) {
		h1D_corrY_tot[lowpt_init]->Add(h1D_corrY_tot[ipt]);
		cout << " *** and to low pT, merging : " << ptArr[ipt].c_str() << endl; 
	}
	cout << "*** high pT bin starts from : " << ptArr[highpt_init].c_str() << endl;
	for (Int_t ipt = highpt_init+1; ipt < nbinsY; ipt++) {
		h1D_corrY_tot[highpt_init]->Add(h1D_corrY_tot[ipt]);
		cout << " *** and to high pT, merging : " << ptArr[ipt].c_str() << endl; 
	}
		
	// |yCM| for drawing (actual binning)
	Double_t rapArrRFBNum[] = {0., 0.9, 1.5, 1.93};// for rap dist.
	const Int_t nRapRFB = sizeof(rapArrRFBNum)/sizeof(Double_t)-1;
	cout << "nRapRFB = " << nRapRFB << endl;

	TH1D* h1D_RFB[nbinsY]; //byHand	
	int rap_init = 2;

	// actual RFB calculation
	double tmpFWval[nRapRFB], tmpBWval[nRapRFB]; 
	double tmpFWerr[nRapRFB], tmpBWerr[nRapRFB];
	double actval[nRapRFB], acterr[nRapRFB];
	for (Int_t ipt = 0; ipt < nbinsY; ipt++) {
		if (ipt != lowpt_init && ipt != highpt_init) continue;
		cout << ipt <<"th pt -- actual RFB *** " << endl;
		h1D_RFB[ipt] = new TH1D(Form("h1D_RFB_%d",ipt),Form("h1D_RFB_%d",ipt),nRapRFB,rapArrRFBNum);
		h1D_RFB[ipt]->Sumw2();
		for (int iy=0; iy <nRapRFB; iy ++ ){ 
			cout << iy<< "th RFB calcul" <<endl;
			tmpFWval[iy] = h1D_corrY_tot[ipt]->GetBinContent(nRap-iy); 
			tmpFWerr[iy] = h1D_corrY_tot[ipt]->GetBinError(nRap-iy); 
			tmpBWval[iy] = h1D_corrY_tot[ipt]->GetBinContent(rap_init+iy+1); 
			tmpBWerr[iy] = h1D_corrY_tot[ipt]->GetBinError(rap_init+iy+1); 
			DivideValue(tmpFWval[iy],tmpFWerr[iy],tmpBWval[iy],tmpBWerr[iy],&actval[iy],&acterr[iy]);
			h1D_RFB[ipt]->SetBinContent(nRapRFB-iy,actval[iy]);	
			h1D_RFB[ipt]->SetBinError(nRapRFB-iy,acterr[iy]);	
		}
	}	
	
	//////////////////////////////////////////////////////////////////

	//TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
	TLegend *legUR = new TLegend(0.52, 0.75, 0.86, 0.92); //upper left
	//TLegend *legBL = new TLegend(0.17,0.20,0.51,0.42,NULL,"brNDC");
	//TLegend *legBL = new TLegend(0.17,0.17,0.42,0.38,NULL,"brNDC");
	TLegend *legBL = new TLegend(0.16,0.17,0.42,0.38,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legBL);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	// --- Draw histograms
		
	// ---- prompt
	TCanvas* c1 = new TCanvas("c1","c1",200,10,600,600);
	c1->cd();
	
	// --- scaling for drawing norm.
//	h1D_RFB[lowpt_init]->Scale(scalePR_lowpt);
//	h1D_RFB[highpt_init]->Scale(scalePR_highpt);
	string ptArr_lowpt;
	string ptArr_highpt;
	formPtArr(ptArrNum[lowpt_init], ptArrNum[highpt_init], &ptArr_lowpt);
	formPtArr(ptArrNum[highpt_init], ptArrNum[nbinsY], &ptArr_highpt);
	
	// draw hist
	/*
	gPad->SetLogy(0);
	SetHistStyle(h1D_RFB[lowpt_init],lowpt_init,0);
	SetHistStyle(h1D_RFB[highpt_init],highpt_init,0);
	h1D_RFB[lowpt_init]->GetXaxis()->SetTitle("|y_{CM}|");
	h1D_RFB[lowpt_init]->GetXaxis()->CenterTitle();
	h1D_RFB[lowpt_init]->GetYaxis()->SetTitle("R_{FB}");
	h1D_RFB[lowpt_init]->GetYaxis()->SetRangeUser(0.,1.3);
	h1D_RFB[lowpt_init]->Draw("pe");
	h1D_RFB[highpt_init]->Draw("pe same");
	dashedLine(0.,1.,1.93,1.,1,1);
	if (isPrompt) legBL -> SetHeader("Prompt J/ #psi");
	else legBL -> SetHeader("Non-prompt J/ #psi");
	legBL -> AddEntry(h1D_RFB[lowpt_init],Form("%s",ptArr_lowpt.c_str()));
	legBL -> AddEntry(h1D_RFB[highpt_init],Form("%s",ptArr_highpt.c_str()));
	legBL->Draw();
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.57, 0.34, lumistring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.57, 0.27, beamstring.c_str());
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	

	c1->SaveAs(Form("RFB_%s/RFB_rap_isPrompt%d_%s.pdf",dirName,(int)isPrompt,runstring.c_str()));
	legBL->Clear();
	*/

	//////////////////////////////////////////////
	/// convert to TGraphErrors	
	//sys_lowpt
	TGraphAsymmErrors* gRFB_sys_lowpt = new TGraphAsymmErrors(h1D_RFB[lowpt_init]); 
	gRFB_sys_lowpt->SetName("gRFB_sys_lowpt");
	for (int iy=0; iy <nRapRFB; iy ++ ){ 
		gRFB_sys_lowpt->SetPointError(iy, exsys[iy], exsys[iy], eysys_lowpt[iy], eysys_lowpt[iy]);
	}	
	gRFB_sys_lowpt->GetXaxis()->SetTitle("|y_{CM}|");	
	gRFB_sys_lowpt->GetXaxis()->CenterTitle();	
	gRFB_sys_lowpt->GetYaxis()->SetTitle("R_{FB}");	
	gRFB_sys_lowpt->GetXaxis()->SetLimits(0.,1.93);	
	gRFB_sys_lowpt->SetMinimum(0.0);	
	gRFB_sys_lowpt->SetMaximum(1.4);	
	gRFB_sys_lowpt->SetFillColor(kRed-9);	
	gRFB_sys_lowpt->Draw("A2");
	//sys_highpt
	TGraphAsymmErrors* gRFB_sys_highpt = new TGraphAsymmErrors(h1D_RFB[highpt_init]); 
	gRFB_sys_highpt->SetName("gRFB_sys_highpt");
	for (int iy=0; iy <nRapRFB; iy ++ ){ 
		gRFB_sys_highpt->SetPointError(iy, exsys[iy], exsys[iy], eysys_highpt[iy], eysys_highpt[iy]);
	}	
	gRFB_sys_highpt->SetFillColor(kTeal+7);
	gRFB_sys_highpt->Draw("2");
	//lowpt
	TGraphAsymmErrors* gRFB_lowpt = new TGraphAsymmErrors(h1D_RFB[lowpt_init]); 
	gRFB_lowpt->SetName("gRFB_lowpt");
	for (int iy=0; iy <nRapRFB; iy ++ ){ 
		gRFB_lowpt->	SetPointEXlow(iy, ex[iy]);
		gRFB_lowpt->	SetPointEXhigh(iy, ex[iy]);
		gRFB_lowpt->GetPoint(iy, pxtmp_lowpt[iy], pytmp_lowpt[iy]);
		eytmp[iy] = gRFB_lowpt-> GetErrorY(iy);
		cout << "pytmp_lowpt["<<iy<<"] = " << pytmp_lowpt[iy]<<endl;
		cout << "eytmp_lowpt["<<iy<<"] = " << eytmp[iy]<<endl;
	}	
	SetGraphStyle(gRFB_lowpt, 1, 3);
	gRFB_lowpt->SetMarkerSize(1.2);
	gRFB_lowpt->Draw("P");	
	//highpt
	TGraphAsymmErrors* gRFB_highpt = new TGraphAsymmErrors(h1D_RFB[highpt_init]); 
	gRFB_highpt->SetName("gRFB_highpt");
	for (int iy=0; iy <nRapRFB; iy ++ ){ 
		gRFB_highpt->	SetPointEXlow(iy, ex[iy]);
		gRFB_highpt->	SetPointEXhigh(iy, ex[iy]);
		gRFB_highpt->GetPoint(iy, pxtmp_highpt[iy], pytmp_highpt[iy]);
		eytmp[iy] = gRFB_highpt-> GetErrorY(iy);
		cout << "pytmp_highpt["<<iy<<"] = " << pytmp_highpt[iy]<<endl;
		cout << "eytmp_highpt["<<iy<<"] = " << eytmp[iy]<<endl;
	}	
	SetGraphStyle(gRFB_highpt, 0, 5);
	gRFB_highpt->SetMarkerSize(1.9);
	gRFB_highpt->Draw("P");	
	
	dashedLine(0.,1.,1.93,1.,1,1);
	if (isPrompt) legBL -> SetHeader("Prompt J/#psi");
	else legBL -> SetHeader("Non-prompt J/#psi");
	legBL->SetTextSize(0.037);
	legBL -> AddEntry(gRFB_lowpt,Form("%s",ptArr_lowpt.c_str()));
	legBL -> AddEntry(gRFB_highpt,Form("%s",ptArr_highpt.c_str()));
	legBL->Draw();
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.54, 0.35, cmsstring.c_str());	
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.56, 0.27, beamstring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.56, 0.20, lumistring.c_str());

	c1->SaveAs(Form("RFB_%s/RFB_rap_isPrompt%d_%s.pdf",dirName,(int)isPrompt,runstring.c_str()));
	
	///////////////////////////////////////////////////////////////////
	// save as a root file
	TFile *outFile = new TFile(Form("RFB_%s/RFB_rap_isPrompt%d.root",dirName,(int)isPrompt),"RECREATE");
	outFile->cd();
	gRFB_sys_lowpt->Write();	
	gRFB_lowpt->Write();	
	gRFB_sys_highpt->Write();	
	gRFB_highpt->Write();	
	outFile->Close();
	
	
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
		*arr = Form("%.0f < p_{T} < %.0f [GeV/c]", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f [GeV/c]", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f [GeV/c]", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f [GeV/c]", binmin, binmax);
	}
}


