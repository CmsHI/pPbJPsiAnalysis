#include <../SONGKYO.h>

/////// main func. ///////
int make_fitSystematics(int MrapNpt=89, int isPA =1, int accCutType=2){

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
	const int nSubOpt[nOpt] = {5,1,2,1};
	//const int nOpt = sizeof(szSysOpt)/sizeof(TString);
	cout << "nOpt = " << nOpt << endl;

	////////////////////////////////////////////////
	////// read in fromfit file :: default!!
  TFile* fFit = new TFile(Form("../FittingResult/fitResHist_%s_nominal.root",szFinal.Data()));
	cout << "fFit : " << fFit << endl;
  TH2D* h2D_PR = (TH2D*)fFit->Get("h2D_nPrompt_Raw");	
	TH2D* h2D_NP = (TH2D*)fFit->Get("h2D_nNonPrompt_Raw");	
	h2D_PR->SetName("h2D_PR");
	h2D_NP->SetName("h2D_NP");
  cout << "xia" << endl;
	
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
      fFit_sys[iopt][isubopt] = new TFile(Form("../FittingSys/fitResHist_%s_%s_0%d.root",szFinal.Data(),szSysOpt[iopt].Data(),isubopt+1));
			h2D_PR_sys[iopt][isubopt] = (TH2D*)fFit_sys[iopt][isubopt]->Get("h2D_nPrompt_Raw");	
			h2D_NP_sys[iopt][isubopt] = (TH2D*)fFit_sys[iopt][isubopt]->Get("h2D_nNonPrompt_Raw");	
			h2D_PR_sys[iopt][isubopt]->SetName(Form("h2D_PR_%s_0%d",szSysOpt[iopt].Data(),isubopt+1));
			h2D_NP_sys[iopt][isubopt]->SetName(Form("h2D_NP_%s_0%d",szSysOpt[iopt].Data(),isubopt+1));
			cout << " *** szSysOpt "<<iopt<<":: "<<isubopt<< "th yield TH2D : "<<h2D_PR_sys[iopt][isubopt]<<" "<<h2D_NP_sys[iopt][isubopt]<<" "<<endl;
/*
			// difference and relative error calculation
			h2D_PR_diff[iopt][isubopt] = (TH2D*)h2D_PR->Clone(Form("h2D_PR_diff_sys%s_%d",szSysOpt[iopt].Data(),isubopt));
			h2D_NP_diff[iopt][isubopt] = (TH2D*)h2D_NP->Clone(Form("h2D_NP_diff_sys%s_%d",szSysOpt[iopt].Data(),isubopt));
			h2D_PR_diff[iopt][isubopt]->Add(h2D_PR_sys[iopt][isubopt],-1);
			h2D_NP_diff[iopt][isubopt]->Add(h2D_NP_sys[iopt][isubopt],-1);
			h2D_PR_err[iopt][isubopt] = (TH2D*)h2D_PR_diff[iopt][isubopt]->Clone(Form("h2D_PR_err_sys%s_%d",szSysOpt[iopt].Data(),isubopt));
			h2D_NP_err[iopt][isubopt] = (TH2D*)h2D_NP_diff[iopt][isubopt]->Clone(Form("h2D_NP_err_sys%s_%d",szSysOpt[iopt].Data(),isubopt));
			h2D_PR_err[iopt][isubopt]->Divide(h2D_PR);
			h2D_NP_err[iopt][isubopt]->Divide(h2D_NP);
*/
		}
	}

	////////////////////////////////////////////////
