#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <TH1.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TAxis.h>
#include <cmath>
#include <TLorentzRotation.h>

const double shiftvar = -0.47; // conversion constant y=0(collision)==y=-0.47(LAB frame)  

//bool dimuCut(int, int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch);
//bool kineCut(bool, double muPt, double muEta, double muP);
bool dimuCut(int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch);
bool kineCut(double muPt, double muEta, double muP);
bool massCut(double lv_dimu_mass);

void rootAna_acceptance(char *strBinnig = "8rap9pt", bool isPrompt=true){
	
	gROOT->Macro("rootlogon.C+");
	gStyle->SetCanvasDefW(800);

	char* sampleName;
	double minylab =-2.4;
	double maxylab=2.4;
	double minpt=0.0;
	double maxpt=30.0;

	//read-in tree
	TChain * ana1 = new TChain("DiAna");
	if (isPrompt){
		sampleName="PRMC_boosted";
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_JPsiWithFSR_5TeV02_1st_KYOvtx_ntuple_20150126.root");
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_JPsiWithFSR_5TeV02_1st_KYOvtx_ntuple_20150126_p2.root");
		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140210.root"); //from Hyunchul
		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140211.root"); //from Hyunchul
	}
	else {
		sampleName="NPMC_boosted";
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_inclBtoJPsiMuMu_5TeV02_1st_KYOvtx_ntuple_20150127.root");
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_inclBtoJPsiMuMu_5TeV02_1st_KYOvtx_ntuple_20150127_p2.root");
		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_nonPromptJpsi_boosted_wofilters_totevt100M_MuonAna_20140407.root"); //from Hyunchul
	}

	const char* strName = Form("%s_%s",strBinnig,sampleName);
	std::cout << "strName: " << strName << std::endl;

	// muons (pdg +13 or -13 already)
	int dkid0_Id, dkid0_ch, dkid0_st;	
	int dkid1_Id, dkid1_ch, dkid1_st;
	double dkid0_pt, dkid0_eta, dkid0_y, dkid0_px, dkid0_py, dkid0_pz, dkid0_phi, dkid0_mass, dkid0_p;
	double dkid1_pt, dkid1_eta, dkid1_y, dkid1_px, dkid1_py, dkid1_pz, dkid1_phi, dkid1_mass, dkid1_p;
	double dimu_pt, dimu_eta, dimu_y, dimu_px, dimu_py, dimu_pz, dimu_phi, dimu_mass, dimu_p;
	// J/psi
	int dmom0_Id, dmom0_ch, dmom0_st;	
	// dgmom (not used)
	int dgmom0_Id, dgmom0_ch, dgmom0_st;
	double dmom0_pt, dmom0_eta, dmom0_y, dmom0_px, dmom0_py, dmom0_pz, dmom0_phi, dmom0_mass, dmom0_p;
	double dgmom0_pt, dgmom0_eta, dgmom0_y, dgmom0_px, dgmom0_py, dgmom0_pz, dgmom0_phi, dgmom0_mass, dgmom0_p;

	using namespace std;

	// Definition of bin
	// --- pt Bin
	Double_t ptBinsArr[] = {0.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt
	//Double_t ptBinsArr[] = {5.0, 6.5, 10.0, 30.0}; // 6rap3pt
	const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
	cout << "nPtBins=" << nPtBins << endl;

  // --- y Bin //set to 1st run (For 2nd run, will be automatically changed later)
  Double_t yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
  //Double_t yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46}; // 6rap3pt
	const Int_t nYBins = sizeof(yBinsArr)/sizeof(double)-1;
	cout << "nYBins=" << nYBins << endl;

	// for 2nd run
	Double_t yBinsArr2nd[nYBins+1] = {};
	for (Int_t i=0; i<nYBins+1; i++) {
		 yBinsArr2nd[i] = -1*yBinsArr[nYBins-i];
		cout <<"yBinsArr["<<i<<"] = " <<yBinsArr[i]<<endl;
		cout <<"yBinsArr2nd["<<i<<"] = " <<yBinsArr2nd[i]<<endl;
	}
	const Int_t nYBins2nd = sizeof(yBinsArr2nd)/sizeof(double)-1;
	cout << "nYBins2nd=" << nYBins2nd << endl;
	if (nYBins != nYBins2nd) { cout << " *** Error!! nYBins should be equal to nYBins2nd"<<endl; return; };

	//read-in branches
	ana1->SetBranchAddress("dimu_eta",	&dimu_eta);
	ana1->SetBranchAddress("dimu_mass",	&dimu_mass);
	ana1->SetBranchAddress("dimu_p",	&dimu_p);
	ana1->SetBranchAddress("dimu_phi",	&dimu_phi);
	ana1->SetBranchAddress("dimu_pt",	&dimu_pt);
	ana1->SetBranchAddress("dimu_px",	&dimu_px);
	ana1->SetBranchAddress("dimu_py",	&dimu_py);
	ana1->SetBranchAddress("dimu_pz",	&dimu_pz);
	ana1->SetBranchAddress("dimu_y",	&dimu_y);

	ana1->SetBranchAddress("dgmom0_Id",	&dgmom0_Id);
	ana1->SetBranchAddress("dgmom0_ch",	&dgmom0_ch);
	ana1->SetBranchAddress("dgmom0_eta",	&dgmom0_eta);
	ana1->SetBranchAddress("dgmom0_mass",	&dgmom0_mass);
	ana1->SetBranchAddress("dgmom0_p",	&dgmom0_p);
	ana1->SetBranchAddress("dgmom0_phi",	&dgmom0_phi);
	ana1->SetBranchAddress("dgmom0_pt",	&dgmom0_pt);
	ana1->SetBranchAddress("dgmom0_px",	&dgmom0_px);
	ana1->SetBranchAddress("dgmom0_py",	&dgmom0_py);
	ana1->SetBranchAddress("dgmom0_pz",	&dgmom0_pz);
	ana1->SetBranchAddress("dgmom0_st",	&dgmom0_st);
	ana1->SetBranchAddress("dgmom0_y",	&dgmom0_y);

	ana1->SetBranchAddress("dmom0_Id",	&dmom0_Id);
	ana1->SetBranchAddress("dmom0_ch",	&dmom0_ch);
	ana1->SetBranchAddress("dmom0_eta",	&dmom0_eta);
	ana1->SetBranchAddress("dmom0_mass",	&dmom0_mass);
	ana1->SetBranchAddress("dmom0_p",	&dmom0_p);
	ana1->SetBranchAddress("dmom0_phi",	&dmom0_phi);
	ana1->SetBranchAddress("dmom0_pt",	&dmom0_pt);
	ana1->SetBranchAddress("dmom0_px",	&dmom0_px);
	ana1->SetBranchAddress("dmom0_py",	&dmom0_py);
	ana1->SetBranchAddress("dmom0_pz",	&dmom0_pz);
	ana1->SetBranchAddress("dmom0_st",	&dmom0_st);
	ana1->SetBranchAddress("dmom0_y",	&dmom0_y);

	ana1->SetBranchAddress("dkid0_Id",	&dkid0_Id);
	ana1->SetBranchAddress("dkid0_ch",	&dkid0_ch);
	ana1->SetBranchAddress("dkid0_eta",	&dkid0_eta);
	ana1->SetBranchAddress("dkid0_mass",	&dkid0_mass);
	ana1->SetBranchAddress("dkid0_p",	&dkid0_p);
	ana1->SetBranchAddress("dkid0_phi",	&dkid0_phi);
	ana1->SetBranchAddress("dkid0_pt",	&dkid0_pt);
	ana1->SetBranchAddress("dkid0_px",	&dkid0_px);
	ana1->SetBranchAddress("dkid0_py",	&dkid0_py);
	ana1->SetBranchAddress("dkid0_pz",	&dkid0_pz);
	ana1->SetBranchAddress("dkid0_st",	&dkid0_st);
	ana1->SetBranchAddress("dkid0_y",	&dkid0_y);

	ana1->SetBranchAddress("dkid1_Id",	&dkid1_Id);
	ana1->SetBranchAddress("dkid1_ch",	&dkid1_ch);
	ana1->SetBranchAddress("dkid1_eta",	&dkid1_eta);
	ana1->SetBranchAddress("dkid1_mass",	&dkid1_mass);
	ana1->SetBranchAddress("dkid1_p",	&dkid1_p);
	ana1->SetBranchAddress("dkid1_phi",	&dkid1_phi);
	ana1->SetBranchAddress("dkid1_pt",	&dkid1_pt);
	ana1->SetBranchAddress("dkid1_px",	&dkid1_px);
	ana1->SetBranchAddress("dkid1_py",	&dkid1_py);
	ana1->SetBranchAddress("dkid1_pz",	&dkid1_pz);
	ana1->SetBranchAddress("dkid1_st",	&dkid1_st);
	ana1->SetBranchAddress("dkid1_y",	&dkid1_y);

	cout << "Entries of tree : " << ana1->GetEntries() << endl;
	std::cout << "strName: " << strName << std::endl;

	////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	// define 2D hist
	// with defined binning - double 1
	TH2D *h2D_NoCut_pt_y_Pbp = new TH2D("h2D_NoCut_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Den_pt_y_Pbp = new TH2D("h2D_Den_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y_Pbp = new TH2D("h2D_Num_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_pt_y_Pbp = new TH2D("h2D_Acc_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	TH2D *h2D_NoCut_ptup_pt_y = new TH2D("h2D_NoCut_ptup_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Den_ptup_pt_y = new TH2D("h2D_Den_ptup_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_ptup_pt_y = new TH2D("h2D_Num_ptup_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_ptup_pt_y_Pbp = new TH2D("h2D_Acc_ptup_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	TH2D *h2D_NoCut_ptdn_pt_y = new TH2D("h2D_NoCut_ptdn_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Den_ptdn_pt_y = new TH2D("h2D_Den_ptdn_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_ptdn_pt_y = new TH2D("h2D_Num_ptdn_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_ptdn_pt_y_Pbp = new TH2D("h2D_Acc_ptdn_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	TH2D *h2D_NoCut_ycup_pt_y = new TH2D("h2D_NoCut_ycup_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Den_ycup_pt_y = new TH2D("h2D_Den_ycup_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_ycup_pt_y = new TH2D("h2D_Num_ycup_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_ycup_pt_y_Pbp = new TH2D("h2D_Acc_ycup_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	TH2D *h2D_NoCut_ycdn_pt_y = new TH2D("h2D_NoCut_ycdn_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Den_ycdn_pt_y = new TH2D("h2D_Den_ycdn_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_ycdn_pt_y = new TH2D("h2D_Num_ycdn_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_ycdn_pt_y_Pbp = new TH2D("h2D_Acc_ycdn_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	std::cout << "TH1, TH2 definition, strName: " << strName << std::endl;

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	//2D hist
	h2D_NoCut_pt_y_Pbp->Sumw2();
	h2D_Den_pt_y_Pbp->Sumw2();
	h2D_Num_pt_y_Pbp->Sumw2();
	h2D_Acc_pt_y_Pbp->Sumw2();
	h2D_NoCut_ptup_pt_y->Sumw2();
	h2D_Den_ptup_pt_y->Sumw2();
	h2D_Num_ptup_pt_y->Sumw2();
	h2D_Acc_ptup_pt_y_Pbp->Sumw2();
	h2D_NoCut_ptdn_pt_y->Sumw2();
	h2D_Den_ptdn_pt_y->Sumw2();
	h2D_Num_ptdn_pt_y->Sumw2();
	h2D_Acc_ptdn_pt_y_Pbp->Sumw2();
	h2D_NoCut_ycup_pt_y->Sumw2();
	h2D_Den_ycup_pt_y->Sumw2();
	h2D_Num_ycup_pt_y->Sumw2();
	h2D_Acc_ycup_pt_y_Pbp->Sumw2();
	h2D_NoCut_ycdn_pt_y->Sumw2();
	h2D_Den_ycdn_pt_y->Sumw2();
	h2D_Num_ycdn_pt_y->Sumw2();
	h2D_Acc_ycdn_pt_y_Pbp->Sumw2();

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	//// Loop Start!
	
	for(int i=0; i<ana1->GetEntries(); i++){
		if(i%100000==0) cout << ">>>>> EVENT " << i << " / " << ana1->GetEntries() <<  endl;
		ana1->GetEntry(i);
		//weighting factor for sys.
	  Double_t wf_ptup;
	  Double_t wf_ptdn;
	  Double_t wf_ycup;
	  Double_t wf_ycdn;
		if (isPrompt) {
			 wf_ptup=0.012*dimu_pt+0.82;
			 wf_ptdn=-0.012*dimu_pt+1.18;
			 wf_ycup=0.145*TMath::Abs(dimu_y-shiftvar)+0.86;
			 wf_ycdn=-0.145*TMath::Abs(dimu_y-shiftvar)+1.14;
		} else {
			 wf_ptup=0.012*dimu_pt+0.82;
			 wf_ptdn=-0.012*dimu_pt+1.18;
			 wf_ycup=0.145*TMath::Abs(dimu_y-shiftvar)+0.86;
			 wf_ycdn=-0.145*TMath::Abs(dimu_y-shiftvar)+1.14;
		}
		////// --- cut01 : no cut
		h2D_NoCut_pt_y_Pbp->Fill(dimu_y,dimu_pt);
		h2D_NoCut_ptup_pt_y->Fill(dimu_y,dimu_pt,wf_ptup);
		h2D_NoCut_ptdn_pt_y->Fill(dimu_y,dimu_pt,wf_ptdn);
		h2D_NoCut_ycup_pt_y->Fill(dimu_y,dimu_pt,wf_ycup);
		h2D_NoCut_ycdn_pt_y->Fill(dimu_y,dimu_pt,wf_ycdn);

		if (!dimuCut(dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch)) continue;

		////// --- cut02 : for denominator
		bool yn = false;
		if (dimuCut(dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) {yn = true;}
		if ( yn == true  ) {
			h2D_Den_pt_y_Pbp->Fill(dimu_y,dimu_pt);
			h2D_Den_ptup_pt_y->Fill(dimu_y,dimu_pt,wf_ptup);
			h2D_Den_ptdn_pt_y->Fill(dimu_y,dimu_pt,wf_ptdn);
			h2D_Den_ycup_pt_y->Fill(dimu_y,dimu_pt,wf_ycup);
			h2D_Den_ycdn_pt_y->Fill(dimu_y,dimu_pt,wf_ycdn);

			////// --- cut03 : for numerator
			bool yn2 = false;
			if (massCut(dimu_mass) && dimuCut(dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch) && kineCut(dkid0_pt,dkid0_eta,dkid0_p) && kineCut(dkid1_pt,dkid1_eta,dkid1_p) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) {yn2 = true;}
			if (yn2 == true){
				h2D_Num_pt_y_Pbp->Fill(dimu_y,dimu_pt);
				h2D_Num_ptup_pt_y->Fill(dimu_y,dimu_pt,wf_ptup);
				h2D_Num_ptdn_pt_y->Fill(dimu_y,dimu_pt,wf_ptdn);
				h2D_Num_ycup_pt_y->Fill(dimu_y,dimu_pt,wf_ycup);
				h2D_Num_ycdn_pt_y->Fill(dimu_y,dimu_pt,wf_ycdn);

			} // end of yn2
		} // end of yn
	} //end of loop

	// (Num/Den) to get acceptance (B : binomial error)
	h2D_Acc_pt_y_Pbp->Divide(h2D_Num_pt_y_Pbp,h2D_Den_pt_y_Pbp,1,1,"B");
	h2D_Acc_ptup_pt_y_Pbp->Divide(h2D_Num_ptup_pt_y,h2D_Den_ptup_pt_y,1,1,"B");
	h2D_Acc_ptdn_pt_y_Pbp->Divide(h2D_Num_ptdn_pt_y,h2D_Den_ptdn_pt_y,1,1,"B");
	h2D_Acc_ycup_pt_y_Pbp->Divide(h2D_Num_ycup_pt_y,h2D_Den_ycup_pt_y,1,1,"B");
	h2D_Acc_ycdn_pt_y_Pbp->Divide(h2D_Num_ycdn_pt_y,h2D_Den_ycdn_pt_y,1,1,"B");


	////////////////////////////////////////////////////////////////////////////
	// rapidity change, for 2nd run hist.
	TH2D *h2D_Den_pt_y_pPb = new TH2D("h2D_Den_pt_y_pPb","",nYBins2nd,yBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y_pPb = new TH2D("h2D_Num_pt_y_pPb","",nYBins2nd,yBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_pt_y_pPb = new TH2D("h2D_Acc_pt_y_pPb","",nYBins2nd,yBinsArr2nd,nPtBins,ptBinsArr);
	h2D_Den_pt_y_pPb->Sumw2();
	h2D_Num_pt_y_pPb->Sumw2();
	h2D_Acc_pt_y_pPb->Sumw2();

	int tmpbinDenPbp, tmpbinDenpPb; 
	int tmpbinNumPbp, tmpbinNumpPb; 
	int tmpbinAccPbp, tmpbinAccpPb; 
	float tmpDenVal, tmpDenErr;
	float tmpNumVal, tmpNumErr;
	float tmpAccVal, tmpAccErr;
	for (Int_t iy=0; iy<nYBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++) {
			tmpbinDenPbp = h2D_Den_pt_y_Pbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmpDenVal = h2D_Den_pt_y_Pbp->GetBinContent(tmpbinDenPbp);
			tmpDenErr = h2D_Den_pt_y_Pbp->GetBinError(tmpbinDenPbp);
			tmpbinDenpPb = h2D_Den_pt_y_pPb->FindBin((yBinsArr2nd[nYBins-iy-1]+yBinsArr2nd[nYBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Den_pt_y_pPb->SetBinContent(tmpbinDenpPb,tmpDenVal);
			h2D_Den_pt_y_pPb->SetBinError(tmpbinDenpPb,tmpDenErr);
			
			tmpbinNumPbp = h2D_Num_pt_y_Pbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmpNumVal = h2D_Num_pt_y_Pbp->GetBinContent(tmpbinNumPbp);
			tmpNumErr = h2D_Num_pt_y_Pbp->GetBinError(tmpbinNumPbp);
			tmpbinNumpPb = h2D_Num_pt_y_pPb->FindBin((yBinsArr2nd[nYBins-iy-1]+yBinsArr2nd[nYBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Num_pt_y_pPb->SetBinContent(tmpbinNumpPb,tmpNumVal);
			h2D_Num_pt_y_pPb->SetBinError(tmpbinNumpPb,tmpNumErr);
			
			tmpbinAccPbp = h2D_Acc_pt_y_Pbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmpAccVal = h2D_Acc_pt_y_Pbp->GetBinContent(tmpbinAccPbp);
			tmpAccErr = h2D_Acc_pt_y_Pbp->GetBinError(tmpbinAccPbp);
			tmpbinAccpPb = h2D_Acc_pt_y_pPb->FindBin((yBinsArr2nd[nYBins-iy-1]+yBinsArr2nd[nYBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Acc_pt_y_pPb->SetBinContent(tmpbinAccpPb,tmpAccVal);
			h2D_Acc_pt_y_pPb->SetBinError(tmpbinAccpPb,tmpAccErr);
			
			/*
			cout << "*** "<<iy<<"th rap, "<<ipt<<"th pt ***" <<endl;
			cout << "tmpbinAccPbp = " <<tmpbinAccPbp << ", tmpbinAccpPb = " << tmpbinAccpPb << endl;
			cout << "tmpAccVal = " <<tmpAccVal << ", tmpAccErr = " << tmpAccErr << endl;
			cout << "yBinsArr :" <<(yBinsArr[iy]+yBinsArr[iy+1])/2 << endl;
			cout << "      from " <<yBinsArr[iy]<<" to "<<yBinsArr[iy+1] << endl;
			cout << "yBinsArr2nd : " << (yBinsArr2nd[nYBins-iy-1]+yBinsArr2nd[nYBins-iy])/2 << endl;
			cout << "      from " << yBinsArr2nd[nYBins-iy-1] << " to " << yBinsArr2nd[nYBins-iy] << endl;	
			cout << "" << endl;
			*/
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// Save the data!

	TFile *outFile = new TFile(Form("AccAna_%s.root",strName),"RECREATE");
	std::cout << "strName: " << strName << std::endl;
	outFile->cd();

	//h2D_NoCut_pt_y_Pbp->Write();
	h2D_Den_pt_y_Pbp->Write();
	h2D_Num_pt_y_Pbp->Write();
	h2D_Acc_pt_y_Pbp->Write();
	h2D_Den_pt_y_pPb->Write();
	h2D_Num_pt_y_pPb->Write();
	h2D_Acc_pt_y_pPb->Write();
	/*
	h2D_NoCut_ptup_pt_y->Write();
	h2D_Den_ptup_pt_y->Write();
	h2D_Num_ptup_pt_y->Write();
	h2D_Acc_ptup_pt_y_Pbp->Write();
	h2D_NoCut_ptdn_pt_y->Write();
	h2D_Den_ptdn_pt_y->Write();
	h2D_Num_ptdn_pt_y->Write();
	h2D_Acc_ptdn_pt_y_Pbp->Write();
	h2D_NoCut_ycup_pt_y->Write();
	h2D_Den_ycup_pt_y->Write();
	h2D_Num_ycup_pt_y->Write();
	h2D_Acc_ycup_pt_y_Pbp->Write();
	h2D_NoCut_ycdn_pt_y->Write();
	h2D_Den_ycdn_pt_y->Write();
	h2D_Num_ycdn_pt_y->Write();
	h2D_Acc_ycdn_pt_y_Pbp->Write();
	*/
	outFile->Close();

} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

bool dimuCut(int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch) {
	return (TMath::Abs(lv_dmom0_Id)==443 && lv_dkid0_ch*lv_dkid1_ch<0);
}

bool kineCut(double muPt, double muEta, double muP) {
		return ( TMath::Abs(muEta) < 2.4 &&
						((TMath::Abs(muEta) < 1.3 && muPt >=3.3) ||
						 (1.3 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.2 && muP >=2.9) ||
						 (2.2 <= TMath::Abs(muEta) && muPt >= 0.8)));
}

bool massCut(double lv_dimu_mass) {
	return ( 2.6 <= lv_dimu_mass && lv_dimu_mass < 3.5 );
}

