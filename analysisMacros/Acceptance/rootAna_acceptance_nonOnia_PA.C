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
#include <TLorentzRotation.h>
#include <TROOT.h>
#include <TAxis.h>
#include <cmath>

bool dimuCut(int lv_dmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch);
bool kineCut(double muPt, double muEta, double muP);
bool massCut(double lv_dimu_mass);

//// select m rapidity and n pt binning : (MN) = (89, 83, 63, 62)
void rootAna_acceptance_nonOnia_PA(int mrapnpt=89, bool isPrompt=false){
	
	using namespace std;
	char* sampleName;
	char* strBinning;
	if (mrapnpt==89) strBinning = "8rap9pt";
	else if (mrapnpt==83) strBinning = "8rap3pt";
	else if (mrapnpt==63) strBinning = "6rap3pt";
	else if (mrapnpt==62) strBinning = "6rap2pt";
	else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return; }
	double minylab =-2.4, maxylab=2.4;
	double minpt=0.0, maxpt=30.0;

	TChain * ana1 = new TChain("DiAna");
	//// acceptance tree from hyunchul
	if (isPrompt){
		sampleName="PRMC_boosted";
		//// ---------- in KUNPL
		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSamplePrivate/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140210.root"); 
		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSamplePrivate/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140211.root"); 
	}
	else {
		sampleName="NPMC_boosted";
		//// ---------- in KUNPL
		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSamplePrivate/pythia6_nonPromptJpsi_boosted_wofilters_totevt100M_MuonAna_20140407.root"); 
	}
	const char* strName = Form("%s_%s",strBinning,sampleName);
	std::cout << "strName: " << strName << std::endl;

	//// 1) muons (pdg +13/-13 and status==1 already)
	int dkid0_ch;	
	int dkid1_ch;
	double dkid0_pt, dkid0_eta, dkid0_p;
	double dkid1_pt, dkid1_eta, dkid1_p;
	//// 2) J/psi (dmom0) and dimuons
	int dmom0_Id;	
	double dimu_pt, dimu_y, dimu_mass;

	//// Definition of the binning
	int nRapBins, nPtBins;
	if (mrapnpt==89) { nRapBins = 8; nPtBins = 9; }
	else if (mrapnpt==83) { nRapBins = 8; nPtBins = 3; }
	else if (mrapnpt==63) { nRapBins = 6; nPtBins = 3;}
	else { nRapBins = 6; nPtBins = 2;}
	const int nRapBinsTmp = nRapBins +1;
	const int nPtBinsTmp = nPtBins +1;
	
	//// pt bins
	Double_t ptBinsArr[nPtBinsTmp];
	Double_t ptBinsArr9pt[10] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0};
	Double_t ptBinsArr3pt[4] = {5.0, 6.5, 10.0, 30.0};
	Double_t ptBinsArr2pt[3] = {5.0, 6.5, 30.0};
	for (int ipt=0; ipt<nPtBinsTmp; ipt++){
		if (mrapnpt==89) {ptBinsArr[ipt]=ptBinsArr9pt[ipt];}
		else if (mrapnpt==83 || mrapnpt==63) {ptBinsArr[ipt]=ptBinsArr3pt[ipt];}
		else {ptBinsArr[ipt]=ptBinsArr2pt[ipt];}
	}

  //// y bins : set to 1st run (For 2nd run, will be automatically changed later)
  Double_t rapBinsArr[nRapBinsTmp];
  Double_t rapBinsArr8rap[9] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4};
  Double_t rapBinsArr6rap[7] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46};
	for (int iy=0; iy<nRapBinsTmp; iy++){
		if (mrapnpt==89 || mrapnpt==83) {rapBinsArr[iy]=rapBinsArr8rap[iy];}
		else {rapBinsArr[iy]=rapBinsArr6rap[iy];}
	}

	// for 2nd run
	Double_t rapBinsArr2nd[nRapBins+1];
	for (Int_t i=0; i<nRapBins+1; i++) {
		 rapBinsArr2nd[i] = -1*rapBinsArr[nRapBins-i];
		cout <<"rapBinsArr["<<i<<"] = " <<rapBinsArr[i]<<endl;
		cout <<"rapBinsArr2nd["<<i<<"] = " <<rapBinsArr2nd[i]<<endl;
	}
	const Int_t nRapBins2nd = sizeof(rapBinsArr2nd)/sizeof(double)-1;
	if (nRapBins != nRapBins2nd) { cout << " *** Error!! nRapBins should be equal to nRapBins2nd"<<endl; return; };

	//read-in branches
	ana1->SetBranchAddress("dmom0_Id",	&dmom0_Id);
	ana1->SetBranchAddress("dimu_mass",	&dimu_mass);
	ana1->SetBranchAddress("dimu_pt",	&dimu_pt);
	ana1->SetBranchAddress("dimu_y",	&dimu_y);

	ana1->SetBranchAddress("dkid0_ch",	&dkid0_ch);
	ana1->SetBranchAddress("dkid0_eta",	&dkid0_eta);
	ana1->SetBranchAddress("dkid0_p",	&dkid0_p);
	ana1->SetBranchAddress("dkid0_pt",	&dkid0_pt);

	ana1->SetBranchAddress("dkid1_ch",	&dkid1_ch);
	ana1->SetBranchAddress("dkid1_eta",	&dkid1_eta);
	ana1->SetBranchAddress("dkid1_p",	&dkid1_p);
	ana1->SetBranchAddress("dkid1_pt",	&dkid1_pt);

	cout << "Entries of tree : " << ana1->GetEntries() << endl;

	// define 2D hist
	TH2D *h2D_Den_pt_y_Pbp = new TH2D("h2D_Den_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y_Pbp = new TH2D("h2D_Num_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_pt_y_Pbp = new TH2D("h2D_Acc_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	h2D_Den_pt_y_Pbp->Sumw2();
	h2D_Num_pt_y_Pbp->Sumw2();
	h2D_Acc_pt_y_Pbp->Sumw2();
	//fine bins for plots in AN (for 8rap9pt only)
	TH2D *h2D_Den_pt_y_Pbp_fine = new TH2D("h2D_Den_pt_y_Pbp_fine","",100,-2.5,2.5,100,0,30);
	TH2D *h2D_Num_pt_y_Pbp_fine = new TH2D("h2D_Num_pt_y_Pbp_fine","",100,-2.5,2.5,100,0,30);
	TH2D *h2D_Acc_pt_y_Pbp_fine = new TH2D("h2D_Acc_pt_y_Pbp_fine","",100,-2.5,2.5,100,0,30);
	h2D_Den_pt_y_Pbp_fine->Sumw2();
	h2D_Num_pt_y_Pbp_fine->Sumw2();
	h2D_Acc_pt_y_Pbp_fine->Sumw2();

	////////////////////////////////////////////////////////////////////////////////////////////////////	
	//// event loop Start!
	for(int i=0; i<ana1->GetEntries(); i++){
		if(i%100000==0) cout << ">>>>> EVENT " << i << " / " << ana1->GetEntries() <<  endl;
		ana1->GetEntry(i);
		//// for denominator
		if (dimuCut(dmom0_Id,dkid0_ch,dkid1_ch) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) { 
			h2D_Den_pt_y_Pbp->Fill(dimu_y,dimu_pt);
			if (mrapnpt==89) h2D_Den_pt_y_Pbp_fine->Fill(dimu_y,dimu_pt);
			//// for numerator 
			if (massCut(dimu_mass) && kineCut(dkid0_pt,dkid0_eta,dkid0_p) && kineCut(dkid1_pt,dkid1_eta,dkid1_p)) { 
				h2D_Num_pt_y_Pbp->Fill(dimu_y,dimu_pt);
				if (mrapnpt==89) h2D_Num_pt_y_Pbp_fine->Fill(dimu_y,dimu_pt);
			}
		}
	}//// end of event loop

	//// (Num/Den) to get acceptance (B : binomial error)
	h2D_Acc_pt_y_Pbp->Divide(h2D_Num_pt_y_Pbp,h2D_Den_pt_y_Pbp,1,1,"B");
	if (mrapnpt==89) h2D_Acc_pt_y_Pbp_fine->Divide(h2D_Num_pt_y_Pbp_fine,h2D_Den_pt_y_Pbp_fine,1,1,"B");

	////////////////////////////////////////////////////////////////////////////////////////////////////	
	//// rapidity reverse, for 2nd run hist.
	TH2D *h2D_Den_pt_y_pPb = new TH2D("h2D_Den_pt_y_pPb","",nRapBins2nd,rapBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y_pPb = new TH2D("h2D_Num_pt_y_pPb","",nRapBins2nd,rapBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_pt_y_pPb = new TH2D("h2D_Acc_pt_y_pPb","",nRapBins2nd,rapBinsArr2nd,nPtBins,ptBinsArr);
	h2D_Den_pt_y_pPb->Sumw2();
	h2D_Num_pt_y_pPb->Sumw2();
	h2D_Acc_pt_y_pPb->Sumw2();

	int tmpbinDenPbp, tmpbinNumPbp, tmpbinAccPbp;
	int tmpbinDenpPb, tmpbinNumpPb, tmpbinAccpPb;
	float tmpDenVal, tmpDenErr, tmpNumVal, tmpNumErr, tmpAccVal, tmpAccErr;
	
	for (Int_t iy=0; iy<nRapBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++) {
			tmpbinDenPbp = h2D_Den_pt_y_Pbp->FindBin((rapBinsArr[iy]+rapBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmpDenVal = h2D_Den_pt_y_Pbp->GetBinContent(tmpbinDenPbp);
			tmpDenErr = h2D_Den_pt_y_Pbp->GetBinError(tmpbinDenPbp);
			tmpbinDenpPb = h2D_Den_pt_y_pPb->FindBin((rapBinsArr2nd[nRapBins-iy-1]+rapBinsArr2nd[nRapBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Den_pt_y_pPb->SetBinContent(tmpbinDenpPb,tmpDenVal);
			h2D_Den_pt_y_pPb->SetBinError(tmpbinDenpPb,tmpDenErr);
			
			tmpbinNumPbp = h2D_Num_pt_y_Pbp->FindBin((rapBinsArr[iy]+rapBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmpNumVal = h2D_Num_pt_y_Pbp->GetBinContent(tmpbinNumPbp);
			tmpNumErr = h2D_Num_pt_y_Pbp->GetBinError(tmpbinNumPbp);
			tmpbinNumpPb = h2D_Num_pt_y_pPb->FindBin((rapBinsArr2nd[nRapBins-iy-1]+rapBinsArr2nd[nRapBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Num_pt_y_pPb->SetBinContent(tmpbinNumpPb,tmpNumVal);
			h2D_Num_pt_y_pPb->SetBinError(tmpbinNumpPb,tmpNumErr);
			
			tmpbinAccPbp = h2D_Acc_pt_y_Pbp->FindBin((rapBinsArr[iy]+rapBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmpAccVal = h2D_Acc_pt_y_Pbp->GetBinContent(tmpbinAccPbp);
			tmpAccErr = h2D_Acc_pt_y_Pbp->GetBinError(tmpbinAccPbp);
			tmpbinAccpPb = h2D_Acc_pt_y_pPb->FindBin((rapBinsArr2nd[nRapBins-iy-1]+rapBinsArr2nd[nRapBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Acc_pt_y_pPb->SetBinContent(tmpbinAccpPb,tmpAccVal);
			h2D_Acc_pt_y_pPb->SetBinError(tmpbinAccpPb,tmpAccErr);
			/*
			cout << "*** "<<iy<<"th rap, "<<ipt<<"th pt ***" <<endl;
			cout << "tmpbinAccPbp = " <<tmpbinAccPbp << ", tmpbinAccpPb = " << tmpbinAccpPb << endl;
			cout << "tmpAccVal = " <<tmpAccVal << ", tmpAccErr = " << tmpAccErr << endl;
			cout << "rapBinsArr :" <<(rapBinsArr[iy]+rapBinsArr[iy+1])/2 << endl;
			cout << "      from " <<rapBinsArr[iy]<<" to "<<rapBinsArr[iy+1] << endl;
			cout << "rapBinsArr2nd : " << (rapBinsArr2nd[nRapBins-iy-1]+rapBinsArr2nd[nRapBins-iy])/2 << endl;
			cout << "      from " << rapBinsArr2nd[nRapBins-iy-1] << " to " << rapBinsArr2nd[nRapBins-iy] << endl;	
			cout << "" << endl;
			*/
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////	
	//// Save the data as an root file
	TFile *outFile = new TFile(Form("AccAna_%s.root",strName),"RECREATE");
	std::cout << "strName: " << strName << std::endl;
	outFile->cd();
	h2D_Den_pt_y_Pbp->Write();
	h2D_Num_pt_y_Pbp->Write();
	h2D_Acc_pt_y_Pbp->Write();
	h2D_Den_pt_y_pPb->Write();
	h2D_Num_pt_y_pPb->Write();
	h2D_Acc_pt_y_pPb->Write();
	if (mrapnpt==89) {
		h2D_Den_pt_y_Pbp_fine->Write();
		h2D_Num_pt_y_Pbp_fine->Write();
		h2D_Acc_pt_y_Pbp_fine->Write();
	}
	outFile->Close();

} // end of main func

////////////////////////////////////////////////////////////////////////////////////////////////////	
//// sub-routines function 

bool dimuCut(int lv_dmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch) {
	return (TMath::Abs(lv_dmom0_Id)==443 && lv_dkid0_ch*lv_dkid1_ch<0);
}

bool kineCut(double muPt, double muEta, double muP) {
/*
	//// ---------- old cut (2015PAS)		
		return ( TMath::Abs(muEta) < 2.4 &&
						((TMath::Abs(muEta) < 1.3 && muPt >=3.3) ||
						 (1.3 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.2 && muP >=2.9) ||
						 (2.2 <= TMath::Abs(muEta) && muPt >= 0.8)));
*/
	//// ---------- new cut (cutG)
	return ( TMath::Abs(muEta) < 2.4 &&
				((TMath::Abs(muEta) < 1.2 && muPt >=3.3) ||
				//(1.2 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.1 && muPt >= -(1.0/0.9)*TMath::Abs(muEta)+(1.2*(1.0/0.9)+2.6)) ||
      	(1.2<=fabs(muEta) && fabs(muEta)<2.1 && muPt>=3.93-1.11*fabs(muEta)) ||
				(2.1 <= TMath::Abs(muEta) && muPt >= 1.3)));

}

bool massCut(double lv_dimu_mass) {
	return ( 2.6 <= lv_dimu_mass && lv_dimu_mass < 3.5 );
}

