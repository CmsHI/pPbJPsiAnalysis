#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void draw_1D_TotSys(int MrapNpt=89, int isPA=1, bool isPrompt=false, double ymax=100, int etOpt = 0)
{
	gROOT->Macro("../Style.C");

  double ymin = 0.0;
  //ymax = 100; //max
  
  const int nOpt = 3; //hFit, hAcc, hEff
	cout << "nOpt = " << nOpt << endl;
  
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return; }
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0 or 1"<< endl; return; }
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
  const TString szFinal = Form("%s_%s_%s",szBinning.Data(),szPA.Data(),szPrompt.Data());
  std::cout << "szFinal: " << szFinal << std::endl;
  
  if (isPA==0 && MrapNpt==62) { cout << "ERORR: no 6rap2pt for pp!!!" <<endl; return; };
  ///////////////////////////////////////////////////
  //////// Definition of binning
  int nRapInt, nPtInt;
  if (MrapNpt==89) { nRapInt = 8; nPtInt = 9; }
  else if (MrapNpt==83) { nRapInt = 8; nPtInt = 3; }
  else if (MrapNpt==63) { nRapInt = 6; nPtInt = 3;}
  else { nRapInt = 6; nPtInt = 2;}
  const int nRapTmp = nRapInt +1;
  const int nPtTmp = nPtInt +1; 
  //// pt bins
  Double_t ptArrNum[nPtTmp];
  Double_t ptArrNum_9pt[10] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0};
  Double_t ptArrNum_2pt[3] = {5.0, 6.5, 30.0};
  for (int ipt=0; ipt<nPtTmp; ipt++){
    if (MrapNpt==89) {ptArrNum[ipt]=ptArrNum_9pt[ipt];}
    else {ptArrNum[ipt]=ptArrNum_2pt[ipt];}
  } 
  ////rap array in yCM (from forward to backward !!!!)
  Double_t rapArrNumFB[nRapTmp];
  Double_t rapArrNumFB_8rap_pp[9] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};// for pt dist.
  Double_t rapArrNumFB_8rap_pA[9] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  Double_t rapArrNumFB_6rap_pA[9] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93};// for pt dist.
  
  for (int iy=0; iy<nRapTmp; iy++){
    if (isPA==0) { 
      rapArrNumFB[iy]=rapArrNumFB_8rap_pp[iy];
    }
    else {
      if (MrapNpt==89) { rapArrNumFB[iy]=rapArrNumFB_8rap_pA[iy]; }
      else { rapArrNumFB[iy]=rapArrNumFB_6rap_pA[iy]; }
    }
  } 
  
  const Int_t nRap = sizeof(rapArrNumFB)/sizeof(double)-1;
  const Int_t nPt = sizeof(ptArrNum)/sizeof(double)-1;
  cout << "nRap = " << nRap << endl;
  cout << "nPt = " << nPt << endl;
 
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
	//TString rapArr[nRap];
	TString *rapArr = new TString[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
		formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
		cout << iy <<"th rapArr = " << rapArr[iy] << endl;
	}
	//TString ptArr[nPt];
	TString *ptArr = new TString[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
		cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
	}
  
  //////////////////////////////////////////////////////////////////////////////
	// --- read-in file
	TFile * f2D = new TFile(Form("../TotalSys/TotSys_%s_%s_etOpt%d.root",szBinning.Data(),szPA.Data(),etOpt));
  
	// --- read-in 2D hist for data reco dist
	TH2D* h2D_tot = (TH2D*)f2D->Get(Form("hTotal%s",szPrompt.Data()));
	  cout << " * h2D_tot : " << h2D_tot << endl;
  TH2D* h2D_maxerr[nOpt];
	for (int iopt=0; iopt<nOpt; iopt++){
	  if (iopt==0) h2D_maxerr[iopt] = (TH2D*)f2D->Get(Form("hFit%s",szPrompt.Data()));
	  else if (iopt==1) h2D_maxerr[iopt] = (TH2D*)f2D->Get(Form("hAcc%s",szPrompt.Data()));
	  else h2D_maxerr[iopt] = (TH2D*)f2D->Get(Form("hEff%s",szPrompt.Data()));
	  //cout << " * h2D_maxerr : " << h2D_maxerr[iopt]->GetName() << endl;
	  cout << " * h2D_maxerr : " << h2D_maxerr[iopt] << endl;
  }

	const int nbinsX = h2D_tot->GetNbinsX();
	const int nbinsY = h2D_tot->GetNbinsY();
	cout << "nbinsX = " << nbinsX << ", nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
  // ---  projection to 1D hist
	TH1D* h1D_tot[nRap]; 
	TH1D* h1D_maxerr[nRap][nOpt]; 
	//// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nRap; iy++) {
    if (isPA==0){ h1D_tot[iy] = h2D_tot->ProjectionY(Form("h1D_tot_%d",iy),nRap-iy,nRap-iy);}
    else { h1D_tot[iy] = h2D_tot->ProjectionY(Form("h1D_tot_%d",iy),iy+1,iy+1);}
	  for (int iopt=0; iopt<nOpt; iopt++){
      if (isPA==0){ h1D_maxerr[iy][iopt] = h2D_maxerr[iopt]->ProjectionY(Form("h1D_maxerr_%d_%d",iy,iopt+1),nRap-iy,nRap-iy);}
      else { h1D_maxerr[iy][iopt] = h2D_maxerr[iopt]->ProjectionY(Form("h1D_maxerr_%d_%d",iy,iopt+1),iy+1,iy+1);}
  	}
  }
 
  //// (isNoErr) remove error & make positive & multiply 100 (%)  
  double tmpval;
	for (Int_t iy = 0; iy < nRap; iy++) {
      for (int ipt=0; ipt<nPt; ipt++) {
	      tmpval=h1D_tot[iy]->GetBinContent(ipt+1);
        if (tmpval <0) tmpval = -1*tmpval;
        h1D_tot[iy]->SetBinContent(ipt+1,tmpval*100.);
        h1D_tot[iy]->SetBinError(ipt+1,0);
	      for (int iopt=0; iopt<nOpt; iopt++){
	        tmpval=h1D_maxerr[iy][iopt]->GetBinContent(ipt+1);
          if (tmpval <0) tmpval = -1*tmpval;
	        h1D_maxerr[iy][iopt]->SetBinContent(ipt+1,tmpval*100.);
	        h1D_maxerr[iy][iopt]->SetBinError(ipt+1,0);
        }
      }
    }
	
	//////////////////////////////////////////////////////////////////
	//// Draw plots
	//////////////////////////////////////////////////////////////////

	TLegend *legUR = new TLegend(0.70, 0.64, 0.98, 0.84);
	SetLegendStyle(legUR);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);
