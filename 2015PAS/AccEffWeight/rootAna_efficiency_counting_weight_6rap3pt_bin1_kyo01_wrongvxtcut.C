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

//const double shiftvar = -0.47; // conversion constant y=0(collision)==y=-0.47(LAB frame)  
bool kineCut(double muPt, double muEta, double muP);
bool massCut1(double lv_dimu_mass);
bool massCut2(double lv_dimu_mass);
void formRapStr(Double_t min, Double_t max, string* arr);
void formStr(Double_t min, Double_t max, string* arr);
float getEffWeight(float mupt1=0, float mueta1=0, float mupt2=0, float mueta2=0); // runId parameter removed
float getRecoWeight(float dimu_pt, float dimu_y, bool isPbp); 

struct Condition {
  double theCentrality; 
  int theType, theSign, Reco_QQ_trig, HLTriggers;
  double theMass, theRapidity, thePt, thePhi;
  double mupl_p, mumi_p, mupl_pt, mumi_pt, mupl_eta, mumi_eta;
  float theCtErr;
//	float theZvtx;
} ;

//read TNP plots for useDataDrivenEff
TFile* fTnpRate = new TFile("./tagAndProbe/tnpRate_nominal_fitRatio_5etaBin.root");
TF1* hTnpRateEtaBin1 = (TF1*)fTnpRate->Get("ferrScale_ieta1");
TF1* hTnpRateEtaBin2 = (TF1*)fTnpRate->Get("ferrScale_ieta2");
TF1* hTnpRateEtaBin3 = (TF1*)fTnpRate->Get("ferrScale_ieta3");
TF1* hTnpRateEtaBin4 = (TF1*)fTnpRate->Get("ferrScale_ieta4");
TF1* hTnpRateEtaBin5 = (TF1*)fTnpRate->Get("ferrScale_ieta5");

///////// read DataRecoRatio weight func. from Yeonju // KYO
// tmp!!!! prompt or non-prompt by hand
TFile* fRatio = new TFile("fitRatio_isPrompt1_Pbp_kyo01.root");
//TFile* fRatio = new TFile("fitRatio_isPrompt0_Pbp_kyo01.root");
TF1* func_0=(TF1*)fRatio->Get("func_0");
TF1* func_1=(TF1*)fRatio->Get("func_1");
TF1* func_2=(TF1*)fRatio->Get("func_2");
TF1* func_3=(TF1*)fRatio->Get("func_3");
TF1* func_4=(TF1*)fRatio->Get("func_4");
TF1* func_5=(TF1*)fRatio->Get("func_5");
TF1* func_6=(TF1*)fRatio->Get("func_6");
TF1* func_7=(TF1*)fRatio->Get("func_7");

/////// main func. ///////

