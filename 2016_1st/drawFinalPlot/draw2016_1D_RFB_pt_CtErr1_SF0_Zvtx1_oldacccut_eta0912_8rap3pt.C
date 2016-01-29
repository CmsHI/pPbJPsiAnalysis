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

#include "CMS_lumi.h"
#include "KYOcommonOptFinal2016.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

//// runCode // 0=merged, 1=1stRun, 2=2ndRun
void draw2016_1D_RFB_pt_CtErr1_SF0_Zvtx1_oldacccut_eta0912_8rap3pt(char* dirName = "8rap3pt", int runCode=0, bool isPrompt=true)
{
	//gROOT->Macro("./JpsiStyleForFinalResult.C");
	gROOT->Macro("./tdrstyle_kyo.C");
	gStyle->SetTitleSize(0.046, "XYZ");
  gStyle->SetEndErrorSize(0);
// Margins:
/*
//  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.132); //KYO
  gStyle->SetPadLeftMargin(0.132); //KYO
  //gStyle->SetPadRightMargin(0.04);
*/	
	gStyle->SetOptTitle(0);
  gStyle->SetPadTopMargin(0.075);
  gStyle->SetPadBottomMargin(0.13); //KYO
  gStyle->SetPadLeftMargin(0.14); //KYO
  gStyle->SetPadRightMargin(0.04);
  //gStyle->SetPadRightMargin(0.075);
	gStyle->SetTitleXOffset(1.0);
	gStyle->SetTitleYOffset(1.11);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	
	writeExtraText = true;
	extraText  = "Preliminary";
	lumi_502TeV  = "34.6 nb^{-1}";
	int iPeriod = 0; 
	int iPos=0;

	// set info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	Double_t lumi_nb;
	Double_t lumi_nb_err;
	Double_t lumi_mub;
	Double_t lumi_mub_err;
	string runstring;
	string lumistring;
	if (runCode ==0) { runstring = "All"; lumi_nb =34.622; lumi_nb_err=1.2; }
	else if (runCode == 1) { runstring = "Pbp"; lumi_nb =20.7; lumi_nb_err=0.7; }//1stRun
	else if (runCode == 2) { runstring = "pPb"; lumi_nb = 14.0; lumi_nb_err=0.5; }//2ndRun
	else { cout << " *** Error!!! choose runCode 0, 1, or 2 " << endl; return ; }
	lumistring = Form("L_{int} = %.1f nb^{  -1}", lumi_nb);
	lumi_mub = lumi_nb * 1000; // (nb)^{-1} -> {#mub}^{-1}
	lumi_mub_err = lumi_nb_err * 1000; // (nb)^{-1} -> {#mub}^{-1}
	
	/////////////////////////////////////////////////////////////////////////
	// bin center & systematic uncertainties by hand
	const int nRap = 8;	
	//const int nPt = 9;
	const int nPt = 3;
	const int nRapTmp = nRap + 1;
	const int nPtTmp = nPt + 1;
	const int nRapRFB = 3;	
	const int nPtRFB = 3;
	Double_t pxtmp[nRapRFB][nPtRFB]; //x point to fill temporary
	Double_t pytmp[nRapRFB][nPtRFB]; //y point to fill temporary
	Double_t eytmp[nRapRFB][nPtRFB]; //y point error to fill temporary
	Double_t px[nRapRFB][nPtRFB] = { // x point
		{5.740347, 7.90416, 13.21239}, // 1.5-1.93
		{-531, 7.95946, 13.31027}, //0.9-1.5
		{-531, 8.25131, 13.61970} // 0.0-0.9
	};
	Double_t ex[nPtRFB] = {0.,0.,0.}; // x stat error
	Double_t exsys[nPtRFB] = {0.25,0.25,0.25}; // x sys err
	Double_t eysys[nRapRFB][nPtRFB]; //absolute y sys error
	Double_t eysysrel[nRapRFB][nPtRFB]; //relative y sys error
	Double_t eysysrelPR[nRapRFB][nPtRFB] = {
		{0.04529, 0.04795, 0.06486},//1.5-1.93
		{0.00, 0.04245, 0.03360},//0.9-1.5
		{0.00, 0.04694, 0.03705} //0.0-0.9
	};
	Double_t eysysrelNP[nRapRFB][nPtRFB] = {
		{0.06133, 0.10487, 0.09280},
		{0.00, 0.06288, 0.05592},
		{0.00, 0.07412, 0.04483}
	};	
	for (int iy=0; iy<nRapRFB; iy++){
		for (int ipt=0; ipt<nPtRFB; ipt++){
			if(isPrompt) eysysrel[iy][ipt] = eysysrelPR[iy][ipt];
			else eysysrel[iy][ipt] = eysysrelNP[iy][ipt];
		}
	}
	
	////y_CM array (from forward to backward)
	Double_t rapArrNumFB[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	//Double_t rapArrNumBF[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1];
		cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
	}
	//pt array
	//Double_t ptArrNum[nPtTmp] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
	Double_t ptArrNum[nPtTmp] = {5.0, 6.5, 10., 30.};
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
	//TFile * f2D = new TFile(Form("../000_fittingResult/total2Dhist_%s_noWeight.root",dirName));
	//TFile * f2D = new TFile(Form("../fittingResult/total2Dhist_%s.root",dirName));
	//TFile * f2D = new TFile(Form("../fittingResult/total2Dhist_%s_noPtWeight.root",dirName));
	//TFile * f2D = new TFile(Form("../fittingResult/total2Dhist_%s_noPtWeight_CtErr1_SF0_Zvtx1.root",dirName));
	TFile * f2D = new TFile(Form("../fittingResult/total2Dhist_%s_noPtWeight_CtErr1_SF0_Zvtx1_oldacccut_eta0912.root",dirName));
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
	TH1D* h1D_corrY_Pbp[nRap]; 
	TH1D* h1D_corrY_pPb[nRap]; 
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nRap; iy++) {
		h1D_corrY_Pbp[iy] = h2D_corrY_Pbp->ProjectionY(Form("h1D_corrY_Pbp_%d",iy),iy+1,iy+1);
		//for 2nd run
		h1D_corrY_pPb[iy] = h2D_corrY_pPb->ProjectionY(Form("h1D_corrY_pPb_%d",iy),nRap-iy,nRap-iy);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	// 1) merge Pbp+pPb corrected yield

	TH1D* h1D_corrY_tot[nRap]; 
	
	for (Int_t iy = 0; iy < nRap; iy++) {
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
		for (int ipt=0; ipt <nPt; ipt ++ ){ 
			cout << ipt <<"th pt" << endl;
			cout << "h1D_corrY_tot = " << h1D_corrY_tot[iy]->GetBinContent(ipt+1)<<endl; 
		}
	}
	
	//////////////////////////////////////////////////////////////////
	/////////// calculate RFB

	TH1D* h1D_RFB_tmp[nRap]; // corrYield with merged pT		
	TH1D* h1D_RFB[nRapRFB]; // actual RFB		
	
	// bin settingg
	string rapAbsArr[nRap];
	for (Int_t iy=0; iy<nRapRFB; iy++) {
		formAbsRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapAbsArr[iy]);
		cout << iy <<"th rapAbsArr = " << rapAbsArr[iy] << endl;
	}
	Double_t ptArrRFBNum[] = {5.0, 6.5, 10., 30.};
	
	// merging pT bins 8rap3pt (KYO - byHand)
	double actPRval01, actPRerr01, actPRval02, actPRerr02, actPRval03, actPRerr03;
	for (int iy=0; iy<nRapRFB*2; iy++){
		h1D_RFB_tmp[iy]= new TH1D(Form("h1D_RFB_tmp_%d",iy),Form("h1D_RFB_tmp_%d",iy),nPtRFB,ptArrRFBNum);
		h1D_RFB_tmp[iy]->Sumw2();
		actPRval01=0; actPRval02=0; actPRval03=0; actPRerr01=0; actPRerr02=0; actPRerr03=0;
		// 1) pT 5-6.5 GeV
		if (iy==0 || iy==nRapRFB*2-1){
			actPRval01=h1D_corrY_tot[iy]->GetBinContent(1);	
			actPRerr01=h1D_corrY_tot[iy]->GetBinError(1);	
			h1D_RFB_tmp[iy]->SetBinContent(1,actPRval01);
			h1D_RFB_tmp[iy]->SetBinError(1,actPRerr01);
		} else {
			h1D_RFB_tmp[iy]->SetBinContent(1,0.);
			h1D_RFB_tmp[iy]->SetBinError(1,0.);
		}
		// 2) pT 6.5-10. GeV
		actPRval02=h1D_corrY_tot[iy]->GetBinContent(2);	
		actPRerr02=h1D_corrY_tot[iy]->GetBinError(2);	
		h1D_RFB_tmp[iy]->SetBinContent(2,actPRval02);
		h1D_RFB_tmp[iy]->SetBinError(2,actPRerr02);
		// 3) pT 10.-30. GeV
		actPRval03=h1D_corrY_tot[iy]->GetBinContent(3);	
		actPRerr03=h1D_corrY_tot[iy]->GetBinError(3);	
		h1D_RFB_tmp[iy]->SetBinContent(3,actPRval03);
		h1D_RFB_tmp[iy]->SetBinError(3,actPRerr03);
		for (int ipt=0; ipt<nPtRFB; ipt++){
			cout << iy<<"th rap, pT merged PR corrYield = " << h1D_RFB_tmp[iy]->GetBinContent(ipt+1) <<endl;
		}
		cout << "h1D_RFB_tmp[iy] = "<<h1D_RFB_tmp[iy]<<endl;
	}

