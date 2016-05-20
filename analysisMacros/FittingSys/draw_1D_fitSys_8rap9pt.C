#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

void draw_1D_fitSys_8rap9pt(TString szBinning="8rap9pt", int isPA=0, bool isPrompt=false, bool isLog=false, bool isNoErr=true)
{
	gROOT->Macro("../Style.C");

  double ymin = 0.0;
  //double ymax = 1.0; //max
  double ymax = 0.1; //10 %
  
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0 or 1"<< endl; return; }
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
  ////rap array in yCM (from forward to backward)
  const Int_t nRap = 8; 
  const Int_t nPt = 9; 
  const int nRapTmp = nRap +1; 
  const int nPtTmp = nPt +1; 
  
  Double_t rapArrNumFB[nRapTmp];
  Double_t rapArrNumFB_pp[nRapTmp] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};// for pt dist.
  Double_t rapArrNumFB_pA[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  //Double_t rapArrNumBF[nRapTmp];
  //Double_t rapArrNumBF_pp[nRapTmp] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};// for rap dist.
  //Double_t rapArrNumBF_pA[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
  for (int iy=0; iy<nRapTmp; iy++){
    if (isPA==0) { rapArrNumFB[iy]=rapArrNumFB_pp[iy];}
    else { rapArrNumFB[iy]=rapArrNumFB_pA[iy];}
  } 
  
  ////pt array
  Double_t ptArrNum[10] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
 
  //// bin width 
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
	}
	Double_t ptBinW[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
	}
	
  //// array string
	string rapArr[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
		cout << iy <<"th rapArr = " << rapArr[iy] << endl;
	}
	string ptArr[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
		cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
	}
  
  //// sys bins
	const int nSubOptTmp = 10;
  const int nOpt = 4;
	TString szSysOpt[nOpt] = {"sys01","sys02","sys03","sys04"};
	const int nSubOpt[nOpt] = {5,1,2,1};
	//const int nOpt = sizeof(szSysOpt)/sizeof(TString);
	cout << "nOpt = " << nOpt << endl;

  //////////////////////////////////////////////////////////////////////////////
	// --- read-in file
	TFile * f2D = new TFile(Form("../FittingSys/fitSysErr_%s_%s_newcut.root",szBinning.Data(),szPA.Data()));
  
	// --- read-in 2D hist for data reco dist
	TH2D* h2D_tot = (TH2D*)f2D->Get(Form("h2D_%s_tot",szPrompt.Data()));
  TH2D* h2D_maxerr[nOpt];
	for (int iopt=0; iopt<nOpt; iopt++){
	  h2D_maxerr[iopt] = (TH2D*)f2D->Get(Form("h2D_%s_maxerr_sys0%d",szPrompt.Data(),iopt+1));
	  cout << " * h2D_maxerr : " << h2D_maxerr[iopt]->GetName() << endl;
  }

	const int nbinsX = h2D_tot->GetNbinsX();
	const int nbinsY = h2D_tot->GetNbinsY();
	cout << "nbinsX = " << nbinsX << ", nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
  // ---  projection to 1D hist
	TH1D* h1D_tot[nbinsX]; 
	TH1D* h1D_maxerr[nbinsX][nOpt]; 
	//// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
    if (isPA==0){ h1D_tot[iy] = h2D_tot->ProjectionY(Form("h1D_tot_%d",iy),nbinsX-iy,nbinsX-iy);}
    else { h1D_tot[iy] = h2D_tot->ProjectionY(Form("h1D_tot_%d",iy),iy+1,iy+1);}
	  for (int iopt=0; iopt<nOpt; iopt++){
      if (isPA==0){ h1D_maxerr[iy][iopt] = h2D_maxerr[iopt]->ProjectionY(Form("h1D_maxerr_%d_sys0%d",iy,iopt+1),nbinsX-iy,nbinsX-iy);}
      else { h1D_maxerr[iy][iopt] = h2D_maxerr[iopt]->ProjectionY(Form("h1D_maxerr_%d_sys0%d",iy,iopt+1),iy+1,iy+1);}
  	}
  }
	
  //////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	//// --- set values as zero for unused bins
	//// 8rap9pt
	for (Int_t iy = 0; iy < nbinsX; iy++) {
	  for (int iopt=0; iopt<nOpt; iopt++){
		  if (iy>=1 && iy<=6) {
			  h1D_tot[iy]->SetBinContent(1,-532);
  			h1D_tot[iy]->SetBinError(1,0);
  			h1D_tot[iy]->SetBinContent(2,-532);
  			h1D_tot[iy]->SetBinError(2,0);
			  h1D_maxerr[iy][iopt]->SetBinContent(1,-532);
  			h1D_maxerr[iy][iopt]->SetBinError(1,0);
  			h1D_maxerr[iy][iopt]->SetBinContent(2,-532);
  			h1D_maxerr[iy][iopt]->SetBinError(2,0);
  		}
  		if (iy>=2 && iy<=5) {
  			h1D_tot[iy]->SetBinContent(3,-532);
  			h1D_tot[iy]->SetBinError(3,0);
  			h1D_maxerr[iy][iopt]->SetBinContent(3,-532);
  			h1D_maxerr[iy][iopt]->SetBinError(3,0);
  		}
      if (isPA ==0){ //for_pp 
        if (iy>=2 && iy<=5) { 
			    h1D_tot[iy]->SetBinContent(4,-532);
			    h1D_tot[iy]->SetBinError(4,0);
			    h1D_maxerr[iy][iopt]->SetBinContent(4,-532);
			    h1D_maxerr[iy][iopt]->SetBinError(4,0);
		    }
      }
      else {
        if (iy>=2 && iy<=4) { // for_pA
			    h1D_tot[iy]->SetBinContent(4,-532);
			    h1D_tot[iy]->SetBinError(4,0);
			    h1D_maxerr[iy][iopt]->SetBinContent(4,-532);
			    h1D_maxerr[iy][iopt]->SetBinError(4,0);
		    }
		  }
		}
	}
    
  if (isNoErr) {
	  for (Int_t iy = 0; iy < nbinsX; iy++) {
      for (int ipt=0; ipt<nbinsY; ipt++) {
	      h1D_tot[iy]->SetBinError(ipt+1,0);
	      for (int iopt=0; iopt<nOpt; iopt++){
	        h1D_maxerr[iy][iopt]->SetBinError(ipt+1,0);
        }
      }
    }
  }
