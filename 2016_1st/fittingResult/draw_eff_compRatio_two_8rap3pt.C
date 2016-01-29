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

// no ordering in rap (just y_lab)
void draw_eff_compRatio_two_8rap3pt(bool isPrompt = true, bool is1st = true, char* dirName02 = "dir_eff_compRatio_oldacccut_check5SF_noSF_20160122", bool isNoError=true)
{
	gROOT->Macro("./JpsiStyle.C");

	// --- read-in file
	// hRatio = f01 / f02
	TFile * f01;
	TFile * f02;
	char* f01name="SF";
	//char* f02name="no SF";
	char* f02name="old SF";
	
	double ratiomin=0.5; 
	double ratiomax=1.5; 

	char* strPrompt;
	char* str1st;
	if (isPrompt) strPrompt = "PRMC";
	else strPrompt = "NPMC";
	if (is1st) str1st = "Pbp";
	else str1st = "pPb";

	const char* sampleName = Form("%spythia_%s",strPrompt,str1st);
	cout << "sampleName = " << sampleName << endl;

	////// new acc new eta (all tag5)
	//f01 = new TFile("./dir_1D_8rap3pt_noPtWeight_tag5_v1/eff_pt_isLog0.root");
	//f02 = new TFile("./dir_1D_8rap3pt_noPtWeight_CtErr1_SF0_Zvtx1/eff_pt_isLog0.root");
	//f02 = new TFile("./dir_1D_8rap3pt_noPtWeight_check5_oldacc_oldeta_private/eff_pt_isLog0.root");//check5 test
	
	////// old acc old eta (PAS) 
	f01 = new TFile("./dir_1D_8rap3pt_noPtWeight_oldacccut_check5/eff_pt_isLog0.root");
	f02 = new TFile("./dir_1D_8rap3pt_noPtWeight_CtErr1_SF0_Zvtx1_oldacccut/eff_pt_isLog0.root");
	
	////// new acc new eta but eta13 (all tag5) 
	//f01 = new TFile("./dir_1D_8rap3pt_noPtWeight_tag5_v1_eta13/eff_pt_isLog0.root");
	//f02 = new TFile("./dir_1D_8rap3pt_noPtWeight_CtErr1_SF0_Zvtx1_eta13/eff_pt_isLog0.root");

	cout << "sampleName = " << sampleName << endl;

	double rapEdge[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4};
	
	// --- read-in eff hist
//	const int nRap = nEdge -1 ; 
	const int nRap = 8; 
	cout << "nRap = " << nRap << endl;
	TH1D* h1D_01[nRap]; 
	TH1D* h1D_02[nRap]; 
	TH1D *hRatio[nRap]; //f01/f02
	for (int iy=0; iy < nRap; iy ++){
		h1D_01[iy] = (TH1D*)f01->Get(Form("h1D_eff_PR_Pbp_%d",iy));
		h1D_02[iy] = (TH1D*)f02->Get(Form("h1D_eff_PR_Pbp_%d",iy));
	cout << "h1D_01["<<iy<<"] = " << h1D_01[iy] << endl;
	cout << "h1D_02["<<iy<<"] = " << h1D_02[iy] << endl;
		hRatio[iy]=(TH1D*)h1D_01[iy]->Clone(Form("hRatio_%d",iy));
		hRatio[iy]->Divide(h1D_02[iy]);
	}
	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms

	TCanvas* c1 = new TCanvas("c1","c1",1600,800);
	c1->Divide(4,2);
	TCanvas* c2 = new TCanvas("c2","c2",1600,800);
	c2->Divide(4,2);
	TLegend *legBR = new TLegend(0.50,0.30,0.80,0.41,NULL,"brNDC");
	SetLegendStyle(legBR);

	TH1D* hdummy = new TH1D("hdummy","",100,0,30);
	hdummy->GetXaxis()->SetTitle("p_{T} (GeV)");
	hdummy->GetYaxis()->SetTitle("Efficiency");
	hdummy->SetMinimum(0.);
	hdummy->SetMaximum(1.);
	hdummy->GetXaxis()->SetRangeUser(0., 30.);
		
	TH1D* hdummy2 = new TH1D("hdummy2","",100,0,30);
	hdummy2->GetXaxis()->SetTitle("p_{T} (GeV)");
	hdummy2->GetYaxis()->SetTitle(Form("[ %s ]/[ %s ]",f01name,f02name));
	hdummy2->SetMinimum(ratiomin);
	hdummy2->SetMaximum(ratiomax);
	hdummy2->GetXaxis()->SetRangeUser(0.,30.);

	for (Int_t iy = 0; iy < nRap; iy++) {
		//// 1)  distributions
		//c1->cd();
		c1->cd(iy+1);
		//SetHistStyle(h1D_02[iy],4,10);
		SetHistStyle(h1D_01[iy],0,0);
		//SetHistStyle(h1D_02[iy],3,10);
		SetHistStyle(h1D_02[iy],5,10);
		//SetHistStyle(h1D_02[iy],5,13);
		hdummy->Draw();
		h1D_01[iy]->Draw("pe same");
		h1D_02[iy]->Draw("pe same");
/*
		if (is1st) {
			if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,.8);
			else if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,2,.8);
			else if (iy==5)  dashedLine (5.,0.,5.,1.,2,.8);
			else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,2,.8);
		}
		else {
			if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,.8);
			else if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,2,.8);
			else if (iy==2)  dashedLine (5.,0.,5.,1.,2,.8);
			else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,.8);
		}
*/
		latex->DrawLatex(0.53,0.25,sampleName);
		latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		//if (iy==0) {
		if (iy==7) {
			legBR->AddEntry(h1D_01[iy],f01name,"p");
			legBR->AddEntry(h1D_02[iy],f02name,"p");
			legBR->Draw();
		}
		//c1->SaveAs(Form("%s/%s_compEff_%d.pdf",dirName02,sampleName,iy));
		//legBR->Clear();
		//c1->Clear();
		
		//// 2) ratio f01/f02
		//c2->cd();
		c2->cd(iy+1);
		SetHistStyle(hRatio[iy],1,0);
		if (isNoError) {
			for (int ipt=0; ipt<3; ipt++) {
				hRatio[iy]->SetBinError(ipt+1,0.);
			}
		}
		hdummy2->Draw();
		//hRatio[iy]->Draw("pe same");
	//hRatio[iy]->GetYaxis()->SetTitle(Form("[ %s ]/[ %s ]",f01name,f02name));
	hRatio[iy]->SetMinimum(ratiomin);
	hRatio[iy]->SetMaximum(ratiomax);
	
		hRatio[iy]->Draw("pe same");
		dashedLine(0.,1.,30.,1.,1,.8);
/*
		if (is1st) {
			if (iy==0 || iy==7) dashedLine (2.,ratiomin,2.,ratiomax,2,.8);
			else if (iy==1 || iy==6) dashedLine (3.,ratiomin,3.,ratiomax,2,.8);
			else if (iy==5)  dashedLine (5.,ratiomin,5.,ratiomax,2,.8);
			else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,.8);
		}
		else {
			if (iy==0 || iy==7) dashedLine (2.,ratiomin,2.,ratiomax,2,.8);
			else if (iy==1 || iy==6) dashedLine (3.,ratiomin,3.,ratiomax,2,.8);
			else if (iy==2)  dashedLine (5.,ratiomin,5.,ratiomax,2,.8);
			else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,.8);
		}
	*/	
		latex->DrawLatex(0.53,0.85,sampleName);
		latex->DrawLatex(0.53,0.79,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
	}

	c1->SaveAs(Form("%s/%s_eff_compDist_two.pdf",dirName02,strPrompt));
	c2->SaveAs(Form("%s/%s_compRatio.pdf",dirName02,strPrompt));

	return;

}

