#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void draw_1D_acceff(TString szBinning="8rap9pt", int isPA=1, bool isLog=false, bool isNoErr=true)
{
	gROOT->Macro("../Style.C");

  //double ratiomin = 0.5;
  //double ratiomax = 2.0;
  double ratiomin = 0.85;
  double ratiomax = 1.5;
  
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0, or 1"<< endl; return; }
  TString szPrompt;
//  if (isPrompt) szPrompt = "PR";
//  else szPrompt = "NP"; 
  
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
	TFile * f2D_01;
  if (isPA==0) {
    f2D_01 = new TFile(Form("../FittingResult/totalHist_%s_%s_newcut_nominal_Zvtx0_SF1.root",szPA.Data(),szBinning.Data()));
  } else {
    f2D_01 = new TFile(Form("../FittingResult/totalHist_%s_%s_newcut_nominal_Zvtx1_SF1.root",szPA.Data(),szBinning.Data()));
  }

	// --- read-in 2D hist for data reco dist
	TH2D* h2D_PR_acc = (TH2D*)f2D_01->Get(Form("h2D_Acc_PR_%s",szPA.Data()));
	TH2D* h2D_PR_eff = (TH2D*)f2D_01->Get(Form("h2D_Eff_PR_%s",szPA.Data()));
	TH2D* h2D_NP_acc = (TH2D*)f2D_01->Get(Form("h2D_Acc_NP_%s",szPA.Data()));
	TH2D* h2D_NP_eff = (TH2D*)f2D_01->Get(Form("h2D_Eff_NP_%s",szPA.Data()));
	cout << "h2D_PR_acc : " << h2D_PR_acc->GetName() << endl;
	cout << "h2D_PR_eff : " << h2D_PR_eff->GetName() << endl;
	cout << "h2D_NP_acc : " << h2D_NP_acc->GetName() << endl;
	cout << "h2D_NP_eff : " << h2D_NP_eff->GetName() << endl;

	const int nbinsX = h2D_PR_acc->GetNbinsX();
	const int nbinsY = h2D_PR_acc->GetNbinsY();
	cout << "nbinsX = " << nbinsX << ", nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
	// ---  projection to 1D hist
	TH1D* h1D_PR_acc[nbinsX]; 
	TH1D* h1D_PR_eff[nbinsX]; 
	TH1D* h1D_NP_acc[nbinsX]; 
	TH1D* h1D_NP_eff[nbinsX]; 
	//// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
    if (isPA==0){
      h1D_PR_acc[iy] = h2D_PR_acc->ProjectionY(Form("h1D_PR_acc_%d",iy),nbinsX-iy,nbinsX-iy);
  		h1D_PR_eff[iy] = h2D_PR_eff->ProjectionY(Form("h1D_PR_eff_%d",iy),nbinsX-iy,nbinsX-iy);
      h1D_NP_acc[iy] = h2D_NP_acc->ProjectionY(Form("h1D_NP_acc_%d",iy),nbinsX-iy,nbinsX-iy);
  		h1D_NP_eff[iy] = h2D_NP_eff->ProjectionY(Form("h1D_NP_eff_%d",iy),nbinsX-iy,nbinsX-iy);
		}
    else {
      h1D_PR_acc[iy] = h2D_PR_acc->ProjectionY(Form("h1D_PR_acc_%d",iy),iy+1,iy+1);
  		h1D_PR_eff[iy] = h2D_PR_eff->ProjectionY(Form("h1D_PR_eff_%d",iy),iy+1,iy+1);
      h1D_NP_acc[iy] = h2D_NP_acc->ProjectionY(Form("h1D_NP_acc_%d",iy),iy+1,iy+1);
  		h1D_NP_eff[iy] = h2D_NP_eff->ProjectionY(Form("h1D_NP_eff_%d",iy),iy+1,iy+1);
    }
    //// Acc*Eff
    h1D_PR_acc[iy]->Multiply(h1D_PR_eff[iy]);
    h1D_NP_acc[iy]->Multiply(h1D_NP_eff[iy]);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	//// --- set values as zero for unused bins
	//// 8rap9pt pA
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_PR_acc[iy]->SetBinContent(1,-532);
			h1D_PR_acc[iy]->SetBinError(1,0);
			h1D_NP_acc[iy]->SetBinContent(1,+532);
			h1D_NP_acc[iy]->SetBinError(1,0);
			h1D_PR_acc[iy]->SetBinContent(2,-532);
			h1D_PR_acc[iy]->SetBinError(2,0);
			h1D_NP_acc[iy]->SetBinContent(2,+532);
			h1D_NP_acc[iy]->SetBinError(2,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_PR_acc[iy]->SetBinContent(3,-532);
			h1D_PR_acc[iy]->SetBinError(3,0);
			h1D_NP_acc[iy]->SetBinContent(3,+532);
			h1D_NP_acc[iy]->SetBinError(3,0);
		}
    if (isPA ==0){ //for_pp 
      if (iy>=2 && iy<=5) { 
			  h1D_PR_acc[iy]->SetBinContent(4,-532);
			  h1D_PR_acc[iy]->SetBinError(4,0);
			  h1D_NP_acc[iy]->SetBinContent(4,+532);
			  h1D_NP_acc[iy]->SetBinError(4,0);
		  }
    }
    else {
      if (iy>=2 && iy<=4) { // for_pA
			  h1D_PR_acc[iy]->SetBinContent(4,-532);
			  h1D_PR_acc[iy]->SetBinError(4,0);
			  h1D_NP_acc[iy]->SetBinContent(4,+532);
			  h1D_NP_acc[iy]->SetBinError(4,0);
		  }
		}
	}