/*	
  ////// after zero-bin setting, normalize!
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_01[iy]->Scale(1./h1D_01[iy]->Integral());
		h1D_01[iy]->Scale(1,"width");
	}
*/
	
	//////////////////////////////////////////////////////////////////
	//// Draw plots
	//////////////////////////////////////////////////////////////////

	TLegend *legUR = new TLegend(0.55, 0.60, 0.86, 0.82);
	SetLegendStyle(legUR);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	TCanvas* c01 = new TCanvas("c01","c01",200,10,1600,800);
	c01->Divide(4,2);
   
  TGraphAsymmErrors* g_tot[nbinsX];
  TGraphAsymmErrors* g_maxerr[nbinsX][nOpt];
	
  for (Int_t iy = 0; iy < nbinsX; iy++) {
    c01->cd(iy+1);
		if (isLog) gPad->SetLogy(1);
		else gPad->SetLogy(0);
	  
		g_tot[iy]=new TGraphAsymmErrors(h1D_tot[iy]);
		g_tot[iy]->SetName(Form("g_tot_%d",iy));
    SetGraphStyle2(g_tot[iy],0,0);
		g_tot[iy]->SetMarkerSize(0.);
		g_tot[iy]->SetLineWidth(2);
    g_tot[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		g_tot[iy]->GetXaxis()->CenterTitle();
		g_tot[iy]->GetXaxis()->SetLimits(0.0,30.0);
		g_tot[iy]->GetYaxis()->SetTitle("rel.Sys.Err");
		g_tot[iy]->GetYaxis()->SetRangeUser(ymin,ymax);
		g_tot[iy]->Draw("AP");
    
    for (int iopt=0; iopt<nOpt; iopt++){
		  g_maxerr[iy][iopt]=new TGraphAsymmErrors(h1D_maxerr[iy][iopt]);
		  g_maxerr[iy][iopt]->SetName(Form("g_maxerr_%d_sys0%d",iy,iopt+1));
		  SetGraphStyle2(g_maxerr[iy][iopt],iopt+3,0);
		  g_maxerr[iy][iopt]->SetMarkerSize(0.);
		  g_maxerr[iy][iopt]->SetLineWidth(2);
		  g_maxerr[iy][iopt]->Draw("P");
	  }	
		
		if (iy==0) {
			legUR -> AddEntry(g_tot[iy],"total","lp");
			legUR -> AddEntry(g_maxerr[iy][0],"syst.1","lp");
			legUR -> AddEntry(g_maxerr[iy][1],"syst.2","lp");
			legUR -> AddEntry(g_maxerr[iy][2],"syst.3","lp");
			legUR -> AddEntry(g_maxerr[iy][3],"syst.4","lp");
			legUR->Draw();
			if (isPrompt) { latex->DrawLatex(0.19,0.88,Form("%s Prompt J/#psi",szPA.Data())); }
			else { latex->DrawLatex(0.19,0.88,Form("%s Non-prompt J/#psi",szPA.Data())); }
		}
		latex->DrawLatex(0.55,0.88,Form("%s",rapArr[iy].c_str()));
	}
	//c01->Modified();
	//c01->Update();
	//c01->SaveAs(Form("dir_1D_%s/eff_isPrompt%d_isLog%d_SF1_SF0_%s.pdf",szBinning.Data(),(int)isPrompt,(int)isLog,szPA.Data()));

	return;

} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < y_{CM} < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < y_{CM} < %.2f", binmin, binmax);
	}
}

void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < |y_{CM}| < %.2f", binmin, binmax);
	}
}

void formPtArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
	}
}

