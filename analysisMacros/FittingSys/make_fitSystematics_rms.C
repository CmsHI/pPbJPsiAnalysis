#include <../SONGKYO.h>

/////// main func. ///////
int make_fitSystematics_rms(int MrapNpt=89, int isPA =1, int accCutType=2, int etOpt = 2){

  using namespace std;
  
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return 0; }
  TString szPA;
  if (isPA==0) szPA="pp";
//  else if (isPA==1) szPA="Pbp";
//  else if (isPA==2) szPA="pPb";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0 or 1"<< endl; return 0; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 0 or 1"<< endl; return 0; }
  
  const TString szFinal = Form("%s_%s_%s",szBinning.Data(),szPA.Data(),szAccCut.Data());
  std::cout << "szFinal: " << szFinal << std::endl;
  
  //////////////////////////////////////////////////////////////////////////////////////
  //// Definition of the binning
  int nRap, nPt;
  if (MrapNpt==89) { nRap = 8; nPt = 9; }
  else if (MrapNpt==83) { nRap = 8; nPt = 3; }
  else if (MrapNpt==63) { nRap = 6; nPt = 3;}
  else { nRap = 6; nPt = 2;}

  //// sys bins
	const int nSubOptTmp = 10;
  const int nOpt = 4;
	TString szSysOpt[nOpt] = {"sys01","sys02","sys03","sys04"};
	//const int nSubOpt[nOpt] = {5,1,2,1};
	const int nSubOpt[nOpt] = {3,1,2,1};
	//const int nOpt = sizeof(szSysOpt)/sizeof(TString);
	cout << "nOpt = " << nOpt << endl;

	////////////////////////////////////////////////
	////// read in fromfit file :: nominal
  TFile* fFit = new TFile(Form("../FittingResult/fitResHist_%s_nominal_etOpt%d.root",szFinal.Data(),etOpt));
	cout << "fFit : " << fFit << endl;
  TH2D* h2D_PR = (TH2D*)fFit->Get("h2D_nPrompt_Raw");	
	TH2D* h2D_NP = (TH2D*)fFit->Get("h2D_nNonPrompt_Raw");	
	h2D_PR->SetName("h2D_PR");
	h2D_NP->SetName("h2D_NP");
	
	////////////////////////////////////////////////
	////// systematics!
	TFile* fFit_sys[nOpt][nSubOptTmp];	
	TH2D* h2D_PR_sys[nOpt][nSubOptTmp];
	TH2D* h2D_NP_sys[nOpt][nSubOptTmp];
