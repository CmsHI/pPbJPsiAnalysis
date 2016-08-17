#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void draw_1D_parameters(TString szBinning="8rap9pt", int isPA=1, bool isPrompt=true, bool isLog=false, TString szParam="coefExp")
{
	gROOT->Macro("../Style.C");

  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0, or 1"<< endl; return; }
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
  double binmin, binmax;
  if (strcmp(szParam,"coefExp")==0) {binmin=-3.0; binmax=3.0;}
  else if (strcmp(szParam,"coefPol")==0) {binmin=-0.3; binmax=1.;}
  else if (strcmp(szParam,"fracG1")==0) {binmin=0.0; binmax=1.5;}
  else if (strcmp(szParam,"meanSig")==0) {binmin=3.08; binmax=3.11;}
  else if (strcmp(szParam,"sigmaSig1")==0) {binmin=0.00; binmax=0.14;}
  else if (strcmp(szParam,"sigmaSig2")==0) {binmin=0.00; binmax=0.14;}
  else if (strcmp(szParam,"sigWidth")==0) {binmin=0.00; binmax=0.11;}
  else if (strcmp(szParam,"alpha")==0) {binmin=0.5; binmax=4.5;} //need to be tuned
  else if (strcmp(szParam,"enne")==0) {binmin=1.5; binmax=3.0;}
  else if (strcmp(szParam,"enne")==0) {binmin=1.5; binmax=3.0;}
  else if (strcmp(szParam,"sigmaNPTrue")==0) {binmin=-0.000005; binmax=0.00006;} // need to be tuned
  else if (strcmp(szParam,"coefExpNPTrue")==0) {binmin=0.0; binmax=1.;}
  else if (strcmp(szParam,"fracRes")==0) {binmin=0.0; binmax=1.;}
  //else if (strcmp(szParam,"fracRes")==0) {binmin=0.0; binmax=1.5;}
  else if (strcmp(szParam,"meanPRResW")==0) {binmin=-0.02; binmax=0.04;} //need to be tuned..
//  else if (strcmp(szParam,"meanPRResW")==0) {binmin=-0.5; binmax=0.5;} //need to be tuned..
  else if (strcmp(szParam,"meanPRResN")==0) {binmin=-0.01; binmax=0.020;} 
  else if (strcmp(szParam,"sigmaPRResW")==0) {binmin=0.; binmax=6.5;} //need to be tuned..
  //else if (strcmp(szParam,"sigmaPRResW")==0) {binmin=1.; binmax=15;} //need to be tuned..
  else if (strcmp(szParam,"sigmaPRResN")==0) {binmin=0.5; binmax=1.5;} 
  else if (strcmp(szParam,"fracCtBkg1")==0) {binmin=0.00; binmax=2.00;} 
  else if (strcmp(szParam,"fracCtBkg2")==0) {binmin=0.00; binmax=2.00;} 
  else if (strcmp(szParam,"fracCtBkg3")==0) {binmin=0.00; binmax=2.00;} 
  else if (strcmp(szParam,"lambdam")==0) {binmin=-0.05; binmax=0.3;}  //need to be tuned
  else if (strcmp(szParam,"lambdap")==0) {binmin=0.05; binmax=1.5;} 
  else if (strcmp(szParam,"lambdasym")==0) {binmin=-0.05; binmax=1.5;}  //need to be tuned
  //else if (strcmp(szParam,"lambdasym")==0) {binmin=0.00; binmax=0.5;}  //need to be tuned

  else if (strcmp(szParam,"NLL")==0) {binmin=-60000; binmax=0;}  //need to be tuned
  else if (strcmp(szParam,"Resolution")==0) {binmin=0.5; binmax=2.0;}  //need to be tuned
  
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
	
  //// array TString
	TString rapArr[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
		cout << iy <<"th rapArr = " << rapArr[iy] << endl;
	}
	TString ptArr[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
		cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
	}

	// --- read-in file
	TFile * f2D_01 = new TFile(Form("../FittingResult/fitResHist_%s_%s_newcut_nominal_etOpt0.root",szBinning.Data(),szPA.Data()));
	//TFile * f2D_01 = new TFile(Form("../FittingResult/fitResHist_%s_%s_newcut_sys04_01.root",szBinning.Data(),szPA.Data()));

	// --- read-in 2D hist for data reco dist
	TH2D* h2D_01 = (TH2D*)f2D_01->Get(Form("otherParam/h2D_%s",szParam.Data()));
	cout << "h2D_01 : " << h2D_01->GetName() << endl;

	const int nbinsX = h2D_01->GetNbinsX();
	const int nbinsY = h2D_01->GetNbinsY();
	cout << "nbinsX = " << nbinsX << ", nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
	// ---  projection to 1D hist
	TH1D* h1D_01[nbinsX]; 
	//// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
    if (isPA==0){
      h1D_01[iy] = h2D_01->ProjectionY(Form("h1D_01_%d",iy),nbinsX-iy,nbinsX-iy);
		}
    else {
      h1D_01[iy] = h2D_01->ProjectionY(Form("h1D_01_%d",iy),iy+1,iy+1);
    }
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	//// --- set values as zero for unused bins
	//// 8rap9pt pA
	for (Int_t iy = 0; iy < nbinsX; iy++) {
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
	//// Draw plots
	//////////////////////////////////////////////////////////////////

	//TLegend *legUR = new TLegend(0.14, 0.73, 0.93, 0.93);
	TLegend *legUR = new TLegend(0.14, 0.70, 0.93, 0.95);
	SetLegendStyle(legUR);
  legUR-> SetNColumns(2);
  legUR->SetTextSize(0.038);
   	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	TCanvas* c01 = new TCanvas("c01","c01",200,10,1600,800);
	c01->Divide(4,2);
  
  TGraphAsymmErrors* g_01[nbinsX];
  TGraphAsymmErrors* g_02[nbinsX];

	for (Int_t iy = 0; iy < nbinsX; iy++) {
		g_01[iy]=new TGraphAsymmErrors(h1D_01[iy]);
		g_01[iy]->SetName(Form("g_01_%d",iy));
		
    c01->cd(iy+1);
		if (isLog) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		//SetGraphStyle(g_01[iy],3,0);
		//SetGraphStyle(g_01[iy],iy,iy+10);
		SetGraphStyle2(g_01[iy],iy,iy+10);
		
		g_01[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		g_01[iy]->GetXaxis()->CenterTitle();
		g_01[iy]->GetXaxis()->SetLimits(0.0,30.0);
		g_01[iy]->GetYaxis()->SetTitle(szParam.Data());
		g_01[iy]->GetYaxis()->CenterTitle();
		g_01[iy]->GetYaxis()->SetRangeUser(binmin,binmax);
		
		g_01[iy]->Draw("AP");
//		if (iy==0) {
//			legBR -> AddEntry(g_01[iy],"SF","lp");
//			legBR->Draw();
//			if (isPrompt) { latex->DrawLatex(0.19,0.23,Form("%s Prompt J/#psi",szPA.Data())); }
//			else { latex->DrawLatex(0.19,0.23,Form("%s Non-prompt J/#psi",szPA.Data())); }
//		}
		//latex->DrawLatex(0.55,0.23,Form("%s",rapArr[iy].Data()));
		latex->DrawLatex(0.55,0.83,Form("%s",rapArr[iy].Data()));
	}
	c01->Modified();
	c01->Update();
	c01->SaveAs(Form("dir_1D_param/%s_%s_%s_wide.pdf",szPA.Data(),szBinning.Data(),szParam.Data()));
	
  //// in a single TCanvas
  TCanvas* c02 = new TCanvas("c02","c02",200,10,600,600);
  c02->cd();
	legUR->SetHeader(Form("  %s",szPA.Data()));
  for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy==0) { g_01[iy]->Draw("AP"); }
    else { g_01[iy]->Draw("P"); }
		legUR -> AddEntry(g_01[iy],rapArr[iy].Data(),"lp");
	}
	legUR->Draw();
	if (strcmp(szParam,"meanSig")==0) { solidLine(0.,3.096916,30.,3.096916,2,1); }
  c02->SaveAs(Form("dir_1D_param/%s_%s_%s.pdf",szPA.Data(),szBinning.Data(),szParam.Data()));


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