/*
	// merging pT bins 8rap9pt (KYO - byHand)
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
		tmpPRerr02=h1D_corrY_tot[iy]->GetBinError(6);	
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
*/

	//////////////////////////////////////////////////////////////////
	// actual RFB calculation	
	for (int iy=0; iy<nRapRFB; iy++){
		h1D_RFB[iy] = (TH1D*)h1D_RFB_tmp[iy]->Clone(Form("h1D_RFB_%d",iy));
		h1D_RFB[iy]->Divide(h1D_RFB_tmp[2*nRapRFB-iy-1]); 
		cout << "h1D_RFB[iy] = "<<h1D_RFB[iy]<<endl;
	}
	
	//////////////////////////////////////////////////////////////////

	//TLegend *legUL = new TLegend(0.16, 0.65, 0.45, 0.92, NULL, "brNDC");
	//TLegend *legUL = new TLegend(0.16, 0.71, 0.45, 0.93, NULL, "brNDC");
	TLegend *legUL = new TLegend(0.16, 0.73, 0.45, 0.94, NULL, "brNDC");
	TLegend *legBL = new TLegend(0.16,0.16,0.45,0.38,NULL,"brNDC");
	TLegend *legUR = new TLegend(0.54, 0.70, 0.87, 0.88); //upper left
	//TLegend *legBR = new TLegend(0.54, 0.17, 0.87, 0.39); //upper left
	TLegend *legBR = new TLegend(0.51, 0.17, 0.71, 0.38); //upper left
	SetLegendStyle(legUL);
	SetLegendStyle(legBL);
	SetLegendStyle(legUR);
	SetLegendStyle(legBR);
	legBR->SetTextSize(0.05);
	legBL->SetTextSize(0.037);
	 	
	//globtex box for beam, rapidity, pT info
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

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
			//abs err calcul.
			eysys[iy][ipt]=eysysrel[iy][ipt]*pytmp[iy][ipt];
			gRFB_sys[iy]->SetPointError(ipt, exsys[ipt], exsys[ipt], eysys[iy][ipt], eysys[iy][ipt]);
		}
	}
	gRFB_sys[0]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gRFB_sys[0]->GetXaxis()->SetTitleSize(0.058);
	gRFB_sys[0]->GetXaxis()->SetLabelSize(0.05);
	gRFB_sys[0]->GetXaxis()->SetTitleFont(42);
	gRFB_sys[0]->GetXaxis()->CenterTitle();
	gRFB_sys[0]->GetYaxis()->SetTitle("R_{FB}");
	gRFB_sys[0]->GetYaxis()->SetTitleSize(0.058);
	gRFB_sys[0]->GetYaxis()->SetLabelSize(0.05);
	gRFB_sys[0]->GetYaxis()->SetTitleFont(42);
	gRFB_sys[0]->GetYaxis()->CenterTitle();

	gRFB_sys[0]->GetXaxis()->SetLimits(0.,17.0);
	gRFB_sys[0]->SetMinimum(0.5);
	gRFB_sys[0]->SetMaximum(1.15);
	gRFB_sys[0]->SetFillColor(kTeal-9);
	gRFB_sys[0]->Draw("A2");
	//gRFB_sys[1]->SetFillColor(kRed-9);
	gRFB_sys[1]->SetFillColor(kRed-10);
	gRFB_sys[1]->Draw("2");
	//gRFB_sys[2]->SetFillColor(kAzure-9);
	gRFB_sys[2]->SetFillColor(kBlue-10);
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
			cout << "eysys["<<iy<<"]["<<ipt<<"] = " << eysys[iy][ipt]<<endl;
			cout << "" << endl;
			gRFB[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gRFB[iy]->SetPointEXlow(ipt, ex[iy]);
			gRFB[iy]->SetPointEXhigh(ipt, ex[iy]);
//			if (iy!=0) gRFB[iy]->SetPoint(0, exsys[ipt], exsys[ipt], eysys[iy][ipt], eysys[iy][ipt]);
		}
	}
	SetGraphStyle(gRFB[0], 0, 5); //1.5-1.93
	gRFB[0]->SetFillColor(kTeal-9);
	gRFB[0]->SetFillStyle(1001);
	SetGraphStyle(gRFB[1], 1, 3); //0.9-1.5
	gRFB[1]->SetFillColor(kRed-10);
	gRFB[1]->SetFillStyle(1001);
	SetGraphStyle(gRFB[2], 2, 0); //0-0.9
	gRFB[2]->SetFillColor(kBlue-10);
	gRFB[2]->SetFillStyle(1001);
	gRFB[0]->SetMarkerSize(3.3);
	//gRFB[0]->Draw("P");
	gRFB[1]->Draw("P");
	gRFB[2]->Draw("P");
	gRFB[0]->Draw("P");

	dashedLine(0.,1.,17.,1.,1,1);
