#include "../SONGKYO.h"

void read_Eff_Pbp_pPb_pt_forExcel(TString szBinning="8rap9pt", int isPbp=2, bool isPrompt=true)
{
	gROOT->Macro("../Style.C");

  TString szPbp;
  if (isPbp==1) szPbp="Pbp";
  else if (isPbp==2) szPbp="pPb";
  else {cout << "select among isPbp = 1 (Pbp) or 2 (pPb)"<< endl; return; }
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
	// --- read-in file
	TFile * f2D_01;
  f2D_01 = new TFile(Form("../FittingResult/totalHist_pA_%s_newcut_nominal_Zvtx1_SF1_etOpt0.root",szBinning.Data()));
	
	// --- read-in 2D hist for data reco dist
	TH2D* h2D_01 = (TH2D*)f2D_01->Get(Form("h2D_Eff_%s_%s",szPrompt.Data(),szPbp.Data()));
	cout << "h2D_01 : " << h2D_01->GetName() << endl;

	const int nRap = h2D_01->GetNbinsX();
	const int nPt = h2D_01->GetNbinsY();
	cout << "nRap = " << nRap << ", nPt = " << nPt << endl;
	
	// ---  projection to 1D hist
	TH1D* h1D_01[nRap]; 
	//// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nRap; iy++) {
    if (isPbp==1){
      h1D_01[iy] = h2D_01->ProjectionY(Form("h1D_01_%d",iy),iy+1,iy+1);
    } else {
      h1D_01[iy] = h2D_01->ProjectionY(Form("h1D_01_%d",iy),nRap-iy,nRap-iy);
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
    if (iy>=2 && iy<=4) { // for_pA
			h1D_01[iy]->SetBinContent(4,-532);
			h1D_01[iy]->SetBinError(4,0);
		}
	}

	//////////////////////////////////////////////////////////////////
	//// print out
	//////////////////////////////////////////////////////////////////
	
  cout << "::: value // stat " << endl;
  for (Int_t iy = 0; iy < nRap; iy++) {
    cout << "::: for excel ::: iy= " << iy << endl;
		for (Int_t ipt=0; ipt<nPt; ipt++ ){
      cout << h1D_01[iy]->GetBinContent(ipt+1) <<"\t"<<h1D_01[iy]->GetBinError(ipt+1) << endl;
    }
  }
	
  return;

} // end of main func.

