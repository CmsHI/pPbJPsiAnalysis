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

#include "KYOcommonOptFinal.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

//// runCode // 0=merged, 1=1stRun, 2=2ndRun
void draw_1D_crossSection_rap(char* dirName = "8rap9pt", int runCode=0, bool isScale = false)
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
	const int ntmp=8;
	Double_t pxtmp_lowpt[ntmp]; //x point
	Double_t pxtmp_highpt[ntmp]; //x point
	Double_t pytmp_lowpt[ntmp]; //y point
	Double_t pytmp_highpt[ntmp]; //y point
	Double_t eytmp[ntmp]; //y point error
	Double_t ex[ntmp]; //x error
	Double_t exsys[ntmp]; //sys x error
	Double_t eysys_pr_lowpt[ntmp]; //sys y error
	Double_t eysys_pr_highpt[ntmp]; //sys y error
	Double_t eysys_np_lowpt[ntmp]; //sys y error
	Double_t eysys_np_highpt[ntmp]; //sys y error
	ex = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	exsys = {0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08};
	eysys_pr_lowpt = {0.628666817,
	0.561047916,
	0.895414993,
	0.589828772,
	1.134482956,
	0.641988608,
	0.475608925,
	0.960925124};
	eysys_pr_highpt = {0.218257817,
	0.102275582,
	0.164164131,
	0.154916299,
	0.132015219,
	0.200981767,
	0.143377822,
	0.291853215};
	eysys_np_lowpt = {0.473823549,
	0.335739426,
	0.761779396,
	0.527160575,
	0.472154727,
	0.603763331,
	0.328782614,
	0.59449566};
	eysys_np_highpt = {0.126110923,
	0.065680913,
	0.108619524,
	0.11810767,
	0.084655322,
	0.122616945,
	0.111377214,
	0.207573424};
	
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

	// scaling for drawing
	double	scalePR_low, scalePR_high, scaleNP_low, scaleNP_high;
	if (isScale) {
		scalePR_low = 1.0; scalePR_high = 2.0; scaleNP_low = 1.0; scaleNP_high=1.5;
	}
	else {
		scalePR_low = 1.0; scalePR_high = 1.0; scaleNP_low = 1.0; scaleNP_high=1.0;
	}
	cout << " *** scale for drawing : PR_low = " <<scalePR_low<<", PR_high = "<<scalePR_high<<endl;
	cout << " *** scale for drawing : NP_low = " <<scaleNP_low<<", NP_high = "<<scaleNP_high<<endl;
	
	// --- read-in file
	TFile * f2D = new TFile(Form("../000_fittingResult/total2Dhist_%s.root",dirName));
	//TFile * f2D = new TFile(Form("/home/songkyo/kyo/2015/000_fittingResult/total2Dhist_%s.root",dirName));
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

