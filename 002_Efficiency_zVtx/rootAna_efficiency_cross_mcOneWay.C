#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>

#include <TROOT.h>
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
#include <math.h>
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
#include <cmath>
#include <TLorentzRotation.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>

const double shiftvar = -0.47; // conversion constant y=0(collision)==y=-0.47(LAB frame)  

bool kineCut(bool, double muPt, double muEta, double muP);
bool massCut1(double lv_dimu_mass);
bool massCut2(double lv_dimu_mass);
void formRapStr(Double_t min, Double_t max, string* arr);
void formStr(Double_t min, Double_t max, string* arr);
float getEffWeight(bool isBoosted=0,  float mupt1=0, float mueta1=0, float mupt2=0, float mueta2=0, int runId=1); // runId 1 = 1st run(Pbp).  runId 2 = 2nd run(pPb)

struct Condition {
  double theCentrality; 
  int theType, theSign, Reco_QQ_trig, HLTriggers;
  double theMass, theRapidity, thePt, thePhi;
  double mupl_p, mumi_p, mupl_pt, mumi_pt, mupl_eta, mumi_eta;
  float theCtErr;
//	float theZvtx;
} ;

//read TNP plots for useDataDrivenEff
TFile* fEffWeight1st1 = new TFile("triggerRatio/WeightFactor_total_etabin1CS_1st_12bin_20140327.root");
TH1D* hEffCorr1st1 = (TH1D*)fEffWeight1st1->Get("hWF");
TFile* fEffWeight1st2 = new TFile("triggerRatio/WeightFactor_total_etabin2CS_1st_12bin_20140327.root");
TH1D* hEffCorr1st2 = (TH1D*)fEffWeight1st2->Get("hWF");
TFile*fEffWeight1st3 = new TFile("triggerRatio/WeightFactor_total_etabin3CS_1st_12bin_20140327.root");
TH1D* hEffCorr1st3 = (TH1D*)fEffWeight1st3->Get("hWF");
TFile* fEffWeight2nd1 = new TFile("triggerRatio/WeightFactor_total_etabin1CS_1st_12bin_20140327.root");
TH1D* hEffCorr2nd1 = (TH1D*)fEffWeight2nd1->Get("hWF");
TFile* fEffWeight2nd2 = new TFile("triggerRatio/WeightFactor_total_etabin2CS_1st_12bin_20140327.root");
TH1D* hEffCorr2nd2 = (TH1D*)fEffWeight2nd2->Get("hWF");
TFile*fEffWeight2nd3 = new TFile("triggerRatio/WeightFactor_total_etabin3CS_1st_12bin_20140327.root");
TH1D* hEffCorr2nd3 = (TH1D*)fEffWeight2nd3->Get("hWF");

//read zTtx functions for useZvtxWeight 
TFile* fZvtx = new TFile("zVtxFit/zVtxFit_20141007.root");
TF1* gRatio = (TF1*)fZvtx->Get("gRatio");