int rootAna_efficiency_counting_weight_6rap3pt_bin1_kyo01_wrongvtxcut(char *strBinning = "6rap3pt", bool isPrompt = true, bool is1st = true, bool isEmbedded = false, bool useCtErrRangeEff =true, bool useDataDrivenEff=true, bool useZvtxWeightStep1 = false, bool useZvtxWeightStep2=true){

  using namespace std;
  
	// # of event range
  int initev =0;
  int nevt = -1; //all
  //int nevt = 50000;

	//////// read zTtx functions for weight

	//// Step1 : embedded -> pythia
	TFile *fZvtxStep1; 
	if (is1st){
		if (isPrompt) fZvtxStep1 = new TFile("zVtxFit_pythia_embedded/zVtxFit_20141218step1_pr_1st.root");
		else fZvtxStep1 = new TFile("zVtxFit_pythia_embedded/zVtxFit_20141218step1_np_1st.root");
	}
	else {
		if (isPrompt) fZvtxStep1 = new TFile("zVtxFit_pythia_embedded/zVtxFit_20141218step1_pr_2nd.root");
		else fZvtxStep1 = new TFile("zVtxFit_pythia_embedded/zVtxFit_20141218step1_np_2nd.root");
	}
	TH1D* hRatio = (TH1D*)fZvtxStep1->Get("hRatio_0");

	//// Step2 : pythia -> data
	TFile* fZvtx = new TFile("zVtxFit/zVtxFit_20141007.root");
	TF1* gRatio = (TF1*)fZvtx->Get("gRatio");

	TFile *f1;
	char* sampleName;
	double minylab =-2.4;
	double maxylab = 2.4;
	double minpt =0.0;
	double maxpt = 30.0;

	////// read-in HiOniaTrees
	if (isEmbedded) {
		if (is1st){
			if (isPrompt) {
				f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySampleEmbedded/tot_PromptJpsi_HIJINGembedding_1st_STARTHI53_V27_20141218.root");
				sampleName="PRMCembedded_Pbp"; 
			}
			else {
				f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySampleEmbedded/tot_B2Jpsi_HIJINGembedding_1st_STARTHI53_V27_20141218.root");
				sampleName="NPMCembedded_Pbp";
			}
		}
		else {
			if (isPrompt) {
				f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySampleEmbedded/tot_PromptJpsi_HIJINGembedding_2nd_STARTHI53_V27_20141218_v2.root");
				sampleName="PRMCembedded_pPb"; 
			}
			else {
				f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySampleEmbedded/tot_B2Jpsi_HIJINGembedding_2nd_STARTHI53_V27_20141218.root");
				sampleName="NPMCembedded_pPb";
			}
		}
	}
	else {
		if (is1st){
			if (isPrompt) {
				f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root"); //actual
				sampleName="PRMCpythia_Pbp"; 
			}
			else {
				f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_B2Jpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root");
				sampleName="NPMCpythia_Pbp";
			}
		}
		else {
			if (isPrompt) {
				f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_PromptJpsi_PYTHIAboosted_2nd_STARTHI53_V27_1Mevt.root"); //actual
				sampleName="PRMCpythia_pPb"; 
			}
			else {
				f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_B2Jpsi_PYTHIAboosted_2nd_STARTHI53_V27_1Mevt.root");
				sampleName="NPMCpythia_pPb";
			}
		}
	}

	///// read in Ctau error range file
	char* runName;
	if(is1st) runName = "Pbp";
	else runName = "pPb";	

    char * dirName = Form("fitRes_%s",strBinning);
    std::ifstream fctau(Form("./%s/summary_%s/fit_ctauErrorRange",dirName,runName),std::ios::in);	
    
    //char * dirName = "fitRes_8rap9pt_20150106";
	//std::ifstream fctau(Form("./%s/summary_%s/fit_ctauErrorRange",dirName,runName),std::ios::in);
	if(!fctau.is_open()) { cout << "Warning : can NOT open the fit_ctauErrorRange file!"<<endl; }

	const char* strName = Form("%s_%s",strBinning,sampleName);
	std::cout << "strName: " << strName << std::endl;

	///////////////////////////////////////////////////
	//////// Definition of binning
	// --- pt Bin
	//Double_t ptBinsArr[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt2gev
	Double_t ptBinsArr[] = {5.0, 6.5, 10.0, 30.0}; // 6rap3pt
	const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
   cout << "nPtBins=" << nPtBins << endl;

	// --- y Bin //set to 1st run (For 2nd run, will be automatically changed later)
  //Double_t tmp_yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
	Double_t tmp_yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46}; // 6rap3pt
  //const Int_t tmp_nYBins = sizeof(tmp_yBinsArr)/sizeof(double)-1;
  const Int_t tmp_nYBins = sizeof(tmp_yBinsArr)/sizeof(double);
  cout << "tmp_nYBins=" << tmp_nYBins << endl;
	
	Double_t yBinsArr[tmp_nYBins] = {};
	if (is1st){
		for (Int_t i=0; i<tmp_nYBins; i++) {
			yBinsArr[i] = tmp_yBinsArr[i]; 
			cout <<"yBinsArr["<<i<<"] = " <<yBinsArr[i]<<endl;
		}
	}
	else {
		// change 1st run to 2nd run
		for (Int_t i=0; i<tmp_nYBins; i++) {
			yBinsArr[i] = -1*tmp_yBinsArr[tmp_nYBins-1-i]; 
			cout <<"yBinsArr["<<i<<"] = " <<yBinsArr[i]<<endl;
		}
	}
  const Int_t nYBins = sizeof(yBinsArr)/sizeof(double)-1;
  cout << "nYBins=" << nYBins << endl;

	// --- ntrk bin
	Double_t ntrBinsArr[] = {0.0, 350.0};
	const Int_t nNtrBins = sizeof(ntrBinsArr)/sizeof(double)-1;
	cout << "nNtrBins=" << nNtrBins << endl;
	// --- Et bin
	//Double_t etBinsArr[] = {0.0, 120.0};
	Double_t etBinsArr[] = {0.0, 20.0}; //6rap3pt bin1
	//Double_t etBinsArr[] = {20.0, 30.0}; //6rap3pt bin2
	//Double_t etBinsArr[] = {30.0, 120.0}; //6rap3pt bin3
	const Int_t nEtBins = sizeof(etBinsArr)/sizeof(double)-1;
	cout << "nEtBins=" << nEtBins << endl;

	// setting for string bin array for checking ctau error range
	string yrange[nYBins]; //1st run string
	string ptrange[nPtBins];
	string ntrrange[nNtrBins];
	string etrange[nEtBins];

	for (Int_t i=0; i<nYBins; i++){	
		formRapStr(yBinsArr[i], yBinsArr[i+1], &yrange[i]);
		cout << "yrange["<<i<<"] = "<< yrange[i].c_str() << endl;
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
	float ctErrmin[nYBins][nPtBins];
	float ctErrmax[nYBins][nPtBins];
	TH2D *h2D_ctErrmin = new TH2D("h2D_ctErrmin","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_ctErrmax = new TH2D("h2D_ctErrmax","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	string headers;
	getline(fctau, headers); // remove prefix
	getline(fctau, headers); // remove column names
	string rapdum, ptdum, centdum, dphidum, ntrkdum, etdum, errmin, errmax;

	// 1st run	
	while(!fctau.eof()) {
		fctau >> rapdum >> ptdum >> centdum >> dphidum >> ntrkdum >> etdum >> errmin >> errmax;
		//cout << rapdum << ptdum << centdum << dphidum << ntrkdum << etdum << errmin << errmax << endl;
	  for (Int_t iy=0; iy<nYBins; iy++){
	    for (Int_t ipt=0; ipt<nPtBins; ipt++) {
				if (useCtErrRangeEff) {
		      if (!yrange[iy].compare(rapdum) && !ptrange[ipt].compare(ptdum) ) {
						ctErrmin[iy][ipt] = atof(errmin.c_str());
						ctErrmax[iy][ipt] = atof(errmax.c_str());
		      }
				} //end of useCtErrRangeEff
				else {
					ctErrmin[iy][ipt]=-532;
					ctErrmax[iy][ipt]=532;
				}
	    } 
	  }
	} //end of while

	// check ctErrmin and max was extracted corectly & put the values into hist
	int tmpminbin, tmpmaxbin;
	for (Int_t iy=0; iy<nYBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++) {
			tmpminbin = h2D_ctErrmin->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);	
			h2D_ctErrmin->SetBinContent(tmpminbin, ctErrmin[iy][ipt]);
			tmpmaxbin = h2D_ctErrmax->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);	
			h2D_ctErrmax->SetBinContent(tmpmaxbin, ctErrmax[iy][ipt]);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	// define 2D hist
	TH2D *h2D_NoCut_Reco_pt_y = new TH2D("h2D_NoCut_Reco_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_NoCut_Gen_pt_y = new TH2D("h2D_NoCut_Gen_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_NoCut_GenJpsi_pt_y = new TH2D("h2D_NoCut_GenJpsi_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	
	TH2D *h2D_Den_pt_y = new TH2D("h2D_Den_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y = new TH2D("h2D_Num_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Eff_pt_y = new TH2D("h2D_Eff_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	TH1D* h1D_zVtx = new TH1D("h1D_zVtx","",80,-30,30);
	
	h2D_NoCut_Reco_pt_y->Sumw2();
	h2D_NoCut_Gen_pt_y->Sumw2();
	h2D_NoCut_GenJpsi_pt_y->Sumw2();
	h2D_Den_pt_y->Sumw2();
	h2D_Num_pt_y->Sumw2();
	h2D_Eff_pt_y->Sumw2();
	h1D_zVtx->Sumw2();

///////////////////////////////////
    //yeonju
    TH2D *h2D_Den_noWeight_pt_y;
    TH2D *h2D_Num_noWeight_pt_y;
    TH2D *h2D_Eff_noWeight_pt_y;
    TH2D *h2D_Eff_diff_pt_y;
    TH2D *h2D_Eff_err_pt_y;

    if(is1st){
        h2D_Den_noWeight_pt_y = new TH2D("h2D_Den_noWeight_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Num_noWeight_pt_y = new TH2D("h2D_Num_noWeight_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Eff_noWeight_pt_y = new TH2D("h2D_Eff_noWeight_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Eff_diff_pt_y= new TH2D("h2D_Eff_diff_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Eff_err_pt_y = new TH2D("h2D_Eff_err_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    } else if(is1st==0) {
        h2D_Den_noWeight_pt_y = new TH2D("h2D_Den_noWeight_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Num_noWeight_pt_y = new TH2D("h2D_Num_noWeight_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Eff_noWeight_pt_y = new TH2D("h2D_Eff_noWeight_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Eff_diff_pt_y = new TH2D("h2D_Eff_diff_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Eff_err_pt_y = new TH2D("h2D_Eff_err_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    }
    h2D_Den_noWeight_pt_y->Sumw2();
    h2D_Num_noWeight_pt_y->Sumw2();
    h2D_Eff_noWeight_pt_y->Sumw2();
    h2D_Eff_diff_pt_y->Sumw2();
    h2D_Eff_err_pt_y->Sumw2();

    TString prom_st, run_st;
    if(isPrompt) prom_st = "PR"; else prom_st = "NP";
    if(is1st) run_st = "Pbp"; else run_st = "pPb";

	////////////////////////////////////////////////////////////////////////////////////////////////

	float theZvtx;

	float effWeight =1;
	float recoWeight =1;
	float zWeight01 =1;
	float zWeight02 =1;
	int tmpbin=531;
	
	// event loop start
	if(nevt == -1) nevt = mytree->GetEntries();
	for(int iev=initev; iev<nevt; ++iev){
	  if(iev%100000==0) cout << ">>>>> EVENT " << iev << " / " << mytree->GetEntries() <<  endl;
	  
	  mytree->GetEntry(iev);
        






		////zvtx Weighting
		theZvtx = zVtx; 
		if (useZvtxWeightStep1) { 
			tmpbin = hRatio -> FindBin(theZvtx);
			zWeight01 = hRatio->GetBinContent(tmpbin); 
		}
		if (useZvtxWeightStep2) { 
			zWeight02 = gRatio -> Eval(theZvtx); 
		}
		// zVtxCut
		if (useZvtxWeightStep2 && TMath::Abs(theZvtx) > 10.) continue;
		h1D_zVtx->Fill(theZvtx, zWeight01*zWeight02);
	
	  if ( Gen_QQ_size ==0 ) continue;
	  
		struct Condition Jpsi_Reco;
	  struct Condition Jpsi_Gen; 
	  
	  // Reco_QQ_size loop
		for (Int_t irqq=0; irqq<Reco_QQ_size; ++irqq) {
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

	  	// --- cut02 : RECO for numerator
	  	bool yn_reco = false;
	  	int bmin, bmax, bminpPb, bmaxpPb; // bins
	  	double emin, emax, eminpPb, emaxpPb; // values
	  
			if ( Jpsi_Reco.theSign ==0 
	    && massCut2(Jpsi_Reco.theMass)
	   	&& ( (Jpsi_Reco.Reco_QQ_trig&1)==1 && (Jpsi_Reco.HLTriggers&1)==1 ) 
			&& kineCut(Jpsi_Reco.mupl_pt, Jpsi_Reco.mupl_eta, Jpsi_Reco.mupl_p) 
	    && kineCut(Jpsi_Reco.mumi_pt, Jpsi_Reco.mumi_eta, Jpsi_Reco.mumi_p) 
			&& minpt<=Jpsi_Reco.thePt && Jpsi_Reco.thePt < maxpt
	    && minylab<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity < maxylab) {
	    	// ctau error range cut
	    	bmin = h2D_ctErrmin->FindBin(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt);
	    	emin = h2D_ctErrmin->GetBinContent(bmin);
	    	bmax = h2D_ctErrmax->FindBin(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt);
	    	emax = h2D_ctErrmax->GetBinContent(bmax);
	    	if (emin < Jpsi_Reco.theCtErr && Jpsi_Reco.theCtErr < emax ){	
	      	yn_reco = true;
	   		}
			}
		
			//// TNP Eff weighting && vZtx weighting && reco ratio weighting	
	   	effWeight = 1;
	  	recoWeight = 1;
	  	if (yn_reco == true){ 
	    	if (useDataDrivenEff){
					effWeight = getEffWeight(Jpsi_Reco.mupl_pt, Jpsi_Reco.mupl_eta, Jpsi_Reco.mumi_pt, Jpsi_Reco.mumi_eta); 
				}
				recoWeight = getRecoWeight(Jpsi_Reco.thePt, Jpsi_Reco.theRapidity, is1st);
				h2D_Num_pt_y->Fill(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt, effWeight*zWeight01*zWeight02*recoWeight);
                h2D_Num_noWeight_pt_y->Fill(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt, effWeight*zWeight01*zWeight02);		
        	}
		} //end of Reco_QQ_size loop
	  
	  ////// Gen_QQ_size loop
	  for (Int_t igqq=0; igqq<Gen_QQ_size; ++igqq) {
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
	    if ( kineCut(Jpsi_Gen.mupl_pt, Jpsi_Gen.mupl_eta, Jpsi_Gen.mupl_p) 
			&& kineCut(Jpsi_Gen.mumi_pt, Jpsi_Gen.mumi_eta, Jpsi_Gen.mumi_p)) {
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
	  	recoWeight = 1.;
	    if (yn_gen == true){
				recoWeight = getRecoWeight(Jpsi_Gen.thePt, Jpsi_Gen.theRapidity, is1st);
				h2D_Den_pt_y->Fill(Jpsi_Gen.theRapidity,Jpsi_Gen.thePt,zWeight01*zWeight02*recoWeight);
                h2D_Den_noWeight_pt_y->Fill(Jpsi_Gen.theRapidity,Jpsi_Gen.thePt,zWeight01*zWeight02);
        } // end of yn_gen
	  } //end of Gen_QQ_size loop
	} //end of event loop
	
	// (Num/Den) to get efficiency (B : binomial error)
	h2D_Eff_pt_y->Divide(h2D_Num_pt_y,h2D_Den_pt_y,1,1,"B");
    h2D_Eff_noWeight_pt_y->Divide(h2D_Num_noWeight_pt_y,h2D_Den_noWeight_pt_y,1,1,"B");

    for(int ipt=0;ipt<nPtBins;ipt++){
        for(int iy=0;iy<nYBins;iy++){
            h2D_Eff_diff_pt_y = (TH2D*) h2D_Eff_noWeight_pt_y->Clone("h2D_Eff_diff_pt_y");
            h2D_Eff_diff_pt_y->Add(h2D_Eff_pt_y,-1);
            h2D_Eff_err_pt_y->Divide(h2D_Eff_diff_pt_y,h2D_Eff_pt_y);
        }
    }

	////////////////////////////////////////////////////////////////////////////
	// Save the data!
	
	// --- cout check
	double EffVal[nYBins][nPtBins];
	double EffValErr[nYBins][nPtBins];
	for (Int_t iy=0; iy<nYBins; iy++){
	  for (Int_t ipt=0; ipt<nPtBins; ipt++ ){
	    EffVal[iy][ipt] = h2D_Eff_pt_y-> GetBinContent(iy+1,ipt+1);
	    EffValErr[iy][ipt] = h2D_Eff_pt_y-> GetBinError(iy+1,ipt+1);
	    cout << "" << endl;
			cout << iy<<"th rap : "<< yBinsArr[iy]<<" - "<<yBinsArr[iy+1]<<endl;
	    cout << ipt<<"th pt : "<< ptBinsArr[ipt]<<" - "<<ptBinsArr[ipt+1]<<endl;
	    cout << "EffVal = " << EffVal[iy][ipt] << endl;
	    cout << "EffValErr = " << EffValErr[iy][ipt] << endl;
	  }
	}
	
	// --- save as a root file
	TFile *outFile = new TFile(Form("EffCounting_%s_useCtErr_%d_useDataDriven_%d_useZvtxStep1_%d_Step2_%d_bin1_kyo01.root",strName, (int)useCtErrRangeEff ,(int)useDataDrivenEff, (int)useZvtxWeightStep1, (int)useZvtxWeightStep2),"RECREATE");
	std::cout << "strName: " << strName << std::endl;
	outFile->cd();

	h2D_ctErrmin->Write();
	h2D_ctErrmax->Write();
	h2D_Den_pt_y->Write();
    h2D_Num_pt_y->Write();
    h2D_Eff_pt_y->Write();

    h2D_Den_noWeight_pt_y->Write();
    h2D_Num_noWeight_pt_y->Write();
    h2D_Eff_noWeight_pt_y->Write();
    h2D_Eff_diff_pt_y->Write();
    h2D_Eff_err_pt_y->Write();


	h1D_zVtx->Write();
	outFile->Close();

	return 0;

} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 


float getEffWeight(float mupt1, float mueta1, float mupt2, float mueta2) { 
  float mup1 = mupt1* cosh(mueta1);
  float mup2 = mupt2* cosh(mueta2);

  if ( kineCut( mupt1, mueta1, mup1)==false)   
    return 0;
  if ( kineCut( mupt2, mueta2, mup2)==false)   
    return 0;

  TF1* hw1;
  TF1* hw2;
  if (  TMath::Abs(mueta1) < 0.6 )      hw1 = hTnpRateEtaBin1;
  else if ( TMath::Abs(mueta1) < 1.3 )  hw1 = hTnpRateEtaBin2;
  else if ( TMath::Abs(mueta1) < 1.8 )  hw1 = hTnpRateEtaBin3;
  else if ( TMath::Abs(mueta1) < 2.2 )  hw1 = hTnpRateEtaBin4;
  else                                  hw1 = hTnpRateEtaBin5;

  if (  TMath::Abs(mueta2) < 0.6 )      hw2 = hTnpRateEtaBin1;
  else if ( TMath::Abs(mueta2) < 1.3 )  hw2 = hTnpRateEtaBin2;
  else if ( TMath::Abs(mueta2) < 1.8 )  hw2 = hTnpRateEtaBin3;
  else if ( TMath::Abs(mueta2) < 2.2 )  hw2 = hTnpRateEtaBin4;
  else                                  hw2 = hTnpRateEtaBin5;

	float effWeight1 = hw1->Eval(mupt1);
	float effWeight2 = hw2->Eval(mupt2);

	return effWeight1 * effWeight2;
}

float getRecoWeight(float dimu_pt, float dimu_y, bool isPbp) { 
				//weighting factor for Acc. Sys. by Yeonju
//        double w_toy = 1.0;
//        int yflag = 99;
        double ptThr = 0.0;
        string st_AccRange = "";
			  
				TF1* t_func;
				if(isPbp){
            if(-2.40<= dimu_y && dimu_y<-1.97) {  t_func=func_0; ptThr=0.0; }
            else if(-1.97<= dimu_y && dimu_y<-1.37) {  t_func=func_1;  ptThr=3.0; }
            else if(-1.37<= dimu_y && dimu_y<-0.47) {  t_func=func_2;  ptThr=6.5; }
            else if(-0.47<= dimu_y && dimu_y<0.43) {  t_func=func_3;  ptThr=6.5; }
            else if(0.43<= dimu_y && dimu_y<1.03) {  t_func=func_4;  ptThr=6.5; }
            else if(1.03<= dimu_y && dimu_y<1.46) {  t_func=func_5;  ptThr=5.0; }
            else if(1.46<= dimu_y && dimu_y<1.93) {  t_func=func_6;  ptThr=3.0; }
            else if(1.93<= dimu_y && dimu_y<2.40) {  t_func=func_7;  ptThr=0.0; }
        } else if(isPbp==0){//pPb
            if(-2.40<= dimu_y && dimu_y<-1.93) {  t_func=func_7; ptThr=0.0; }
            else if(-1.93<= dimu_y && dimu_y<-1.46) {  t_func=func_6;  ptThr=3.0; }
            else if(-1.46<= dimu_y && dimu_y<-1.03) {  t_func=func_5;  ptThr=5.0; }
            else if(-1.03<= dimu_y && dimu_y<-0.43) {  t_func=func_4;  ptThr=6.5; }
            else if(-0.43<= dimu_y && dimu_y<0.47) {  t_func=func_3;  ptThr=6.5; }
            else if(0.47<= dimu_y && dimu_y<1.37) {  t_func=func_2;  ptThr=6.5; }
            else if(1.37<= dimu_y && dimu_y<1.97) {  t_func=func_1;  ptThr=3.0; }
            else if(1.97<= dimu_y && dimu_y<2.40) {  t_func=func_0;  ptThr=0.0; }
        }
//        if(yflag==99) continue;
 //       if(dimu_pt<ptThr) st_AccRange = "out of the range";
        float w_toy = t_func->Eval(dimu_pt);
//        if(i<100) cout << " pt : " << dimu_pt << ", y : " << dimu_y << ", weigt : " << w_toy << ", " << st_AccRange <<  endl;

	return w_toy;
}


bool kineCut(double muPt, double muEta, double muP) {
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


