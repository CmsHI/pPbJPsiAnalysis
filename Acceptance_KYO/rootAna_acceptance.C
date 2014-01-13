#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
//#include <TH1D.h>
#include <TH1.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
//#include <TH2D.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TAxis.h>
#include <cmath>
#include <TLorentzRotation.h>

// Basically our basic frame is collision frame, in this code all value means in collision frame.

const double shiftvar = -0.47; // conversion constant y=0(collision)==y=-0.47(LAB frame)  

bool dimuCut(int, int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch);
bool kineCut(bool, double muPt, double muEta, double muP);
bool massCut(double lv_dimu_mass);

void rootAna_acceptance(char *stringA = "20140113_pt6bin", bool isBoosted = true, int OniaCode=11, double rapbinmin=-2.4, double rapbinmax=1.46, double ptbinmin=6.5, double ptbinmax=30.0){
	
	gROOT->Macro("rootlogon.C+");
	gStyle->SetCanvasDefW(800);

	TFile *f1;
	char* samplename;
	double minylab;
	double maxylab;
	double minpt;
	double maxpt;

// read-in root file
	if (isBoosted) {
		//f1 = new TFile("./pythia6_PromptJpsi_boosted_totevt5M_MuonAna_20130210.root"); samplename="PRMC_boosted";
		f1 = new TFile("/afs/cern.ch/work/k/kyolee/private/cms538HI/src/pAJpsiAcceptance201309/4_GetAcceptance/pythia6_PromptJpsi_boosted_totevt5M_MuonAna_20130210.root"); samplename="PRMC_boosted";
		minylab=-2.4; maxylab=2.4;
		minpt=0.0; maxpt=30.0;
	} else {
		//minylab=-2.4;
		//maxylab=2.4;
	}

	const char* datestring = Form("%s_%s",stringA,samplename);
	std::cout << "datestring: " << datestring << std::endl;

	//read-in tree
	TTree *ana2 = (TTree*)f1->Get("DiAna");

	int dkid0_Id, dkid0_ch, dkid0_st;	
	int dkid1_Id, dkid1_ch, dkid1_st;
	double dkid0_pt, dkid0_eta, dkid0_y, dkid0_px, dkid0_py, dkid0_pz, dkid0_phi, dkid0_mass, dkid0_p;
	double dkid1_pt, dkid1_eta, dkid1_y, dkid1_px, dkid1_py, dkid1_pz, dkid1_phi, dkid1_mass, dkid1_p;
	double dimu_pt, dimu_eta, dimu_y, dimu_px, dimu_py, dimu_pz, dimu_phi, dimu_mass, dimu_p;
	int dmom0_Id, dmom0_ch, dmom0_st;	
	int dgmom0_Id, dgmom0_ch, dgmom0_st;
	double dmom0_pt, dmom0_eta, dmom0_y, dmom0_px, dmom0_py, dmom0_pz, dmom0_phi, dmom0_mass, dmom0_p;
	double dgmom0_pt, dgmom0_eta, dgmom0_y, dgmom0_px, dgmom0_py, dgmom0_pz, dgmom0_phi, dgmom0_mass, dgmom0_p;

	using namespace std;

	// Definition of bin
	// --- pt Bin
	Double_t ptBinsArr1[] = {6.5, 30.0}; //1bin
	//Double_t ptBinsArr2[] = {6.5, 8.0, 10.0, 13.0, 30.0}; //4bin
	//Double_t ptBinsArr2[] = {6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 13.0, 16.0, 20.0, 30.0}; //9bin
	Double_t ptBinsArr2[] = {6.5, 7.5, 8.5, 9.5, 11.0, 14.0, 30.0}; //6bin
	const Int_t nPtBins1 = sizeof(ptBinsArr1)/sizeof(double)-1;
	const Int_t nPtBins2 = sizeof(ptBinsArr2)/sizeof(double)-1;
	cout << "nPtBins1=" << nPtBins1 << endl;
	cout << "nPtBins2=" << nPtBins2 << endl;

	// --- y Bin
	Double_t yBinsArr1[] = {-2.4, -1.97, -1.72, -1.47, -1.22, -0.97, -0.72, -0.47, -0.22, 0.03, 0.28, 0.53, 0.78, 1.03, 1.46}; //14bin
	Double_t yBinsArr2[] = {-2.4, -0.47, 1.46}; //2bin
	Double_t yBinsArr3[] = {-2.4, 1.46}; //rapInteg
	const Int_t nYBins1 = sizeof(yBinsArr1)/sizeof(double)-1;
	const Int_t nYBins2 = sizeof(yBinsArr2)/sizeof(double)-1;
	const Int_t nYBins3 = sizeof(yBinsArr3)/sizeof(double)-1;
	cout << "nYBins1=" << nYBins1 << endl;
	cout << "nYBins2=" << nYBins2 << endl;
	cout << "nYBins3=" << nYBins3 << endl;


	//read-in branches
	ana2->SetBranchAddress("dimu_eta",	&dimu_eta);
	ana2->SetBranchAddress("dimu_mass",	&dimu_mass);
	ana2->SetBranchAddress("dimu_p",	&dimu_p);
	ana2->SetBranchAddress("dimu_phi",	&dimu_phi);
	ana2->SetBranchAddress("dimu_pt",	&dimu_pt);
	ana2->SetBranchAddress("dimu_px",	&dimu_px);
	ana2->SetBranchAddress("dimu_py",	&dimu_py);
	ana2->SetBranchAddress("dimu_pz",	&dimu_pz);
	ana2->SetBranchAddress("dimu_y",	&dimu_y);

	ana2->SetBranchAddress("dgmom0_Id",	&dgmom0_Id);
	ana2->SetBranchAddress("dgmom0_ch",	&dgmom0_ch);
	ana2->SetBranchAddress("dgmom0_eta",	&dgmom0_eta);
	ana2->SetBranchAddress("dgmom0_mass",	&dgmom0_mass);
	ana2->SetBranchAddress("dgmom0_p",	&dgmom0_p);
	ana2->SetBranchAddress("dgmom0_phi",	&dgmom0_phi);
	ana2->SetBranchAddress("dgmom0_pt",	&dgmom0_pt);
	ana2->SetBranchAddress("dgmom0_px",	&dgmom0_px);
	ana2->SetBranchAddress("dgmom0_py",	&dgmom0_py);
	ana2->SetBranchAddress("dgmom0_pz",	&dgmom0_pz);
	ana2->SetBranchAddress("dgmom0_st",	&dgmom0_st);
	ana2->SetBranchAddress("dgmom0_y",	&dgmom0_y);

	ana2->SetBranchAddress("dmom0_Id",	&dmom0_Id);
	ana2->SetBranchAddress("dmom0_ch",	&dmom0_ch);
	ana2->SetBranchAddress("dmom0_eta",	&dmom0_eta);
	ana2->SetBranchAddress("dmom0_mass",	&dmom0_mass);
	ana2->SetBranchAddress("dmom0_p",	&dmom0_p);
	ana2->SetBranchAddress("dmom0_phi",	&dmom0_phi);
	ana2->SetBranchAddress("dmom0_pt",	&dmom0_pt);
	ana2->SetBranchAddress("dmom0_px",	&dmom0_px);
	ana2->SetBranchAddress("dmom0_py",	&dmom0_py);
	ana2->SetBranchAddress("dmom0_pz",	&dmom0_pz);
	ana2->SetBranchAddress("dmom0_st",	&dmom0_st);
	ana2->SetBranchAddress("dmom0_y",	&dmom0_y);

	ana2->SetBranchAddress("dkid0_Id",	&dkid0_Id);
	ana2->SetBranchAddress("dkid0_ch",	&dkid0_ch);
	ana2->SetBranchAddress("dkid0_eta",	&dkid0_eta);
	ana2->SetBranchAddress("dkid0_mass",	&dkid0_mass);
	ana2->SetBranchAddress("dkid0_p",	&dkid0_p);
	ana2->SetBranchAddress("dkid0_phi",	&dkid0_phi);
	ana2->SetBranchAddress("dkid0_pt",	&dkid0_pt);
	ana2->SetBranchAddress("dkid0_px",	&dkid0_px);
	ana2->SetBranchAddress("dkid0_py",	&dkid0_py);
	ana2->SetBranchAddress("dkid0_pz",	&dkid0_pz);
	ana2->SetBranchAddress("dkid0_st",	&dkid0_st);
	ana2->SetBranchAddress("dkid0_y",	&dkid0_y);

	ana2->SetBranchAddress("dkid1_Id",	&dkid1_Id);
	ana2->SetBranchAddress("dkid1_ch",	&dkid1_ch);
	ana2->SetBranchAddress("dkid1_eta",	&dkid1_eta);
	ana2->SetBranchAddress("dkid1_mass",	&dkid1_mass);
	ana2->SetBranchAddress("dkid1_p",	&dkid1_p);
	ana2->SetBranchAddress("dkid1_phi",	&dkid1_phi);
	ana2->SetBranchAddress("dkid1_pt",	&dkid1_pt);
	ana2->SetBranchAddress("dkid1_px",	&dkid1_px);
	ana2->SetBranchAddress("dkid1_py",	&dkid1_py);
	ana2->SetBranchAddress("dkid1_pz",	&dkid1_pz);
	ana2->SetBranchAddress("dkid1_st",	&dkid1_st);
	ana2->SetBranchAddress("dkid1_y",	&dkid1_y);

	cout << "Entries of tree : " << ana2->GetEntries() << endl;
	std::cout << "datestring: " << datestring << std::endl;

	////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	// define 2D hist
	// fine binning
	TH2D *h2D_NoCut_pt_y = new TH2D(Form("h2D_NoCut_pt_y_%s",stringA),"",500,-2.5,2.5,300,0,30);
	TH2D *h2D_Den_pt_y = new TH2D(Form("h2D_Den_pt_y_%s",stringA),"",500,-2.5,2.5,300,0,30);
	TH2D *h2D_Num_pt_y = new TH2D(Form("h2D_Num_pt_y_%s",stringA),"",500,-2.5,2.5,300,0,30);
	TH2D *h2D_Acc_pt_y = new TH2D(Form("h2D_Acc_pt_y_%s",stringA),"",500,-2.5,2.5,300,0,30);
	// with defined binning 1 (y dependence)
	TH2D *h2D_NoCut_pt_y_bin1 = new TH2D(Form("h2D_NoCut_pt_y_bin1_%s",stringA),"",nYBins1,yBinsArr1,nPtBins1,ptBinsArr1);
	TH2D *h2D_Den_pt_y_bin1 = new TH2D(Form("h2D_Den_pt_y_bin1_%s",stringA),"",nYBins1,yBinsArr1,nPtBins1,ptBinsArr1);
	TH2D *h2D_Num_pt_y_bin1 = new TH2D(Form("h2D_Num_pt_y_bin1_%s",stringA),"",nYBins1,yBinsArr1,nPtBins1,ptBinsArr1);
	TH2D *h2D_Acc_pt_y_bin1 = new TH2D(Form("h2D_Acc_pt_y_bin1_%s",stringA),"",nYBins1,yBinsArr1,nPtBins1,ptBinsArr1);
	// with defined binning 2 (pt dependence)
	TH2D *h2D_NoCut_pt_y_bin2 = new TH2D(Form("h2D_NoCut_pt_y_bin2_%s",stringA),"",nYBins2,yBinsArr2,nPtBins2,ptBinsArr2);
	TH2D *h2D_Den_pt_y_bin2 = new TH2D(Form("h2D_Den_pt_y_bin2_%s",stringA),"",nYBins2,yBinsArr2,nPtBins2,ptBinsArr2);
	TH2D *h2D_Num_pt_y_bin2 = new TH2D(Form("h2D_Num_pt_y_bin2_%s",stringA),"",nYBins2,yBinsArr2,nPtBins2,ptBinsArr2);
	TH2D *h2D_Acc_pt_y_bin2 = new TH2D(Form("h2D_Acc_pt_y_bin2_%s",stringA),"",nYBins2,yBinsArr2,nPtBins2,ptBinsArr2);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// define 1D hist
	// fine binning
	TH1D *h1D_NoCut_y = new TH1D(Form("h1D_NoCut_y_%s",stringA),"",500,-2.5,2.5);
	TH1D *h1D_NoCut_pt = new TH1D(Form("h1D_NoCut_pt_%s",stringA),"",300,0,30);
	TH1D *h1D_Den_y = new TH1D(Form("h1D_Den_y_%s",stringA),"",500,-2.5,2.5);
	TH1D *h1D_Den_pt = new TH1D(Form("h1D_Den_pt_%s",stringA),"",300,0,30);
	TH1D *h1D_Num_y = new TH1D(Form("h1D_Num_y_%s",stringA),"",500,-2.5,2.5);
	TH1D *h1D_Num_pt = new TH1D(Form("h1D_Num_pt_%s",stringA),"",300,0,30);
	TH1D *h1D_Acc_y = new TH1D(Form("h1D_Acc_y_%s",stringA),"",500,-2.5,2.5);
	TH1D *h1D_Acc_pt = new TH1D(Form("h1D_Acc_pt_%s",stringA),"",300,0,30);
	// with defined binning 1 (y dependence)
	TH1D *h1D_NoCut_y_bin1 = new TH1D(Form("h1D_NoCut_y_bin1_%s",stringA),"",nYBins1,yBinsArr1);
	TH1D *h1D_Den_y_bin1 = new TH1D(Form("h1D_Den_y_bin1_%s",stringA),"",nYBins1,yBinsArr1);
	TH1D *h1D_Num_y_bin1 = new TH1D(Form("h1D_Num_y_bin1_%s",stringA),"",nYBins1,yBinsArr1);
	TH1D *h1D_Acc_y_bin1 = new TH1D(Form("h1D_Acc_y_bin1_%s",stringA),"",nYBins1,yBinsArr1);
	// with defined binning 2 (pt dependence - FW)
	TH1D *h1D_NoCut_pt_bin2_FW = new TH1D(Form("h1D_NoCut_pt_bin2_FW_%s",stringA),"",nPtBins2,ptBinsArr2);
	TH1D *h1D_Den_pt_bin2_FW = new TH1D(Form("h1D_Den_pt_bin2_FW_%s",stringA),"",nPtBins2,ptBinsArr2);
	TH1D *h1D_Num_pt_bin2_FW = new TH1D(Form("h1D_Num_pt_bin2_FW_%s",stringA),"",nPtBins2,ptBinsArr2);
	TH1D *h1D_Acc_pt_bin2_FW = new TH1D(Form("h1D_Acc_pt_bin2_FW_%s",stringA),"",nPtBins2,ptBinsArr2);
	// with defined binning 2 (pt dependence - BW)
	TH1D *h1D_NoCut_pt_bin2_BW = new TH1D(Form("h1D_NoCut_pt_bin2_BW_%s",stringA),"",nPtBins2,ptBinsArr2);
	TH1D *h1D_Den_pt_bin2_BW = new TH1D(Form("h1D_Den_pt_bin2_BW_%s",stringA),"",nPtBins2,ptBinsArr2);
	TH1D *h1D_Num_pt_bin2_BW = new TH1D(Form("h1D_Num_pt_bin2_BW_%s",stringA),"",nPtBins2,ptBinsArr2);
	TH1D *h1D_Acc_pt_bin2_BW = new TH1D(Form("h1D_Acc_pt_bin2_BW_%s",stringA),"",nPtBins2,ptBinsArr2);
	// with defined binning 3 (pt dependence)
	TH1D *h1D_NoCut_pt_bin2 = new TH1D(Form("h1D_NoCut_pt_bin2_%s",stringA),"",nPtBins2,ptBinsArr2);
	TH1D *h1D_Den_pt_bin2 = new TH1D(Form("h1D_Den_pt_bin2_%s",stringA),"",nPtBins2,ptBinsArr2);
	TH1D *h1D_Num_pt_bin2 = new TH1D(Form("h1D_Num_pt_bin2_%s",stringA),"",nPtBins2,ptBinsArr2);
	TH1D *h1D_Acc_pt_bin2 = new TH1D(Form("h1D_Acc_pt_bin2_%s",stringA),"",nPtBins2,ptBinsArr2);
	

	std::cout << "TH1, TH2 definition, datestring: " << datestring << std::endl;

	// mass check
	TH1D *h1D_NoCut_M = new TH1D("h1D_NoCut_M","h1D_NoCut_M",100,2.6,3.5);
	TH1D *h1D_Den_M = new TH1D("h1D_Den_M","h1D_Den_M",100,2.6,3.5);
	TH1D *h1D_Num_M = new TH1D("h1D_Num_M","h1D_Num_M",100,2.6,3.5);

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	//2D hist
	h2D_Den_pt_y->Sumw2();
	h2D_Num_pt_y->Sumw2();
	h2D_Acc_pt_y->Sumw2();

	h2D_Den_pt_y_bin1->Sumw2();
	h2D_Num_pt_y_bin1->Sumw2();
	h2D_Acc_pt_y_bin1->Sumw2();

	h2D_Den_pt_y_bin2->Sumw2();
	h2D_Num_pt_y_bin2->Sumw2();
	h2D_Acc_pt_y_bin2->Sumw2();

	//1D hist
	h1D_Den_y->Sumw2();
	h1D_Num_y->Sumw2();
	h1D_Acc_y->Sumw2();
	h1D_Den_pt->Sumw2();
	h1D_Num_pt->Sumw2();
	h1D_Acc_pt->Sumw2();

	h1D_Den_y_bin1->Sumw2();
	h1D_Num_y_bin1->Sumw2();
	h1D_Acc_y_bin1->Sumw2();

	h1D_Den_pt_bin2_FW->Sumw2();
	h1D_Num_pt_bin2_FW->Sumw2();
	h1D_Acc_pt_bin2_FW->Sumw2();
	h1D_Den_pt_bin2_BW->Sumw2();
	h1D_Num_pt_bin2_BW->Sumw2();
	h1D_Acc_pt_bin2_BW->Sumw2();

	h1D_Den_pt_bin2->Sumw2();
	h1D_Num_pt_bin2->Sumw2();
	h1D_Acc_pt_bin2->Sumw2();

	h1D_NoCut_M->Sumw2();
	h1D_Den_M->Sumw2();
	h1D_Num_M->Sumw2();

	////////////////////////////////////////////////////////////////////////////////////////////////

	std::cout << "before check acc. datestring: " << datestring << std::endl;

	// loop start
	for(int i=0; i<ana2->GetEntries(); i++){
		ana2->GetEntry(i);

		// --- cut01 : no cut
		h2D_NoCut_pt_y->Fill(dimu_y,dimu_pt);
		h2D_NoCut_pt_y_bin1->Fill(dimu_y,dimu_pt);
		h2D_NoCut_pt_y_bin2->Fill(dimu_y,dimu_pt);

		h1D_NoCut_y->Fill(dimu_y);
		h1D_NoCut_pt->Fill(dimu_pt);
		h1D_NoCut_y_bin1->Fill(dimu_y);
		h1D_NoCut_pt_bin2_FW->Fill(dimu_pt);
		h1D_NoCut_pt_bin2_BW->Fill(dimu_pt);
		h1D_NoCut_pt_bin2->Fill(dimu_pt);

		h1D_NoCut_M->Fill(dimu_mass);

		if (!dimuCut(OniaCode, dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch)) continue;

		// --- cut02 : for denominator
		bool yn = false;
		if (dimuCut(OniaCode,dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) {yn = true;} // arbitrary

		if ( yn == true  ) {
			h2D_Den_pt_y->Fill(dimu_y,dimu_pt);
			h2D_Den_pt_y_bin1->Fill(dimu_y,dimu_pt);
			h2D_Den_pt_y_bin2->Fill(dimu_y,dimu_pt);

			h1D_Den_y->Fill(dimu_y);
			h1D_Den_pt->Fill(dimu_pt);

			h1D_Den_M->Fill(dimu_mass);

			// cut for 1D hist with binning
			if (ptbinmin<=dimu_pt && dimu_pt < ptbinmax)	{
				h1D_Den_y_bin1->Fill(dimu_y);
			}
			if (rapbinmin<=dimu_y && dimu_y<shiftvar) {
				h1D_Den_pt_bin2_FW->Fill(dimu_pt);
			}
			if (shiftvar<=dimu_y && dimu_y<rapbinmax) {
				h1D_Den_pt_bin2_BW->Fill(dimu_pt);
			}
			if (rapbinmin<=dimu_y && dimu_y<rapbinmax) {
				h1D_Den_pt_bin2->Fill(dimu_pt);
			}

			// --- cut03 : for numerator
			bool yn2 = false;
			if (massCut(dimu_mass) && dimuCut(OniaCode,dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch) && kineCut(isBoosted,dkid0_pt,dkid0_eta,dkid0_p) && kineCut(isBoosted,dkid1_pt,dkid1_eta,dkid1_p) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) {yn2 = true;} // arbitrary

			if (yn2 == true){
				h2D_Num_pt_y->Fill(dimu_y,dimu_pt);
				h2D_Num_pt_y_bin1->Fill(dimu_y,dimu_pt);
				h2D_Num_pt_y_bin2->Fill(dimu_y,dimu_pt);

				h1D_Num_y->Fill(dimu_y);
				h1D_Num_pt->Fill(dimu_pt);
			
				h1D_Num_M->Fill(dimu_mass);
				
				// cut for 1D hist with binning
				if (ptbinmin<=dimu_pt && dimu_pt < ptbinmax)	{
					h1D_Num_y_bin1->Fill(dimu_y);
				}
				if (rapbinmin<=dimu_y && dimu_y<shiftvar) {
					h1D_Num_pt_bin2_FW->Fill(dimu_pt);
				}
				if (shiftvar<=dimu_y && dimu_y<rapbinmax) {
					h1D_Num_pt_bin2_BW->Fill(dimu_pt);
				}
				if (rapbinmin<=dimu_y && dimu_y<rapbinmax) {
					h1D_Num_pt_bin2->Fill(dimu_pt);
				}
			} // end of yn2
		} // end of yn
	} //end of loop

	// (Num/Den) to get acceptance (B : binomial error)
	h2D_Acc_pt_y->Divide(h2D_Num_pt_y,h2D_Den_pt_y,1,1,"B");
	h2D_Acc_pt_y_bin1->Divide(h2D_Num_pt_y_bin1,h2D_Den_pt_y_bin1,1,1,"B");
	h2D_Acc_pt_y_bin2->Divide(h2D_Num_pt_y_bin2,h2D_Den_pt_y_bin2,1,1,"B");

	h1D_Acc_y->Divide(h1D_Num_y,h1D_Den_y,1,1,"B");
	h1D_Acc_pt->Divide(h1D_Num_pt,h1D_Den_pt,1,1,"B");
	h1D_Acc_y_bin1->Divide(h1D_Num_y_bin1,h1D_Den_y_bin1,1,1,"B");
	h1D_Acc_pt_bin2_FW->Divide(h1D_Num_pt_bin2_FW,h1D_Den_pt_bin2_FW,1,1,"B");
	h1D_Acc_pt_bin2_BW->Divide(h1D_Num_pt_bin2_BW,h1D_Den_pt_bin2_BW,1,1,"B");
	h1D_Acc_pt_bin2->Divide(h1D_Num_pt_bin2,h1D_Den_pt_bin2,1,1,"B");
//	TH1D *h1DAcc1Opt1Pt = h2D_Acc_pt_y->ProjectionY();
//	TH1D *h1DAcc1Opt1Y = h2D_Acc_pt_y->ProjectionX();

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Save the data!
/*
	// save01 : as a pdf file
	TCanvas *c1 = new TCanvas("c1","");
	TCanvas *c2 = new TCanvas("c2","c2",1000,600);
	TCanvas *c3 = new TCanvas("c2","c2",600,600);

	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);

	// 2D Acceptance with fine binning	
	c1->cd();
	h2D_Acc_pt_y->GetXaxis()->SetTitle("y");
	h2D_Acc_pt_y->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Acc_pt_y->GetZaxis()->SetRangeUser(0.,1.);
 	h2D_Acc_pt_y->GetXaxis()->SetRangeUser(-2.5,2.5);
	h2D_Acc_pt_y->Draw("colz");
	//double accerror = sqrt ( 1/(den*den*num) + (num*num)/(den*den*den*den*den));
	c1->SaveAs(Form("h2D_Acc_pt_y_%s.pdf",datestring));
	c1->Clear();

	// 2D Acceptance with binning 1 (y dependence)
	c2->cd();
	h2D_Acc_pt_y_bin1->GetXaxis()->SetTitle("y");
	h2D_Acc_pt_y_bin1->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Acc_pt_y_bin1->GetZaxis()->SetRangeUser(0.,1.);
 	h2D_Acc_pt_y_bin1->GetXaxis()->SetRangeUser(-2.5,2.5);
	h2D_Acc_pt_y_bin1->Draw("colz text e");
	c2->SaveAs(Form("h2D_Acc_pt_y_bin1_%s.pdf",datestring));
	c2->Clear();

	// 2D Acceptance with binning 2 (pt dependence)
	c2->cd();
	h2D_Acc_pt_y_bin2->GetXaxis()->SetTitle("y");
	h2D_Acc_pt_y_bin2->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Acc_pt_y_bin2->GetZaxis()->SetRangeUser(0.,1.);
 	h2D_Acc_pt_y_bin2->GetXaxis()->SetRangeUser(-2.5,2.5);
	h2D_Acc_pt_y_bin2->Draw("colz text e");
	c2->SaveAs(Form("h2D_Acc_pt_y_bin2_%s.pdf",datestring));
	c2->Clear();
	// 1D Acceptance with binning 1 (y dependence)
	c3->cd();
	h1D_Acc_y_bin1->GetXaxis()->SetTitle("y_{lab}");
	h1D_Acc_y_bin1->GetYaxis()->SetTitle("Acceptance");
	h1D_Acc_y_bin1->GetXaxus()->SetRangeUser(-2.4,1.47);
	h1D_Acc_y_bin1->GetYaxis()->SetRangeUser(0.,1.);
	h1D_Acc_y_bin1->Draw("pe");
	c3->SaveAs(Form("h2D_Acc_pt_y_bin2_%s.pdf",datestring));
*/

	// entries && bin contests check
	// 01 :: current binning
	cout << " " << endl;
	cout << "********* for y ************" << endl;
	int num_y_totcon = 0;
	int den_y_totcon = 0;
	for (Int_t i=0; i< nYBins1; i++) {
		cout << "from y= " << yBinsArr1[i] << ", to y= "<< yBinsArr1[i+1] <<endl;
		cout << "  entries num = " << h1D_Num_y_bin1->GetBinContent(i+1) << endl;
		cout << "  entries den = " << h1D_Den_y_bin1->GetBinContent(i+1) << endl;
		cout << "  entries num/den = " << h1D_Num_y_bin1->GetBinContent(i+1)/h1D_Den_y_bin1->GetBinContent(i+1) << endl;
		num_y_totcon += h1D_Num_y_bin1->GetBinContent(i+1);
		den_y_totcon += h1D_Den_y_bin1->GetBinContent(i+1);
		cout << "  entries acc = "  <<  h1D_Acc_y_bin1->GetBinContent(i+1) << endl;
		//effErr_yDep = h1D_Acc_y_bin1->GetBinError(i+1);
	}
	cout << " * TOT BinContent* " << endl;
	cout << "  tot contents num = " << num_y_totcon << endl;
	cout << "  tot contents den = " << den_y_totcon << endl;
	cout << "  tot contents num/den = " << (double)num_y_totcon/(double)den_y_totcon << endl;
	cout << " " << endl;
	cout << " " << endl;

	cout << "********* for pT ************" << endl;
	cout << "*** BinContent *** " << endl;	
	int num_pt_FW_totcon = 0;
	int den_pt_FW_totcon = 0;
	int num_pt_BW_totcon = 0;
	int den_pt_BW_totcon = 0;
	int num_pt_totcon = 0;
	int den_pt_totcon = 0;
	for (Int_t i=0; i< nPtBins2; i++) {
		cout << "from pt=" << ptBinsArr2[i] <<", to pt="<< ptBinsArr2[i+1]<<endl;
		cout << "  FW entries num = " << h1D_Num_pt_bin2_FW->GetBinContent(i+1)<< endl; 
		cout << "  FW entries den = " << h1D_Den_pt_bin2_FW->GetBinContent(i+1)<< endl; 
		cout << "  FW entries num/den = " << h1D_Num_pt_bin2_FW->GetBinContent(i+1)/h1D_Den_pt_bin2_FW->GetBinContent(i+1) << endl; 
		cout << "  BW entries num = " << h1D_Num_pt_bin2_BW->GetBinContent(i+1) << endl; 
		cout << "  BW entries den = " << h1D_Den_pt_bin2_BW->GetBinContent(i+1) << endl; 
		cout << "  BW entries num/den = " << h1D_Num_pt_bin2_BW->GetBinContent(i+1)/h1D_Den_pt_bin2_BW->GetBinContent(i+1) << endl; 
		cout << "  integ entries num = " << h1D_Num_pt_bin2->GetBinContent(i+1) << endl; 
		cout << "  integ entries den = " << h1D_Den_pt_bin2->GetBinContent(i+1) << endl; 
		cout << "  integ entries num/den = " << h1D_Num_pt_bin2->GetBinContent(i+1)/h1D_Den_pt_bin2->GetBinContent(i+1) << endl; 
		num_pt_FW_totcon += h1D_Num_pt_bin2_FW->GetBinContent(i+1);
		num_pt_BW_totcon += h1D_Num_pt_bin2_BW->GetBinContent(i+1);
		den_pt_FW_totcon += h1D_Den_pt_bin2_FW->GetBinContent(i+1);
		den_pt_BW_totcon += h1D_Den_pt_bin2_BW->GetBinContent(i+1);
		num_pt_totcon += h1D_Num_pt_bin2->GetBinContent(i+1);
		den_pt_totcon += h1D_Den_pt_bin2->GetBinContent(i+1);
		cout << "  FW entries eff = " << h1D_Acc_pt_bin2_FW->GetBinContent(i+1) << endl;
		cout << "  BW entries eff = " << h1D_Acc_pt_bin2_BW->GetBinContent(i+1) << endl;
		cout << "  integ entries eff = " << h1D_Acc_pt_bin2->GetBinContent(i+1) << endl;

	}
	cout << " * TOT BinContent* " << endl;
	cout << "  FW tot contents num = " << num_pt_FW_totcon << endl;
	cout << "  FW tot contents den = " << den_pt_FW_totcon << endl;
	cout << "  BW tot contents num = " << num_pt_BW_totcon << endl;
	cout << "  BW tot contents den = " << den_pt_BW_totcon << endl;
	cout << "  FW tot contents num/den = " << (double)num_pt_FW_totcon/(double)den_pt_FW_totcon<< endl;
	cout << "  BW tot contents num/den = " << (double)num_pt_BW_totcon/(double)den_pt_BW_totcon<< endl;
	cout << "  FW+BW tot contents num = " << num_pt_FW_totcon + num_pt_BW_totcon<< endl;
	cout << "  FW+BW tot contents den = " << den_pt_FW_totcon + den_pt_BW_totcon<< endl;
	cout << "  FW+BW tot contents num/den = " << ((double)num_pt_FW_totcon+(double)num_pt_BW_totcon) /((double)den_pt_FW_totcon + (double)den_pt_BW_totcon) << endl;
	cout << "  ** tot contents num = " << num_pt_totcon<< endl;
	cout << "  ** tot contents den = " << den_pt_totcon<< endl;
	cout << "  ** tot contents num/den = " << (double)num_pt_totcon/(double)den_pt_totcon << endl;
	cout << " " << endl;


	// --- save02 : save the acceptance value as a ASCII
	// binning 1 y dependence
//	streamsize prec = cout.precision(); // for setprecision (like %.3f in C)
//	std::ofstream outFile_yDep(Form("AccyDep6530_%s.out",datestring),std::ios::app);
	std::ofstream outFile_yDep(Form("Acc_yDep_%s.out",datestring),std::ios::app);
	Double_t acc_yDep=0;
	Double_t accErr_yDep=0;
//	Int_t accBin_yDep = h1D_Acc_y_bin1->GetNbinsX();
	cout << "*** Get Acceptance value (yDep) ***" << endl;
//	cout << "nYBins1 = " << nYBins1 << ", accBin_yDep = "<< accBin_yDep <<" should be same!"<< endl;
	for (Int_t i=0; i< nYBins1; i++) {
		cout << "from y=" << yBinsArr1[i] << ", to y="<< yBinsArr1[i+1] <<endl;
//		cout << "from pt=" << ptBinsArr1[0] <<", to pt="<< ptBinsArr1[1]<<endl;
		acc_yDep = h1D_Acc_y_bin1->GetBinContent(i+1);
		accErr_yDep = h1D_Acc_y_bin1->GetBinError(i+1);
		cout << std::fixed; //// fix flating point notation for setprecision (like %.3f in C)
		cout << "acc= " << setprecision(3) <<acc_yDep << ", accErr = "<< setprecision(3)<<accErr_yDep<<endl;
		outFile_yDep << setprecision(3) <<  acc_yDep << "\t" << setprecision(3) << accErr_yDep << endl;
	}
	outFile_yDep.close();

	// binning 2 pt dependence
//	streamsize prec = cout.precision(); // for setprecision (like %.3f in C)
	std::ofstream outFile_ptDep(Form("Acc_ptDep_%s.out",datestring),std::ios::app);
	Double_t acc_ptDep=0;
	Double_t accErr_ptDep=0;
	cout << "*** Get Acceptance value (ptDep) ***" << endl;
	// FW
	for (Int_t i=0; i< nPtBins2; i++) {
		cout << "from pt=" << ptBinsArr2[i] <<", to pt="<< ptBinsArr2[i+1]<<endl;
		cout << "from y=" << yBinsArr2[0] << ", to y="<< yBinsArr2[1] <<endl;
		acc_ptDep = h1D_Acc_pt_bin2_FW->GetBinContent(i+1);
		accErr_ptDep = h1D_Acc_pt_bin2_FW->GetBinError(i+1);
		cout << std::fixed; //// fix flating point notation for setprecision (like %.3f in C)
		cout << "acc= " << setprecision(3) <<acc_ptDep << ", accErr = "<< setprecision(3)<<accErr_ptDep<<endl;
		outFile_ptDep << setprecision(3) <<  acc_ptDep << "\t" << setprecision(3) << accErr_ptDep << endl;
	}
	// BW
	for (Int_t i=0; i< nPtBins2; i++) {
		cout << "from pt=" << ptBinsArr2[i] <<", to pt="<< ptBinsArr2[i+1]<<endl;
		cout << "from y=" << yBinsArr2[1] << ", to y="<< yBinsArr2[2] <<endl;
		acc_ptDep = h1D_Acc_pt_bin2_BW->GetBinContent(i+1);
		accErr_ptDep = h1D_Acc_pt_bin2_BW->GetBinError(i+1);
		cout << std::fixed; //// fix flating point notation for setprecision (like %.3f in C)
		cout << "acc= " << setprecision(3) <<acc_ptDep << ", accErr = "<< setprecision(3)<<accErr_ptDep<<endl;
		outFile_ptDep << setprecision(3) << acc_ptDep << "\t" << setprecision(3) << accErr_ptDep << endl;
	}
	outFile_ptDep.close();

	std::ofstream outFile_cross(Form("Acc_cross_%s.out",datestring),std::ios::out);
	Double_t acc_cross=0;
	Double_t accErr_cross=0;
	cout << "*** Get Acciciency value (cross) ***" << endl;
	for (Int_t i=0; i<nPtBins2; i++) {
		cout << "from pt=" << ptBinsArr2[i] <<", to pt="<< ptBinsArr2[i+1]<<endl;
		cout << "from y=" << yBinsArr3[0] << ", to y="<< yBinsArr3[1] <<endl;
		acc_cross = h1D_Acc_pt_bin2->GetBinContent(i+1);
		accErr_cross = h1D_Acc_pt_bin2->GetBinError(i+1);
		cout << std::fixed; //// fix flating point notation for setprecision (like %.3f in C)
		cout << " * acc= " << setprecision(3) <<acc_cross << ", accErr = "<< setprecision(3)<<accErr_cross <<endl;
		outFile_cross << setprecision(3) <<  acc_cross << "\t" << setprecision(3) << accErr_cross << endl;
	}
	outFile_cross.close();



	// --- save03 : save as a root file

	TFile *outFile = new TFile(Form("AccAna_%s.root",datestring),"RECREATE");
	std::cout << "datestring: " << datestring << std::endl;
	outFile->cd();

	h2D_NoCut_pt_y->Write();
	h2D_Den_pt_y->Write();
	h2D_Num_pt_y->Write();
	h2D_Acc_pt_y->Write();
	h2D_NoCut_pt_y_bin1->Write();
	h2D_Den_pt_y_bin1->Write();
	h2D_Num_pt_y_bin1->Write();
	h2D_Acc_pt_y_bin1->Write();
	h2D_NoCut_pt_y_bin2->Write();
	h2D_Den_pt_y_bin2->Write();
	h2D_Num_pt_y_bin2->Write();
	h2D_Acc_pt_y_bin2->Write();

	h1D_NoCut_y->Write();
	h1D_NoCut_pt->Write();
	h1D_Den_y->Write();
	h1D_Den_pt->Write();
	h1D_Num_y->Write();
	h1D_Num_pt->Write();
	h1D_Acc_y->Write();
	h1D_Acc_pt->Write();

	h1D_NoCut_y_bin1->Write();
	h1D_Den_y_bin1->Write();
	h1D_Num_y_bin1->Write();
	h1D_Acc_y_bin1->Write();

	h1D_NoCut_pt_bin2_FW->Write();
	h1D_Den_pt_bin2_FW->Write();
	h1D_Num_pt_bin2_FW->Write();
	h1D_Acc_pt_bin2_FW->Write();

	h1D_NoCut_pt_bin2_BW->Write();
	h1D_Den_pt_bin2_BW->Write();
	h1D_Num_pt_bin2_BW->Write();
	h1D_Acc_pt_bin2_BW->Write();

	h1D_NoCut_pt_bin2->Write();
	h1D_Den_pt_bin2->Write();
	h1D_Num_pt_bin2->Write();
	h1D_Acc_pt_bin2->Write();
	
h1D_NoCut_M->Write();
	h1D_Den_M->Write();
	h1D_Num_M->Write();

	outFile->Close();

} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

bool dimuCut(int OniaCode, int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch) {

	int OniaId;
	if (OniaCode==1 || OniaCode==4 || OniaCode==5) {OniaId=553;} 
        else if (OniaCode==2 || OniaCode==3 || OniaCode==6 || OniaCode==7 || OniaCode==8 || OniaCode==9) {OniaId=100553;} 
        else if (OniaCode==11) {OniaId=443;}
        else if (OniaCode==12) {OniaId=100443;}

	if (TMath::Abs(lv_dmom0_Id)==OniaId && lv_dkid0_ch*lv_dkid1_ch<0) {return true;}
	
	else {return false;}
}

bool kineCut(bool isBoosted, double muPt, double muEta, double muP) {

//	if(muEta<-2.4 || muEta>=1.47)
	if(muEta<-2.4 || muEta>=2.4)
	{return false;}
	else if(TMath::Abs(muEta)>=2.2 && TMath::Abs(muEta)<2.4 && muPt<=0.8)
	{return false;}
	else if(TMath::Abs(muEta)>=1.3 && TMath::Abs(muEta)<2.2 && muP<=2.9)
	{return false;}
	else if(TMath::Abs(muEta)<1.3 && muPt<=3.3)
	{return false;}

	else {return true;}

}

bool massCut(double lv_dimu_mass) {
	if(lv_dimu_mass < 2.6 || lv_dimu_mass >= 3.5)
	{return false;}
	else {return true;}
}
