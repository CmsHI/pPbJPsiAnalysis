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
int make2Droot_fitSysErr_ethf(char *dirName = "ethf"){

  using namespace std;
	const int nET = 3; // 0-20, 20-30, 30-120
	cout << "nET = " << nET << endl;

	//const int nybin = 8; //for 8rap9pt(2gev)
	//const int nptbin = 9;
	const int nybin = 6; //for 6rap3pt (ethf)
	const int nptbin = 3;
	const int nSubSysTmp = 10;

	TString sysOpt[] = {"01","02","03","04"};
	//const int nSubSys[] = {6,1,2,1};
	const int nSubSys[] = {5,1,2,1};
	
	const int nOpt = sizeof(sysOpt)/sizeof(TString);
	cout << "nOpt = " << nOpt << endl;

	////////////////////////////////////////////////
	////// read in fromfit file :: default!!
	TFile* fFitPbp[nET];
	TFile* fFitpPb[nET];
	TH2D* h2D_PR_Pbp[nET];
	TH2D* h2D_NP_Pbp[nET];
	TH2D* h2D_PR_pPb[nET];
	TH2D* h2D_NP_pPb[nET];
	for (int iet=0; iet<nET; iet++) {
		fFitPbp[iet] = new TFile(Form("./2Dhist_fitRes_%s_20150503_Pbp_bin%d.root",dirName,iet+1));
		fFitpPb[iet] = new TFile(Form("./2Dhist_fitRes_%s_20150503_pPb_bin%d.root",dirName,iet+1));
		h2D_PR_Pbp[iet] = (TH2D*)fFitPbp[iet]->Get("h2D_nPrompt");	
		h2D_NP_Pbp[iet] = (TH2D*)fFitPbp[iet]->Get("h2D_nNonprompt");	
		h2D_PR_pPb[iet] = (TH2D*)fFitpPb[iet]->Get("h2D_nPrompt");	
		h2D_NP_pPb[iet] = (TH2D*)fFitpPb[iet]->Get("h2D_nNonprompt");	
		h2D_PR_Pbp[iet]->SetName(Form("h2D_PR_Pbp_bin%d",iet+1));
		h2D_PR_pPb[iet]->SetName(Form("h2D_PR_pPb_bin%d",iet+1));
		h2D_NP_Pbp[iet]->SetName(Form("h2D_NP_Pbp_bin%d",iet+1));
		h2D_NP_pPb[iet]->SetName(Form("h2D_NP_pPb_bin%d",iet+1));
	}
	
	////////////////////////////////////////////////
	////// systematics!
	TFile* fFitPbp_sys[nET][nOpt][nSubSysTmp];	
	TFile* fFitpPb_sys[nET][nOpt][nSubSysTmp];	
	TH2D* h2D_PR_Pbp_sys[nET][nOpt][nSubSysTmp];
	TH2D* h2D_PR_pPb_sys[nET][nOpt][nSubSysTmp];
	TH2D* h2D_NP_Pbp_sys[nET][nOpt][nSubSysTmp];
	TH2D* h2D_NP_pPb_sys[nET][nOpt][nSubSysTmp];
	
	for (int iet=0; iet<nET; iet++) {
		for (int iopt=0; iopt<nOpt; iopt++){
			for (int isys=0; isys<nSubSys[iopt]; isys++){
				fFitPbp_sys[iet][iopt][isys] = new TFile(Form("./2Dhist_fitRes_%s_sys%s_0%d_Pbp_bin%d.root",dirName,sysOpt[iopt].Data(),isys+1,iet+1));	
				fFitpPb_sys[iet][iopt][isys] = new TFile(Form("./2Dhist_fitRes_%s_sys%s_0%d_pPb_bin%d.root",dirName,sysOpt[iopt].Data(),isys+1,iet+1));	
				h2D_PR_Pbp_sys[iet][iopt][isys] = (TH2D*)fFitPbp_sys[iet][iopt][isys]->Get("h2D_nPrompt");	
				h2D_NP_Pbp_sys[iet][iopt][isys] = (TH2D*)fFitPbp_sys[iet][iopt][isys]->Get("h2D_nNonprompt");	
				h2D_PR_pPb_sys[iet][iopt][isys] = (TH2D*)fFitpPb_sys[iet][iopt][isys]->Get("h2D_nPrompt");	
				h2D_NP_pPb_sys[iet][iopt][isys] = (TH2D*)fFitpPb_sys[iet][iopt][isys]->Get("h2D_nNonprompt");	
				h2D_PR_Pbp_sys[iet][iopt][isys]->SetName(Form("h2D_PR_Pbp_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));
				h2D_PR_pPb_sys[iet][iopt][isys]->SetName(Form("h2D_PR_pPb_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));
				h2D_NP_Pbp_sys[iet][iopt][isys]->SetName(Form("h2D_NP_Pbp_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));
				h2D_NP_pPb_sys[iet][iopt][isys]->SetName(Form("h2D_NP_pPb_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));
				cout << "*** iet = " << iet << endl;
				cout << "* sysOpt "<<iopt<<":: "<<isys<< "th 4 yield TH2D : "<<h2D_PR_Pbp_sys[iet][iopt][isys]<<" "<<h2D_PR_pPb_sys[iet][iopt][isys]<<" "<<h2D_NP_Pbp_sys[iet][iopt][isys]<<" "<<h2D_NP_pPb_sys[iet][iopt][isys]<<endl;
			}
		}
	}
	
	////////////////////////////////////////////////
	////// merge Pbp and pPb
	TH2D* h2D_PR_merge[nET]; //y binning in y_lab^1st
	TH2D* h2D_NP_merge[nET];
	TH2D* h2D_PR_merge_sys[nET][nOpt][nSubSysTmp];
	TH2D* h2D_NP_merge_sys[nET][nOpt][nSubSysTmp];
	double tmpPRyieldPbp;	
	double tmpPRyieldpPb;	
	double tmpNPyieldPbp;	
	double tmpNPyieldpPb;	
	double tmpPRyield_default[nET][nybin][nptbin];	
	double tmpNPyield_default[nET][nybin][nptbin];	
	double tmpPRyield_sys[nET][nOpt][nSubSysTmp][nybin][nptbin];	
	double tmpNPyield_sys[nET][nOpt][nSubSysTmp][nybin][nptbin];	

	/////////// default
	for (int iet=0; iet<nET; iet++) {
		h2D_PR_merge[iet]=(TH2D*)h2D_PR_Pbp[iet]->Clone(Form("h2D_PR_merge_bin%d",iet+1));	//y binning in y_lab^1st
		h2D_NP_merge[iet]=(TH2D*)h2D_NP_Pbp[iet]->Clone(Form("h2D_NP_merge_bin%d",iet+1));	
		
		for (int iy=0; iy<nybin;iy++ ){
			for (int ipt=0; ipt<nptbin;ipt++ ){
				tmpPRyield_default[iet][iy][ipt]=0;
				tmpNPyield_default[iet][iy][ipt]=0;
				tmpPRyieldPbp = h2D_PR_Pbp[iet]->GetBinContent(iy+1,ipt+1);	
				tmpPRyieldpPb = h2D_PR_pPb[iet]->GetBinContent(nybin-iy,ipt+1);
				tmpNPyieldPbp = h2D_NP_Pbp[iet]->GetBinContent(iy+1,ipt+1);	
				tmpNPyieldpPb = h2D_NP_pPb[iet]->GetBinContent(nybin-iy,ipt+1);
				//merge Pbp+pPb
				tmpPRyield_default[iet][iy][ipt] = tmpPRyieldPbp+tmpPRyieldpPb; 
				tmpNPyield_default[iet][iy][ipt] = tmpNPyieldPbp+tmpNPyieldpPb; 
				//fill into hist
				h2D_PR_merge[iet]->SetBinContent(iy+1,ipt+1,tmpPRyield_default[iet][iy][ipt]);
				h2D_NP_merge[iet]->SetBinContent(iy+1,ipt+1,tmpNPyield_default[iet][iy][ipt]);
			}
		}
	}

	//////////// sys	
	for (int iet=0; iet<nET; iet++) {
		for (int iopt=0; iopt<nOpt; iopt++){
			for (int isys=0; isys<nSubSys[iopt]; isys++){
				h2D_PR_merge_sys[iet][iopt][isys]=(TH2D*)h2D_PR_Pbp_sys[iet][iopt][isys]->Clone(Form("h2D_PR_merge_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));	//y binning in y_lab^1st
				h2D_NP_merge_sys[iet][iopt][isys]=(TH2D*)h2D_NP_Pbp_sys[iet][iopt][isys]->Clone(Form("h2D_NP_merge_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));	
				for (int iy=0; iy<nybin;iy++ ){
					for (int ipt=0; ipt<nptbin;ipt++ ){
						tmpPRyield_sys[iet][iopt][isys][iy][ipt]=0;
						tmpNPyield_sys[iet][iopt][isys][iy][ipt]=0;
						tmpPRyieldPbp=0; tmpPRyieldpPb=0;
						tmpNPyieldPbp=0; tmpNPyieldpPb=0;
						tmpPRyieldPbp = h2D_PR_Pbp_sys[iet][iopt][isys]->GetBinContent(iy+1,ipt+1);	
						tmpPRyieldpPb = h2D_PR_pPb_sys[iet][iopt][isys]->GetBinContent(nybin-iy,ipt+1);
						tmpNPyieldPbp = h2D_NP_Pbp_sys[iet][iopt][isys]->GetBinContent(iy+1,ipt+1);	
						tmpNPyieldpPb = h2D_NP_pPb_sys[iet][iopt][isys]->GetBinContent(nybin-iy,ipt+1);
						//merge Pbp+pPb
						tmpPRyield_sys[iet][iopt][isys][iy][ipt] = tmpPRyieldPbp+tmpPRyieldpPb; 
						tmpNPyield_sys[iet][iopt][isys][iy][ipt] = tmpNPyieldPbp+tmpNPyieldpPb; 
						//fill into hist
						h2D_PR_merge_sys[iet][iopt][isys]->SetBinContent(iy+1,ipt+1,tmpPRyield_sys[iet][iopt][isys][iy][ipt]);
						h2D_NP_merge_sys[iet][iopt][isys]->SetBinContent(iy+1,ipt+1,tmpNPyield_sys[iet][iopt][isys][iy][ipt]);
					}
				}
			}
		}
	}
	
	////////////////////////////////////////////////
	// difference and relative error calculation
	TH2D* h2D_PR_diff[nET][nOpt][nSubSysTmp];
	TH2D* h2D_NP_diff[nET][nOpt][nSubSysTmp];
	TH2D* h2D_PR_err[nET][nOpt][nSubSysTmp];
	TH2D* h2D_NP_err[nET][nOpt][nSubSysTmp];
	
	for (int iet=0; iet<nET; iet++) {
		for (int iopt=0; iopt<nOpt; iopt++){
			for (int isys=0; isys<nSubSys[iopt]; isys++){
				h2D_PR_diff[iet][iopt][isys] = (TH2D*)h2D_PR_merge[iet]->Clone(Form("h2D_PR_diff_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));
				h2D_NP_diff[iet][iopt][isys] = (TH2D*)h2D_NP_merge[iet]->Clone(Form("h2D_NP_diff_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));
				h2D_PR_diff[iet][iopt][isys]->Add(h2D_PR_merge_sys[iet][iopt][isys],-1);
				h2D_NP_diff[iet][iopt][isys]->Add(h2D_NP_merge_sys[iet][iopt][isys],-1);
				h2D_PR_err[iet][iopt][isys] = (TH2D*)h2D_PR_diff[iet][iopt][isys]->Clone(Form("h2D_PR_err_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));
				h2D_NP_err[iet][iopt][isys] = (TH2D*)h2D_NP_diff[iet][iopt][isys]->Clone(Form("h2D_NP_err_bin%d_sys%s_%d",iet+1,sysOpt[iopt].Data(),isys));
				h2D_PR_err[iet][iopt][isys]->Divide(h2D_PR_merge[iet]);
				h2D_NP_err[iet][iopt][isys]->Divide(h2D_NP_merge[iet]);
			}
		}
	}
	
	////////////////////////////////////////////////
	////// select the largest difference among nSubSys
	
	TH2D* h2D_PR_maxdiff[nET][nOpt]; //y binning in y_lab^1st
	TH2D* h2D_NP_maxdiff[nET][nOpt];
	double tmpPRval;	
	double tmpNPval;	
	double tmpPRmaxdiff[nET][nOpt][nybin][nptbin];	
	double tmpNPmaxdiff[nET][nOpt][nybin][nptbin];	
		
	for (int iet=0; iet<nET; iet++) {
		for (int iopt=0; iopt<nOpt; iopt++){
			h2D_PR_maxdiff[iet][iopt] = (TH2D*)h2D_PR_diff[iet][iopt][0]->Clone(Form("h2D_PR_maxdiff_bin%d_sys%s",iet+1,sysOpt[iopt].Data()));
			h2D_NP_maxdiff[iet][iopt] = (TH2D*)h2D_NP_diff[iet][iopt][0]->Clone(Form("h2D_NP_maxdiff_bin%d_sys%s",iet+1,sysOpt[iopt].Data()));
			for (int iy=0; iy<nybin;iy++ ){
				for (int ipt=0; ipt<nptbin;ipt++ ){
					tmpPRmaxdiff[iet][iopt][iy][ipt]=0;
					tmpNPmaxdiff[iet][iopt][iy][ipt]=0;
					for (int isys=0; isys<nSubSys[iopt]; isys++){
						tmpPRval=0;
						tmpNPval=0;
						tmpPRval = h2D_PR_diff[iet][iopt][isys]->GetBinContent(iy+1,ipt+1);	
						tmpNPval = h2D_NP_diff[iet][iopt][isys]->GetBinContent(iy+1,ipt+1);	
						tmpPRval = TMath::Abs(tmpPRval);
						tmpNPval = TMath::Abs(tmpNPval);
						if (tmpPRmaxdiff[iet][iopt][iy][ipt] < tmpPRval) { tmpPRmaxdiff[iet][iopt][iy][ipt] = tmpPRval; }
						if (tmpNPmaxdiff[iet][iopt][iy][ipt] < tmpNPval) { tmpNPmaxdiff[iet][iopt][iy][ipt] = tmpNPval; }
					}
					cout << iopt << "opt, maxdiff PR err for" <<iy<<"th y, "<<ipt<<"th pT = " << tmpPRmaxdiff[iet][iopt][iy][ipt]<<endl;
					cout << iopt << "opt, maxdiff NP err for" <<iy<<"th y, "<<ipt<<"th pT = " << tmpNPmaxdiff[iet][iopt][iy][ipt]<<endl;
					h2D_PR_maxdiff[iet][iopt]->SetBinContent(iy+1,ipt+1,tmpPRmaxdiff[iet][iopt][iy][ipt]);
					h2D_NP_maxdiff[iet][iopt]->SetBinContent(iy+1,ipt+1,tmpNPmaxdiff[iet][iopt][iy][ipt]);
				}
			}
		}
	}

	////////////////////////////////////////////////
	/// check relative err for each sysOpt (for AN)
	TH2D* h2D_PR_maxerr[nET][nOpt]; //y binning in y_lab^1st
	TH2D* h2D_NP_maxerr[nET][nOpt];
	for (int iet=0; iet<nET; iet++) {
		for (int iopt=0; iopt<nOpt; iopt++){
				h2D_PR_maxerr[iet][iopt] = (TH2D*)h2D_PR_maxdiff[iet][iopt]->Clone(Form("h2D_PR_maxerr_bin%d_sys%s",iet+1,sysOpt[iopt].Data()));
				h2D_NP_maxerr[iet][iopt] = (TH2D*)h2D_NP_maxdiff[iet][iopt]->Clone(Form("h2D_NP_maxerr_bin%d_sys%s",iet+1,sysOpt[iopt].Data()));
				//divide by default
				h2D_PR_maxerr[iet][iopt]->Divide(h2D_PR_merge[iet]);
				h2D_NP_maxerr[iet][iopt]->Divide(h2D_NP_merge[iet]);
		}
	}
	
	////////////////////////////////////////////////
	////// calculate the total (quardrature sum of sys01, 02, 03, 04)
	TH2D* h2D_PR_tot[nET];
	TH2D* h2D_NP_tot[nET];
	
	double tmpPRdiff[nET][nOpt];
	double tmpPRtot[nET][nybin][nptbin];	
	double tmpNPdiff[nET][nOpt];
	double tmpNPtot[nET][nybin][nptbin];	
	
	for (int iet=0; iet<nET; iet++) {
		h2D_PR_tot[iet]= (TH2D*)h2D_PR_maxdiff[iet][0]->Clone(Form("h2D_PR_tot_bin%d",iet+1)); //y binning in y_lab^1st
		h2D_NP_tot[iet] = (TH2D*)h2D_NP_maxdiff[iet][0]->Clone(Form("h2D_NP_tot_bin%d",iet+1)); //y binning in y_lab^1st
		for (int iy=0; iy<nybin;iy++ ){
			for (int ipt=0; ipt<nptbin;ipt++ ){
				tmpPRtot[iet][iy][ipt]=0;
				tmpNPtot[iet][iy][ipt]=0;
				for (int iopt=0; iopt<nOpt; iopt++){
					tmpPRdiff[iet][iopt]=0; tmpNPdiff[iet][iopt]=0;
					tmpPRdiff[iet][iopt] = h2D_PR_maxdiff[iet][iopt]->GetBinContent(iy+1,ipt+1);	
					tmpNPdiff[iet][iopt] = h2D_NP_maxdiff[iet][iopt]->GetBinContent(iy+1,ipt+1);	
				}
				//quardrature
				tmpPRtot[iet][iy][ipt]= TMath::Sqrt(tmpPRdiff[iet][0]*tmpPRdiff[iet][0]+tmpPRdiff[iet][1]*tmpPRdiff[iet][1]+tmpPRdiff[iet][2]*tmpPRdiff[iet][2]+tmpPRdiff[iet][3]*tmpPRdiff[iet][3]);	
				tmpNPtot[iet][iy][ipt]= TMath::Sqrt(tmpNPdiff[iet][0]*tmpNPdiff[iet][0]+tmpNPdiff[iet][1]*tmpNPdiff[iet][1]+tmpNPdiff[iet][2]*tmpNPdiff[iet][2]+tmpNPdiff[iet][3]*tmpNPdiff[iet][3]);	
				cout << iet << "th iet" << endl;
				cout << iy<<ipt<<"iyipt : tmpPRdiff = " << tmpPRdiff[iet][0]<<", "<<tmpPRdiff[iet][1]<<", "<<tmpPRdiff[iet][2]<<", "<<tmpPRdiff[iet][3]<<endl;
				cout << "tmpPRtot = " << tmpPRtot[iet][iy][ipt]<<endl;
				h2D_PR_tot[iet]->SetBinContent(iy+1, ipt+1, tmpPRtot[iet][iy][ipt]);	
				h2D_NP_tot[iet]->SetBinContent(iy+1, ipt+1, tmpNPtot[iet][iy][ipt]);	
			}
		}	
	}

	////////////////////////////////////////////////
	//divide by default
	for (int iet=0; iet<nET; iet++) {
		h2D_PR_tot[iet]->Divide(h2D_PR_merge[iet]);
		h2D_NP_tot[iet]->Divide(h2D_NP_merge[iet]);
	}
	
	////////////////////////////////////////////////
	//set unsed values as zero		
	for (int iet=0; iet<nET; iet++) {
		for (int iy=0; iy<nybin;iy++ ){
			for (int ipt=0; ipt<nptbin;ipt++ ){
				//if ( !( (iy>=1&&iy<=6&&ipt==0) || (iy>=2&&iy<=5&&(ipt==1||ipt==2)) || (iy>=2&&iy<=4&&ipt==3) ) ) { continue;}
				if ( ! (iy>=1&&iy<=4&&ipt==0) ) {continue;}
				for (int iopt=0; iopt<nOpt; iopt++){
					h2D_PR_maxerr[iet][iopt]->SetBinContent(iy+1,ipt+1,0);
					h2D_NP_maxerr[iet][iopt]->SetBinContent(iy+1,ipt+1,0);
					for (int isys=0; isys<nSubSys[iopt]; isys++){
						h2D_PR_err[iet][iopt][isys]->SetBinContent(iy+1,ipt+1,0);
						h2D_NP_err[iet][iopt][isys]->SetBinContent(iy+1,ipt+1,0);
					}
				}
				h2D_PR_tot[iet]->SetBinContent(iy+1, ipt+1, 0);
				h2D_NP_tot[iet]->SetBinContent(iy+1, ipt+1, 0);
			}
		}		
	}
	
	////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("fitSysErr_%s.root",dirName),"RECREATE");
	std::cout << "dirName: " << dirName << std::endl;
	cout << "firSysErr_"<<dirName<<".root has been created :) " <<endl;	

	outFile->cd();
	//raw yield
	for (int iet=0; iet<nET; iet++) {
		h2D_PR_Pbp[iet]->Write();	
		h2D_PR_pPb[iet]->Write();	
		h2D_PR_merge[iet]->Write();	
		h2D_NP_Pbp[iet]->Write();	
		h2D_NP_pPb[iet]->Write();	
		h2D_NP_merge[iet]->Write();	
		for (int iopt=0; iopt<nOpt; iopt++){
			for (int isys=0; isys<nSubSys[iopt]; isys++){
				h2D_PR_Pbp_sys[iet][iopt][isys]->Write();
				h2D_PR_pPb_sys[iet][iopt][isys]->Write();
				h2D_PR_merge_sys[iet][iopt][isys]->Write();
				h2D_PR_diff[iet][iopt][isys]->Write();
				h2D_PR_err[iet][iopt][isys]->Write();
				h2D_NP_Pbp_sys[iet][iopt][isys]->Write();
				h2D_NP_pPb_sys[iet][iopt][isys]->Write();
				h2D_NP_merge_sys[iet][iopt][isys]->Write();
				h2D_NP_diff[iet][iopt][isys]->Write();
				h2D_NP_err[iet][iopt][isys]->Write();
			}
			h2D_PR_maxdiff[iet][iopt]->Write();
			h2D_NP_maxdiff[iet][iopt]->Write();
			h2D_PR_maxerr[iet][iopt]->Write();
			h2D_NP_maxerr[iet][iopt]->Write();
		}
		h2D_PR_tot[iet]->Write();
		h2D_NP_tot[iet]->Write();
	}	
	outFile->Close();

	return 0;

} // end of main func