int rootAna_efficiency_cross_mcOneWay(char *strBinning = "crossFiner", bool isBoosted = true, bool isPrompt = 1, bool useCtErrRangeEff =false, bool useDataDrivenEff=false, bool useZvtxWeight=false){

  using namespace std;
  
  // # of event range
  int initev =0;
  int nevt = -1; //all
  //int nevt = 100;

	TCanvas* c0 = new TCanvas("c0","",900,400);
	c0->Divide(3,1);
	c0->cd(1);
	hEffCorr1st1->Draw();
	c0->cd(2);
	hEffCorr1st2->Draw();
	c0->cd(3);
	hEffCorr1st3->Draw();
	c0->SaveAs("wieght1.gif");
	TCanvas* c00 = new TCanvas("c00","",900,400);
	c00->Divide(3,1);
	c00->cd(1);
	hEffCorr2nd1->Draw();
	c00->cd(2);
	hEffCorr2nd2->Draw();
	c00->cd(3);
	hEffCorr2nd3->Draw();
	c00->SaveAs("wieght2.gif");

	TCanvas* c000 = new TCanvas("c000","",400,400);
	c000->cd();
	gRatio->Draw();
	c000->SaveAs("gRatio.gif");
		
		
	TFile *f1;
	char* sampleName;
	double minylab =-2.4;
	double maxylab = 2.4;
	double minpt =0.0;
	double maxpt = 30.0;

	////// read-in TTree dilepton sample
	if (isPrompt==1) {
		// old cut
		//f1 = new TFile("/home/songkyo/kyo/2013/08_EffTmp/PRMC_eff_rootFile/merged_PRMC_eff_Histos.root");
		// actual
		//f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27-v2_1Mevt.root"); //old
		f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root"); //actual
		sampleName="PRMC_boosted"; 
	}
	else {
		//old cut
		//f1 = new TFile("/home/songkyo/kyo/2013/08_EffTmp/NPMC_eff_rootFile/merged_NPMC_eff_Histos.root");
		// actual
		f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_B2Jpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root");
		sampleName="NPMC_boosted";
	}

	///// read in Ctau error range file
	std::ifstream fctPbp("./outCtErr_cross66/fit_ctauErrorRange_Pbp.txt",std::ios::in); //1stRun double53
	std::ifstream fctpPb("./outCtErr_cross66/fit_ctauErrorRange_pPb.txt",std::ios::in); //2ndRun double53
	if(!fctPbp.is_open()) { cout << "Warning : can NOT open the Pbp file!"<<endl; }
	if(!fctpPb.is_open()) { cout << "Warning : can NOT open the pPb file!"<<endl; }

	const char* strName = Form("%s_%s",strBinning,sampleName);
	std::cout << "strName: " << strName << std::endl;

	///////////////////////////////////////////////////
	//////// Definition of binning
	// --- pt Bin
//	Double_t ptBinsArr[] = {0.0, 3.0, 6.5, 7.5, 8.5, 9.5, 11.0, 14.0, 30.0}; // cross76
	Double_t ptBinsArr[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 9.5, 11.0, 14.0, 30.0}; // crossFiner
	//Double_t ptBinsArr[] = {0.0, 3.0, 6.5, 8.5, 11.5, 30.0}; // double53
	//Double_t ptBinsArr[] = {0.0, 3.0, 6.5, 10.0, 30.0}; // ntrk33
	const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
   cout << "nPtBins=" << nPtBins << endl;

	// --- y Bin
//  Double_t yBinsArr[] = {-2.4, -1.97, -1.47, -0.47, 0.53, 1.03, 1.93, 2.4}; // cross76
  Double_t yBinsArr[] = {-2.4, -1.97, -1.67, -1.37, -0.97, -0.47, 0.03, 0.43, 0.73, 1.03, 1.46, 1.93, 2.4}; //double53 || crossFiner
  //Double_t yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93}; //ntrk33
  const Int_t nYBins = sizeof(yBinsArr)/sizeof(double)-1;
  cout << "nYBins=" << nYBins << endl;

	// --- ntrk bin
	Double_t ntrBinsArr[] = {0.0, 350.0};
	//Double_t ntrBinsArr[] = {0.0, 65.0, 95.0, 350.0};
	const Int_t nNtrBins = sizeof(ntrBinsArr)/sizeof(double)-1;
	cout << "nNtrBins=" << nNtrBins << endl;
	// --- Et bin
	Double_t etBinsArr[] = {0.0, 120.0};
	const Int_t nEtBins = sizeof(etBinsArr)/sizeof(double)-1;
	cout << "nEtBins=" << nEtBins << endl;

	// setting for string bin array for checking ctau error range
	string yrangePbp[nYBins]; //1st run string
	string yrangepPb[nYBins]; //2nd run string
	string ptrange[nPtBins];
	string ntrrange[nNtrBins];
	string etrange[nEtBins];

	for (Int_t i=0; i<nYBins; i++){	
		formRapStr(yBinsArr[i], yBinsArr[i+1], &yrangePbp[i]);
		formRapStr(-1*yBinsArr[i+1], -1*yBinsArr[i], &yrangepPb[i]); // (y_lab 2nd run) -> (y_lab 1st run)
		cout << "yrangePbp["<<i<<"] = "<< yrangePbp[i].c_str() << endl;
		cout << "yrangepPb["<<i<<"] = "<< yrangepPb[i].c_str() << endl;
	}
	for (Int_t i=0; i<nPtBins; i++){	
		formStr(ptBinsArr[i], ptBinsArr[i+1], &ptrange[i]);
		cout << "ptrange["<<i<<"] = "<< ptrange[i].c_str() << endl;
	}
	for (Int_t i=0; i<nNtrBins; i++){	
		formStr(ntrBinsArr[i], ntrBinsArr[i+1], &ntrrange[i]);
		cout << "ntrrange["<<i<<"] = "<< ntrrange[i].c_str() << endl;
	}
	for (Int_t i=0; i<nEtBins; i++){	
		formStr(etBinsArr[i], etBinsArr[i+1], &etrange[i]);
		cout << "etrange["<<i<<"] = "<< etrange[i].c_str() << endl;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	/////// read-in tree
	TTree *mytree = (TTree*)f1->Get("myTree");
	
	UInt_t          eventNb;
	Int_t           Centrality;
  Int_t           HLTriggers;
	Float_t					zVtx;
	Int_t           Reco_QQ_size;
	Int_t           Reco_QQ_type[20];   //[Reco_QQ_size] : for Data GG, GT, TT?
	Int_t           Reco_QQ_sign[20];   //[Reco_QQ_size] : 0=+,- / 1=+/+, 2=-/-
	Int_t           Reco_QQ_trig[20];   //[Reco_QQ_size] : DoubleMuOpen ==1
	Float_t 				Reco_QQ_ctau[20];
	Float_t 				Reco_QQ_ctauErr[20];
	TClonesArray    *Reco_QQ_4mom;
	TClonesArray    *Reco_QQ_mupl_4mom;
	TClonesArray    *Reco_QQ_mumi_4mom;
	Int_t           Gen_QQ_size;
	Int_t           Gen_QQ_type[20];	//[Gen_QQ_size] : for MC PR=0 / NP=1
  TClonesArray    *Gen_QQ_4mom;
  TClonesArray    *Gen_QQ_mupl_4mom;
  TClonesArray    *Gen_QQ_mumi_4mom;

  TBranch        *b_eventNb;
  TBranch        *b_Centrality;   //!
  TBranch        *b_HLTriggers;
  TBranch        *b_zVtx;
  TBranch        *b_Reco_QQ_size;   //!
  TBranch        *b_Reco_QQ_type;   //!
  TBranch        *b_Reco_QQ_sign;   //!
  TBranch        *b_Reco_QQ_trig;   //!
  TBranch        *b_Reco_QQ_ctau;   //!
  TBranch        *b_Reco_QQ_ctauErr;   //!
	TBranch        *b_Reco_QQ_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_4mom;   //!
  TBranch        *b_Reco_QQ_mumi_4mom;   //!
  TBranch        *b_Gen_QQ_size;   //!
  TBranch        *b_Gen_QQ_type;
  TBranch        *b_Gen_QQ_4mom;   //!
  TBranch        *b_Gen_QQ_mupl_4mom;   //!
  TBranch        *b_Gen_QQ_mumi_4mom;   //!

  TLorentzVector* JP_Reco = new TLorentzVector;
  TLorentzVector* m1P_Reco = new TLorentzVector;
  TLorentzVector* m2P_Reco = new TLorentzVector;
  TLorentzVector* JP_Gen_tmp = new TLorentzVector; // GEN dimuon - NoCut
  TLorentzVector* JP_Gen_tmp_qq = new TLorentzVector; // GEN Jpsi - NoCut
  TLorentzVector* JP_Gen = new TLorentzVector; //for GEN dimuon - Actual Denominator
  TLorentzVector* m1P_Gen = new TLorentzVector;
  TLorentzVector* m2P_Gen = new TLorentzVector;

	Reco_QQ_4mom =0;
	Reco_QQ_mupl_4mom =0;
	Reco_QQ_mumi_4mom =0;
	Gen_QQ_4mom =0;
	Gen_QQ_mupl_4mom =0;
	Gen_QQ_mumi_4mom =0;

	// read-in branches
	mytree->SetBranchAddress("eventNb", &eventNb, &b_eventNb);
	mytree->SetBranchAddress("Centrality", &Centrality, &b_Centrality);
  mytree->SetBranchAddress("HLTriggers", &HLTriggers, &b_HLTriggers);
  mytree->SetBranchAddress("zVtx", &zVtx, &b_zVtx);
  mytree->SetBranchAddress("Reco_QQ_size", &Reco_QQ_size, &b_Reco_QQ_size);
  mytree->SetBranchAddress("Reco_QQ_type", Reco_QQ_type, &b_Reco_QQ_type);
  mytree->SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign, &b_Reco_QQ_sign);
  mytree->SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig, &b_Reco_QQ_trig);
  mytree->SetBranchAddress("Reco_QQ_ctau", Reco_QQ_ctau, &b_Reco_QQ_ctau);
  mytree->SetBranchAddress("Reco_QQ_ctauErr", Reco_QQ_ctauErr, &b_Reco_QQ_ctauErr);
  mytree->SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);
  mytree->SetBranchAddress("Reco_QQ_mupl_4mom", &Reco_QQ_mupl_4mom, &b_Reco_QQ_mupl_4mom);
  mytree->SetBranchAddress("Reco_QQ_mumi_4mom", &Reco_QQ_mumi_4mom, &b_Reco_QQ_mumi_4mom);
  mytree->SetBranchAddress("Gen_QQ_size", &Gen_QQ_size, &b_Gen_QQ_size);
  mytree->SetBranchAddress("Gen_QQ_type", Gen_QQ_type, &b_Gen_QQ_type);
  mytree->SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
  mytree->SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);
  mytree->SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);


	//////////////////////////////////////////////////////
	////// read ctauErr txt
	float ctErrminPbp[nYBins][nPtBins];
	float ctErrmaxPbp[nYBins][nPtBins];
	float ctErrminpPb[nYBins][nPtBins];
	float ctErrmaxpPb[nYBins][nPtBins];
	TH2D *h2D_ctErrminPbp = new TH2D("h2D_ctErrminPbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_ctErrmaxPbp = new TH2D("h2D_ctErrmaxPbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_ctErrminpPb = new TH2D("h2D_ctErrminpPb","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_ctErrmaxpPb = new TH2D("h2D_ctErrmaxpPb","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	string headers;
	getline(fctPbp, headers); // remove prefix
	getline(fctPbp, headers); // remove column names
	getline(fctpPb, headers); // remove prefix
	getline(fctpPb, headers); // remove column names
	string rapdum, ptdum, centdum, dphidum, ntrkdum, etdum, errmin, errmax;

	// 1st run	
	while(!fctPbp.eof()) {
		fctPbp >> rapdum >> ptdum >> centdum >> dphidum >> ntrkdum >> etdum >> errmin >> errmax;
		//cout << rapdum << ptdum << centdum << dphidum << ntrkdum << etdum << errmin << errmax << endl;
	  for (Int_t iy=0; iy<nYBins; iy++){
	    for (Int_t ipt=0; ipt<nPtBins; ipt++) {
				if (useCtErrRangeEff) {
		      if (!yrangePbp[iy].compare(rapdum) && !ptrange[ipt].compare(ptdum) ) {
						ctErrminPbp[iy][ipt] = atof(errmin.c_str());
						ctErrmaxPbp[iy][ipt] = atof(errmax.c_str());
		      }
		      // Special bin setting with no ctau error ranges from txt //for Cross76!! //byHand KYO
		      else if ( 
					( ptrange[ipt].compare("0.0-3.0") ==0 && yrangePbp[iy].compare("-2.4--1.97")!=0 ) || 
					( ptrange[ipt].compare("3.0-6.5")==0 
					&& (yrangePbp[iy].compare("-2.4--1.97")!=0 && yrangePbp[iy].compare("1.03-1.93")!=0 ) ) || 
					(yrangePbp[iy].compare("1.93-2.4")!=0) ){
						ctErrminPbp[iy][ipt]=-532;
						ctErrmaxPbp[iy][ipt]=532;
			    }
					else {
						ctErrminPbp[iy][ipt]=-532;
						ctErrmaxPbp[iy][ipt]=532;
					}
				} //end of useCtErrRangeEff
				else {
					ctErrminPbp[iy][ipt]=-532;
					ctErrmaxPbp[iy][ipt]=532;
				}
	    } 
	  }
	} //end of while
	
	// 2nd run :: NOTICE :: Read (y_lab of 2nd run) -> and Save as (y_lab of 1st run)
	while(!fctpPb.eof()) {
	  fctpPb >> rapdum >> ptdum >> centdum >> dphidum >> ntrkdum >> etdum >> errmin >> errmax;
	  //cout << rapdum << ptdum << centdum << dphidum << ntrkdum << etdum << errmin << errmax << endl;
	  for (Int_t iy=0; iy<nYBins; iy++){
	    for (Int_t ipt=0; ipt<nPtBins; ipt++) {
				if (useCtErrRangeEff) {
	    		if (!yrangepPb[iy].compare(rapdum) && !ptrange[ipt].compare(ptdum) ) {
						ctErrminpPb[iy][ipt] = atof(errmin.c_str());
						ctErrmaxpPb[iy][ipt] = atof(errmax.c_str());
	     	 	}
	     	 	// Special bin setting with no ctau error ranges from txt //byHand KYO
	     	 	else if ( 
					( ptrange[ipt].compare("0.0-3.0") ==0 && yrangepPb[iy].compare("1.97-2.4")!=0 ) || 
					( ptrange[ipt].compare("3.0-6.5")==0 
			 		&& (yrangepPb[iy].compare("1.97-2.4")!=0 && yrangepPb[iy].compare("-1.93--1.03")!=0 ) ) ||
					yrangepPb[iy].compare("-2.4--1.93")!=0 ) { 
						ctErrminpPb[iy][ipt]=-532;
						ctErrmaxpPb[iy][ipt]=532;
	     	 	}
					else {
						ctErrminpPb[iy][ipt]=-532;
						ctErrmaxpPb[iy][ipt]=532;
					}
				} //end of useCtErrRangeEff
				else {
					ctErrminpPb[iy][ipt]=-532;
					ctErrmaxpPb[iy][ipt]=532;
				}
	    }
	  }
	} //end of while

	// check ctErrmin and max was extracted corectly & put the values into hist
	int tmpminbinPbp, tmpmaxbinPbp;
	int tmpminbinpPb, tmpmaxbinpPb;
	for (Int_t iy=0; iy<nYBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++) {
			// 1st run
			tmpminbinPbp = h2D_ctErrminPbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);	
			h2D_ctErrminPbp->SetBinContent(tmpminbinPbp, ctErrminPbp[iy][ipt]);
			tmpmaxbinPbp = h2D_ctErrmaxPbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);	
			h2D_ctErrmaxPbp->SetBinContent(tmpmaxbinPbp, ctErrmaxPbp[iy][ipt]);
			// 2nd run (invert to y_lab of 1st run)	
			tmpminbinpPb = h2D_ctErrminpPb->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);	
			h2D_ctErrminpPb->SetBinContent(tmpminbinpPb, ctErrminpPb[iy][ipt]);
			tmpmaxbinpPb = h2D_ctErrmaxpPb->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);	
			h2D_ctErrmaxpPb->SetBinContent(tmpmaxbinpPb, ctErrmaxpPb[iy][ipt]);
		}
	}

	
	////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	// define 2D hist
	TH2D *h2D_NoCut_Reco_pt_y = new TH2D("h2D_NoCut_Reco_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_NoCut_Gen_pt_y = new TH2D("h2D_NoCut_Gen_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_NoCut_GenJpsi_pt_y = new TH2D("h2D_NoCut_GenJpsi_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	
	TH2D *h2D_Den_pt_y = new TH2D("h2D_Den_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y_Pbp = new TH2D("h2D_Num_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y_pPb = new TH2D("h2D_Num_pt_y_pPb","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Eff_pt_y_Pbp = new TH2D("h2D_Eff_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Eff_pt_y_pPb = new TH2D("h2D_Eff_pt_y_pPb","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	TH1D* h1D_zVtx = new TH1D("h1D_zVtx","",100,-30,30);
	
	h2D_NoCut_Reco_pt_y->Sumw2();
	h2D_NoCut_Gen_pt_y->Sumw2();
	h2D_NoCut_GenJpsi_pt_y->Sumw2();
	h2D_Den_pt_y->Sumw2();
	h2D_Num_pt_y_Pbp->Sumw2();
	h2D_Num_pt_y_pPb->Sumw2();
	h2D_Eff_pt_y_Pbp->Sumw2();
	h2D_Eff_pt_y_pPb->Sumw2();

	h1D_zVtx->Sumw2();

	/////// for pT mean value calculation
	/*
	TH1D* hNumPt[nYBins][nPtBins];
	for (Int_t iy=0; iy<nYBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++ ){
			hNumPt[iy][ipt] = new TH1D(Form("hNumPt_%d_%d",iy,ipt),"",100,ptBinsArr[ipt],ptBinsArr[ipt+1]);	
			//cout << "hNumPt["<<iy<<"]["<<ipt<<"] = " << hNumPt[iy][ipt] << endl;
		}
	}
	*/

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	float theZvtx;

	float effWeight =1;
	float zWeight =1;
	
	// event loop start
	if(nevt == -1) nevt = mytree->GetEntries();
	for(int iev=initev; iev<nevt; ++iev){
	  if(iev%100000==0) cout << ">>>>> EVENT " << iev << " / " << mytree->GetEntries() <<  endl;
	  
	  mytree->GetEntry(iev);

		//zvtx Weighting
		theZvtx = zVtx; 
		if (useZvtxWeight) { zWeight = gRatio -> Eval(theZvtx); }
		h1D_zVtx->Fill(theZvtx, zWeight);
		//cout << "" << endl;
	
	  if ( Gen_QQ_size ==0 ) continue;
	  struct Condition Jpsi_Reco;
	  struct Condition Jpsi_Gen; 
	  
	  // Reco_QQ_size loop
		for (Int_t irqq=0; irqq<Reco_QQ_size; ++irqq) {
	    //struct Condition Jpsi_Reco; 
	    JP_Reco = (TLorentzVector*) Reco_QQ_4mom->At(irqq);
	    m1P_Reco = (TLorentzVector*) Reco_QQ_mupl_4mom->At(irqq);
	    m2P_Reco = (TLorentzVector*) Reco_QQ_mumi_4mom->At(irqq);
	    
	    Jpsi_Reco.theCentrality = 97.5; // for pp!
	    Jpsi_Reco.HLTriggers = HLTriggers;
	    Jpsi_Reco.theType = Reco_QQ_type[irqq]; // GG, GT, TT
	    Jpsi_Reco.theSign = Reco_QQ_sign[irqq];
	    Jpsi_Reco.Reco_QQ_trig  = Reco_QQ_trig[irqq];
	    Jpsi_Reco.theCtErr = Reco_QQ_ctauErr[irqq];
	    
	    Jpsi_Reco.theMass = JP_Reco->M();
	    Jpsi_Reco.theRapidity = JP_Reco->Rapidity(); // y_{lab}	
	    Jpsi_Reco.thePt = JP_Reco->Pt();
	    Jpsi_Reco.thePhi = JP_Reco->Phi();
	    
	    Jpsi_Reco.mupl_p = sqrt( (m1P_Reco->Px())*(m1P_Reco->Px()) + (m1P_Reco->Py())*(m1P_Reco->Py()) + (m1P_Reco->Pz())*(m1P_Reco->Pz()) );
	    Jpsi_Reco.mumi_p = sqrt( (m2P_Reco->Px())*(m2P_Reco->Px()) + (m2P_Reco->Py())*(m2P_Reco->Py()) + (m2P_Reco->Pz())*(m2P_Reco->Pz()) );
	    Jpsi_Reco.mupl_pt = m1P_Reco->Pt();
	    Jpsi_Reco.mumi_pt = m2P_Reco->Pt();
	    Jpsi_Reco.mupl_eta = m1P_Reco->Eta();
	    Jpsi_Reco.mumi_eta = m2P_Reco->Eta();
	    
	    // --- cut01 : RECO - No cut at all
			h2D_NoCut_Reco_pt_y->Fill(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt);
	  
	  	// trigger check for - DoubleMuOpen_v1
	  	//if ( ! (Jpsi_Reco.Reco_QQ_trig&1)==1 ) continue;
	  	if ( ! ( (Jpsi_Reco.Reco_QQ_trig&1)==1 && (Jpsi_Reco.HLTriggers&1)==1 ) ) continue;

	  	// --- cut02 : RECO for numerator
	  	bool yn_reco_Pbp = false;
	  	bool yn_reco_pPb = false;
	  	int bminPbp, bmaxPbp, bminpPb, bmaxpPb; // bins
	  	double eminPbp, emaxPbp, eminpPb, emaxpPb; // values
	  
			if ( Jpsi_Reco.theSign ==0 
	    //&& massCut1(Jpsi_Reco.theMass)
	    && massCut2(Jpsi_Reco.theMass)
	    && kineCut(isBoosted, Jpsi_Reco.mupl_pt, Jpsi_Reco.mupl_eta, Jpsi_Reco.mupl_p) 
	    && kineCut(isBoosted, Jpsi_Reco.mumi_pt, Jpsi_Reco.mumi_eta, Jpsi_Reco.mumi_p) 
			&& minpt<=Jpsi_Reco.thePt && Jpsi_Reco.thePt < maxpt
	    && minylab<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity < maxylab) {
	    	// ctau error range cut
	    	bminPbp = h2D_ctErrminPbp->FindBin(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt);
	    	eminPbp = h2D_ctErrminPbp->GetBinContent(bminPbp);
	    	bmaxPbp = h2D_ctErrmaxPbp->FindBin(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt);
	    	emaxPbp = h2D_ctErrmaxPbp->GetBinContent(bmaxPbp);
	    	bminpPb = h2D_ctErrminpPb->FindBin(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt);
	    	eminpPb = h2D_ctErrminpPb->GetBinContent(bminpPb);
	    	bmaxpPb = h2D_ctErrmaxpPb->FindBin(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt);
	    	emaxpPb = h2D_ctErrmaxpPb->GetBinContent(bmaxpPb);
	    	/*
	      cout << iev << "th "<< irqq<< endl;
	      cout << "** Jpsi_Reco.theRapidity = " << Jpsi_Reco.theRapidity << endl;
	      cout << "** Jpsi_Reco.thePt = " << Jpsi_Reco.thePt << endl;
	      cout << "** Jpsi_Reco.theCtErr = " << Jpsi_Reco.theCtErr << endl;
	      cout << "eminPbp = " << eminPbp << endl;
	      cout << "emaxPbp = " << emaxPbp << endl;
	      cout << "** CtErrRange from " << eminPbp <<" to "<< emaxPbp << endl;
	      cout << "eminpPb = " << eminpPb << endl;
	      cout << "emaxpPb = " << emaxpPb << endl;
	      cout << "** CtErrRange from " << eminpPb <<" to "<< emaxpPb << endl;
	    	*/
	    	if (eminPbp < Jpsi_Reco.theCtErr && Jpsi_Reco.theCtErr < emaxPbp ){	
	      	yn_reco_Pbp = true;
	   		}
	    	if (eminpPb < Jpsi_Reco.theCtErr && Jpsi_Reco.theCtErr < emaxpPb ){	
	      	yn_reco_pPb = true;
	   		}
			}
		
			// TNP Eff weighting && vZtx weighting	
	  	//float effWeight = 1;
	 		//float zWeight = 1; 
	  	effWeight = 1;
			
			//1st run
	  	if (yn_reco_Pbp == true){ 
	    	if (useDataDrivenEff)
	      effWeight = getEffWeight( isBoosted, Jpsi_Reco.mupl_pt, Jpsi_Reco.mupl_eta, Jpsi_Reco.mumi_pt, Jpsi_Reco.mumi_eta, 1);
				h2D_Num_pt_y_Pbp->Fill(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt, effWeight*zWeight);
			}
			//2nd run
			if (yn_reco_pPb == true){ 
	    	if (useDataDrivenEff)
	      effWeight = getEffWeight( isBoosted, Jpsi_Reco.mupl_pt, Jpsi_Reco.mupl_eta, Jpsi_Reco.mumi_pt, Jpsi_Reco.mumi_eta, 2);
	    	h2D_Num_pt_y_pPb->Fill(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt, effWeight*zWeight);
			}
		} //end of Reco_QQ_size loop
	  
	  ////// Gen_QQ_size loop
	  for (Int_t igqq=0; igqq<Gen_QQ_size; ++igqq) {
			//struct Condition Jpsi_Gen; 
	    m1P_Gen = (TLorentzVector*) Gen_QQ_mupl_4mom->At(igqq);
	    m2P_Gen = (TLorentzVector*) Gen_QQ_mumi_4mom->At(igqq);
	    JP_Gen_tmp_qq = (TLorentzVector*) Gen_QQ_4mom->At(igqq); // Gen Jpsi (for filling NoCut)
	    *JP_Gen_tmp = *m1P_Gen +  *m2P_Gen; // Gen dimuon pairs (for filling NoCut)
	    
	    // variables only used for Reco
	    Jpsi_Gen.HLTriggers = 0;
			Jpsi_Gen.Reco_QQ_trig  = 0;
	    Jpsi_Gen.theSign = 0; //already +- pair
	    
	    Jpsi_Gen.theCentrality = 97.5; // for pp!
	    Jpsi_Gen.theType = Gen_QQ_type[igqq]; // PR or NP
	    Jpsi_Gen.mupl_p = sqrt( (m1P_Gen->Px())*(m1P_Gen->Px()) + (m1P_Gen->Py())*(m1P_Gen->Py()) + (m1P_Gen->Pz())*(m1P_Gen->Pz()) );
	    Jpsi_Gen.mumi_p = sqrt( (m2P_Gen->Px())*(m2P_Gen->Px()) + (m2P_Gen->Py())*(m2P_Gen->Py()) + (m2P_Gen->Pz())*(m2P_Gen->Pz()) );
	    Jpsi_Gen.mupl_pt = m1P_Gen->Pt();
	    Jpsi_Gen.mumi_pt = m2P_Gen->Pt();
	    Jpsi_Gen.mupl_eta = m1P_Gen->Eta();
	    Jpsi_Gen.mumi_eta = m2P_Gen->Eta();
	    
			// --- cut01 : GEN - No cut (only DimuCut = +-pair from 443)
	    h2D_NoCut_Gen_pt_y->Fill(JP_Gen_tmp->Rapidity(),JP_Gen_tmp->Pt()); // Gen dimuon
	    h2D_NoCut_GenJpsi_pt_y->Fill(JP_Gen_tmp_qq->Rapidity(),JP_Gen_tmp_qq->Pt()); // Gen Jpsi
	    
	    // --- cut02 : GEN for denominator
	    bool yn_gen = false;
	    if ( kineCut(isBoosted, Jpsi_Gen.mupl_pt, Jpsi_Gen.mupl_eta, Jpsi_Gen.mupl_p) 
			&& kineCut(isBoosted, Jpsi_Gen.mumi_pt, Jpsi_Gen.mumi_eta, Jpsi_Gen.mumi_p)) {
	      *JP_Gen = *m1P_Gen +  *m2P_Gen; // used for actual denominator ( GEN dimuon pairs)
				Jpsi_Gen.theMass = JP_Gen->M();
				Jpsi_Gen.theRapidity = JP_Gen->Rapidity();	
				Jpsi_Gen.thePt = JP_Gen->Pt();
				Jpsi_Gen.thePhi = JP_Gen->Phi();
				if ( massCut1(Jpsi_Gen.theMass)  
				     && minpt<=Jpsi_Gen.thePt && Jpsi_Gen.thePt <maxpt 
				     && minylab<=Jpsi_Gen.theRapidity && Jpsi_Gen.theRapidity <maxylab) {
				  yn_gen = true;
				}
	    }
	    if (yn_gen == true){
				h2D_Den_pt_y->Fill(Jpsi_Gen.theRapidity,Jpsi_Gen.thePt,zWeight);
	    } // end of yn_gen
	  } //end of Gen_QQ_size loop
	} //end of event loop
	
	// (Num/Den) to get efficiency (B : binomial error)
	h2D_Eff_pt_y_Pbp->Divide(h2D_Num_pt_y_Pbp,h2D_Den_pt_y,1,1,"B");
	h2D_Eff_pt_y_pPb->Divide(h2D_Num_pt_y_pPb,h2D_Den_pt_y,1,1,"B");
	
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Save the data!
	
	// --- cout check
	double EffValPbp[nYBins][nPtBins];
	double EffValPbpErr[nYBins][nPtBins];
	double EffValpPb[nYBins][nPtBins];
	double EffValpPbErr[nYBins][nPtBins];
	for (Int_t iy=0; iy<nYBins; iy++){
	  for (Int_t ipt=0; ipt<nPtBins; ipt++ ){
	    EffValPbp[iy][ipt] = h2D_Eff_pt_y_Pbp-> GetBinContent(iy+1,ipt+1);
	    EffValPbpErr[iy][ipt] = h2D_Eff_pt_y_Pbp-> GetBinError(iy+1,ipt+1);
	    EffValpPb[iy][ipt] = h2D_Eff_pt_y_pPb-> GetBinContent(iy+1,ipt+1);
	    EffValpPbErr[iy][ipt] = h2D_Eff_pt_y_pPb-> GetBinError(iy+1,ipt+1);
	    cout << iy<<"th rap : "<< yBinsArr[iy]<<" - "<<yBinsArr[iy+1]<<endl;
	    cout << ipt<<"th pt : "<< ptBinsArr[ipt]<<" - "<<ptBinsArr[ipt+1]<<endl;
	    cout << "yrangePbp["<<iy<<"] = "<< yrangePbp[iy].c_str() << endl;
	    cout << "yrangepPb["<<iy<<"] = "<< yrangepPb[iy].c_str() << endl;
	    cout << "EffValPbp = " << EffValPbp[iy][ipt] << endl;
	    cout << "EffValPbpErr = " << EffValPbpErr[iy][ipt] << endl;
	    cout << "EffValpPb = " << EffValpPb[iy][ipt] << endl;
	    cout << "EffValpPbErr = " << EffValpPbErr[iy][ipt] << endl;
	  }
	}
	
	// --- save as a root file

	TFile *outFile = new TFile(Form("EffAna_%s_useCtErrRange%d_useDataDriven%d_useZvtxWeight%d_mcOneWay.root",strName, (int)useCtErrRangeEff ,(int)useDataDrivenEff, (int)useZvtxWeight),"RECREATE");
	std::cout << "strName: " << strName << std::endl;
	outFile->cd();

	h2D_ctErrminPbp->Write();
	h2D_ctErrmaxPbp->Write();
	h2D_ctErrminpPb->Write();
	h2D_ctErrmaxpPb->Write();
	
//	h2D_NoCut_Reco_pt_y->Write();
//	h2D_NoCut_Gen_pt_y->Write();
//	h2D_NoCut_GenJpsi_pt_y->Write();
	h2D_Den_pt_y->Write();
	h2D_Num_pt_y_Pbp->Write();
	h2D_Num_pt_y_pPb->Write();
	h2D_Eff_pt_y_Pbp->Write();
	h2D_Eff_pt_y_pPb->Write();
				
	h1D_zVtx->Write();
	
	/*	
	for (Int_t iy=0; iy<nYBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++ ){
			hNumPt[iy][ipt] ->Write();	
		}
	}
	*/
	outFile->Close();

	return 0;

} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 


float getEffWeight(bool isBoosted,  float mupt1, float mueta1, float mupt2, float mueta2, int runId) { 
  float mup1 = mupt1* cosh(mueta1);
  float mup2 = mupt2* cosh(mueta2);

  if ( kineCut( isBoosted, mupt1, mueta1, mup1)==false)   
    return 0;
  if ( kineCut( isBoosted, mupt2, mueta2, mup2)==false)   
    return 0;

  TH1D* hw1;
  TH1D* hw2;
  if ( runId == 1)  {
    if ( mueta1 < -0.8 )   hw1 = hEffCorr1st1;
    else if ( mueta1 < 0.8 )   hw1 = hEffCorr1st2;
    else    hw1 = hEffCorr1st3;

    if ( mueta2 < -0.8 )   hw2 = hEffCorr1st1;
    else if ( mueta2 < 0.8 )   hw2 = hEffCorr1st2;
    else    hw2 = hEffCorr1st3;
  }
  else if ( runId == 2 ) {
    if ( mueta1 < -0.8 )   hw1 = hEffCorr2nd1;
    else if ( mueta1 < 0.8 )   hw1 = hEffCorr2nd2;
    else    hw1 = hEffCorr2nd3;

    if ( mueta2 < -0.8 )   hw2 = hEffCorr2nd1;
    else if ( mueta2 < 0.8 )   hw2 = hEffCorr2nd2;
    else    hw2 = hEffCorr2nd3;
  }
  else  
    cout << " ERROR!  getEffWeight::runId is wrong!!!!!" << endl; 
  
  int bin1 = hw1->FindBin(mupt1);
  int bin2 = hw2->FindBin(mupt2);
  float effWeight1 = hw1->GetBinContent(bin1);
  float effWeight2 = hw2->GetBinContent(bin2);
  return effWeight1 * effWeight2;
}

bool kineCut(bool isBoosted, double muPt, double muEta, double muP) {
	return ( TMath::Abs(muEta) < 2.4 &&
				((TMath::Abs(muEta) < 1.3 && muPt >=3.3) ||
				(1.3 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.2 && muP >=2.9) ||
				(2.2 <= TMath::Abs(muEta) && muPt >= 0.8)));
}

bool massCut1(double lv_dimu_mass) {
	return ( 2.6 <= lv_dimu_mass && lv_dimu_mass < 3.5 );
}
bool massCut2(double lv_dimu_mass) {
	//return ( 2.95 <= lv_dimu_mass && lv_dimu_mass < 3.25 );
	return ( 2.9 <= lv_dimu_mass && lv_dimu_mass < 3.3 );
}

void formRapStr(Double_t min, Double_t max, string* arr) { //byHand KYO
	if (min==-2.4 || min==2.4) *arr = Form("%.1f-%.2f",min,max);
	else if (max==-2.4 || max==2.4) *arr = Form("%.2f-%.1f",min,max);
	else *arr = Form("%.2f-%.2f",min,max);
}
void formStr(Double_t min, Double_t max, string* arr) {
	*arr = Form("%.1f-%.1f",min,max);
}


