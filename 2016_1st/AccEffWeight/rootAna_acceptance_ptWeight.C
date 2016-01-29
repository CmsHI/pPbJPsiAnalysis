#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <TH1.h>
#include <TF1.h>
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

// MrapNpt : (89, 83, 63, 62)
void rootAna_acceptance_ptWeight(int mrapnpt=83, bool isPrompt=false, bool isStiff=true){
	
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
	if (isPrompt){
		sampleName="PRMC_boosted";
		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140210.root"); //from Hyunchul
		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140211.root"); //from Hyunchul
	}
	else {
		sampleName="NPMC_boosted";
		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_nonPromptJpsi_boosted_wofilters_totevt100M_MuonAna_20140407.root"); //from Hyunchul
	}
	const char* strName = Form("%s_%s",strBinning,sampleName);
	std::cout << "strName: " << strName << std::endl;

	// muons (pdg +13/-13 and status==1 already)
	int dkid0_ch;	
	int dkid1_ch;
	double dkid0_pt, dkid0_eta, dkid0_p;
	double dkid1_pt, dkid1_eta, dkid1_p;
	// J/psi(dmom0) and dimuons
	int dmom0_Id;	
	double dimu_pt, dimu_y, dimu_mass;

	//// Definition of bin
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

	//// define 2D hist
	// not-weighted
	TH2D *h2D_Den_noPtWeight_pt_y_Pbp = new TH2D("h2D_Den_noPtWeight_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_noPtWeight_pt_y_Pbp = new TH2D("h2D_Num_noPtWeight_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_noPtWeight_pt_y_Pbp = new TH2D("h2D_Acc_noPtWeight_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	h2D_Den_noPtWeight_pt_y_Pbp->Sumw2();
	h2D_Num_noPtWeight_pt_y_Pbp->Sumw2();
	h2D_Acc_noPtWeight_pt_y_Pbp->Sumw2();
	// weighted
	TH2D *h2D_Den_pt_y_Pbp = new TH2D("h2D_Den_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y_Pbp = new TH2D("h2D_Num_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_pt_y_Pbp = new TH2D("h2D_Acc_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	h2D_Den_pt_y_Pbp->Sumw2();
	h2D_Num_pt_y_Pbp->Sumw2();
	h2D_Acc_pt_y_Pbp->Sumw2();
	//fine bins for plots in AN
	TH2D *h2D_Den_pt_y_Pbp_fine = new TH2D("h2D_Den_pt_y_Pbp_fine","",100,-2.5,2.5,100,0,30);
	TH2D *h2D_Num_pt_y_Pbp_fine = new TH2D("h2D_Num_pt_y_Pbp_fine","",100,-2.5,2.5,100,0,30);
	TH2D *h2D_Acc_pt_y_Pbp_fine = new TH2D("h2D_Acc_pt_y_Pbp_fine","",100,-2.5,2.5,100,0,30);
	h2D_Den_pt_y_Pbp_fine->Sumw2();
	h2D_Num_pt_y_Pbp_fine->Sumw2();
	h2D_Acc_pt_y_Pbp_fine->Sumw2();

	////////////////////////////////////////////////////////////////////////////////////////////////
	//////// Ratio input file
	string strPr = "PR"; // always use Prompt Pbp!! (for non-prompt also)
	//TFile* inFile = new TFile(Form("fitRatio_%sPbp.root",strPr.c_str()));
	TFile* inFile = new TFile(Form("fitRatio_%sPbp_isStiff%d.root",strPr.c_str(),(int)isStiff));
	TF1* Func[nRapBins];
	for(int iy=0;iy<nRapBins;iy++){
		Func[iy]=(TF1*) inFile->Get(Form("f_ptRatio_%sPbp_%d",strPr.c_str(),iy));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//// Loop Start!
	for(int i=0; i<ana1->GetEntries(); i++){
		if(i%100000==0) cout << ">>>>> EVENT " << i << " / " << ana1->GetEntries() <<  endl;
		ana1->GetEntry(i);
		
		////weighting factor for Acc. Sys. by Yeonju
		double recoWeight = 1.0;
		int yflag = 99;
		double ptThr = 0.0;
		string st_AccRange = "";
		// Funcy[yflag] is ordered from FW to BW in y_CM!! (kyo)   
		if (nRapBins == 8) {
			if(-2.40<= dimu_y && dimu_y<-1.97) {  yflag = 0; ptThr=2.0; }
			else if(-1.97<= dimu_y && dimu_y<-1.37) {  yflag = 1;  ptThr=3.0; }
			else if(-1.37<= dimu_y && dimu_y<-0.47) {  yflag = 2;  ptThr=6.5; }
			else if(-0.47<= dimu_y && dimu_y<0.43) {  yflag = 3;  ptThr=6.5; }
			else if(0.43<= dimu_y && dimu_y<1.03) {  yflag = 4;  ptThr=6.5; }
			else if(1.03<= dimu_y && dimu_y<1.46) {  yflag = 5;  ptThr=5.0; }
			else if(1.46<= dimu_y && dimu_y<1.93) {  yflag = 6;  ptThr=3.0; }
			else if(1.93<= dimu_y && dimu_y<2.40) {  yflag = 7;  ptThr=2.0; }
		}
		if (nRapBins == 6) {
			if(-2.40<= dimu_y && dimu_y<-1.97) {  yflag = 0; ptThr=2.0; }
			else if(-1.97<= dimu_y && dimu_y<-1.37) {  yflag = 1;  ptThr=3.0; }
			else if(-1.37<= dimu_y && dimu_y<-0.47) {  yflag = 2;  ptThr=6.5; }
			else if(-0.47<= dimu_y && dimu_y<0.43) {  yflag = 3;  ptThr=6.5; }
			else if(0.43<= dimu_y && dimu_y<1.03) {  yflag = 4;  ptThr=6.5; }
			else if(1.03<= dimu_y && dimu_y<1.46) {  yflag = 5;  ptThr=5.0; }
		}
		if(yflag==99) continue;
		if(dimu_pt<ptThr) st_AccRange = "out of the range";
		recoWeight = Func[yflag]->Eval(dimu_pt);

		////// for denominator
		if (dimuCut(dmom0_Id,dkid0_ch,dkid1_ch) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) { 
			h2D_Den_noPtWeight_pt_y_Pbp->Fill(dimu_y,dimu_pt);
			h2D_Den_pt_y_Pbp->Fill(dimu_y,dimu_pt,recoWeight);
			if (mrapnpt==89) h2D_Den_pt_y_Pbp_fine->Fill(dimu_y,dimu_pt);
			if (massCut(dimu_mass) && kineCut(dkid0_pt,dkid0_eta,dkid0_p) && kineCut(dkid1_pt,dkid1_eta,dkid1_p)) { 
				h2D_Num_noPtWeight_pt_y_Pbp->Fill(dimu_y,dimu_pt);
				h2D_Num_pt_y_Pbp->Fill(dimu_y,dimu_pt,recoWeight);
				if (mrapnpt==89) h2D_Num_pt_y_Pbp_fine->Fill(dimu_y,dimu_pt);
			}
		}
	}// end of loop
	// (Num/Den) to get acceptance (B : binomial error)
	h2D_Acc_noPtWeight_pt_y_Pbp->Divide(h2D_Num_noPtWeight_pt_y_Pbp,h2D_Den_noPtWeight_pt_y_Pbp,1,1,"B");
	h2D_Acc_pt_y_Pbp->Divide(h2D_Num_pt_y_Pbp,h2D_Den_pt_y_Pbp,1,1,"B");
	if (mrapnpt==89) h2D_Acc_pt_y_Pbp_fine->Divide(h2D_Num_pt_y_Pbp_fine,h2D_Den_pt_y_Pbp_fine,1,1,"B");

	////////////////////////////////////////////////////////////////////////////
	// difference and relative error calculation
  TH2D* h2D_Acc_diff_pt_y_Pbp;
  TH2D* h2D_Acc_err_pt_y_Pbp = new TH2D("h2D_Acc_err_pt_y_Pbp","",nRapBins,rapBinsArr,nPtBins,ptBinsArr);
	h2D_Acc_err_pt_y_Pbp->Sumw2();
	for(int ipt=0;ipt<nPtBins;ipt++){
		for(int iy=0;iy<nRapBins;iy++){
			h2D_Acc_diff_pt_y_Pbp = (TH2D*) h2D_Acc_pt_y_Pbp->Clone("h2D_Acc_diff_pt_y_Pbp");
			h2D_Acc_diff_pt_y_Pbp->Add(h2D_Acc_noPtWeight_pt_y_Pbp,-1); 
			h2D_Acc_err_pt_y_Pbp->Divide(h2D_Acc_diff_pt_y_Pbp,h2D_Acc_pt_y_Pbp);
		}
	}

  ////////////////////////////////////////////////////////////////////////////
	// rapidity change, for 2nd run hist.
	TH2D *h2D_Acc_noPtWeight_pt_y_pPb = new TH2D("h2D_Acc_noPtWeight_pt_y_pPb","",nRapBins2nd,rapBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Den_pt_y_pPb = new TH2D("h2D_Den_pt_y_pPb","",nRapBins2nd,rapBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y_pPb = new TH2D("h2D_Num_pt_y_pPb","",nRapBins2nd,rapBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_pt_y_pPb = new TH2D("h2D_Acc_pt_y_pPb","",nRapBins2nd,rapBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_err_pt_y_pPb = new TH2D("h2D_Acc_err_pt_y_pPb","",nRapBins2nd,rapBinsArr2nd,nPtBins,ptBinsArr);
	h2D_Acc_err_pt_y_pPb->Sumw2();
	h2D_Acc_pt_y_pPb->Sumw2();
	h2D_Acc_noPtWeight_pt_y_pPb->Sumw2();

	int tmpbin01Pbp, tmpbin01pPb; 
	int tmpbin02Pbp, tmpbin02pPb; 
	int tmpbin03Pbp, tmpbin03pPb; 
	int tmpbin04Pbp, tmpbin04pPb; 
	int tmpbin05Pbp, tmpbin05pPb; 
	float tmp01Val, tmp01Err;
	float tmp02Val, tmp02Err;
	float tmp03Val, tmp03Err;
	float tmp04Val, tmp04Err;
	float tmp05Val, tmp05Err;
	for (Int_t iy=0; iy<nRapBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++) {
			tmpbin01Pbp = h2D_Acc_noPtWeight_pt_y_Pbp->FindBin((rapBinsArr[iy]+rapBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp01Val = h2D_Acc_noPtWeight_pt_y_Pbp->GetBinContent(tmpbin01Pbp);
			tmp01Err = h2D_Acc_noPtWeight_pt_y_Pbp->GetBinError(tmpbin01Pbp);
			tmpbin01pPb = h2D_Acc_noPtWeight_pt_y_pPb->FindBin((rapBinsArr2nd[nRapBins-iy-1]+rapBinsArr2nd[nRapBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Acc_noPtWeight_pt_y_pPb->SetBinContent(tmpbin01pPb,tmp01Val);
			h2D_Acc_noPtWeight_pt_y_pPb->SetBinError(tmpbin01pPb,tmp01Err);
			
			tmpbin02Pbp = h2D_Den_pt_y_Pbp->FindBin((rapBinsArr[iy]+rapBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp02Val = h2D_Den_pt_y_Pbp->GetBinContent(tmpbin02Pbp);
			tmp02Err = h2D_Den_pt_y_Pbp->GetBinError(tmpbin02Pbp);
			tmpbin02pPb = h2D_Den_pt_y_pPb->FindBin((rapBinsArr2nd[nRapBins-iy-1]+rapBinsArr2nd[nRapBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Den_pt_y_pPb->SetBinContent(tmpbin02pPb,tmp02Val);
			h2D_Den_pt_y_pPb->SetBinError(tmpbin02pPb,tmp02Err);
			
			tmpbin03Pbp = h2D_Num_pt_y_Pbp->FindBin((rapBinsArr[iy]+rapBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp03Val = h2D_Num_pt_y_Pbp->GetBinContent(tmpbin03Pbp);
			tmp03Err = h2D_Num_pt_y_Pbp->GetBinError(tmpbin03Pbp);
			tmpbin03pPb = h2D_Num_pt_y_pPb->FindBin((rapBinsArr2nd[nRapBins-iy-1]+rapBinsArr2nd[nRapBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Num_pt_y_pPb->SetBinContent(tmpbin03pPb,tmp03Val);
			h2D_Num_pt_y_pPb->SetBinError(tmpbin03pPb,tmp03Err);
			
			tmpbin04Pbp = h2D_Acc_pt_y_Pbp->FindBin((rapBinsArr[iy]+rapBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp04Val = h2D_Acc_pt_y_Pbp->GetBinContent(tmpbin04Pbp);
			tmp04Err = h2D_Acc_pt_y_Pbp->GetBinError(tmpbin04Pbp);
			tmpbin04pPb = h2D_Acc_pt_y_pPb->FindBin((rapBinsArr2nd[nRapBins-iy-1]+rapBinsArr2nd[nRapBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Acc_pt_y_pPb->SetBinContent(tmpbin04pPb,tmp04Val);
			h2D_Acc_pt_y_pPb->SetBinError(tmpbin04pPb,tmp04Err);
			
			tmpbin05Pbp = h2D_Acc_err_pt_y_Pbp->FindBin((rapBinsArr[iy]+rapBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp05Val = h2D_Acc_err_pt_y_Pbp->GetBinContent(tmpbin05Pbp);
			tmp05Err = h2D_Acc_err_pt_y_Pbp->GetBinError(tmpbin05Pbp);
			tmpbin05pPb = h2D_Acc_err_pt_y_pPb->FindBin((rapBinsArr2nd[nRapBins-iy-1]+rapBinsArr2nd[nRapBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Acc_err_pt_y_pPb->SetBinContent(tmpbin05pPb,tmp05Val);
			h2D_Acc_err_pt_y_pPb->SetBinError(tmpbin05pPb,tmp05Err);
		}
	}
		
	////////////////////////////////////////////////////////////////////////////
	// Save the data!
	//TFile *outFile = new TFile(Form("AccAna_%s_ptWeight.root",strName),"RECREATE");
	TFile *outFile = new TFile(Form("AccAna_%s_ptWeight_isStiff%d.root",strName,(int)isStiff),"RECREATE");
	std::cout << "strName: " << strName << std::endl;
	
	outFile->cd();

	h2D_Den_noPtWeight_pt_y_Pbp->Write();
	h2D_Num_noPtWeight_pt_y_Pbp->Write();
	h2D_Acc_noPtWeight_pt_y_Pbp->Write();
	h2D_Den_pt_y_Pbp->Write();
	h2D_Num_pt_y_Pbp->Write();
	h2D_Acc_pt_y_Pbp->Write();
	h2D_Acc_diff_pt_y_Pbp->Write();
	h2D_Acc_err_pt_y_Pbp->Write();
	
	h2D_Acc_noPtWeight_pt_y_pPb->Write();
	h2D_Den_pt_y_pPb->Write();
	h2D_Num_pt_y_pPb->Write();
	h2D_Acc_pt_y_pPb->Write();
	h2D_Acc_err_pt_y_pPb->Write();
	
	if (mrapnpt==89) {
		h2D_Den_pt_y_Pbp_fine->Write();
		h2D_Num_pt_y_Pbp_fine->Write();
		h2D_Acc_pt_y_Pbp_fine->Write();
	}
   
	outFile->Close();

} // end of main func

////////////////////////////////////////////////////////////////////////////
// sub-routines function 

bool dimuCut(int lv_dmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch) {
	return (TMath::Abs(lv_dmom0_Id)==443 && lv_dkid0_ch*lv_dkid1_ch<0);
}

bool kineCut(double muPt, double muEta, double muP) {
	//cutG (final)
	return ( TMath::Abs(muEta) < 2.4 &&
				((TMath::Abs(muEta) < 1.2 && muPt >=3.3) ||
				(1.2 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.1 && muPt >= -(1.0/0.9)*TMath::Abs(muEta)+(1.2*(1.0/0.9)+2.6)) ||
				(2.1 <= TMath::Abs(muEta) && muPt >= 1.3)));
}

bool massCut(double lv_dimu_mass) {
	return ( 2.6 <= lv_dimu_mass && lv_dimu_mass < 3.5 );
}

