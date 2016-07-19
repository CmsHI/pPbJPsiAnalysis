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
int make2Droot_TotSys(char *dirName = "8rap9pt2gev"){

  using namespace std;

	const int nybin = 8;
	const int nptbin = 9;

	////////////////////////////////////////////////
	////// read in from each sys. file
	TFile* fFit = new TFile(Form("../fittingSys/fitSysErr_%s.root",dirName));
	TFile* fAccPR = new TFile("../AcceEffWeight/AccAna_isPrompt1_Pbp_kyo01.root");
	TFile* fAccNP = new TFile("../AcceEffWeight/AccAna_isPrompt0_Pbp_kyo01.root");
//	TFile* fEff = new TFile("total_sys_unc_from_efficiency.root");
	TFile* fEff = new TFile("../Efficiency/total_sys_unc_from_efficiency.root");
	TH2D* hFitPR = (TH2D*)fFit->Get("h2D_PR_tot");	
	TH2D* hFitNP = (TH2D*)fFit->Get("h2D_NP_tot");	
	TH2D* hAccPR = (TH2D*)fAccPR->Get("h2D_Acc_err_pt_y_Pbp");	
	TH2D* hAccNP = (TH2D*)fAccNP->Get("h2D_Acc_err_pt_y_Pbp");	
	TH2D* hEffPR = (TH2D*)fEff->Get("totEffSys_prompt");	
	TH2D* hEffNP = (TH2D*)fEff->Get("totEffSys_nonprompt");	
	hFitPR->SetName("hFitPR");
	hFitNP->SetName("hFitNP");
	hAccPR->SetName("hAccPR");
	hAccNP->SetName("hAccNP");
	hEffPR->SetName("hEffPR");
	hEffNP->SetName("hEffNP");
	
	TH2D* hTotalPR=(TH2D*)hFitPR->Clone("hTotalPR");	//y binning in y_lab^1st
	TH2D* hTotalNP=(TH2D*)hFitNP->Clone("hTotalNP");	//y binning in y_lab^1st
	
	double tmpPRtot[nybin][nptbin];
	double tmpNPtot[nybin][nptbin];
	double tmp_pr1, tmp_pr2, tmp_pr3;
	double tmp_np1, tmp_np2, tmp_np3;

	for (int iy=0; iy<nybin;iy++ ){
		for (int ipt=0; ipt<nptbin;ipt++ ){
			tmpPRtot[iy][ipt]=0;
			tmpNPtot[iy][ipt]=0;
			tmp_pr1 = hFitPR->GetBinContent(iy+1,ipt+1);	
			tmp_pr2 = hAccPR->GetBinContent(iy+1,ipt+1);	
			tmp_pr3 = hEffPR->GetBinContent(iy+1,ipt+1);	
			tmp_np1 = hFitNP->GetBinContent(iy+1,ipt+1);	
			tmp_np2 = hAccNP->GetBinContent(iy+1,ipt+1);	
			tmp_np3 = hEffNP->GetBinContent(iy+1,ipt+1);	
			//calculate the total relative error
			tmpPRtot[iy][ipt]=TMath::Sqrt(tmp_pr1*tmp_pr1+tmp_pr2*tmp_pr2+tmp_pr3*tmp_pr3);
			tmpNPtot[iy][ipt]=TMath::Sqrt(tmp_np1*tmp_np1+tmp_np2*tmp_np2+tmp_np3*tmp_np3);
			//fill into hist
			hTotalPR->SetBinContent(iy+1,ipt+1,tmpPRtot[iy][ipt]);
			hTotalNP->SetBinContent(iy+1,ipt+1,tmpNPtot[iy][ipt]);
		}
	}

	//set unsed values as zero		
	for (int iy=0; iy<nybin;iy++ ){
		for (int ipt=0; ipt<nptbin;ipt++ ){
			if ( !( (iy>=1&&iy<=6&&ipt==0) || (iy>=2&&iy<=5&&(ipt==1||ipt==2)) || (iy>=2&&iy<=4&&ipt==3) ) ) { continue;}
			hTotalPR->SetBinContent(iy+1, ipt+1, 0);
			hTotalNP->SetBinContent(iy+1, ipt+1, 0);
		}
	}	
	
	////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("TotSys_%s.root",dirName),"RECREATE");
	std::cout << "dirName: " << dirName << std::endl;
	cout << "TotSys_"<<dirName<<".root has been created :) " <<endl;	

	outFile->cd();
	hFitPR->Write();
	hAccPR->Write();
	hEffPR->Write();
	hTotalPR->Write();
	hFitNP->Write();
	hAccNP->Write();
	hEffNP->Write();
	hTotalNP->Write();
	outFile->Close();

	return 0;

} // end of main func