/*
	TH2D* h2D_PR_diff[nOpt][nSubOptTmp];
	TH2D* h2D_NP_diff[nOpt][nSubOptTmp];
	TH2D* h2D_PR_err[nOpt][nSubOptTmp];
	TH2D* h2D_NP_err[nOpt][nSubOptTmp];
*/
	
	for (int iopt=0; iopt<nOpt; iopt++){
		for (int isubopt=0; isubopt<nSubOpt[iopt]; isubopt++){
      fFit_sys[iopt][isubopt] = new TFile(Form("../FittingSys/fitResHist_%s_%s_0%d_etOpt%d.root",szFinal.Data(),szSysOpt[iopt].Data(),isubopt+1,etOpt));
			h2D_PR_sys[iopt][isubopt] = (TH2D*)fFit_sys[iopt][isubopt]->Get("h2D_nPrompt_Raw");	
			h2D_NP_sys[iopt][isubopt] = (TH2D*)fFit_sys[iopt][isubopt]->Get("h2D_nNonPrompt_Raw");	
			h2D_PR_sys[iopt][isubopt]->SetName(Form("h2D_PR_%s_0%d",szSysOpt[iopt].Data(),isubopt+1));
			h2D_NP_sys[iopt][isubopt]->SetName(Form("h2D_NP_%s_0%d",szSysOpt[iopt].Data(),isubopt+1));
			cout << " *** szSysOpt "<<iopt<<":: "<<isubopt<< "th yield TH2D : "<<h2D_PR_sys[iopt][isubopt]<<" "<<h2D_NP_sys[iopt][isubopt]<<" "<<endl;
		}
	}

	////////////////////////////////////////////////
	// difference and relative error calculation
	TH2D* h2D_PR_diff[nOpt][nSubOptTmp];
	TH2D* h2D_NP_diff[nOpt][nSubOptTmp];
	TH2D* h2D_PR_err[nOpt][nSubOptTmp];
	TH2D* h2D_NP_err[nOpt][nSubOptTmp];
	
	for (int iopt=0; iopt<nOpt; iopt++){
		for (int isubopt=0; isubopt<nSubOpt[iopt]; isubopt++){
			h2D_PR_diff[iopt][isubopt] = (TH2D*)h2D_PR->Clone(Form("h2D_PR_diff_%s_0%d",szSysOpt[iopt].Data(),isubopt+1));
			h2D_NP_diff[iopt][isubopt] = (TH2D*)h2D_NP->Clone(Form("h2D_NP_diff_%s_0%d",szSysOpt[iopt].Data(),isubopt+1));
			h2D_PR_diff[iopt][isubopt]->Add(h2D_PR_sys[iopt][isubopt],-1);
			h2D_NP_diff[iopt][isubopt]->Add(h2D_NP_sys[iopt][isubopt],-1);
			h2D_PR_err[iopt][isubopt] = (TH2D*)h2D_PR_diff[iopt][isubopt]->Clone(Form("h2D_PR_err_%s_0%d",szSysOpt[iopt].Data(),isubopt+1));
			h2D_NP_err[iopt][isubopt] = (TH2D*)h2D_NP_diff[iopt][isubopt]->Clone(Form("h2D_NP_err_%s_0%d",szSysOpt[iopt].Data(),isubopt+1));
			h2D_PR_err[iopt][isubopt]->Divide(h2D_PR);
			h2D_NP_err[iopt][isubopt]->Divide(h2D_NP);
		}
	}

	////////////////////////////////////////////////
	////// take "rms" of nSubOpt
	
	TH2D* h2D_PR_maxdiff[nOpt]; //y binning in y_lab^1st
	TH2D* h2D_NP_maxdiff[nOpt];
	double tmpPRval[nSubOptTmp]; // for rms	
	double tmpNPval[nSubOptTmp];	
	double tmpPRmaxdiff[nOpt][nRap][nPt];	 // actually rms diff in this code!
	double tmpNPmaxdiff[nOpt][nRap][nPt];	
		
	for (int iopt=0; iopt<nOpt; iopt++){
		h2D_PR_maxdiff[iopt] = (TH2D*)h2D_PR_diff[iopt][0]->Clone(Form("h2D_PR_maxdiff_%s",szSysOpt[iopt].Data()));
		h2D_NP_maxdiff[iopt] = (TH2D*)h2D_NP_diff[iopt][0]->Clone(Form("h2D_NP_maxdiff_%s",szSysOpt[iopt].Data()));
		for (int iy=0; iy<nRap;iy++ ){
			for (int ipt=0; ipt<nPt;ipt++ ){
				tmpPRmaxdiff[iopt][iy][ipt]=0;
				tmpNPmaxdiff[iopt][iy][ipt]=0;
				for (int isubopt=0; isubopt<nSubOpt[iopt]; isubopt++){
					tmpPRval[isubopt]=0;
					tmpNPval[isubopt]=0;
					tmpPRval[isubopt] = h2D_PR_diff[iopt][isubopt]->GetBinContent(iy+1,ipt+1);	
					tmpNPval[isubopt] = h2D_NP_diff[iopt][isubopt]->GetBinContent(iy+1,ipt+1);	
          cout << " --- tmpPRval["<<isubopt<<"] = "<<tmpPRval[isubopt]<<endl;
          tmpPRmaxdiff[iopt][iy][ipt] += tmpPRval[isubopt]*tmpPRval[isubopt];
          tmpNPmaxdiff[iopt][iy][ipt] += tmpNPval[isubopt]*tmpNPval[isubopt];
          cout << "--- tmpPRmaxdiff["<<isubopt<<"] = "<<tmpPRmaxdiff[iopt][iy][ipt]<<endl;
				}
				tmpPRmaxdiff[iopt][iy][ipt] = TMath::Sqrt((tmpPRmaxdiff[iopt][iy][ipt])/nSubOpt[iopt]); //RMS
				tmpNPmaxdiff[iopt][iy][ipt] = TMath::Sqrt((tmpNPmaxdiff[iopt][iy][ipt])/nSubOpt[iopt]); //RMS
        cout << iopt << "opt, maxdiff PR err for" <<iy<<"th y, "<<ipt<<"th pT = " << tmpPRmaxdiff[iopt][iy][ipt]<<endl;
				cout << iopt << "opt, maxdiff NP err for" <<iy<<"th y, "<<ipt<<"th pT = " << tmpNPmaxdiff[iopt][iy][ipt]<<endl;
				h2D_PR_maxdiff[iopt]->SetBinContent(iy+1,ipt+1,tmpPRmaxdiff[iopt][iy][ipt]);
				h2D_NP_maxdiff[iopt]->SetBinContent(iy+1,ipt+1,tmpNPmaxdiff[iopt][iy][ipt]);
			}
		}
	}
  
  ////////////////////////////////////////////////
	/// check relative err for each szSysOpt ( (norminal)-(sys)/(nominal) )
	TH2D* h2D_PR_maxerr[nOpt]; //y binning in y_lab^1st
	TH2D* h2D_NP_maxerr[nOpt];
	for (int iopt=0; iopt<nOpt; iopt++){
		h2D_PR_maxerr[iopt] = (TH2D*)h2D_PR_maxdiff[iopt]->Clone(Form("h2D_PR_maxerr_%s",szSysOpt[iopt].Data()));
		h2D_NP_maxerr[iopt] = (TH2D*)h2D_NP_maxdiff[iopt]->Clone(Form("h2D_NP_maxerr_%s",szSysOpt[iopt].Data()));
		//// divide by nominal
		h2D_PR_maxerr[iopt]->Divide(h2D_PR);
		h2D_NP_maxerr[iopt]->Divide(h2D_NP);
	}
	
  /////////////////////////////////////////////////////////////////
	////// calculate the total (quardrature sum of sys01, 02, 03, 04)
  /////////////////////////////////////////////////////////////////
	
  TH2D* h2D_PR_tot = (TH2D*)h2D_PR_maxerr[0]->Clone("h2D_PR_tot"); //y binning in y_lab^1st
	TH2D* h2D_NP_tot = (TH2D*)h2D_NP_maxerr[0]->Clone("h2D_NP_tot"); //y binning in y_lab^1st
	
	double tmpPRerr[nOpt];
	double tmpPRtot[nRap][nPt];	
	double tmpNPerr[nOpt];
	double tmpNPtot[nRap][nPt];	
	
	for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
			tmpPRtot[iy][ipt]=0;
			tmpNPtot[iy][ipt]=0;
			for (int iopt=0; iopt<nOpt; iopt++){
				tmpPRerr[iopt]=0; tmpNPerr[iopt]=0;
				tmpPRerr[iopt] = h2D_PR_maxerr[iopt]->GetBinContent(iy+1,ipt+1);	
				tmpNPerr[iopt] = h2D_NP_maxerr[iopt]->GetBinContent(iy+1,ipt+1);	
			}
			//// quardrature of sys01, 02, 03, 04
			tmpPRtot[iy][ipt]= TMath::Sqrt(tmpPRerr[0]*tmpPRerr[0]+tmpPRerr[1]*tmpPRerr[1]+tmpPRerr[2]*tmpPRerr[2]+tmpPRerr[3]*tmpPRerr[3]);	
			tmpNPtot[iy][ipt]= TMath::Sqrt(tmpNPerr[0]*tmpNPerr[0]+tmpNPerr[1]*tmpNPerr[1]+tmpNPerr[2]*tmpNPerr[2]+tmpNPerr[3]*tmpNPerr[3]);	
			h2D_PR_tot->SetBinContent(iy+1, ipt+1, tmpPRtot[iy][ipt]);	
			h2D_NP_tot->SetBinContent(iy+1, ipt+1, tmpNPtot[iy][ipt]);	
		}
	}	
	
  //// set unsed values as zero		
	for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
			if ( MrapNpt==89 && isPA==0 && !( (iy>=1&&iy<=6&&(ipt==0||ipt==1)) || (iy>=2&&iy<=5&&(ipt==2||ipt==3)) ) ) { continue;}
			if ( MrapNpt==89 && isPA==1 && !( (iy>=1&&iy<=6&&(ipt==0||ipt==1)) || (iy>=2&&iy<=5&&(ipt==2)) || (iy>=2&&iy<=4&&ipt==3) ) ) { continue;}
      if (MrapNpt==62 && isPA==1 && !( (iy>=1 && iy<=4&&(ipt==0)) ) ) {continue; } 
			for (int iopt=0; iopt<nOpt; iopt++){
				h2D_PR_maxerr[iopt]->SetBinContent(iy+1,ipt+1,0);
				h2D_NP_maxerr[iopt]->SetBinContent(iy+1,ipt+1,0);
				for (int isubopt=0; isubopt<nSubOpt[iopt]; isubopt++){
					h2D_PR_err[iopt][isubopt]->SetBinContent(iy+1,ipt+1,0);
					h2D_NP_err[iopt][isubopt]->SetBinContent(iy+1,ipt+1,0);
				}
			}
			h2D_PR_tot->SetBinContent(iy+1, ipt+1, 0);
			h2D_NP_tot->SetBinContent(iy+1, ipt+1, 0);
		}
	}	
  
  //// cout check	
  for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
			cout << " ***** " <<endl;
      cout << iy<<"th ybin, "<<ipt<<"th ptbin"<<endl;
      cout << "PR_tot : " << h2D_PR_tot->GetBinContent(iy+1, ipt+1)<<endl;
      cout << "NP_tot : " << h2D_NP_tot->GetBinContent(iy+1, ipt+1)<<endl;
		}
	}	
	
	////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("fitSysErr_%s_etOpt%d_rms.root",szFinal.Data(),etOpt),"RECREATE");
	cout << "firSysErr_"<<szFinal<<"etOpt"<<etOpt<<"_rms.root has been created :) " <<endl;	

	outFile->cd();
	//raw yield
	h2D_PR->Write();	
	h2D_NP->Write();	
	for (int iopt=0; iopt<nOpt; iopt++){
		for (int isubopt=0; isubopt<nSubOpt[iopt]; isubopt++){
			h2D_PR_sys[iopt][isubopt]->Write();
			h2D_PR_diff[iopt][isubopt]->Write();
			h2D_PR_err[iopt][isubopt]->Write();
			h2D_NP_sys[iopt][isubopt]->Write();
			h2D_NP_diff[iopt][isubopt]->Write();
			h2D_NP_err[iopt][isubopt]->Write();
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