//	TCanvas *ctmp1 = new TCanvas("ctmp1","ctmp1",600,600);
//	TCanvas *ctmp2 = new TCanvas("ctmp2","ctmp2",600,600);

	// ---  projection to 1D hist
	TH1D* h1D_corrY_PR_Pbp_tmp[nbinsY]; // in y_lab(1st)
	TH1D* h1D_corrY_NP_Pbp_tmp[nbinsY]; 
	TH1D* h1D_corrY_PR_Pbp[nbinsY]; 
	TH1D* h1D_corrY_NP_Pbp[nbinsY]; 
	TH1D* h1D_corrY_PR_pPb_tmp[nbinsY]; // in y_lab(2nd)
	TH1D* h1D_corrY_NP_pPb_tmp[nbinsY]; 
	TH1D* h1D_corrY_PR_pPb[nbinsY]; //in y_CM
	TH1D* h1D_corrY_NP_pPb[nbinsY]; 

	// iy=0 refers to forwards !!! (ordering here && in CM)
	int tmpbin;
	double tmpval=0;
	double tmperr=0;
	for (Int_t ipt = 0; ipt < nbinsY; ipt++) {
		h1D_corrY_PR_Pbp_tmp[ipt] = h2D_corrY_PR_Pbp->ProjectionX(Form("h1D_corrY_PR_Pbp_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_corrY_NP_Pbp_tmp[ipt] = h2D_corrY_NP_Pbp->ProjectionX(Form("h1D_corrY_NP_Pbp_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_corrY_PR_pPb_tmp[ipt] = h2D_corrY_PR_pPb->ProjectionX(Form("h1D_corrY_PR_pPb_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_corrY_NP_pPb_tmp[ipt] = h2D_corrY_NP_pPb->ProjectionX(Form("h1D_corrY_NP_pPb_tmp_%d",ipt),ipt+1,ipt+1);
		h1D_corrY_PR_Pbp[ipt] = new TH1D(Form("h1D_corrY_PR_Pbp_%d",ipt),Form("h1D_corrY_PR_Pbp_%d",ipt),nRap,rapArrNumBF);
		h1D_corrY_NP_Pbp[ipt] = new TH1D(Form("h1D_corrY_NP_Pbp_%d",ipt),Form("h1D_corrY_NP_Pbp_%d",ipt),nRap,rapArrNumBF);
		h1D_corrY_PR_pPb[ipt] = new TH1D(Form("h1D_corrY_PR_pPb_%d",ipt),Form("h1D_corrY_PR_pPb_%d",ipt),nRap,rapArrNumBF);
		h1D_corrY_NP_pPb[ipt] = new TH1D(Form("h1D_corrY_NP_pPb_%d",ipt),Form("h1D_corrY_NP_pPb_%d",ipt),nRap,rapArrNumBF);
		h1D_corrY_PR_Pbp[ipt]->Sumw2();
		h1D_corrY_NP_Pbp[ipt]->Sumw2();
		h1D_corrY_PR_pPb[ipt]->Sumw2();
		h1D_corrY_NP_pPb[ipt]->Sumw2();
		
		for (Int_t iy=0; iy<nbinsX; iy++){
			//1st run
			tmpval = h1D_corrY_PR_Pbp_tmp[ipt]->GetBinContent(iy+1);
			tmperr = h1D_corrY_PR_Pbp_tmp[ipt]->GetBinError(iy+1);
			h1D_corrY_PR_Pbp[ipt]->SetBinContent(nbinsX-iy,tmpval);
			h1D_corrY_PR_Pbp[ipt]->SetBinError(nbinsX-iy,tmperr);
			tmpval = h1D_corrY_NP_Pbp_tmp[ipt]->GetBinContent(iy+1);
			tmperr = h1D_corrY_NP_Pbp_tmp[ipt]->GetBinError(iy+1);
			h1D_corrY_NP_Pbp[ipt]->SetBinContent(nbinsX-iy,tmpval);
			h1D_corrY_NP_Pbp[ipt]->SetBinError(nbinsX-iy,tmperr);
			//2nd run
			tmpval = h1D_corrY_PR_pPb_tmp[ipt]->GetBinContent(iy+1);
			tmperr = h1D_corrY_PR_pPb_tmp[ipt]->GetBinError(iy+1);
			h1D_corrY_PR_pPb[ipt]->SetBinContent(iy+1,tmpval);
			h1D_corrY_PR_pPb[ipt]->SetBinError(iy+1,tmperr);
			tmpval = h1D_corrY_NP_pPb_tmp[ipt]->GetBinContent(iy+1);
			tmperr = h1D_corrY_NP_pPb_tmp[ipt]->GetBinError(iy+1);
			h1D_corrY_NP_pPb[ipt]->SetBinContent(iy+1,tmpval);
			h1D_corrY_NP_pPb[ipt]->SetBinError(iy+1,tmperr);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////

	// 1) merge Pbp+pPb corrected yield
	// 2) calcualte cross-section = corrY/(dPt*dY*lumi*branching)
	TH1D* h1D_corrY_PR_tot[nbinsY]; 
	TH1D* h1D_corrY_NP_tot[nbinsY]; 
	TH1D* h1D_cross_PR_tot[nbinsY]; 
	TH1D* h1D_cross_NP_tot[nbinsY]; 
	for (Int_t ipt = 0; ipt < nbinsY; ipt++) {
		// --- merging
		if (runCode ==0) {	
			h1D_corrY_PR_tot[ipt] = (TH1D*)h1D_corrY_PR_Pbp[ipt]->Clone(Form("h1D_corrY_PR_tot_%d",ipt));
			h1D_corrY_PR_tot[ipt]->Add(h1D_corrY_PR_pPb[ipt]);	
			h1D_corrY_NP_tot[ipt] = (TH1D*)h1D_corrY_NP_Pbp[ipt]->Clone(Form("h1D_corrY_NP_tot_%d",ipt));
			h1D_corrY_NP_tot[ipt]->Add(h1D_corrY_NP_pPb[ipt]);	
		}
		else if (runCode ==1) {
			h1D_corrY_PR_tot[ipt] = (TH1D*)h1D_corrY_PR_Pbp[ipt]->Clone(Form("h1D_corrY_PR_tot_%d",ipt));
			h1D_corrY_NP_tot[ipt] = (TH1D*)h1D_corrY_NP_Pbp[ipt]->Clone(Form("h1D_corrY_NP_tot_%d",ipt));
		}
		else if (runCode ==2) {
			h1D_corrY_PR_tot[ipt] = (TH1D*)h1D_corrY_PR_pPb[ipt]->Clone(Form("h1D_corrY_PR_tot_%d",ipt));
			h1D_corrY_NP_tot[ipt] = (TH1D*)h1D_corrY_NP_pPb[ipt]->Clone(Form("h1D_corrY_NP_tot_%d",ipt));
		}
		cout <<" "<<endl;
		cout <<ipt<<"th pt !!!"<<endl;
		//cout <<" *** a) corrected yield  *** "<<endl;
		//for (int iy=0; iy <nbinsX; iy ++ ){ 
		//	cout << iy <<"th rap" << endl;
		//	cout << "h1D_corrY_PR_tot : getBinWidth (rap) = " << h1D_corrY_PR_tot[ipt]->GetXaxis()->GetBinWidth(iy+1)<<endl; 
		//	cout << "h1D_corrY_PR_tot : getBinWidth (pt) = " << ptBinW[ipt]<<endl; 
		//	cout << "h1D_corrY_PR_tot = " << h1D_corrY_PR_tot[ipt]->GetBinContent(iy+1)<<endl; 
		//	cout << "h1D_corrY_NP_tot = " << h1D_corrY_NP_tot[ipt]->GetBinContent(iy+1)<<endl; 
		//}		
		// --- norm. rap binWidth
		h1D_corrY_PR_tot[ipt]->Scale(1,"width");
		h1D_corrY_NP_tot[ipt]->Scale(1,"width");
		// --- norm. pt binWidth
//		h1D_corrY_PR_tot[ipt]->Scale(1./ptBinW[ipt]);
//		h1D_corrY_NP_tot[ipt]->Scale(1./ptBinW[ipt]);
		//cout <<" *** b) corrYield/(dPt*dRap)  *** "<<endl;
		//for (int iy=0; iy <nbinsX; iy ++ ){ 
		//	cout << iy <<"th rap" << endl;
		//	cout << "h1D_corrY_PR_tot = " << h1D_corrY_PR_tot[ipt]->GetBinContent(iy+1)<<endl; 
		//	cout << "h1D_corrY_NP_tot = " << h1D_corrY_NP_tot[ipt]->GetBinContent(iy+1)<<endl; 
		//}		
		// --- norm. (lumi*br)
		h1D_corrY_PR_tot[ipt]->Scale(1./lumi_mub);
		h1D_corrY_NP_tot[ipt]->Scale(1./lumi_mub);
		h1D_corrY_PR_tot[ipt]->Scale(1./br);
		h1D_corrY_NP_tot[ipt]->Scale(1./br);
		cout <<" *** c) cross-section  *** "<<endl;
		for (int iy=0; iy <nbinsX; iy ++ ){ 
			cout << iy <<"th rap" << endl;
			cout << "h1D_corrY_PR_tot = " << h1D_corrY_PR_tot[ipt]->GetBinContent(iy+1)<<endl; 
			cout << "h1D_corrY_NP_tot = " << h1D_corrY_NP_tot[ipt]->GetBinContent(iy+1)<<endl; 
		}		
	}
	
	//////////////////////////////////////////////////////////////////

	const int lowpt_init=4;
	const int highpt_init=7;	
		
	// ---- prompt
	// *** bin merging
	cout << "*** low pT bin starts from : " << ptArr[lowpt_init].c_str() << endl;
	for (Int_t ipt = lowpt_init+1; ipt < highpt_init; ipt++) {
		h1D_corrY_PR_tot[lowpt_init]->Add(h1D_corrY_PR_tot[ipt]);
		cout << " *** and to low pT, merging : " << ptArr[ipt].c_str() << endl; 
	}
	cout << "*** high pT bin starts from : " << ptArr[highpt_init].c_str() << endl;
	for (Int_t ipt = highpt_init+1; ipt < nbinsY; ipt++) {
		h1D_corrY_PR_tot[highpt_init]->Add(h1D_corrY_PR_tot[ipt]);
		cout << " *** and to high pT, merging : " << ptArr[ipt].c_str() << endl; 
	}
	
	// --- scaling for drawing norm.
	h1D_corrY_PR_tot[lowpt_init]->Scale(scalePR_low);
	h1D_corrY_NP_tot[lowpt_init]->Scale(scaleNP_low);
	h1D_corrY_PR_tot[highpt_init]->Scale(scalePR_high);
	h1D_corrY_NP_tot[highpt_init]->Scale(scaleNP_high);
	string ptArr_low;
	string ptArr_high;
	formPtArr(ptArrNum[lowpt_init], ptArrNum[highpt_init], &ptArr_low);
	formPtArr(ptArrNum[highpt_init], ptArrNum[nbinsY], &ptArr_high);
/*	
	gPad->SetLogy(0);
	SetHistStyle(h1D_corrY_PR_tot[lowpt_init],lowpt_init,0);
	SetHistStyle(h1D_corrY_PR_tot[highpt_init],highpt_init,0);
	h1D_corrY_PR_tot[lowpt_init]->GetXaxis()->SetTitle("y_{CM}");
	h1D_corrY_PR_tot[lowpt_init]->GetXaxis()->CenterTitle();
	h1D_corrY_PR_tot[lowpt_init]->GetXaxis()->SetRangeUser(-3.,2.);
	h1D_corrY_PR_tot[lowpt_init]->GetYaxis()->SetTitle("d#sigma/dy [ #mub]");
	h1D_corrY_PR_tot[lowpt_init]->GetYaxis()->SetRangeUser(0.,80);
	h1D_corrY_PR_tot[lowpt_init]->Draw("pe");
	h1D_corrY_PR_tot[highpt_init]->Draw("pe same");
	legUR -> SetHeader("Prompt J/ #psi");
	if (isScale && scalePR_low != 1.0) legUR -> AddEntry(h1D_corrY_PR_tot[lowpt_init],Form("%s [x%.1f]",ptArr_low.c_str(),scalePR_low));
	else legUR -> AddEntry(h1D_corrY_PR_tot[lowpt_init],Form("%s",ptArr_low.c_str()));
	if (isScale && scalePR_high != 1.0) legUR -> AddEntry(h1D_corrY_PR_tot[highpt_init],Form("%s [x%.1f]",ptArr_high.c_str(),scalePR_high));
	else legUR -> AddEntry(h1D_corrY_PR_tot[highpt_init],Form("%s",ptArr_high.c_str()));
	legUR->Draw();
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.17, 0.85, lumistring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.17, 0.78, beamstring.c_str());
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.17, 0.70, cmsstring.c_str());	
	c_pr->SaveAs(Form("cross_%s/crossSection_rap_pr_isScale%d.pdf",dirName,(int)isScale));
	legUR->Clear();
*/
	/////////////////////////////////////
	// ---- non-prompt
	// *** bin merging
	cout << "*** low pT bin starts from : " << ptArr[lowpt_init].c_str() << endl;
	for (Int_t ipt = lowpt_init+1; ipt < highpt_init; ipt++) {
		h1D_corrY_NP_tot[lowpt_init]->Add(h1D_corrY_NP_tot[ipt]);
		cout << " *** and to low pT, merging : " << ptArr[ipt].c_str() << endl; 
	}
	cout << "*** high pT bin starts from : " << ptArr[highpt_init].c_str() << endl;
	for (Int_t ipt = highpt_init+1; ipt < nbinsY; ipt++) {
		h1D_corrY_NP_tot[highpt_init]->Add(h1D_corrY_NP_tot[ipt]);
		cout << " *** and to high pT, merging : " << ptArr[ipt].c_str() << endl; 
	}
/*	
	gPad->SetLogy(0);
	SetHistStyle(h1D_corrY_NP_tot[lowpt_init],lowpt_init,0);
	SetHistStyle(h1D_corrY_NP_tot[highpt_init],highpt_init,0);
	h1D_corrY_NP_tot[lowpt_init]->GetXaxis()->SetTitle("y_{CM}");
	h1D_corrY_NP_tot[lowpt_init]->GetXaxis()->CenterTitle();
	h1D_corrY_NP_tot[lowpt_init]->GetXaxis()->SetRangeUser(-3.,2.);
	h1D_corrY_NP_tot[lowpt_init]->GetYaxis()->SetTitle("d#sigma/dy [ #mub]");
	h1D_corrY_NP_tot[lowpt_init]->GetYaxis()->SetRangeUser(0.,28);
	h1D_corrY_NP_tot[lowpt_init]->Draw("pe");
	h1D_corrY_NP_tot[highpt_init]->Draw("pe same");
	legUR -> SetHeader("Non-prompt J/ #psi");
	if (isScale && scaleNP_low != 1.0) legUR -> AddEntry(h1D_corrY_NP_tot[lowpt_init],Form("%s [x%.1f]",ptArr_low.c_str(),scaleNP_low));
	else legUR -> AddEntry(h1D_corrY_NP_tot[lowpt_init],Form("%s",ptArr_low.c_str()));
	if (isScale && scaleNP_high != 1.0) legUR -> AddEntry(h1D_corrY_NP_tot[highpt_init],Form("%s [x%.1f]",ptArr_high.c_str(),scaleNP_high));
	else legUR -> AddEntry(h1D_corrY_NP_tot[highpt_init],Form("%s",ptArr_high.c_str()));
	legUR->Draw();
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.17, 0.85, lumistring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.17, 0.78, beamstring.c_str());
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.17, 0.70, cmsstring.c_str());	

	c_np->SaveAs(Form("cross_%s/crossSection_rap_np_isScale%d.pdf",dirName,(int)isScale));
	legUR->Clear();
*/

	//////////////////////////////////////////////
	/// convert to TGraphErrors 	

	//TLegend *legUR = new TLegend(0.50, 0.66, 0.86, 0.89); //upper left
	TLegend *legUR = new TLegend(0.50, 0.70, 0.86, 0.92); //upper left
	SetLegendStyle(legUR);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);
	
	////////////1) prompt
	TCanvas* c_pr = new TCanvas("c_pr","c_pr",200,10,800,600);
	c_pr->cd();
	
	//sys_lowpt
	TGraphAsymmErrors* gCross_pr_sys_lowpt = new TGraphAsymmErrors(h1D_corrY_PR_tot[lowpt_init]);
	gCross_pr_sys_lowpt->SetName("gCross_pr_sys_lowpt");
	for (Int_t iy=0; iy<nbinsX; iy++){
//		gCross_pr_sys_lowpt->SetPointError(iy, exsys[iy], exsys[iy], eysys_pr_lowpt[iy], eysys_pr_lowpt[iy]);
		gCross_pr_sys_lowpt->SetPointError(iy, exsys[iy], exsys[iy], scalePR_low*eysys_pr_lowpt[iy], scalePR_low*eysys_pr_lowpt[iy]);
	}
	gCross_pr_sys_lowpt->GetXaxis()->SetTitle("y_{CM}");	
	gCross_pr_sys_lowpt->GetXaxis()->CenterTitle();	
	gCross_pr_sys_lowpt->GetYaxis()->SetTitle("d#sigma/dy (#mub)");	
	gCross_pr_sys_lowpt->GetXaxis()->SetLimits(-2.87,1.93);	
	gCross_pr_sys_lowpt->SetMinimum(0.0);	
	gCross_pr_sys_lowpt->SetMaximum(85.0);	
	gCross_pr_sys_lowpt->SetFillColor(kRed-9);	
	gCross_pr_sys_lowpt->Draw("A2");
	//sys_highpt
	TGraphAsymmErrors* gCross_pr_sys_highpt = new TGraphAsymmErrors(h1D_corrY_PR_tot[highpt_init]);
	gCross_pr_sys_highpt->SetName("gCross_pr_sys_highpt");
	for (Int_t iy=0; iy<nbinsX; iy++){
//		gCross_pr_sys_highpt->SetPointError(iy, exsys[iy], exsys[iy], eysys_pr_highpt[iy], eysys_pr_highpt[iy]);
		gCross_pr_sys_highpt->SetPointError(iy, exsys[iy], exsys[iy], scalePR_high*eysys_pr_highpt[iy], scalePR_high*eysys_pr_highpt[iy]);
	}
	gCross_pr_sys_highpt->SetFillColor(kTeal-9);	
	gCross_pr_sys_highpt->Draw("2");
	
	//lowpt
	TGraphAsymmErrors* gCross_pr_lowpt = new TGraphAsymmErrors(h1D_corrY_PR_tot[lowpt_init]);
	gCross_pr_lowpt->SetName("gCross_pr_lowpt");
	for (Int_t iy=0; iy<nbinsX; iy++){
		gCross_pr_lowpt->SetPointEXlow(iy,ex[iy]);
		gCross_pr_lowpt->SetPointEXhigh(iy,ex[iy]);
		gCross_pr_lowpt->GetPoint(iy, pxtmp_lowpt[iy], pytmp_lowpt[iy]);
		eytmp[iy] = gCross_pr_lowpt-> GetErrorY(iy);
		cout << "pr : pytmp_lowpt["<<iy<<"] = " << pytmp_lowpt[iy]<<endl;
		cout << "pr : eytmp_lowpt["<<iy<<"] = " << eytmp[iy]<<endl;
	}
	SetGraphStyle(gCross_pr_lowpt,1,3);
	gCross_pr_lowpt->SetMarkerSize(1.2);
	gCross_pr_lowpt->Draw("P");
	//highpt
	TGraphAsymmErrors* gCross_pr_highpt = new TGraphAsymmErrors(h1D_corrY_PR_tot[highpt_init]);
	gCross_pr_highpt->SetName("gCross_pr_highpt");
	for (Int_t iy=0; iy<nbinsX; iy++){
		gCross_pr_highpt->SetPointEXlow(iy,ex[iy]);
		gCross_pr_highpt->SetPointEXhigh(iy,ex[iy]);
		gCross_pr_highpt->GetPoint(iy, pxtmp_highpt[iy], pytmp_highpt[iy]);
		eytmp[iy] = gCross_pr_highpt-> GetErrorY(iy);
		cout << "pr : pytmp_highpt["<<iy<<"] = " << pytmp_highpt[iy]<<endl;
		cout << "pr : eytmp_highpt["<<iy<<"] = " << eytmp[iy]<<endl;
	}
	SetGraphStyle(gCross_pr_highpt,0,5);
	gCross_pr_highpt->SetMarkerSize(1.9);
	gCross_pr_highpt->Draw("P");

	legUR -> SetHeader("Prompt J/#psi");
	if (isScale && scalePR_low != 1.0) legUR -> AddEntry(gCross_pr_lowpt,Form("6.5 < p_{T} < 10 (GeV/c) [x%1.f]",scalePR_low), "lp");
	else legUR -> AddEntry(gCross_pr_lowpt,"6.5 < p_{T} < 10 (GeV/c)","lp");
	if (isScale && scalePR_high != 1.0) legUR -> AddEntry(gCross_pr_highpt,Form("10 < p_{T} < 30 (GeV/c) [x%.1f]",scalePR_high),"lp");
	else legUR -> AddEntry(gCross_pr_highpt,"10 < p_{T} < 30 (GeV/c)","lp");
	legUR->Draw();
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.17, 0.88, cmsstring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.17, 0.79, beamstring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.17, 0.72, lumistring.c_str());
	c_pr->SaveAs(Form("cross_%s/crossSection_rap_pr_isScale%d.pdf",dirName,(int)isScale));
	legUR->Clear();

	////////////1) non-prompt
	TCanvas* c_np = new TCanvas("c_np","c_np",200,10,800,600);
	c_np->cd();
	
	//sys_lowpt
	TGraphAsymmErrors* gCross_np_sys_lowpt = new TGraphAsymmErrors(h1D_corrY_NP_tot[lowpt_init]);
	gCross_np_sys_lowpt->SetName("gCross_np_sys_lowpt");
	for (Int_t iy=0; iy<nbinsX; iy++){
//		gCross_np_sys_lowpt->SetPointError(iy, exsys[iy], exsys[iy], eysys_np_lowpt[iy], eysys_np_lowpt[iy]);
		gCross_np_sys_lowpt->SetPointError(iy, exsys[iy], exsys[iy], scaleNP_low*eysys_np_lowpt[iy], scaleNP_low*eysys_np_lowpt[iy]);
	}
	gCross_np_sys_lowpt->GetXaxis()->SetTitle("y_{CM}");	
	gCross_np_sys_lowpt->GetXaxis()->CenterTitle();	
	gCross_np_sys_lowpt->GetYaxis()->SetTitle("d#sigma/dy (#mub)");	
	gCross_np_sys_lowpt->GetXaxis()->SetLimits(-2.87,1.93);	
	gCross_np_sys_lowpt->SetMinimum(0.0);	
	gCross_np_sys_lowpt->SetMaximum(27.0);	
	gCross_np_sys_lowpt->SetFillColor(kRed-9);	
	gCross_np_sys_lowpt->Draw("A2");
	//sys_highpt
	TGraphAsymmErrors* gCross_np_sys_highpt = new TGraphAsymmErrors(h1D_corrY_NP_tot[highpt_init]);
	gCross_np_sys_highpt->SetName("gCross_np_sys_highpt");
	for (Int_t iy=0; iy<nbinsX; iy++){
//		gCross_np_sys_highpt->SetPointError(iy, exsys[iy], exsys[iy], eysys_np_highpt[iy], eysys_np_highpt[iy]);
		gCross_np_sys_highpt->SetPointError(iy, exsys[iy], exsys[iy], scaleNP_high*eysys_np_highpt[iy], scaleNP_high*eysys_np_highpt[iy]);
	}
	gCross_np_sys_highpt->SetFillColor(kTeal-9);	
	gCross_np_sys_highpt->Draw("2");
	
	//lowpt
	TGraphAsymmErrors* gCross_np_lowpt = new TGraphAsymmErrors(h1D_corrY_NP_tot[lowpt_init]);
	gCross_np_lowpt->SetName("gCross_np_lowpt");
	for (Int_t iy=0; iy<nbinsX; iy++){
		gCross_np_lowpt->SetPointEXlow(iy,ex[iy]);
		gCross_np_lowpt->SetPointEXhigh(iy,ex[iy]);
		gCross_np_lowpt->GetPoint(iy, pxtmp_lowpt[iy], pytmp_lowpt[iy]);
		eytmp[iy] = gCross_np_lowpt-> GetErrorY(iy);
		cout << "np : pytmp_lowpt["<<iy<<"] = " << pytmp_lowpt[iy]<<endl;
		cout << "np : eytmp_lowpt["<<iy<<"] = " << eytmp[iy]<<endl;
	}
	SetGraphStyle(gCross_np_lowpt,1,3);
	gCross_np_lowpt->SetMarkerSize(1.2);
	gCross_np_lowpt->Draw("P");
	//highpt
	TGraphAsymmErrors* gCross_np_highpt = new TGraphAsymmErrors(h1D_corrY_NP_tot[highpt_init]);
	gCross_np_highpt->SetName("gCross_np_highpt");
	for (Int_t iy=0; iy<nbinsX; iy++){
		gCross_np_highpt->SetPointEXlow(iy,ex[iy]);
		gCross_np_highpt->SetPointEXhigh(iy,ex[iy]);
		gCross_np_highpt->GetPoint(iy, pxtmp_highpt[iy], pytmp_highpt[iy]);
		eytmp[iy] = gCross_np_highpt-> GetErrorY(iy);
		cout << "np : pytmp_highpt["<<iy<<"] = " << pytmp_highpt[iy]<<endl;
		cout << "np : eytmp_highpt["<<iy<<"] = " << eytmp[iy]<<endl;
	}
	SetGraphStyle(gCross_np_highpt,0,5);
	gCross_np_highpt->SetMarkerSize(1.9);
	gCross_np_highpt->Draw("P");

	legUR -> SetHeader("Non-prompt J/#psi");
	if (isScale && scaleNP_low != 1.0) legUR -> AddEntry(gCross_np_lowpt,Form("6.5 < p_{T} < 10 (GeV/c) [x%1.f]",scaleNP_low), "lp");
	else legUR -> AddEntry(gCross_np_lowpt,"6.5 < p_{T} < 10 (GeV/c)","lp");
	if (isScale && scaleNP_high != 1.0) legUR -> AddEntry(gCross_np_highpt,Form("10 < p_{T} < 30 (GeV/c) [x%.1f]",scaleNP_high),"lp");
	else legUR -> AddEntry(gCross_np_highpt,"10 < p_{T} < 30 (GeV/c)","lp");
	legUR->Draw();
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.17, 0.88, cmsstring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.17, 0.79, beamstring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.17, 0.72, lumistring.c_str());
	c_np->SaveAs(Form("cross_%s/crossSection_rap_np_isScale%d.pdf",dirName,(int)isScale));
	legUR->Clear();

	///////////////////////////////////////////////////////////////////
	// save as a root file
	TFile *outFile = new TFile(Form("cross_%s/crossSection_rap_isScale%d.root",dirName,(int)isScale),"RECREATE");
	outFile->cd();
	gCross_pr_sys_lowpt->Write();	
	gCross_pr_lowpt->Write();	
	gCross_pr_sys_highpt->Write();	
	gCross_pr_highpt->Write();	
	gCross_np_sys_lowpt->Write();	
	gCross_np_lowpt->Write();	
	gCross_np_sys_highpt->Write();	
	gCross_np_highpt->Write();	
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
		*arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
	}
}