/*
  //// TGraph
  TCanvas* c01;
  if (nRap==8) {
    c01 = new TCanvas("c01","c01",200,10,1600,800);
  	c01->Divide(4,2);
  } else if (nRap==6) {
    c01 = new TCanvas("c01","c01",200,10,1200,800);
  	c01->Divide(3,2);
  }
   
  TGraphAsymmErrors* g_tot[nRap];
  TGraphAsymmErrors* g_maxerr[nRap][nOpt];
	
  for (Int_t iy = 0; iy < nRap; iy++) {
    c01->cd(iy+1);
		//if (isLog) gPad->SetLogy(1);
		//else gPad->SetLogy(0);
	  
		g_tot[iy]=new TGraphAsymmErrors(h1D_tot[iy]);
		g_tot[iy]->SetName(Form("g_tot_%d",iy));
    SetGraphStyle2(g_tot[iy],0,0);
		g_tot[iy]->SetMarkerSize(0.);
    g_tot[iy]->SetLineWidth(2);
    g_tot[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		g_tot[iy]->GetXaxis()->CenterTitle();
		g_tot[iy]->GetXaxis()->SetLimits(0.0,30.0);
		g_tot[iy]->GetYaxis()->SetTitle("relative Systematic Error (%)");
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
			else { latex->DrawLatex(0.19,0.88,Form("%s Nonprompt J/#psi",szPA.Data())); }
		}
		latex->DrawLatex(0.55,0.88,Form("%s",rapArr[iy].Data()));
	}
	c01->Modified();
	c01->Update();
	c01->SaveAs(Form("dir_fitSys/%s_fitSys_total_%s_etOpt%d_%s_upto%.0f.pdf",szPA.Data(),szBinning.Data(),etOpt,szPrompt.Data(),ymax));
*/	
  //// hist
  TCanvas* c02;
  if (nRap==8) {
    c02 = new TCanvas("c02","c02",200,10,1600,800);
  	c02->Divide(4,2);
  } else if (nRap==6) {
    c02 = new TCanvas("c02","c02",200,10,1200,800);
  	c02->Divide(3,2);
  }
  
  for (Int_t iy = 0; iy < nRap; iy++) {
    c02->cd(iy+1);
    for (int iopt=0; iopt<nOpt; iopt++){
		  SetHistStyle2(h1D_maxerr[iy][iopt],iopt+3,0);
		  //h1D_maxerr[iy][iopt]->SetMarkerSize(0.);
		  h1D_maxerr[iy][iopt]->SetLineWidth(2);
		  if (iopt==0){
        h1D_maxerr[iy][iopt]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		    h1D_maxerr[iy][iopt]->GetXaxis()->CenterTitle();
		    h1D_maxerr[iy][iopt]->GetXaxis()->SetLimits(0.0,30.0);
		    h1D_maxerr[iy][iopt]->GetYaxis()->SetTitle("relative Systematic Error (%)");
		    h1D_maxerr[iy][iopt]->GetYaxis()->SetRangeUser(ymin,ymax);
        h1D_maxerr[iy][iopt]->Draw("hist");
	    }	
      h1D_maxerr[iy][iopt]->Draw("hist same");
	  }	
    SetHistStyle2(h1D_tot[iy],0,0);
		//h1D_tot[iy]->SetMarkerSize(0.);
    h1D_tot[iy]->SetLineWidth(2);
    //h1D_tot[iy]->SetLineStyle(7);
		h1D_tot[iy]->Draw("hist same");
		
		if (iy==0) {
			if (isPrompt) { latex->DrawLatex(0.19,0.88,Form("%s Prompt J/#psi",szPA.Data())); }
			else { latex->DrawLatex(0.19,0.88,Form("%s Nonprompt J/#psi",szPA.Data())); }
		}
    if ((MrapNpt==89 && iy==3) || (MrapNpt==62 && iy==2) ) {
			legUR -> AddEntry(h1D_tot[iy],"total","l");
			legUR -> AddEntry(h1D_maxerr[iy][0],"Fit","l");
			legUR -> AddEntry(h1D_maxerr[iy][1],"Acc.","l");
			legUR -> AddEntry(h1D_maxerr[iy][2],"Eff.","l");
			legUR->Draw();
		}
		latex->DrawLatex(0.55,0.88,Form("%s",rapArr[iy].Data()));
	}
	c02->Modified();
	c02->Update();
	c02->SaveAs(Form("dir_TotSys/TotSys_%s_etOpt%d_upto%.0f.pdf",szFinal.Data(),etOpt,ymax));

	return;

} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
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

void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr) {
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

void formPtArr(Double_t binmin, Double_t binmax, TString* arr) {
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

