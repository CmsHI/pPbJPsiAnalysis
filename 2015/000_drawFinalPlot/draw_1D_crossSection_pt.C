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
void draw_1D_crossSection_pt(char* dirName = "8rap9pt", int runCode=0, bool isScale = false,bool isLog = false)
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
	const int ntmp = 9;
	Double_t px[8][ntmp]; //x point
	Double_t pxtmp[8][ntmp]; //x point
	Double_t pytmp[8][ntmp]; //y point
	Double_t eytmp[8][ntmp]; //y point error
	Double_t ex[ntmp]; // x error
	Double_t exsys[ntmp]; //sys x error
	Double_t eysys_pr[8][ntmp]; //sys y error
	Double_t eysys_np[8][ntmp]; //sys y error
	px[0] = {1.80888, 3.48529, 4.47275, 5.68792, 6.9644, 7.96271, 9.1754, 11.5315, 17.7588}; 
	px[1] = {0.00000, 3.53123, 4.5027, 5.71709, 6.96523, 7.9693, 9.17314, 11.4952, 17.6927}; 
	px[2] = {0.00000, 0.00000, 0.00000, 0.00000, 7.01977, 7.99712, 9.19936, 11.5743, 17.7732}; 
	px[3] = {0.00000, 0.00000, 0.00000, 0.00000, 7.12292, 8.01305, 9.22816, 11.6279, 17.8879}; 
	px[4] = {0.00000, 0.00000, 0.00000, 0.00000, 7.05476, 8.00208, 9.21589, 11.5645, 17.7176}; 
	px[5] = {0.00000, 0.00000, 0.00000, 5.82095, 6.97886, 7.96732, 9.18979, 11.5158, 17.4116}; 
	px[6] = {0.00000, 3.52893, 4.48328, 5.69351, 6.96188, 7.95707, 9.14886, 11.4747, 17.231}; 
	px[7] = {1.78552, 3.47853, 4.46938, 5.6761, 6.96419, 7.97702, 9.16158, 11.5077, 17.3061}; 
	ex = {0,0,0,0,0,0,0,0,0};
	exsys = {0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4};
	//exsys = {1.,1.,1.,1.,1.,1.,1.,1.,1.};
	eysys_pr[0] = {6.823585599,
	2.754553939,
	2.09561067,
	0.751742545,
	0.292440732,
	0.177658156,
	0.113507836,
	0.042074566,
	0.003424401};
	eysys_pr[1] = {0.00000,
	2.066727035,
	2.298526697,
	0.506996157,
	0.166204429,
	0.082983383,
	0.067116223,
	0.019912669,
	0.001803345};	
	eysys_pr[2] = {0.00000,
	0.00000,
	0.00000,
	0.00000,
	0.158412943,
	0.172238804,
	0.079986558,
	0.01679906,
	0.002508664};
	eysys_pr[3] = {0.00000,
	0.00000,
	0.00000,
	0.00000,
	0.614088927,
	0.146153707,
	0.068923197,
	0.009788824,
	0.001781335};
	eysys_pr[4] = {0.00000,
	0.00000,
	0.00000,
	0.00000,
	0.310255688,
	0.160917164,
	0.067657419,
	0.014814111,
	0.001799864};
	eysys_pr[5] = {0.00000,
	0.00000,
	0.00000,
	0.598184541,
	0.291530553,
	0.123517678,
	0.122278663,
	0.021084514,
	0.001946998};
	eysys_pr[6] = {0.00000,
	1.774023634,
	3.720423311,
	0.583501479,
	0.192082261,
	0.133188232,
	0.07282919,
	0.008813247,
	0.001347129};
	eysys_pr[7] = {8.591314953,
	3.74427176,
	1.808696997,
	0.78623807,
	0.243599481,
	0.16402451,
	0.080699254,
	0.02293258,
	0.002158739};
	eysys_np[0] = {4.906648373,
	1.298000157,
	0.84588776,
	0.39274983,
	0.173108453,
	0.137606813,
	0.10351739,
	0.036142245,
	0.003766755};
	eysys_np[1] = {0.00000,
	0.962486758,
	1.273613649,
	0.354226692,
	0.119848294,
	0.043266719,
	0.05300501,
	0.01894851,
	0.00206701};
	eysys_np[2] = {0.00000,
	0.00000,
	0.00000,
	0.00000,
	0.128308574,
	0.166978353,
	0.077367096,
	0.015572486,
	0.002431642};
	eysys_np[3] = {0.00000,
	0.00000,
	0.00000,
	0.00000,
	0.244033742,
	0.098904713,
	0.061190229,
	0.009067791,
	0.001507802};
	eysys_np[4] = {0.00000,
	0.00000,
	0.00000,
	0.00000,
	0.248351543,
	0.118099805,
	0.063202782,
	0.013466951,
	0.002164002};
	eysys_np[5] = {0.00000,
	0.00000,
	0.00000,
	0.434174112,
	0.209918891,
	0.098543151,
	0.101866676,
	0.015888018,
	0.002042434};
	eysys_np[6] = {0.00000,
	0.893125017,
	1.234527744,
	0.422629695,
	0.098374138,
	0.041311919,
	0.053527578,
	0.008328016,
	0.001080907};
	eysys_np[7] = {4.758456686,
	1.024871016,
	0.600404252,
	0.305984184,
	0.193471649,
	0.119464542,
	0.045602308,
	0.013624571,
	0.00201605};
	
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
	TH1D* h1D_corrY_PR_Pbp[nbinsX]; 
	TH1D* h1D_corrY_NP_Pbp[nbinsX]; 
	TH1D* h1D_corrY_PR_pPb[nbinsX]; 
	TH1D* h1D_corrY_NP_pPb[nbinsX]; 
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_corrY_PR_Pbp[iy] = h2D_corrY_PR_Pbp->ProjectionY(Form("h1D_corrY_PR_Pbp_%d",iy),iy+1,iy+1);
		h1D_corrY_NP_Pbp[iy] = h2D_corrY_NP_Pbp->ProjectionY(Form("h1D_corrY_NP_Pbp_%d",iy),iy+1,iy+1);
		//for 2nd run
		h1D_corrY_PR_pPb[iy] = h2D_corrY_PR_pPb->ProjectionY(Form("h1D_corrY_PR_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
		h1D_corrY_NP_pPb[iy] = h2D_corrY_NP_pPb->ProjectionY(Form("h1D_corrY_NP_pPb_%d",iy),nbinsX-iy,nbinsX-iy);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////

	// 1) merge Pbp+pPb corrected yield
	// 2) calcualte cross-section = corrY/(dPt*dY*lumi*branching)
	TH1D* h1D_cross_PR_tot[nbinsX]; 
	TH1D* h1D_cross_NP_tot[nbinsX]; 
	for (Int_t iy = 0; iy < nbinsX; iy++) {
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
		h1D_cross_PR_tot[iy]->Scale(1./rapBinW[iy]);
		h1D_cross_NP_tot[iy]->Scale(1./rapBinW[iy]);
		// --- norm. (lumi*br)
		h1D_cross_PR_tot[iy]->Scale(1./lumi_mub);
		h1D_cross_NP_tot[iy]->Scale(1./lumi_mub);
		h1D_cross_PR_tot[iy]->Scale(1./br);
		h1D_cross_NP_tot[iy]->Scale(1./br);
		cout <<" *** c) cross-section  *** "<<endl;
		for (int ipt=0; ipt <nbinsY; ipt ++ ){ 
			cout << ipt <<"th pt" << endl;
			cout << "h1D_cross_PR_tot = " << h1D_cross_PR_tot[iy]->GetBinContent(ipt+1)<<endl; 
			cout << "h1D_cross_NP_tot = " << h1D_cross_NP_tot[iy]->GetBinContent(ipt+1)<<endl; 
		}
	}
		
	// set values as zero for unused bins
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_cross_PR_tot[iy]->SetBinContent(1,-532);
			h1D_cross_NP_tot[iy]->SetBinContent(1,-532);
			h1D_cross_PR_tot[iy]->SetBinError(1,0);
			h1D_cross_NP_tot[iy]->SetBinError(1,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_cross_PR_tot[iy]->SetBinContent(2,-532);
			h1D_cross_PR_tot[iy]->SetBinContent(3,-532);
			h1D_cross_NP_tot[iy]->SetBinContent(2,-532);
			h1D_cross_NP_tot[iy]->SetBinContent(3,-532);
			h1D_cross_PR_tot[iy]->SetBinError(2,0);
			h1D_cross_PR_tot[iy]->SetBinError(3,0);
			h1D_cross_NP_tot[iy]->SetBinError(2,0);
			h1D_cross_NP_tot[iy]->SetBinError(3,0);
		}
		if (iy>=2 && iy<=4) {
			h1D_cross_PR_tot[iy]->SetBinContent(4,-532);
			h1D_cross_NP_tot[iy]->SetBinContent(4,-532);
			h1D_cross_PR_tot[iy]->SetBinError(4,0);
			h1D_cross_NP_tot[iy]->SetBinError(4,0);
		}
	}
	//////////////////////////////////////////////////////////////////

//	TLegend *legUR = new TLegend(0.52, 0.55, 0.86, 0.92); //upper left
	TLegend *legUR = new TLegend(0.59, 0.55, 0.86, 0.92); //upper left
	SetLegendStyle(legUR);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//global uncertainty
	TBox * globalbox = new TBox(0.5, 6.4, 1.5, 13.6);
	globalbox->SetFillColor(kYellow);
	
	// --- Draw histograms
	
	const int fw_init = 0;
	const int bw_init = 3;
/*	
	// ---- prompt forward
	TCanvas* c_pr_fw = new TCanvas("c_pr_fw","c_pr_fw",200,10,800,600);
	c_pr_fw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	SetHistStyle(h1D_cross_PR_tot[fw_init],fw_init,0);
	h1D_cross_PR_tot[fw_init]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	h1D_cross_PR_tot[fw_init]->GetXaxis()->CenterTitle();
	h1D_cross_PR_tot[fw_init]->GetXaxis()->SetRangeUser(0.,30.);
	h1D_cross_PR_tot[fw_init]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy [ #mub/(GeV/c)]");
	//h1D_cross_PR_tot[fw_init]->GetYaxis()->SetRangeUser(0.01,150000);
	if (isLog) h1D_cross_PR_tot[fw_init]->GetYaxis()->SetRangeUser(0.01,700);
	else h1D_cross_PR_tot[fw_init]->GetYaxis()->SetRangeUser(0.0,350);
	h1D_cross_PR_tot[fw_init]->Draw("pe");
	for (Int_t iy = fw_init+1; iy < bw_init; iy++) {
		SetHistStyle(h1D_cross_PR_tot[iy],iy,0);
		h1D_cross_PR_tot[iy]->Draw("pe same");
	}
	legUR -> SetHeader("Prompt J/#psi");
	for (Int_t iy = fw_init; iy < bw_init; iy++) {
		legUR -> AddEntry(h1D_cross_PR_tot[iy],Form("%s",rapArr[iy].c_str()),"lp");
	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_pr_fw->SaveAs(Form("cross_%s/crossSection_pt_pr_fw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();

	// ---- prompt backward
	TCanvas* c_pr_bw = new TCanvas("c_pr_bw","c_pr_bw",200,10,800,600);
	c_pr_bw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	SetHistStyle(h1D_cross_PR_tot[bw_init],bw_init,0);
	h1D_cross_PR_tot[bw_init]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	h1D_cross_PR_tot[bw_init]->GetXaxis()->CenterTitle();
	h1D_cross_PR_tot[bw_init]->GetXaxis()->SetRangeUser(0.,30.);
	h1D_cross_PR_tot[bw_init]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy [ #mub/(GeV/c)]");
	if (isLog) h1D_cross_PR_tot[bw_init]->GetYaxis()->SetRangeUser(0.01,700);
	else h1D_cross_PR_tot[bw_init]->GetYaxis()->SetRangeUser(0.0,350);
	h1D_cross_PR_tot[bw_init]->Draw("pe");
	for (Int_t iy = bw_init+1; iy < nbinsX; iy++) {
		SetHistStyle(h1D_cross_PR_tot[iy],iy,0);
		h1D_cross_PR_tot[iy]->Draw("pe same");
	}
	legUR -> SetHeader("Prompt J/#psi");
	for (Int_t iy = bw_init; iy < nbinsX; iy++) {
		legUR -> AddEntry(h1D_cross_PR_tot[iy],Form("%s",rapArr[iy].c_str()),"lp");
	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_pr_bw->SaveAs(Form("cross_%s/crossSection_pt_pr_bw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();

	// ---- non-prompt forward
	TCanvas* c_np_fw = new TCanvas("c_np_fw","c_np_fw",200,10,800,600);
	c_np_fw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	SetHistStyle(h1D_cross_NP_tot[fw_init],fw_init,0);
	h1D_cross_NP_tot[fw_init]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	h1D_cross_NP_tot[fw_init]->GetXaxis()->CenterTitle();
	h1D_cross_NP_tot[fw_init]->GetXaxis()->SetRangeUser(0.,30.);
	h1D_cross_NP_tot[fw_init]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy [ #mub/(GeV/c)]");
	if (isLog) h1D_cross_NP_tot[fw_init]->GetYaxis()->SetRangeUser(0.01,130);
	else h1D_cross_NP_tot[fw_init]->GetYaxis()->SetRangeUser(0.,65);
	h1D_cross_NP_tot[fw_init]->Draw("pe");
	for (Int_t iy = fw_init+1; iy < bw_init; iy++) {
		SetHistStyle(h1D_cross_NP_tot[iy],iy,0);
		h1D_cross_NP_tot[iy]->Draw("pe same");
	}
	legUR -> SetHeader("Non-prompt J/#psi");
	for (Int_t iy = fw_init; iy < bw_init; iy++) {
		legUR -> AddEntry(h1D_cross_NP_tot[iy],Form("%s",rapArr[iy].c_str()),"lp");
	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_np_fw->SaveAs(Form("cross_%s/crossSection_pt_np_fw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();

	// ---- non-prompt backward
	TCanvas* c_np_bw = new TCanvas("c_np_bw","c_np_bw",200,10,800,600);
	c_np_bw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	SetHistStyle(h1D_cross_NP_tot[bw_init],bw_init,0);
	h1D_cross_NP_tot[bw_init]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	h1D_cross_NP_tot[bw_init]->GetXaxis()->CenterTitle();
	h1D_cross_NP_tot[bw_init]->GetXaxis()->SetRangeUser(0.,30.);
	h1D_cross_NP_tot[bw_init]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy [ #mub/(GeV/c)]");
	if (isLog) h1D_cross_NP_tot[bw_init]->GetYaxis()->SetRangeUser(0.01,130);
	else h1D_cross_NP_tot[bw_init]->GetYaxis()->SetRangeUser(0.,65);
	h1D_cross_NP_tot[bw_init]->Draw("pe");
	for (Int_t iy = bw_init+1; iy < nbinsX; iy++) {
		SetHistStyle(h1D_cross_NP_tot[iy],iy,0);
		h1D_cross_NP_tot[iy]->Draw("pe same");
	}
	legUR -> SetHeader("Non-prompt J/#psi");
	for (Int_t iy = bw_init; iy < nbinsX; iy++) {
		legUR -> AddEntry(h1D_cross_NP_tot[iy],Form("%s",rapArr[iy].c_str()),"lp");
	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_np_bw->SaveAs(Form("cross_%s/crossSection_pt_np_bw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();
*/


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
			gCross_pr_sys[iy]->SetPointError(ipt, exsys[ipt], exsys[ipt], eysys_pr[iy][ipt], eysys_pr[iy][ipt]);
			gCross_pr[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
//			eytmp[iy][ipt] = gCross_pr[iy]-> GetErrorYhigh(ipt);
//			eytmp[iy][ipt] = gCross_pr[iy]-> GetErrorYlow(ipt);
			eytmp[iy][ipt] = gCross_pr[iy]-> GetErrorY(ipt);
			cout << "pr : pytmp["<<iy<<"]["<<ipt<<"] = " << pytmp[iy][ipt]<<endl;
			cout << "pr : eytmp["<<iy<<"]["<<ipt<<"] = " << eytmp[iy][ipt]<<endl;
			gCross_pr[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gCross_pr[iy]->SetPointEXlow(ipt, ex[ipt]);
			gCross_pr[iy]->SetPointEXhigh(ipt, ex[ipt]);
			
			gCross_np_sys[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			gCross_np_sys[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gCross_np_sys[iy]->SetPointError(ipt, exsys[ipt], exsys[ipt], eysys_np[iy][ipt], eysys_np[iy][ipt]);
			gCross_np[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			eytmp[iy][ipt] = gCross_np[iy]-> GetErrorY(ipt);
			cout << "np : pytmp["<<iy<<"]["<<ipt<<"] = " << pytmp[iy][ipt]<<endl;
			cout << "np : eytmp["<<iy<<"]["<<ipt<<"] = " << eytmp[iy][ipt]<<endl;
			gCross_np[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gCross_np[iy]->SetPointEXlow(ipt, ex[ipt]);
			gCross_np[iy]->SetPointEXhigh(ipt, ex[ipt]);
		}
	}
	
	////prompt fw
	TCanvas* c_pr_fw = new TCanvas("c_pr_fw","c_pr_fw",200,10,800,600);
	c_pr_fw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	//sys
	gCross_pr_sys[0]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	gCross_pr_sys[0]->GetXaxis()->CenterTitle("");
	gCross_pr_sys[0]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy #mub/(GeV/c)");
	if (isLog) {
		gCross_pr_sys[0]->SetMinimum(0.01);
		gCross_pr_sys[0]->SetMaximum(700.);
	}
	else {
		gCross_pr_sys[0]->SetMinimum(0.0);
		gCross_pr_sys[0]->SetMaximum(350.);
	}
	gCross_pr_sys[0]->GetXaxis()->SetLimits(0.0, 25.);
	gCross_pr_sys[0]->SetFillColor(kViolet-9);
	gCross_pr_sys[0]->Draw("A2");
	gCross_pr_sys[1]->SetFillColor(kTeal+7);
	gCross_pr_sys[1]->Draw("2");
	gCross_pr_sys[2]->SetFillColor(kRed-9);
	gCross_pr_sys[2]->Draw("2");
	//value
	SetGraphStyle(gCross_pr[0],	8,12);
	SetGraphStyle(gCross_pr[1],	0,15);
	SetGraphStyle(gCross_pr[2],	1,13);
	gCross_pr[0]->SetMarkerSize(1.3);
	gCross_pr[0]->Draw("P");
	gCross_pr[1]->SetMarkerSize(1.7);
	gCross_pr[1]->Draw("P");
	gCross_pr[2]->Draw("P");
	legUR -> SetHeader("Prompt J/#psi");
	for (Int_t iy = fw_init; iy < bw_init; iy++) {
		legUR -> AddEntry(gCross_pr[iy],Form("%s",rapArr[iy].c_str()),"lp");
	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_pr_fw->SaveAs(Form("cross_%s/crossSection_pt_pr_fw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();
	
	////prompt bw
	TCanvas* c_pr_bw = new TCanvas("c_pr_bw","c_pr_bw",200,10,800,600);
	c_pr_bw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	//sys
	gCross_pr_sys[3]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	gCross_pr_sys[3]->GetXaxis()->CenterTitle("");
	gCross_pr_sys[3]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy #mub/(GeV/c)");
	if (isLog) {
		gCross_pr_sys[3]->SetMinimum(0.01);
		gCross_pr_sys[3]->SetMaximum(700.);
	}
	else {
		gCross_pr_sys[3]->SetMinimum(0.0);
		gCross_pr_sys[3]->SetMaximum(350.);
	}
	gCross_pr_sys[3]->GetXaxis()->SetLimits(0.0, 25.);
	gCross_pr_sys[3]->SetFillColor(kViolet-9);
	gCross_pr_sys[3]->Draw("A2");
	gCross_pr_sys[4]->SetFillColor(kTeal+7);
	gCross_pr_sys[4]->Draw("2");
	gCross_pr_sys[5]->SetFillColor(kRed-9);
	gCross_pr_sys[5]->Draw("2");
	gCross_pr_sys[6]->SetFillColor(kAzure-9);
	gCross_pr_sys[6]->Draw("2");
	//gCross_pr_sys[7]->SetFillColor(kOrange-4);
	gCross_pr_sys[7]->SetFillColor(kOrange-9);
	gCross_pr_sys[7]->Draw("2");
	//value
	SetGraphStyle(gCross_pr[3],	8,12);
	SetGraphStyle(gCross_pr[4],	0,15);
	SetGraphStyle(gCross_pr[5],	1,13);
	SetGraphStyle(gCross_pr[6],	2,10);
	SetGraphStyle(gCross_pr[7],	5,11);
	gCross_pr[3]->SetMarkerSize(1.3);
	gCross_pr[3]->Draw("P");
	gCross_pr[4]->SetMarkerSize(1.7);
	gCross_pr[4]->Draw("P");
	gCross_pr[5]->Draw("P");
	gCross_pr[6]->Draw("P");
	gCross_pr[7]->Draw("P");
	legUR -> SetHeader("Prompt J/#psi");
	for (Int_t iy = bw_init; iy < nbinsX; iy++) {
		legUR -> AddEntry(gCross_pr[iy],Form("%s",rapArr[iy].c_str()),"lp");
	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_pr_bw->SaveAs(Form("cross_%s/crossSection_pt_pr_bw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();
	
	////non-prompt fw
	TCanvas* c_np_fw = new TCanvas("c_np_fw","c_np_fw",200,10,800,600);
	c_np_fw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	//sys
	gCross_np_sys[0]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	gCross_np_sys[0]->GetXaxis()->CenterTitle("");
	gCross_np_sys[0]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy #mub/(GeV/c)");
	if (isLog) {
		gCross_np_sys[0]->SetMinimum(0.01);
		gCross_np_sys[0]->SetMaximum(130.);
	}
	else {
		gCross_np_sys[0]->SetMinimum(0.0);
		gCross_np_sys[0]->SetMaximum(65.);
	}
	gCross_np_sys[0]->GetXaxis()->SetLimits(0.0, 25.);
	gCross_np_sys[0]->SetFillColor(kViolet-9);
	gCross_np_sys[0]->Draw("A2");
	gCross_np_sys[1]->SetFillColor(kTeal+7);
	gCross_np_sys[1]->Draw("2");
	gCross_np_sys[2]->SetFillColor(kRed-9);
	gCross_np_sys[2]->Draw("2");
	//value
	SetGraphStyle(gCross_np[0],	8,12);
	SetGraphStyle(gCross_np[1],	0,15);
	SetGraphStyle(gCross_np[2],	1,13);
	gCross_np[0]->SetMarkerSize(1.3);
	gCross_np[0]->Draw("P");
	gCross_np[1]->SetMarkerSize(1.7);
	gCross_np[1]->Draw("P");
	gCross_np[2]->Draw("P");
	legUR -> SetHeader("Non-prompt J/#psi");
	for (Int_t iy = fw_init; iy < bw_init; iy++) {
		legUR -> AddEntry(gCross_np[iy],Form("%s",rapArr[iy].c_str()),"lp");
	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_np_fw->SaveAs(Form("cross_%s/crossSection_pt_np_fw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();
	


	
	////non-prompt bw
	TCanvas* c_np_bw = new TCanvas("c_np_bw","c_np_bw",200,10,800,600);
	c_np_bw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
	//sys
	gCross_np_sys[3]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	gCross_np_sys[3]->GetXaxis()->CenterTitle("");
	gCross_np_sys[3]->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}dy #mub/(GeV/c)");
	if (isLog) {
		gCross_np_sys[3]->SetMinimum(0.01);
		gCross_np_sys[3]->SetMaximum(130.);
	}
	else {
		gCross_np_sys[3]->SetMinimum(0.0);
		gCross_np_sys[3]->SetMaximum(65.);
	}
	gCross_np_sys[3]->GetXaxis()->SetLimits(0.0, 25.);
	gCross_np_sys[3]->SetFillColor(kViolet-9);
	gCross_np_sys[3]->Draw("A2");
	gCross_np_sys[4]->SetFillColor(kTeal+7);
	gCross_np_sys[4]->Draw("2");
	gCross_np_sys[5]->SetFillColor(kRed-9);
	gCross_np_sys[5]->Draw("2");
	gCross_np_sys[6]->SetFillColor(kAzure-9);
	gCross_np_sys[6]->Draw("2");
	//gCross_np_sys[7]->SetFillColor(kOrange-4);
	gCross_np_sys[7]->SetFillColor(kOrange-9);
	gCross_np_sys[7]->Draw("2");
	//value
	SetGraphStyle(gCross_np[3],	8,12);
	SetGraphStyle(gCross_np[4],	0,15);
	SetGraphStyle(gCross_np[5],	1,13);
	SetGraphStyle(gCross_np[6],	2,10);
	SetGraphStyle(gCross_np[7],	5,11);
	gCross_np[3]->SetMarkerSize(1.3);
	gCross_np[3]->Draw("P");
	gCross_np[4]->SetMarkerSize(1.7);
	gCross_np[4]->Draw("P");
	gCross_np[5]->Draw("P");
	gCross_np[6]->Draw("P");
	gCross_np[7]->Draw("P");
	legUR -> SetHeader("Non-prompt J/#psi");
	for (Int_t iy = bw_init; iy < nbinsX; iy++) {
		legUR -> AddEntry(gCross_np[iy],Form("%s",rapArr[iy].c_str()),"lp");
	}
	legUR->Draw();
	if (isLog) {
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.17, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.17, 0.19, cmsstring.c_str());	
	}
	else{
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.34, lumistring.c_str());
		latex->SetTextSize(0.04);
		latex->DrawLatex(0.57, 0.27, beamstring.c_str());
		latex->SetTextSize(0.05);
		latex->DrawLatex(0.57, 0.19, cmsstring.c_str());	
	}
	c_np_bw->SaveAs(Form("cross_%s/crossSection_pt_np_bw_isLog%d_isScale%d.pdf",dirName,(int)isLog,(int)isScale));
	legUR->Clear();
	


	///////////////////////////////////////////////////////////////////
	// save as a root file
	TFile *outFile = new TFile(Form("cross_%s/crossSection_pt_isLog%d_isScale%d.root",dirName,(int)isLog,(int)isScale),"RECREATE");
	outFile->cd();
	for (Int_t iy = 0; iy < nRap; iy++) {
		gCross_pr_sys[iy]->Write();	
		gCross_pr[iy]->Write();	
		gCross_np_sys[iy]->Write();	
		gCross_np[iy]->Write();	
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

