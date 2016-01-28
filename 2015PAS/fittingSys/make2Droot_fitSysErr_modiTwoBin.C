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
int make2Droot_fitSysErr_modiTwoBin(char *dirName = "8rap9pt2gev"){

  using namespace std;

	const int nybin = 8;
	const int nptbin = 9;
	const int nSubSysTmp = 10;

	TString sysOpt[] = {"01","02","03","04"};
	//const int nSubSys[] = {6,1,2,1};
	const int nSubSys[] = {6,1,2,1};
	
	const int nOpt = sizeof(sysOpt)/sizeof(TString);
	cout << "nOpt = " << nOpt << endl;

	////////////////////////////////////////////////
	////// read in fromfit file :: default!!
	TFile* fFitPbp = new TFile(Form("./2Dhist_fitRes_%s_Pbp.root",dirName));
	TFile* fFitpPb = new TFile(Form("./2Dhist_fitRes_%s_pPb.root",dirName));
	TH2D* h2D_PR_Pbp = (TH2D*)fFitPbp->Get("h2D_nPrompt");	
	TH2D* h2D_NP_Pbp = (TH2D*)fFitPbp->Get("h2D_nNonprompt");	
	TH2D* h2D_PR_pPb = (TH2D*)fFitpPb->Get("h2D_nPrompt");	
	TH2D* h2D_NP_pPb = (TH2D*)fFitpPb->Get("h2D_nNonprompt");	
	h2D_PR_Pbp->SetName("h2D_PR_Pbp");
	h2D_PR_pPb->SetName("h2D_PR_pPb");
	h2D_NP_Pbp->SetName("h2D_NP_Pbp");
	h2D_NP_pPb->SetName("h2D_NP_pPb");
	
	////////////////////////////////////////////////
	////// systematics!
	TFile* fFitPbp_sys[nOpt][nSubSysTmp];	
	TFile* fFitpPb_sys[nOpt][nSubSysTmp];	
	TH2D* h2D_PR_Pbp_sys[nOpt][nSubSysTmp];
	TH2D* h2D_PR_pPb_sys[nOpt][nSubSysTmp];
	TH2D* h2D_NP_Pbp_sys[nOpt][nSubSysTmp];
	TH2D* h2D_NP_pPb_sys[nOpt][nSubSysTmp];
/*
	TH2D* h2D_PR_Pbp_diff[nOpt][nSubSysTmp];
	TH2D* h2D_PR_pPb_diff[nOpt][nSubSysTmp];
	TH2D* h2D_NP_Pbp_diff[nOpt][nSubSysTmp];
	TH2D* h2D_NP_pPb_diff[nOpt][nSubSysTmp];
	TH2D* h2D_PR_Pbp_err[nOpt][nSubSysTmp];
	TH2D* h2D_PR_pPb_err[nOpt][nSubSysTmp];
	TH2D* h2D_NP_Pbp_err[nOpt][nSubSysTmp];
	TH2D* h2D_NP_pPb_err[nOpt][nSubSysTmp];
*/
	
	for (int iopt=0; iopt<nOpt; iopt++){
		for (int isys=0; isys<nSubSys[iopt]; isys++){
			fFitPbp_sys[iopt][isys] = new TFile(Form("./2Dhist_fitRes_%s_sys%s_0%d_Pbp.root",dirName,sysOpt[iopt].Data(),isys+1));	
			fFitpPb_sys[iopt][isys] = new TFile(Form("./2Dhist_fitRes_%s_sys%s_0%d_pPb.root",dirName,sysOpt[iopt].Data(),isys+1));	
			h2D_PR_Pbp_sys[iopt][isys] = (TH2D*)fFitPbp_sys[iopt][isys]->Get("h2D_nPrompt");	
			h2D_NP_Pbp_sys[iopt][isys] = (TH2D*)fFitPbp_sys[iopt][isys]->Get("h2D_nNonprompt");	
			h2D_PR_pPb_sys[iopt][isys] = (TH2D*)fFitpPb_sys[iopt][isys]->Get("h2D_nPrompt");	
			h2D_NP_pPb_sys[iopt][isys] = (TH2D*)fFitpPb_sys[iopt][isys]->Get("h2D_nNonprompt");	
			h2D_PR_Pbp_sys[iopt][isys]->SetName(Form("h2D_PR_Pbp_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_PR_pPb_sys[iopt][isys]->SetName(Form("h2D_PR_pPb_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_NP_Pbp_sys[iopt][isys]->SetName(Form("h2D_NP_Pbp_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_NP_pPb_sys[iopt][isys]->SetName(Form("h2D_NP_pPb_sys%s_%d",sysOpt[iopt].Data(),isys));
			cout << "* sysOpt "<<iopt<<":: "<<isys<< "th 4 yield TH2D : "<<h2D_PR_Pbp_sys[iopt][isys]<<" "<<h2D_PR_pPb_sys[iopt][isys]<<" "<<h2D_NP_Pbp_sys[iopt][isys]<<" "<<h2D_NP_pPb_sys[iopt][isys]<<endl;
/*
			// difference and relative error calculation
			h2D_PR_Pbp_diff[iopt][isys] = (TH2D*)h2D_PR_Pbp->Clone(Form("h2D_PR_Pbp_diff_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_PR_pPb_diff[iopt][isys] = (TH2D*)h2D_PR_pPb->Clone(Form("h2D_PR_pPb_diff_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_NP_Pbp_diff[iopt][isys] = (TH2D*)h2D_NP_Pbp->Clone(Form("h2D_NP_Pbp_diff_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_NP_pPb_diff[iopt][isys] = (TH2D*)h2D_NP_pPb->Clone(Form("h2D_NP_pPb_diff_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_PR_Pbp_diff[iopt][isys]->Add(h2D_PR_Pbp_sys[iopt][isys],-1);
			h2D_PR_pPb_diff[iopt][isys]->Add(h2D_PR_pPb_sys[iopt][isys],-1);
			h2D_NP_Pbp_diff[iopt][isys]->Add(h2D_NP_Pbp_sys[iopt][isys],-1);
			h2D_NP_pPb_diff[iopt][isys]->Add(h2D_NP_pPb_sys[iopt][isys],-1);
			h2D_PR_Pbp_err[iopt][isys] = (TH2D*)h2D_PR_Pbp_diff[iopt][isys]->Clone(Form("h2D_PR_Pbp_err_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_PR_pPb_err[iopt][isys] = (TH2D*)h2D_PR_pPb_diff[iopt][isys]->Clone(Form("h2D_PR_pPb_err_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_NP_Pbp_err[iopt][isys] = (TH2D*)h2D_NP_Pbp_diff[iopt][isys]->Clone(Form("h2D_NP_Pbp_err_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_NP_pPb_err[iopt][isys] = (TH2D*)h2D_NP_pPb_diff[iopt][isys]->Clone(Form("h2D_NP_pPb_err_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_PR_Pbp_err[iopt][isys]->Divide(h2D_PR_Pbp);
			h2D_PR_pPb_err[iopt][isys]->Divide(h2D_PR_pPb);
			h2D_NP_Pbp_err[iopt][isys]->Divide(h2D_NP_Pbp);
			h2D_NP_pPb_err[iopt][isys]->Divide(h2D_NP_pPb);
*/
		}
	}

	////////////////////////////////////////////////
	////// merge Pbp and pPb
	TH2D* h2D_PR_merge; //y binning in y_lab^1st
	TH2D* h2D_NP_merge;
	TH2D* h2D_PR_merge_sys[nOpt][nSubSysTmp];
	TH2D* h2D_NP_merge_sys[nOpt][nSubSysTmp];
	double tmpPRyieldPbp;	
	double tmpPRyieldpPb;	
	double tmpNPyieldPbp;	
	double tmpNPyieldpPb;	
	double tmpPRyield_default[nybin][nptbin];	
	double tmpNPyield_default[nybin][nptbin];	
	double tmpPRyield_sys[nOpt][nSubSysTmp][nybin][nptbin];	
	double tmpNPyield_sys[nOpt][nSubSysTmp][nybin][nptbin];	

	/////////// default
	h2D_PR_merge=(TH2D*)h2D_PR_Pbp->Clone("h2D_PR_merge");	//y binning in y_lab^1st
	h2D_NP_merge=(TH2D*)h2D_NP_Pbp->Clone("h2D_NP_merge");	
	
	for (int iy=0; iy<nybin;iy++ ){
		for (int ipt=0; ipt<nptbin;ipt++ ){
			tmpPRyield_default[iy][ipt]=0;
			tmpNPyield_default[iy][ipt]=0;
			tmpPRyieldPbp = h2D_PR_Pbp->GetBinContent(iy+1,ipt+1);	
			tmpPRyieldpPb = h2D_PR_pPb->GetBinContent(nybin-iy,ipt+1);
			tmpNPyieldPbp = h2D_NP_Pbp->GetBinContent(iy+1,ipt+1);	
			tmpNPyieldpPb = h2D_NP_pPb->GetBinContent(nybin-iy,ipt+1);
			//merge Pbp+pPb
			tmpPRyield_default[iy][ipt] = tmpPRyieldPbp+tmpPRyieldpPb; 
			tmpNPyield_default[iy][ipt] = tmpNPyieldPbp+tmpNPyieldpPb; 
			//fill into hist
			h2D_PR_merge->SetBinContent(iy+1,ipt+1,tmpPRyield_default[iy][ipt]);
			h2D_NP_merge->SetBinContent(iy+1,ipt+1,tmpNPyield_default[iy][ipt]);

		}
	}

	//////////// sys	
	for (int iopt=0; iopt<nOpt; iopt++){
		for (int isys=0; isys<nSubSys[iopt]; isys++){
			h2D_PR_merge_sys[iopt][isys]=(TH2D*)h2D_PR_Pbp_sys[iopt][isys]->Clone(Form("h2D_PR_merge_sys%s_%d",sysOpt[iopt].Data(),isys));	//y binning in y_lab^1st
			h2D_NP_merge_sys[iopt][isys]=(TH2D*)h2D_NP_Pbp_sys[iopt][isys]->Clone(Form("h2D_NP_merge_sys%s_%d",sysOpt[iopt].Data(),isys));	
			for (int iy=0; iy<nybin;iy++ ){
				for (int ipt=0; ipt<nptbin;ipt++ ){
					tmpPRyield_sys[iopt][isys][iy][ipt]=0;
					tmpNPyield_sys[iopt][isys][iy][ipt]=0;
					tmpPRyieldPbp=0; tmpPRyieldpPb=0;
					tmpNPyieldPbp=0; tmpNPyieldpPb=0;
					tmpPRyieldPbp = h2D_PR_Pbp_sys[iopt][isys]->GetBinContent(iy+1,ipt+1);	
					tmpPRyieldpPb = h2D_PR_pPb_sys[iopt][isys]->GetBinContent(nybin-iy,ipt+1);
					tmpNPyieldPbp = h2D_NP_Pbp_sys[iopt][isys]->GetBinContent(iy+1,ipt+1);	
					tmpNPyieldpPb = h2D_NP_pPb_sys[iopt][isys]->GetBinContent(nybin-iy,ipt+1);
					//merge Pbp+pPb
					tmpPRyield_sys[iopt][isys][iy][ipt] = tmpPRyieldPbp+tmpPRyieldpPb; 
					tmpNPyield_sys[iopt][isys][iy][ipt] = tmpNPyieldPbp+tmpNPyieldpPb; 
					//fill into hist
					h2D_PR_merge_sys[iopt][isys]->SetBinContent(iy+1,ipt+1,tmpPRyield_sys[iopt][isys][iy][ipt]);
					h2D_NP_merge_sys[iopt][isys]->SetBinContent(iy+1,ipt+1,tmpNPyield_sys[iopt][isys][iy][ipt]);
				}
			}
		}
	}

	////////////////////////////////////////////////
	// difference and relative error calculation
	TH2D* h2D_PR_diff[nOpt][nSubSysTmp];
	TH2D* h2D_NP_diff[nOpt][nSubSysTmp];
	TH2D* h2D_PR_err[nOpt][nSubSysTmp];
	TH2D* h2D_NP_err[nOpt][nSubSysTmp];
	
	for (int iopt=0; iopt<nOpt; iopt++){
		for (int isys=0; isys<nSubSys[iopt]; isys++){
			h2D_PR_diff[iopt][isys] = (TH2D*)h2D_PR_merge->Clone(Form("h2D_PR_diff_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_NP_diff[iopt][isys] = (TH2D*)h2D_NP_merge->Clone(Form("h2D_NP_diff_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_PR_diff[iopt][isys]->Add(h2D_PR_merge_sys[iopt][isys],-1);
			h2D_NP_diff[iopt][isys]->Add(h2D_NP_merge_sys[iopt][isys],-1);
			h2D_PR_err[iopt][isys] = (TH2D*)h2D_PR_diff[iopt][isys]->Clone(Form("h2D_PR_err_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_NP_err[iopt][isys] = (TH2D*)h2D_NP_diff[iopt][isys]->Clone(Form("h2D_NP_err_sys%s_%d",sysOpt[iopt].Data(),isys));
			h2D_PR_err[iopt][isys]->Divide(h2D_PR_merge);
			h2D_NP_err[iopt][isys]->Divide(h2D_NP_merge);
		}
	}

	////////////////////////////////////////////////
	////// select the largest difference among nSubSys
	
	TH2D* h2D_PR_maxdiff[nOpt]; //y binning in y_lab^1st
	TH2D* h2D_NP_maxdiff[nOpt];
	double tmpPRval;	
	double tmpNPval;	
	double tmpPRmaxdiff[nOpt][nybin][nptbin];	
	double tmpNPmaxdiff[nOpt][nybin][nptbin];	
		
	for (int iopt=0; iopt<nOpt; iopt++){
		h2D_PR_maxdiff[iopt] = (TH2D*)h2D_PR_diff[iopt][0]->Clone(Form("h2D_PR_maxdiff_sys%s",sysOpt[iopt].Data()));
		h2D_NP_maxdiff[iopt] = (TH2D*)h2D_NP_diff[iopt][0]->Clone(Form("h2D_NP_maxdiff_sys%s",sysOpt[iopt].Data()));
		for (int iy=0; iy<nybin;iy++ ){
			for (int ipt=0; ipt<nptbin;ipt++ ){
				tmpPRmaxdiff[iopt][iy][ipt]=0;
				tmpNPmaxdiff[iopt][iy][ipt]=0;
				for (int isys=0; isys<nSubSys[iopt]; isys++){
					tmpPRval=0;
					tmpNPval=0;
					tmpPRval = h2D_PR_diff[iopt][isys]->GetBinContent(iy+1,ipt+1);	
					tmpNPval = h2D_NP_diff[iopt][isys]->GetBinContent(iy+1,ipt+1);	
					tmpPRval = TMath::Abs(tmpPRval);
					tmpNPval = TMath::Abs(tmpNPval);
					if (tmpPRmaxdiff[iopt][iy][ipt] < tmpPRval) { tmpPRmaxdiff[iopt][iy][ipt] = tmpPRval; }
					if (tmpNPmaxdiff[iopt][iy][ipt] < tmpNPval) { tmpNPmaxdiff[iopt][iy][ipt] = tmpNPval; }
				}
				cout << iopt << "opt, maxdiff PR err for" <<iy<<"th y, "<<ipt<<"th pT = " << tmpPRmaxdiff[iopt][iy][ipt]<<endl;
				cout << iopt << "opt, maxdiff NP err for" <<iy<<"th y, "<<ipt<<"th pT = " << tmpNPmaxdiff[iopt][iy][ipt]<<endl;
				h2D_PR_maxdiff[iopt]->SetBinContent(iy+1,ipt+1,tmpPRmaxdiff[iopt][iy][ipt]);
				h2D_NP_maxdiff[iopt]->SetBinContent(iy+1,ipt+1,tmpNPmaxdiff[iopt][iy][ipt]);
			}
		}
	}

	////////////////////////////////////////////////
	/// check relative err for each sysOpt (for AN)
	TH2D* h2D_PR_maxerr[nOpt]; //y binning in y_lab^1st
	TH2D* h2D_NP_maxerr[nOpt];
	for (int iopt=0; iopt<nOpt; iopt++){
		h2D_PR_maxerr[iopt] = (TH2D*)h2D_PR_maxdiff[iopt]->Clone(Form("h2D_PR_maxerr_sys%s",sysOpt[iopt].Data()));
		h2D_NP_maxerr[iopt] = (TH2D*)h2D_NP_maxdiff[iopt]->Clone(Form("h2D_NP_maxerr_sys%s",sysOpt[iopt].Data()));
		//divide by default
		h2D_PR_maxerr[iopt]->Divide(h2D_PR_merge);
		h2D_NP_maxerr[iopt]->Divide(h2D_NP_merge);
	}
		
		///////////////////////////////////////////
		///// *****************  Special setting for 2-3 GeV/c bin *************** ///
		///////////////////////////////////////////
//	for (int iopt=0; iopt<nOpt; iopt++){
//		if (iopt==2 || iopt==3) {	
//			double tmptmpPR = h2D_PR_maxerr[iopt]->GetBinContent(0,1);		
//			double tmptmpNP = h2D_NP_maxerr[iopt]->GetBinContent(0,1);		
//			h2D_PR_maxerr[iopt]->SetBinContent( 8, 1, tmptmpPR );
//			h2D_NP_maxerr[iopt]->SetBinContent( 8, 1, tmptmpNP );
//		}
//	}

	////////////////////////////////////////////////
	////// calculate the total (quardrature sum of sys01, 02, 03, 04)
	TH2D* h2D_PR_tot = (TH2D*)h2D_PR_maxdiff[0]->Clone("h2D_PR_tot"); //y binning in y_lab^1st
	TH2D* h2D_NP_tot = (TH2D*)h2D_NP_maxdiff[0]->Clone("h2D_NP_tot"); //y binning in y_lab^1st
	
	double tmpPRdiff[nOpt];
	double tmpPRtot[nybin][nptbin];	
	double tmpNPdiff[nOpt];
	double tmpNPtot[nybin][nptbin];	
	
	for (int iy=0; iy<nybin;iy++ ){
		for (int ipt=0; ipt<nptbin;ipt++ ){
			tmpPRtot[iy][ipt]=0;
			tmpNPtot[iy][ipt]=0;
			for (int iopt=0; iopt<nOpt; iopt++){
				tmpPRdiff[iopt]=0; tmpNPdiff[iopt]=0;
				tmpPRdiff[iopt] = h2D_PR_maxdiff[iopt]->GetBinContent(iy+1,ipt+1);	
				tmpNPdiff[iopt] = h2D_NP_maxdiff[iopt]->GetBinContent(iy+1,ipt+1);	
			}
			//quardrature
			tmpPRtot[iy][ipt]= TMath::Sqrt(tmpPRdiff[0]*tmpPRdiff[0]+tmpPRdiff[1]*tmpPRdiff[1]+tmpPRdiff[2]*tmpPRdiff[2]+tmpPRdiff[3]*tmpPRdiff[3]);	
			tmpNPtot[iy][ipt]= TMath::Sqrt(tmpNPdiff[0]*tmpNPdiff[0]+tmpNPdiff[1]*tmpNPdiff[1]+tmpNPdiff[2]*tmpNPdiff[2]+tmpNPdiff[3]*tmpNPdiff[3]);	
			cout << iy<<ipt<<"iyipt : tmpPRdiff = " << tmpPRdiff[0]<<", "<<tmpPRdiff[1]<<", "<<tmpPRdiff[2]<<", "<<tmpPRdiff[3]<<endl;
			cout << "tmpPRtot = " << tmpPRtot[iy][ipt]<<endl;
			h2D_PR_tot->SetBinContent(iy+1, ipt+1, tmpPRtot[iy][ipt]);	
			h2D_NP_tot->SetBinContent(iy+1, ipt+1, tmpNPtot[iy][ipt]);	
		}
	}	
	
	//divide by default
	h2D_PR_tot->Divide(h2D_PR_merge);
	h2D_NP_tot->Divide(h2D_NP_merge);
	
	//set unsed values as zero		
	for (int iy=0; iy<nybin;iy++ ){
		for (int ipt=0; ipt<nptbin;ipt++ ){
			if ( !( (iy>=1&&iy<=6&&ipt==0) || (iy>=2&&iy<=5&&(ipt==1||ipt==2)) || (iy>=2&&iy<=4&&ipt==3) ) ) { continue;}
			for (int iopt=0; iopt<nOpt; iopt++){
				h2D_PR_maxerr[iopt]->SetBinContent(iy+1,ipt+1,0);
				h2D_NP_maxerr[iopt]->SetBinContent(iy+1,ipt+1,0);
				for (int isys=0; isys<nSubSys[iopt]; isys++){
					h2D_PR_err[iopt][isys]->SetBinContent(iy+1,ipt+1,0);
					h2D_NP_err[iopt][isys]->SetBinContent(iy+1,ipt+1,0);
				}
			}
			h2D_PR_tot->SetBinContent(iy+1, ipt+1, 0);
			h2D_NP_tot->SetBinContent(iy+1, ipt+1, 0);
		///////////////////////////////////////////
		///// *****************  Special setting for 2-3 GeV/c bin *************** ///
		///////////////////////////////////////////
			h2D_PR_tot->SetBinContent( 8, 1, 0.0345 );
			h2D_NP_tot->SetBinContent( 8, 1, 0.1448 );
		
		
		}
	}	
			
	
	////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("fitSysErr_%s.root",dirName),"RECREATE");
	std::cout << "dirName: " << dirName << std::endl;
	cout << "firSysErr_"<<dirName<<".root has been created :) " <<endl;	

	outFile->cd();
	//raw yield
	h2D_PR_Pbp->Write();	
	h2D_PR_pPb->Write();	
	h2D_PR_merge->Write();	
	h2D_NP_Pbp->Write();	
	h2D_NP_pPb->Write();	
	h2D_NP_merge->Write();	
	for (int iopt=0; iopt<nOpt; iopt++){
		for (int isys=0; isys<nSubSys[iopt]; isys++){
			h2D_PR_Pbp_sys[iopt][isys]->Write();
			h2D_PR_pPb_sys[iopt][isys]->Write();
			h2D_PR_merge_sys[iopt][isys]->Write();
			h2D_PR_diff[iopt][isys]->Write();
			h2D_PR_err[iopt][isys]->Write();
			h2D_NP_Pbp_sys[iopt][isys]->Write();
			h2D_NP_pPb_sys[iopt][isys]->Write();
			h2D_NP_merge_sys[iopt][isys]->Write();
			h2D_NP_diff[iopt][isys]->Write();
			h2D_NP_err[iopt][isys]->Write();
		}
		h2D_PR_maxdiff[iopt]->Write();
		h2D_NP_maxdiff[iopt]->Write();
		h2D_PR_maxerr[iopt]->Write();
		h2D_NP_maxerr[iopt]->Write();
	}
	h2D_PR_tot->Write();
	h2D_NP_tot->Write();
	outFile->Close();
	return 0;

} // end of main func

