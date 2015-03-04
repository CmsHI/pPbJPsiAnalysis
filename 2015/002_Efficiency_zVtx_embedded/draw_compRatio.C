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


void draw_compRatio(bool isPrompt = true, bool is1st = true, char* dirName = "draw_1D_tnpV14", char* dirName02 = "draw_compRatio_tnpV14")
{
	gROOT->Macro("./JpsiStyle.C");

	// --- read-in file
	TFile * f01;
	TFile * f02;
	char* f01name="with SF";
	char* f02name="without SF";
	
	char* sampleName;
	//double ratiomin=0.8; 
	//double ratiomax=1.2; 
	double ratiomin=0.7; 
	double ratiomax=1.3; 
	double relmin=-0.3; 
	double relmax=0.3; 

	if (is1st) {
		if (isPrompt) {
			//f01 = new TFile(Form("%s/PRMCpythia_Pbp_EffPt.root",dirName));
			//f02 = new TFile(Form("%s/PRMCembedded_Pbp_EffPt.root",dirName));
			//f01 = new TFile(Form("%s/PRMCpythia_Pbp_weight_EffPt.root",dirName));
			//f02 = new TFile(Form("%s/PRMCpythia_Pbp_noweight_EffPt.root",dirName));
			f01 = new TFile(Form("%s/PRMCpythia_Pbp_sf1_EffPt.root",dirName));
			f02 = new TFile(Form("%s/PRMCpythia_Pbp_sf0_EffPt.root",dirName));
			sampleName = "PRMC_Pbp";
		}
		else {
			//f01 = new TFile(Form("%s/NPMCpythia_Pbp_EffPt.root",dirName));
			//f02 = new TFile(Form("%s/NPMCembedded_Pbp_EffPt.root",dirName));
			//f01 = new TFile(Form("%s/NPMCpythia_Pbp_weight_EffPt.root",dirName));
			//f02 = new TFile(Form("%s/NPMCpythia_Pbp_noweight_EffPt.root",dirName));
			sampleName = "NPMC_Pbp";
		}	 
	}
	else {
		if (isPrompt) {
			//f01 = new TFile(Form("%s/PRMCpythia_pPb_EffPt.root",dirName));
			//f02 = new TFile(Form("%s/PRMCembedded_pPb_EffPt.root",dirName));
			sampleName = "PRMC_pPb";
		}
		else {
			//f01 = new TFile(Form("%s/NPMCpythia_pPb_EffPt.root",dirName));
			//f02 = new TFile(Form("%s/NPMCembedded_pPb_EffPt.root",dirName));
			sampleName = "NPMC_pPb";
		}	 
	}
	cout << "sampleName = " << sampleName << endl;

	// --- read-in tree
	TTree *tr = (TTree*)f01->Get("tRap");
	int nEdge;
	double rapEdge[531];
	TBranch *b_nEdge;
	TBranch *b_rapEdge;
	tr->SetBranchAddress("nEdge",&nEdge,&b_nEdge);
	tr->SetBranchAddress("rapEdge",&rapEdge,&b_rapEdge);
	cout << "tr entries = " << tr->GetEntries() << endl;
	for (int ev=0; ev <tr->GetEntries(); ev++){
		tr->GetEntry(ev);
	}
	cout << "nEdge = " << nEdge << endl;
	for (Int_t i=0; i< nEdge; i++) {
		cout << "rapEdge["<<i<<"] = " << rapEdge[i] << endl;
	}

	// --- read-in eff hist
	const int nRap = nEdge -1 ; 
	cout << "nRap = " << nRap << endl;
	TH1D* h1D_EffPt01[nRap]; 
	TH1D* h1D_EffPt02[nRap]; 
	TH1D *hRatio[nRap]; //f01/f02
	TH1D *hRel[nRap]; // (f01-f02)/f02
	for (int iy=0; iy < nRap; iy ++){
		h1D_EffPt01[iy] = (TH1D*)f01->Get(Form("h1D_EffPt_%d",iy));
		h1D_EffPt02[iy] = (TH1D*)f02->Get(Form("h1D_EffPt_%d",iy));
	cout << "h1D_EffPt01["<<iy<<"] = " << h1D_EffPt01[iy] << endl;
	cout << "h1D_EffPt02["<<iy<<"] = " << h1D_EffPt02[iy] << endl;
		hRatio[iy]=(TH1D*)h1D_EffPt01[iy]->Clone(Form("hRatio_%d",iy));
		hRatio[iy]->Divide(h1D_EffPt02[iy]);
		hRel[iy]=(TH1D*)h1D_EffPt01[iy]->Clone(Form("hRel_%d",iy));
		hRel[iy]->Add(h1D_EffPt02[iy],-1);
		hRel[iy]->Divide(h1D_EffPt02[iy]);
	}
	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms

//	TCanvas* c1 = new TCanvas("c1","c1",600,600);
//	TCanvas* c2 = new TCanvas("c2","c2",600,600);
//	TCanvas* c3 = new TCanvas("c3","c3",600,600);
	TCanvas* c1 = new TCanvas("c1","c1",1600,800);
	c1->Divide(4,2);
	TCanvas* c2 = new TCanvas("c2","c2",1600,800);
	c2->Divide(4,2);
	TCanvas* c3 = new TCanvas("c3","c3",1600,800);
	c3->Divide(4,2);
	//TLegend *legBR = new TLegend(0.58,0.30,0.85,0.40,NULL,"brNDC");
	TLegend *legBR = new TLegend(0.50,0.30,0.80,0.41,NULL,"brNDC");
	SetLegendStyle(legBR);

	for (Int_t iy = 0; iy < nRap; iy++) {
		//// 1)  distributions
		//c1->cd();
		c1->cd(iy+1);
		SetHistStyle(h1D_EffPt01[iy],3,0);
		SetHistStyle(h1D_EffPt02[iy],4,0);
		h1D_EffPt01[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1D_EffPt01[iy]->GetYaxis()->SetTitle("Efficiency");
		h1D_EffPt01[iy]->SetMinimum(0.);
		h1D_EffPt01[iy]->SetMaximum(1.);
		h1D_EffPt01[iy]->Draw("pe");
		h1D_EffPt02[iy]->Draw("pe same");
		if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,2,.8);
		else if (iy==2)  dashedLine (5.,0.,5.,1.,2,.8);
		else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,.8);
		latex->DrawLatex(0.53,0.25,sampleName);
		latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		if (iy==0) {
			legBR->AddEntry(h1D_EffPt01[iy],f01name,"lp");
			legBR->AddEntry(h1D_EffPt02[iy],f02name,"lp");
			legBR->Draw();
		}
		//c1->SaveAs(Form("%s/%s_compEff_%d.pdf",dirName02,sampleName,iy));
		//legBR->Clear();
		//c1->Clear();

		//// 2) ratio f01/f02
		//c2->cd();
		c2->cd(iy+1);
		SetHistStyle(hRatio[iy],5,0);
		hRatio[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		hRatio[iy]->GetYaxis()->SetTitle(Form("%s / %s",f01name,f02name));
		hRatio[iy]->SetMinimum(ratiomin);
		hRatio[iy]->SetMaximum(ratiomax);
		hRatio[iy]->Draw("pe");
		dashedLine(0.,1.,30.,1.,1,.8);
		if (iy==1 || iy==6) dashedLine (3.,ratiomin,3.,ratiomax,2,.8);
		else if (iy==2)  dashedLine (5.,ratiomin,5.,ratiomax,2,.8);
		else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,.8);
		latex->DrawLatex(0.53,0.25,sampleName);
		latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		//c2->SaveAs(Form("%s/%s_compRatio_%d.pdf",dirName02,sampleName,iy));
		//c2->Clear();

		//// 3) relative (f01-f02)/f02
		//c3->cd();
		c3->cd(iy+1);
		SetHistStyle(hRel[iy],5,0);
		hRel[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		hRel[iy]->GetYaxis()->SetTitle(Form("(%s-%s) / %s",f01name,f02name,f02name));
		hRel[iy]->SetMinimum(relmin);
		hRel[iy]->SetMaximum(relmax);
		hRel[iy]->Draw("pe");
		dashedLine(0.,0.,30.,0.,1,.8);
		if (iy==1 || iy==6) dashedLine (3.,relmin,3.,relmax,2,.8);
		else if (iy==2)  dashedLine (5.,relmin,5.,relmax,2,.8);
		else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,relmin,6.5,relmax,2,.8);
		latex->DrawLatex(0.53,0.25,sampleName);
		latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		//c3->SaveAs(Form("%s/%s_compRel_%d.pdf",dirName02,sampleName,iy));
		//c3->Clear();

	}

	c1->SaveAs(Form("%s/%s_compEff.pdf",dirName02,sampleName));
	c2->SaveAs(Form("%s/%s_compRatio.pdf",dirName02,sampleName));
	c3->SaveAs(Form("%s/%s_compRel.pdf",dirName02,sampleName));

	return;

}

