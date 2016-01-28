#include "KYOcommonOpt.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <string>

#include <TROOT.h>
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>
#include <TNtuple.h>
#include <TMath.h>
#include <math.h>
#include <cmath>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "TRandom3.h"
#include "TClonesArray.h"
#include <TAxis.h>
#include <TLorentzRotation.h>
#include <TCut.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>

double tfGaus(double *x, double *par);
double tfDivideGaus(double *x, double *par);
double tfLine(double *x, double *par);

int rootAna_zVtxFit(char *stringA = "20141007", bool isBoosted = true){

	gROOT->Macro("./JpsiStyle.C");
	//gStyle->SetCanvasDefW(800);

	// read-in root file
	TFile *f01 = new TFile("/home/songkyo/kyo/pPbDataSample/merged_pPbData_1st_ntuple_PromptReco-v1_GR_P_V43D_pileupRej_muID_tot.root");
	TFile *f02 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root");

	//read-in tree
	TTree *tree01 = (TTree*)f01->Get("myTree");
	TTree *tree02 = (TTree*)f02->Get("myTree");

	// cut definitions
/*
	TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )";
	//TCut recoCut = "Reco_QQ_size>=1";	
	TCut recoCut = "Reco_QQ_size>=1 && Reco_QQ_sign==0";	
	TCut massRange = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
	TCut rapRangePbp = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < 2.4";
	TCut rapRangepPb = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < 2.4";
	TCut ptRange = "Reco_QQ_4mom.Pt() > 0.0 && Reco_QQ_4mom.Pt() < 30.0";
	TCut etaPbp = "Reco_QQ_mupl_4mom.Eta()>-2.4 && Reco_QQ_mupl_4mom.Eta()<2.4 && Reco_QQ_mumi_4mom.Eta()>-2.4 && Reco_QQ_mumi_4mom.Eta()<2.4";
	TCut etapPb = "Reco_QQ_mupl_4mom.Eta()>-2.4 && Reco_QQ_mupl_4mom.Eta()<2.4 && Reco_QQ_mumi_4mom.Eta()>-2.4 && Reco_QQ_mumi_4mom.Eta()<2.4";
	TCut accRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mupl_4mom.Eta())<1.3 && Reco_QQ_mupl_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.2 && sqrt(Reco_QQ_mupl_4mom.Px()*Reco_QQ_mupl_4mom.Px()+ Reco_QQ_mupl_4mom.Py()*Reco_QQ_mupl_4mom.Py() + Reco_QQ_mupl_4mom.Pz()*Reco_QQ_mupl_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt()>=0.8) )";
	TCut accRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mumi_4mom.Eta())<1.3 && Reco_QQ_mumi_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.2 && sqrt(Reco_QQ_mumi_4mom.Px()*Reco_QQ_mumi_4mom.Px()+ Reco_QQ_mumi_4mom.Py()*Reco_QQ_mumi_4mom.Py() + Reco_QQ_mumi_4mom.Pz()*Reco_QQ_mumi_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt()>=0.8) )";
	TCut accReco = accRecoPlus && accRecoMinus;

	TCut runCut = "runNb >=210498 && runNb <= 210658"; 
	TCut totalCutPbp = trigCut && recoCut && massRange && rapRangePbp && etaPbp && ptRange && accReco;
	TCut totalCutpPb = trigCut && recoCut && massRange && rapRangepPb && etapPb && ptRange && accReco;
*/

	// define 1D hist
	const int nbin[] = {80};
	const double minbin[] = {-30};
	const double maxbin[] = {+30};
	TString histName[] = {"zVtx"};
//	TString histTitle[] = {"mass (GeV/c^{2})", "p_{T} (GeV/c)", "y_{CM}", "#phi (rad)", "primary vertex Z (cm)"};
	TString histTitle[] = {"primary vertex Z"};
	const int nHist = sizeof(nbin)/sizeof(int);
	cout << "nHist = "<<nHist<<endl;
	TH1D *h01[nHist];
	TH1D *h02[nHist];
	TH1D *hRatio[nHist];

	//define fitting function
	TF1* gCommon = new TF1("gCommon","gaus",-30., 30.); //prefit to extract param
	TF1* g1 = new TF1("g1",tfGaus,-30., 30.,3);
	TF1* g2 = new TF1("g2",tfGaus,-30., 30.,3);
	TF1* g3 = new TF1("g3",tfGaus,-30., 30.,3);
	Double_t parC[3]; // for Common
	Double_t par01[3]; //for h01
	Double_t par02[3]; //for h02
	Double_t par03[3]; //for hRatio
	
	TF1* gRatio = new TF1("gRatio",tfDivideGaus,-30., 30.,6);
	Double_t parRatio[6]; //for hRatio

	const int nEntry = 5000;
	TCanvas* c0[nHist];
	TCanvas* c1[nHist];
	//TLegend *legU = new TLegend(0.17,0.71,0.75,0.90,NULL,"brNDC");
	TLegend *legU = new TLegend(0.17,0.76,0.80,0.90,NULL,"brNDC");
	legU-> SetNColumns(2);
	SetLegendStyle(legU);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);


	for (int i=0; i<nHist; i++) {
		h01[i] = new TH1D(Form("h01_%s_%d",histName[i].Data(),i),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		h02[i] = new TH1D(Form("h02_%s_%d",histName[i].Data(),i),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		h01[i]->Sumw2();
		h02[i]->Sumw2();
		std::cout << i <<"th hist name = " << histName[i].Data() << std::endl;
		tree01->Draw(Form("%s>>%s",histName[i].Data(),h01[i]->GetName()),"","");
		tree02->Draw(Form("%s>>%s",histName[i].Data(),h02[i]->GetName()),"","");
	
		std::cout << histName[i].Data() <<" : h01 entries = " << h01[i]->GetEntries() << std::endl;
		std::cout << histName[i].Data() <<" : h02 entries = " << h02[i]->GetEntries() << std::endl;
		//std::cout << histName[i].Data() <<" : h01 integral = " << h01[i]->Integral() << std::endl;
		//std::cout << histName[i].Data() <<" : h02 integral = " << h02[i]->Integral() << std::endl;
		h01[i]->Scale(1/h01[i]->Integral());
		h02[i]->Scale(1/h02[i]->Integral());

		//prefit
		h01[i]->Fit("gCommon", "N");
		gCommon->GetParameters(&parC[0]);
		g1->SetParLimits(0, parC[0]-0.01,parC[0]+0.01);
		g1->SetParLimits(1, parC[1]-0.01,parC[1]+0.01);
		g1->SetParLimits(2, parC[2]-0.01,parC[2]+0.01);
		h02[i]->Fit("gCommon", "N");
		gCommon->GetParameters(&parC[0]);
		g2->SetParLimits(0, parC[0]-0.01,parC[0]+0.01);
		g2->SetParLimits(1, parC[1]-0.01,parC[1]+0.01);
		g2->SetParLimits(2, parC[2]-0.01,parC[2]+0.01);

		//Draw and Fit
		c1[i] = new TCanvas(Form("c1_%s",histName[i].Data()),"",600,600);
		c1[i]->Clear();
		c1[i]->cd();
		gPad->SetLogy(0);
		SetHistStyle(h01[i],3,10);
		SetHistStyle(h02[i],4,11);
		h01[i]->SetFillColor(kOrange+7);
		h01[i]->SetFillStyle(3003);
		h02[i]->SetFillColor(kGreen+3);
		h02[i]->SetFillStyle(3005);
		h01[i]->SetAxisRange(0.0,0.08,"Y");

		h01[i]->Draw("pe");
		g1->SetLineColor(kOrange+7);
		h01[i]->Fit("g1","N");
		g1->Draw("same");
		g1->GetParameters(&par01[0]);
		latex->DrawLatex(0.20, 0.77, Form("mean : %.2f",par01[1]));	
		latex->DrawLatex(0.20, 0.71, Form("sigma : %.2f",par01[2]));
		
		h02[i]->Draw("pe same");
		g2->SetLineColor(kGreen+3);
		h02[i]->Fit("g2","N");
		g2->Draw("same");
		g2->GetParameters(&par02[0]);
		latex->DrawLatex(0.67, 0.77, Form("mean : %.2f",par02[1]));	
		latex->DrawLatex(0.67, 0.71, Form("sigma : %.2f",par02[2]));
		
		legU->AddEntry(h01[i],"Data_1st      ","lp");
		legU->AddEntry(h02[i],"promptMC_1st","lp");
		legU->Draw();

		c1[i]->SaveAs(Form("zVtxFit_%s_%s.pdf",histName[i].Data(),stringA));
		c1[i]->SaveAs(Form("zVtxFit_%s_%s.png",histName[i].Data(),stringA));
		c1[i]->Clear();
		legU->Clear();

	// divide two TF
	gRatio->SetParameters(par01[0],par01[1],par01[2],par02[0],par02[1],par02[2]);
	//cout <<"check param 0 = " << gRatio->GetParameter(0) << endl;		
	//cout <<"check param 1 = " << gRatio->GetParameter(1) << endl;		
	//cout <<"check param 2 = " << gRatio->GetParameter(2) << endl;		
	//cout <<"check param 3 = " << gRatio->GetParameter(3) << endl;		
	//cout <<"check param 4 = " << gRatio->GetParameter(4) << endl;		
	//cout <<"check param 5 = " << gRatio->GetParameter(5) << endl;		
	gRatio->SetLineColor(kViolet-4);

	//ratio
	gPad->SetLogy(0);
	hRatio[i] = (TH1D*)h01[i]->Clone(Form("hRatio_%d",i));
	hRatio[i]->Divide(h02[i]);
	hRatio[i]->GetYaxis()->SetRangeUser(0.,2.0);
	hRatio[i]->GetYaxis()->SetTitle("Ratio = Data/promptMC");
	SetHistStyle(hRatio[i],1,0);
	//hRatio[i]->SetMarkerColor(kWhite);
	hRatio[i]->SetMarkerColor(kBlack);
	hRatio[i]->Draw("pe");
/*	
	//just cross check w/ gaussian : fit the ratio itself
	hRatio[i]->Fit("gCommon", "N");
	gCommon->GetParameters(&parC[0]);
	g3->SetParLimits(0, parC[0]-0.01,parC[0]+0.01);
	g3->SetParLimits(1, parC[1]-0.01,parC[1]+0.01);
	g3->SetParLimits(2, parC[2]-0.01,parC[2]+0.01);
	g3->SetLineColor(kBlack);
	hRatio[i]->Fit("g3","N");
	g3->GetParameters(&par03[0]);
	//latex->DrawLatex(0.20, 0.77, Form("mean : %.2f",par03[1]));	
	//latex->DrawLatex(0.20, 0.71, Form("sigma : %.2f",par03[2]));
	g3->Draw("same");
*/
	gRatio->Draw("same");
	
	dashedLine(minbin[i],1.,maxbin[i],1.,1,1);
	c1[i]->SaveAs(Form("zVtxFit_ratio_%s_%s.pdf",histName[i].Data(),stringA));
	c1[i]->SaveAs(Form("zVtxFit_ratio_%s_%s.png",histName[i].Data(),stringA));
	c1[i]->Clear();

	}

	//Save as a root file
	TFile *outFile = new TFile(Form("zVtxFit_%s.root",stringA),"RECREATE");		
	outFile->cd();
	for (int i=0; i<nHist; i++) {
		h01[i]->Write();
		h02[i]->Write();
		hRatio[i]->SetMarkerColor(kRed);
		hRatio[i]->Write();
	}
	g1->Write();
	g2->Write();
	gRatio->Write();
	outFile->Close();

	return 0;	

}

double tfGaus(double *x, double *par) {
//	return par[0]*TMath::Exp( -0.5*( (x[0]-par[1])^2/(par[2])^2 ) );
//	return par[0]*TMath::Exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) );
	return par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) );
}

double tfDivideGaus(double *x, double *par) {
	return (par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) )) / (par[3]*exp( -0.5*( ((x[0]-par[4])*(x[0]-par[4]))/(par[5]*par[5]) )) );
}


double tfLine(double *x, double *par) {
	return par[0]*x[0]+par[1];
}
