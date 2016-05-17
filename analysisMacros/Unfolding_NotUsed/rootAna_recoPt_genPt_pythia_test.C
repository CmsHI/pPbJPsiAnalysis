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
bool softCut(bool muPurity, bool TrkArb, bool TMSta, double nTrk, double nPix, double dxy, double dz);
bool massCut1(double lv_dimu_mass);
bool massCut2(double lv_dimu_mass);
void formPtStr(Double_t binmin, Double_t binmax, string* arr);
void formRapStr(Double_t min, Double_t max, string* arr);

struct Condition {
	double theCentrality;
	int theType, theSign, Reco_QQ_trig, HLTriggers;
	double theMass, theRapidity, thePt, thePhi;
	double mupl_p, mumi_p, mupl_pt, mumi_pt, mupl_eta, mumi_eta;
	float theCtErr;
	bool mupl_isHighPurity, mumi_isHighPurity, mupl_TrkMuArb, mumi_TrkMuArb, mupl_TMOneStaTight, mumi_TMOneStaTight;
	Int_t mupl_nTrkWMea, mumi_nTrkWMea, mupl_nPixWMea, mumi_nPixWMea;
	Float_t mupl_dxy, mumi_dxy, mupl_dz, mumi_dz;
};

void rootAna_recoPt_genPt_pythia_test(char *strBinning = "8rap18ptMatchdR", bool isPrompt = false, bool is1st = false){

	using namespace std;
	
	// # of event range
	int initev =0;
	int nevt = -1; //all
	//int nevt = 10000;
		
	char* samplename;
	double minylab=-2.4;
	double maxylab=2.4;
	double minpt=0.0;
	double maxpt=35.0;

// read-in root file
	TFile *f1;
	if (isPrompt) {
		if ( is1st) {
			f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_noMuID_sglTrig_genMatch_20150205.root");
			samplename="PRMCboosted_Pbp"; 
		}
		else {
			f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_PromptJpsi_PYTHIAboosted_2nd_STARTHI53_V27_noMuID_sglTrig_genMatch_20150205.root");
			samplename="PRMCboosted_pPb"; 
		}
	} else  {
		if(is1st)
		{
			f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_B2Jpsi_PYTHIAboosted_1st_STARTHI53_V27_noMuID_sglTrig_genMatch_20150206.root");
			samplename="NPMCboosted_Pbp";
		}
		else
		{
			f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_B2Jpsi_PYTHIAboosted_2nd_STARTHI53_V27_noMuID_sglTrig_genMatch_20150206.root");
			samplename="NPMCboosted_pPb";
		}
	} 

	const char* datestring = Form("%s_%s",strBinning,samplename);
	std::cout << "datestring: " << datestring << std::endl;
	
	///////////////////////////////////////////////////
	// Definition of binning
	// --- pt Bin
		Double_t ptBinsArr[] = {0.0,1.5,3.0,3.5,4.0,4.5,5.0,5.8,6.5,7.0,7.5,8.0,8.5,9.0,10.0,12.5,14.0,22.0,30.0,35.0}; //20141228Finer
	//Double_t ptBinsArr[] = {0.0,3.0,4.0,5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0, 35.0}; //20141228
	const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
	cout << "nPtBins=" << nPtBins << endl;

	// --- y Bin //set to 1st run (For 2nd run, will be automatically changed later)
	Double_t tmp_yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
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


	// form array string
	string ptrange[nPtBins];
	string yrange[nPtBins];
	for (Int_t i=0; i<nPtBins; i++) {
		formPtStr(ptBinsArr[i], ptBinsArr[i+1], &ptrange[i]);
		cout << i <<"th ptrange = " << ptrange[i] << endl;
	}

	for (Int_t i=0; i<nYBins; i++){
		formRapStr(yBinsArr[i], yBinsArr[i+1], &yrange[i]);
		cout << "yrange["<<i<<"] = "<< yrange[i].c_str() << endl;
	}

	
	///////////////////////////////////////////////////
	//read-in tree
	TTree *mytree = (TTree*)f1->Get("myTree");

  UInt_t          eventNb;
  Int_t           Centrality;
  Int_t           HLTriggers;
	Float_t         zVtx;
	Int_t           Reco_QQ_size;
  Int_t           Reco_QQ_type[20];   //[Reco_QQ_size] : for Data GG, GT, TT?
  Int_t           Reco_QQ_sign[20];   //[Reco_QQ_size] : 0=+,- / 1=+/+, 2=-/-
  Int_t           Reco_QQ_trig[20];   //[Reco_QQ_size] : DoubleMuOpen ==1
	Float_t         Reco_QQ_ctau[20];
	Float_t         Reco_QQ_ctauErr[20];
  TClonesArray    *Reco_QQ_4mom;
  TClonesArray    *Reco_QQ_mupl_4mom;
  TClonesArray    *Reco_QQ_mumi_4mom;
  TClonesArray    *Reco_QQ_mupl_matchedGen_4mom;
  TClonesArray    *Reco_QQ_mumi_matchedGen_4mom;
   bool            Reco_QQ_mupl_isHighPurity[20];
   bool            Reco_QQ_mumi_isHighPurity[20];
   bool            Reco_QQ_mupl_TrkMuArb[20];
   bool            Reco_QQ_mumi_TrkMuArb[20];
   bool            Reco_QQ_mupl_TMOneStaTight[20];
   bool            Reco_QQ_mumi_TMOneStaTight[20];
   Int_t           Reco_QQ_mupl_nTrkWMea[20];
   Int_t           Reco_QQ_mumi_nTrkWMea[20];
   Int_t           Reco_QQ_mupl_nPixWMea[20];
   Int_t           Reco_QQ_mumi_nPixWMea[20];
   Float_t         Reco_QQ_mupl_dxy[20];
   Float_t         Reco_QQ_mumi_dxy[20];
   Float_t         Reco_QQ_mupl_dz[20];
   Float_t         Reco_QQ_mumi_dz[20];

	float						Reco_QQ_mupl_matchedGen_dR[20];
	float						Reco_QQ_mumi_matchedGen_dR[20];
//  Int_t           Gen_QQ_size;
//  Int_t           Gen_QQ_type[20];	//[Gen_QQ_size] : for MC PR=0 / NP=1
//  TClonesArray    *Gen_QQ_4mom;
//  TClonesArray    *Gen_QQ_mupl_4mom;
//  TClonesArray    *Gen_QQ_mumi_4mom;

  TBranch        *b_eventNb;
  TBranch        *b_Centrality;   //!
	TBranch        *b_HLTriggers;
	TBranch        *b_zVtx;
  TBranch        *b_Reco_QQ_size;   //!
  TBranch        *b_Reco_QQ_type;   //!
  TBranch        *b_Reco_QQ_sign;   //!
  TBranch        *b_Reco_QQ_trig;   //!
	TBranch        *b_Reco_QQ_ctau; 
  TBranch        *b_Reco_QQ_ctauErr;
	TBranch        *b_Reco_QQ_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_4mom;   //!
  TBranch        *b_Reco_QQ_mumi_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_matchedGen_4mom;   //!
  TBranch        *b_Reco_QQ_mumi_matchedGen_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_matchedGen_dR;   //!
  TBranch        *b_Reco_QQ_mumi_matchedGen_dR;   //!

   TBranch        *b_Reco_QQ_mupl_isHighPurity;
   TBranch        *b_Reco_QQ_mumi_isHighPurity;
   TBranch        *b_Reco_QQ_mupl_TrkMuArb;
   TBranch        *b_Reco_QQ_mumi_TrkMuArb;
   TBranch        *b_Reco_QQ_mupl_TMOneStaTight;
   TBranch        *b_Reco_QQ_mumi_TMOneStaTight;
   TBranch        *b_Reco_QQ_mupl_nTrkWMea;
   TBranch        *b_Reco_QQ_mumi_nTrkWMea;
   TBranch        *b_Reco_QQ_mupl_nPixWMea;
   TBranch        *b_Reco_QQ_mumi_nPixWMea;
   TBranch        *b_Reco_QQ_mupl_dxy;
   TBranch        *b_Reco_QQ_mumi_dxy;
   TBranch        *b_Reco_QQ_mupl_dz;
   TBranch        *b_Reco_QQ_mumi_dz;
//  TBranch        *b_Gen_QQ_size;   //!
//  TBranch        *b_Gen_QQ_type;
//  TBranch        *b_Gen_QQ_4mom;   //!
//  TBranch        *b_Gen_QQ_mupl_4mom;   //!
//  TBranch        *b_Gen_QQ_mumi_4mom;   //!

  TLorentzVector* JP_Reco = new TLorentzVector;
  TLorentzVector* m1P_Reco = new TLorentzVector;
  TLorentzVector* m2P_Reco = new TLorentzVector;
//  TLorentzVector* JP_Gen_tmp = new TLorentzVector;
//  TLorentzVector* JP_Gen_tmp_qq = new TLorentzVector;
  TLorentzVector* JP_Gen = new TLorentzVector;
  TLorentzVector* m1P_Gen = new TLorentzVector;
  TLorentzVector* m2P_Gen = new TLorentzVector;
		
	Reco_QQ_4mom =0;
	Reco_QQ_mupl_4mom =0;
	Reco_QQ_mumi_4mom =0;
	Reco_QQ_mupl_matchedGen_4mom =0;
	Reco_QQ_mumi_matchedGen_4mom =0;
//	Gen_QQ_4mom =0;
//	Gen_QQ_mupl_4mom =0;
//	Gen_QQ_mumi_4mom =0;

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
  mytree->SetBranchAddress("Reco_QQ_mupl_matchedGen_4mom", &Reco_QQ_mupl_matchedGen_4mom, &b_Reco_QQ_mupl_matchedGen_4mom);
  mytree->SetBranchAddress("Reco_QQ_mumi_matchedGen_4mom", &Reco_QQ_mumi_matchedGen_4mom, &b_Reco_QQ_mumi_matchedGen_4mom);
  mytree->SetBranchAddress("Reco_QQ_mupl_matchedGen_dR", &Reco_QQ_mupl_matchedGen_dR, &b_Reco_QQ_mupl_matchedGen_dR);
  mytree->SetBranchAddress("Reco_QQ_mumi_matchedGen_dR", &Reco_QQ_mumi_matchedGen_dR, &b_Reco_QQ_mumi_matchedGen_dR);
   mytree->SetBranchAddress("Reco_QQ_mupl_isHighPurity", &Reco_QQ_mupl_isHighPurity, &b_Reco_QQ_mupl_isHighPurity);
   mytree->SetBranchAddress("Reco_QQ_mumi_isHighPurity", &Reco_QQ_mumi_isHighPurity, &b_Reco_QQ_mumi_isHighPurity);
   mytree->SetBranchAddress("Reco_QQ_mupl_TrkMuArb", &Reco_QQ_mupl_TrkMuArb, &b_Reco_QQ_mupl_TrkMuArb);
   mytree->SetBranchAddress("Reco_QQ_mumi_TrkMuArb", &Reco_QQ_mumi_TrkMuArb, &b_Reco_QQ_mumi_TrkMuArb);
   mytree->SetBranchAddress("Reco_QQ_mupl_TMOneStaTight", &Reco_QQ_mupl_TMOneStaTight, &b_Reco_QQ_mupl_TMOneStaTight);
   mytree->SetBranchAddress("Reco_QQ_mumi_TMOneStaTight", &Reco_QQ_mumi_TMOneStaTight, &b_Reco_QQ_mumi_TMOneStaTight);
   mytree->SetBranchAddress("Reco_QQ_mupl_nTrkWMea", &Reco_QQ_mupl_nTrkWMea, &b_Reco_QQ_mupl_nTrkWMea);
   mytree->SetBranchAddress("Reco_QQ_mumi_nTrkWMea", &Reco_QQ_mumi_nTrkWMea, &b_Reco_QQ_mumi_nTrkWMea);
   mytree->SetBranchAddress("Reco_QQ_mupl_nPixWMea", &Reco_QQ_mupl_nPixWMea, &b_Reco_QQ_mupl_nPixWMea);
   mytree->SetBranchAddress("Reco_QQ_mumi_nPixWMea", &Reco_QQ_mumi_nPixWMea, &b_Reco_QQ_mumi_nPixWMea);
   mytree->SetBranchAddress("Reco_QQ_mupl_dxy", &Reco_QQ_mupl_dxy, &b_Reco_QQ_mupl_dxy);
   mytree->SetBranchAddress("Reco_QQ_mumi_dxy", &Reco_QQ_mumi_dxy, &b_Reco_QQ_mumi_dxy);
   mytree->SetBranchAddress("Reco_QQ_mupl_dz", &Reco_QQ_mupl_dz, &b_Reco_QQ_mupl_dz);
   mytree->SetBranchAddress("Reco_QQ_mumi_dz", &Reco_QQ_mumi_dz, &b_Reco_QQ_mumi_dz);
//	mytree->SetBranchAddress("Gen_QQ_size", &Gen_QQ_size, &b_Gen_QQ_size);
//	mytree->SetBranchAddress("Gen_QQ_type", Gen_QQ_type, &b_Gen_QQ_type);
//  mytree->SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
//  mytree->SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);
//  mytree->SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);

	////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	// define 2D hist
	
	// 1) integrated rapidity
	TH2D *h2D_recoY_genY_tot = new TH2D(Form("h2D_recoY_genY_tot_%s",strBinning),"",1000,-2.5,2.5,1000,-2.5,2.5);
	TH2D *h2D_recoPt_genPt_tot = new TH2D(Form("h2D_recoPt_genPt_tot_%s",strBinning),"",1000,0.,30,1000,0.,30);
	TH2D *h2D_rawMatrix_tot = new TH2D(Form("h2D_rawMatrix_tot_%s",strBinning),"",nPtBins,ptBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_fixedMatrix_tot = new TH2D(Form("h2D_fixedMatrix_tot_%s",strBinning),"",nPtBins,0,nPtBins,nPtBins,0,nPtBins);

	h2D_recoY_genY_tot->Sumw2();
	h2D_recoPt_genPt_tot->Sumw2();
	h2D_rawMatrix_tot->Sumw2();
	h2D_fixedMatrix_tot->Sumw2();

	// 2) differential rapidity
	TH2D* h2D_recoY_genY[nYBins];
	TH2D* h2D_recoPt_genPt[nYBins];
	TH2D* h2D_rawMatrix[nYBins];
	TH2D* h2D_fixedMatrix[nYBins];
   TH1D *h1D_Reco_Matrix[nYBins];
   TH1D *h1D_Gen_Matrix[nYBins];

	
	for (int in=0; in <nYBins; in++ ) {
		h2D_recoY_genY[in] = new TH2D(Form("h2D_recoY_genY_%s_%d",strBinning,in),"",1000,-2.5,2.5,1000,-2.5,2.5);
		h2D_recoPt_genPt[in] = new TH2D(Form("h2D_recoPt_genPt_%s_%d",strBinning,in),"",1000,0.,30.,1000,0.,30.);
		h2D_rawMatrix[in] = new TH2D(Form("h2D_rawMatrix_%s_%d",strBinning,in),"",nPtBins,ptBinsArr,nPtBins,ptBinsArr);
		h2D_fixedMatrix[in] = new TH2D(Form("h2D_fixedMatrix_%s_%d",strBinning,in),"",nPtBins,0,nPtBins,nPtBins,0,nPtBins);
	   h1D_Reco_Matrix[in] = new TH1D(Form("h1D_Reco_Matrix_%s_%d",strBinning,in),"",nPtBins,0,nPtBins);
	   h1D_Gen_Matrix[in] = new TH1D(Form("h1D_Gen_Matrix_%s_%d",strBinning,in),"",nPtBins,0,nPtBins);
		//cout << in<<"th recoPt_genPt = " <<h2D_recoPt_genPt[in] << endl; 
		h2D_recoY_genY[in]->Sumw2();
		h2D_recoPt_genPt[in]->Sumw2();
		h2D_rawMatrix[in]->Sumw2();
		h2D_fixedMatrix[in]->Sumw2();
   	h1D_Reco_Matrix[in]->Sumw2();
   	h1D_Gen_Matrix[in]->Sumw2();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	// event loop start
	if(nevt == -1) nevt = mytree->GetEntries();
	for(int iev=initev; iev<nevt; ++iev){
		if(iev%100000==0) cout << ">>>>> EVENT " << iev << " / " << mytree->GetEntries() <<  endl;

		//bool yn_reco = false;
		//bool yn_gen = false;
		struct Condition Jpsi_Reco; 
		struct Condition Jpsi_Gen; 

		mytree->GetEntry(iev);
		
		//////  GEN-RECO matching temporary
		//if (! (Gen_QQ_size ==1 && Reco_QQ_size==1) ) continue;

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
	
         Jpsi_Reco.mupl_isHighPurity = Reco_QQ_mupl_isHighPurity[irqq];
         Jpsi_Reco.mumi_isHighPurity = Reco_QQ_mumi_isHighPurity[irqq];
         Jpsi_Reco.mupl_TrkMuArb = Reco_QQ_mupl_TrkMuArb[irqq];
         Jpsi_Reco.mumi_TrkMuArb = Reco_QQ_mumi_TrkMuArb[irqq];
         Jpsi_Reco.mupl_TMOneStaTight = Reco_QQ_mupl_TMOneStaTight[irqq];
         Jpsi_Reco.mumi_TMOneStaTight = Reco_QQ_mumi_TMOneStaTight[irqq];
         Jpsi_Reco.mupl_nTrkWMea = Reco_QQ_mupl_nTrkWMea[irqq];
         Jpsi_Reco.mumi_nTrkWMea = Reco_QQ_mumi_nTrkWMea[irqq];
         Jpsi_Reco.mupl_nPixWMea = Reco_QQ_mupl_nPixWMea[irqq];
         Jpsi_Reco.mumi_nPixWMea = Reco_QQ_mumi_nPixWMea[irqq];
         Jpsi_Reco.mupl_dxy = Reco_QQ_mupl_dxy[irqq];
         Jpsi_Reco.mumi_dxy = Reco_QQ_mumi_dxy[irqq];
         Jpsi_Reco.mupl_dz = Reco_QQ_mupl_dz[irqq];
         Jpsi_Reco.mumi_dz = Reco_QQ_mumi_dz[irqq];

			bool yn_reco = false;
			bool yn_gen = false;

			if ( Jpsi_Reco.theSign ==0 
				//&& massCut1(Jpsi_Reco.theMass)
				&& massCut2(Jpsi_Reco.theMass)
				&& ( (Jpsi_Reco.Reco_QQ_trig&1)==1 && (Jpsi_Reco.HLTriggers&1)==1 )
				&& kineCut(Jpsi_Reco.mupl_pt, Jpsi_Reco.mupl_eta, Jpsi_Reco.mupl_p) 
				&& kineCut(Jpsi_Reco.mumi_pt, Jpsi_Reco.mumi_eta, Jpsi_Reco.mumi_p) 
            && softCut(Jpsi_Reco.mupl_isHighPurity, Jpsi_Reco.mupl_TrkMuArb, Jpsi_Reco.mupl_TMOneStaTight, Jpsi_Reco.mupl_nTrkWMea, Jpsi_Reco.mupl_nPixWMea, Jpsi_Reco.mupl_dxy, Jpsi_Reco.mupl_dz)
            && softCut(Jpsi_Reco.mumi_isHighPurity, Jpsi_Reco.mumi_TrkMuArb, Jpsi_Reco.mumi_TMOneStaTight, Jpsi_Reco.mumi_nTrkWMea, Jpsi_Reco.mumi_nPixWMea, Jpsi_Reco.mumi_dxy, Jpsi_Reco.mumi_dz)
				&& minpt<=Jpsi_Reco.thePt && Jpsi_Reco.thePt < maxpt
				&& minylab<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity < maxylab) {
					yn_reco = true;
			}

			// Gen matching with new root file !!
			if (yn_reco) {
				m1P_Gen = (TLorentzVector*) Reco_QQ_mupl_matchedGen_4mom->At(irqq);
				m2P_Gen = (TLorentzVector*) Reco_QQ_mumi_matchedGen_4mom->At(irqq);

				// variables only used for Reco
				Jpsi_Gen.HLTriggers = 0;
				Jpsi_Gen.Reco_QQ_trig  = 0;
				Jpsi_Gen.theSign = 0; //already +- pair
			
				Jpsi_Gen.theCentrality = 97.5; // for pp!
				//Jpsi_Gen.theType = Gen_QQ_type[igqq]; // PR or NP

				Jpsi_Gen.mupl_p = sqrt( (m1P_Gen->Px())*(m1P_Gen->Px()) + (m1P_Gen->Py())*(m1P_Gen->Py()) + (m1P_Gen->Pz())*(m1P_Gen->Pz()) );
				Jpsi_Gen.mumi_p = sqrt( (m2P_Gen->Px())*(m2P_Gen->Px()) + (m2P_Gen->Py())*(m2P_Gen->Py()) + (m2P_Gen->Pz())*(m2P_Gen->Pz()) );
				Jpsi_Gen.mupl_pt = m1P_Gen->Pt();
				Jpsi_Gen.mumi_pt = m2P_Gen->Pt();
				Jpsi_Gen.mupl_eta = m1P_Gen->Eta();
				Jpsi_Gen.mumi_eta = m2P_Gen->Eta();
				if ( //massCut1(Jpsi_Gen.theMass)
				kineCut(Jpsi_Gen.mupl_pt, Jpsi_Gen.mupl_eta, Jpsi_Gen.mupl_p) 
				&& kineCut(Jpsi_Gen.mumi_pt, Jpsi_Gen.mumi_eta, Jpsi_Gen.mumi_p)
				&& Reco_QQ_mupl_matchedGen_dR[irqq] < 0.2 // KYO dR cut
				&& Reco_QQ_mumi_matchedGen_dR[irqq] < 0.2 ) {
					*JP_Gen = *m1P_Gen +  *m2P_Gen; // instead of Gen_QQ
					Jpsi_Gen.theMass = JP_Gen->M();
					Jpsi_Gen.theRapidity = JP_Gen->Rapidity();	
					Jpsi_Gen.thePt = JP_Gen->Pt();
					Jpsi_Gen.thePhi = JP_Gen->Phi();
					if ( massCut1(Jpsi_Gen.theMass)  
							&& minpt<=Jpsi_Gen.thePt && Jpsi_Gen.thePt <maxpt 
							&& minylab<=Jpsi_Gen.theRapidity && Jpsi_Gen.theRapidity <maxylab) {
								yn_gen = true;
					}
				} //yn_gen
			} //yn_reco
				
			// fill the histogram
			if (yn_reco == true &&yn_gen == true){
				// 1) integrated rapidity
				h2D_recoY_genY_tot->Fill(Jpsi_Gen.theRapidity,Jpsi_Reco.theRapidity);
				h2D_recoPt_genPt_tot->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
				h2D_rawMatrix_tot->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
				// 2) differential rapidity
				for (int in=0; in <nYBins; in++ ) {
					if (yBinsArr[in]<=Jpsi_Gen.theRapidity && Jpsi_Gen.theRapidity < yBinsArr[in+1]){ 
						//cout << in<<"th rap range from " << yBinsArr[in] << "to " << yBinsArr[in+1] << endl;
						h2D_recoY_genY[in]->Fill(Jpsi_Gen.theRapidity,Jpsi_Reco.theRapidity);
						h2D_recoPt_genPt[in]->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
						h2D_rawMatrix[in]->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
					}
				}
			}
		} // end of Reco_QQ_size loop

/*	
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
	
			// --- cut02 : GEN for denominator
			//bool yn_gen = false;
			if ( //massCut1(Jpsi_Gen.theMass)
			kineCut(Jpsi_Gen.mupl_pt, Jpsi_Gen.mupl_eta, Jpsi_Gen.mupl_p) 
			&& kineCut(Jpsi_Gen.mumi_pt, Jpsi_Gen.mumi_eta, Jpsi_Gen.mumi_p)) {
				*JP_Gen = *m1P_Gen +  *m2P_Gen; // instead of Gen_QQ
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
		} //end of Gen_QQ_size loop
*/
/*
		// fill the histogram
		if (yn_reco == true &&yn_gen == true){
			h2D_recoY_genY_tot->Fill(Jpsi_Gen.theRapidity,Jpsi_Reco.theRapidity);
			h2D_recoPt_genPt_tot->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
			h2D_rawMatrix_tot->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
			//h2D_fixedMatrix_tot->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
		}
*/
	} //end of event loop

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// calculate for fixedMatrix
	// 1) integrated rapidity
	double fix_entries[nPtBins][nPtBins];
	double fixden_entries[nPtBins];

	for (Int_t igenpt=0; igenpt< nPtBins; igenpt++){	 
		fixden_entries[igenpt] =0;
		//cout << "***** igenpt= " <<igenpt << " ***********" << endl;
		for (Int_t irecopt=0; irecopt<nPtBins; irecopt++) {
			fix_entries[igenpt][irecopt] = (double)h2D_rawMatrix_tot->GetBinContent(igenpt+1, irecopt+1);
			fixden_entries[igenpt] += (double)h2D_rawMatrix_tot->GetBinContent(igenpt+1, irecopt+1);
			//cout << "* irecopt= "<<irecopt<< endl;
			//cout << "fix_entries = " <<fix_entries[igenpt][irecopt]<<endl;
			//cout << "fixden_entries = " <<fixden_entries[igenpt]<<endl;
		}
	}

	for (Int_t igenpt=0; igenpt< nPtBins; igenpt++){	 
		for (Int_t irecopt=0; irecopt<nPtBins; irecopt++) {
			//cout << "***** igenpt= " <<igenpt <<", irecopt= "<<irecopt<< endl;
			//cout << "fix_entries = " <<fix_entries[igenpt][irecopt]<<endl;
			//cout << "fixden_entries = " <<fixden_entries[igenpt]<<endl;
			h2D_fixedMatrix_tot->SetBinContent(igenpt+1, irecopt+1, fix_entries[igenpt][irecopt]/fixden_entries[igenpt]);	
		}
	}

	// axis
	for (Int_t igenpt=0; igenpt< nPtBins; igenpt++){	 
		h2D_fixedMatrix_tot->GetXaxis()->SetBinLabel(igenpt+1,ptrange[igenpt].c_str());
	}
	for (Int_t irecopt=0; irecopt< nPtBins; irecopt++){	 
		h2D_fixedMatrix_tot->GetYaxis()->SetBinLabel(irecopt+1,ptrange[irecopt].c_str());
	}

	////////////////////////////////////////////////////////////////////////////
	// 2) differential rapidity
	double fix_entries_diff[nYBins][nPtBins][nPtBins];
	double fixden_entries_diff[nYBins][nPtBins];
   double Reco_entries[nYBins][nPtBins];
   double Gen_entries[nYBins][nPtBins];

	for (Int_t in=0; in< nYBins; in++){
		for (Int_t igenpt=0; igenpt< nPtBins; igenpt++){	 
			fixden_entries_diff[in][igenpt] =0;
			Reco_entries[in][igenpt] = 0;
			Gen_entries[in][igenpt] = 0;
			for (Int_t irecopt=0; irecopt<nPtBins; irecopt++) {
				fix_entries_diff[in][igenpt][irecopt] = (double)h2D_rawMatrix[in]->GetBinContent(igenpt+1, irecopt+1);
				fixden_entries_diff[in][igenpt] += (double)h2D_rawMatrix[in]->GetBinContent(igenpt+1, irecopt+1);
				Reco_entries[in][igenpt] += (double)h2D_rawMatrix[in]->GetBinContent(irecopt+1, igenpt+1);
				Gen_entries[in][igenpt] += (double)h2D_rawMatrix[in]->GetBinContent(igenpt+1, irecopt+1);
			}
		}
	}

	for (Int_t in=0; in< nYBins; in++){
		for (Int_t igenpt=0; igenpt< nPtBins; igenpt++){	 
			for (Int_t irecopt=0; irecopt<nPtBins; irecopt++) {
				h2D_fixedMatrix[in]->SetBinContent(igenpt+1, irecopt+1, fix_entries_diff[in][igenpt][irecopt]/fixden_entries_diff[in][igenpt]);	
			}
			h1D_Reco_Matrix[in]->SetBinContent(igenpt+1, Reco_entries[in][igenpt]);
			h1D_Gen_Matrix[in]->SetBinContent(igenpt+1, Gen_entries[in][igenpt]);
		}
	}
	// axis
	for (Int_t in=0; in< nYBins; in++){
		for (Int_t igenpt=0; igenpt< nPtBins; igenpt++){	 
			h2D_fixedMatrix[in]->GetXaxis()->SetBinLabel(igenpt+1,ptrange[igenpt].c_str());
		}
		for (Int_t irecopt=0; irecopt< nPtBins; irecopt++){	 
			h2D_fixedMatrix[in]->GetYaxis()->SetBinLabel(irecopt+1,ptrange[irecopt].c_str());
		}
	}

	
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Save the data!

	// --- save03 : save as a root file
	TFile *outFile = new TFile(Form("recoPt_genPt_%s_test.root",datestring),"RECREATE");
	std::cout << "datestring: " << datestring << std::endl;
	outFile->cd();
	h2D_recoY_genY_tot->Write();
	h2D_recoPt_genPt_tot->Write();
	h2D_rawMatrix_tot->Write();
	h2D_fixedMatrix_tot->Write();
	for (Int_t in=0; in< nYBins; in++){
		h2D_recoY_genY[in]->Write();
		h2D_recoPt_genPt[in]->Write();
		h2D_rawMatrix[in]->Write();
		h2D_fixedMatrix[in]->Write();
      h1D_Reco_Matrix[in]->Write();
      h1D_Gen_Matrix[in]->Write();
	}	
	outFile->Close();

} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

bool kineCut(double muPt, double muEta, double muP) {
	return ( TMath::Abs(muEta) < 2.4 &&
				((TMath::Abs(muEta) < 1.3 && muPt >=3.3) ||
				(1.3 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.2 && muP >=2.9) ||
				(2.2 <= TMath::Abs(muEta) && muPt >= 0.8)));
}

bool massCut1(double lv_dimu_mass) {
	if(lv_dimu_mass < 2.6 || lv_dimu_mass >= 3.5) {return false;}
	else {return true;}
}

bool massCut2(double lv_dimu_mass) {
	//if(lv_dimu_mass < 2.95 || lv_dimu_mass >= 3.25) {return false;}
	if(lv_dimu_mass < 2.9 || lv_dimu_mass >= 3.3) {return false;}
	else {return true;}
}

bool softCut(bool muPurity, bool TrkArb, bool TMSta, double nTrk, double nPix, double dxy, double dz)     
{  
   return (muPurity == 1
         && TrkArb == 1
         && TMSta == 1
         && nTrk > 5
         && nPix > 0 
         && dxy < 0.3
         && dz < 20
   );
}

void formPtStr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
//		*arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
		*arr = Form("%.0f-%.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f-%.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f-%.1f", binmin, binmax);
	} else {
		*arr = Form("%.1f-%.1f", binmin, binmax);
	}
}

void formRapStr(Double_t min, Double_t max, string* arr) { //byHand KYO
	if (min==-2.4 || min==2.4) *arr = Form("%.1f-%.2f",min,max);
	else if (max==-2.4 || max==2.4) *arr = Form("%.2f-%.1f",min,max);
	else *arr = Form("%.2f-%.2f",min,max);
}

