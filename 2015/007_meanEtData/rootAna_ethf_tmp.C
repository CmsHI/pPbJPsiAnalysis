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
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "TRandom3.h"
#include "TClonesArray.h"
#include <TAxis.h>
#include <TLorentzRotation.h>
#include <TCut.h>

#include "KYOcommonOpt.h"


int rootAna_ethf_data(char *stringA = "6rap3pt"){

	// # of event range
	const int nEntry = 8000;
		
	gROOT->Macro("./JpsiStyle.C");

	// read-in root file (data)
	TFile *fDataPbp1; // 210498<= runNb <= 210658 should be rejected
	TFile *fDataPbp2; // only 210498<= runNb <= 210658 will be used : 1st 7 run
	TFile *fDatapPb;
	fDataPbp1 = new TFile("/home/songkyo/kyo/pPbDataSample/merged_pPbData_1st_ntuple_PromptReco-v1_GR_P_V43D_pileupRej_noMuID_tot.root");
	fDataPbp2 = new TFile("/home/songkyo/kyo/pPbDataSample/merged_pPbData_1st_ntuple_ReprocessedReco-v1_GR_P_V43F_pileupRej_muID_tot.root");
	fDatapPb = new TFile("/home/songkyo/kyo/pPbDataSample/merged_pPbData_2nd_ntuple_PromptReco-v1_GR_P_V43D_pileupRej_muID_tot.root");
	
	TTree *treeDataPbp1 = (TTree*)fDataPbp1->Get("myTree");
	TTree *treeDataPbp2 = (TTree*)fDataPbp2->Get("myTree");
	TTree *treeDatapPb = (TTree*)fDatapPb->Get("myTree");


	TFile *fDataMB;
	fDataMB = new TFile("/home/songkyo/kyo/pPbDataSample/MinBiasTree_pPb_211313-211631.root");
	TTree* treeDatapPb = new TTree();
	treeDatapPb->AddFriend("skimanalysis/HltTree",fDataMB);
	treeDatapPb->AddFriend("hiEvtAnalyzer/HiTree",fDataMB);

	//// Event selection
	//TCut trigCut = "HltTree.Event == HiTree.evt && HltTree.HLT_PAZeroBiasPixel_SingleTrack_v1==1";
	TCut evCut = "HltTree.pPAcollisionEventSelection==1 && HltTree.pVertexFilterCutGplus==1";

	// define 1D hist
	int etBin[] = {120, 20, 10, 90};
	double etmin[] = {0., 0., 20., 30.};
	double etmax[] = {120, 20, 30, 120};
	const Int_t nEtBin = sizeof(etBin)/sizeof(int);
	cout << "nEtBin = " << nEtBin << endl;
	
	TCanvas* c1 = new TCanvas("c1","c1",600,600);
	TLegend *legUR = new TLegend(0.60,0.70,0.87,0.87,NULL,"brNDC");
	TLegend *legUM = new TLegend(0.30,0.68,0.65,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);

	TH1D* hEtMB[nEtBin];
	for (Int_t i=0; i<nEtBin; i++){	
		hEtMB[i] = new TH1D(Form("hEtMB_%d",i),";E_{T}^{HF |#eta>4|};Events",etBin[i],etmin[i],etmax[i]);
		hEtMB[i]->Sumw2();
		treeDatapPb->Draw(Form("HiTree.hiHFeta4>>%s",hEtMB[i]->GetName()),evCut,"",nEntry);
		//treeDatapPb->Draw(Form("HiTree.hiHFeta4>>%s",hEtMB[i]->GetName()),evCut,"");
		std::cout << i<<"th hEtMB entries = " << hEtMB[i]->GetEntries() << std::endl;
		std::cout << i<<"th hEtMB integral = " << hEtMB[i]->Integral() << std::endl;
	}
		
	// Save data as a root file
	TFile *outFile = new TFile(Form("ethf_MB_%s.root",stringA),"RECREATE");
	std::cout << "stringA : " << stringA << std::endl;
	outFile->cd();
	for (Int_t i=0; i<nEtBin; i++){	
		hEtMB[i]->Write();
	}
	outFile->Close();
	

	// Draw normalized distributions to compare 1st & 2nd run
	c1->cd();

	for (Int_t i=0; i<nEtBin; i++){	
		hEtMB[i]->Scale(1/hEtMB[i]->Integral());	
		SetHistStyle(hEtMB[i],4,0);

		gPad->SetLogy(0);
		hEtMB[i]->Draw("pe");
		c1->SaveAs(Form("ethf_MB_%d.pdf",i));
		c1->Clear();
		legUR->Clear();
	
	}
	
	return 0;	

}

/*
	using namespace std;

	// Definition of binning
	// --- pt Bin
	Double_t ptBinsArr1[] = {6.5, 30.0}; //1bin
	Double_t ptBinsArr2[] = {6.5, 7.5, 8.5, 9.5, 11.0, 14.0, 30.0}; //6bin
	const Int_t nPtBins1 = sizeof(ptBinsArr1)/sizeof(double)-1;
	const Int_t nPtBins2 = sizeof(ptBinsArr2)/sizeof(double)-1;
	cout << "nPtBins1=" << nPtBins1 << endl;
	cout << "nPtBins2=" << nPtBins2 << endl;

	// --- y Bin
	Double_t yBinsArr1[] = {-2.4, -1.97, -1.72, -1.47, -1.22, -0.97, -0.72, -0.47, -0.22, 0.03, 0.28, 0.53, 0.78, 1.03, 1.46}; //14bin
	Double_t yBinsArr2[] = {-2.4, -0.47, 1.46}; //2bin
	Double_t yBinsArr3[] = {-2.4, 1.46}; //rap integ
	const Int_t nYBins1 = sizeof(yBinsArr1)/sizeof(double)-1;
	const Int_t nYBins2 = sizeof(yBinsArr2)/sizeof(double)-1;
	const Int_t nYBins3 = sizeof(yBinsArr3)/sizeof(double)-1;
	cout << "nYBins1=" << nYBins1 << endl;
	cout << "nYBins2=" << nYBins2 << endl;
	cout << "nYBins3=" << nYBins2 << endl;
*/

