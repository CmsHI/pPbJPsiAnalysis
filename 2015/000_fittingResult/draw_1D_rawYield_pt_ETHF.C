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
void formEtArr(Double_t min, Double_t max, string* arr);

//// runCode // 0=merged, 1=1stRun, 2=2ndRun
void draw_1D_rawYield_pt_ETHF(char* dirName = "6rap3pt", int runCode=1, bool isScale = false,bool isLog = false, bool isPrompt=true)
{
	gROOT->Macro("./JpsiStyle.C");
	//TCanvas *ctmp0 = new TCanvas("ctmp0","RFB vs ET",900,600);
	//ctmp0->Divide(3,2);	
	
	TCanvas *ctmp0 = new TCanvas("ctmp0","ET 0-20",900,600);
	ctmp0->Divide(3,2);	
	TCanvas *ctmp1 = new TCanvas("ctmp1","ET 20-30",900,600);
	ctmp1->Divide(3,2);	
	TCanvas *ctmp2 = new TCanvas("ctmp2","ET30-120",900,600);
	ctmp2->Divide(3,2);	
	

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
	
	//rap array in yCM (from forward to backward)
	//Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	//Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93};// for pt dist.
	//Double_t rapArrNumBF[] = {-1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	const Int_t nRap = sizeof(rapArrNumFB)/sizeof(Double_t)-1;
	cout << "nRap = " << nRap << endl;
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
		cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
	}
	//pt array
	//Double_t ptArrNum[] = {0.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
	Double_t ptArrNum[] = {5.0, 6.5, 10., 30.}; //6rap3pt
	const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;
	cout << "nPt = " << nPt << endl;
	Double_t ptBinW[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
		cout << ipt <<"th ptBinW = " << ptBinW[ipt] <<endl;
	}
	//ethf array
	Double_t etArrNum[] = {0.0, 20.0, 30.0, 120.0};
	const Int_t nEtBins = sizeof(etArrNum)/sizeof(double)-1;
	cout << "nEtBins=" << nEtBins << endl;

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
	string etArr[nEtBins];
	for (Int_t i=0; i<nEtBins; i++){
		formEtArr(etArrNum[i], etArrNum[i+1], &etArr[i]);
		cout << "etArr["<<i<<"] = "<< etArr[i].c_str() << endl;
	}	
	
	// --- read-in file
	TFile * f2D = new TFile(Form("../000_fittingResult/total2Dhist_%s.root",dirName));
	//TFile * f2D = new TFile(Form("/home/songkyo/kyo/2015/000_fittingResult/total2Dhist_%s.root",dirName));
	cout << "dirName = " << dirName << endl;
	cout << "runCode = " << runCode << ", runstring = " << runstring.c_str() << endl;

	// --- read-in 2D hist for corrected yield
	TH2D* h2D_corrY_Pbp[nEtBins];
	TH2D* h2D_corrY_pPb[nEtBins];
	for (int iet=0; iet<nEtBins; iet++){
		if (isPrompt) {
			h2D_corrY_Pbp[iet] = (TH2D*)f2D->Get(Form("h2D_corrY_PR_Pbp_%d",iet));
			h2D_corrY_pPb[iet] = (TH2D*)f2D->Get(Form("h2D_corrY_PR_pPb_%d",iet));
		}
		else {
			h2D_corrY_Pbp[iet] = (TH2D*)f2D->Get(Form("h2D_corrY_NP_Pbp_%d",iet));
			h2D_corrY_pPb[iet] = (TH2D*)f2D->Get(Form("h2D_corrY_NP_pPb_%d",iet));
		}
		cout << iet << "th h2D_corrY_Pbp = " << h2D_corrY_Pbp[iet] << endl;
		cout << iet << "th h2D_corrY_pPb = " << h2D_corrY_pPb[iet] << endl;
	}	

	const int nbinsX = h2D_corrY_Pbp[0]->GetNbinsX();
	const int nbinsY = h2D_corrY_Pbp[0]->GetNbinsY();
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };

	// ---  projection to 1D hist
	TH1D* h1D_corrY_Pbp[nEtBins][nbinsX]; 
	TH1D* h1D_corrY_pPb[nEtBins][nbinsX]; 
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iet=0; iet<nEtBins; iet++){
		for (Int_t iy = 0; iy < nbinsX; iy++) {
			h1D_corrY_Pbp[iet][iy] = h2D_corrY_Pbp[iet]->ProjectionY(Form("h1D_corrY_Pbp_%d_%d",iet,iy),iy+1,iy+1);
			h1D_corrY_Pbp[iet][iy]->SetName(Form("h1D_corrY_Pbp_%d_%d",iet,iy));
			//for 2nd run
			h1D_corrY_pPb[iet][iy] = h2D_corrY_pPb[iet]->ProjectionY(Form("h1D_corrY_pPb_%d_%d",iet,iy),nbinsX-iy,nbinsX-iy);
			h1D_corrY_pPb[iet][iy]->SetName(Form("h1D_corrY_pPb_%d_%d",iet,iy));
			
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// set values as zero for unused bins
	for (Int_t iet=0; iet<nEtBins; iet++){
		for (Int_t iy = 0; iy < nbinsX; iy++) {
			if (iy>=1 && iy<=4) {
				h1D_corrY_Pbp[iet][iy]->SetBinContent(1,0);
				h1D_corrY_Pbp[iet][iy]->SetBinError(1,0);
				h1D_corrY_pPb[iet][iy]->SetBinContent(1,0);
				h1D_corrY_pPb[iet][iy]->SetBinError(1,0);
			}
		}
	}

	 ////// after zero-bin setting, normalize!
	for (Int_t iet=0; iet<nEtBins; iet++){
		for (Int_t iy = 0; iy < nbinsX; iy++) {
			h1D_corrY_Pbp[iet][iy]->Scale(1./h1D_corrY_Pbp[iet][iy]->Integral());	
			h1D_corrY_pPb[iet][iy]->Scale(1./h1D_corrY_pPb[iet][iy]->Integral());	
			h1D_corrY_Pbp[iet][iy]->Scale(1,"width");
			h1D_corrY_pPb[iet][iy]->Scale(1,"width");
		}
	}

	//////////////////////////////////////////////////////////////////
	//// --- Draw histograms
	TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92); //upper left	
	TLegend *legBL = new TLegend(0.18, 0.18, 0.59, 0.35); //upper left
	SetLegendStyle(legUR);
	SetLegendStyle(legBL);

	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);	

	for (Int_t iet=0; iet<nEtBins; iet++){
		for (Int_t iy = 0; iy < nbinsX; iy++) {
			if (iet==0) { ctmp0->cd(iy+1);}
			else if (iet==1) { ctmp1->cd(iy+1); }
			else if (iet==2) { ctmp2->cd(iy+1); }
			 
			SetHistStyle(h1D_corrY_Pbp[iet][iy],1,0);
			SetHistStyle(h1D_corrY_pPb[iet][iy],2,0);
			h1D_corrY_Pbp[iet][iy]->Draw("pe"); 
			h1D_corrY_pPb[iet][iy]->Draw("pe same"); 
			//h1D_corrY_pPb[iet][iy]->Draw("pe"); 
		}
	}



