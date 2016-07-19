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
#include <TString.h>

/////// main func. ///////
int make2Droot_TotSys_ethf(char *dirName = "6rap3pt"){

  using namespace std;
	const int nET = 3; // 0-20, 20-30, 30-120
	cout << "nET = " << nET << endl;

//	const int nybin = 8;
//	const int nptbin = 9;
	const int nybin = 6;
	const int nptbin = 3;

	////////////////////////////////////////////////
	////// read in from each sys. file
	//TFile* fFit = new TFile(Form("../000_fittingSys/fitSysErr_%s_relForAN.root",dirName));
	TFile* fFit = new TFile("../fittingSys/fitSysErr_ethf.root");
	TFile* fAccPR = new TFile("../AccEffWeight/AccAna_isPrompt1_Pbp_kyo01.root");
	TFile* fAccNP = new TFile("../AccEffWeight/AccAna_isPrompt0_Pbp_kyo01.root");
//	TFile* fEff = new TFile("total_sys_unc_from_efficiency_old.root");
	//TFile* fEff = new TFile("total_sys_unc_from_efficiency.root");
	TFile* fEff = new TFile("../Efficiency/total_sys_unc_from_efficiency.root");

	TH2D* hFitPR[nET];
	TH2D* hFitNP[nET];
	TH2D* hAccPR[nET];
	TH2D* hAccNP[nET];
	TH2D* hEffPR[nET];
	TH2D* hEffNP[nET];
	for (int iet=0; iet<nET; iet++) {
		hFitPR[iet] = (TH2D*)fFit->Get(Form("h2D_PR_tot_bin%d",iet+1));	
		hFitNP[iet] = (TH2D*)fFit->Get(Form("h2D_NP_tot_bin%d",iet+1));	
		hAccPR[iet] = (TH2D*)fAccPR->Get("h2D_Acc_err_pt_y_Pbp");	
		hAccNP[iet] = (TH2D*)fAccNP->Get("h2D_Acc_err_pt_y_Pbp");	
		hEffPR[iet] = (TH2D*)fEff->Get("totEffSys_prompt");	
		hEffNP[iet] = (TH2D*)fEff->Get("totEffSys_nonprompt");	
		hFitPR[iet]->SetName(Form("hFitPR_bin%d",iet+1));
		hFitNP[iet]->SetName(Form("hFitNP_bin%d",iet+1));
		hAccPR[iet]->SetName(Form("hAccPR_bin%d",iet+1));
		hAccNP[iet]->SetName(Form("hAccNP_bin%d",iet+1));
		hEffPR[iet]->SetName(Form("hEffPR_bin%d",iet+1));
		hEffNP[iet]->SetName(Form("hEffNP_bin%d",iet+1));
	}	

	TH2D* hTotalPR[nET];
	TH2D* hTotalNP[nET];
	for (int iet=0; iet<nET; iet++) {
		hTotalPR[iet]=(TH2D*)hFitPR[iet]->Clone(Form("hTotalPR_bin%d",iet+1));	//y binning in y_lab^1st
		hTotalNP[iet]=(TH2D*)hFitNP[iet]->Clone(Form("hTotalNP_bin%d",iet+1));	//y binning in y_lab^1st
	}
	//hTotalPR[0]->Draw("text col");
	//hAccPR[0]->Draw("text col");
	//hEffPR[0]->Draw("text col");

	double tmpAccPR[nET][nybin][nptbin];
	double tmpAccNP[nET][nybin][nptbin];
	double tmpEffPR[nET][nybin][nptbin];
	double tmpEffNP[nET][nybin][nptbin];
	
	// take the largest among Acc and Eff bin
	for (int iet=0; iet<nET; iet++) {
		for (int iy=0; iy<nybin;iy++ ){
			//1) 5-6.5 GeV/c
			if (iy==0 || iy==5) {
				tmpAccPR[iet][iy][0] = hAccPR[iet]->GetBinContent(iy+1,4); 	
				tmpAccNP[iet][iy][0] = hAccNP[iet]->GetBinContent(iy+1,4); 	
				tmpEffPR[iet][iy][0] = hEffPR[iet]->GetBinContent(iy+1,4); 	
				tmpEffNP[iet][iy][0] = hEffNP[iet]->GetBinContent(iy+1,4); 	
			}
			else {
				tmpAccPR[iet][iy][0] = 0.;
				tmpAccNP[iet][iy][0] = 0.;
				tmpEffPR[iet][iy][0] = 0.;
				tmpEffNP[iet][iy][0] = 0.;
			}
			//2) 6.5-10 GeV/c
			tmpAccPR[iet][iy][1]=-532;
			tmpAccNP[iet][iy][1]=-532;
			tmpEffPR[iet][iy][1]=-532;
			tmpEffNP[iet][iy][1]=-532;
			for (int ipt=4; ipt <7; ipt++){
				if (tmpAccPR[iet][iy][1] < hAccPR[iet]->GetBinContent(iy+1,ipt+1)) {tmpAccPR[iet][iy][1] = hAccPR[iet]->GetBinContent(iy+1,ipt+1);}
				if (tmpAccNP[iet][iy][1] < hAccNP[iet]->GetBinContent(iy+1,ipt+1)) {tmpAccNP[iet][iy][1] = hAccNP[iet]->GetBinContent(iy+1,ipt+1);}
				if (tmpEffPR[iet][iy][1] < hEffPR[iet]->GetBinContent(iy+1,ipt+1)) {tmpEffPR[iet][iy][1] = hEffPR[iet]->GetBinContent(iy+1,ipt+1);}
				if (tmpEffNP[iet][iy][1] < hEffNP[iet]->GetBinContent(iy+1,ipt+1)) {tmpEffNP[iet][iy][1] = hEffNP[iet]->GetBinContent(iy+1,ipt+1);}
			}
			//2) 10-30 GeV/c
			tmpAccPR[iet][iy][2]=-532;
			tmpAccNP[iet][iy][2]=-532;
			tmpEffPR[iet][iy][2]=-532;
			tmpEffNP[iet][iy][2]=-532;
			for (int ipt=7; ipt <9; ipt++){
				if (tmpAccPR[iet][iy][2] < hAccPR[iet]->GetBinContent(iy+1,ipt+1)) {tmpAccPR[iet][iy][2] = hAccPR[iet]->GetBinContent(iy+1,ipt+1);}
				if (tmpAccNP[iet][iy][2] < hAccNP[iet]->GetBinContent(iy+1,ipt+1)) {tmpAccNP[iet][iy][2] = hAccNP[iet]->GetBinContent(iy+1,ipt+1);}
				if (tmpEffPR[iet][iy][2] < hEffPR[iet]->GetBinContent(iy+1,ipt+1)) {tmpEffPR[iet][iy][2] = hEffPR[iet]->GetBinContent(iy+1,ipt+1);}
				if (tmpEffNP[iet][iy][2] < hEffNP[iet]->GetBinContent(iy+1,ipt+1)) {tmpEffNP[iet][iy][2] = hEffNP[iet]->GetBinContent(iy+1,ipt+1);}
			}
		}
	}		
/*
	for (int iet=0; iet<nET; iet++) {
		for (int iy=0; iy<nybin;iy++ ){
			for (int ipt=0; ipt <nptbin; ipt++){
				cout << iet<<", "<<iy<<", "<<ipt<<", tmpEffPR = " <<tmpEffPR[iet][iy][ipt]<<endl;
				hTotalPR[iet]->SetBinContent(iy+1,ipt+1,tmpEffPR[iet][iy][ipt]);
			}
		}
	}		
	hTotalPR[0]->Draw("text col");
*/

	double tmpPRtot[nET][nybin][nptbin];
	double tmpNPtot[nET][nybin][nptbin];
	double tmp_pr1;
	double tmp_np1;

	for (int iet=0; iet<nET; iet++) {
		for (int iy=0; iy<nybin;iy++ ){
			for (int ipt=0; ipt<nptbin;ipt++ ){
				tmpPRtot[iet][iy][ipt]=0;
				tmpNPtot[iet][iy][ipt]=0;
				tmp_pr1 = hFitPR[iet]->GetBinContent(iy+1,ipt+1);	
				tmp_np1 = hFitNP[iet]->GetBinContent(iy+1,ipt+1);	
				//calculate the total relative error
				tmpPRtot[iet][iy][ipt]=TMath::Sqrt(tmp_pr1*tmp_pr1 + tmpAccPR[iet][iy][ipt]*tmpAccPR[iet][iy][ipt] + tmpEffPR[iet][iy][ipt]*tmpEffPR[iet][iy][ipt]);
				tmpNPtot[iet][iy][ipt]=TMath::Sqrt(tmp_pr1*tmp_pr1 + tmpAccNP[iet][iy][ipt]*tmpAccNP[iet][iy][ipt] + tmpEffNP[iet][iy][ipt]*tmpEffNP[iet][iy][ipt]);
				//fill into hist
				hTotalPR[iet]->SetBinContent(iy+1,ipt+1,tmpPRtot[iet][iy][ipt]);
				hTotalNP[iet]->SetBinContent(iy+1,ipt+1,tmpNPtot[iet][iy][ipt]);
			}
		}
	}
	
	//set unsed values as zero		
	for (int iet=0; iet<nET; iet++) {
		for (int iy=0; iy<nybin;iy++ ){
			for (int ipt=0; ipt<nptbin;ipt++ ){
				if ( !(ipt==0 && iy>=1 && iy<=4) ) {continue;}
				hTotalPR[iet]->SetBinContent(iy+1, ipt+1, 0);
				hTotalNP[iet]->SetBinContent(iy+1, ipt+1, 0);
			}
		}	
	}	
	
	////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("TotSys_ethf_%s.root",dirName),"RECREATE");
	std::cout << "dirName: " << dirName << std::endl;
	cout << "TotSys_ethf_"<<dirName<<".root has been created :) " <<endl;	

	outFile->cd();
	for (int iet=0; iet<nET; iet++) {
		hFitPR[iet]->Write();
		hAccPR[iet]->Write();
		hEffPR[iet]->Write();
		hTotalPR[iet]->Write();
		hFitNP[iet]->Write();
		hAccNP[iet]->Write();
		hEffNP[iet]->Write();
		hTotalNP[iet]->Write();
	}
	outFile->Close();


	return 0;

} // end of main func

