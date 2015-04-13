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
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
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
void draw_1D_RFB_pt(char* dirName = "8rap9pt", int runCode=0, bool isZoomIn = true, bool isPrompt=false)
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
	// bin center & systematic uncertainties by hand
	// for 1.5 - 1.93 
	const int ntmp = 3;
	Double_t px[3][ntmp]; //x point
	Double_t pxtmp[3][ntmp]; //x point
	Double_t pytmp[3][ntmp]; //y point
	Double_t eytmp[3][ntmp]; //y point error
	Double_t ex[ntmp]; // x error
	Double_t exsys[ntmp]; //sys x error
	Double_t eysys[3][ntmp]; //sys y error
	px[0] = {5.740347, 7.90416, 13.21239}; // 1.5-1.93
	px[1] = {-531, 7.95946, 13.31027}; //0.9-1.5
	px[2] = {-531, 8.25131, 13.61970}; //0.0-0.9
	ex = {0.,0.,0.};
	exsys = {0.4,0.4,0.4};
	if (isPrompt) {
		//1.5-1.93
		eysys[0] = {
		0.026101147,
		0.035871392,
		0.055155744};
		//0.9-1.5
		eysys[1] = {
		0.00,
		0.03050345,
		0.034713623};
		//0.0-0.9
		eysys[2] = {
		0.00, 
		0.03971931,
		0.040430809};
	}
	else {
		//1.5-1.93
		eysys[0] = {
		0.056049769,
		0.07101411,
		0.080530222};
		//0.9-1.5
		eysys[1] = {
		0.00,
		0.069039457,
		0.04044452};
		//0.0-0.9
		eysys[2] = {
		0.00, 
		0.053426749,
		0.044291073};
	}	

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
	TH1D* h1D_corrY_Pbp[nbinsX]; 
	TH1D* h1D_corrY_pPb[nbinsX]; 
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_corrY_Pbp[iy] = h2D_corrY_Pbp->ProjectionY(Form("h1D_corrY_Pbp_%d",iy),iy+1,iy+1);
		//for 2nd run
		h1D_corrY_pPb[iy] = h2D_corrY_pPb->ProjectionY(Form("h1D_corrY_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	// 1) merge Pbp+pPb corrected yield

	TH1D* h1D_corrY_tot[nbinsX]; 
	
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		// --- merging
		if (runCode ==0) {	
			h1D_corrY_tot[iy] = (TH1D*)h1D_corrY_Pbp[iy]->Clone(Form("h1D_corrY_tot_%d",iy));
			h1D_corrY_tot[iy]->Add(h1D_corrY_pPb[iy]);	
		}
		else if (runCode ==1) {
			h1D_corrY_tot[iy] = (TH1D*)h1D_corrY_Pbp[iy]->Clone(Form("h1D_corrY_tot_%d",iy));
		}
		else if (runCode ==2) {
			h1D_corrY_tot[iy] = (TH1D*)h1D_corrY_pPb[iy]->Clone(Form("h1D_corrY_tot_%d",iy));
		}
		cout <<" "<<endl;
		cout <<iy<<"th rap !!!"<<endl;
		for (int ipt=0; ipt <nbinsY; ipt ++ ){ 
			cout << ipt <<"th pt" << endl;
			cout << "h1D_corrY_tot = " << h1D_corrY_tot[iy]->GetBinContent(ipt+1)<<endl; 
		}
	}
	
	//////////////////////////////////////////////////////////////////
	/////////// calculate RFB

	const int nRapRFB = 3;	
	TH1D* h1D_RFB_tmp[nbinsX]; // corrYield with merged pT		
	TH1D* h1D_RFB[nRapRFB]; // actual RFB		
	
	// bin settingg
	string rapAbsArr[nRap];
	for (Int_t iy=0; iy<nRapRFB; iy++) {
		formAbsRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapAbsArr[iy]);
		cout << iy <<"th rapAbsArr = " << rapAbsArr[iy] << endl;
	}
	Double_t ptArrRFBNum[] = {5.0, 6.5, 10., 30.};
	const Int_t nPtRFB = sizeof(ptArrRFBNum)/sizeof(Double_t)-1;
	cout << "nPtRFB = " << nPtRFB << endl;

	// merging pT bins (KYO - byHand)
	double tmpPRval01, tmpPRerr01, tmpPRval02, tmpPRerr02, tmpPRval03, tmpPRerr03;
	double actPRval01, actPRerr01, actPRval02, actPRerr02, actPRval03, actPRerr03;
	for (int iy=0; iy<nRapRFB*2; iy++){
		h1D_RFB_tmp[iy]= new TH1D(Form("h1D_RFB_tmp_%d",iy),Form("h1D_RFB_tmp_%d",iy),nPtRFB,ptArrRFBNum);
		h1D_RFB_tmp[iy]->Sumw2();
		actPRval01=0; actPRval02=0; actPRval03=0; actPRerr01=0; actPRerr02=0; actPRerr03=0;
		// 1) pT 5-6.5 GeV
		tmpPRval01=0; tmpPRval02=0;tmpPRval03=0; tmpPRerr01=0; tmpPRerr02=0;tmpPRerr03=0;
		if (iy==0 || iy==nRapRFB*2-1){
			actPRval01=h1D_corrY_tot[iy]->GetBinContent(4);	
			actPRerr01=h1D_corrY_tot[iy]->GetBinError(4);	
			h1D_RFB_tmp[iy]->SetBinContent(1,actPRval01);
			h1D_RFB_tmp[iy]->SetBinError(1,actPRerr01);
		} else {
			h1D_RFB_tmp[iy]->SetBinContent(1,0.);
			h1D_RFB_tmp[iy]->SetBinError(1,0.);
		}
		// 2) pT 6.5-10. GeV
		tmpPRval01=0; tmpPRval02=0;tmpPRval03=0; tmpPRerr01=0; tmpPRerr02=0;tmpPRerr03=0;
		tmpPRval01=h1D_corrY_tot[iy]->GetBinContent(5);	
		tmpPRerr01=h1D_corrY_tot[iy]->GetBinError(5);	
		tmpPRval02=h1D_corrY_tot[iy]->GetBinContent(6);	
		tmpPRerr02=h1D_corrY_tot[iy]->GetBinError(7);	
		tmpPRval03=h1D_corrY_tot[iy]->GetBinContent(7);	
		tmpPRerr03=h1D_corrY_tot[iy]->GetBinError(7);	
		actPRval02=tmpPRval01+tmpPRval02+tmpPRval03;
		actPRerr02=TMath::Sqrt( TMath::Power(tmpPRerr01,2) + TMath::Power(tmpPRerr02,2) +TMath::Power(tmpPRerr03,2) );
		h1D_RFB_tmp[iy]->SetBinContent(2,actPRval02);
		h1D_RFB_tmp[iy]->SetBinError(2,actPRerr02);
		// 3) pT 10.-30. GeV
		tmpPRval01=0; tmpPRval02=0;tmpPRval03=0; tmpPRerr01=0; tmpPRerr02=0;tmpPRerr03=0;
		tmpPRval01=h1D_corrY_tot[iy]->GetBinContent(8);	
		tmpPRerr01=h1D_corrY_tot[iy]->GetBinError(8);	
		tmpPRval02=h1D_corrY_tot[iy]->GetBinContent(9);	
		tmpPRerr02=h1D_corrY_tot[iy]->GetBinError(9);	
		actPRval03=tmpPRval01+tmpPRval02;
		actPRerr03=TMath::Sqrt( TMath::Power(tmpPRerr01,2) + TMath::Power(tmpPRerr02,2) );
		h1D_RFB_tmp[iy]->SetBinContent(3,actPRval03);
		h1D_RFB_tmp[iy]->SetBinError(3,actPRerr03);
		for (int ipt=0; ipt<nPtRFB; ipt++){
			cout << iy<<"th rap, pT merged PR corrYield = " << h1D_RFB_tmp[iy]->GetBinContent(ipt+1) <<endl;
		}
		cout << "h1D_RFB_tmp[iy] = "<<h1D_RFB_tmp[iy]<<endl;
	}

	//////////////////////////////////////////////////////////////////
	// actual RFB calculation	
	for (int iy=0; iy<nRapRFB; iy++){
		h1D_RFB[iy] = (TH1D*)h1D_RFB_tmp[iy]->Clone(Form("h1D_RFB_%d",iy));
		h1D_RFB[iy]->Divide(h1D_RFB_tmp[2*nRapRFB-iy-1]); 
		cout << "h1D_RFB[iy] = "<<h1D_RFB[iy]<<endl;
	}
	
	//////////////////////////////////////////////////////////////////

	//TLegend *legUL = new TLegend(0.16, 0.65, 0.45, 0.92, NULL, "brNDC");
	TLegend *legUL = new TLegend(0.16, 0.71, 0.45, 0.93, NULL, "brNDC");
	TLegend *legBL = new TLegend(0.16,0.16,0.45,0.38,NULL,"brNDC");
	SetLegendStyle(legUL);
	SetLegendStyle(legBL);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	TCanvas* c1 = new TCanvas("c1","c1",600,600);
	c1->cd();
	
	//////////////////////////////////////////////////////////////////
	// convert to TGraphAsymErrors
	const int fw_init = 0;
	
	//sys
	TGraphAsymmErrors*gRFB_sys[nRapRFB]; 
	for (int iy=0; iy<nRapRFB; iy++){
		gRFB_sys[iy] = new TGraphAsymmErrors(h1D_RFB[iy]);
		gRFB_sys[iy] -> SetName(Form("gRFB_sys_%d",iy));
		for (int ipt=0; ipt<nPtRFB; ipt++){
			gRFB_sys[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			gRFB_sys[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gRFB_sys[iy]->SetPointError(ipt, exsys[ipt], exsys[ipt], eysys[iy][ipt], eysys[iy][ipt]);
		}
	}
	gRFB_sys[0]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gRFB_sys[0]->GetXaxis()->CenterTitle();
	gRFB_sys[0]->GetYaxis()->SetTitle("R_{FB}");
	gRFB_sys[0]->GetXaxis()->SetLimits(0.,20.0);
	if (isZoomIn) {
		gRFB_sys[0]->SetMinimum(0.5);
		gRFB_sys[0]->SetMaximum(1.25);
	} 
	else {
		gRFB_sys[0]->SetMinimum(0.0);
		gRFB_sys[0]->SetMaximum(1.4);
	}
	gRFB_sys[0]->SetFillColor(kTeal+7);
	gRFB_sys[0]->Draw("A2");
	gRFB_sys[1]->SetFillColor(kRed-9);
	gRFB_sys[1]->Draw("2");
	gRFB_sys[2]->SetFillColor(kAzure-9);
	gRFB_sys[2]->Draw("2");

	//RFB
	TGraphAsymmErrors*gRFB[nRapRFB]; 
	for (int iy=0; iy<nRapRFB; iy++){
		gRFB[iy] = new TGraphAsymmErrors(h1D_RFB[iy]);
		gRFB[iy] -> SetName(Form("gRFB_%d",iy));
		for (int ipt=0; ipt<nPtRFB; ipt++){
			gRFB[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			eytmp[iy][ipt] = gRFB[iy]-> GetErrorY(ipt);
			cout << "pytmp["<<iy<<"]["<<ipt<<"] = " << pytmp[iy][ipt]<<endl;
			cout << "eytmp["<<iy<<"]["<<ipt<<"] = " << eytmp[iy][ipt]<<endl;
			gRFB[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gRFB[iy]->SetPointEXlow(ipt, ex[iy]);
			gRFB[iy]->SetPointEXhigh(ipt, ex[iy]);
//			if (iy!=0) gRFB[iy]->SetPoint(0, exsys[ipt], exsys[ipt], eysys[iy][ipt], eysys[iy][ipt]);
		}
	}
	SetGraphStyle(gRFB[0], 0, 5); //1.5-1.93
	SetGraphStyle(gRFB[1], 1, 3); //0.9-1.5
	SetGraphStyle(gRFB[2], 2, 0); //0-0.9
	gRFB[0]->SetMarkerSize(1.9);
	gRFB[0]->Draw("P");
	gRFB[1]->Draw("P");
	gRFB[2]->Draw("P");

	dashedLine(0.,1.,20.,1.,1,1);
	if (isZoomIn) {
		if (isPrompt) legUL -> SetHeader("Prompt J/#psi");
		else legUL -> SetHeader("Non-prompt J/#psi");
		legUL->SetTextSize(0.037);
		for (Int_t iy = fw_init; iy < nRapRFB; iy++) {
			legUL -> AddEntry(gRFB[iy],Form("%s",rapAbsArr[iy].c_str()),"lp");
		}
		legUL->Draw();
	} 
	else {
		if (isPrompt) legBL -> SetHeader("Prompt J/#psi");
		else legBL -> SetHeader("Non-prompt J/#psi");
		legBL->SetTextSize(0.037);
		for (Int_t iy = fw_init; iy < nRapRFB; iy++) {
			legBL -> AddEntry(gRFB[iy],Form("%s",rapAbsArr[iy].c_str()),"lp");
		}
		legBL->Draw();
	}
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.53, 0.35, cmsstring.c_str());	
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.55, 0.27, beamstring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.55, 0.20, lumistring.c_str());

	c1->SaveAs(Form("RFB_%s/RFB_pt_isPrompt%d_%s.pdf",dirName,(int)isPrompt,runstring.c_str()));
	
	///////////////////////////////////////////////////////////////////
	// save as a root file
	TFile *outFile = new TFile(Form("RFB_%s/RFB_pt_isPrompt%d.root",dirName,(int)isPrompt),"RECREATE");
	outFile->cd();
	for (int iy=0; iy<nRapRFB; iy++){
		gRFB_sys[iy]->Write();	
		gRFB[iy]->Write();	
	}
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
	if ( binmax==1.93 ) { //tmp
		*arr = Form("%.1f < |y_{CM}| < %.2f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.1f", binmin, binmax);
	} else {
		*arr = Form("%.1f < |y_{CM}| < %.1f", binmin, binmax);
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


