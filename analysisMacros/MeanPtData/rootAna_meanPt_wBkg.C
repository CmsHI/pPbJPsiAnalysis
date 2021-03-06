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

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);
void formEtArr(Double_t binmin, Double_t binmax, string* arr);

int rootAna_meanPt_wBkg(char *strBin = "8rap9pt"){

	//const int nbin = 100; //arbitrary binning for each pT hist.
	const int nbin = 20; //arbitrary binning for each pT hist.
	const int nEntry = 10000;
		
	gROOT->Macro("./JpsiStyle.C");

	// read-in root file (data)
	TFile *fDataPbp1; // 210498<= runNb <= 210658 should be rejected
	TFile *fDataPbp2; // only 210498<= runNb <= 210658 will be used : 1st 7 run (Reprocessed)
	TFile *fDatapPb;
	fDataPbp1 = new TFile("/home/songkyo/kyo/pPbDataSample/Data/pPbData_1st_ntuple_PromptReco-v1_GR_P_V43D_pileupRej_newAccCut_tot.root");
	fDataPbp2 = new TFile("/home/songkyo/kyo/pPbDataSample/Data/pPbData_1st_ntuple_PromptReco-v1_GR_P_V43F_pileupRej_newAccCut_tot.root");
	fDatapPb = new TFile("/home/songkyo/kyo/pPbDataSample/Data/pPbData_2nd_PromptReco-v1_GR_P_V43D_pileupRej_newAccCut_tot.root");

	TTree *treeDataPbp1 = (TTree*)fDataPbp1->Get("myTree");
	TTree *treeDataPbp2 = (TTree*)fDataPbp2->Get("myTree");
	TTree *treeDatapPb = (TTree*)fDatapPb->Get("myTree");

	/////////////////////////////////////////////////////////////	
	////////////// binning
	Double_t ptArr[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt
	const Int_t nPt = sizeof(ptArr)/sizeof(double)-1;
	cout << "nPt=" << nPt << endl;
	// in Ycm (will be change to 1st lab and 2nd lab later)
	Double_t yArr[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87}; // 8rap9pt
	const Int_t nRap = sizeof(yArr)/sizeof(double)-1;
	cout << "nRap=" << nRap << endl;
	string rapstrArr[nRap];
	for (int ir=0; ir<nRap; ir++){
		formRapArr(yArr[ir+1], yArr[ir], &rapstrArr[ir]);
		cout << "rap string arr = " << rapstrArr[ir].c_str() << endl;
	}
	Double_t etArr[] = {0.0, 120.0}; // 8rap9pt
	//Double_t etArr[] = {0.0, 20., 30., 120.0}; // ethf
	const Int_t nEt = sizeof(etArr)/sizeof(double)-1;
	cout << "nEt=" << nEt << endl;
	string etstrArr[nEt];
	for (int in=0; in<nEt; in++){
		 formEtArr(etArr[in], etArr[in+1], &etstrArr[in]);
		 cout << "et string arr = " << etstrArr[in].c_str() << endl;
	}

	/////////////////////////////////////////////////////////////	
	////////////// cut definitions
	TCut trigCut = "( (Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )"; 
	TCut recoCut = "Reco_QQ_size>=1 && Reco_QQ_sign==0";	
//	TCut massRange = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
	TCut massRange = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
	TCut massRangeBkg = "((2.6 < Reco_QQ_4mom.M() && Reco_QQ_4mom.M() <2.9) || (3.3 < Reco_QQ_4mom.M() && Reco_QQ_4mom.M() < 3.5))";
	TCut ctauRange = "Reco_QQ_ctau > -3.0 && Reco_QQ_ctau < 5.0 && Reco_QQ_ctauErr > 0.0 && Reco_QQ_ctauErr < 1.0"; 
	TCut accRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mupl_4mom.Eta())<1.2 && Reco_QQ_mupl_4mom.Pt()>=3.3) || ( 1.2<= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.1 && Reco_QQ_mupl_4mom.Pt() >= -(1.0/0.9)*TMath::Abs(Reco_QQ_mupl_4mom.Eta())+(1.2*(1.0/0.9)+2.6)) ||(2.1<=TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt()>=1.3) )";
	TCut accRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mumi_4mom.Eta())<1.2 && Reco_QQ_mumi_4mom.Pt()>=3.3) || ( 1.2<= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.1 && Reco_QQ_mumi_4mom.Pt() >= -(1.0/0.9)*TMath::Abs(Reco_QQ_mumi_4mom.Eta())+(1.2*(1.0/0.9)+2.6)) ||(2.1<=TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt()>=1.3) )";
	TCut ptRange[nEt][nRap][nPt];
	TCut rapRangePbp[nEt][nRap][nPt];
	TCut rapRangepPb[nEt][nRap][nPt];
	TCut totalCutPbp[nEt][nRap][nPt];
	TCut totalCutpPb[nEt][nRap][nPt];
	TCut totalCutBkgPbp[nEt][nRap][nPt];
	TCut totalCutBkgpPb[nEt][nRap][nPt];
	TCut runCut = "runNb >=210498 && runNb <= 210658"; //1st 7 run

	// forming TCut for diffential binning	
	for (int in=0; in<nEt; in++){
		for (int ir=0; ir<nRap; ir++ ) {
			for (int ipt=0; ipt<nPt; ipt++) {
				ptRange[in][ir][ipt] = Form("Reco_QQ_4mom.Pt()>%.1f && Reco_QQ_4mom.Pt()<%.1f",ptArr[ipt] , ptArr[ipt+1]);
				//change y_CM to  y_lab
				rapRangePbp[in][ir][ipt] = Form("-0.47-1.*Reco_QQ_4mom.Rapidity()>%.2f && -0.47-1.*Reco_QQ_4mom.Rapidity()<%.2f",yArr[ir+1] , yArr[ir]);
				rapRangepPb[in][ir][ipt] = Form("-0.47+Reco_QQ_4mom.Rapidity()>%.2f && -0.47+Reco_QQ_4mom.Rapidity()<%.2f",yArr[ir+1] , yArr[ir]);
				cout << "ptRange = " <<ptRange[in][ir][ipt] << endl;
				cout << "rapRangePbp = " <<rapRangePbp[in][ir][ipt] << endl;
				cout << "rapRangepPb = " <<rapRangepPb[in][ir][ipt] << endl;
				totalCutPbp[in][ir][ipt] = trigCut && recoCut && massRange && ctauRange && accRecoPlus && accRecoMinus && rapRangePbp[in][ir][ipt] && ptRange[in][ir][ipt];
				totalCutpPb[in][ir][ipt] = trigCut && recoCut && massRange && ctauRange && accRecoPlus && accRecoMinus && rapRangepPb[in][ir][ipt] && ptRange[in][ir][ipt];
				totalCutBkgPbp[in][ir][ipt] = trigCut && recoCut && massRangeBkg && ctauRange && accRecoPlus && accRecoMinus && rapRangePbp[in][ir][ipt] && ptRange[in][ir][ipt];
				totalCutBkgpPb[in][ir][ipt] = trigCut && recoCut && massRangeBkg && ctauRange && accRecoPlus && accRecoMinus && rapRangepPb[in][ir][ipt] && ptRange[in][ir][ipt];
			}
		}
	}


	/////////////////////////////////////////////////////////////	
	/////////////////////////////////////////////////////////////	
	// define 1D hist
	TH1D *hMeanPt[nEt][nRap][nPt];
	TH1D *hMeanPtBkg[nEt][nRap][nPt];
	double meanVal[nEt][nRap][nPt];
	double maxVal[nEt][nRap][nPt];
	double meanValBkg[nEt][nRap][nPt];
	double maxValBkg[nEt][nRap][nPt];

	//TCanvas* c1 = new TCanvas("c1","c1",600,600);
	TCanvas* cmulti[nEt][nRap];
	TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
	TLegend *legUM = new TLegend(0.30,0.68,0.65,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	for (int in=0; in<nEt; in++){
		for (int ir=0; ir<nRap; ir++ ) {
	 		cmulti[in][ir]= new TCanvas(Form("cmulti_%d_%d",in,ir),Form("%s",rapstrArr[ir].c_str()),1500,600);
			cmulti[in][ir]->Divide(5,2);
			for (int ipt=0; ipt<nPt; ipt++) {
				hMeanPt[in][ir][ipt] = new TH1D(Form("hMeanPt_%d_%d_%d",in, ir, ipt),";p_{T} (GeV/c);events", nbin, ptArr[ipt], ptArr[ipt+1]);
				hMeanPt[in][ir][ipt] ->Sumw2();
				SetHistStyle(hMeanPt[in][ir][ipt],4,0);
				hMeanPtBkg[in][ir][ipt] = new TH1D(Form("hMeanPtBkg_%d_%d_%d",in, ir, ipt),";p_{T} (GeV/c);events", nbin, ptArr[ipt], ptArr[ipt+1]);
				hMeanPtBkg[in][ir][ipt] ->Sumw2();
				SetHistStyle(hMeanPtBkg[in][ir][ipt],3,10);
				cmulti[in][ir]->cd(ipt+1);
				gPad->SetLogy(1);
/*				
				treeDataPbp1->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPt[in][ir][ipt]->GetName()),(totalCutPbp[in][ir][ipt]&&(!runCut)),"",nEntry);
				treeDataPbp2->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPt[in][ir][ipt]->GetName()),(totalCutPbp[in][ir][ipt]&&runCut),"",nEntry);
				treeDatapPb->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPt[in][ir][ipt]->GetName()),totalCutpPb[in][ir][ipt],"",nEntry);
				treeDataPbp1->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPtBkg[in][ir][ipt]->GetName()),(totalCutBkgPbp[in][ir][ipt]&&(!runCut)),"pe same",nEntry);
				treeDataPbp2->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPtBkg[in][ir][ipt]->GetName()),(totalCutBkgPbp[in][ir][ipt]&&runCut),"pe same",nEntry);
				treeDatapPb->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPtBkg[in][ir][ipt]->GetName()),totalCutBkgpPb[in][ir][ipt],"pe same",nEntry);
*/

				treeDataPbp1->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPt[in][ir][ipt]->GetName()),(totalCutPbp[in][ir][ipt]&&(!runCut)),"");
				treeDataPbp2->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPt[in][ir][ipt]->GetName()),(totalCutPbp[in][ir][ipt]&&runCut),"");
				treeDatapPb->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPt[in][ir][ipt]->GetName()),totalCutpPb[in][ir][ipt],"");
				treeDataPbp1->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPtBkg[in][ir][ipt]->GetName()),(totalCutBkgPbp[in][ir][ipt]&&(!runCut)),"pe same");
				treeDataPbp2->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPtBkg[in][ir][ipt]->GetName()),(totalCutBkgPbp[in][ir][ipt]&&runCut),"pe same");
				treeDatapPb->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPtBkg[in][ir][ipt]->GetName()),totalCutBkgpPb[in][ir][ipt],"pe same");

				meanVal[in][ir][ipt] =  hMeanPt[in][ir][ipt]->GetMean(1);
				maxVal[in][ir][ipt] = 50*hMeanPt[in][ir][ipt]->GetMaximum();
				meanValBkg[in][ir][ipt] =  hMeanPtBkg[in][ir][ipt]->GetMean(1);
				maxValBkg[in][ir][ipt] = 50*hMeanPtBkg[in][ir][ipt]->GetMaximum();
				hMeanPt[in][ir][ipt]->GetYaxis()->SetRangeUser(0.1,maxVal[in][ir][ipt]);
				hMeanPtBkg[in][ir][ipt]->GetYaxis()->SetRangeUser(0.1,maxValBkg[in][ir][ipt]);
				std::cout << hMeanPt[in][ir][ipt]->GetName() <<" : entries = " << hMeanPt[in][ir][ipt]->GetEntries() << std::endl;
				cout << " Cand <p_{T}> = " << meanVal[in][ir][ipt] << endl;			
				cout << " Bkg <p_{T}> = " << meanValBkg[in][ir][ipt] << endl;			
				latex->DrawLatex(0.54, 0.87, rapstrArr[ir].c_str());
				latex->DrawLatex(0.54, 0.80, Form("Cand <p_{T}> = %.2f (GeV/c)",meanVal[in][ir][ipt]));
				latex->DrawLatex(0.54, 0.72, Form("Bkg <p_{T}> = %.2f (GeV/c)",meanValBkg[in][ir][ipt]));
				//c1->Update();
				//c1->SaveAs(Form("%s.png",hMeanPt[in][ir][ipt]->GetName()));
				//c1->Clear();
			}
			cmulti[in][ir]->Update();
			cmulti[in][ir]->SaveAs(Form("dir_meanPt/meanPt_%s_%d_%d_wBkg.pdf",strBin,in,ir));
			//cmulti[in][ir]->Clear();
		}
	}
	
	/// Save as a root file
	TFile *outFile = new TFile(Form("meanPt_%s_wBkg.root",strBin),"RECREATE");
	std::cout << "strBin : " << strBin << std::endl;
	outFile->cd();
	for (int in=0; in<nEt; in++){
		for (int ir=0; ir<nRap; ir++ ) {
			for (int ipt=0; ipt<nPt; ipt++) {
				hMeanPt[in][ir][ipt]->Write();
				hMeanPtBkg[in][ir][ipt]->Write();
			}
		}
	}
	outFile->Close();

	return 0;	

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

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

void formEtArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < E_{T}^{HF} < %.0f GeV", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < E_{T}^{HF} < %.0f GeV", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < E_{T}^{HF} < %.1f GeV", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
	}
}

