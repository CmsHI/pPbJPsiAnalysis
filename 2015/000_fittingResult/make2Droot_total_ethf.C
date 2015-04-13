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

/////// main func. ///////
int make2Droot_total_ethf(char *dirName = "6rap3pt", char *dateName = "no"){

  using namespace std;

	const int nET = 3; // 0-20, 20-30, 30-120

	////////////////////////////////////////////////
	////// read in Acceptance file
	TFile *fAccPR = new TFile("../001_Acceptance/AccAna_6rap3pt_PRMC_boosted.root");
	TFile *fAccNP = new TFile("../001_Acceptance/AccAna_6rap3pt_NPMC_boosted.root");
	
	TH2D* h2D_Acc_pt_y_PRMC_Pbp[nET];
	TH2D* h2D_Acc_pt_y_PRMC_pPb[nET];
	TH2D* h2D_Acc_pt_y_NPMC_Pbp[nET];
	TH2D* h2D_Acc_pt_y_NPMC_pPb[nET];
	TH2D* h2D_Acc_Den_pt_y_PRMC_Pbp[nET];
	TH2D* h2D_Acc_Den_pt_y_PRMC_pPb[nET];
	TH2D* h2D_Acc_Den_pt_y_NPMC_Pbp[nET];
	TH2D* h2D_Acc_Den_pt_y_NPMC_pPb[nET];
	TH2D* h2D_Acc_Num_pt_y_PRMC_Pbp[nET];
	TH2D* h2D_Acc_Num_pt_y_PRMC_pPb[nET];
	TH2D* h2D_Acc_Num_pt_y_NPMC_Pbp[nET];
	TH2D* h2D_Acc_Num_pt_y_NPMC_pPb[nET];
	
	for (int iet=0; iet<nET; iet++) {
		h2D_Acc_pt_y_PRMC_Pbp[iet] = (TH2D*)fAccPR->Get("h2D_Acc_pt_y_Pbp");
		h2D_Acc_pt_y_PRMC_pPb[iet] = (TH2D*)fAccPR->Get("h2D_Acc_pt_y_pPb");
		h2D_Acc_pt_y_NPMC_Pbp[iet] = (TH2D*)fAccNP->Get("h2D_Acc_pt_y_Pbp");
		h2D_Acc_pt_y_NPMC_pPb[iet] = (TH2D*)fAccNP->Get("h2D_Acc_pt_y_pPb");
		h2D_Acc_pt_y_PRMC_Pbp[iet]->SetName(Form("h2D_Acc_pt_y_PRMC_Pbp_%d",iet));
		h2D_Acc_pt_y_PRMC_pPb[iet]->SetName(Form("h2D_Acc_pt_y_PRMC_pPb_%d",iet));
		h2D_Acc_pt_y_NPMC_Pbp[iet]->SetName(Form("h2D_Acc_pt_y_NPMC_Pbp_%d",iet));
		h2D_Acc_pt_y_NPMC_pPb[iet]->SetName(Form("h2D_Acc_pt_y_NPMC_pPb_%d",iet));
		cout << iet<<"th 4 Acc TH2D : "<<h2D_Acc_pt_y_PRMC_Pbp[iet]<<" "<<h2D_Acc_pt_y_PRMC_pPb[iet]<<" "<<h2D_Acc_pt_y_NPMC_Pbp[iet]<<" "<<h2D_Acc_pt_y_NPMC_pPb[iet]<<endl;
	
		h2D_Acc_Den_pt_y_PRMC_Pbp[iet] = (TH2D*)fAccPR->Get("h2D_Den_pt_y_Pbp");
		h2D_Acc_Den_pt_y_PRMC_pPb[iet] = (TH2D*)fAccPR->Get("h2D_Den_pt_y_pPb");
		h2D_Acc_Den_pt_y_NPMC_Pbp[iet] = (TH2D*)fAccNP->Get("h2D_Den_pt_y_Pbp");
		h2D_Acc_Den_pt_y_NPMC_pPb[iet] = (TH2D*)fAccNP->Get("h2D_Den_pt_y_pPb");
		h2D_Acc_Den_pt_y_PRMC_Pbp[iet]->SetName(Form("h2D_Acc_Den_pt_y_PRMC_Pbp_%d",iet));
		h2D_Acc_Den_pt_y_PRMC_pPb[iet]->SetName(Form("h2D_Acc_Den_pt_y_PRMC_pPb_%d",iet));
		h2D_Acc_Den_pt_y_NPMC_Pbp[iet]->SetName(Form("h2D_Acc_Den_pt_y_NPMC_Pbp_%d",iet));
		h2D_Acc_Den_pt_y_NPMC_pPb[iet]->SetName(Form("h2D_Acc_Den_pt_y_NPMC_pPb_%d",iet));
	
		h2D_Acc_Num_pt_y_PRMC_Pbp[iet] = (TH2D*)fAccPR->Get("h2D_Num_pt_y_Pbp");
		h2D_Acc_Num_pt_y_PRMC_pPb[iet] = (TH2D*)fAccPR->Get("h2D_Num_pt_y_pPb");
		h2D_Acc_Num_pt_y_NPMC_Pbp[iet] = (TH2D*)fAccNP->Get("h2D_Num_pt_y_Pbp");
		h2D_Acc_Num_pt_y_NPMC_pPb[iet] = (TH2D*)fAccNP->Get("h2D_Num_pt_y_pPb");
		h2D_Acc_Num_pt_y_PRMC_Pbp[iet]->SetName(Form("h2D_Acc_Num_pt_y_PRMC_Pbp_%d",iet));
		h2D_Acc_Num_pt_y_PRMC_pPb[iet]->SetName(Form("h2D_Acc_Num_pt_y_PRMC_pPb_%d",iet));
		h2D_Acc_Num_pt_y_NPMC_Pbp[iet]->SetName(Form("h2D_Acc_Num_pt_y_NPMC_Pbp_%d",iet));
		h2D_Acc_Num_pt_y_NPMC_pPb[iet]->SetName(Form("h2D_Acc_Num_pt_y_NPMC_pPb_%d",iet));
	}
		
	

	////////////////////////////////////////////////
	////// read in Efficiency file
	TFile* fEffPRPbp[nET];
	TFile* fEffPRpPb[nET];
	TFile* fEffNPPbp[nET];
	TFile* fEffNPpPb[nET];
	TH2D* h2D_Eff_pt_y_PRMC_Pbp[nET];
	TH2D* h2D_Eff_pt_y_PRMC_pPb[nET];
	TH2D* h2D_Eff_pt_y_NPMC_Pbp[nET];
	TH2D* h2D_Eff_pt_y_NPMC_pPb[nET];
	TH2D* h2D_Eff_Den_pt_y_PRMC_Pbp[nET];
	TH2D* h2D_Eff_Den_pt_y_PRMC_pPb[nET];
	TH2D* h2D_Eff_Den_pt_y_NPMC_Pbp[nET];
	TH2D* h2D_Eff_Den_pt_y_NPMC_pPb[nET];
	TH2D* h2D_Eff_Num_pt_y_PRMC_Pbp[nET];
	TH2D* h2D_Eff_Num_pt_y_PRMC_pPb[nET];
	TH2D* h2D_Eff_Num_pt_y_NPMC_Pbp[nET];
	TH2D* h2D_Eff_Num_pt_y_NPMC_pPb[nET];
	
	for (int iet=0; iet<nET; iet++) {
//		fEffPRPbp[iet] = new TFile(Form("../002_Efficiency_zVtx_embedded/EffCounting_6rap3pt_PRMCpythia_Pbp_useCtErr_1_useDataDriven_0_useZvtxStep1_0_Step2_1_bin%d.root",iet+1));
//		fEffPRpPb[iet] = new TFile(Form("../002_Efficiency_zVtx_embedded/EffCounting_6rap3pt_PRMCpythia_pPb_useCtErr_1_useDataDriven_0_useZvtxStep1_0_Step2_1_bin%d.root",iet+1));
//		fEffNPPbp[iet] = new TFile(Form("../002_Efficiency_zVtx_embedded/EffCounting_6rap3pt_NPMCpythia_Pbp_useCtErr_1_useDataDriven_0_useZvtxStep1_0_Step2_1_bin%d.root",iet+1));
//		fEffNPpPb[iet] = new TFile(Form("../002_Efficiency_zVtx_embedded/EffCounting_6rap3pt_NPMCpythia_pPb_useCtErr_1_useDataDriven_0_useZvtxStep1_0_Step2_1_bin%d.root",iet+1));
		fEffPRPbp[iet] = new TFile(Form("../002_Efficiency_zVtx_embedded/EffCounting_6rap3pt_PRMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_bin%d.root",iet+1));
		fEffPRpPb[iet] = new TFile(Form("../002_Efficiency_zVtx_embedded/EffCounting_6rap3pt_PRMCpythia_pPb_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_bin%d.root",iet+1));
		fEffNPPbp[iet] = new TFile(Form("../002_Efficiency_zVtx_embedded/EffCounting_6rap3pt_NPMCpythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_bin%d.root",iet+1));
		fEffNPpPb[iet] = new TFile(Form("../002_Efficiency_zVtx_embedded/EffCounting_6rap3pt_NPMCpythia_pPb_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1_bin%d.root",iet+1));
		h2D_Eff_pt_y_PRMC_Pbp[iet] = (TH2D*)fEffPRPbp[iet]->Get("h2D_Eff_pt_y");
		h2D_Eff_pt_y_PRMC_pPb[iet] = (TH2D*)fEffPRpPb[iet]->Get("h2D_Eff_pt_y");
		h2D_Eff_pt_y_NPMC_Pbp[iet] = (TH2D*)fEffNPPbp[iet]->Get("h2D_Eff_pt_y");
		h2D_Eff_pt_y_NPMC_pPb[iet] = (TH2D*)fEffNPpPb[iet]->Get("h2D_Eff_pt_y");
		h2D_Eff_pt_y_PRMC_Pbp[iet]->SetName(Form("h2D_Eff_pt_y_PRMC_Pbp_%d",iet));
		h2D_Eff_pt_y_PRMC_pPb[iet]->SetName(Form("h2D_Eff_pt_y_PRMC_pPb_%d",iet));
		h2D_Eff_pt_y_NPMC_Pbp[iet]->SetName(Form("h2D_Eff_pt_y_NPMC_Pbp_%d",iet));
		h2D_Eff_pt_y_NPMC_pPb[iet]->SetName(Form("h2D_Eff_pt_y_NPMC_pPb_%d",iet));
		cout << iet<<"th 4 Eff TH2D : "<<h2D_Eff_pt_y_PRMC_Pbp[iet]<<" "<<h2D_Eff_pt_y_PRMC_pPb[iet]<<" "<<h2D_Eff_pt_y_NPMC_Pbp[iet]<<" "<<h2D_Eff_pt_y_NPMC_pPb[iet]<<endl;
	
		h2D_Eff_Den_pt_y_PRMC_Pbp[iet] = (TH2D*)fEffPRPbp[iet]->Get("h2D_Den_pt_y");
		h2D_Eff_Den_pt_y_PRMC_pPb[iet] = (TH2D*)fEffPRpPb[iet]->Get("h2D_Den_pt_y");
		h2D_Eff_Den_pt_y_NPMC_Pbp[iet] = (TH2D*)fEffNPPbp[iet]->Get("h2D_Den_pt_y");
		h2D_Eff_Den_pt_y_NPMC_pPb[iet] = (TH2D*)fEffNPpPb[iet]->Get("h2D_Den_pt_y");
		h2D_Eff_Den_pt_y_PRMC_Pbp[iet]->SetName(Form("h2D_Eff_Den_pt_y_PRMC_Pbp_%d",iet));
		h2D_Eff_Den_pt_y_PRMC_pPb[iet]->SetName(Form("h2D_Eff_Den_pt_y_PRMC_pPb_%d",iet));
		h2D_Eff_Den_pt_y_NPMC_Pbp[iet]->SetName(Form("h2D_Eff_Den_pt_y_NPMC_Pbp_%d",iet));
		h2D_Eff_Den_pt_y_NPMC_pPb[iet]->SetName(Form("h2D_Eff_Den_pt_y_NPMC_pPb_%d",iet));
	
		h2D_Eff_Num_pt_y_PRMC_Pbp[iet] = (TH2D*)fEffPRPbp[iet]->Get("h2D_Num_pt_y");
		h2D_Eff_Num_pt_y_PRMC_pPb[iet] = (TH2D*)fEffPRpPb[iet]->Get("h2D_Num_pt_y");
		h2D_Eff_Num_pt_y_NPMC_Pbp[iet] = (TH2D*)fEffNPPbp[iet]->Get("h2D_Num_pt_y");
		h2D_Eff_Num_pt_y_NPMC_pPb[iet] = (TH2D*)fEffNPpPb[iet]->Get("h2D_Num_pt_y");
		h2D_Eff_Num_pt_y_PRMC_Pbp[iet]->SetName(Form("h2D_Eff_Num_pt_y_PRMC_Pbp_%d",iet));
		h2D_Eff_Num_pt_y_PRMC_pPb[iet]->SetName(Form("h2D_Eff_Num_pt_y_PRMC_pPb_%d",iet));
		h2D_Eff_Num_pt_y_NPMC_Pbp[iet]->SetName(Form("h2D_Eff_Num_pt_y_NPMC_Pbp_%d",iet));
		h2D_Eff_Num_pt_y_NPMC_pPb[iet]->SetName(Form("h2D_Eff_Num_pt_y_NPMC_pPb_%d",iet));

	}	

	////////////////////////////////////////////////
	////// read in fromfit file
	TFile* fFitPbp[nET];
	TFile* fFitpPb[nET];
	TH2D* h2D_fit_pt_y_PR_Pbp[nET];	
	TH2D* h2D_fit_pt_y_NP_Pbp[nET];	
	TH2D* h2D_fit_pt_y_nSig_Pbp[nET];
	TH2D* h2D_fit_pt_y_nBkg_Pbp[nET];
	TH2D* h2D_fit_pt_y_bFrac_Pbp[nET];
	TH2D* h2D_fit_pt_y_ctErrmin_Pbp[nET];
	TH2D* h2D_fit_pt_y_ctErrmax_Pbp[nET];
	TH2D* h2D_fit_pt_y_PR_pPb[nET];	
	TH2D* h2D_fit_pt_y_NP_pPb[nET];	
	TH2D* h2D_fit_pt_y_nSig_pPb[nET];
	TH2D* h2D_fit_pt_y_nBkg_pPb[nET];
	TH2D* h2D_fit_pt_y_bFrac_pPb[nET];
	TH2D* h2D_fit_pt_y_ctErrmin_pPb[nET];
	TH2D* h2D_fit_pt_y_ctErrmax_pPb[nET];
	
	for (int iet=0; iet<nET; iet++) {
//		fFitPbp[iet] = new TFile(Form("../000_fittingResult/2Dhist_fitRes_6rap3pt_%s_Pbp_bin%d.root",dateName,iet+1));	
//		fFitpPb[iet] = new TFile(Form("../000_fittingResult/2Dhist_fitRes_6rap3pt_%s_pPb_bin%d.root",dateName,iet+1));	
		fFitPbp[iet] = new TFile(Form("../000_fittingResult/2Dhist_fitRes_6rap3pt_Pbp_bin%d.root",iet+1));	
		fFitpPb[iet] = new TFile(Form("../000_fittingResult/2Dhist_fitRes_6rap3pt_pPb_bin%d.root",iet+1));	
		h2D_fit_pt_y_PR_Pbp[iet] = (TH2D*)fFitPbp[iet]->Get("h2D_nPrompt");	
		h2D_fit_pt_y_NP_Pbp[iet] = (TH2D*)fFitPbp[iet]->Get("h2D_nNonprompt");	
		h2D_fit_pt_y_nSig_Pbp[iet] = (TH2D*)fFitPbp[iet]->Get("h2D_nSig");	
		h2D_fit_pt_y_nBkg_Pbp[iet] = (TH2D*)fFitPbp[iet]->Get("h2D_nBkg");	
		h2D_fit_pt_y_bFrac_Pbp[iet] = (TH2D*)fFitPbp[iet]->Get("h2D_bFraction");	
		h2D_fit_pt_y_ctErrmin_Pbp[iet] = (TH2D*)fFitPbp[iet]->Get("h2D_ctErrmin");	
		h2D_fit_pt_y_ctErrmax_Pbp[iet] = (TH2D*)fFitPbp[iet]->Get("h2D_ctErrmax");	
		h2D_fit_pt_y_PR_pPb[iet] = (TH2D*)fFitpPb[iet]->Get("h2D_nPrompt");	
		h2D_fit_pt_y_NP_pPb[iet] = (TH2D*)fFitpPb[iet]->Get("h2D_nNonprompt");	
		h2D_fit_pt_y_nSig_pPb[iet] = (TH2D*)fFitpPb[iet]->Get("h2D_nSig");	
		h2D_fit_pt_y_nBkg_pPb[iet] = (TH2D*)fFitpPb[iet]->Get("h2D_nBkg");	
		h2D_fit_pt_y_bFrac_pPb[iet] = (TH2D*)fFitpPb[iet]->Get("h2D_bFraction");	
		h2D_fit_pt_y_ctErrmin_pPb[iet] = (TH2D*)fFitpPb[iet]->Get("h2D_ctErrmin");	
		h2D_fit_pt_y_ctErrmax_pPb[iet] = (TH2D*)fFitpPb[iet]->Get("h2D_ctErrmax");	
		h2D_fit_pt_y_PR_Pbp[iet]->SetName(Form("h2D_fit_pt_y_PR_Pbp_%d",iet));
		h2D_fit_pt_y_PR_pPb[iet]->SetName(Form("h2D_fit_pt_y_PR_pPb_%d",iet));
		h2D_fit_pt_y_NP_Pbp[iet]->SetName(Form("h2D_fit_pt_y_NP_Pbp_%d",iet));
		h2D_fit_pt_y_NP_pPb[iet]->SetName(Form("h2D_fit_pt_y_NP_pPb_%d",iet));
		h2D_fit_pt_y_nSig_Pbp[iet]->SetName(Form("h2D_fit_pt_y_nSig_Pbp_%d",iet));
		h2D_fit_pt_y_nSig_pPb[iet]->SetName(Form("h2D_fit_pt_y_nSig_pPb_%d",iet));
		h2D_fit_pt_y_nBkg_Pbp[iet]->SetName(Form("h2D_fit_pt_y_nBkg_Pbp_%d",iet));
		h2D_fit_pt_y_nBkg_pPb[iet]->SetName(Form("h2D_fit_pt_y_nBkg_pPb_%d",iet));
		h2D_fit_pt_y_bFrac_Pbp[iet]->SetName(Form("h2D_fit_pt_y_bFrac_Pbp_%d",iet));
		h2D_fit_pt_y_bFrac_pPb[iet]->SetName(Form("h2D_fit_pt_y_bFrac_pPb_%d",iet));
		h2D_fit_pt_y_ctErrmin_Pbp[iet]->SetName(Form("h2D_fit_pt_y_ctErrmin_Pbp_%d",iet));
		h2D_fit_pt_y_ctErrmin_pPb[iet]->SetName(Form("h2D_fit_pt_y_ctErrmin_pPb_%d",iet));
		h2D_fit_pt_y_ctErrmax_Pbp[iet]->SetName(Form("h2D_fit_pt_y_ctErrmax_Pbp_%d",iet));
		h2D_fit_pt_y_ctErrmax_pPb[iet]->SetName(Form("h2D_fit_pt_y_ctErrmax_pPb_%d",iet));
	
		cout << iet<<"th 4 yield TH2D : "<<h2D_fit_pt_y_PR_Pbp[iet]<<" "<<h2D_fit_pt_y_PR_pPb[iet]<<" "<<h2D_fit_pt_y_NP_Pbp[iet]<<" "<<h2D_fit_pt_y_NP_pPb[iet]<<endl;
		cout << iet << "th 2 nSig TH2D : "<<h2D_fit_pt_y_nSig_Pbp[iet]<<" "<<h2D_fit_pt_y_nSig_pPb[iet]<<endl;
		cout << iet <<"th 2 nBkg TH2D : "<<h2D_fit_pt_y_nBkg_Pbp[iet]<<" "<<h2D_fit_pt_y_nBkg_pPb[iet]<<endl;
		cout << iet <<"th 2 bFrac TH2D : "<<h2D_fit_pt_y_bFrac_Pbp[iet]<<" "<<h2D_fit_pt_y_bFrac_pPb[iet]<<endl;
		cout << iet <<"th 2 ctErrmin TH2D : "<<h2D_fit_pt_y_ctErrmin_Pbp[iet]<<" "<<h2D_fit_pt_y_ctErrmin_pPb[iet]<<endl;
		cout << iet <<"th 2 ctErrmax TH2D : "<<h2D_fit_pt_y_ctErrmax_Pbp[iet]<<" "<<h2D_fit_pt_y_ctErrmax_pPb[iet]<<endl;
	
	}

	////////////////////////////////////////////////
	////// data corrected yield = rawYield/(Acc*Eff)
	TH2D* h2D_corrY_PR_Pbp[nET];
	TH2D* h2D_corrY_PR_pPb[nET];
	TH2D* h2D_corrY_NP_Pbp[nET];
	TH2D* h2D_corrY_NP_pPb[nET];
	////// MC corrected yield RECO/(Acc*Eff)
	TH2D* h2D_MCcorrY_PR_Pbp[nET];
	TH2D* h2D_MCcorrY_PR_pPb[nET];
	TH2D* h2D_MCcorrY_NP_Pbp[nET];
	TH2D* h2D_MCcorrY_NP_pPb[nET];
	
	for (int iet=0; iet<nET; iet++) {
		h2D_corrY_PR_Pbp[iet] = (TH2D*)h2D_fit_pt_y_PR_Pbp[iet]->Clone(Form("h2D_corrY_PR_Pbp_%d",iet));
		h2D_corrY_PR_Pbp[iet]->SetName(Form("h2D_corrY_PR_Pbp_%d",iet));
		h2D_corrY_PR_Pbp[iet]->Divide(h2D_Acc_pt_y_PRMC_Pbp[iet]);
		h2D_corrY_PR_Pbp[iet]->Divide(h2D_Eff_pt_y_PRMC_Pbp[iet]);
		h2D_corrY_NP_Pbp[iet] = (TH2D*)h2D_fit_pt_y_NP_Pbp[iet]->Clone(Form("h2D_corrY_NP_Pbp_%d",iet));
		h2D_corrY_NP_Pbp[iet]->SetName(Form("h2D_corrY_NP_Pbp_%d",iet));
		h2D_corrY_NP_Pbp[iet]->Divide(h2D_Acc_pt_y_NPMC_Pbp[iet]);
		h2D_corrY_NP_Pbp[iet]->Divide(h2D_Eff_pt_y_NPMC_Pbp[iet]);
		h2D_corrY_PR_pPb[iet] = (TH2D*)h2D_fit_pt_y_PR_pPb[iet]->Clone(Form("h2D_corrY_PR_pPb_%d",iet));
		h2D_corrY_PR_pPb[iet]->SetName(Form("h2D_corrY_PR_pPb_%d",iet));
		h2D_corrY_PR_pPb[iet]->Divide(h2D_Acc_pt_y_PRMC_pPb[iet]);
		h2D_corrY_PR_pPb[iet]->Divide(h2D_Eff_pt_y_PRMC_pPb[iet]);
		h2D_corrY_NP_pPb[iet] = (TH2D*)h2D_fit_pt_y_NP_pPb[iet]->Clone(Form("h2D_corrY_NP_pPb_%d",iet));
		h2D_corrY_NP_pPb[iet]->SetName(Form("h2D_corrY_NP_pPb_%d",iet));
		h2D_corrY_NP_pPb[iet]->Divide(h2D_Acc_pt_y_NPMC_pPb[iet]);
		h2D_corrY_NP_pPb[iet]->Divide(h2D_Eff_pt_y_NPMC_pPb[iet]);
	
		h2D_MCcorrY_PR_Pbp[iet] = (TH2D*)h2D_Eff_Num_pt_y_PRMC_Pbp[iet]->Clone(Form("h2D_MCcorrY_PR_Pbp_%d",iet));
		h2D_MCcorrY_PR_Pbp[iet]->SetName(Form("h2D_MCcorrY_PR_Pbp_%d",iet));
		h2D_MCcorrY_PR_Pbp[iet]->Divide(h2D_Acc_pt_y_PRMC_Pbp[iet]);
		h2D_MCcorrY_PR_Pbp[iet]->Divide(h2D_Eff_pt_y_PRMC_Pbp[iet]);
		h2D_MCcorrY_NP_Pbp[iet] = (TH2D*)h2D_Eff_Num_pt_y_NPMC_Pbp[iet]->Clone(Form("h2D_MCcorrY_NP_Pbp_%d",iet));
		h2D_MCcorrY_NP_Pbp[iet]->SetName(Form("h2D_MCcorrY_NP_Pbp_%d",iet));
		h2D_MCcorrY_NP_Pbp[iet]->Divide(h2D_Acc_pt_y_NPMC_Pbp[iet]);
		h2D_MCcorrY_NP_Pbp[iet]->Divide(h2D_Eff_pt_y_NPMC_Pbp[iet]);
		h2D_MCcorrY_PR_pPb[iet] = (TH2D*)h2D_Eff_Num_pt_y_PRMC_pPb[iet]->Clone(Form("h2D_MCcorrY_PR_pPb_%d",iet));
		h2D_MCcorrY_PR_pPb[iet]->SetName(Form("h2D_MCcorrY_PR_pPb_%d",iet));
		h2D_MCcorrY_PR_pPb[iet]->Divide(h2D_Acc_pt_y_PRMC_pPb[iet]);
		h2D_MCcorrY_PR_pPb[iet]->Divide(h2D_Eff_pt_y_PRMC_pPb[iet]);
		h2D_MCcorrY_NP_pPb[iet] = (TH2D*)h2D_Eff_Num_pt_y_NPMC_pPb[iet]->Clone(Form("h2D_MCcorrY_NP_pPb_%d",iet));
		h2D_MCcorrY_NP_pPb[iet]->SetName(Form("h2D_MCcorrY_NP_pPb_%d",iet));
		h2D_MCcorrY_NP_pPb[iet]->Divide(h2D_Acc_pt_y_NPMC_pPb[iet]);
		h2D_MCcorrY_NP_pPb[iet]->Divide(h2D_Eff_pt_y_NPMC_pPb[iet]);
	}	
	
	
	////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("total2Dhist_%s.root",dirName),"RECREATE");
	std::cout << "dirName: " << dirName << std::endl;
	cout << "total2Dhist_"<<dirName<<".root has been created :) " <<endl;	

	// DIRECTORY : num, den distributions for acc & eff
	TDirectoryFile *otherMCInfo = new TDirectoryFile("otherMCInfo","otherMCInfo");
	// DIRECTORY : other information for fittng
	TDirectoryFile *otherFitInfo = new TDirectoryFile("otherFitInfo","otherFitInfo");
	
	for (int iet=0; iet<nET; iet++) {
		outFile->cd();
		//Acc	
		h2D_Acc_pt_y_PRMC_Pbp[iet]->Write();
		h2D_Acc_pt_y_PRMC_pPb[iet]->Write();
		h2D_Acc_pt_y_NPMC_Pbp[iet]->Write();
		h2D_Acc_pt_y_NPMC_pPb[iet]->Write();
		//Eff
		h2D_Eff_pt_y_PRMC_Pbp[iet]->Write();
		h2D_Eff_pt_y_PRMC_pPb[iet]->Write();
		h2D_Eff_pt_y_NPMC_Pbp[iet]->Write();
		h2D_Eff_pt_y_NPMC_pPb[iet]->Write();
		//raw yield
		h2D_fit_pt_y_PR_Pbp[iet]->Write();	
		h2D_fit_pt_y_PR_pPb[iet]->Write();	
		h2D_fit_pt_y_NP_Pbp[iet]->Write();	
		h2D_fit_pt_y_NP_pPb[iet]->Write();	
		//corrYield
		h2D_corrY_PR_Pbp[iet]->Write();	
		h2D_corrY_PR_pPb[iet]->Write();	
		h2D_corrY_NP_Pbp[iet]->Write();	
		h2D_corrY_NP_pPb[iet]->Write();	
		
		outFile->cd();
		otherMCInfo->cd();
		// acc den & num
		h2D_Acc_Den_pt_y_PRMC_Pbp[iet]->Write();
		h2D_Acc_Den_pt_y_PRMC_pPb[iet]->Write();
		h2D_Acc_Den_pt_y_NPMC_Pbp[iet]->Write();
		h2D_Acc_Den_pt_y_NPMC_pPb[iet]->Write();
		h2D_Acc_Num_pt_y_PRMC_Pbp[iet]->Write();
		h2D_Acc_Num_pt_y_PRMC_pPb[iet]->Write();
		h2D_Acc_Num_pt_y_NPMC_Pbp[iet]->Write();
		h2D_Acc_Num_pt_y_NPMC_pPb[iet]->Write();
		// eff den & num
		h2D_Eff_Den_pt_y_PRMC_Pbp[iet]->Write();
		h2D_Eff_Den_pt_y_PRMC_pPb[iet]->Write();
		h2D_Eff_Den_pt_y_NPMC_Pbp[iet]->Write();
		h2D_Eff_Den_pt_y_NPMC_pPb[iet]->Write();
		h2D_Eff_Num_pt_y_PRMC_Pbp[iet]->Write();
		h2D_Eff_Num_pt_y_PRMC_pPb[iet]->Write();
		h2D_Eff_Num_pt_y_NPMC_Pbp[iet]->Write();
		h2D_Eff_Num_pt_y_NPMC_pPb[iet]->Write();
		// MC corrYield
		h2D_MCcorrY_PR_Pbp[iet]->Write();	
		h2D_MCcorrY_PR_pPb[iet]->Write();	
		h2D_MCcorrY_NP_Pbp[iet]->Write();	
		h2D_MCcorrY_NP_pPb[iet]->Write();	

		outFile->cd();
		otherFitInfo->cd();
		h2D_fit_pt_y_nSig_Pbp[iet]->Write();	
		h2D_fit_pt_y_nSig_pPb[iet]->Write();	
		h2D_fit_pt_y_nBkg_Pbp[iet]->Write();	
		h2D_fit_pt_y_nBkg_pPb[iet]->Write();	
		h2D_fit_pt_y_bFrac_Pbp[iet]->Write();	
		h2D_fit_pt_y_bFrac_pPb[iet]->Write();	
		h2D_fit_pt_y_ctErrmin_Pbp[iet]->Write();	
		h2D_fit_pt_y_ctErrmin_pPb[iet]->Write();	
		h2D_fit_pt_y_ctErrmax_Pbp[iet]->Write();	
		h2D_fit_pt_y_ctErrmax_pPb[iet]->Write();	
	}		
	otherMCInfo->Write();
	otherFitInfo->Write();
	outFile->Close();

	return 0;

} // end of main func

