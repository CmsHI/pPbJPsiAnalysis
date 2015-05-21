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
#include <fstream>
#include "TString.h"

#include "KYOcommonOpt.h"

void draw_comp_cent(bool isPrompt=true, bool isPair=true, bool isEmbedded=true, bool isBinomial=true, char *dirName = "drawComprm3step")
{

	gROOT->Macro("./JpsiStyle.C");
	gStyle->SetPaintTextFormat(".3f"); // for text colz

	char* ntuple01 = "default 7_4_0";
	char* ntuple02 = "3 steps removed";
	//char* ntuple02 = "muonSeeded";

	double ymin = 0.85;
	double ymax = 1.15;

	string strPrompt;
	string strEmbd;
	string strPair;
	if(isPrompt){ strPrompt = "prompt"; }
	else { strPrompt = "nonprompt"; }
	if(isEmbedded){ strEmbd = "embd";	}
	else { strEmbd = "nonembd"; }
	if(isPair){ strPair = "Pair"; }
	else { strPair = "Trk"; }
	cout << "strPrompt = " << strPrompt.c_str() << endl;
	cout << "strEmbd = " << strEmbd.c_str() << endl;
	cout << "strPair = " << strPair.c_str() << endl;

	TFile * refFile01;
	TFile * refFile02;

	//refFile01 = new TFile(Form("./eff_74X_setBr_wDetached/Eff_isPtCut0_doWeight0_isEmbedded1_%s_embd_%s.root",strPrompt.c_str(),strPair.c_str()));
	refFile01 = new TFile(Form("./eff_cent_setBr_goni/Eff_%s_embd_%s.root",strPrompt.c_str(),strPair.c_str()));
	refFile02 = new TFile(Form("./eff_cent_setBr_rm3step/Eff_%s_embd_%s.root",strPrompt.c_str(),strPair.c_str()));
	//refFile02 = new TFile(Form("./eff_cent_setBr_muonSeeded/Eff_%s_embd_%s.root",strPrompt.c_str(),strPair.c_str()));

	TH1D * h1D_EffPt_01 = (TH1D*)refFile01->Get("hEffPt");
	TH1D * h1D_EffPt_02 = (TH1D*)refFile02->Get("hEffPt");
	TH1D * h1D_EffRap_01 = (TH1D*)refFile01->Get("hEffRap");
	TH1D * h1D_EffRap_02 = (TH1D*)refFile02->Get("hEffRap");
	TH1D * h1D_EffLxy_01 = (TH1D*)refFile01->Get("hEffLxy");
	TH1D * h1D_EffLxy_02 = (TH1D*)refFile02->Get("hEffLxy");
	TH1D * h1D_EffCent_01 = (TH1D*)refFile01->Get("hEffCent");
	TH1D * h1D_EffCent_02 = (TH1D*)refFile02->Get("hEffCent");
	h1D_EffPt_01->SetName("h1D_EffPt_01");
	h1D_EffPt_02->SetName("h1D_EffPt_02");
	h1D_EffRap_01->SetName("h1D_EffRap_01");
	h1D_EffRap_02->SetName("h1D_EffRap_02");
	h1D_EffLxy_01->SetName("h1D_EffLxy_01");
	h1D_EffLxy_02->SetName("h1D_EffLxy_02");
	h1D_EffCent_01->SetName("h1D_EffCent_01");
	h1D_EffCent_02->SetName("h1D_EffCent_02");

	TH1D* hRatio_EffPt = (TH1D*)h1D_EffPt_02->Clone("hRatio_EffPt");
	if (isBinomial) hRatio_EffPt->Divide(h1D_EffPt_02,h1D_EffPt_01,1,1,"B");
	else hRatio_EffPt->Divide(h1D_EffPt_01);
	TH1D* hRatio_EffRap = (TH1D*)h1D_EffRap_02->Clone("hRatio_EffRap");
	if (isBinomial) hRatio_EffRap->Divide(h1D_EffRap_02,h1D_EffRap_01,1,1,"B");
	else hRatio_EffRap->Divide(h1D_EffRap_01);
	TH1D* hRatio_EffLxy = (TH1D*)h1D_EffLxy_02->Clone("hRatio_EffLxy");
	if (isBinomial) hRatio_EffLxy->Divide(h1D_EffLxy_02,h1D_EffLxy_01,1,1,"B");
	else hRatio_EffLxy->Divide(h1D_EffLxy_01);
	TH1D* hRatio_EffCent = (TH1D*)h1D_EffCent_02->Clone("hRatio_EffCent");
	if (isBinomial) hRatio_EffCent->Divide(h1D_EffCent_02,h1D_EffCent_01,1,1,"B");
	else hRatio_EffCent->Divide(h1D_EffCent_01);

	//latex box for beam, rapidity, pT info
	TLatex* tex = new TLatex();
	tex->SetNDC();
	tex->SetTextAlign(12);
	tex->SetTextSize(0.04);

	//Legend
	TLegend *legUR = new TLegend(0.56,0.68,0.90,0.90,NULL,"brNDC");
	//TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.20,0.71,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.40,0.20,0.75,0.35,NULL,"brNDC");
	TLegend *legBR = new TLegend(0.56,0.20,0.90,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);
	SetLegendStyle(legBR);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms
	TCanvas* c1 = new TCanvas("c1","c1",600,600);
	
	////// 01 Pt
	c1->cd();
	h1D_EffPt_01->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	h1D_EffPt_01->GetXaxis()->CenterTitle();
	h1D_EffPt_01->GetYaxis()->SetTitle("Efficiency");
	//h1D_EffPt_01->GetXaxis()->SetRangeUser(0,16);
	if (isPrompt) h1D_EffPt_01->GetXaxis()->SetRangeUser(0,12);
	else  h1D_EffPt_01->GetXaxis()->SetRangeUser(0,22);
	h1D_EffPt_01->SetMinimum(0);	
	h1D_EffPt_01->SetMaximum(1);	
	SetHistStyle(h1D_EffPt_01,3,0);
	SetHistStyle(h1D_EffPt_02,4,10);
	h1D_EffPt_01->Draw("");
	h1D_EffPt_02->Draw("same");
	if (isPair){
		legUL->SetHeader(strPrompt.c_str());
		legUL->AddEntry(h1D_EffPt_01,ntuple01,"lp");
		legUL->AddEntry(h1D_EffPt_02,ntuple02,"lp");
		legUL->Draw();
	}else{
		legBR->SetHeader(strPrompt.c_str());
		legBR->AddEntry(h1D_EffPt_01,ntuple01,"lp");
		legBR->AddEntry(h1D_EffPt_02,ntuple02,"lp");
		legBR->Draw();
	}
	c1->SaveAs(Form("%s/comp_EffPt_%s_%s.pdf",dirName,strPrompt.c_str(),strPair.c_str()));
//	c1->SaveAs(Form("%s/comp_EffPt_%s_%s.png",dirName,strPrompt.c_str(),strPair.c_str()));	
	legUL->Clear();
	legBR->Clear();
	c1->Clear();

	if (isPair) hRatio_EffPt->GetXaxis()->SetTitle("p_{T}^{#mu#mu} (GeV/c)");	
	else hRatio_EffPt->GetXaxis()->SetTitle("p_{T}^{#mu} (GeV/c)");	
	hRatio_EffPt->GetXaxis()->CenterTitle();	
	if (isPrompt) hRatio_EffPt->GetXaxis()->SetRangeUser(0,12);
	else  hRatio_EffPt->GetXaxis()->SetRangeUser(0,22);
	hRatio_EffPt->GetYaxis()->SetTitle(Form("Ratio = [ %s ]/[ %s ]",ntuple02,ntuple01));	
	hRatio_EffPt->SetMinimum(ymin);	
	hRatio_EffPt->SetMaximum(ymax);	
	hRatio_EffPt->Draw();	
	SetHistStyle(hRatio_EffPt,2,0);
	//dashedLine(0.,1.,16.,1.,1,1);
	if (isPrompt) dashedLine(0.,1.,12.,1.,1,1);
	else dashedLine(0.,1.,22.,1.,1,1);
	c1->SaveAs(Form("%s/compRatio_EffPt_%s_%s.pdf",dirName,strPrompt.c_str(),strPair.c_str()));	
//	c1->SaveAs(Form("%s/compRatio_EffPt_%s_%s.png",dirName,strPrompt.c_str(),strPair.c_str()));
	c1->Clear();

	////// 02 Y
	c1->cd();
	if (isPair) h1D_EffRap_01->GetXaxis()->SetTitle("y");
	else  h1D_EffRap_01->GetXaxis()->SetTitle("#eta");
	h1D_EffRap_01->GetXaxis()->CenterTitle();
	h1D_EffRap_01->GetYaxis()->SetTitle("Efficiency");
	h1D_EffRap_01->GetXaxis()->SetRangeUser(-2.5,2.5);
	h1D_EffRap_01->SetMinimum(0);	
	h1D_EffRap_01->SetMaximum(1);	
	SetHistStyle(h1D_EffRap_01,3,0);
	SetHistStyle(h1D_EffRap_02,4,10);
	h1D_EffRap_01->Draw("");
	h1D_EffRap_02->Draw("same");
	if (isPair){
		legUL->SetHeader(strPrompt.c_str());
		legUL->AddEntry(h1D_EffPt_01,ntuple01,"lp");
		legUL->AddEntry(h1D_EffPt_02,ntuple02,"lp");
		legUL->Draw();
	}else{
		legBM->SetHeader(strPrompt.c_str());
		legBM->AddEntry(h1D_EffPt_01,ntuple01,"lp");
		legBM->AddEntry(h1D_EffPt_02,ntuple02,"lp");
		legBM->Draw();
	}
	c1->SaveAs(Form("%s/comp_EffRap_%s_%s.pdf",dirName,strPrompt.c_str(),strPair.c_str()));
//	c1->SaveAs(Form("%s/comp_EffRap_%s_%s.png",dirName,strPrompt.c_str(),strPair.c_str()));	
	legUL->Clear();
	legBM->Clear();
	c1->Clear();

	if (isPair) hRatio_EffRap->GetXaxis()->SetTitle("y");
	else  hRatio_EffRap->GetXaxis()->SetTitle("#eta");
	hRatio_EffRap->GetXaxis()->CenterTitle();	
	hRatio_EffRap->GetXaxis()->SetRangeUser(-2.5,2.5);
	hRatio_EffRap->GetYaxis()->SetTitle(Form("Ratio = [ %s ]/[ %s ]",ntuple02,ntuple01));	
	hRatio_EffRap->SetMinimum(ymin);	
	hRatio_EffRap->SetMaximum(ymax);	
	hRatio_EffRap->Draw();	
	SetHistStyle(hRatio_EffRap,2,0);
	dashedLine(-2.5,1.,2.5,1.,1,1);
	c1->SaveAs(Form("%s/compRatio_EffRap_%s_%s.pdf",dirName,strPrompt.c_str(),strPair.c_str()));	
//	c1->SaveAs(Form("%s/compRatio_EffRap_%s_%s.png",dirName,strPrompt.c_str(),strPair.c_str()));
	c1->Clear();

	////// 03 Lxy
	c1->cd();
	h1D_EffLxy_01->GetXaxis()->SetTitle("L_{xy}");
	h1D_EffLxy_01->GetXaxis()->CenterTitle();
	h1D_EffLxy_01->GetYaxis()->SetTitle("Efficiency");
	h1D_EffLxy_01->GetXaxis()->SetRangeUser(0,1);
	h1D_EffLxy_01->SetMinimum(0);	
	h1D_EffLxy_01->SetMaximum(1);	
	SetHistStyle(h1D_EffLxy_01,3,0);
	SetHistStyle(h1D_EffLxy_02,4,10);
	h1D_EffLxy_01->Draw("");
	h1D_EffLxy_02->Draw("same");
	if (isPair){
		legUL->SetHeader(strPrompt.c_str());
		legUL->AddEntry(h1D_EffPt_01,ntuple01,"lp");
		legUL->AddEntry(h1D_EffPt_02,ntuple02,"lp");
		legUL->Draw();
	}else{
		legBM->SetHeader(strPrompt.c_str());
		legBM->AddEntry(h1D_EffPt_01,ntuple01,"lp");
		legBM->AddEntry(h1D_EffPt_02,ntuple02,"lp");
		legBM->Draw();
	}
	c1->SaveAs(Form("%s/comp_EffLxy_%s_%s.pdf",dirName,strPrompt.c_str(),strPair.c_str()));
//	c1->SaveAs(Form("%s/comp_EffLxy_%s_%s.png",dirName,strPrompt.c_str(),strPair.c_str()));	
	legUL->Clear();
	legBM->Clear();
	c1->Clear();

	hRatio_EffLxy->GetXaxis()->SetTitle("L_{xy}");	
	hRatio_EffLxy->GetXaxis()->CenterTitle();	
	hRatio_EffLxy->GetXaxis()->SetRangeUser(0,1);
	hRatio_EffLxy->GetYaxis()->SetTitle(Form("Ratio = [ %s ]/[ %s ]",ntuple02,ntuple01));	
	hRatio_EffLxy->SetMinimum(ymin);	
	hRatio_EffLxy->SetMaximum(ymax);	
	hRatio_EffLxy->Draw();	
	SetHistStyle(hRatio_EffLxy,2,0);
	dashedLine(0.,1.,1.,1.,1,1);
	c1->SaveAs(Form("%s/compRatio_EffLxy_%s_%s.pdf",dirName,strPrompt.c_str(),strPair.c_str()));	
//	c1->SaveAs(Form("%s/compRatio_EffLxy_%s_%s.png",dirName,strPrompt.c_str(),strPair.c_str()));
	c1->Clear();

	////// 04 Cent
	c1->cd();
	h1D_EffCent_01->GetXaxis()->SetTitle("Centrality");
	h1D_EffCent_01->GetXaxis()->CenterTitle();
	h1D_EffCent_01->GetYaxis()->SetTitle("Efficiency");
	h1D_EffCent_01->GetXaxis()->SetRangeUser(0,200);
	h1D_EffCent_01->SetMinimum(0);	
	h1D_EffCent_01->SetMaximum(1);	
	SetHistStyle(h1D_EffCent_01,3,0);
	SetHistStyle(h1D_EffCent_02,4,10);
	h1D_EffCent_01->Draw("");
	h1D_EffCent_02->Draw("same");
	if (isPair){
		legUL->SetHeader(strPrompt.c_str());
		legUL->AddEntry(h1D_EffPt_01,ntuple01,"lp");
		legUL->AddEntry(h1D_EffPt_02,ntuple02,"lp");
		legUL->Draw();
	}else{
		legBM->SetHeader(strPrompt.c_str());
		legBM->AddEntry(h1D_EffPt_01,ntuple01,"lp");
		legBM->AddEntry(h1D_EffPt_02,ntuple02,"lp");
		legBM->Draw();
	}
	c1->SaveAs(Form("%s/comp_EffCent_%s_%s.pdf",dirName,strPrompt.c_str(),strPair.c_str()));
//	c1->SaveAs(Form("%s/comp_EffCent_%s_%s.png",dirName,strPrompt.c_str(),strPair.c_str()));	
	legUL->Clear();
	legBM->Clear();
	c1->Clear();

	hRatio_EffCent->GetXaxis()->SetTitle("Centrality");	
	hRatio_EffCent->GetXaxis()->CenterTitle();	
	hRatio_EffCent->GetXaxis()->SetRangeUser(0,200);
	hRatio_EffCent->GetYaxis()->SetTitle(Form("Ratio = [ %s ]/[ %s ]",ntuple02,ntuple01));	
	hRatio_EffCent->SetMinimum(ymin);	
	hRatio_EffCent->SetMaximum(ymax);	
	hRatio_EffCent->Draw();	
	SetHistStyle(hRatio_EffCent,2,0);
	dashedLine(0.,1.,200.,1.,1,1);
	c1->SaveAs(Form("%s/compRatio_EffCent_%s_%s.pdf",dirName,strPrompt.c_str(),strPair.c_str()));	
//	c1->SaveAs(Form("%s/compRatio_EffCent_%s_%s.png",dirName,strPrompt.c_str(),strPair.c_str()));
	c1->Clear();



	TFile* outFile = new TFile(Form("%s/compRatio_%s_%s.root",dirName,strPrompt.c_str(),strPair.c_str()), "RECREATE");
	outFile->cd();
	hRatio_EffPt->Write();
	hRatio_EffRap->Write();
	hRatio_EffLxy->Write();
	hRatio_EffCent->Write();
	outFile->Close();

	return;

}

