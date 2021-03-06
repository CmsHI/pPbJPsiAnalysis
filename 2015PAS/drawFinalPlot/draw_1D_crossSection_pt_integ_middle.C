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
void draw_1D_crossSection_pt_integ_middle(char* dirName = "8rap9pt2gev", int runCode=0, bool isScale = false,bool isLog = false)
{
	//gROOT->Macro("./JpsiStyleForFinalResult.C");
	gROOT->Macro("./tdrstyle_kyo.C");

	// pileup rejection!!
	const Double_t pileReg = 128234./123240.;
	const Double_t pileRegRelErr = 0.23;
	cout << "pileReg = " << pileReg << endl;
	
	// zvtx correction!!
	const Double_t zvtxCor = 1.064;
	//const Double_t zvtxCor = 1.; //No

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
	const int nRap = 8;
	const int nPt = 9;
	const int nRapTmp = nRap + 1;
	const int nPtTmp = nPt + 1;
	Double_t pxtmp[nRap][nPt]; //x point to fill temporary
	Double_t pytmp[nRap][nPt]; //y point to fill temporary
	Double_t eytmp[nRap][nPt]; //y point error to fill temporary
	/*
	//PAS
	Double_t px[nRap][nPt] = { // x point
		{2.49530, 3.48529, 4.47275, 5.68792, 6.9644, 7.96271, 9.1754, 11.5315, 17.7588},
		{0.00000, 3.53123, 4.5027, 5.71709, 6.96523, 7.9693, 9.17314, 11.4952, 17.6927},
		{0.00000, 0.00000, 0.00000, 0.00000, 7.01977, 7.99712, 9.19936, 11.5743, 17.7732},
		{0.00000, 0.00000, 0.00000, 0.00000, 7.12292, 8.01305, 9.22816, 11.6279, 17.8879},
		{0.00000, 0.00000, 0.00000, 0.00000, 7.05476, 8.00208, 9.21589, 11.5645, 17.7176},
		{0.00000, 0.00000, 0.00000, 5.82095, 6.97886, 7.96732, 9.18979, 11.5158, 17.4116},
		{0.00000, 3.52893, 4.48328, 5.69351, 6.96188, 7.95707, 9.14886, 11.4747, 17.231},
		{2.49481, 3.47853, 4.46938, 5.6761, 6.96419, 7.97702, 9.16158, 11.5077, 17.3061},
	};
	*/
	/*
	// new acc cut
	Double_t px[nRap][nPt] = {	
		{2.51443, 3.50757, 4.47953, 5.70624, 6.96258, 7.96788, 9.17032, 11.5446, 17.8838}, 
		{0.00000, 3.56222, 4.52616, 5.74214, 6.97103, 7.97166, 9.18028, 11.4711, 17.7127}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.01909, 7.99970, 9.19743, 11.5722, 17.7907}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.12323, 8.01502, 9.23109, 11.6318, 17.8046},  
		{0.00000, 0.00000, 0.00000, 0.00000, 7.04981, 8.00419, 9.21309, 11.5536, 17.6251}, 
		{0.00000, 0.00000, 0.00000, 5.85766, 6.98218, 7.97548, 9.19420, 11.5218, 17.4151},
		{0.00000, 3.57190, 4.51540, 5.72587, 6.97029, 7.95706, 9.14684, 11.4619, 17.1339}, 
		{2.51848, 3.49477, 4.47722, 5.69022, 6.96768, 7.98316, 9.17100, 11.4908, 17.1958}
	}; // x point
	*/
	// middle
	Double_t px[nRap][nPt] = {	
		{2.5, 3.5, 4.5, 5.75, 7, 8, 9.25, 12., 22}, 
		{0., 3.5, 4.5, 5.75, 7, 8, 9.25, 12., 22},
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22},
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22}, 
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22}, 
		{0., 0., 0., 5.75, 7, 8, 9.25, 12., 22},
		{0., 3.5, 4.5, 5.75, 7, 8, 9.25, 12., 22}, 
		{2.5, 3.5, 4.5, 5.75, 7, 8, 9.25, 12., 22}
	}; // x point
	
	Double_t ex[nPt] = {0,0,0,0,0,0,0,0,0}; // x stat err
	//Double_t exsys[nPt] = {0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4}; // x sys err
	Double_t exsys[nPt] = {0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2}; // x sys err
	Double_t eysysrel_pr[nRap][nPt]; //relative y sys error : PR 
	Double_t eysysrel_np[nRap][nPt]; //relative y sys error :NP
	Double_t eysys_pr[nRap][nPt]; //absolute y sys error : PR
	Double_t eysys_np[nRap][nPt]; //absolute y sys error :NP

	//rap array in yCM (from forward to backward)
	Double_t rapArrNumFB[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	//Double_t rapArrNumBF[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	//const Int_t nRap = sizeof(rapArrNumFB)/sizeof(Double_t)-1;
	//cout << "nRap = " << nRap << endl;
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
		cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
	}
	//pt array
	Double_t ptArrNum[nPtTmp] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.}; //8rap9pt2gev
	//const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;
	//cout << "nPt = " << nPt << endl;
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

	//////////////////////////////////////////////////////////////	
	// ----read-in sys. file 
	TFile * fSys = new TFile(Form("../totalSys/TotSys_%s.root",dirName));
	TH2D* hTotalPR = (TH2D*)fSys->Get("hTotalPR");
	TH2D* hTotalNP = (TH2D*)fSys->Get("hTotalNP");
	cout << " hTotalPR = " <<  hTotalPR << endl;	
	cout << " hTotalNP = " <<  hTotalNP << endl;	

	//////////////////////////////////////////////////////////////	
	// --- read-in corr-yield file
	TFile * f2D = new TFile(Form("../fittingResult/total2Dhist_%s.root",dirName));
	//TFile * f2D = new TFile(Form("../fittingResult/total2Dhist_%s_noPtWeight.root",dirName));
	cout << "dirName = " << dirName << endl;
	cout << "runCode = " << runCode << ", runstring = " << runstring.c_str() << endl;

	// --- read-in 2D hist for corrected yield
	TH2D* h2D_corrY_PR_Pbp = (TH2D*)f2D->Get("h2D_corrY_PR_Pbp");
	TH2D* h2D_corrY_NP_Pbp = (TH2D*)f2D->Get("h2D_corrY_NP_Pbp");
	TH2D* h2D_corrY_PR_pPb = (TH2D*)f2D->Get("h2D_corrY_PR_pPb");
	TH2D* h2D_corrY_NP_pPb = (TH2D*)f2D->Get("h2D_corrY_NP_pPb");
	cout << "h2D_corrY_PR_Pbp = " << h2D_corrY_PR_Pbp << endl;
	cout << "h2D_corrY_NP_Pbp = " << h2D_corrY_NP_Pbp << endl;
	cout << "h2D_corrY_PR_pPb = " << h2D_corrY_PR_pPb << endl;
	cout << "h2D_corrY_NP_pPb = " << h2D_corrY_NP_pPb << endl;
	const int nbinsX = h2D_corrY_PR_Pbp->GetNbinsX();
	const int nbinsY = h2D_corrY_PR_Pbp->GetNbinsY();
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };

	// ---  projection to 1D hist
	TH1D* h1D_corrY_PR_Pbp[nRap]; 
	TH1D* h1D_corrY_NP_Pbp[nRap]; 
	TH1D* h1D_corrY_PR_pPb[nRap]; 
	TH1D* h1D_corrY_NP_pPb[nRap]; 
	TH1D* h1D_syserr_PR[nRap];
	TH1D* h1D_syserr_NP[nRap];
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nRap; iy++) {
		//for 1st run
		h1D_corrY_PR_Pbp[iy] = h2D_corrY_PR_Pbp->ProjectionY(Form("h1D_corrY_PR_Pbp_%d",iy),iy+1,iy+1);
		h1D_corrY_NP_Pbp[iy] = h2D_corrY_NP_Pbp->ProjectionY(Form("h1D_corrY_NP_Pbp_%d",iy),iy+1,iy+1);
		//for sys.err (same order with 1st run)
		h1D_syserr_PR[iy] = hTotalPR->ProjectionY(Form("h1D_syserr_PR_%d",iy),iy+1,iy+1);
		h1D_syserr_NP[iy] = hTotalNP->ProjectionY(Form("h1D_syserr_NP_%d",iy),iy+1,iy+1);
		//for 2nd run
		h1D_corrY_PR_pPb[iy] = h2D_corrY_PR_pPb->ProjectionY(Form("h1D_corrY_PR_pPb_%d",iy),nRap-iy,nRap-iy);
		h1D_corrY_NP_pPb[iy] = h2D_corrY_NP_pPb->ProjectionY(Form("h1D_corrY_NP_pPb_%d",iy),nRap-iy,nRap-iy);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	// read out sysrel!	
	for (Int_t iy = 0; iy < nRap; iy++) {
		for (int ipt=0; ipt <nPt; ipt ++ ){ 
			eysysrel_pr[iy][ipt] = h1D_syserr_PR[iy]->GetBinContent(ipt+1);
			eysysrel_np[iy][ipt] = h1D_syserr_NP[iy]->GetBinContent(ipt+1);
			//cout <<iy<<"th iy, "<<ipt<<"th pt eysysrel_pr = "<<eysysrel_pr[iy][ipt]<<endl;
			//cout <<iy<<"th iy,  "<<ipt<<"th pt eysysrel_np = "<<eysysrel_np[iy][ipt]<<endl;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	// 1) merge Pbp+pPb corrected yield
	// 2) calcualte cross-section = corrY/(dPt*dY*lumi*branching)
	TH1D* h1D_cross_PR_tot[nRap]; 
	TH1D* h1D_cross_NP_tot[nRap]; 
	for (Int_t iy = 0; iy < nRap; iy++) {
		// --- merging
		if (runCode ==0) {	
			h1D_cross_PR_tot[iy] = (TH1D*)h1D_corrY_PR_Pbp[iy]->Clone(Form("h1D_cross_PR_tot_%d",iy));
			h1D_cross_PR_tot[iy]->Add(h1D_corrY_PR_pPb[iy]);	
			h1D_cross_NP_tot[iy] = (TH1D*)h1D_corrY_NP_Pbp[iy]->Clone(Form("h1D_cross_NP_tot_%d",iy));
			h1D_cross_NP_tot[iy]->Add(h1D_corrY_NP_pPb[iy]);	
		}
		else if (runCode ==1) {
			h1D_cross_PR_tot[iy] = (TH1D*)h1D_corrY_PR_Pbp[iy]->Clone(Form("h1D_cross_PR_tot_%d",iy));
			h1D_cross_NP_tot[iy] = (TH1D*)h1D_corrY_NP_Pbp[iy]->Clone(Form("h1D_cross_NP_tot_%d",iy));
		}
		else if (runCode ==2) {
			h1D_cross_PR_tot[iy] = (TH1D*)h1D_corrY_PR_pPb[iy]->Clone(Form("h1D_cross_PR_tot_%d",iy));
			h1D_cross_NP_tot[iy] = (TH1D*)h1D_corrY_NP_pPb[iy]->Clone(Form("h1D_cross_NP_tot_%d",iy));
		}
		cout <<" "<<endl;
		cout <<iy<<"th rap !!!"<<endl;
		// --- norm. pt binWidth
		h1D_cross_PR_tot[iy]->Scale(1,"width");
		h1D_cross_NP_tot[iy]->Scale(1,"width");
		// --- norm. rap binWidth
//		h1D_cross_PR_tot[iy]->Scale(1./rapBinW[iy]);
//		h1D_cross_NP_tot[iy]->Scale(1./rapBinW[iy]);
		// 1.93 for ATLAS !!!
		h1D_cross_PR_tot[iy]->Scale(1./1.93);
		h1D_cross_NP_tot[iy]->Scale(1./1.93);
		// --- norm. (lumi*br)
		h1D_cross_PR_tot[iy]->Scale(1./lumi_mub);
		h1D_cross_NP_tot[iy]->Scale(1./lumi_mub);
		// nb for ATLAS !!
//		h1D_cross_PR_tot[iy]->Scale(1./lumi_nb);
//		h1D_cross_NP_tot[iy]->Scale(1./lumi_nb);
		// no br for ATLAS!!!
//		h1D_cross_PR_tot[iy]->Scale(1./br);
//		h1D_cross_NP_tot[iy]->Scale(1./br);
		// --- pile-up correction
		h1D_cross_PR_tot[iy]->Scale(pileReg);	
		h1D_cross_NP_tot[iy]->Scale(pileReg);	
		// --- zvtx correction
		h1D_cross_PR_tot[iy]->Scale(zvtxCor);	
		h1D_cross_NP_tot[iy]->Scale(zvtxCor);	
//		cout <<" *** c) cross-section  *** "<<endl;
//		for (int ipt=0; ipt <nPt; ipt ++ ){ 
//			cout << ipt <<"th pt" << endl;
//			cout << "h1D_cross_PR_tot = " << h1D_cross_PR_tot[iy]->GetBinContent(ipt+1)<<endl; 
//			cout << "h1D_cross_NP_tot = " << h1D_cross_NP_tot[iy]->GetBinContent(ipt+1)<<endl; 
//		}
	}
		
	// set values as zero for unused bins :: pT < 6.5 for ATLAS
	const int tmpPtBin = 4; // < 6.5
	for (int iy = 0; iy < nRap; iy++) {
		for (int ipt=0; ipt < tmpPtBin; ipt ++ ){ 
			h1D_cross_PR_tot[iy]->SetBinContent(ipt+1,-532);
			h1D_cross_NP_tot[iy]->SetBinContent(ipt+1,-532);
			h1D_cross_PR_tot[iy]->SetBinError(ipt+1,0);
			h1D_cross_NP_tot[iy]->SetBinError(ipt+1,0);
		}
	}
	//////////////////////////////////////////////////////////////////

	///// merge) integrate the rapidity for ATLAS
	const int fwrap_init = 0;
	const int bwrap_init = 3;
	const int bwrap_final = 6;

	/// find the largest sys among rap bins
	for (int ipt=0; ipt < nPt; ipt ++ ){ 
		for (Int_t iy = fwrap_init+1; iy < bwrap_init; iy++) {
			if(eysysrel_pr[fwrap_init][ipt]<eysysrel_pr[iy][ipt]) eysysrel_pr[fwrap_init][ipt] = eysysrel_pr[iy][ipt];
			if(eysysrel_np[fwrap_init][ipt]<eysysrel_np[iy][ipt]) eysysrel_np[fwrap_init][ipt] = eysysrel_np[iy][ipt];
		}
		for (Int_t iy = bwrap_init+1; iy < bwrap_final; iy++) {
			if(eysysrel_pr[bwrap_init][ipt]<eysysrel_pr[iy][ipt]) eysysrel_pr[bwrap_init][ipt] = eysysrel_pr[iy][ipt];
			if(eysysrel_np[bwrap_init][ipt]<eysysrel_np[iy][ipt]) eysysrel_np[bwrap_init][ipt] = eysysrel_np[iy][ipt];
		}
		//cout << ipt << "th pt, eysysrel_pr[fwrap_init][ipt] =  " << eysysrel_pr[fwrap_init][ipt] << endl;
		//cout << ipt << "th pt, eysysrel_pr[bwrap_init][ipt] =  " << eysysrel_pr[bwrap_init][ipt] << endl;
		//cout << ipt << "th pt, eysysrel_np[fwrap_init][ipt] =  " << eysysrel_np[fwrap_init][ipt] << endl;
		//cout << ipt << "th pt, eysysrel_np[bwrap_init][ipt] =  " << eysysrel_np[bwrap_init][ipt] << endl;
	}	

	// ***  bin merging
	cout << "*** fw rap bin starts from : " << rapArr[fwrap_init].c_str() << endl;
	for (Int_t iy = fwrap_init+1; iy < bwrap_init; iy++) {
		h1D_cross_PR_tot[fwrap_init]->Add(h1D_cross_PR_tot[iy]);
		h1D_cross_NP_tot[fwrap_init]->Add(h1D_cross_NP_tot[iy]);
		cout << " *** and to fw, merging : " << rapArr[iy].c_str() << endl; 
	}
	cout << "*** bw rap bin starts from : " << rapArr[bwrap_init].c_str() << endl;
	for (Int_t iy = bwrap_init+1; iy < bwrap_final; iy++) {
		h1D_cross_PR_tot[bwrap_init]->Add(h1D_cross_PR_tot[iy]);
		h1D_cross_NP_tot[bwrap_init]->Add(h1D_cross_NP_tot[iy]);
		cout << " *** and to bw, merging : " << rapArr[iy].c_str() << endl; 
	}
		

///	TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
//	TLegend *legUR = new TLegend(0.59, 0.55, 0.86, 0.92); //upper left
	TLegend *legUR = new TLegend(0.59, 0.79, 0.86, 0.92); //upper left
	SetLegendStyle(legUR);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//global uncertainty
	TBox * globalbox = new TBox(0.5, 6.4, 1.5, 13.6);
	globalbox->SetFillColor(kYellow);
	
//	const int fw_init = 0;
//	const int bw_init = 3;

	//////////////////////////////////////////////////////////////////////////////////////
	// convert to TGraphAsymErrors
	TGraphAsymmErrors* gCross_pr_sys[nRap];	
	TGraphAsymmErrors* gCross_pr[nRap];	
	TGraphAsymmErrors* gCross_np_sys[nRap];	
	TGraphAsymmErrors* gCross_np[nRap];	
	for (Int_t iy = 0; iy < nRap; iy++) {
		gCross_pr_sys[iy] = new TGraphAsymmErrors(h1D_cross_PR_tot[iy]);
		gCross_pr[iy] = new TGraphAsymmErrors(h1D_cross_PR_tot[iy]);
		gCross_np_sys[iy] = new TGraphAsymmErrors(h1D_cross_NP_tot[iy]);
		gCross_np[iy] = new TGraphAsymmErrors(h1D_cross_NP_tot[iy]);
		gCross_pr_sys[iy]->SetName(Form("gCross_pr_sys_%d",iy));
		gCross_pr[iy]->SetName(Form("gCross_pr_%d",iy));
		gCross_np_sys[iy]->SetName(Form("gCross_np_sys_%d",iy));
		gCross_np[iy]->SetName(Form("gCross_np_%d",iy));
		for (Int_t ipt=0; ipt<nPt; ipt++ ){
			gCross_pr_sys[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			gCross_pr_sys[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			//absolute error calculation 
			eysys_pr[iy][ipt]=eysysrel_pr[iy][ipt]*pytmp[iy][ipt];
			gCross_pr_sys[iy]->SetPointError(ipt, exsys[ipt], exsys[ipt], eysys_pr[iy][ipt], eysys_pr[iy][ipt]);
			gCross_pr[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			eytmp[iy][ipt] = gCross_pr[iy]-> GetErrorY(ipt);
			//cout << "pr : pytmp["<<iy<<"]["<<ipt<<"] = " << pytmp[iy][ipt]<<endl;
			//cout << "pr : eytmp["<<iy<<"]["<<ipt<<"] = " << eytmp[iy][ipt]<<endl;
			//cout << "pr : eysys_pr["<<iy<<"]["<<ipt<<"] = " << eysys_pr[iy][ipt]<<endl;
			gCross_pr[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gCross_pr[iy]->SetPointEXlow(ipt, ex[ipt]);
			gCross_pr[iy]->SetPointEXhigh(ipt, ex[ipt]);
			
			gCross_np_sys[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			gCross_np_sys[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			//absolute error calculation 
			eysys_np[iy][ipt]=eysysrel_np[iy][ipt]*pytmp[iy][ipt];
			gCross_np_sys[iy]->SetPointError(ipt, exsys[ipt], exsys[ipt], eysys_np[iy][ipt], eysys_np[iy][ipt]);
			gCross_np[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			eytmp[iy][ipt] = gCross_np[iy]-> GetErrorY(ipt);
			//cout << "np : pytmp["<<iy<<"]["<<ipt<<"] = " << pytmp[iy][ipt]<<endl;
			//cout << "np : eytmp["<<iy<<"]["<<ipt<<"] = " << eytmp[iy][ipt]<<endl;
			//cout << "np : eysys_np["<<iy<<"]["<<ipt<<"] = " << eysys_np[iy][ipt]<<endl;
			gCross_np[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gCross_np[iy]->SetPointEXlow(ipt, ex[ipt]);
			gCross_np[iy]->SetPointEXhigh(ipt, ex[ipt]);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	////Draw
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
	////prompt fw
	TCanvas* c_pr_fw = new TCanvas("c_pr_fw","c_pr_fw",200,10,800,600);
	c_pr_fw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	//sys
	gCross_pr_sys[fwrap_init]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gCross_pr_sys[fwrap_init]->GetXaxis()->CenterTitle(1);
	gCross_pr_sys[fwrap_init]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy x BR(J/#psi #rightarrow #mu#mu) [nb/(GeV/c)]");
	if (isLog) {
		gCross_pr_sys[fwrap_init]->SetMinimum(0.01);
//		gCross_pr_sys[fwrap_init]->SetMaximum(700.);
		gCross_pr_sys[fwrap_init]->SetMaximum(2000.);
	}
	else {
		gCross_pr_sys[fwrap_init]->SetMinimum(0.0);
//		gCross_pr_sys[fwrap_init]->SetMaximum(350.);
		gCross_pr_sys[fwrap_init]->SetMaximum(1400.);
	}
	gCross_pr_sys[fwrap_init]->GetXaxis()->SetLimits(0.0, 25.);
	gCross_pr_sys[fwrap_init]->SetFillColor(kViolet-9);
	gCross_pr_sys[fwrap_init]->Draw("A2");
	//value
	SetGraphStyle(gCross_pr[fwrap_init],	8,12);
	SetGraphStyle(gCross_pr[1],	0,15);
	SetGraphStyle(gCross_pr[2],	1,13);
	gCross_pr[fwrap_init]->SetMarkerSize(1.3);
	gCross_pr[fwrap_init]->Draw("P");
	legUR -> SetHeader("Prompt J/#psi");
	legUR -> AddEntry(gCross_pr[fwrap_init],"0 < y_{CM} < 1.93","lp");
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
		//latex->DrawLatex(0.59, 0.50, "Global uncertainty : 3.6 \%");
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
		//latex->DrawLatex(0.59, 0.50, "Global uncertainty : 3.6 \%");
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_pr_fw->SaveAs(Form("cross_%s/crossSection_pt_integ_middle_pr_fw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();
	
	////prompt bw
	TCanvas* c_pr_bw = new TCanvas("c_pr_bw","c_pr_bw",200,10,800,600);
	c_pr_bw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	//sys
	gCross_pr_sys[bwrap_init]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gCross_pr_sys[bwrap_init]->GetXaxis()->CenterTitle(1);
	gCross_pr_sys[bwrap_init]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy x BR(J/#psi #rightarrow #mu#mu) [nb/(GeV/c)]");
	if (isLog) {
		gCross_pr_sys[bwrap_init]->SetMinimum(0.01);
//		gCross_pr_sys[bwrap_init]->SetMaximum(700.);
		gCross_pr_sys[bwrap_init]->SetMaximum(2000.);
	}
	else {
		gCross_pr_sys[bwrap_init]->SetMinimum(0.0);
//		gCross_pr_sys[bwrap_init]->SetMaximum(350.);
		gCross_pr_sys[bwrap_init]->SetMaximum(1400.);
	}
	gCross_pr_sys[bwrap_init]->GetXaxis()->SetLimits(0.0, 25.);
	gCross_pr_sys[bwrap_init]->SetFillColor(kViolet-9);
	gCross_pr_sys[bwrap_init]->Draw("A2");
	//value
	SetGraphStyle(gCross_pr[bwrap_init],	8,12);
	SetGraphStyle(gCross_pr[4],	0,15);
	SetGraphStyle(gCross_pr[5],	1,13);
	SetGraphStyle(gCross_pr[6],	2,10);
	SetGraphStyle(gCross_pr[7],	5,11);
	gCross_pr[bwrap_init]->SetMarkerSize(1.3);
	gCross_pr[bwrap_init]->Draw("P");
	legUR -> SetHeader("Prompt J/#psi");
	legUR -> AddEntry(gCross_pr[bwrap_init],"-1.93 < y_{CM} < 0","lp");
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
//		latex->DrawLatex(0.59, 0.50, "Global uncertainty : 3.6 \%");
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
//		latex->DrawLatex(0.59, 0.50, "Global uncertainty : 3.6 \%");
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_pr_bw->SaveAs(Form("cross_%s/crossSection_pt_integ_middle_pr_bw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();
	
	////non-prompt fw
	TCanvas* c_np_fw = new TCanvas("c_np_fw","c_np_fw",200,10,800,600);
	c_np_fw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	//sys
	gCross_np_sys[fwrap_init]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gCross_np_sys[fwrap_init]->GetXaxis()->CenterTitle(1);
	gCross_np_sys[fwrap_init]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy x BR(J/#psi #rightarrow #mu#mu) [nb/(GeV/c)]");
	if (isLog) {
		gCross_np_sys[fwrap_init]->SetMinimum(0.01);
		gCross_np_sys[fwrap_init]->SetMaximum(2000.);
	}
	else {
		gCross_np_sys[fwrap_init]->SetMinimum(0.0);
//		gCross_np_sys[fwrap_init]->SetMaximum(65.);
		gCross_np_sys[fwrap_init]->SetMaximum(500.);
	}
	gCross_np_sys[fwrap_init]->GetXaxis()->SetLimits(0.0, 25.);
	gCross_np_sys[fwrap_init]->SetFillColor(kViolet-9);
	gCross_np_sys[fwrap_init]->Draw("A2");
	gCross_np_sys[1]->SetFillColor(kTeal+7);
//	gCross_np_sys[1]->Draw("2");
	gCross_np_sys[2]->SetFillColor(kRed-9);
//	gCross_np_sys[2]->Draw("2");
	//value
	SetGraphStyle(gCross_np[fwrap_init],	8,12);
	SetGraphStyle(gCross_np[1],	0,15);
	SetGraphStyle(gCross_np[2],	1,13);
	gCross_np[fwrap_init]->SetMarkerSize(1.3);
	gCross_np[fwrap_init]->Draw("P");
	gCross_np[1]->SetMarkerSize(1.7);
//	gCross_np[1]->Draw("P");
//	gCross_np[2]->Draw("P");
	legUR -> SetHeader("Non-prompt J/#psi");
	legUR -> AddEntry(gCross_np[fwrap_init],"0 < y_{CM} < 1.93","lp");
	//for (Int_t iy = fw_init; iy < bw_init; iy++) {
	//	legUR -> AddEntry(gCross_np[iy],Form("%s",rapArr[iy].c_str()),"lp");
//	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
//		latex->DrawLatex(0.59, 0.50, "Global uncertainty : 3.6 \%");
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
//		latex->DrawLatex(0.59, 0.50, "Global uncertainty : 3.6 \%");
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_np_fw->SaveAs(Form("cross_%s/crossSection_pt_integ_middle_np_fw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();
	


	
	////non-prompt bw
	TCanvas* c_np_bw = new TCanvas("c_np_bw","c_np_bw",200,10,800,600);
	c_np_bw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	//sys
	gCross_np_sys[bwrap_init]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	gCross_np_sys[bwrap_init]->GetXaxis()->CenterTitle(1);
	gCross_np_sys[bwrap_init]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy x BR(J/#psi #rightarrow #mu#mu) [nb/(GeV/c)]");
	if (isLog) {
		gCross_np_sys[bwrap_init]->SetMinimum(0.01);
		gCross_np_sys[bwrap_init]->SetMaximum(2000.);
	}
	else {
		gCross_np_sys[bwrap_init]->SetMinimum(0.0);
		gCross_np_sys[bwrap_init]->SetMaximum(500.);
	}
	gCross_np_sys[bwrap_init]->GetXaxis()->SetLimits(0.0, 25.);
	gCross_np_sys[bwrap_init]->SetFillColor(kViolet-9);
	gCross_np_sys[bwrap_init]->Draw("A2");
	gCross_np_sys[4]->SetFillColor(kTeal+7);
//	gCross_np_sys[4]->Draw("2");
	gCross_np_sys[5]->SetFillColor(kRed-9);
//	gCross_np_sys[5]->Draw("2");
	gCross_np_sys[6]->SetFillColor(kAzure-9);
//	gCross_np_sys[6]->Draw("2");
	//gCross_np_sys[7]->SetFillColor(kOrange-4);
	gCross_np_sys[7]->SetFillColor(kOrange-9);
//	gCross_np_sys[7]->Draw("2");
	//value
	SetGraphStyle(gCross_np[bwrap_init],	8,12);
	SetGraphStyle(gCross_np[4],	0,15);
	SetGraphStyle(gCross_np[5],	1,13);
	SetGraphStyle(gCross_np[6],	2,10);
	SetGraphStyle(gCross_np[7],	5,11);
	gCross_np[bwrap_init]->SetMarkerSize(1.3);
	gCross_np[bwrap_init]->Draw("P");
	gCross_np[4]->SetMarkerSize(1.7);
//	gCross_np[4]->Draw("P");
//	gCross_np[5]->Draw("P");
//	gCross_np[6]->Draw("P");
//	gCross_np[7]->Draw("P");
	legUR -> SetHeader("Non-prompt J/#psi");
	legUR -> AddEntry(gCross_np[bwrap_init],"-1.93 < y_{CM} < 0","lp");
//	for (Int_t iy = bw_init; iy < nRap; iy++) {
//		legUR -> AddEntry(gCross_np[iy],Form("%s",rapArr[iy].c_str()),"lp");
//	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
//		latex->DrawLatex(0.59, 0.50, "Global uncertainty : 3.6 \%");
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
//		latex->DrawLatex(0.59, 0.50, "Global uncertainty : 3.6 \%");
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_np_bw->SaveAs(Form("cross_%s/crossSection_pt_integ_middle_np_bw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();
	


	///////////////////////////////////////////////////////////////////
	// save as a root file
	TFile *outFile = new TFile(Form("cross_%s/crossSection_pt_integ_middle_isLog%d_isScale%d.root",dirName,(int)isLog,(int)isScale),"RECREATE");
	outFile->cd();
		gCross_pr_sys[fwrap_init]->SetName("gCross_pr_sys_fwrap");
		gCross_pr_sys[fwrap_init]->Write();	
		gCross_pr[fwrap_init]->SetName("gCross_pr_fwrap");
		gCross_pr[fwrap_init]->Write();	
		gCross_np_sys[fwrap_init]->SetName("gCross_np_sys_fwrap");
		gCross_np_sys[fwrap_init]->Write();	
		gCross_np[fwrap_init]->SetName("gCross_np_fwrap");
		gCross_np[fwrap_init]->Write();	
		gCross_pr_sys[bwrap_init]->SetName("gCross_pr_sys_bwrap");
		gCross_pr_sys[bwrap_init]->Write();	
		gCross_pr[bwrap_init]->SetName("gCross_pr_bwrap");
		gCross_pr[bwrap_init]->Write();	
		gCross_np_sys[bwrap_init]->SetName("gCross_np_sys_bwrap");
		gCross_np_sys[bwrap_init]->Write();	
		gCross_np[bwrap_init]->SetName("gCross_np_bwrap");
		gCross_np[bwrap_init]->Write();	
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