/*	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	// 1) merge Pbp+pPb corrected yield
	TH1D* h1D_corrY_tot[nEtBins][nbinsX]; 
	for (Int_t iet=0; iet<nEtBins; iet++){
		for (Int_t iy = 0; iy < nbinsX; iy++) {
			if (runCode ==0) {	
				h1D_corrY_tot[iet][iy] = (TH1D*)h1D_corrY_Pbp[iet][iy]->Clone(Form("h1D_corrY_tot_%d_%d",iet,iy));
				h1D_corrY_tot[iet][iy]->Add(h1D_corrY_pPb[iet][iy]);	
			}
			else if (runCode ==1) {
				h1D_corrY_tot[iet][iy] = (TH1D*)h1D_corrY_Pbp[iet][iy]->Clone(Form("h1D_corrY_tot_%d_%d",iet,iy));
			}
			else if (runCode ==2) {
				h1D_corrY_tot[iet][iy] = (TH1D*)h1D_corrY_pPb[iet][iy]->Clone(Form("h1D_corrY_tot_%d_%d",iet,iy));
			}
//			if (iet==0) { ctmp0->cd(iy+1); h1D_corrY_tot[iet][iy]->Draw("pe"); }
//			if (iet==1) { ctmp1->cd(iy+1); h1D_corrY_tot[iet][iy]->Draw("pe"); }
//			if (iet==2) { ctmp2->cd(iy+1); h1D_corrY_tot[iet][iy]->Draw("pe"); }
		}
	}
	
	//////////////////////////////////////////////////////////////////
	/////////// calculate RFB
	
	const int nRapRFB = 3;	
	TH1D* h1D_RFB_tot_tmp[nEtBins][nbinsX]; // corrYield with merged pT		
	TH1D* h1D_RFB_tot[nEtBins][nRapRFB]; // actual RFB vs pt	
	
	// bin settingg
	string rapAbsArr[nRap];
	for (Int_t iy=0; iy<nRapRFB; iy++) {
		formAbsRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapAbsArr[iy]);
		cout << iy <<"th rapAbsArr = " << rapAbsArr[iy] << endl;
	}
	Double_t ptArrRFBNum[] = {5.0, 6.5, 30.};
	const Int_t nPtRFB = sizeof(ptArrRFBNum)/sizeof(Double_t)-1;
	cout << "nPtRFB = " << nPtRFB << endl;

	// merging pT bins (KYO - byHand)
	double tmpPRval01, tmpPRerr01, tmpPRval02, tmpPRerr02;
	double actPRval01, actPRerr01, actPRval02, actPRerr02;
	for (int iet=0; iet<nEtBins; iet++){
		for (int iy=0; iy<nRapRFB*2; iy++){
			h1D_RFB_tot_tmp[iet][iy]= new TH1D(Form("h1D_RFB_tot_tmp_%d_%d",iet,iy),Form("h1D_RFB_tot_tmp_%d_%d",iet,iy),nPtRFB,ptArrRFBNum);
			h1D_RFB_tot_tmp[iet][iy]->Sumw2();
			actPRval01=0; actPRval02=0; actPRerr01=0; actPRerr02=0;
			// 1) pT 5-6.5 GeV
			tmpPRval01=0; tmpPRval02=0; tmpPRerr01=0; tmpPRerr02=0;
			if (iy==0 || iy==nRapRFB*2-1){
				actPRval01=h1D_corrY_tot[iet][iy]->GetBinContent(1);	
				actPRerr01=h1D_corrY_tot[iet][iy]->GetBinError(1);	
				h1D_RFB_tot_tmp[iet][iy]->SetBinContent(1,actPRval01);
				h1D_RFB_tot_tmp[iet][iy]->SetBinError(1,actPRerr01);
			} else {
				h1D_RFB_tot_tmp[iet][iy]->SetBinContent(1,0.);
				h1D_RFB_tot_tmp[iet][iy]->SetBinError(1,0.);
			}
			// 2) pT 6.5-30. GeV
			tmpPRval01=0; tmpPRval02=0; tmpPRerr01=0; tmpPRerr02=0;
			tmpPRval01=h1D_corrY_tot[iet][iy]->GetBinContent(2);	
			tmpPRerr01=h1D_corrY_tot[iet][iy]->GetBinError(2);	
			tmpPRval02=h1D_corrY_tot[iet][iy]->GetBinContent(3);	
			tmpPRerr02=h1D_corrY_tot[iet][iy]->GetBinError(3);	
			actPRval02=tmpPRval01+tmpPRval02;
			actPRerr02=TMath::Sqrt( TMath::Power(tmpPRerr01,2) + TMath::Power(tmpPRerr02,2) );
			h1D_RFB_tot_tmp[iet][iy]->SetBinContent(2,actPRval02);
			h1D_RFB_tot_tmp[iet][iy]->SetBinError(2,actPRerr02);
			//cout << iet<<"th,"<<iy<<"th h1D_RFB_tot_tmp = "<<h1D_RFB_tot_tmp[iet][iy]<<endl;
//			if (iet==0) { ctmp0->cd(iy+1); h1D_RFB_tot_tmp[iet][iy]->Draw("pe"); }
//			if (iet==1) { ctmp1->cd(iy+1); h1D_RFB_tot_tmp[iet][iy]->Draw("pe"); }
//			if (iet==2) { ctmp2->cd(iy+1); h1D_RFB_tot_tmp[iet][iy]->Draw("pe"); }
		}
	}

	// actual RFB calculation	vs pT!!
	for (int iet=0; iet<nEtBins; iet++){
		for (int iy=0; iy<nRapRFB; iy++){
			h1D_RFB_tot[iet][iy] = (TH1D*)h1D_RFB_tot_tmp[iet][iy]->Clone(Form("h1D_RFB_tot_%d_%d",iet,iy));
			h1D_RFB_tot[iet][iy]->Divide(h1D_RFB_tot_tmp[iet][2*nRapRFB-iy-1]); 
			cout << iet<<"th, "<<iy<<"th h1D_RFB_tot = "<<h1D_RFB_tot[iet][iy]<<endl;
			//if (iet==0) { ctmp0->cd(iy+1); h1D_RFB_tot[iet][iy]->Draw("pe"); }
			//if (iet==1) { ctmp1->cd(iy+1); h1D_RFB_tot[iet][iy]->Draw("pe"); }
			//if (iet==2) { ctmp2->cd(iy+1); h1D_RFB_tot[iet][iy]->Draw("pe"); }
		}
	}	

	///////////////////////////////////////////////
	//////// --- RFB vs ETHF : 4 histograms
	//in=0 : pT 5-6.5, rap 1.5-1.93 
	//in=1 : pT 6.5-30, rap 1.5-1.93 
	//in=2 : pT 6.5-30, rap 0.9-1.5 
	//in=3 : pT 6.5-30, rap 0.0-0.9 
	const int nHist=4; 
	double tmpRFBval01, tmpRFBerr01, tmpRFBval02, tmpRFBerr02, tmpRFBval03, tmpRFBerr03;
	TH1D* h1D_RFB_ETHF_tot[nHist];
	
	for (int in=0; in< nHist; in++){
		h1D_RFB_ETHF_tot[in]= new TH1D(Form("h1D_RFB_ETHF_tot_%d",in),Form("h1D_RFB_ETHF_tot_%d",in),nEtBins,etArrNum);
		h1D_RFB_ETHF_tot[in]->Sumw2();
		tmpRFBval01=0;tmpRFBerr01=0;tmpRFBval02=0;tmpRFBerr02=0;tmpRFBval03=0;tmpRFBerr03=0;
		for (int iet=0; iet<nEtBins;iet++){
			if (in==0) {
				tmpRFBval01=h1D_RFB_tot[iet][0]->GetBinContent(1);	
				tmpRFBerr01=h1D_RFB_tot[iet][0]->GetBinError(1);	
				h1D_RFB_ETHF_tot[in]->SetBinContent(iet+1,tmpRFBval01);
				h1D_RFB_ETHF_tot[in]->SetBinError(iet+1,tmpRFBerr01);
			}			
			else {
				tmpRFBval01=h1D_RFB_tot[iet][in-1]->GetBinContent(2);	
				tmpRFBerr01=h1D_RFB_tot[iet][in-1]->GetBinError(2);	
				h1D_RFB_ETHF_tot[in]->SetBinContent(iet+1,tmpRFBval01);
				h1D_RFB_ETHF_tot[in]->SetBinError(iet+1,tmpRFBerr01);
			}
		}
		 h1D_RFB_ETHF_tot[in]->SetMinimum(0.0);
		 h1D_RFB_ETHF_tot[in]->SetMaximum(1.4);
		ctmp0->cd(in+1); h1D_RFB_ETHF_tot[in]->Draw("pe");
	dashedLine(0.,1.,120.,1.,1,1);
	}		
*/













	// convert to TGraphAsymErrors




/*
	// root file
	//TFile *fOut = new TFile(Form("1Dhist_%s/tmp_crossSection_pt.root",dirName),"RECREATE");
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

void formEtArr(Double_t min, Double_t max, string* arr) {
	*arr = Form("%.0f-%.0f",min,max);
}