/*	
  ////// after zero-bin setting, normalize!
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_PR_acc[iy]->Scale(1./h1D_PR_acc[iy]->Integral());
		h1D_PR_acc[iy]->Scale(1,"width");
		h1D_NP_acc[iy]->Scale(1./h1D_NP_acc[iy]->Integral());
		h1D_NP_acc[iy]->Scale(1,"width");
	}
*/
 
	//////////////////////////////////////////////////////////////////
	//// Draw plots
	//////////////////////////////////////////////////////////////////

	TLegend *legBR = new TLegend(0.55, 0.28, 0.86, 0.40);
	SetLegendStyle(legBR);
	 	
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
		g_01[iy]=new TGraphAsymmErrors(h1D_PR_acc[iy]);
		g_01[iy]->SetName(Form("g_01_%d",iy));
		g_02[iy]=new TGraphAsymmErrors(h1D_NP_acc[iy]);
		g_02[iy]->SetName(Form("g_02_%d",iy));
		
    c01->cd(iy+1);
		if (isLog) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetGraphStyle(g_01[iy],3,0);
		SetGraphStyle(g_02[iy],4,10);
		
		g_01[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		g_01[iy]->GetXaxis()->CenterTitle();
		g_01[iy]->GetXaxis()->SetLimits(0.0,30.0);
		g_01[iy]->GetYaxis()->SetTitle("Efficiency");
		g_01[iy]->GetYaxis()->SetRangeUser(0,0.7);
		
		g_01[iy]->Draw("AP");
		g_02[iy]->Draw("P");
		if (iy==0) {
			legBR -> AddEntry(g_01[iy],"prompt J/#psi","lp");
			legBR -> AddEntry(g_02[iy],"non-prompt J/#psi","lp");
			legBR->Draw();
//			if (isPrompt) { latex->DrawLatex(0.19,0.23,Form("%s Prompt J/#psi",szPA.Data())); }
//			else { latex->DrawLatex(0.19,0.23,Form("%s Non-prompt J/#psi",szPA.Data())); }
		}
		latex->DrawLatex(0.55,0.23,Form("%s",rapArr[iy].Data()));
	}
	c01->Modified();
	c01->Update();
	c01->SaveAs(Form("dir_1D_%s/acceff_%s.pdf",szBinning.Data(),szPA.Data()));

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