//		if (isPrompt) legBR -> SetHeader("Prompt J/#psi");
//		else legBR -> SetHeader("Non-prompt J/#psi");
	for (Int_t iy = fw_init; iy < nRapRFB; iy++) {
//		legBR -> AddEntry(gRFB[iy],Form("%s",rapAbsArr[iy].c_str()),"lp");
//		legBR -> AddEntry(gRFB[nRapRFB-1-iy],Form("%s",rapAbsArr[nRapRFB-1-iy].c_str()),"lp");
		legBR -> AddEntry(gRFB[nRapRFB-1-iy],Form("  %s",rapAbsArr[nRapRFB-1-iy].c_str()),"lpf");
	}
	legBR->Draw();
	globtex->SetTextSize(0.06);
	//globtex->SetTextFont(62);
	//if (isPrompt) globtex->DrawLatex(0.92, 0.85, "Prompt J/#psi");
	//else globtex->DrawLatex(0.92, 0.85, "Non-prompt J/#psi");
	if (isPrompt) globtex->DrawLatex(0.22, 0.85, "Prompt J/#psi");
	else globtex->DrawLatex(0.22, 0.85, "Non-prompt J/#psi");

	CMS_lumi( c1, iPeriod, iPos );
	c1->Update();
	c1->SaveAs(Form("RFB_%s_CtErr1_SF0_Zvtx1_oldacccut_eta0912/RFB_pt_isPrompt%d_%s.pdf",dirName,(int)isPrompt,runstring.c_str()));
	c1->SaveAs(Form("RFB_%s_CtErr1_SF0_Zvtx1_oldacccut_eta0912/RFB_pt_isPrompt%d_%s.png",dirName,(int)isPrompt,runstring.c_str()));
	
	///////////////////////////////////////////////////////////////////
	// save as a root file
	TFile *outFile = new TFile(Form("RFB_%s_CtErr1_SF0_Zvtx1_oldacccut_eta0912/RFB_pt_isPrompt%d.root",dirName,(int)isPrompt),"RECREATE");
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
  if( iPosX == 0  ) relPosX = 0.15; // KYO
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