/*
	////// merge Pbp and pPb
	TH2D* h2D_PR_merge; //y binning in y_lab^1st
	TH2D* h2D_NP_merge;
	TH2D* h2D_PR_merge_sys[nOpt][nSubOptTmp];
	TH2D* h2D_NP_merge_sys[nOpt][nSubOptTmp];
	double tmpPRyieldPbp;	
	double tmpPRyieldpPb;	
	double tmpNPyieldPbp;	
	double tmpNPyieldpPb;	
	double tmpPRyield_default[nRap][nPt];	
	double tmpNPyield_default[nRap][nPt];	
	double tmpPRyield_sys[nOpt][nSubOptTmp][nRap][nPt];	
	double tmpNPyield_sys[nOpt][nSubOptTmp][nRap][nPt];	

	/////////// default
	h2D_PR_merge=(TH2D*)h2D_PR->Clone("h2D_PR_merge");	//y binning in y_lab^1st
	h2D_NP_merge=(TH2D*)h2D_NP->Clone("h2D_NP_merge");	
	
	for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
			tmpPRyield_default[iy][ipt]=0;
			tmpNPyield_default[iy][ipt]=0;
			tmpPRyieldPbp = h2D_PR->GetBinContent(iy+1,ipt+1);	
			tmpPRyieldpPb = h2D_PR_pPb->GetBinContent(nRap-iy,ipt+1);
			tmpNPyieldPbp = h2D_NP->GetBinContent(iy+1,ipt+1);	
			tmpNPyieldpPb = h2D_NP_pPb->GetBinContent(nRap-iy,ipt+1);
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
		for (int isubopt=0; isubopt<nSubOpt[iopt]; isubopt++){
			h2D_PR_merge_sys[iopt][isubopt]=(TH2D*)h2D_PR_sys[iopt][isubopt]->Clone(Form("h2D_PR_merge_sys%s_%d",szSysOpt[iopt].Data(),isubopt));	//y binning in y_lab^1st
			h2D_NP_merge_sys[iopt][isubopt]=(TH2D*)h2D_NP_sys[iopt][isubopt]->Clone(Form("h2D_NP_merge_sys%s_%d",szSysOpt[iopt].Data(),isubopt));	
			for (int iy=0; iy<nRap;iy++ ){
				for (int ipt=0; ipt<nPt;ipt++ ){
					tmpPRyield_sys[iopt][isubopt][iy][ipt]=0;
					tmpNPyield_sys[iopt][isubopt][iy][ipt]=0;
					tmpPRyieldPbp=0; tmpPRyieldpPb=0;
					tmpNPyieldPbp=0; tmpNPyieldpPb=0;
					tmpPRyieldPbp = h2D_PR_sys[iopt][isubopt]->GetBinContent(iy+1,ipt+1);	
					tmpPRyieldpPb = h2D_PR_pPb_sys[iopt][isubopt]->GetBinContent(nRap-iy,ipt+1);
					tmpNPyieldPbp = h2D_NP_sys[iopt][isubopt]->GetBinContent(iy+1,ipt+1);	
					tmpNPyieldpPb = h2D_NP_pPb_sys[iopt][isubopt]->GetBinContent(nRap-iy,ipt+1);
					//merge Pbp+pPb
					tmpPRyield_sys[iopt][isubopt][iy][ipt] = tmpPRyieldPbp+tmpPRyieldpPb; 
					tmpNPyield_sys[iopt][isubopt][iy][ipt] = tmpNPyieldPbp+tmpNPyieldpPb; 
					//fill into hist
					h2D_PR_merge_sys[iopt][isubopt]->SetBinContent(iy+1,ipt+1,tmpPRyield_sys[iopt][isubopt][iy][ipt]);
					h2D_NP_merge_sys[iopt][isubopt]->SetBinContent(iy+1,ipt+1,tmpNPyield_sys[iopt][isubopt][iy][ipt]);
				}
			}
		}
	}
*/

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
	////// select the largest difference among nSubOpt
	
	TH2D* h2D_PR_maxdiff[nOpt]; //y binning in y_lab^1st
	TH2D* h2D_NP_maxdiff[nOpt];
	double tmpPRval;	
	double tmpNPval;	
	double tmpPRmaxdiff[nOpt][nRap][nPt];	
	double tmpNPmaxdiff[nOpt][nRap][nPt];	
		
	for (int iopt=0; iopt<nOpt; iopt++){
		h2D_PR_maxdiff[iopt] = (TH2D*)h2D_PR_diff[iopt][0]->Clone(Form("h2D_PR_maxdiff_%s",szSysOpt[iopt].Data()));
		h2D_NP_maxdiff[iopt] = (TH2D*)h2D_NP_diff[iopt][0]->Clone(Form("h2D_NP_maxdiff_%s",szSysOpt[iopt].Data()));
		for (int iy=0; iy<nRap;iy++ ){
			for (int ipt=0; ipt<nPt;ipt++ ){
				tmpPRmaxdiff[iopt][iy][ipt]=0;
				tmpNPmaxdiff[iopt][iy][ipt]=0;
				for (int isubopt=0; isubopt<nSubOpt[iopt]; isubopt++){
					tmpPRval=0;
					tmpNPval=0;
					tmpPRval = h2D_PR_diff[iopt][isubopt]->GetBinContent(iy+1,ipt+1);	
					tmpNPval = h2D_NP_diff[iopt][isubopt]->GetBinContent(iy+1,ipt+1);	
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
	/// check relative err for each szSysOpt (for AN)
	TH2D* h2D_PR_maxerr[nOpt]; //y binning in y_lab^1st
	TH2D* h2D_NP_maxerr[nOpt];
	for (int iopt=0; iopt<nOpt; iopt++){
		h2D_PR_maxerr[iopt] = (TH2D*)h2D_PR_maxdiff[iopt]->Clone(Form("h2D_PR_maxerr_%s",szSysOpt[iopt].Data()));
		h2D_NP_maxerr[iopt] = (TH2D*)h2D_NP_maxdiff[iopt]->Clone(Form("h2D_NP_maxerr_%s",szSysOpt[iopt].Data()));
		//divide by default
		h2D_PR_maxerr[iopt]->Divide(h2D_PR);
		h2D_NP_maxerr[iopt]->Divide(h2D_NP);
	}
	
/*  	
	///////////////////////////////////////////
	///// *****************  Special setting for 2-3 GeV/c bin *************** ///
  ///////////////////////////////////////////
	for (int iopt=0; iopt<nOpt; iopt++){
		if (iopt==2 || iopt==3) {	
			double tmptmpPR = h2D_PR_maxerr[iopt]->GetBinContent(0,1);		
			double tmptmpNP = h2D_NP_maxerr[iopt]->GetBinContent(0,1);		
			h2D_PR_maxerr[iopt]->SetBinContent( 8, 1, tmptmpPR );
			h2D_NP_maxerr[iopt]->SetBinContent( 8, 1, tmptmpNP );
		}
	}
*/
	
  /////////////////////////////////////////////////////////////////
	////// calculate the total (quardrature sum of sys01, 02, 03, 04)
  /////////////////////////////////////////////////////////////////
	
  TH2D* h2D_PR_tot = (TH2D*)h2D_PR_maxdiff[0]->Clone("h2D_PR_tot"); //y binning in y_lab^1st
	TH2D* h2D_NP_tot = (TH2D*)h2D_NP_maxdiff[0]->Clone("h2D_NP_tot"); //y binning in y_lab^1st
	
	double tmpPRdiff[nOpt];
	double tmpPRtot[nRap][nPt];	
	double tmpNPdiff[nOpt];
	double tmpNPtot[nRap][nPt];	
	
	for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
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
	h2D_PR_tot->Divide(h2D_PR);
	h2D_NP_tot->Divide(h2D_NP);

	//set unsed values as zero		
	for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
			if ( isPA==0 && !( (iy>=1&&iy<=6&&(ipt==0||ipt==1)) || (iy>=2&&iy<=5&&(ipt==2||ipt==3)) ) ) { continue;}
			if ( isPA==1 && !( (iy>=1&&iy<=6&&(ipt==0||ipt==1)) || (iy>=2&&iy<=5&&(ipt==2)) || (iy>=2&&iy<=4&&ipt==3) ) ) { continue;}
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
		/*
    ///////////////////////////////////////////
		///// *****************  Special setting for 2-3 GeV/c bin *************** ///
		///////////////////////////////////////////
			h2D_PR_tot->SetBinContent( 8, 1, 0.0345 );
			h2D_NP_tot->SetBinContent( 8, 1, 0.1448 );
		*/
		}
	}	
	
	////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("fitSysErr_%s.root",szFinal.Data()),"RECREATE");
	cout << "firSysErr_"<<szFinal<<".root has been created :) " <<endl;	

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

