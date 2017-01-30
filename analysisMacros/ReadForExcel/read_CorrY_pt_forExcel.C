#include "../SONGKYO.h"

void read_CorrY_pt_forExcel(TString szBinning="8rap9pt", int isPA=1, bool isPrompt=true)
{
	gROOT->Macro("../Style.C");

  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0, or 1"<< endl; return; }
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
	// --- read-in file
	TFile * f2D_01;
  if (isPA==0) {
    f2D_01 = new TFile(Form("../FittingResult/totalHist_%s_%s_newcut_nominal_Zvtx0_SF1_etOpt0.root",szPA.Data(),szBinning.Data()));
  } else {
    f2D_01 = new TFile(Form("../FittingResult/totalHist_%s_%s_newcut_nominal_Zvtx1_SF1_etOpt0.root",szPA.Data(),szBinning.Data()));
  }
	
  TFile * f2D_02;
  f2D_02 = new TFile(Form("../TotalSys/TotSys_%s_%s_etOpt0.root",szBinning.Data(),szPA.Data()));

	// --- read-in 2D hist for data reco dist
	TH2D* h2D_01 = (TH2D*)f2D_01->Get(Form("h2D_CorrY_%s_%s",szPrompt.Data(),szPA.Data()));
	TH2D* h2D_02 = (TH2D*)f2D_02->Get(Form("hTotal%s",szPrompt.Data()));
	cout << "h2D_01 : " << h2D_01->GetName() << endl;
	cout << "h2D_02 : " << h2D_02->GetName() << endl;

	const int nRap = h2D_01->GetNbinsX();
	const int nPt = h2D_01->GetNbinsY();
	cout << "nRap = " << nRap << ", nPt = " << nPt << endl;
	if (nRap != h2D_02->GetNbinsX()) { cout << " *** Error!!! h2D_01 and h2D_02 binnigs different!"; return; };
	if (nPt != h2D_02->GetNbinsY()) { cout << " *** Error!!! h2D_01 and h2D_02 binnigs different!"; return; };
	
	// ---  projection to 1D hist
	TH1D* h1D_01[nRap]; 
	TH1D* h1D_02[nRap]; 
	//// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nRap; iy++) {
    if (isPA==0){
      h1D_01[iy] = h2D_01->ProjectionY(Form("h1D_01_%d",iy),nRap-iy,nRap-iy);
      h1D_02[iy] = h2D_02->ProjectionY(Form("h1D_02_%d",iy),nRap-iy,nRap-iy);
		}
    else {
      h1D_01[iy] = h2D_01->ProjectionY(Form("h1D_01_%d",iy),iy+1,iy+1);
      h1D_02[iy] = h2D_02->ProjectionY(Form("h1D_02_%d",iy),iy+1,iy+1);
    }
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	//// --- set values as zero for unused bins
	//// 8rap9pt
	for (Int_t iy = 0; iy < nRap; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_01[iy]->SetBinContent(1,-532);
			h1D_01[iy]->SetBinError(1,0);
			h1D_01[iy]->SetBinContent(2,-532);
			h1D_01[iy]->SetBinError(2,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_01[iy]->SetBinContent(3,-532);
			h1D_01[iy]->SetBinError(3,0);
		}
    if (isPA ==0){ //for_pp 
      if (iy>=2 && iy<=5) { 
			  h1D_01[iy]->SetBinContent(4,-532);
			  h1D_01[iy]->SetBinError(4,0);
		  }
    }
    else {
      if (iy>=2 && iy<=4) { // for_pA
			  h1D_01[iy]->SetBinContent(4,-532);
			  h1D_01[iy]->SetBinError(4,0);
		  }
		}
	}

	//////////////////////////////////////////////////////////////////
	//// print out
	//////////////////////////////////////////////////////////////////
	
  cout << "::: value // stat // rel syst " << endl;
  for (Int_t iy = 0; iy < nRap; iy++) {
    cout << "::: for excel ::: iy= " << iy << endl;
		for (Int_t ipt=0; ipt<nPt; ipt++ ){
      cout << h1D_01[iy]->GetBinContent(ipt+1) <<"\t"<<h1D_01[iy]->GetBinError(ipt+1) << "\t "<<TMath::Abs(h1D_02[iy]->GetBinContent(ipt+1))<<endl;
    }
  }
	
  return;

} // end of main func.

