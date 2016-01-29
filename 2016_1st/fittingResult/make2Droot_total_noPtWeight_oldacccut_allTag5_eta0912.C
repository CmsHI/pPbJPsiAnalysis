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
int make2Droot_total_noPtWeight_oldacccut_allTag5_eta0912(char *dirName = "8rap3pt", char *dateName = "no"){

  using namespace std;

	////////////////////////////////////////////////
	////// read in Acceptance file
	// *** without pt weight
	TFile *fAccPR = new TFile(Form("../Acceptance/AccAna_%s_PRMC_boosted_oldacccut_eta0912.root",dirName));
	TFile *fAccNP = new TFile(Form("../Acceptance/AccAna_%s_NPMC_boosted_oldacccut_eta0912.root",dirName));
	TH2D* h2D_Acc_pt_y_PR_Pbp = (TH2D*)fAccPR->Get("h2D_Acc_pt_y_Pbp");
	TH2D* h2D_Acc_pt_y_PR_pPb = (TH2D*)fAccPR->Get("h2D_Acc_pt_y_pPb");
	TH2D* h2D_Acc_pt_y_NP_Pbp = (TH2D*)fAccNP->Get("h2D_Acc_pt_y_Pbp");
	TH2D* h2D_Acc_pt_y_NP_pPb = (TH2D*)fAccNP->Get("h2D_Acc_pt_y_pPb");
	h2D_Acc_pt_y_PR_Pbp->SetName("h2D_Acc_pt_y_PR_Pbp");
	h2D_Acc_pt_y_PR_pPb->SetName("h2D_Acc_pt_y_PR_pPb");
	h2D_Acc_pt_y_NP_Pbp->SetName("h2D_Acc_pt_y_NP_Pbp");
	h2D_Acc_pt_y_NP_pPb->SetName("h2D_Acc_pt_y_NP_pPb");
	cout << "4 Acc TH2D : "<<h2D_Acc_pt_y_PR_Pbp<<" "<<h2D_Acc_pt_y_PR_pPb<<" "<<h2D_Acc_pt_y_NP_Pbp<<" "<<h2D_Acc_pt_y_NP_pPb<<endl;
	
	TH2D* h2D_Acc_Den_pt_y_PR_Pbp = (TH2D*)fAccPR->Get("h2D_Den_pt_y_Pbp");
	TH2D* h2D_Acc_Den_pt_y_PR_pPb = (TH2D*)fAccPR->Get("h2D_Den_pt_y_pPb"); 
	TH2D* h2D_Acc_Den_pt_y_NP_Pbp = (TH2D*)fAccNP->Get("h2D_Den_pt_y_Pbp"); 
	TH2D* h2D_Acc_Den_pt_y_NP_pPb = (TH2D*)fAccNP->Get("h2D_Den_pt_y_pPb"); 
	h2D_Acc_Den_pt_y_PR_Pbp->SetName("h2D_Acc_Den_pt_y_PR_Pbp");
	h2D_Acc_Den_pt_y_PR_pPb->SetName("h2D_Acc_Den_pt_y_PR_pPb");
	h2D_Acc_Den_pt_y_NP_Pbp->SetName("h2D_Acc_Den_pt_y_NP_Pbp");
	h2D_Acc_Den_pt_y_NP_pPb->SetName("h2D_Acc_Den_pt_y_NP_pPb");
	
	TH2D* h2D_Acc_Num_pt_y_PR_Pbp = (TH2D*)fAccPR->Get("h2D_Num_pt_y_Pbp");
	TH2D* h2D_Acc_Num_pt_y_PR_pPb = (TH2D*)fAccPR->Get("h2D_Num_pt_y_pPb");
	TH2D* h2D_Acc_Num_pt_y_NP_Pbp = (TH2D*)fAccNP->Get("h2D_Num_pt_y_Pbp");  
	TH2D* h2D_Acc_Num_pt_y_NP_pPb = (TH2D*)fAccNP->Get("h2D_Num_pt_y_pPb");  
	h2D_Acc_Num_pt_y_PR_Pbp->SetName("h2D_Acc_Num_pt_y_PR_Pbp");
	h2D_Acc_Num_pt_y_PR_pPb->SetName("h2D_Acc_Num_pt_y_PR_pPb");
	h2D_Acc_Num_pt_y_NP_Pbp->SetName("h2D_Acc_Num_pt_y_NP_Pbp");
	h2D_Acc_Num_pt_y_NP_pPb->SetName("h2D_Acc_Num_pt_y_NP_pPb");


	////////////////////////////////////////////////
	////// read in Efficiency file
	// *** without pt weight
	TFile *fEffPRPbp = new TFile(Form("../Efficiency/EffAna_%s_PRMCpythia_Pbp_CtErr1_SF1_Zvtx1_oldacccut_allTag5_eta0912.root",dirName));
	TFile *fEffPRpPb = new TFile(Form("../Efficiency/EffAna_%s_PRMCpythia_pPb_CtErr1_SF1_Zvtx1_oldacccut_allTag5_eta0912.root",dirName));
	TFile *fEffNPPbp = new TFile(Form("../Efficiency/EffAna_%s_NPMCpythia_Pbp_CtErr1_SF1_Zvtx1_oldacccut_allTag5_eta0912.root",dirName));
	TFile *fEffNPpPb = new TFile(Form("../Efficiency/EffAna_%s_NPMCpythia_pPb_CtErr1_SF1_Zvtx1_oldacccut_allTag5_eta0912.root",dirName));
	TH2D* h2D_Eff_pt_y_PR_Pbp = (TH2D*)fEffPRPbp->Get("h2D_Eff_pt_y"); 
	TH2D* h2D_Eff_pt_y_PR_pPb = (TH2D*)fEffPRpPb->Get("h2D_Eff_pt_y");
	TH2D* h2D_Eff_pt_y_NP_Pbp = (TH2D*)fEffNPPbp->Get("h2D_Eff_pt_y"); 
	TH2D* h2D_Eff_pt_y_NP_pPb = (TH2D*)fEffNPpPb->Get("h2D_Eff_pt_y");
	h2D_Eff_pt_y_PR_Pbp->SetName("h2D_Eff_pt_y_PR_Pbp");
	h2D_Eff_pt_y_PR_pPb->SetName("h2D_Eff_pt_y_PR_pPb");
	h2D_Eff_pt_y_NP_Pbp->SetName("h2D_Eff_pt_y_NP_Pbp");
	h2D_Eff_pt_y_NP_pPb->SetName("h2D_Eff_pt_y_NP_pPb");
	cout << "4 Eff TH2D : "<<h2D_Eff_pt_y_PR_Pbp<<" "<<h2D_Eff_pt_y_PR_pPb<<" "<<h2D_Eff_pt_y_NP_Pbp<<" "<<h2D_Eff_pt_y_NP_pPb<<endl;
	TH2D* h2D_Eff_Den_pt_y_PR_Pbp = (TH2D*)fEffPRPbp->Get("h2D_Den_pt_y");
	TH2D* h2D_Eff_Den_pt_y_PR_pPb = (TH2D*)fEffPRpPb->Get("h2D_Den_pt_y");
	TH2D* h2D_Eff_Den_pt_y_NP_Pbp = (TH2D*)fEffNPPbp->Get("h2D_Den_pt_y");
	TH2D* h2D_Eff_Den_pt_y_NP_pPb = (TH2D*)fEffNPpPb->Get("h2D_Den_pt_y");
	h2D_Eff_Den_pt_y_PR_Pbp->SetName("h2D_Eff_Den_pt_y_PR_Pbp");
	h2D_Eff_Den_pt_y_PR_pPb->SetName("h2D_Eff_Den_pt_y_PR_pPb");
	h2D_Eff_Den_pt_y_NP_Pbp->SetName("h2D_Eff_Den_pt_y_NP_Pbp");
	h2D_Eff_Den_pt_y_NP_pPb->SetName("h2D_Eff_Den_pt_y_NP_pPb");
	
	TH2D* h2D_Eff_Num_pt_y_PR_Pbp = (TH2D*)fEffPRPbp->Get("h2D_Num_pt_y");
	TH2D* h2D_Eff_Num_pt_y_PR_pPb = (TH2D*)fEffPRpPb->Get("h2D_Num_pt_y");
	TH2D* h2D_Eff_Num_pt_y_NP_Pbp = (TH2D*)fEffNPPbp->Get("h2D_Num_pt_y");
	TH2D* h2D_Eff_Num_pt_y_NP_pPb = (TH2D*)fEffNPpPb->Get("h2D_Num_pt_y");
	h2D_Eff_Num_pt_y_PR_Pbp->SetName("h2D_Eff_Num_pt_y_PR_Pbp");
	h2D_Eff_Num_pt_y_PR_pPb->SetName("h2D_Eff_Num_pt_y_PR_pPb");
	h2D_Eff_Num_pt_y_NP_Pbp->SetName("h2D_Eff_Num_pt_y_NP_Pbp");
	h2D_Eff_Num_pt_y_NP_pPb->SetName("h2D_Eff_Num_pt_y_NP_pPb");
	
	////////////////////////////////////////////////
	////// read in fromfit file
	TFile* fFitPbp = new TFile(Form("./2Dhist_fitRes_%s_oldacccut_eta0912_Pbp.root",dirName));
	TFile* fFitpPb = new TFile(Form("./2Dhist_fitRes_%s_oldacccut_eta0912_pPb.root",dirName));
	TH2D* h2D_fit_pt_y_PR_Pbp = (TH2D*)fFitPbp->Get("h2D_nPrompt");	
	TH2D* h2D_fit_pt_y_NP_Pbp = (TH2D*)fFitPbp->Get("h2D_nNonprompt");	
	TH2D* h2D_fit_pt_y_nSig_Pbp = (TH2D*)fFitPbp->Get("h2D_nSig");	
	TH2D* h2D_fit_pt_y_nBkg_Pbp = (TH2D*)fFitPbp->Get("h2D_nBkg");	
	TH2D* h2D_fit_pt_y_bFrac_Pbp = (TH2D*)fFitPbp->Get("h2D_bFraction");	
	TH2D* h2D_fit_pt_y_ctErrmin_Pbp = (TH2D*)fFitPbp->Get("h2D_ctErrmin");	
	TH2D* h2D_fit_pt_y_ctErrmax_Pbp = (TH2D*)fFitPbp->Get("h2D_ctErrmax");	
	TH2D* h2D_fit_pt_y_PR_pPb = (TH2D*)fFitpPb->Get("h2D_nPrompt");	
	TH2D* h2D_fit_pt_y_NP_pPb = (TH2D*)fFitpPb->Get("h2D_nNonprompt");	
	TH2D* h2D_fit_pt_y_nSig_pPb = (TH2D*)fFitpPb->Get("h2D_nSig");	
	TH2D* h2D_fit_pt_y_nBkg_pPb = (TH2D*)fFitpPb->Get("h2D_nBkg");	
	TH2D* h2D_fit_pt_y_bFrac_pPb = (TH2D*)fFitpPb->Get("h2D_bFraction");	
	TH2D* h2D_fit_pt_y_ctErrmin_pPb = (TH2D*)fFitpPb->Get("h2D_ctErrmin");	
	TH2D* h2D_fit_pt_y_ctErrmax_pPb = (TH2D*)fFitpPb->Get("h2D_ctErrmax");	
	h2D_fit_pt_y_PR_Pbp->SetName("h2D_fit_pt_y_PR_Pbp");
	h2D_fit_pt_y_PR_pPb->SetName("h2D_fit_pt_y_PR_pPb");
	h2D_fit_pt_y_NP_Pbp->SetName("h2D_fit_pt_y_NP_Pbp");
	h2D_fit_pt_y_NP_pPb->SetName("h2D_fit_pt_y_NP_pPb");
	h2D_fit_pt_y_nSig_Pbp->SetName("h2D_fit_pt_y_nSig_Pbp");
	h2D_fit_pt_y_nSig_pPb->SetName("h2D_fit_pt_y_nSig_pPb");
	h2D_fit_pt_y_nBkg_Pbp->SetName("h2D_fit_pt_y_nBkg_Pbp");
	h2D_fit_pt_y_nBkg_pPb->SetName("h2D_fit_pt_y_nBkg_pPb");
	h2D_fit_pt_y_bFrac_Pbp->SetName("h2D_fit_pt_y_bFrac_Pbp");
	h2D_fit_pt_y_bFrac_pPb->SetName("h2D_fit_pt_y_bFrac_pPb");
	h2D_fit_pt_y_ctErrmin_Pbp->SetName("h2D_fit_pt_y_ctErrmin_Pbp");
	h2D_fit_pt_y_ctErrmin_pPb->SetName("h2D_fit_pt_y_ctErrmin_pPb");
	h2D_fit_pt_y_ctErrmax_Pbp->SetName("h2D_fit_pt_y_ctErrmax_Pbp");
	h2D_fit_pt_y_ctErrmax_pPb->SetName("h2D_fit_pt_y_ctErrmax_pPb");
	
	cout << "4 yield TH2D : "<<h2D_fit_pt_y_PR_Pbp<<" "<<h2D_fit_pt_y_PR_pPb<<" "<<h2D_fit_pt_y_NP_Pbp<<" "<<h2D_fit_pt_y_NP_pPb<<endl;
	cout << "2 nSig TH2D : "<<h2D_fit_pt_y_nSig_Pbp<<" "<<h2D_fit_pt_y_nSig_pPb<<endl;
	cout << "2 nBkg TH2D : "<<h2D_fit_pt_y_nBkg_Pbp<<" "<<h2D_fit_pt_y_nBkg_pPb<<endl;
	cout << "2 bFrac TH2D : "<<h2D_fit_pt_y_bFrac_Pbp<<" "<<h2D_fit_pt_y_bFrac_pPb<<endl;
	cout << "2 ctErrmin TH2D : "<<h2D_fit_pt_y_ctErrmin_Pbp<<" "<<h2D_fit_pt_y_ctErrmin_pPb<<endl;
	cout << "2 ctErrmax TH2D : "<<h2D_fit_pt_y_ctErrmax_Pbp<<" "<<h2D_fit_pt_y_ctErrmax_pPb<<endl;
	
	////////////////////////////////////////////////
	////// data corrected yield = rawYield/(Acc*Eff)
	TH2D* h2D_corrY_PR_Pbp = (TH2D*)h2D_fit_pt_y_PR_Pbp->Clone("h2D_corrY_PR_Pbp");
	h2D_corrY_PR_Pbp->SetName("h2D_corrY_PR_Pbp");
	h2D_corrY_PR_Pbp->Divide(h2D_Acc_pt_y_PR_Pbp);
	h2D_corrY_PR_Pbp->Divide(h2D_Eff_pt_y_PR_Pbp);
	TH2D* h2D_corrY_NP_Pbp = (TH2D*)h2D_fit_pt_y_NP_Pbp->Clone("h2D_corrY_NP_Pbp");
	h2D_corrY_NP_Pbp->SetName("h2D_corrY_NP_Pbp");
	h2D_corrY_NP_Pbp->Divide(h2D_Acc_pt_y_NP_Pbp);
	h2D_corrY_NP_Pbp->Divide(h2D_Eff_pt_y_NP_Pbp);
	TH2D* h2D_corrY_PR_pPb = (TH2D*)h2D_fit_pt_y_PR_pPb->Clone("h2D_corrY_PR_pPb");
	h2D_corrY_PR_pPb->SetName("h2D_corrY_PR_pPb");
	h2D_corrY_PR_pPb->Divide(h2D_Acc_pt_y_PR_pPb);
	h2D_corrY_PR_pPb->Divide(h2D_Eff_pt_y_PR_pPb);
	TH2D* h2D_corrY_NP_pPb = (TH2D*)h2D_fit_pt_y_NP_pPb->Clone("h2D_corrY_NP_pPb");
	h2D_corrY_NP_pPb->SetName("h2D_corrY_NP_pPb");
	h2D_corrY_NP_pPb->Divide(h2D_Acc_pt_y_NP_pPb);
	h2D_corrY_NP_pPb->Divide(h2D_Eff_pt_y_NP_pPb);
	
	////////////////////////////////////////////////
	////// MC corrected yield RECO/(Acc*Eff)
	TH2D* h2D_MCcorrY_PR_Pbp = (TH2D*)h2D_Eff_Num_pt_y_PR_Pbp->Clone("h2D_MCcorrY_PR_Pbp");
	h2D_MCcorrY_PR_Pbp->SetName("h2D_MCcorrY_PR_Pbp");
	h2D_MCcorrY_PR_Pbp->Divide(h2D_Acc_pt_y_PR_Pbp);
	h2D_MCcorrY_PR_Pbp->Divide(h2D_Eff_pt_y_PR_Pbp);
	TH2D* h2D_MCcorrY_NP_Pbp = (TH2D*)h2D_Eff_Num_pt_y_NP_Pbp->Clone("h2D_MCcorrY_NP_Pbp");
	h2D_MCcorrY_NP_Pbp->SetName("h2D_MCcorrY_NP_Pbp");
	h2D_MCcorrY_NP_Pbp->Divide(h2D_Acc_pt_y_NP_Pbp);
	h2D_MCcorrY_NP_Pbp->Divide(h2D_Eff_pt_y_NP_Pbp);
	TH2D* h2D_MCcorrY_PR_pPb = (TH2D*)h2D_Eff_Num_pt_y_PR_pPb->Clone("h2D_MCcorrY_PR_pPb");
	h2D_MCcorrY_PR_pPb->SetName("h2D_MCcorrY_PR_pPb");
	h2D_MCcorrY_PR_pPb->Divide(h2D_Acc_pt_y_PR_pPb);
	h2D_MCcorrY_PR_pPb->Divide(h2D_Eff_pt_y_PR_pPb);
	TH2D* h2D_MCcorrY_NP_pPb = (TH2D*)h2D_Eff_Num_pt_y_NP_pPb->Clone("h2D_MCcorrY_NP_pPb");
	h2D_MCcorrY_NP_pPb->SetName("h2D_MCcorrY_NP_pPb");
	h2D_MCcorrY_NP_pPb->Divide(h2D_Acc_pt_y_NP_pPb);
	h2D_MCcorrY_NP_pPb->Divide(h2D_Eff_pt_y_NP_pPb);
	
	
	////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("total2Dhist_%s_noPtWeight_oldacccut_allTag5_eta0912.root",dirName),"RECREATE");
	std::cout << "dirName: " << dirName << std::endl;
	cout << "total2Dhist_"<<dirName<<"_noPtWeight_oldacccut_allTag5_eta0912.root has been created :) " <<endl;	

	outFile->cd();
	//Acc	
	h2D_Acc_pt_y_PR_Pbp->Write();
	h2D_Acc_pt_y_PR_pPb->Write();
	h2D_Acc_pt_y_NP_Pbp->Write();
	h2D_Acc_pt_y_NP_pPb->Write();
	//Eff
	h2D_Eff_pt_y_PR_Pbp->Write();
	h2D_Eff_pt_y_PR_pPb->Write();
	h2D_Eff_pt_y_NP_Pbp->Write();
	h2D_Eff_pt_y_NP_pPb->Write();
	//raw yield
	h2D_fit_pt_y_PR_Pbp->Write();	
	h2D_fit_pt_y_PR_pPb->Write();	
	h2D_fit_pt_y_NP_Pbp->Write();	
	h2D_fit_pt_y_NP_pPb->Write();	
	//corrYield
	h2D_corrY_PR_Pbp->Write();	
	h2D_corrY_PR_pPb->Write();	
	h2D_corrY_NP_Pbp->Write();	
	h2D_corrY_NP_pPb->Write();	
	
	// DIRECTORY : num, den distributions for acc & eff
	TDirectoryFile *otherMCInfo = new TDirectoryFile("otherMCInfo","otherMCInfo");
	otherMCInfo->cd();
	// acc den & num
	h2D_Acc_Den_pt_y_PR_Pbp->Write();
	h2D_Acc_Den_pt_y_PR_pPb->Write();
	h2D_Acc_Den_pt_y_NP_Pbp->Write();
	h2D_Acc_Den_pt_y_NP_pPb->Write();
	h2D_Acc_Num_pt_y_PR_Pbp->Write();
	h2D_Acc_Num_pt_y_PR_pPb->Write();
	h2D_Acc_Num_pt_y_NP_Pbp->Write();
	h2D_Acc_Num_pt_y_NP_pPb->Write();
	// eff den & num
	h2D_Eff_Den_pt_y_PR_Pbp->Write();
	h2D_Eff_Den_pt_y_PR_pPb->Write();
	h2D_Eff_Den_pt_y_NP_Pbp->Write();
	h2D_Eff_Den_pt_y_NP_pPb->Write();
	h2D_Eff_Num_pt_y_PR_Pbp->Write();
	h2D_Eff_Num_pt_y_PR_pPb->Write();
	h2D_Eff_Num_pt_y_NP_Pbp->Write();
	h2D_Eff_Num_pt_y_NP_pPb->Write();
	// MC corrYield
	h2D_MCcorrY_PR_Pbp->Write();	
	h2D_MCcorrY_PR_pPb->Write();	
	h2D_MCcorrY_NP_Pbp->Write();	
	h2D_MCcorrY_NP_pPb->Write();	
	otherMCInfo->Write();
	
	// DIRECTORY : other information for fittng
	outFile->cd();
	TDirectoryFile *otherFitInfo = new TDirectoryFile("otherFitInfo","otherFitInfo");
	otherFitInfo->cd();
	h2D_fit_pt_y_nSig_Pbp->Write();	
	h2D_fit_pt_y_nSig_pPb->Write();	
	h2D_fit_pt_y_nBkg_Pbp->Write();	
	h2D_fit_pt_y_nBkg_pPb->Write();	
	h2D_fit_pt_y_bFrac_Pbp->Write();	
	h2D_fit_pt_y_bFrac_pPb->Write();	
	h2D_fit_pt_y_ctErrmin_Pbp->Write();	
	h2D_fit_pt_y_ctErrmin_pPb->Write();	
	h2D_fit_pt_y_ctErrmax_Pbp->Write();	
	h2D_fit_pt_y_ctErrmax_pPb->Write();	
	otherFitInfo->Write();
		
	outFile->Close();
	
	return 0;

} // end of main func

