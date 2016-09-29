#include <../SONGKYO.h>

/////// main func. ///////
int make_TotSys(int MrapNpt = 89, int isPA =0, int etOpt=0){

  using namespace std;

  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return 0; }
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0 or 1"<< endl; return 0; }
  
  const TString szFinal = Form("%s_%s",szBinning.Data(),szPA.Data());
  std::cout << "szFinal: " << szFinal << std::endl;
  
  //////////////////////////////////////////////////////////////////////////////////////
  //// Definition of the binning
  int nRap, nPt;
  if (MrapNpt==89) { nRap = 8; nPt = 9; }
  else if (MrapNpt==83) { nRap = 8; nPt = 3; }
  else if (MrapNpt==63) { nRap = 6; nPt = 3;}
  else { nRap = 6; nPt = 2;}

  
  if (isPA==0 && MrapNpt==62) { cout << "ERORR: no 6rap2pt for pp!!!" <<endl; return 0; };
	//////////////////////////////////////////????????????????????????//////
	////// read in from each sys. file
	TFile* fFit;
  //fFit = new TFile(Form("../FittingSys/fitSysErr_%s_%s_newcut_etOpt%d_rms.root",szBinning.Data(),szPA.Data(),etOpt));
  fFit = new TFile(Form("../FittingSys/fitSysErr_%s_%s_newcut_etOpt%d_max.root",szBinning.Data(),szPA.Data(),etOpt));
	
  TFile* fAccEff;
  if (isPA==0) { fAccEff = new TFile(Form("../AccEffSys/AccEffSys_%s_%s_newcut_nominal_Zvtx0_SF1_etOpt%d.root",szPA.Data(),szBinning.Data(),etOpt)); }
  else { fAccEff = new TFile(Form("../AccEffSys/AccEffSys_%s_%s_newcut_nominal_Zvtx1_SF1_etOpt%d.root",szPA.Data(),szBinning.Data(),etOpt)); }
  
  TFile* fTNP_PR;
  TFile* fTNP_NP;
  if (isPA==0) { 
    fTNP_PR = new TFile(Form("../TNPSys/Total_TnP_sys_%s_pp_PR.root",szBinning.Data()));
    fTNP_NP = new TFile(Form("../TNPSys/Total_TnP_sys_%s_pp_NP.root",szBinning.Data()));
  } else {
    fTNP_PR = new TFile(Form("../TNPSys/Total_TnP_sys_%s_Pbp_PR.root",szBinning.Data()));
    fTNP_NP = new TFile(Form("../TNPSys/Total_TnP_sys_%s_Pbp_NP.root",szBinning.Data()));
  }
	
  ////////////////////////////////////////////////////////////////////////////////////////
	////// read in hist.
	TH2D* hFitPR = (TH2D*)fFit->Get("h2D_PR_tot");	
	TH2D* hFitNP = (TH2D*)fFit->Get("h2D_NP_tot");	
	TH2D* hAccPR = (TH2D*)fAccEff->Get(Form("h2D_Acc_PR_%s_final",szPA.Data()));	
	TH2D* hAccNP = (TH2D*)fAccEff->Get(Form("h2D_Acc_NP_%s_final",szPA.Data()));	
	TH2D* hEffMCPR = (TH2D*)fAccEff->Get(Form("h2D_Eff_PR_%s_final",szPA.Data()));
	TH2D* hEffMCNP = (TH2D*)fAccEff->Get(Form("h2D_Eff_NP_%s_final",szPA.Data()));
	TH2D* hTNPPR = (TH2D*)fTNP_PR->Get("htot");
	TH2D* hTNPNP = (TH2D*)fTNP_NP->Get("htot");
	hFitPR->SetName("hFitPR");
	hFitNP->SetName("hFitNP");
	hAccPR->SetName("hAccPR");
	hAccNP->SetName("hAccNP");
	hEffMCPR->SetName("hEffMCPR");
	hEffMCNP->SetName("hEffMCNP");
	hTNPPR->SetName("hTNPPR");
	hTNPNP->SetName("hTNPNP");

  ////////////////////////////////////////////////////////////////////////////////////////
  /// *** check total efficiency syst. = Sqrt( (hEffMC)^2 + (hTNP)^2 +(TrackingEff)^2 )  
  //  *** where tracking efficiency = 2 %
  ////////////////////////////////////////////////////////////////////////////////////////
  
  TH2D* hEffPR=(TH2D*)hFitPR->Clone("hEffPR");	//y binning in y_lab^1st
  TH2D* hEffNP=(TH2D*)hFitNP->Clone("hEffNP");	//y binning in y_lab^1st
	
  double tmpPRtot[nRap][nPt];
	double tmpNPtot[nRap][nPt];
	double tmp_PR1, tmp_PR2, tmp_PR3;
	double tmp_NP1, tmp_NP2, tmp_NP3;
	
  for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
			tmpPRtot[iy][ipt]=0;
			tmpNPtot[iy][ipt]=0;
			tmp_PR1=0; tmp_PR2=0; tmp_PR3=0;
			tmp_NP1=0; tmp_NP2=0; tmp_NP3=0;
      tmp_PR1 = hEffMCPR->GetBinContent(iy+1,ipt+1);	
			tmp_PR2 = hTNPPR->GetBinContent(iy+1,ipt+1);	
			tmp_NP1 = hEffMCNP->GetBinContent(iy+1,ipt+1);	
			tmp_NP2 = hTNPNP->GetBinContent(iy+1,ipt+1);	
			//// calculate the total relative error
			tmpPRtot[iy][ipt]=TMath::Sqrt( tmp_PR1*tmp_PR1 + tmp_PR2*tmp_PR2 + 0.02*0.02 );
			tmpNPtot[iy][ipt]=TMath::Sqrt( tmp_NP1*tmp_NP1 + tmp_NP2*tmp_NP2 + 0.02*0.02 );
			//// fill into hist
			hEffPR->SetBinContent(iy+1,ipt+1,tmpPRtot[iy][ipt]);
			hEffNP->SetBinContent(iy+1,ipt+1,tmpNPtot[iy][ipt]);
		}
	}
  
  ////////////////////////////////////////////////////////////////////////////////////////
  //// ***** Final total Systematic uncertainties!! Sqrt( (Acc)^2 + (Eff)^2 + (Fit)^2 )
  ////////////////////////////////////////////////////////////////////////////////////////

	TH2D* hTotalPR=(TH2D*)hFitPR->Clone("hTotalPR");	//y binning in y_lab^1st
	TH2D* hTotalNP=(TH2D*)hFitNP->Clone("hTotalNP");	//y binning in y_lab^1st
	
	for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
			tmpPRtot[iy][ipt]=0;
			tmpNPtot[iy][ipt]=0;
			tmp_PR1=0; tmp_PR2=0; tmp_PR3=0;
			tmp_NP1=0; tmp_NP2=0; tmp_NP3=0;
			tmp_PR1 = hFitPR->GetBinContent(iy+1,ipt+1);	
			tmp_PR2 = hAccPR->GetBinContent(iy+1,ipt+1);	
			tmp_PR3 = hEffPR->GetBinContent(iy+1,ipt+1);	
			tmp_NP1 = hFitNP->GetBinContent(iy+1,ipt+1);	
			tmp_NP2 = hAccNP->GetBinContent(iy+1,ipt+1);	
			tmp_NP3 = hEffNP->GetBinContent(iy+1,ipt+1);	
			//// calculate the total relative error
			tmpPRtot[iy][ipt]=TMath::Sqrt(tmp_PR1*tmp_PR1+tmp_PR2*tmp_PR2+tmp_PR3*tmp_PR3);
			tmpNPtot[iy][ipt]=TMath::Sqrt(tmp_NP1*tmp_NP1+tmp_NP2*tmp_NP2+tmp_NP3*tmp_NP3);
			//// fill into hist
			hTotalPR->SetBinContent(iy+1,ipt+1,tmpPRtot[iy][ipt]);
			hTotalNP->SetBinContent(iy+1,ipt+1,tmpNPtot[iy][ipt]);
		}
	}

	//// set unsed values as zero		
	for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
			if ( isPA==0 && !( (iy>=1&&iy<=6&&(ipt==0||ipt==1)) || (iy>=2&&iy<=5&&(ipt==2||ipt==3)) ) ) { continue;}
			if ( isPA==1 && !( (iy>=1&&iy<=6&&(ipt==0||ipt==1)) || (iy>=2&&iy<=5&&(ipt==2)) || (iy>=2&&iy<=4&&ipt==3) ) ) { continue;}
      if (MrapNpt==62 && isPA==1 && !( (iy>=1 && iy<=4&&(ipt==0)) ) ) {continue; } 
			hTotalPR->SetBinContent(iy+1, ipt+1, 0);
			hTotalNP->SetBinContent(iy+1, ipt+1, 0);
		}
	}	
	
  //// set unsed values as zero		
	for (int iy=0; iy<nRap;iy++ ){
		for (int ipt=0; ipt<nPt;ipt++ ){
			if ( isPA==0 && !( (iy>=1&&iy<=6&&(ipt==0||ipt==1)) || (iy>=2&&iy<=5&&(ipt==2||ipt==3)) ) ) { continue;}
			if ( isPA==1 && !( (iy>=1&&iy<=6&&(ipt==0||ipt==1)) || (iy>=2&&iy<=5&&(ipt==2)) || (iy>=2&&iy<=4&&ipt==3) ) ) { continue;}
      if (MrapNpt==62 && isPA==1 && !( (iy>=1 && iy<=4&&(ipt==0)) ) ) {continue; } 
			hFitPR->SetBinContent(iy+1, ipt+1, 0);
			hFitNP->SetBinContent(iy+1, ipt+1, 0);
			hAccPR->SetBinContent(iy+1, ipt+1, 0);
			hAccNP->SetBinContent(iy+1, ipt+1, 0);
			hEffMCPR->SetBinContent(iy+1, ipt+1, 0);
			hEffMCNP->SetBinContent(iy+1, ipt+1, 0);
			hTNPPR->SetBinContent(iy+1, ipt+1, 0);
			hTNPNP->SetBinContent(iy+1, ipt+1, 0);
			hEffPR->SetBinContent(iy+1, ipt+1, 0);
			hEffNP->SetBinContent(iy+1, ipt+1, 0);
			hTotalPR->SetBinContent(iy+1, ipt+1, 0);
			hTotalNP->SetBinContent(iy+1, ipt+1, 0);
		}
	}	
	
  ////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("TotSys_%s_etOpt%d.root",szFinal.Data(),etOpt),"RECREATE");
	cout << "TotSys_"<<szFinal.Data()<<".root has been created :) " <<endl;	

	outFile->cd();
	hFitPR->Write();
	hAccPR->Write();
	hEffMCPR->Write();
	hTNPPR->Write();
	hEffPR->Write();
	hTotalPR->Write();
	hFitNP->Write();
	hAccNP->Write();
	hEffMCNP->Write();
	hTNPNP->Write();
	hEffNP->Write();
	hTotalNP->Write();
	
  outFile->Close();

	return 0;

} // end of main func

