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
#include <TGraph.h>
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

#include "CMS_lumi.h"
#include "KYOcommonOptFinal.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);
void formEtArr(Double_t min, Double_t max, string* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

//// runCode // 0=merged, 1=1stRun, 2=2ndRun
void draw_1D_RFB_ETHF_FWonly(char* dirName = "6rap3pt", int runCode=0, bool isZoomIn = true, bool isPrompt=false)
{
	//gROOT->Macro("./JpsiStyleForFinalResult.C");
	gROOT->Macro("./tdrstyle_kyo.C");
	gStyle->SetTitleSize(0.046, "XYZ");
  gStyle->SetEndErrorSize(0);
// Margins:
//  gStyle->SetPadBottomMargin(0.132); //KYO
//  gStyle->SetPadLeftMargin(0.132); //KYO
	gStyle->SetOptTitle(0);
  gStyle->SetPadTopMargin(0.075);
  gStyle->SetPadBottomMargin(0.13); //KYO
  gStyle->SetPadLeftMargin(0.13); //KYO
  gStyle->SetPadRightMargin(0.075);
	gStyle->SetTitleXOffset(1.15);
	gStyle->SetTitleYOffset(1.22);
	
	writeExtraText = true;
	extraText  = "Preliminary";
	lumi_502TeV  = "34.6 nb^{-1}";
	int iPeriod = 0; 
	int iPos=0;

	//double pxshift = 0.5;
	double pxshift = 1.;

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
	//in=0 : pT 5-6.5, rap 1.5-1.93 
	//in=1 : pT 6.5-30, rap 1.5-1.93 
	//in=2 : pT 6.5-30, rap 0.9-1.5 
	//in=3 : pT 6.5-30, rap 0.0-0.9 
	const int ntmp = 3;
	Double_t px[ntmp]; //x point
	Double_t pxtmp[4][ntmp]; //x point
	Double_t pytmp[4][ntmp]; //y point
	Double_t eytmp[4][ntmp]; //y point error
	Double_t ex[ntmp]; // x error
	Double_t exsys[ntmp]; //sys x error
	Double_t eysysrel[4][ntmp]; //sysrel y error
	Double_t eysys[4][ntmp]; //sys y error
	px = {9.4, 24.3, 37.2}; 
	ex = {0.,0.,0.};
	//exsys = {1.0,1.0,1.0};
	exsys = {0.5, 0.5, 0.5};
	if (isPrompt) {
		eysysrel[0] = {
		0.05299,
		0.06155,
		0.06715}; //1.5-1.93 low
		eysysrel[1] = {
		0.05088,
		0.05442,
		0.05913}; //1.5-1.93
		eysysrel[2] = {
		0.03741,
		0.04958,
		0.04227}; //0.9-1.5
		eysysrel[3] = {
		0.04425,
		0.04528,
		0.05314}; //0.0-0.9
	}
	else {
		eysysrel[0] = {
		0.04971,
		0.06138,
		0.06459}; // 1.5-1.93 low
		eysysrel[1] = {
		0.05188,
		0.05442,
		0.05999}; //0.5-1.93
		eysysrel[2] = {
		0.03694,
		0.04952,
		0.04221}; //0.9-1.5
		eysysrel[3] = {
		0.04242,
		0.04349,
		0.05162}; //0.0-0.9
	}	

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
		}
	}
	
	//////////////////////////////////////////////////////////////////
	/////////// calculate RFB
	
	const int nRapRFB = 3;	
	TH1D* h1D_RFB_tmp[nEtBins][nbinsX]; // corrYield with merged pT		
	TH1D* h1D_RFB[nEtBins][nRapRFB]; // actual RFB vs pt	
	
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
			h1D_RFB_tmp[iet][iy]= new TH1D(Form("h1D_RFB_tmp_%d_%d",iet,iy),Form("h1D_RFB_tmp_%d_%d",iet,iy),nPtRFB,ptArrRFBNum);
			h1D_RFB_tmp[iet][iy]->Sumw2();
			actPRval01=0; actPRval02=0; actPRerr01=0; actPRerr02=0;
			// 1) pT 5-6.5 GeV
			tmpPRval01=0; tmpPRval02=0; tmpPRerr01=0; tmpPRerr02=0;
			if (iy==0 || iy==nRapRFB*2-1){
				actPRval01=h1D_corrY_tot[iet][iy]->GetBinContent(1);	
				actPRerr01=h1D_corrY_tot[iet][iy]->GetBinError(1);	
				h1D_RFB_tmp[iet][iy]->SetBinContent(1,actPRval01);
				h1D_RFB_tmp[iet][iy]->SetBinError(1,actPRerr01);
			} else {
				h1D_RFB_tmp[iet][iy]->SetBinContent(1,0.);
				h1D_RFB_tmp[iet][iy]->SetBinError(1,0.);
			}
			// 2) pT 6.5-30. GeV
			tmpPRval01=0; tmpPRval02=0; tmpPRerr01=0; tmpPRerr02=0;
			tmpPRval01=h1D_corrY_tot[iet][iy]->GetBinContent(2);	
			tmpPRerr01=h1D_corrY_tot[iet][iy]->GetBinError(2);	
			tmpPRval02=h1D_corrY_tot[iet][iy]->GetBinContent(3);	
			tmpPRerr02=h1D_corrY_tot[iet][iy]->GetBinError(3);	
			actPRval02=tmpPRval01+tmpPRval02;
			actPRerr02=TMath::Sqrt( TMath::Power(tmpPRerr01,2) + TMath::Power(tmpPRerr02,2) );
			h1D_RFB_tmp[iet][iy]->SetBinContent(2,actPRval02);
			h1D_RFB_tmp[iet][iy]->SetBinError(2,actPRerr02);
		}
	}

	// actual RFB calculation	vs pT!!
	for (int iet=0; iet<nEtBins; iet++){
		for (int iy=0; iy<nRapRFB; iy++){
			h1D_RFB[iet][iy] = (TH1D*)h1D_RFB_tmp[iet][iy]->Clone(Form("h1D_RFB_%d_%d",iet,iy));
			h1D_RFB[iet][iy]->Divide(h1D_RFB_tmp[iet][2*nRapRFB-iy-1]); 
			cout << iet<<"th, "<<iy<<"th h1D_RFB = "<<h1D_RFB[iet][iy]<<endl;
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
	TH1D* h1D_RFB_ETHF[nHist];
	
	for (int in=0; in< nHist; in++){
		h1D_RFB_ETHF[in]= new TH1D(Form("h1D_RFB_ETHF_%d",in),Form("h1D_RFB_ETHF_%d",in),nEtBins,etArrNum);
		h1D_RFB_ETHF[in]->Sumw2();
		tmpRFBval01=0;tmpRFBerr01=0;tmpRFBval02=0;tmpRFBerr02=0;tmpRFBval03=0;tmpRFBerr03=0;
		for (int iet=0; iet<nEtBins;iet++){
			if (in==0) {
				tmpRFBval01=h1D_RFB[iet][0]->GetBinContent(1);	
				tmpRFBerr01=h1D_RFB[iet][0]->GetBinError(1);	
				h1D_RFB_ETHF[in]->SetBinContent(iet+1,tmpRFBval01);
				h1D_RFB_ETHF[in]->SetBinError(iet+1,tmpRFBerr01);
			}			
			else {
				tmpRFBval01=h1D_RFB[iet][in-1]->GetBinContent(2);	
				tmpRFBerr01=h1D_RFB[iet][in-1]->GetBinError(2);	
				h1D_RFB_ETHF[in]->SetBinContent(iet+1,tmpRFBval01);
				h1D_RFB_ETHF[in]->SetBinError(iet+1,tmpRFBerr01);
			}
		}
	}		
	
	//////////////////////////////////////////////////////////////////
	TCanvas* c1 = new TCanvas("c1","c1",600,600);
	c1->cd();
	
//	TLegend *legBL = new TLegend(0.16, 0.16, 0.43, 0.36); //bottom left
//	TLegend *legBL = new TLegend(0.16, 0.16, 0.43, 0.35); //bottom left
	TLegend *legBL = new TLegend(0.16, 0.16, 0.43, 0.41); //bottom left
//	TLegend *legUR = new TLegend(0.36, 0.77, 0.89, 0.95); //upper left
	TLegend *legUR = new TLegend(0.34, 0.71, 0.89, 0.88); //upper left
	SetLegendStyle(legBL);	
	SetLegendStyle(legUR);	
	//legBL->SetTextSize(0.032);
	legBL->SetTextSize(0.037);
	
	//globtex box for beam, rapidity, pT info
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	//globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

	// convert to TGraphAsymErrors
	//RFB
	TGraphAsymmErrors*gRFB[nHist]; 
	for (int in=0; in< nHist; in++){
		gRFB[in] = new TGraphAsymmErrors(h1D_RFB_ETHF[in]);
		gRFB[in]->SetName(Form("gRFB_%d",in));
		for (int iet=0; iet<nEtBins;iet++){
			gRFB[in]->GetPoint(iet, pxtmp[in][iet], pytmp[in][iet]);
			eytmp[in][iet] = gRFB[in] -> GetErrorY(iet);
//			gRFB[in]->SetPoint(iet, px[iet], pytmp[in][iet]);
			gRFB[in]->SetPoint(iet, px[iet]+pxshift*in, pytmp[in][iet]);
			gRFB[in]->SetPointEXlow(iet, ex[in]);
			gRFB[in]->SetPointEXhigh(iet, ex[in]);
		}
	}

	//sys
	TGraphAsymmErrors* gRFB_sys[nHist];
	for (int in=0; in< nHist; in++){
		gRFB_sys[in] = new TGraphAsymmErrors(h1D_RFB_ETHF[in]);
		gRFB_sys[in]->SetName(Form("gRFB_sys_%d",in));
		for (int iet=0; iet<nEtBins;iet++){
			gRFB_sys[in]->GetPoint(iet, pxtmp[in][iet], pytmp[in][iet]);
			//abs err calcul.
			eysys[in][iet] = eysysrel[in][iet]*pytmp[in][iet];
			//gRFB_sys[in]->SetPoint(iet, px[iet], pytmp[in][iet]);
			gRFB_sys[in]->SetPoint(iet, px[iet]+pxshift*in, pytmp[in][iet]);
			gRFB_sys[in]->SetPointError(iet, exsys[iet], exsys[iet], eysys[in][iet], eysys[in][iet]);
			cout << "" << endl;
			cout << "pytmp["<<in<<"]["<<iet<<"] = " << pytmp[in][iet]<<endl;
			cout << "eytmp["<<in<<"]["<<iet<<"] = " << eytmp[in][iet]<<endl;
			cout << "eysys["<<in<<"]["<<iet<<"] = " << eysys[in][iet]<<endl;
		}
	}
	
	gRFB_sys[0]->GetXaxis()->SetTitle("E_{T}^{HF |#eta|>4} [GeV]");
	gRFB_sys[0]->GetXaxis()->CenterTitle();
	gRFB_sys[0]->GetYaxis()->SetTitle("R_{FB}");
	gRFB_sys[0]->GetXaxis()->SetLimits(0.,50.0);
//	gRFB_sys[0]->GetXaxis()->SetLimits(0.,55.0);
	if (isZoomIn) {
//		gRFB_sys[0]->SetMinimum(0.5);
		gRFB_sys[0]->SetMinimum(0.4);
//		gRFB_sys[0]->SetMaximum(1.25);
		gRFB_sys[0]->SetMaximum(1.15);
	}
	else {
		gRFB_sys[0]->SetMinimum(0.0);
		gRFB_sys[0]->SetMaximum(1.4);
	}
	gRFB_sys[0]->SetFillColor(kViolet-9);
	gRFB_sys[0]->Draw("A2");
	gRFB_sys[1]->SetFillColor(kTeal+7);
	gRFB_sys[1]->Draw("2");
//	gRFB_sys[2]->SetFillColor(kRed-9);
//	gRFB_sys[2]->Draw("2");
//	gRFB_sys[3]->SetFillColor(kAzure-9);
//	gRFB_sys[3]->Draw("2");

	SetGraphStyle(gRFB[0], 8, 2); //1.5-1.93 low
	SetGraphStyle(gRFB[1], 0, 5); //1.5-1.93
	SetGraphStyle(gRFB[2], 1, 3); //0.9-1.5
	SetGraphStyle(gRFB[3], 2, 0); //0-0.9
	gRFB[0]->SetMarkerSize(1.6);
	gRFB[0]->Draw("P");
	gRFB[1]->SetMarkerSize(2.1);
	gRFB[1]->Draw("P");
//	gRFB[2]->Draw("P");
//	gRFB[3]->Draw("P");

	dashedLine(0.,1.,50.,1.,1,1);
	legBL -> SetHeader ("1.5 < |y_{CM}| < 1.93");
	legBL -> AddEntry(gRFB[0],"5 < p_{T} < 6.5 GeV/c","lp");
	legBL -> AddEntry(gRFB[1],"6.5 < p_{T} < 30 GeV/c","lp");
//	legBL -> AddEntry(gRFB[0],"1.5 < |y_{CM}| < 1.93,   5 < p_{T} < 6.5 GeV/c","lp");
//	legBL -> AddEntry(gRFB[1],"1.5 < |y_{CM}| < 1.93,  6.5 < p_{T} < 30 GeV/c","lp");
//	legBL -> AddEntry(gRFB[2],"0.9 < |y_{CM}| < 1.5,   6.5 < p_{T} < 30 GeV/c","lp");
//	legBL -> AddEntry(gRFB[3],"0.0 < |y_{CM}| < 0.9,   6.5 < p_{T} < 30 GeV/c","lp");
	legBL->Draw();
	globtex->SetTextSize(0.045);
	globtex->SetTextFont(62);
	if (isPrompt) globtex->DrawLatex(0.88, 0.86, "Prompt J/#psi");
	else globtex->DrawLatex(0.88, 0.86, "Non-prompt J/#psi");

	CMS_lumi( c1, iPeriod, iPos );
	c1->Update();
	c1->SaveAs(Form("RFB_%s/RFB_ETHF_FWonly_isPrompt%d_%s.pdf",dirName,(int)isPrompt,runstring.c_str()));
	c1->SaveAs(Form("RFB_%s/RFB_ETHF_FWonly_isPrompt%d_%s.png",dirName,(int)isPrompt,runstring.c_str()));

	#if 0
	///////////////////////////////////////////////////////////////////
	// save as a root file
	TFile *outFile = new TFile(Form("RFB_%s/RFB_ETHF_FWonly_isPrompt%d.root",dirName,(int)isPrompt),"RECREATE");
	outFile->cd();
	for (int in=0; in< nHist; in++){
		gRFB_sys[in]->Write();	
		gRFB[in]->Write();	
	}
	outFile->Close();
	#endif

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


void CMS_lumi( TPad* pad, int iPeriod, int iPosX )
{            
  bool outOfFrame    = false;
  if( iPosX/10==0 ) 
    {
      outOfFrame = true;
    }
  int alignY_=3;
  int alignX_=2;
  if( iPosX/10==0 ) alignX_=1;
  if( iPosX==0    ) alignX_=1;
  if( iPosX==0    ) alignY_=1;
  if( iPosX/10==1 ) alignX_=1;
  if( iPosX/10==2 ) alignX_=2;
  if( iPosX/10==3 ) alignX_=3;
  //if( iPosX == 0  ) relPosX = 0.12;
  if( iPosX == 0  ) relPosX = 0.15;
  int align_ = 10*alignX_ + alignY_;

  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  //  float e = 0.025;

  pad->cd();

  TString lumiText;
  if( iPeriod==0 )
    {
      lumiText += lumi_502TeV;
      lumiText += " (pPb 5.02 TeV)";
    }
   
  cout << lumiText << endl;

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);    

  float extraTextSize = extraOverCmsTextSize*cmsTextSize;

  latex.SetTextFont(42);
  latex.SetTextAlign(31); 
  latex.SetTextSize(lumiTextSize*t);    
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

  if( outOfFrame )
    {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextAlign(11); 
      latex.SetTextSize(cmsTextSize*t);    
      latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
    }
  
  pad->cd();

  float posX_=0;
  if( iPosX%10<=1 )
    {
      posX_ =   l + relPosX*(1-l-r);
    }
  else if( iPosX%10==2 )
    {
      posX_ =  l + 0.5*(1-l-r);
    }
  else if( iPosX%10==3 )
    {
      posX_ =  1-r - relPosX*(1-l-r);
    }
  float posY_ = 1-t - relPosY*(1-t-b);
  if( !outOfFrame )
    {
      if( drawLogo )
	{
	  posX_ =   l + 0.045*(1-l-r)*W/H;
	  posY_ = 1-t - 0.045*(1-t-b);
	  float xl_0 = posX_;
	  float yl_0 = posY_ - 0.15;
	  float xl_1 = posX_ + 0.15*H/W;
	  float yl_1 = posY_;
	  TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
	  TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
	  pad_logo->Draw();
	  pad_logo->cd();
	  CMS_logo->Draw("X");
	  pad_logo->Modified();
	  pad->cd();
	}
      else
	{
	  latex.SetTextFont(cmsTextFont);
	  latex.SetTextSize(cmsTextSize*t);
	  latex.SetTextAlign(align_);
	  latex.DrawLatex(posX_, posY_, cmsText);
	  if( writeExtraText ) 
	    {
	      latex.SetTextFont(extraTextFont);
	      latex.SetTextAlign(align_);
	      latex.SetTextSize(extraTextSize*t);
	      latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
	    }
	}
    }
  else if( writeExtraText )
    {
      if( iPosX==0) 
	{
	  posX_ =   l +  relPosX*(1-l-r);
	  posY_ =   1-t+lumiTextOffset*t;
	}
      latex.SetTextFont(extraTextFont);
      latex.SetTextSize(extraTextSize*t);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, extraText);      
    }
  return;
}









